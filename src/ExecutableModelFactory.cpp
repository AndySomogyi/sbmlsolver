/*
 * ExecutableModelFactory.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: andy
 */

#include <ExecutableModelFactory.h>
#include <SBMLSolverOptions.h>

#if defined(BUILD_LLVM)
#include "llvm/LLVMModelGenerator.h"
#include "llvm/LLVMCompiler.h"
#endif

#if defined(BUILD_LEGACY_C)
#include "c/rrCModelGenerator.h"
#endif

#if defined(ENABLE_FBC)
#include "fbc/FBCExecutableModel.h"
#endif

#include "rrLogger.h"
#include <string>
#include <algorithm>

#include "testing/CXXExecutableModel.h"
#include "testing/CXXEnzymeExecutableModel.h"

#include <iostream>

// TODO When we have gcc 4.4 as minimal compiler, drop poco and use C++ standard regex
#include <Poco/RegularExpression.h>

using Poco::RegularExpression;



using namespace std;
namespace rr {

/*
static ModelGenerator* createModelGenerator(const string& compiler, const string& tempFolder,
            const string& supportCodeFolder);
*/

/**
 * implement the couple Compiler methods, this will go, here for source compatiblity.
 */

std::string Compiler::getDefaultCompiler()
{
#if defined(BUILD_LLVM)
    return "LLVM";
#else
    #if defined(_WIN32)
        return joinPath("..", "compilers", "tcc", "tcc.exe");
    #else
        // the default compiler on Unix systems is 'cc', the standard enviornment
        // for the default compiler is 'CC'.
        return getenv("CC") ? getenv("CC") : "gcc";
    #endif
#endif
}

Compiler* Compiler::New() {
    return new rrllvm::LLVMCompiler();
}

#ifdef ENABLE_FBC


// NOTE: The regular expressions need to be file scope static as they cause problems when
// created as function scope static.


static const RegularExpression header_re("<\\s*sbml(.*)>.*<.*model",
		RegularExpression::RE_MULTILINE | RegularExpression::RE_DOTALL |
		RegularExpression::RE_UNGREEDY);


static const RegularExpression xmlns_re("xmlns\\s*:\\s*(\\w+)\\s*=\\s*\"\\s*(\\S+)\\s*\"",
		RegularExpression::RE_MULTILINE | RegularExpression::RE_DOTALL |
		RegularExpression::RE_UNGREEDY);

static const char* fbcns = "http://www.sbml.org/sbml/level3/version1/fbc/";

/**
 * Checks if the given string is a SBML model using the FBC extension.
 *
 * This just does a string search as it is much faster than loading into a DOM.
 *
 * Future versions will also accept a DOM object in addition to a string.
 *
 * returns an unsigned 0 means no fbc, 1 that is uses the fbc extension, and 3 means that
 * fbc is required.
 */
static unsigned is_fbc(const std::string& str)
{
    //<?xml version="1.0" encoding="UTF-8"?> <sbml level="3" version="1" xmlns="http://www.sbml.org/sbml/level3/version1/core">

	/* Start of SBML doc looks like:
	 <?xml version="1.0" encoding="UTF-8"?>
         <sbml xmlns="http://www.sbml.org/sbml/level3/version1/core"
             xmlns:fbc="http://www.sbml.org/sbml/level3/version1/fbc/version1"
             xmlns:html="http://www.w3.org/1999/xhtml" level="3" version="1" fbc:required="false">
         <model metaid="meta_MODEL_01186" id="MODEL_01186">
		 ...
	 */
	unsigned result = 0;
	RegularExpression::MatchVec matches;

	if(header_re.match(str, 0, matches) > 1) {
		RegularExpression::Match m = matches[1];
		std::string header = str.substr(m.offset, m.length);

		int offset = 0;
		while(xmlns_re.match(str, offset, matches) >= 3) {
			RegularExpression::Match m = matches[0];
			// full match
			offset = m.offset + m.length;

			// xmlns str
			m = matches[2];
			std::string ns = str.substr(m.offset, m.length);

			cout << ns << "\n";

			// check for the fbc ns str
			if(ns.find(fbcns) != std:: string::npos) {
				// has fbc ns
				result++;

				// TODO, check for fbc:required="false">

				return result;

			}
		}
	}
    return result;
}

#endif


ExecutableModel* rr::ExecutableModelFactory::createModel(
        const std::string& sbml, const Dictionary* dict)
{
    LoadSBMLOptions opt(dict);

    if(opt.hasKey("cxxEnzymeTest")) {
        return new rrtesting::CXXEnzymeExecutableModel(dict);
    }

#ifdef ENABLE_FBC
    unsigned fbcType = is_fbc(sbml);

    if(fbcType > 0) {
    	rr::fbc::FBCExecutableModel *model = new rr::fbc::FBCExecutableModel(sbml, dict);

    	model->writeLP("/Users/andy/rr-test.lp");

    	return model;


    }

#endif

    return rrllvm::LLVMModelGenerator::createModel(sbml, opt.modelGeneratorOpt);
}




} /* namespace rr */


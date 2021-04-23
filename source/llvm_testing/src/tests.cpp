#include "tests.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrSparse.h"
#include "rrLogger.h"


#ifdef _MSC_VER
#pragma warning(disable: 4146)
#pragma warning(disable: 4141)
#pragma warning(disable: 4267)
#pragma warning(disable: 4624)
#endif
#include "llvm/ModelGeneratorContext.h"
#include "llvm/LLVMModelDataSymbols.h"
#include "llvm/ModelDataIRBuilder.h"
#include "llvm/LLVMIncludes.h"
#include "llvm/AssignmentRuleEvaluator.h"
#include "llvm/EvalInitialConditionsCodeGen.h"
#ifdef _MSC_VER
#pragma warning(default: 4146)
#pragma warning(default: 4141)
#pragma warning(default: 4267)
#pragma warning(default: 4624)
#endif


#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/SBMLReader.h>

#include "llvm/LLVMModelData.h"

#include <utility>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "cpplapack.h"


using namespace rr;
using namespace llvm;
using namespace libsbml;
using namespace rrllvm;



std::string getModelFileName(const std::string& version, int caseNumber)
{
    std::string dummy;
    std::string logFileName;
    std::string settingsFileName;

    //Create a log file name
    createTestSuiteFileNameParts(caseNumber, ".log", dummy, logFileName,
            settingsFileName, dummy);

    //Read SBML models.....
    std::string home = getenv("HOME");
    std::string modelFilePath = home + "/src/sbml_test/";

    modelFilePath = joinPath(joinPath(modelFilePath, "cases"), "semantic");
    std::string modelFileName;

    createTestSuiteFileNameParts(caseNumber, "-sbml-" + version + ".xml",
            modelFilePath, modelFileName, settingsFileName, dummy);

    modelFileName = joinPath(modelFilePath, modelFileName);

    return modelFileName;
}

bool runInitialValueAssigmentTest(const std::string& version, int caseNumber)
{
    LLVMModelData *md = 0;
    std::string modelFileName = getModelFileName(version, caseNumber);

    SBMLDocument *doc = readSBMLFromFile(modelFileName.c_str());

    try
    {

        ModelGeneratorContext c(doc, true);

        c.getModelDataSymbols().print();

        createModelData(c.getModelDataSymbols(), 0);

        ExecutionEngine &engine = c.getExecutionEngine();

        EvalInitialConditionsCodeGen iv(c);

        EvalInitialConditionsCodeGen::FunctionPtr pfunc;

		pfunc = (EvalInitialConditionsCodeGen::FunctionPtr) engine.getPointerToFunction(iv.createFunction());

        pfunc(md, 0);

        rrLog(lInfo) << md;

        std::cout << "done with " << modelFileName << std::endl;

    }
    catch(std::exception &e)
    {
        std::cout << "Failure in " << modelFileName.c_str() << ", " << e.what() << "\n";
    }

    delete doc;

    return true;
}

bool runModelDataAccessorTest(const std::string& version, int caseNumber)
{
    LLVMModelData *md;
    std::string modelFileName = getModelFileName(version, caseNumber);

    SBMLDocument *doc = readSBMLFromFile(modelFileName.c_str());

    ModelGeneratorContext c(doc, true);

    md = createModelData(c.getModelDataSymbols(), 0);

    LLVMModelDataIRBuilderTesting builder(c.getModelDataSymbols(), c.getBuilder());

    //builder.test(c.getModule(), c.getBuilder(), c.getExecutionEngine());

    builder.createAccessors(c.getModule());

    ExecutionEngine &engine = c.getExecutionEngine();
    Function *getFunc = engine.FindFunctionNamed("get_size");

    //getFunc->dump();

    // JIT the function, returning a function pointer.


    // Cast it to the right type (takes no arguments, returns a double) so we
    // can call it as a native function.
    int (*pfunc)(LLVMModelData*) = (int (*)(LLVMModelData*))engine.getPointerToFunction(getFunc);

    double value = pfunc(md);

    std::cout << "get_size returned " << value << "\n";



    std::vector<std::string> floatSpeciesIds = c.getModelDataSymbols().getFloatingSpeciesIds();


    for(int i = 0; i < md->numIndCompartments; i++)
    {
        md->compartmentVolumesAlias[i] = 1.5;
    }

    for(int i = 0; i < floatSpeciesIds.size(); i++)
    {
        std::string getName = "get_floatingspecies_conc_" + floatSpeciesIds[i];
        Function *getFunc = engine.FindFunctionNamed(getName.c_str());

        //getFunc->dump();

        // JIT the function, returning a function pointer.


              // Cast it to the right type (takes no arguments, returns a double) so we
              // can call it as a native function.
        double (*pfunc)(LLVMModelData*) = (double (*)(LLVMModelData*))engine.getPointerToFunction(getFunc);

        double value = pfunc(md);

        std::cout << getName << " returned " << value << "\n";

        std::string setName = "set_floatingspecies_conc_" + floatSpeciesIds[i];
        Function *setFunc = engine.FindFunctionNamed(setName.c_str());

        void (*psetfunc)(LLVMModelData*,double) = (void (*)(LLVMModelData*,double))engine.getPointerToFunction(setFunc);

        psetfunc(md, i+1);

    }


    LLVMModelData_free(md);

    delete doc;

    return true;
}


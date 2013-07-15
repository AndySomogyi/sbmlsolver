/*
 * test_compiler.cpp
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#include "test_compiler.h"
#include "rrModelGeneratorFactory.h"
#include "rrModelGenerator.h"
#include "rrCompiler.h"

#include "rrOSSpecifics.h"
#include <iostream>

using namespace std;

namespace rr
{

void test_compiler()
{
    cout << __FUNC__ << "\n";

    ModelGenerator *mg = ModelGeneratorFactory::createModelGenerator("", "", "", "LLVM");

    cout << "temporary directory: " << mg->getTemporaryDirectory() << "\n";

    Compiler *c = mg->getCompiler();

    cout << "compiler: " << c->getCompiler() << "\n";

    cout << "compiler location: " << c->getCompilerLocation() << "\n";

    cout << "support code dir: " << c->getSupportCodeFolder() << "\n";

    delete mg;
}

} /* namespace rr */

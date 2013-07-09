#include "tests.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMModelDataSymbols.h"
#include "rrLLVMModelDataIRBuilder.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrLLVMIncludes.h"
#include "rrLLVMAssignmentRuleEvaluator.h"
#include "rrLLVMInitialValueCodeGen.h"


#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/SBMLReader.h>

#include "rrModelData.h"

#include <utility>
#include <cstdlib>



using namespace std;







using namespace rr;



#include <iostream>
#include <fstream>

using namespace std;
using namespace rr;
using namespace llvm;

using namespace libsbml;

string getModelFileName(const string& version, int caseNumber)
{
    string dummy;
    string logFileName;
    string settingsFileName;

    //Create a log file name
    createTestSuiteFileNameParts(caseNumber, ".log", dummy, logFileName,
            settingsFileName);

    //Read SBML models.....
    string home = getenv("HOME");
    string modelFilePath = home + "/src/sbml_test/";

    modelFilePath = joinPath(joinPath(modelFilePath, "cases"), "semantic");
    string modelFileName;

    createTestSuiteFileNameParts(caseNumber, "-sbml-" + version + ".xml",
            modelFilePath, modelFileName, settingsFileName);

    modelFileName = joinPath(modelFilePath, modelFileName);

    cout << modelFileName << "\n";

    return modelFileName;
}

bool runInitialValueAssigmentTest(const string& version, int caseNumber)
{
    ModelData md = {0};
    string modelFileName = getModelFileName(version, caseNumber);

    SBMLDocument *doc = readSBMLFromFile(modelFileName.c_str());

    try
    {

        LLVMModelGeneratorContext c(doc, true);

        c.getModelDataSymbols().initAllocModelDataBuffers(md);

        ExecutionEngine *engine = c.getExecutionEngine();

        StructType *s = LLVMModelDataIRBuilder::getStructType(c.getModule(),
                c.getExecutionEngine());

        LLVMInitialValueCodeGen iv(c);

        Function *func = (Function*)iv.codeGen();

        // Cast it to the right type (takes no arguments, returns a double) so we
        // can call it as a native function.
        void (*pfunc)(
                ModelData*) = (void (*)(ModelData*))engine->getPointerToFunction(func);

        pfunc(&md);

        for (int i = 0; i < md.numFloatingSpecies; i++)
        {
            cout << md.floatingSpeciesAmounts[i] << ", ";
        }
        cout << "\n";



    }
    catch(std::exception &e)
    {
        cout << "Failure in " << modelFileName.c_str() << ", " << e.what() << "\n";
    }

    delete doc;

    return true;
}

bool runModelDataAccessorTest(const string& version, int caseNumber)
{
    ModelData md;
    string modelFileName = getModelFileName(version, caseNumber);

    SBMLDocument *doc = readSBMLFromFile(modelFileName.c_str());

    LLVMModelGeneratorContext c(doc, true);

    c.getModelDataSymbols().initAllocModelDataBuffers(md);

    md.size = sizeof(ModelData);

    LLVMModelDataIRBuilder builder(c.getModelDataSymbols(), c.getBuilder());

    //builder.test(c.getModule(), c.getBuilder(), c.getExecutionEngine());

    builder.createAccessors(c.getModule());

    ExecutionEngine *engine = c.getExecutionEngine();
    Function *getFunc = engine->FindFunctionNamed("get_size");

    //getFunc->dump();

    // JIT the function, returning a function pointer.


    // Cast it to the right type (takes no arguments, returns a double) so we
    // can call it as a native function.
    int (*pfunc)(ModelData*) = (int (*)(ModelData*))engine->getPointerToFunction(getFunc);

    double value = pfunc(&md);

    cout << "get_size returned " << value << "\n";



    vector<string> floatSpeciesIds = c.getModelDataSymbols().getFloatingSpeciesIds();

    for(int i = 0; i < floatSpeciesIds.size(); i++)
    {
        builder.createFloatingSpeciesAccessors(c.getModule(), floatSpeciesIds[i]);
        md.floatingSpeciesConcentrations[i] = i+1;
    }

    for(int i = 0; i < md.numCompartments; i++)
    {
        md.compartmentVolumes[i] = 1.5;
    }

    for(int i = 0; i < floatSpeciesIds.size(); i++)
    {
        string getName = "get_floatingspecies_conc_" + floatSpeciesIds[i];
        Function *getFunc = engine->FindFunctionNamed(getName.c_str());

        //getFunc->dump();

        // JIT the function, returning a function pointer.


              // Cast it to the right type (takes no arguments, returns a double) so we
              // can call it as a native function.
        double (*pfunc)(ModelData*) = (double (*)(ModelData*))engine->getPointerToFunction(getFunc);

        double value = pfunc(&md);

        cout << getName << " returned " << value << "\n";

        string setName = "set_floatingspecies_conc_" + floatSpeciesIds[i];
        Function *setFunc = engine->FindFunctionNamed(setName.c_str());

        void (*psetfunc)(ModelData*,double) = (void (*)(ModelData*,double))engine->getPointerToFunction(setFunc);

        psetfunc(&md, i+1);

        cout << "amount: " << md.floatingSpeciesAmounts[i] << ", conc: " << md.floatingSpeciesConcentrations[i] << "\n";
    }


    freeModelDataBuffers(md);

    delete doc;

    return true;
}

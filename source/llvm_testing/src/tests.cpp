#include "tests.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMModelDataSymbols.h"
#include "rrLLVMModelDataIRBuilder.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrLLVMIncludes.h"
#include "rrLLVMAssignmentRuleEvaluator.h"
#include "rrLLVMInitialValueCodeGen.h"
#include "rrSparse.h"





#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/SBMLReader.h>

#include "rrModelData.h"

#include <utility>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "cpplapack.h"

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

bool runSparseTest(const int m, const int n, const int nnz)
{
    double *A = (double*)calloc(m*n, sizeof(double));

    double *x = (double*)calloc(n, sizeof(double));

    double *y = (double*)calloc(m, sizeof(double));
    double *ys = (double*)calloc(m, sizeof(double));

    int true_nnz = 0;


    // row major:    offset = row*NUMCOLS + column
    // column major: offset = row + column*NUMROWS


    // set x and first row of A to ones.
    //for (int i = 0; i < n; i++) {
    //    A[0 + i*m] = 1;
    //    x[i] = 1;
    //}

    for (int k = 0; k < nnz; k++) {
        int i = rand() % m;
        int j = rand() % n;

        if (A[i + j*m] == 0) {
            A[i + j*m] = 1;
            true_nnz++;
        }
    }

    for (int k = 0; k < n; k++) {
        x[k] = rand() % 10;
    }

    //cout << "A: ";
    //for(int i = 0; i < m*n; i++) {
    //    cout << A[i] << ",";
    //}
    //cout << "\n";

    cout << "x: ";
    for(int i = 0; i < n; i++) {
        cout << x[i] << ",";
    }
    cout << "\n";


    cpp_dgemv('n', m, n, 1, A, m, x, 1, 0, y, 1);

    cout << "dense y:  ";
    for(int i = 0; i < m; i++) {
        cout << y[i] << ",";
    }
    cout << "\n";


    vector<int> rowidx;
    vector<int> colidx;
    vector<double> values;

    for(int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double value = A[i + j*m];

            if (value != 0)
            {
                rowidx.push_back(i);
                colidx.push_back(j);
                values.push_back(value);
            }
        }
    }

    assert(rowidx.size() == true_nnz);
    assert(colidx.size() == true_nnz);
    assert(values.size() == true_nnz);

    csr_matrix *mat = csr_matrix_new(m, n, rowidx, colidx, values);

    csr_matrix_dgemv(mat, x, ys);

    cout << "sparse y: ";
    for(int i = 0; i < m; i++) {
        cout << ys[i] << ",";
    }
    cout << "\n";

    // change some values
    for(int k = 0; k < true_nnz; k++)
    {
        int i = rowidx[k];
        int j = colidx[k];
        double value = rand() % 10;

        csr_matrix_set_nz(mat, i, j, value);
        A[i + j*m] = value;

        assert(csr_matrix_get_nz(mat, i, j) == value);
    }

    memset(y, 0, m*sizeof(double));
    cpp_dgemv('n', m, n, 1, A, m, x, 1, 0, y, 1);

    cout << "dense y:  ";
    for (int i = 0; i < m; i++)
    {
        cout << y[i] << ",";
    }
    cout << "\n";


    memset(ys, 0, m*sizeof(double));
    csr_matrix_dgemv(mat, x, ys);

    cout << "sparse y: ";
    for (int i = 0; i < m; i++)
    {
        cout << ys[i] << ",";
    }
    cout << "\n";

    csr_matrix_delete(mat);


    free(A);
    free(x);
    free(y);
    free(ys);
}

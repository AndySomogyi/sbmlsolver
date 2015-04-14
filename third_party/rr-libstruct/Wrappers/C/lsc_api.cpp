#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "lsC_API.h"
//---------------------------------------------------------------------------

// load a new stoichiometry matrix and reset current loaded model
/*LIB_EXTERN*/  int LibStructural_loadStoichiometryMatrix ( const double ** inMatrix, const int nRows, const int nCols)
{
    DoubleMatrix oMatrix(inMatrix, nRows, nCols);
    LibStructural::getInstance()->loadStoichiometryMatrix( oMatrix );
    return 0;
}

// load species names and initial values
/*LIB_EXTERN*/  int LibStructural_loadSpecies ( const char** speciesNames, const double* speciesValues, const int nLength)
{
    vector< string > oNames; vector< double> oValues;
    for (int i = 0; i < nLength; i++)
    {
        oNames.push_back(string(speciesNames[i]));
        oValues.push_back(speciesValues[i]);
    }
    LibStructural::getInstance()->loadSpecies(oNames, oValues);
    return 0;
}

// load reaction names
        //            string test = matrixToString(actual);
//            clog<<test;
/*LIB_EXTERN*/  int LibStructural_loadReactionNames ( const char** reactionNames, const int nLength)
{
    vector< string > oNames;
    for (int i = 0; i < nLength; i++)
    {
        oNames.push_back(string(reactionNames[i]));
    }
    LibStructural::getInstance()->loadReactionNames(oNames);
    return 0;
}


        //            string test = matrixToString(actual);
//            clog<<test;
#ifndef NO_SBML

// Initialization method, takes SBML as input
/*LIB_EXTERN*/  int LibStructural_loadSBML(const char* sSBML, char** oResult, int *nLength)
{
    try
    {
        *oResult = strdup(LibStructural::getInstance()->loadSBML(string(sSBML)).c_str());
        *nLength = strlen(*oResult);
        return 0;
    }
    catch (...)
    {
        return -1;
    }
}

/*LIB_EXTERN*/  int LibStructural_loadSBMLFromFile(const char* sFileName, char* *outMessage, int *nLength)
{
    try
    {
        *outMessage = strdup(LibStructural::getInstance()->loadSBMLFromFile(string(sFileName)).c_str());
        *nLength = strlen(*outMessage);
        return 0;
    }
    catch (...)
    {
        return -1;
    }
}


//Initialization method, takes SBML as input
/*LIB_EXTERN*/  int LibStructural_loadSBMLwithTests(const char* sSBML, char* *oResult, int *nLength)
{
    try
    {
        string sbmlString(sSBML);
        string sResult = LibStructural::getInstance()->loadSBMLwithTests(sbmlString);
        (*oResult) = strdup(sResult.c_str());
        (*nLength) = strlen(*oResult);
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

#endif

//Uses QR factorization for Conservation analysis
/*LIB_EXTERN*/  int LibStructural_analyzeWithQR(char* *outMessage, int *nLength)
{
    *outMessage = strdup(LibStructural::getInstance()->analyzeWithQR().c_str());
    *nLength = strlen(*outMessage);
    return 0;
}

//Uses LU Decomposition for Conservation analysis
/*LIB_EXTERN*/  int LibStructural_analyzeWithLU(char* *outMessage, int *nLength)
{
    *outMessage = strdup(LibStructural::getInstance()->analyzeWithLU().c_str());
    *nLength = strlen(*outMessage);
    return 0;
}

//Uses LU Decomposition for Conservation analysis
/*LIB_EXTERN*/  int LibStructural_analyzeWithLUandRunTests(char* *outMessage, int *nLength)
{
    *outMessage = strdup(LibStructural::getInstance()->analyzeWithLUandRunTests().c_str());
    *nLength = strlen(*outMessage);
    return 0;
}

//Uses fully pivoted LU Decomposition for Conservation analysis
/*LIB_EXTERN*/  int LibStructural_analyzeWithFullyPivotedLU(char* *outMessage, int *nLength)
{
    *outMessage = strdup(LibStructural::getInstance()->analyzeWithFullyPivotedLU().c_str());
    *nLength = strlen(*outMessage);
    return 0;
}

//Uses fully pivoted LU Decomposition for Conservation analysis
/*LIB_EXTERN*/  int LibStructural_analyzeWithFullyPivotedLUwithTests(char* *outMessage, int *nLength)
{
    *outMessage = strdup(LibStructural::getInstance()->analyzeWithFullyPivotedLUwithTests().c_str());
    *nLength = strlen(*outMessage);
    return 0;
}

//Returns L0 Matrix
/*LIB_EXTERN*/  int LibStructural_getL0Matrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix *oTemp = LibStructural::getInstance()->getL0Matrix();
    ls::CopyMatrix(*oTemp, *outMatrix, *outRows, *outCols);
    delete oTemp;
    return 0;
}

//Returns Nr Matrix
/*LIB_EXTERN*/  int LibStructural_getNrMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getNrMatrix();
    if (oMatrix == NULL)
        return -1;
        //            string test = matrixToString(actual);
//            clog<<test;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    return 0;
}


//Returns N0 Matrix
/*LIB_EXTERN*/  int LibStructural_getN0Matrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getN0Matrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    return 0;
}

//Returns L, the Link Matrix
/*LIB_EXTERN*/  int LibStructural_getLinkMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getLinkMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    return 0;
}

//Returns K0
/*LIB_EXTERN*/  int LibStructural_getK0Matrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getK0Matrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    return 0;
}

//Returns K Matrix
/*LIB_EXTERN*/  int LibStructural_getKMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getKMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    return 0;
}

////Returns the reordered list of species
///*LIB_EXTERN*/  int LibStructural_getNthReorderedSpeciesId(int n,char* *outMessage, int *nLength)
//{
        //            string test = matrixToString(actual);
//            clog<<test;
//    outMessage = strdup(LibStructural::getInstance()->getReorderedSpecies()[n].c_str());
//    nLength = strlen(outMessage);
//    return nLength;
//}

////Returns the list of independent species
///*LIB_EXTERN*/  int LibStructural_getNthIndependentSpeciesId(int n,char* *outMessage, int *nLength)
//{
//    outMessage = strdup(LibStructural::getInstance()->getIndependentSpecies()[n].c_str());
//    nLength = strlen(outMessage);
//    return nLength;
//}

////Returns the list of dependent species
///*LIB_EXTERN*/  int LibStructural_getNthDependentSpeciesId(int n,char* *outMessage, int *nLength)
//{
//    outMessage = strdup(LibStructural::getInstance()->getDependentSpecies()[n].c_str());
//    nLength = strlen(outMessage);
//    return nLength;
//}

////Returns the list of Reactions
///*LIB_EXTERN*/  int LibStructural_getNthReactionId(int n,char* *outMessage, int *nLength)
//{
//    outMessage = strdup(LibStructural::getInstance()->getReactions()[n].c_str());
//    nLength = strlen(outMessage);
//    return nLength;
//}


//Returns Gamma, the conservation law array
/*LIB_EXTERN*/
int LibStructural_getGammaMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getGammaMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    return 0;
}

/*LIB_EXTERN*/ int LibStructural_getGammaMatrixGJ(double** inMatrix, int numRows, int numCols,
                                          double** *outMatrix, int *outRows, int *outCols)
{
    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
    DoubleMatrix *oResult = LibStructural::getInstance()->getGammaMatrixGJ( oMatrix );

    ls::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;

    return 0;
}

/*LIB_EXTERN*/ int LibStructural_findPositiveGammaMatrix(double** inMatrix, int numRows, int numCols,
                                                     const char** inRowLabels,
                                                     double** *outMatrix, int *outRows, int *outCols,
                                                     char** *outRowLabels, int *outRowCount)
{
    DoubleMatrix oMatrix(inMatrix, numRows, numCols);


    vector< string > rowNames;
    for (int i = 0; i < numRows; i++)
    {
        rowNames.push_back(string(inRowLabels[i]));
    }

    DoubleMatrix *oResult = LibStructural::getInstance()->findPositiveGammaMatrix( oMatrix, rowNames );
    if (oResult == NULL)
        return -1;
    ls::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;
    ls::CopyStringVector(rowNames, *outRowLabels, *outRowCount);

    return 0;
}


////Returns algebraic expressions for conserved cycles
///*LIB_EXTERN*/  int LibStructural_getNthConservedEntity(int n,char* *outMessage, int *nLength)
//{
//    outMessage = strdup(LibStructural::getInstance()->getConservedLaws()[n].c_str());
//    nLength = strlen(outMessage);
//    return nLength;
//}

//Returns values for conserved cycles using Initial conditions
/*LIB_EXTERN*/  int LibStructural_getNumConservedSums()
{
    return (int) LibStructural::getInstance()->getConservedSums().size();
}
///*LIB_EXTERN*/ double LibStructural_getNthConservedSum(int n)
//{
//    return LibStructural::getInstance()->getConservedSums()[n];
//}

//Returns Initial Conditions used in the model
////*LIB_EXTERN*/  int vector< pair <string, double> > LibStructural_getInitialConditions();

//Returns the original stoichiometry matrix
/*LIB_EXTERN*/  int LibStructural_getStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getStoichiometryMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    return 0;
}

//Returns reordered stoichiometry matrix
/*LIB_EXTERN*/  int LibStructural_getReorderedStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getReorderedStoichiometryMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    return 0;
}

//Tests if conservation laws are correct
/*LIB_EXTERN*/  int  LibStructural_validateStructuralMatrices(int* *outResults, int* outLength)
{
    vector< string > oResult = LibStructural::getInstance()->validateStructuralMatrices();

    *outResults = (int*) malloc(sizeof(int)*oResult.size()); memset(*outResults, 0, sizeof(int)*oResult.size());
    *outLength = oResult.size();

    for (int i = 0; i < *outLength; i++)
    {
        (*outResults)[i] = (int) (oResult[i]=="Pass");
    }
    return 0;
}

//Return Details about conservation tests
/*LIB_EXTERN*/  int LibStructural_getTestDetails(char* *outMessage, int *nLength)
{
    *outMessage = strdup(LibStructural::getInstance()->getTestDetails().c_str());
    *nLength = strlen(*outMessage);
    return 0;

}

//Returns the name of the model
/*LIB_EXTERN*/  int LibStructural_getModelName(char* *outMessage, int *nLength)
{
    *outMessage = strdup(LibStructural::getInstance()->getModelName().c_str());
    *nLength = strlen(*outMessage);
    return 0;

}

//Returns the total number of species
/*LIB_EXTERN*/  int LibStructural_getNumSpecies()
{
    return LibStructural::getInstance()->getNumSpecies();
}

//Returns the number of independent species
/*LIB_EXTERN*/  int LibStructural_getNumIndSpecies()
{
    return LibStructural::getInstance()->getNumIndSpecies();
}

//Returns the number of dependent species
/*LIB_EXTERN*/  int LibStructural_getNumDepSpecies()
{
    return LibStructural::getInstance()->getNumDepSpecies();
}

//Returns the total number of reactions
/*LIB_EXTERN*/  int LibStructural_getNumReactions()
{
    return LibStructural::getInstance()->getNumReactions();
}

//Returns the number of independent reactions
/*LIB_EXTERN*/  int LibStructural_getNumIndReactions()
{
    return LibStructural::getInstance()->getNumIndReactions();
}
//Returns the number of dependent reactions
/*LIB_EXTERN*/  int LibStructural_getNumDepReactions()
{
    return LibStructural::getInstance()->getNumDepReactions();
}



//Returns rank of stoichiometry matrix
/*LIB_EXTERN*/  int LibStructural_getRank()
{
    return LibStructural::getInstance()->getRank();
}

//Returns the number of nonzero values in Stoichiometry matrix
/*LIB_EXTERN*/  double LibStructural_getNmatrixSparsity()
{
    return LibStructural::getInstance()->getNmatrixSparsity();
}

//Set user specified tolerance
/*LIB_EXTERN*/  void LibStructural_setTolerance(double dTolerance)
{
    LibStructural::getInstance()->setTolerance(dTolerance);
}

/*LIB_EXTERN*/ void LibStructural_freeVector(void* vector)
{
    if (vector) free(vector);
}

/*LIB_EXTERN*/ void LibStructural_freeMatrix(void** matrix, int numRows)
{
    for (int i = 0; i < numRows; i++)
    {
        if (matrix[i]) free(matrix[i]);
    }
    free(matrix);
}


/*LIB_EXTERN*/ int LibStructural_getConservedSums(double* *outArray, int *outLength)
{
    vector<double> oSums = LibStructural::getInstance()->getConservedSums();
    ls::CopyDoubleVector(oSums, *outArray, *outLength);
    return 0;

}

/*LIB_EXTERN*/  int LibStructural_getConservedLaws(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getConservedLaws();
    ls::CopyStringVector(oValues, *outArray, *outLength);
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getReactionIds(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getReactions();
    ls::CopyStringVector(oValues, *outArray, *outLength);
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getDependentSpeciesIds(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getDependentSpecies();
    ls::CopyStringVector(oValues, *outArray, *outLength);

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getIndependentSpeciesIds(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getIndependentSpecies();
    ls::CopyStringVector(oValues, *outArray, *outLength);
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getDependentReactionIds(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getDependentReactionIds();
    ls::CopyStringVector(oValues, *outArray, *outLength);
    return 0;
}
/*LIB_EXTERN*/  int LibStructural_getIndependentReactionIds(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getIndependentReactionIds();
    ls::CopyStringVector(oValues, *outArray, *outLength);
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getReorderedReactionIds(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getReorderedReactions();
    ls::CopyStringVector(oValues, *outArray, *outLength);
    return 0;
}


/*LIB_EXTERN*/  int LibStructural_getSpeciesIds(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getSpecies();
    ls::CopyStringVector(oValues, *outArray, *outLength);
    return 0;
}


/*LIB_EXTERN*/  int LibStructural_getReorderedSpeciesIds(char** *outArray, int *outLength)
{
    vector<string> oValues = LibStructural::getInstance()->getReorderedSpecies();
    ls::CopyStringVector(oValues, *outArray, *outLength);
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getInitialConditions(char** *outVariableNames, double* *outValues, int *outLength)
{
    vector< pair < string, double> > oInitialConditions = LibStructural::getInstance()->getInitialConditions();

    *outLength = oInitialConditions.size();

    *outVariableNames = (char**)malloc(sizeof(char*)**outLength); memset(*outVariableNames, 0, sizeof(char*)**outLength);
    *outValues = (double*) malloc(sizeof(double)**outLength); memset(*outValues, 0, sizeof(double)**outLength);

    for (int i = 0; i < *outLength; i++)
    {
        pair<string,double> oTemp = oInitialConditions[i];
        (*outVariableNames)[i] = strdup(oTemp.first.c_str());
        (*outValues)[i] = oTemp.second;
    }

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getL0MatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural_getDependentSpeciesIds(outRowLabels, outRowCount);
    LibStructural_getIndependentSpeciesIds(outColLabels, outColCount);

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getNrMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural_getIndependentSpeciesIds(outRowLabels, outRowCount);
    LibStructural_getReactionIds(outColLabels, outColCount);

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getColumnReorderedNrMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    vector<string> oRows; vector<string> oCols;
    LibStructural::getInstance()->getColumnReorderedNrMatrixLabels(oRows, oCols);

    ls::CopyStringVector(oRows, *outRowLabels, *outRowCount);
    ls::CopyStringVector(oCols, *outColLabels, *outColCount);

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getColumnReorderedNrMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getColumnReorderedNrMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    delete oMatrix;
    return 0;
}

// Returns the NIC Matrix (partition of linearly independent columns of Nr)
/*LIB_EXTERN*/  int LibStructural_getNICMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getNICMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    delete oMatrix;
    return 0;
}

// Returns the NDC Matrix (partition of linearly dependent columns of Nr)
/*LIB_EXTERN*/  int LibStructural_getNDCMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getNDCMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    delete oMatrix;
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getNICMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    vector<string> oRows; vector<string> oCols;
    LibStructural::getInstance()->getNICMatrixLabels(oRows, oCols);

    ls::CopyStringVector(oRows, *outRowLabels, *outRowCount);
    ls::CopyStringVector(oCols, *outColLabels, *outColCount);

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getNDCMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    vector<string> oRows; vector<string> oCols;
    LibStructural::getInstance()->getNDCMatrixLabels(oRows, oCols);

    ls::CopyStringVector(oRows, *outRowLabels, *outRowCount);
    ls::CopyStringVector(oCols, *outColLabels, *outColCount);

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getN0MatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural_getDependentSpeciesIds(outRowLabels, outRowCount);
    LibStructural_getReactionIds(outColLabels, outColCount);
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getLinkMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural_getReorderedSpeciesIds(outRowLabels, outRowCount);
    LibStructural_getIndependentSpeciesIds(outColLabels, outColCount);

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getK0MatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural* instance = LibStructural::getInstance();

    vector<string> oReactionLables = instance->getReorderedReactions();
    DoubleMatrix *k0 = instance->getK0Matrix();


    int nDependent = k0->numCols();
    int nIndependent = k0->numRows();

    *outRowCount = nIndependent;
    *outColCount = nDependent;

    *outRowLabels = (char**) malloc(sizeof(char*)**outRowCount); memset(*outRowLabels, 0, sizeof(char*)**outRowCount);
    *outColLabels = (char**) malloc(sizeof(char*)**outColCount); memset(*outColLabels, 0, sizeof(char*)**outColCount);

    for (int i = 0; i < nDependent; i++)
    {
        (*outColLabels)[i] = strdup(oReactionLables[nIndependent + i].c_str());
    }


    for (int i = 0; i < nIndependent; i++)
    {
        (*outRowLabels)[i] = strdup(oReactionLables[i].c_str());
    }

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getKMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural* instance = LibStructural::getInstance();

    vector<string> oReactionLables = instance->getReorderedReactions();
    DoubleMatrix *k = instance->getKMatrix();


    int nDependent = k->numCols();
    int nIndependent = k->numRows() - nDependent;

    *outRowCount = k->numRows();
    *outColCount = nDependent;

    *outRowLabels = (char**) malloc(sizeof(char*)**outRowCount); memset(*outRowLabels, 0, sizeof(char*)**outRowCount);
    *outColLabels = (char**) malloc(sizeof(char*)**outColCount); memset(*outColLabels, 0, sizeof(char*)**outColCount);



    for (int i = 0; i < nDependent; i++)
    {
        (*outColLabels)[i] = strdup(oReactionLables[nIndependent + i].c_str());
        (*outRowLabels)[i] = strdup(oReactionLables[nIndependent + i].c_str());
    }


    for (int i = 0; i < nIndependent; i++)
    {
        (*outRowLabels)[i+nDependent] = strdup(oReactionLables[i].c_str());
    }

    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getGammaMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural_getReorderedSpeciesIds(outColLabels, outColCount);
    DoubleMatrix *G = LibStructural::getInstance()->getGammaMatrix();

    *outRowCount = G->numRows();
    *outRowLabels = (char**) malloc(sizeof(char*)**outRowCount); memset(*outRowLabels, 0, sizeof(char*)**outRowCount);
    for (int i = 0; i < *outRowCount; i++)
    {
        stringstream stream; stream << i;
        (*outRowLabels)[i] = strdup(stream.str().c_str());
    }
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getStoichiometryMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural_getSpeciesIds(outRowLabels, outRowCount);
    LibStructural_getReactionIds(outColLabels, outColCount);
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getFullyReorderedStoichiometryMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    vector<string> oRows; vector<string> oCols;
    LibStructural::getInstance()->getFullyReorderedStoichiometryMatrixLabels(oRows, oCols);

    ls::CopyStringVector(oRows, *outRowLabels, *outRowCount);
    ls::CopyStringVector(oCols, *outColLabels, *outColCount);

    return 0;
}
/*LIB_EXTERN*/  int LibStructural_getReorderedStoichiometryMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
    LibStructural_getReorderedSpeciesIds(outRowLabels, outRowCount);
    LibStructural_getReactionIds(outColLabels, outColCount);
    return 0;
}

/*LIB_EXTERN*/  int LibStructural_getFullyReorderedStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols)
{
    try
    {
    DoubleMatrix* oMatrix = LibStructural::getInstance()->getFullyReorderedStoichiometryMatrix();
    if (oMatrix == NULL)
        return -1;
    ls::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
    delete oMatrix;
    }
    catch(...)
    {
        return -1;
    }
    return 0;
}

/*LIB_EXTERN*/  double LibStructural_getTolerance()
{
    return LibStructural::getInstance()->getTolerance();
}

///// More C-API
//LIB_EXTERN double LibLA_getTolerance()
//{
//    return getInstance()->getTolerance();
//}
//
//LIB_EXTERN void LibLA_setTolerance(const double value)
//{
//    getInstance()->setTolerance(value);
//}
//
//LIB_EXTERN int LibLA_getEigenValues(double** inMatrix, int numRows, int numCols,
//                                    double* *outReal, double * *outImag, int *outLength)
//{
//    try
//    {
//        DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//        vector< Complex > oVector = getInstance()->getEigenValues( oMatrix );
//
//        ls::CopyComplexVector(oVector, *outReal, *outImag, *outLength);
//
//        return 0;
//    }
//    catch(...)
//    {
//        return -1;
//    }
//}
//
//LIB_EXTERN int LibLA_ZgetEigenValues(double** inMatrixReal, double** inMatrixImag, int numRows, int numCols,
//                                     double* *outReal, double * *outImag, int *outLength)
//{
//    try
//    {
//        ComplexMatrix oMatrix(numRows, numCols);
//
//        for (int i = 0; i < numRows; i++)
//        {
//            for (int j = 0; j < numCols; j++)
//            {
//                oMatrix(i,j).set( inMatrixReal[i][j],inMatrixImag[i][j]);
//            }
//        }
//
//        vector< Complex > oVector = getInstance()->ZgetEigenValues( oMatrix );
//
//        ls::CopyComplexVector(oVector, *outReal, *outImag, *outLength);
//
//        oVector.clear();
//        return 0;
//    }
//    catch(...)
//    {
//        return -1;
//    }
//}
//
//LIB_EXTERN int LibLA_getQRWithPivot(double** inMatrix, int numRows, int numCols,
//                                    double** *outQ, int *outQRows, int * outQCols,
//                                    double** *outR, int *outRRows, int * outRCols,
//                                    double** *outP, int *outPRows, int * outPCols)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    vector< DoubleMatrix * > oVector = LibLA::getInstance()->getQRWithPivot( oMatrix );
//
//    ls::CopyMatrix(*oVector[0], *outQ, *outQRows, *outQCols); delete oVector[0];
//    ls::CopyMatrix(*oVector[1], *outR, *outRRows, *outRCols); delete oVector[1];
//    ls::CopyMatrix(*oVector[2], *outP, *outPRows, *outPCols); delete oVector[2];
//
//
//    return 0;
//}
//
//LIB_EXTERN int LibLA_getQR(double** inMatrix, int numRows, int numCols,
//                           double** *outQ, int *outQRows, int * outQCols,
//                           double** *outR, int *outRRows, int * outRCols)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    vector< DoubleMatrix * > oVector = LibLA::getInstance()->getQR( oMatrix );
//
//    ls::CopyMatrix(*oVector[0], *outQ, *outQRows, *outQCols); delete oVector[0];
//    ls::CopyMatrix(*oVector[1], *outR, *outRRows, *outRCols); delete oVector[1];
//
//    return 0;
//}
//
//LIB_EXTERN int LibLA_getSingularValsBySVD(double** inMatrix, int numRows, int numCols,
//                                          double* *outSingularVals, int *outLength)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    vector< double > oVector = LibLA::getInstance()->getSingularValsBySVD( oMatrix );
//
//    ls::CopyDoubleVector(oVector, *outSingularVals, *outLength);
//
//    return 0;
//}
//
//LIB_EXTERN int LibLA_getLUwithFullPivoting(double** inMatrix, int numRows, int numCols,
//                                           double** *outL, int *outLRows, int * outLCols,
//                                           double** *outU, int *outURows, int * outUCols,
//                                           int** *outP, int *outPRows, int * outPCols,
//                                           int** *outQ, int *outQRows, int * outQCols,
//                                           int *info)
//{
//
//    try
//    {
//        DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//        LU_Result* oResult = LibLA::getInstance()->getLUwithFullPivoting( oMatrix );
//
//        ls::CopyMatrix(*(oResult->L), *outL, *outLRows, *outLCols);
//        ls::CopyMatrix(*(oResult->U), *outU, *outURows, *outUCols);
//        ls::CopyMatrix(*(oResult->P), *outP, *outPRows, *outPCols);
//        ls::CopyMatrix(*(oResult->Q), *outQ, *outQRows, *outQCols);
//
//        *info = oResult->nInfo;
//
//        delete oResult;
//
//        return 0;
//    }
//    catch(...)
//    {
//        return -1;
//    }
//}
//
//LIB_EXTERN int LibLA_getLU(double** inMatrix, int numRows, int numCols,
//                           double** *outL, int *outLRows, int * outLCols,
//                           double** *outU, int *outURows, int * outUCols,
//                           int** *outP, int *outPRows, int * outPCols,
//                           int *info)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    LU_Result* oResult = LibLA::getInstance()->getLU( oMatrix );
//
//    ls::CopyMatrix(*(oResult->L), *outL, *outLRows, *outLCols);
//    ls::CopyMatrix(*(oResult->U), *outU, *outURows, *outUCols);
//    ls::CopyMatrix(*(oResult->P), *outP, *outPRows, *outPCols);
//
//    *info = oResult->nInfo;
//
//    delete oResult;
//    return 0;
//}
//
//LIB_EXTERN int LibLA_inverse(double** inMatrix, int numRows, int numCols,
//                             double** *outMatrix, int *outRows, int *outCols)
//{
//
//    try
//    {
//        DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//        DoubleMatrix *oResult = LibLA::getInstance()->inverse( oMatrix );
//
//        ls::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;
//
//        return 0;
//    }
//    catch (...)
//    {
//        return -1;
//    }
//}
//
//
//LIB_EXTERN int LibLA_leftNullspace(double** inMatrix, int numRows, int numCols,
//                                   double** *outMatrix, int *outRows, int *outCols)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    DoubleMatrix *oResult = LibLA::getInstance()->getLeftNullSpace( oMatrix );
//
//    ls::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;
//
//    return 0;
//}
//LIB_EXTERN int LibLA_rightNullspace(double** inMatrix, int numRows, int numCols,
//                                    double** *outMatrix, int *outRows, int *outCols)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    DoubleMatrix *oResult = LibLA::getInstance()->getRightNullSpace( oMatrix );
//
//    ls::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;
//
//    return 0;
//}
//
//LIB_EXTERN int LibLA_scaledLeftNullspace(double** inMatrix, int numRows, int numCols,
//                                         double** *outMatrix, int *outRows, int *outCols)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    DoubleMatrix *oResult = LibLA::getInstance()->getScaledLeftNullSpace( oMatrix );
//
//    ls::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;
//
//    return 0;
//}
//LIB_EXTERN int LibLA_scaledRightNullspace(double** inMatrix, int numRows, int numCols,
//                                          double** *outMatrix, int *outRows, int *outCols)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    DoubleMatrix *oResult = LibLA::getInstance()->getScaledRightNullSpace( oMatrix );
//
//    ls::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;
//
//    return 0;
//}
//
//LIB_EXTERN int LibLA_getRank(double** inMatrix, int numRows, int numCols)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    int nRank = LibLA::getInstance()->getRank( oMatrix );
//    return nRank;
//}
//
//
//LIB_EXTERN double LibLA_getRCond(double** inMatrix, int numRows, int numCols)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    double dRcond = LibLA::getInstance()->getRCond( oMatrix );
//    return dRcond;
//}
//
//
//LIB_EXTERN int LibLA_Zinverse(double** inMatrixReal, double **inMatrixImag, int numRows, int numCols,
//                              double** *outMatrixReal, double ** *outMatrixImag, int *outRows, int *outCols)
//{
//    try
//    {
//        ComplexMatrix oMatrix(numRows, numCols);
//
//        for (int i = 0; i < numRows; i++)
//        {
//            for (int j = 0; j < numCols; j++)
//            {
//                oMatrix(i,j).set(inMatrixReal[i][j],inMatrixImag[i][j]);
//            }
//        }
//
//        ComplexMatrix *oResult = LibLA::getInstance()->Zinverse( oMatrix );
//        ls::CopyMatrix(*oResult, *outMatrixReal, *outMatrixImag, *outRows, *outCols); delete oResult;
//
//        return 0;
//    }
//    catch(...)
//    {
//        return -1;
//    }
//}
//
//LIB_EXTERN void LibLA_freeVector(void* vector)
//{
//    if (vector) free(vector);
//}
//
//LIB_EXTERN void LibLA_freeMatrix(void** matrix, int numRows)
//{
//    for (int i = 0; i < numRows; i++)
//    {
//        if (matrix[i]) free(matrix[i]);
//    }
//    free(matrix);
//}
//
//
//
//LIB_EXTERN int LibLA_gaussJordan(double** inMatrix, int numRows, int numCols,
//                                 double** *outMatrix, int *outRows, int *outCols, int* *oPivots, int *nLength)
//{
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    vector<int> oPivotVec  = ls::GaussJordan(oMatrix, LibLA::getInstance()->getTolerance());
//    ls::CopyMatrix(oMatrix, *outMatrix, *outRows, *outCols);
//
//    ls::CopyIntVector(oPivotVec, *oPivots, *nLength);
//    return 0;
//}
//
//std::vector<int> LibLA::gaussJordan(DoubleMatrix &oMatrix)
//{
//    return ls::GaussJordan(oMatrix, gLapackTolerance);
//}
//
//
////LIB_EXTERN int LibLA_gaussJordan2(double** inMatrix, int numRows, int numCols,
////                                  double** *outMatrix, int *outRows, int *outCols)
////{
////    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
////    ls::gaussJordan(oMatrix, LibLA::getInstance()->getTolerance());
////    ls::CopyMatrix(oMatrix, outMatrix, outRows, outCols);
////    return 0;
////}
//
//
//void LibLA::fullyPivotedGaussJordan(DoubleMatrix &oMatrix, vector<int> &rowPivots, vector<int> &colPivots)
//{
//    return ls::FullyPivotedGaussJordan(oMatrix, gLapackTolerance, rowPivots, colPivots);
//}
//
//
//LIB_EXTERN int LibLA_fullyPivotedGaussJordan(double** inMatrix, int numRows, int numCols,
//                                             double** *outMatrix, int *outRows, int *outCols,
//                                             int* *oRowPivots, int *nRowLength,
//                                             int* *oColPivots, int *nColLength)
//{
//
//    DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//    vector< int > vecRowPivots;  vector< int> vecColPivots;
//    ls::FullyPivotedGaussJordan(oMatrix, LibLA::getInstance()->getTolerance(), vecRowPivots, vecColPivots);
//
//    ls::CopyMatrix(oMatrix, *outMatrix, *outRows, *outCols);
//
//    ls::CopyIntVector(vecRowPivots, *oRowPivots, *nRowLength);
//    ls::CopyIntVector(vecColPivots, *oColPivots, *nColLength);
//
//    return 0;
//
//}
//LibLA_getEigenVectors
//
//LIB_EXTERN int LibLA_getEigenVectors(double** inMatrix, int numRows, int numCols,
//                                     double** *outMatrixReal, double** *outMatrixImag, int *outRows, int *outCols)
//{
//
//    try
//    {
//        DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//        ComplexMatrix *oResult = LibLA::getInstance()->getEigenVectors( oMatrix );
//
//        ls::CopyMatrix(*oResult, *outMatrixReal, *outMatrixImag, *outRows, *outCols); delete oResult;
//
//        return 0;
//    }
//    catch (...)
//    {
//        return -1;
//    }
//}
//
//LIB_EXTERN int LibLA_ZgetEigenVectors(double** inMatrixReal,   double **  inMatrixImag, int numRows, int numCols,
//                                      double** *outMatrixReal, double** *outMatrixImag, int *outRows, int *outCols)
//{
//    try
//    {
//        ComplexMatrix oMatrix(numRows, numCols);
//
//        for (int i = 0; i < numRows; i++)
//        {
//            for (int j = 0; j < numCols; j++)
//            {
//                oMatrix(i,j).set(inMatrixReal[i][j],inMatrixImag[i][j]);
//            }
//        }
//
//        ComplexMatrix *oResult = LibLA::getInstance()->ZgetEigenVectors( oMatrix );
//        ls::CopyMatrix(*oResult, *outMatrixReal, *outMatrixImag, *outRows, *outCols); delete oResult;
//
//        return 0;
//    }
//    catch(...)
//    {
//        return -1;
//    }
//}
//
//LIB_EXTERN int LibLA_getSVD(double** inMatrix, int numRows, int numCols,
//                            double** *outU, int *outRowsU, int *outColsU,
//                            double* *outSingVals, int *outLength,
//                            double** *outV, int *outRowsV, int *outColsV)
//{
//    try
//    {
//        DoubleMatrix oMatrix(inMatrix, numRows, numCols);
//
//        DoubleMatrix*   matrixU  = NULL;
//        vector<double>* singVals = NULL;
//        DoubleMatrix*   matrixV  = NULL;
//
//        LibLA::getInstance()->getSVD(oMatrix, matrixU, singVals, matrixV);
//
//        ls::CopyMatrix(*matrixU, *outU, *outRowsU, *outColsU); delete matrixU;
//        ls::CopyDoubleVector(*singVals, *outSingVals, *outLength); delete singVals;
//        ls::CopyMatrix(*matrixV, *outV, *outRowsV, *outColsV); delete matrixV;
//
//        return 0;
//    }
//    catch(...)
//    {
//        return -1;
//    }
//
//}
//
//LIB_EXTERN int LibLA_ZgetSVD(double** inMatrixReal, double** inMatrixImag, int numRows, int numCols,
//                             double** *outUReal, double** *outUImag, int *outRowsU, int *outColsU,
//                             double* *outSingVals, int *outLength,
//                             double** *outVReal, double** *outVImag, int *outRowsV, int *outColsV)
//{
//    try
//    {
//        ComplexMatrix oMatrix(numRows, numCols);
//
//        for (int i = 0; i < numRows; i++)
//        {
//            for (int j = 0; j < numCols; j++)
//            {
//                oMatrix(i,j).set(inMatrixReal[i][j],inMatrixImag[i][j]);
//            }
//        }
//
//        ComplexMatrix *matrixU = NULL; vector<double>* singVals= NULL; ComplexMatrix *matrixV= NULL;
//
//        LibLA::getInstance()->ZgetSVD(oMatrix, matrixU, singVals, matrixV);
//
//        ls::CopyMatrix(*matrixU, *outUReal, *outUImag, *outRowsU, *outColsU); delete matrixU;
//        ls::CopyDoubleVector(*singVals, *outSingVals, *outLength); delete singVals;
//        ls::CopyMatrix(*matrixV, *outVReal, *outVImag, *outRowsV, *outColsV); delete matrixV;
//
//        return 0;
//    }
//    catch(...)
//    {
//        return -1;
//    }
//}



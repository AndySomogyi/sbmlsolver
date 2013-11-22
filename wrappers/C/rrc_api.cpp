/**
 * @file rrc_core_api.cpp
 * @brief roadRunner C API 2012
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib for more details.
 *
 * Copyright (C) 2012-2013
 *   University of Washington, Seattle, WA, USA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * In plain english this means:
 *
 * You CAN freely download and use this software, in whole or in part, for personal,
 * company internal, or commercial purposes;
 *
 * You CAN use the software in packages or distributions that you create.
 *
 * You SHOULD include a copy of the license in any redistribution you may make;
 *
 * You are NOT required include the source of software, or of any modifications you may
 * have made to it, in any redistribution you may assemble that includes it.
 *
 * YOU CANNOT:
 *
 * redistribute any piece of this software without proper attribution;
*/

#pragma hdrstop
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "rrRoadRunner.h"
#include "rrRoadRunnerList.h"
#include "c/rrCModelGenerator.h"
#include "rrLogger.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrc_api.h"           // Need to include this before the support header..
#include "rrc_utilities.h"   //Support functions, not exposed as api functions and or data
#include "rrc_cpp_support.h"   //Support functions, not exposed as api functions and or data

#if defined(_MSC_VER)
    #include <direct.h>
    #define getcwd _getcwd
    #define chdir  _chdir
#elif defined(__BORLANDC__)
      #include <dir.h>
#else
    #include <unistd.h>
#endif

/*!
 \brief macro for MAX_PATH
 \ingroup macros
*/
 #if defined(_WIN32)
    #include <windef.h>
   #define RR_MAX_PATH MAX_PATH
#else
    #include <limits.h>
    #define RR_MAX_PATH PATH_MAX
#endif
//---------------------------------------------------------------------------

namespace rrc
{

using namespace std;
using namespace rr;

static NewArrayList        sel_getFluxControlCoefficientIds(RoadRunner* rr);
static NewArrayList        sel_getAvailableSteadyStateSymbols(RoadRunner* rr);
static NewArrayList        sel_getAvailableTimeCourseSymbols(RoadRunner* rr);
static vector<string>    sel_getBoundarySpeciesAmountIds(RoadRunner* rr);
static vector<string>    sel_getBoundarySpeciesConcIds(RoadRunner* rr);
static NewArrayList        sel_getConcentrationControlCoefficientIds(RoadRunner* rr);
static NewArrayList        sel_getUnscaledConcentrationControlCoefficientIds(RoadRunner* rr);
static NewArrayList        sel_getElasticityCoefficientIds(RoadRunner* rr);
static NewArrayList        sel_getUnscaledElasticityCoefficientIds(RoadRunner* rr);
static vector<string>    sel_getFloatingSpeciesConcSymbols(RoadRunner* rr);
static vector<string>    sel_getBoundarySpeciesConcSymbols(RoadRunner* rr);

RRHandle rrcCallConv createRRInstance()
{
    try
    {
        string rrInstallFolder(getParentFolder(getRRCAPILocation()));
        return new RoadRunner("", getUsersTempDataFolder(), joinPath(rrInstallFolder, "rr_support"));
    }
    catch_ptr_macro
}

RRHandle rrcCallConv createRRInstanceEx(const char* tempFolder, const char* compiler_cstr)
{
    try
    {
        char* text1 = getRRCAPILocation();
        string text2 = getParentFolder(text1);
        string rrInstallFolder(text2);
        rr::freeText(text1);
        string compiler = compiler_cstr ? compiler_cstr : "";

        if(tempFolder != NULL && !fileExists(tempFolder))
        {
            stringstream msg;
            msg<<"The temporary folder: "<<tempFolder<<" do not exist";
            Log(Logger::LOG_ERROR)<<msg.str();
            throw(Exception(msg.str()));
        }
        else if(tempFolder)
        {
            return new RoadRunner(compiler, tempFolder, joinPath(rrInstallFolder, "rr_support"));
        }
        else
        {
            return new RoadRunner(compiler, getUsersTempDataFolder(), joinPath(rrInstallFolder, "rr_support"));
        }
    }
    catch_ptr_macro
}

RRInstanceListPtr rrcCallConv createRRInstances(int count)
{
    try
    {
        string tempFolder = getUsersTempDataFolder();

        RoadRunnerList* listHandle = new RoadRunnerList(count, tempFolder);

        //Create the C list structure
        RRInstanceListPtr rrList = new RRInstanceList;
        rrList->RRList = (void*) listHandle;
        rrList->Count = count;

        //Create 'count' handles
        rrList->Handle = new RRHandle[count];

        //Populate handles
        for(int i = 0; i < count; i++)
        {
            rrList->Handle[i] = (*listHandle)[i];
        }
        return rrList;
    }
    catch_ptr_macro
}

bool rrcCallConv freeRRInstances(RRInstanceListPtr rrList)
{
    try
    {
        //Delete the C++ list
        RoadRunnerList* listHandle = (RoadRunnerList*) rrList->RRList;

        delete listHandle;

        //Free  C handles
        delete [] rrList->Handle;

        //Free the C list
        delete rrList;
        return true;
    }
    catch_bool_macro
}

char* rrcCallConv getInstallFolder()
{
    if(!gInstallFolder[0])
    {
        strncpy(gInstallFolder, "/usr/local", RR_MAX_PATH);
    }
    return gInstallFolder;
}

bool rrcCallConv setInstallFolder(const char* folder)
{
    try
    {
        return strncpy(gInstallFolder, folder, RR_MAX_PATH) != NULL ? true : false;
    }
    catch_bool_macro
}

char* rrcCallConv getAPIVersion()
{
    try
    {
        return rr::createText("0.99");
    }
    catch_ptr_macro
}

char* rrcCallConv getCPPAPIVersion(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->getVersion());
    }
    catch_ptr_macro
}

char* rrcCallConv getRRCAPILocation()
{
#if defined(_WIN32) || defined(WIN32)
    char path[MAX_PATH];
    HINSTANCE handle = NULL;
    const char* dllName = "rrc_api";
    handle = GetModuleHandle(dllName);
    int nrChars = GetModuleFileNameA(handle, path, sizeof(path));
    if(nrChars != 0)
    {
        string aPath = getFilePath(path);
        char* text = rr::createText(aPath);
        return text;
    }
    return NULL;
#else
    return rr::createText(joinPath(getInstallFolder(),"/lib"));
#endif
}

char* rrcCallConv getCopyright()
{
    try
    {
        RRHandle handle = createRRInstance();
        if(!handle)
        {
            return NULL;
        }

        RoadRunner* rri = castFrom(handle);
        char* text = rr::createText(rri->getCopyright());
        freeRRInstance(handle);
        return text;
    }
    catch_ptr_macro
}

char* rrcCallConv getInfo(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->getInfo());
    }
    catch_ptr_macro
}

char* rrcCallConv getExtendedAPIInfo()
{
    try
    {
        RRHandle handle = createRRInstance();
        RoadRunner* rri = castFrom(handle);
        char* text = rr::createText(rri->getExtendedVersionInfo());
        freeRRInstance(handle);
        return text;
    }
    catch_ptr_macro
}

char* rrcCallConv getlibSBMLVersion(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->getlibSBMLVersion());
    }
    catch_ptr_macro
}

char* rrcCallConv getCurrentSBML(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->writeSBML());
    }
    catch_ptr_macro
}

//Flags and Options
bool rrcCallConv setComputeAndAssignConservationLaws(RRHandle handle, const bool OnOrOff)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setConservedMoietyAnalysis(OnOrOff);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setTempFolder(RRHandle handle, const char* folder)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->setTempFileFolder(folder);
    }
    catch_bool_macro
}

char* rrcCallConv getTempFolder(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->getTempFolder());
    }
    catch_ptr_macro
}

bool rrcCallConv setCompiler(RRHandle handle, const char* fName)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(rri->getCompiler())
        {
            return rri->getCompiler()->setCompiler(fName);
        }
        else
        {
            return false;
        }
    }
    catch_bool_macro
}

char* rrcCallConv getCompiler(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        Compiler *compiler = rri->getCompiler();
        return strdup(compiler ? compiler->getCompiler().c_str() : "");
    }
    catch_ptr_macro
}

bool rrcCallConv setCompilerLocation(RRHandle handle, const char* folder)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(rri->getCompiler())
        {
            return rri->getCompiler()->setCompilerLocation(folder);
        }
        else
        {
            return false;
        }
    }
    catch_bool_macro
}

char* rrcCallConv getCompilerLocation(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->getCompiler()->getCompilerLocation());
    }
    catch_ptr_macro
}

bool rrcCallConv setSupportCodeFolder(RRHandle handle,const char* folder)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(rri->getCompiler())
        {
            return rri->getCompiler()->setSupportCodeFolder(folder);
        }
        else
        {
            return false;
        }
    }
    catch_bool_macro
}

char* rrcCallConv getSupportCodeFolder(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->getCompiler()->getSupportCodeFolder());
    }
    catch_ptr_macro
}

char* rrcCallConv getWorkingDirectory()
{
    try
    {
        return rr::createText(rr::getCWD());
    }
    catch_ptr_macro
}

bool rrcCallConv loadSBMLFromFile(RRHandle _handle, const char* fileName)
{
    try
    {
        //Check first if file exists first
        if(!fileExists(fileName))
        {
            stringstream msg;
            msg<<"The file "<<fileName<<" was not found";
            setError(msg.str());
            return false;
        }

        RoadRunner* rri = castFrom(_handle);
        if(!rri->load(fileName))
        {
            setError("Failed to load SBML semantics");    //There are many ways loading a model can fail, look at logFile to know more
            return false;
        }
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv loadSBMLFromFileE(RRHandle _handle, const char* fileName, bool forceRecompile)
{
    try
    {
        //Check first if file exists first
        if(!fileExists(fileName))
        {
            stringstream msg;
            msg<<"The file "<<fileName<<" was not found";
            setError(msg.str());
            return false;
        }

        RoadRunner* rri = castFrom(_handle);

        LoadSBMLOptions opt;
        opt.modelGeneratorOpt = forceRecompile ?
                opt.modelGeneratorOpt | LoadSBMLOptions::RECOMPILE :
                opt.modelGeneratorOpt & ~LoadSBMLOptions::RECOMPILE;

        if(!rri->load(fileName, &opt))
        {
            setError("Failed to load SBML semantics");    //There are many ways loading a model can fail, look at logFile to know more
            return false;
        }
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv loadSBML(RRHandle handle, const char* sbml)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->load(sbml);
    }
    catch_bool_macro
}

bool rrcCallConv loadSBMLEx(RRHandle handle, const char* sbml, bool forceRecompile)
{
    try
    {
        RoadRunner* rri = castFrom(handle);

        LoadSBMLOptions opt;
                opt.modelGeneratorOpt = forceRecompile ?
                        opt.modelGeneratorOpt | LoadSBMLOptions::RECOMPILE :
                        opt.modelGeneratorOpt & ~LoadSBMLOptions::RECOMPILE;

        if(!rri->load(sbml, &opt))
        {
            setError("Failed to load SBML semantics");
            return false;
        }
        return true;
    }
    catch_bool_macro
}


bool rrcCallConv loadSimulationSettings(RRHandle handle, const char* fileName)
{
    try
    {
        //Check first if file exists first
        if(!fileExists(fileName))
        {
            stringstream msg;
            msg<<"The file "<<fileName<<" was not found";
            setError(msg.str());
            return false;
        }

        RoadRunner* rri = castFrom(handle);
        rri->setSimulateOptions(SimulateOptions(fileName));
        return true;
    }
    catch_bool_macro
}

char* rrcCallConv getSBML(RRHandle handle)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->getSBML());
    }
    catch_ptr_macro
}

bool rrcCallConv isModelLoaded(RRHandle handle)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        return rri->isModelLoaded();
    }
    catch_bool_macro
}

bool rrcCallConv unLoadModel(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->unLoadModel();
    }
    catch_bool_macro
}

bool rrcCallConv setTimeStart(RRHandle handle, const double timeStart)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->getSimulateOptions().start = timeStart;
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setTimeEnd(RRHandle handle, const double timeEnd)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        SimulateOptions &opt = rri->getSimulateOptions();
        opt.duration = timeEnd - opt.start;
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setNumPoints(RRHandle handle, const int nrPoints)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        SimulateOptions &opt = rri->getSimulateOptions();
        opt.steps = nrPoints - 1;
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv getTimeStart(RRHandle handle, double* timeStart)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *timeStart = rri->getSimulateOptions().start;
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv getTimeEnd(RRHandle handle, double* timeEnd)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *timeEnd = rri->getSimulateOptions().duration + rri->getSimulateOptions().start;
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv getNumPoints(RRHandle handle, int* numPoints)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *numPoints = rri->getSimulateOptions().steps + 1;
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setTimeCourseSelectionList(RRHandle handle, const char* list)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList aList(list,", ");
        rri->setSelections(aList);
        return true;
    }
    catch_bool_macro
}


RRStringArrayPtr rrcCallConv getTimeCourseSelectionList(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);

        vector<SelectionRecord> selections = rri->getSelections();

        vector<string> list(selections.size());

        for(int i = 0; i < selections.size(); ++i)
        {
            list[i] = selections[i].to_string();
        }

        StringList sNames = list;

        if(!sNames.Count())
        {
            return NULL;
        }

        return createList(sNames);
    }
    catch_ptr_macro
}

RRCDataPtr rrcCallConv simulate(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);

        rri->getSimulateOptions().flags |= SimulateOptions::RESET_MODEL;

        if(!rri->simulate())
        {
            return NULL;
        }

        RoadRunnerData result = *rri->getSimulationResult();

        //Extract the data and return struct..
        RRCData* aResult  = createRRCData(result);
        return aResult;
    }
    catch_ptr_macro
}

RRCDataPtr rrcCallConv getSimulationResult(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        RoadRunnerData result = *rri->getSimulationResult();

        //Extract the data and return struct..
        return  createRRCData(result);
    }
    catch_ptr_macro
}

RRDataHandle rrcCallConv getRoadRunnerData(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->getSimulationResult();
    }
    catch_ptr_macro
}

RRCDataPtr rrcCallConv simulateEx(RRHandle handle, const double timeStart, const double timeEnd, const int numberOfPoints)
{
    try
    {
        setTimeStart(handle, timeStart);
        setTimeEnd (handle, timeEnd);
        setNumPoints(handle, numberOfPoints);
        return simulate(handle);
    }
    catch_ptr_macro
}

RRStringArrayPtr rrcCallConv getReactionIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList rNames = rri->getReactionIds();

        if(!rNames.Count())
        {
            return NULL;
        }
        return createList(rNames);
    }
    catch_ptr_macro
}

RRVectorPtr rrcCallConv getRatesOfChange(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> rates = rri->getRatesOfChange();

        if(!rates.size())
        {
            return NULL;
        }

        return rrc::createVector(rates);
    }
    catch_ptr_macro
}

RRStringArrayPtr rrcCallConv getRatesOfChangeIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList rNames = rri->getRateOfChangeIds();

        if(!rNames.Count())
        {
            return NULL;
        }

        return createList(rNames);
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getUnscaledElasticityMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix tempMat = rri->getUnscaledElasticityMatrix();
        RRDoubleMatrixPtr matrix = createMatrix(&tempMat);
        return matrix;
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getScaledElasticityMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix tempMat = rri->getScaledElasticityMatrix();
        RRDoubleMatrixPtr matrix = createMatrix(&tempMat);
        return matrix;
    }
    catch_ptr_macro
}

bool rrcCallConv getValue(RRHandle handle, const char* symbolId, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getValue(symbolId);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setValue(RRHandle handle, const char* symbolId, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->setValue(symbolId, value);
    }
    catch_bool_macro
}

RRDoubleMatrixPtr rrcCallConv getStoichiometryMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        ExecutableModel* model = rri->getModel();

        if(model)
        {
            int rows = 0;
            int cols = 0;
            double* data = 0;

            model->getStoichiometryMatrix(&rows, &cols, &data);

            RRDoubleMatrixPtr matrix = new RRDoubleMatrix;
            matrix->RSize = rows;
            matrix->CSize = cols;
            matrix->Data =  new double[rows*cols];

            memcpy(matrix->Data, data, rows*cols*sizeof(double));

            free(data);

            return matrix;
        }
        else
        {
            return 0;
        }
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getConservationMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        DoubleMatrix tempMat = rri->getConservationMatrix();

        RRDoubleMatrixPtr matrix = new RRDoubleMatrix;
        matrix->RSize = tempMat.RSize();
        matrix->CSize = tempMat.CSize();
        matrix->Data =  new double[tempMat.RSize()*tempMat.CSize()];

        int index = 0;
        for(u_int row = 0; row < tempMat.RSize(); row++)
        {
            for(u_int col = 0; col < tempMat.CSize(); col++)
            {
                matrix->Data[index++] = tempMat(row,col);
            }
        }
        return matrix;
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getLinkMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix *tempMat = rri->getLinkMatrix();
        return createMatrix(tempMat);
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getNrMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix *tempMat = rri->getNrMatrix();
        return createMatrix(tempMat);
    }
    catch_ptr_macro
}

C_DECL_SPEC bool rrcCallConv hasError()
{
    return (gLastError != NULL) ? true : false;
}

char* rrcCallConv getLastError()
{
    if(!gLastError)
    {
        gLastError = rr::createText("No Error");
    }
    return gLastError;
}

bool rrcCallConv reset(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->reset();
        return true;
    }
    catch_bool_macro
}

int rrcCallConv getNumberOfReactions(RRHandle handle)
{
     try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->getNumberOfReactions();
    }
    catch_int_macro
}

bool rrcCallConv getReactionRate(RRHandle handle, const int rateNr, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getReactionRate(rateNr);
        return true;
    }
    catch_bool_macro
}

RRVectorPtr rrcCallConv getReactionRates(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> vec =  rri->getReactionRates();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch_ptr_macro
}

int rrcCallConv getNumberOfBoundarySpecies(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->getNumberOfBoundarySpecies();
    }
    catch_int_macro
}

RRStringArrayPtr rrcCallConv getBoundarySpeciesIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList bNames = rri->getBoundarySpeciesIds();

        if(!bNames.Count())
        {
            return NULL;
        }

        return createList(bNames);
    }
    catch_ptr_macro
}

int rrcCallConv getNumberOfFloatingSpecies(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->getNumberOfFloatingSpecies();
    }
    catch_int_macro
}

RRStringArrayPtr rrcCallConv getFloatingSpeciesIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList fNames = rri->getFloatingSpeciesIds();

        if(!fNames.Count())
        {
            return NULL;
        }

        return createList(fNames);
    }
    catch_ptr_macro
}

int rrcCallConv getNumberOfGlobalParameters(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->getNumberOfGlobalParameters();
    }
    catch_int_macro
}

RRStringArrayPtr rrcCallConv getGlobalParameterIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList pNames = rri->getGlobalParameterIds();

        if(!pNames.Count())
        {
            return NULL;
        }

        return createList(pNames);
    }
    catch_ptr_macro
}

bool rrcCallConv getFloatingSpeciesInitialConcentrationByIndex(RRHandle handle, int index, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        ExecutableModel *model = rri->getModel();

        if (model && model->getNumFloatingSpecies() > index)
        {
            return model->getFloatingSpeciesInitConcentrations(1, &index, value) >= 0;
        }
        return true;
    }
    catch_bool_macro
}

RRVectorPtr rrcCallConv getFloatingSpeciesConcentrations(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> vec =  rri->getFloatingSpeciesConcentrations();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch_ptr_macro
}

RRVectorPtr rrcCallConv getBoundarySpeciesConcentrations(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> vec =  rri->getBoundarySpeciesConcentrations();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch_ptr_macro
}

RRVectorPtr rrcCallConv getFloatingSpeciesInitialConcentrations(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> vec =  rri->getFloatingSpeciesInitialConcentrations();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch_ptr_macro
}

bool rrcCallConv setFloatingSpeciesByIndex (RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setFloatingSpeciesByIndex(index, value);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setBoundarySpeciesByIndex (RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setBoundarySpeciesByIndex(index, value);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setGlobalParameterByIndex(RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setGlobalParameterByIndex(index, value);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setFloatingSpeciesInitialConcentrationByIndex(RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setFloatingSpeciesInitialConcentrationByIndex(index, value);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setFloatingSpeciesInitialConcentrations(RRHandle handle, const RRVector* vec)
{
    try
    {
        vector<double> tempVec;
        copyVector(vec, tempVec);
        RoadRunner* rri = castFrom(handle);
        rri->changeInitialConditions(tempVec);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setFloatingSpeciesConcentrations(RRHandle handle, const RRVector* vec)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        vector<double> tempVec;
        copyVector(vec, tempVec);
        rri->setFloatingSpeciesConcentrations(tempVec);

        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setBoundarySpeciesConcentrations(RRHandle handle, const RRVector* vec)
{
    try
    {
        vector<double> tempVec;
        copyVector(vec, tempVec);
        RoadRunner* rri = castFrom(handle);
        rri->setBoundarySpeciesConcentrations(tempVec);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv oneStep(RRHandle handle, const double currentTime, const double stepSize, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->oneStep(currentTime, stepSize);
        return true;
    }
    catch_bool_macro
}

RRVectorPtr rrcCallConv getGlobalParameterValues(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> vec =  rri->getGlobalParameterValues();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch_ptr_macro
}

RRListPtr rrcCallConv getAvailableTimeCourseSymbols(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList slSymbols = sel_getAvailableTimeCourseSymbols(rri);
        return createArrayList(slSymbols);
    }
    catch_ptr_macro
}

RRListPtr rrcCallConv getAvailableSteadyStateSymbols(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList slSymbols = sel_getAvailableSteadyStateSymbols(rri);
        return createArrayList(slSymbols);
    }
    catch_ptr_macro
}

bool rrcCallConv getBoundarySpeciesByIndex (RRHandle handle, const int index, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getBoundarySpeciesByIndex(index);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv getFloatingSpeciesByIndex (RRHandle handle, const int index, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getFloatingSpeciesByIndex(index);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv getGlobalParameterByIndex (RRHandle handle, const int index, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getGlobalParameterByIndex(index);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv getuCC (RRHandle handle, const char* variable, const char* parameter, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getuCC(variable, parameter);
        return true;
    }
    catch_bool_macro
}


bool rrcCallConv getCC (RRHandle handle, const char* variable, const char* parameter, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getCC(variable, parameter);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv getuEE(RRHandle handle, const char* name, const char* species, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getuEE(name, species);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv getEE(RRHandle handle, const char* name, const char* species, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getEE(name, species);
        return true;
    }
    catch_bool_macro
}

int rrcCallConv getNumberOfDependentSpecies(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->getNumberOfDependentSpecies();
    }
    catch_int_macro
}

int rrcCallConv getNumberOfIndependentSpecies(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->getNumberOfIndependentSpecies();
    }
    catch_int_macro
}

bool rrcCallConv steadyState(RRHandle handle, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->steadyState();
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv evalModel(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->evalModel();
        return true;
    }
    catch_bool_macro
}

char* rrcCallConv getParamPromotedSBML(RRHandle handle, const char* sArg)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        string param =  rri->getParamPromotedSBML(sArg);
        char* text = rr::createText(param.c_str());
        return text;
    }
    catch_ptr_macro
}

RRVectorPtr rrcCallConv computeSteadyStateValues(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> vec =  rri->computeSteadyStateValues();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch_ptr_macro
}

bool rrcCallConv setSteadyStateSelectionList(RRHandle handle, const char* list)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList aList(list, " ,");
        rri->setSteadyStateSelections(aList);
        return true;
    }
    catch_bool_macro
}

RRStringArrayPtr rrcCallConv getSteadyStateSelectionList(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<SelectionRecord>& ss = rri->getSteadyStateSelections();

        vector<string> sNames;

        for(int i = 0; i < ss.size(); ++i)
        {
            sNames.push_back(ss[i].to_string());
        }

        return createList(sNames);
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getFullJacobian(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix tempMat = rri->getFullJacobian();
        return createMatrix(&tempMat);
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getReducedJacobian(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix tempMat = rri->getReducedJacobian();
        return createMatrix(&tempMat);
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getEigenvalues(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix tempMat = rri->getEigenvalues();
        return createMatrix(&tempMat);
    }
    catch_ptr_macro
}

char* rrcCallConv getCSourceFileName(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        CModelGenerator* generator = dynamic_cast<CModelGenerator*>(rri->getModelGenerator());
        if(!generator)
        {
            return NULL;
        }

        string fNameS = generator->getSourceCodeFileName();

        fNameS = getFileNameNoExtension(fNameS);
        return rr::createText(fNameS);
    }
    catch_ptr_macro
}

RRCCode* rrcCallConv getCCode(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        CModelGenerator* generator = dynamic_cast<CModelGenerator*>(rri->getModelGenerator());
        if(!generator)
        {
            return NULL;
        }

        RRCCode* cCode = new RRCCode;
        cCode->Header = NULL;
        cCode->Source = NULL;
        string header = generator->getHeaderCode();
        string source = generator->getSourceCode();

        if(header.size())
        {
            cCode->Header = rr::createText(header);
        }

        if(source.size())
        {
            cCode->Source = rr::createText(source);
        }
        return cCode;
    }
    catch_ptr_macro
}

// *******  Not yet implemented  ********
// codeGenerationMode = 0 if mode is C code generation
// codeGenerationMode = 1 ig mode is internal math interpreter
bool rrcCallConv setCodeGenerationMode (RRHandle handle, int mode)
{
    return false;
}

bool rrcCallConv getScaledFloatingSpeciesElasticity(RRHandle handle, const char* reactionId, const char* speciesId, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getScaledFloatingSpeciesElasticity(reactionId, speciesId);
        return true;
    }
    catch_bool_macro
}

RRStringArrayPtr rrcCallConv getFloatingSpeciesInitialConditionIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList aList = rri->getFloatingSpeciesInitialConditionIds();
        return createList(aList);
    }
    catch_ptr_macro
}

RRVectorPtr rrcCallConv getRatesOfChangeEx(RRHandle handle, const RRVectorPtr vec)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> tempList = rrc::createVector(vec);
        tempList = rri->getRatesOfChangeEx(tempList);
        return rrc::createVector (tempList);
    }
    catch_ptr_macro
}

RRVectorPtr rrcCallConv getReactionRatesEx(RRHandle handle, const RRVectorPtr vec)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> conc = rrc::createVector(vec);

        ExecutableModel *model = rri->getModel();

        if (!model)
        {
            throw CoreException(gEmptyModelMessage);
        }

        model->setFloatingSpeciesConcentrations(conc.size(), 0, &conc[0]);

        vector<double> result(model->getNumReactions());

        model->getReactionRates(result.size(), 0, &result[0]);

        return rrc::createVector(result);
    }
    catch_ptr_macro
}

RRListPtr rrcCallConv getElasticityCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList aList = sel_getElasticityCoefficientIds(rri);
        RRListPtr bList = createArrayList(aList);
        return bList;
    }
    catch_ptr_macro
}

bool rrcCallConv setConfigurationXML(RRHandle handle, const char* caps)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!caps)
        {
            return false;
        }
        rri->setConfigurationXML(caps);
        return true;
    }
    catch_bool_macro
}

char* rrcCallConv getConfigurationXML(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rr::createText(rri->getConfigurationXML());
    }
    catch_ptr_macro
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RRStringArrayPtr rrcCallConv getEigenvalueIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList aList = rri->getEigenvalueIds();
        return createList(aList);
    }
    catch_ptr_macro
}

RRListPtr rrcCallConv getFluxControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return createArrayList(sel_getFluxControlCoefficientIds(rri));
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getUnscaledConcentrationControlCoefficientMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix aMat = rri->getUnscaledConcentrationControlCoefficientMatrix();
        //return createMatrix(&(rri->getUnscaledConcentrationControlCoefficientMatrix()));
        return createMatrix(&(aMat));
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getScaledConcentrationControlCoefficientMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix aMat = rri->getScaledConcentrationControlCoefficientMatrix();
        return createMatrix(&(aMat));
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getUnscaledFluxControlCoefficientMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix aMat = rri->getUnscaledFluxControlCoefficientMatrix();
        return createMatrix(&(aMat));
    }
    catch_ptr_macro
}

RRDoubleMatrixPtr rrcCallConv getScaledFluxControlCoefficientMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix aMat = rri->getScaledFluxControlCoefficientMatrix();
        return createMatrix(&(aMat));
    }
    catch_ptr_macro
}

static NewArrayList RoadRunner_getUnscaledFluxControlCoefficientIds(RoadRunner *rr)
{
    NewArrayList oResult;

    vector<string> oReactions = rr->getReactionIds();
    vector<string> oParameters = rr->getGlobalParameterIds();
    vector<string> oBoundary = rr->getBoundarySpeciesIds();
    vector<string> oConservation = rr->getConservedSumIds();

    for(int i = 0; i < oReactions.size(); i++)
    {
        string s = oReactions[i];

        NewArrayList oCCReaction;
        vector<string> oInner;
        oCCReaction.Add(s);

        for(int i = 0; i < oParameters.size(); i++)
        {
            oInner.push_back("ucc(" + s + "," + oParameters[i] + ")");
        }

        for(int i = 0; i < oBoundary.size(); i++)
        {
            oInner.push_back("ucc(" + s + "," + oBoundary[i] + ")");
        }

        for(int i = 0; i < oConservation.size(); i++)
        {
            oInner.push_back("ucc(" + s + "," + oConservation[i] + ")");
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

RRListPtr rrcCallConv getUnscaledFluxControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList arrList = RoadRunner_getUnscaledFluxControlCoefficientIds(rri);
        return createArrayList(arrList);
    }
    catch_ptr_macro
}

RRList* rrcCallConv getConcentrationControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList list = sel_getConcentrationControlCoefficientIds(rri);
        return createArrayList(list);
    }
    catch_ptr_macro
}

RRListPtr rrcCallConv getUnscaledConcentrationControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return createArrayList(sel_getUnscaledConcentrationControlCoefficientIds(rri));
    }
    catch_ptr_macro
}

int rrcCallConv getNumberOfCompartments(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->getNumberOfCompartments();
    }
    catch_int_macro
}

bool rrcCallConv getCompartmentByIndex(RRHandle handle, const int index, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getCompartmentByIndex(index);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setCompartmentByIndex (RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setCompartmentByIndex(index, value);
        return true;
    }
    catch_bool_macro
}

RRStringArrayPtr rrcCallConv getCompartmentIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return createList(rri->getCompartmentIds());
    }
    catch_ptr_macro
}

bool rrcCallConv getRateOfChange(RRHandle handle, const int index, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getRateOfChange(index);
        return true;
    }
    catch_bool_macro
}

char* rrcCallConv getBuildDate()
{
    return rr::createText(__DATE__);
}

char* rrcCallConv getBuildTime()
{
    return rr::createText(__TIME__);
}

char* rrcCallConv getBuildDateTime()
{
    return rr::createText(string(__DATE__) + string(" ") + string(__TIME__));
}

bool rrcCallConv freeRRInstance(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        delete rri;
        rri = NULL;
        return true;
    }
    catch_bool_macro
}



//  Help("Returns the Symbols of all Flux Control Coefficients.")
NewArrayList sel_getFluxControlCoefficientIds(RoadRunner* rr)
{
    NewArrayList oResult;
    vector<string> oReactions       = rr->getReactionIds();
    vector<string> oParameters      = rr->getGlobalParameterIds();
    vector<string> oBoundary        = rr->getBoundarySpeciesIds();
    vector<string> oConservation    = rr->getConservedSumIds();

    for(int i = 0; i < oReactions.size(); i++)
    {
        string s = oReactions[i];

        NewArrayList oCCReaction;
        StringList oInner;
        oCCReaction.Add(s);

        for(int i = 0; i < oParameters.size(); i++)
        {
            oInner.add("cc(" + s + "," + oParameters[i] + ")");
        }

        for(int i = 0; i < oBoundary.size(); i++)
        {
            oInner.add("cc(" + s + "," + oBoundary[i] + ")");
        }

        for(int i = 0; i < oConservation.size(); i++)
        {
            oInner.add("cc(" + s + "," + oConservation[i] + ")");
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

// Help(
//            "Returns symbols of the currently loaded model, that can be used for steady state analysis. format: array of arrays  { { \"groupname\", { \"item1\", \"item2\" ... } } }  or { { \"groupname\", { \"subgroup\", { \"item1\" ... } } } }."
//            )
NewArrayList sel_getAvailableSteadyStateSymbols(RoadRunner* rr)
{
    NewArrayList oResult;

    oResult.Add("Floating Species",                                 sel_getFloatingSpeciesConcSymbols(rr) );
    oResult.Add("Boundary Species",                                 sel_getBoundarySpeciesConcSymbols(rr) );
    oResult.Add("Floating Species (amount)",                        rr->getFloatingSpeciesIds());
    oResult.Add("Boundary Species (amount)",                        rr->getBoundarySpeciesIds());
    oResult.Add("Global Parameters",                                rr->getGlobalParameterIds());
    oResult.Add("Volumes",                                          rr->getCompartmentIds());
    oResult.Add("Fluxes",                                           rr->getReactionIds());
    oResult.Add("Flux Control Coefficients",                        sel_getFluxControlCoefficientIds(rr) );
    oResult.Add("Concentration Control Coefficients",               sel_getConcentrationControlCoefficientIds(rr) );
    oResult.Add("Unscaled Concentration Control Coefficients",      sel_getUnscaledConcentrationControlCoefficientIds(rr));
    oResult.Add("Elasticity Coefficients",                          sel_getElasticityCoefficientIds(rr) );
    oResult.Add("Unscaled Elasticity Coefficients",                 sel_getUnscaledElasticityCoefficientIds(rr) );
    oResult.Add("Eigenvalues",                                      rr->getEigenvalueIds() );

    return oResult;
}

NewArrayList sel_getAvailableTimeCourseSymbols(RoadRunner* rr)
{
    NewArrayList oResult;

    oResult.Add("Floating Species",                 sel_getFloatingSpeciesConcSymbols(rr) );
    oResult.Add("Boundary Species",                 sel_getBoundarySpeciesConcSymbols(rr) );
    oResult.Add("Floating Species (amount)",        rr->getFloatingSpeciesIds() );
    oResult.Add("Boundary Species (amount)",        rr->getBoundarySpeciesIds() );
    oResult.Add("Global Parameters",                rr->getGlobalParameterIds() );
    oResult.Add("Fluxes",                           rr->getReactionIds() );
    oResult.Add("Rates of Change",                  rr->getRateOfChangeIds() );
    oResult.Add("Volumes",                          rr->getCompartmentIds() );
    oResult.Add("Elasticity Coefficients",          sel_getElasticityCoefficientIds(rr) );
    oResult.Add("Unscaled Elasticity Coefficients", sel_getUnscaledElasticityCoefficientIds(rr) );
    oResult.Add("Eigenvalues",                      rr->getEigenvalueIds() );
    return oResult;
}


// Help("Gets the list of boundary species amount names")
vector<string> sel_getBoundarySpeciesAmountIds(RoadRunner* rr)
{
    vector<string> result;// = new ArrayList();
    vector<string> list = rr->getBoundarySpeciesIds();
//    foreach (string s in getBoundarySpeciesIds()) oResult.add("[" + s + "]");
    for(int item = 0; item < list.size(); item++)// (object item in floatingSpeciesNames)
    {
        result.push_back(format("{0}", list[item]));
    }

    return result;
}

vector<string> sel_getBoundarySpeciesConcIds(RoadRunner* rr)
{
    vector<string> result;// = new ArrayList();
    vector<string> list = rr->getBoundarySpeciesIds();
//    foreach (string s in getBoundarySpeciesIds()) oResult.add("[" + s + "]");
    for(int item = 0; item < list.size(); item++)// (object item in floatingSpeciesNames)
    {
        result.push_back(format("[{0}]", list[item]));
    }

    return result;
}



//  Help("Returns the Symbols of all Unscaled Flux Control Coefficients.")


// Help("Returns the Symbols of all Concentration Control Coefficients.")
NewArrayList sel_getConcentrationControlCoefficientIds(RoadRunner* rr)
{
    NewArrayList oResult;// = new ArrayList();


    vector<string> oFloating        = rr->getFloatingSpeciesIds();
    vector<string> oParameters      = rr->getGlobalParameterIds();
    vector<string> oBoundary        = rr->getBoundarySpeciesIds();
    vector<string> oConservation    = rr->getConservedSumIds();

    for(int i = 0; i < oFloating.size(); i++)
    {
        string s = oFloating[i];
        NewArrayList oCCFloating;
        StringList oInner;
        oCCFloating.Add(s);

        for(int i = 0; i < oParameters.size(); i++)
        {
            oInner.add("CC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.size(); i++)
        {
            oInner.add("CC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.size(); i++)
        {
            oInner.add("CC:" + s + "," + oConservation[i]);
        }

        oCCFloating.Add(oInner);
        oResult.Add(oCCFloating);
    }

    return oResult;
}

// Help("Returns the Symbols of all Unscaled Concentration Control Coefficients.")
NewArrayList sel_getUnscaledConcentrationControlCoefficientIds(RoadRunner* rr)
{
    NewArrayList oResult;


    vector<string> oFloating        = rr->getFloatingSpeciesIds();
    vector<string> oParameters      = rr->getGlobalParameterIds();
    vector<string> oBoundary        = rr->getBoundarySpeciesIds();
    vector<string> oConservation    = rr->getConservedSumIds();

    for(int i = 0; i < oFloating.size(); i++)
    {
        string s = oFloating[i];
        NewArrayList oCCFloating;
        vector<string> oInner;
        oCCFloating.Add(s);

        for(int i = 0; i < oParameters.size(); i++)
        {
            oInner.push_back("uCC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.size(); i++)
        {
            oInner.push_back("uCC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.size(); i++)
        {
            oInner.push_back("uCC:" + s + "," + oConservation[i]);
        }

        oCCFloating.Add(oInner);
        oResult.Add(oCCFloating);
    }

    return oResult;
}

// Help("Returns the Symbols of all Elasticity Coefficients.")
NewArrayList sel_getElasticityCoefficientIds(RoadRunner* rr)
{
    NewArrayList oResult;


    vector<string> reactionNames        = rr->getReactionIds();
    vector<string> floatingSpeciesNames = rr->getFloatingSpeciesIds();
    vector<string> boundarySpeciesNames = rr->getBoundarySpeciesIds();
    vector<string> conservationNames    = rr->getConservedSumIds();
    vector<string> globalParameterNames = rr->getGlobalParameterIds();

    for(int i = 0; i < reactionNames.size(); i++)
    {
        string reac_name = reactionNames[i];
        NewArrayList oCCReaction;
        oCCReaction.Add(reac_name);
        StringList oInner;

        for(int j = 0; j < floatingSpeciesNames.size(); j++)
        {
            oInner.add(format("EE:{0},{1}", reac_name, floatingSpeciesNames[j]));
        }

        for(int j = 0; j < boundarySpeciesNames.size(); j++)
        {
            oInner.add(format("EE:{0},{1}", reac_name, boundarySpeciesNames[j]));
        }

        for(int j = 0; j < globalParameterNames.size(); j++)
        {
            oInner.add(format("EE:{0},{1}", reac_name, globalParameterNames[j]));
        }

        for(int j = 0; j < conservationNames.size(); j++)
        {
            oInner.add(format("EE:{0},{1}", reac_name, conservationNames[j]));
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

// Help("Returns the Symbols of all Unscaled Elasticity Coefficients.")
NewArrayList sel_getUnscaledElasticityCoefficientIds(RoadRunner* rr)
{
    NewArrayList oResult;

    vector<string> oReactions( rr->getReactionIds() );
    vector<string> oFloating = rr->getFloatingSpeciesIds();
    vector<string> oBoundary = rr->getBoundarySpeciesIds();
    vector<string> oGlobalParameters = rr->getGlobalParameterIds();
    vector<string> oConservation = rr->getConservedSumIds();

    for(int i = 0; i < oReactions.size(); i++)
    {
        string reac_name = oReactions[i];
        NewArrayList oCCReaction;
        StringList oInner;
        oCCReaction.Add(reac_name);

        for(int j = 0; j < oFloating.size(); j++)
        {
            string variable = oFloating[j];
            oInner.add(format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oBoundary.size(); j++)
        {
            string variable = oBoundary[j];
            oInner.add(format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oGlobalParameters.size(); j++)
        {
            string variable = oGlobalParameters[j];
            oInner.add(format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oConservation.size(); j++)
        {
            string variable = oConservation[j];
            oInner.add(format("uEE:{0},{1}", reac_name, variable));
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}


vector<string> sel_getFloatingSpeciesConcSymbols(RoadRunner* rr)
{
    vector<string> ids = rr->getFloatingSpeciesIds();
    vector<string> result;

    for(int i = 0; i < ids.size(); i++)
    {
        result.push_back("[" + ids[i] + "]");
    }

    return result;
}


vector<string> sel_getBoundarySpeciesConcSymbols(RoadRunner* rr)
{
    vector<string> ids = rr->getBoundarySpeciesIds();
    vector<string> result;

    for(int i = 0; i < ids.size(); i++)
    {
        result.push_back("[" + ids[i] + "]");
    }

    return result;
}

}

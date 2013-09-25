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
#include "rrCapability.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
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
            Log(lError)<<msg.str();
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
        rri->computeAndAssignConservationLaws(OnOrOff);
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
        if(!rri->loadSBMLFromFile(fileName))
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
                opt.modelGeneratorOpt | LoadSBMLOptions::ForceReCompile :
                opt.modelGeneratorOpt & ~LoadSBMLOptions::ForceReCompile;

        if(!rri->loadSBMLFromFile(fileName, &opt))
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
        return rri->loadSBML(sbml);
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
                        opt.modelGeneratorOpt | LoadSBMLOptions::ForceReCompile :
                        opt.modelGeneratorOpt & ~LoadSBMLOptions::ForceReCompile;

        if(!rri->loadSBML(sbml, &opt))
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
        if(!rri->setSimulateOptions(SimulateOptions(fileName)))
        {
            setError("Failed to load simulation settings");
            return false;
        }
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
        SimulateOptions opt = rri->getSimulateOptions();
        opt.start = timeStart;
        rri->setSimulateOptions(opt);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setTimeEnd(RRHandle handle, const double timeEnd)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        SimulateOptions opt = rri->getSimulateOptions();
        opt.duration = timeEnd - opt.start;
        rri->setSimulateOptions(opt);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setNumPoints(RRHandle handle, const int nrPoints)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        SimulateOptions opt = rri->getSimulateOptions();
        opt.steps = nrPoints - 1;
        rri->setSimulateOptions(opt);
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
        *numPoints = rri->getSimulateOptions().steps;
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setTimeCourseSelectionList(RRHandle handle, const char* list)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setTimeCourseSelectionList(list);
        return true;
    }
    catch_bool_macro
}


RRStringArrayPtr rrcCallConv getTimeCourseSelectionList(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList sNames = rri->getTimeCourseSelectionList();

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

        SimulateOptions options;
        options.start = rri->getSimulateOptions().start;
        options.duration = rri->getSimulateOptions().duration;
        options.steps = rri->getSimulateOptions().steps;
        options.flags = options.flags | SimulateOptions::ResetModel;

        if(!rri->simulate(&options))
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
        DoubleMatrix tempMat = rri->getScaledReorderedElasticityMatrix();
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
        DoubleMatrix tempMat = rri->getStoichiometryMatrix();

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
        *value  =  rri->getFloatingSpeciesInitialConcentrationByIndex(index);
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
        NewArrayList slSymbols = rri->getAvailableTimeCourseSymbols();
        return createArrayList(slSymbols);
    }
    catch_ptr_macro
}

RRListPtr rrcCallConv getAvailableSteadyStateSymbols(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList slSymbols = rri->getAvailableSteadyStateSymbols();
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
        rri->setSteadyStateSelectionList(aList);
        return true;
    }
    catch_bool_macro
}

RRStringArrayPtr rrcCallConv getSteadyStateSelectionList(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList sNames = rri->getSteadyStateSelectionList();

        if(sNames.Count() == 0)
        {
            return NULL;
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
        vector<double> tempList = rrc::createVector(vec);
        tempList = rri->getReactionRatesEx(tempList);
        return rrc::createVector(tempList);;
    }
    catch_ptr_macro
}

RRListPtr rrcCallConv getElasticityCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList aList = rri->getElasticityCoefficientIds();
        RRListPtr bList = createArrayList(aList);
        return bList;
    }
    catch_ptr_macro
}

bool rrcCallConv RoadRunner_setConfigurationXML(RRHandle handle, const char* caps)
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

char* rrcCallConv RoadRunner_getConfigurationXML(RRHandle handle)
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
        return createArrayList(rri->getFluxControlCoefficientIds());
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

RRListPtr rrcCallConv getUnscaledFluxControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList arrList = rri->getUnscaledFluxControlCoefficientIds();
        return createArrayList(arrList);
    }
    catch_ptr_macro
}

RRList* rrcCallConv getConcentrationControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList list = rri->getConcentrationControlCoefficientIds();
        return createArrayList(list);
    }
    catch_ptr_macro
}

RRListPtr rrcCallConv getUnscaledConcentrationControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return createArrayList(rri->getUnscaledConcentrationControlCoefficientIds());
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

}
//We only need to give the linker the folder where libs are
//using the pragma comment. Works for MSVC and codegear
#if defined(CG_IDE)

#if defined(STATIC_RR)
    #pragma comment(lib, "roadrunner-static.lib")
#else
    #pragma comment(lib, "roadrunner.lib")
#endif

#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "pugi-static.lib")
#pragma comment(lib, "libsbml-static.lib")
#pragma comment(lib, "sundials_cvode.lib")
#pragma comment(lib, "sundials_nvecserial.lib")
#pragma comment(lib, "libxml2_xe.lib")
#pragma comment(lib, "blas.lib")
#pragma comment(lib, "lapack.lib")
#pragma comment(lib, "libf2c.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "nleq-static.lib")
#endif


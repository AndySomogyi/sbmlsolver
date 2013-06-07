/**
 * @file rrc_api.cpp
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
#include "rrCModelGenerator.h"
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

#if defined(_WIN32) || defined(WIN32)
        string compiler(joinPath(rrInstallFolder,"compilers\\tcc\\tcc.exe"));
#elif defined(__linux)
        string compiler("gcc");
#else
        string compiler("gcc");
#endif
    //RoadRunner(const string& tempFolder, const string& supportCodeFolder, const string& compiler)
            return new RoadRunner(getUsersTempDataFolder(), joinPath(rrInstallFolder, "rr_support"), compiler);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRHandle rrcCallConv createRRInstanceEx(const char* tempFolder)
{
    try
    {
        char* text1 = getRRCAPILocation();
        string text2 = getParentFolder(text1);
        string rrInstallFolder(text2);
        freeText(text1);

#if defined(_WIN32) || defined(WIN32)
            string compiler(joinPath(rrInstallFolder, "compilers\\tcc\\tcc.exe"));
#elif defined(__linux)
            string compiler("gcc");
#else
            string compiler("gcc");
#endif
        if(tempFolder != NULL && !fileExists(tempFolder))
        {
            stringstream msg;
            msg<<"The temporary folder: "<<tempFolder<<" do not exist";
            Log(lError)<<msg.str();
            throw(Exception(msg.str()));
        }
        else if(tempFolder)
        {
            return new RoadRunner(tempFolder, joinPath(rrInstallFolder, "rr_support"), compiler);
        }
        else
        {
            return new RoadRunner(getUsersTempDataFolder(), joinPath(rrInstallFolder, "rr_support"), compiler);
        }
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRInstanceListHandle rrcCallConv createRRInstances(int count)
{
    try
    {
        string tempFolder = getUsersTempDataFolder();

        RoadRunnerList* listHandle = new RoadRunnerList(count, tempFolder);

        //Create the C list structure
        RRInstanceListHandle rrList = new RRInstanceList;
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

bool rrcCallConv freeRRInstances(RRInstanceListHandle rrList)
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

char* rrcCallConv getAPIVersion()
{
    try
    {
        return createText("0.99");
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

char* rrcCallConv getCPPAPIVersion()
{
    try
    {
        RRHandle handle = createRRInstance();
        if(!handle)
        {
            return NULL;
        }

        RoadRunner* rri = castFrom(handle);
        char* text = createText(rri->getVersion());
        freeRRInstance(handle);

        return text;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
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
        char* text = createText(aPath);
        return text;
    }
    return NULL;
#else
    return createText(joinPath(getInstallFolder(),"/lib"));
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
        char* text = createText(rri->getCopyright());
        freeRRInstance(handle);

        return text;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

char* rrcCallConv getInfo(RRHandle handle)
{
    try
    {
           RoadRunner* rri = castFrom(handle);
        char* text = NULL;
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }
        else
        {
            text = createText(rri->getInfo());
        }
        return text;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

char* rrcCallConv getExtendedAPIInfo()
{
    try
    {
        RRHandle handle = createRRInstance();
        if(!handle)
        {
            return NULL;
        }

           RoadRunner* rri = castFrom(handle);
        char* text = createText(rri->getExtendedVersionInfo());
        freeRRInstance(handle);
        return text;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

char* rrcCallConv getlibSBMLVersion(RRHandle handle)
{
    try
    {
           RoadRunner* rri = castFrom(handle);
        char* text = NULL;
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }
        else
        {
            text = createText(rri->getlibSBMLVersion());
        }
        return text;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

char* rrcCallConv getCurrentSBML(RRHandle handle)
{
    try
    {
           RoadRunner* rri = castFrom(handle);
        char* text = NULL;
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }
        else
        {
            text = createText(rri->writeSBML());
        }
        return text;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
     }
}

bool rrcCallConv setTempFolder(RRHandle handle, const char* folder)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->setTempFileFolder(folder);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

char* rrcCallConv getTempFolder(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return createText(rri->getTempFolder());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

bool rrcCallConv setCompiler(RRHandle handle, const char* fName)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        if(rri->getCompiler())
        {
            return rri->getCompiler()->setCompiler(fName);
        }
        else
        {
            return false;
        }
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrcCallConv setCompilerLocation(RRHandle handle, const char* folder)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        if(rri->getCompiler())
        {
            return rri->getCompiler()->setCompilerLocation(folder);
        }
        else
        {
            return false;
        }
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

char* rrcCallConv getCompilerLocation(RRHandle handle)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        return createText(rri->getCompiler()->getCompilerLocation());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

bool rrcCallConv setSupportCodeFolder(RRHandle handle,const char* folder)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        if(rri->getCompiler())
        {
            return rri->getCompiler()->setSupportCodeFolder(folder);
        }
        else
        {
            return false;
        }
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

char* rrcCallConv getSupportCodeFolder(RRHandle handle)
{
    try
    {
          RoadRunner* rri = castFrom(handle);


        return createText(rri->getCompiler()->getSupportCodeFolder());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}


char* rrcCallConv getWorkingDirectory()
{
    try
    {
        return createText(rr::getCWD());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
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
        if(!rri->loadSBMLFromFile(fileName, forceRecompile))
        {
            setError("Failed to load SBML semantics");    //There are many ways loading a model can fail, look at logFile to know more
            return false;
        }
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrcCallConv loadSBML(RRHandle handle, const char* sbml)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        return rri->loadSBML(sbml, true);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv loadSBMLE(RRHandle handle, const char* sbml, bool forceRecompilation)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        if(!rri->loadSBML(sbml, forceRecompilation))
        {
            setError("Failed to load SBML semantics");
            return false;
        }
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
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

        if(!rri->loadSimulationSettings(fileName))
        {
            setError("Failed to load simulation settings");
            return false;
        }
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

char* rrcCallConv getSBML(RRHandle handle)
{
    try
    {
          RoadRunner* rri = castFrom(handle);
        return createText(rri->getSBML());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return NULL;
    }
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
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        return rri->unLoadModel();
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return NULL;
    }
}

bool rrcCallConv setTimeStart(RRHandle handle, const double timeStart)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setTimeStart(timeStart);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrcCallConv setTimeEnd(RRHandle handle, const double timeEnd)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setTimeEnd(timeEnd);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrcCallConv setNumPoints(RRHandle handle, const int nrPoints)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setNumPoints(nrPoints);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv getTimeStart(RRHandle handle, double* timeStart)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        *timeStart = rri->getTimeStart();
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
      return false;
}

bool rrcCallConv getTimeEnd(RRHandle handle, double* timeEnd)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *timeEnd = rri->getTimeEnd();
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv getNumPoints(RRHandle handle, int* numPoints)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *numPoints = rri->getNumPoints();
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv setTimeCourseSelectionList(RRHandle handle, const char* list)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setTimeCourseSelectionList(list);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrcCallConv createTimeCourseSelectionList(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        return rri->createTimeCourseSelectionList() > 0 ? true : false;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

RRStringArrayHandle rrcCallConv getTimeCourseSelectionList(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        StringList sNames = rri->getTimeCourseSelectionList();

        if(!sNames.Count())
        {
            return NULL;
        }

        return createList(sNames);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }

}

RRDataHandle rrcCallConv simulate(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);

        if(!rri->simulate2Ex(rri->getTimeStart(), rri->getTimeEnd(), rri->getNumPoints()))
        {
            return NULL;
        }

        RoadRunnerData result = rri->getSimulationResult();

        //Extract the data and return struct..
        RRData* aResult  = createRRData(result);
        return aResult;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRDataHandle rrcCallConv getSimulationResult(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        RoadRunnerData result = rri->getSimulationResult();

        //Extract the data and return struct..

        RRData* aResult  = createRRData(result);
        return aResult;
    }
    catch_ptr_macro
}


RRDataHandle rrcCallConv simulateEx(RRHandle handle, const double timeStart, const double timeEnd, const int numberOfPoints)
{
    try
    {
        setTimeStart(handle, timeStart);
        setTimeEnd (handle, timeEnd);
        setNumPoints(handle, numberOfPoints);
          return simulate(handle);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRStringArrayHandle rrcCallConv getReactionIds(RRHandle handle)
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRVectorHandle rrcCallConv getRatesOfChange(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        vector<double> rates = rri->getRatesOfChange();

        if(!rates.size())
        {
            return NULL;
        }

        RRVector* list = new RRVector;
        list->Count = rates.size();
        list->Data = new double[list->Count];

        for(int i = 0; i < list->Count; i++)
        {
            list->Data[i] = rates[i];
        }
        return list;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRStringArrayHandle rrcCallConv getRatesOfChangeIds(RRHandle handle)
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRMatrixHandle rrcCallConv getUnscaledElasticityMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        DoubleMatrix tempMat = rri->getUnscaledElasticityMatrix();

        RRMatrixHandle matrix = createMatrix(&tempMat);
        return matrix;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getScaledElasticityMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        DoubleMatrix tempMat = rri->getScaledReorderedElasticityMatrix();


        RRMatrixHandle matrix = createMatrix(&tempMat);
        return matrix;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

bool rrcCallConv getValue(RRHandle handle, const char* symbolId, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getValue(symbolId);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrcCallConv setValue(RRHandle handle, const char* symbolId, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->setValue(symbolId, value);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}


RRMatrixHandle rrcCallConv getStoichiometryMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        DoubleMatrix tempMat = rri->getStoichiometryMatrix();

        RRMatrixHandle matrix = new RRMatrix;
        matrix->RSize = tempMat.RSize();
        matrix->CSize = tempMat.CSize();
        matrix->Data =  new double[tempMat.RSize()*tempMat.CSize()];

        int index = 0;
        for(rr::u_int row = 0; row < tempMat.RSize(); row++)
        {
            for(rr::u_int col = 0; col < tempMat.CSize(); col++)
            {
                matrix->Data[index++] = tempMat(row,col);
            }
        }
        return matrix;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getConservationMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        DoubleMatrix tempMat = rri->getConservationMatrix();

        RRMatrixHandle matrix = new RRMatrix;
        matrix->RSize = tempMat.RSize();
        matrix->CSize = tempMat.CSize();
        matrix->Data =  new double[tempMat.RSize()*tempMat.CSize()];

        int index = 0;
        for(rr::u_int row = 0; row < tempMat.RSize(); row++)
        {
            for(rr::u_int col = 0; col < tempMat.CSize(); col++)
            {
                matrix->Data[index++] = tempMat(row,col);
            }
        }
        return matrix;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getLinkMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        DoubleMatrix *tempMat = rri->getLinkMatrix();

        return createMatrix(tempMat);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}



RRMatrixHandle rrcCallConv getNrMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
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
        gLastError = createText("No Error");
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

int rrcCallConv getNumberOfReactions(RRHandle handle)
{
     try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
           setError(ALLOCATE_API_ERROR_MSG);
           return -1;
        }
        return rri->getNumberOfReactions();
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return -1;
    }
}

bool rrcCallConv getReactionRate(RRHandle handle, const int rateNr, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getReactionRate(rateNr);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

RRVectorHandle rrcCallConv getReactionRates(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);

        vector<double> vec =  rri->getReactionRates();

        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

int rrcCallConv getNumberOfBoundarySpecies(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }
        return rri->getNumberOfBoundarySpecies();
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return -1;
    }
}

RRStringArrayHandle rrcCallConv getBoundarySpeciesIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        StringList bNames = rri->getBoundarySpeciesIds();

        if(!bNames.Count())
        {
            return NULL;
        }

        return createList(bNames);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
           return NULL;
    }
}

int rrcCallConv getNumberOfFloatingSpecies(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }
        return rri->getNumberOfFloatingSpecies();
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
           return -1;
    }
}

RRStringArrayHandle rrcCallConv getFloatingSpeciesIds(RRHandle handle)
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

int rrcCallConv getNumberOfGlobalParameters(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }
        return rri->getNumberOfGlobalParameters();
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
           return -1;
    }
}

RRStringArrayHandle rrcCallConv getGlobalParameterIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        StringList pNames = rri->getGlobalParameterIds();

        if(!pNames.Count())
        {
            return NULL;
        }

        return createList(pNames);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
           return NULL;
    }
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

RRVectorHandle rrcCallConv getFloatingSpeciesConcentrations(RRHandle handle)
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

RRVectorHandle rrcCallConv getBoundarySpeciesConcentrations(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        vector<double> vec =  rri->getBoundarySpeciesConcentrations();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}


RRVectorHandle rrcCallConv getFloatingSpeciesInitialConcentrations(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        vector<double> vec =  rri->getFloatingSpeciesInitialConcentrations();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

bool rrcCallConv setFloatingSpeciesByIndex (RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setFloatingSpeciesByIndex(index, value);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv setBoundarySpeciesByIndex (RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setBoundarySpeciesByIndex(index, value);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
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

bool rrcCallConv setFloatingSpeciesInitialConcentrations(RRHandle handle, const struct RRVector* vec)
{
    try
    {
        vector<double> tempVec;
        copyVector(vec, tempVec);
        RoadRunner* rri = castFrom(handle);
        rri->changeInitialConditions(tempVec);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv setFloatingSpeciesConcentrations(RRHandle handle, const struct RRVector* vec)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        vector<double> tempVec;
        copyVector(vec, tempVec);
        rri->setFloatingSpeciesConcentrations(tempVec);

        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv setBoundarySpeciesConcentrations(RRHandle handle, const struct RRVector* vec)
{
    try
    {
        vector<double> tempVec;
        copyVector(vec, tempVec);
        RoadRunner* rri = castFrom(handle);
        rri->setBoundarySpeciesConcentrations(tempVec);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv oneStep(RRHandle handle, const double currentTime, const double stepSize, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->oneStep(currentTime, stepSize);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

RRVectorHandle rrcCallConv getGlobalParameterValues(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> vec =  rri->getGlobalParameterValues();
        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRListHandle rrcCallConv getAvailableTimeCourseSymbols(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList slSymbols = rri->getAvailableTimeCourseSymbols();
        return createArrayList(slSymbols);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRListHandle rrcCallConv getAvailableSteadyStateSymbols(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList slSymbols = rri->getAvailableSteadyStateSymbols();
        return createArrayList(slSymbols);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

bool rrcCallConv getBoundarySpeciesByIndex (RRHandle handle, const int index, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getBoundarySpeciesByIndex(index);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv getFloatingSpeciesByIndex (RRHandle handle, const int index, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getFloatingSpeciesByIndex(index);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv getGlobalParameterByIndex (RRHandle handle, const int index, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getGlobalParameterByIndex(index);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv getuCC (RRHandle handle, const char* variable, const char* parameter, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        *value = rri->getuCC(variable, parameter);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}


bool rrcCallConv getCC (RRHandle handle, const char* variable, const char* parameter, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getCC(variable, parameter);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv getuEE(RRHandle handle, const char* name, const char* species, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getuEE(name, species);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv getEE(RRHandle handle, const char* name, const char* species, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getEE(name, species);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

int rrcCallConv getNumberOfDependentSpecies(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }

        return rri->getNumberOfDependentSpecies();
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return -1;
    }
}

int rrcCallConv getNumberOfIndependentSpecies(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }

        return rri->getNumberOfIndependentSpecies();
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return -1;
    }
}


bool rrcCallConv steadyState(RRHandle handle, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
           *value = rri->steadyState();
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv evalModel(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        rri->evalModel();
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

char* rrcCallConv getParamPromotedSBML(RRHandle handle, const char* sArg)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        string param =  rri->getParamPromotedSBML(sArg);

        char* text = createText(param.c_str());
        return text;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRVectorHandle rrcCallConv computeSteadyStateValues(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        vector<double> vec =  rri->computeSteadyStateValues();

        RRVector* aVec = rrc::createVector(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

RRStringArrayHandle rrcCallConv getSteadyStateSelectionList(RRHandle handle)
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getFullJacobian(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix tempMat = rri->getFullJacobian();
        return createMatrix(&tempMat);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getReducedJacobian(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        DoubleMatrix tempMat = rri->getReducedJacobian();
        return createMatrix(&tempMat);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getEigenvalues(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix tempMat = rri->getEigenvalues();
        return createMatrix(&tempMat);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

char* rrcCallConv getCSourceFileName(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        CModelGenerator* generator = dynamic_cast<CModelGenerator*>(rri->getModelGenerator());
        if(!generator)
        {
            return NULL;
        }

        string fNameS = generator->getSourceCodeFileName();

        fNameS = getFileNameNoExtension(fNameS);
        return createText(fNameS);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
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
            cCode->Header = createText(header);
        }

        if(source.size())
        {
            cCode->Source = createText(source);
        }
        return cCode;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

RRStringArrayHandle rrcCallConv getFloatingSpeciesInitialConditionIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList aList = rri->getFloatingSpeciesInitialConditionIds();
        return createList(aList);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRVectorHandle rrcCallConv getRatesOfChangeEx(RRHandle handle, const RRVectorHandle vec)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        vector<double> tempList = rrc::createVector(vec);
        tempList = rri->getRatesOfChangeEx(tempList);
        return rrc::createVector (tempList);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRVectorHandle rrcCallConv getReactionRatesEx(RRHandle handle, const RRVectorHandle vec)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> tempList = rrc::createVector(vec);
        tempList = rri->getReactionRatesEx(tempList);
        return rrc::createVector(tempList);;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRListHandle rrcCallConv getElasticityCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        NewArrayList aList = rri->getElasticityCoefficientIds();
        RRListHandle bList = createArrayList(aList);
        return bList;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

bool rrcCallConv setCapabilities(RRHandle handle, const char* caps)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!caps)
        {
            return false;
        }
        rri->setCapabilities(caps);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

char* rrcCallConv getCapabilities(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return createText(rri->getCapabilitiesAsXML());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRStringArrayHandle rrcCallConv getListOfCapabilities(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        StringList list = rri->getListOfCapabilities();
        return createList(list);
    }
    catch_ptr_macro
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RRStringArrayHandle rrcCallConv getEigenvalueIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        StringList aList = rri->getEigenvalueIds();
        return createList(aList);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRListHandle rrcCallConv getFluxControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        return createArrayList(rri->getFluxControlCoefficientIds());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getUnscaledConcentrationControlCoefficientMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        DoubleMatrix aMat = rri->getUnscaledConcentrationControlCoefficientMatrix();
        //return createMatrix(&(rri->getUnscaledConcentrationControlCoefficientMatrix()));
        return createMatrix(&(aMat));
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getScaledConcentrationControlCoefficientMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        DoubleMatrix aMat = rri->getScaledConcentrationControlCoefficientMatrix();
        return createMatrix(&(aMat));
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getUnscaledFluxControlCoefficientMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        //return createMatrix(&(rri->getUnscaledFluxControlCoefficientMatrix()));
        DoubleMatrix aMat = rri->getUnscaledFluxControlCoefficientMatrix();
        return createMatrix(&(aMat));
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRMatrixHandle rrcCallConv getScaledFluxControlCoefficientMatrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        //return createMatrix(&(rri->getScaledFluxControlCoefficientMatrix()));a
        DoubleMatrix aMat = rri->getScaledFluxControlCoefficientMatrix();
        return createMatrix(&(aMat));
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRListHandle rrcCallConv getUnscaledFluxControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        NewArrayList arrList = rri->getUnscaledFluxControlCoefficientIds();
        return createArrayList(arrList);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRList* rrcCallConv getConcentrationControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        NewArrayList list = rri->getConcentrationControlCoefficientIds();
        return createArrayList(list);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRListHandle rrcCallConv getUnscaledConcentrationControlCoefficientIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        return createArrayList(rri->getUnscaledConcentrationControlCoefficientIds());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

int rrcCallConv getNumberOfCompartments(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }
        return rri->getNumberOfCompartments();
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return -1;
    }
}

bool rrcCallConv getCompartmentByIndex(RRHandle handle, const int index, double *value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        *value = rri->getCompartmentByIndex(index);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrcCallConv setCompartmentByIndex (RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setCompartmentByIndex(index, value);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

RRStringArrayHandle rrcCallConv getCompartmentIds(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        return createList(rri->getCompartmentIds());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

bool rrcCallConv getRateOfChange(RRHandle handle, const int index, double* value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        *value = rri->getRateOfChange(index);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

char* rrcCallConv getBuildDate()
{
    return createText(__DATE__);
}

char* rrcCallConv getBuildTime()
{
    return createText(__TIME__);
}

char* rrcCallConv getBuildDateTime()
{
    return createText(string(__DATE__) + string(" ") + string(__TIME__));
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
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


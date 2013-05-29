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
//#include "rrParameter.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerList.h"
#include "rrCModelGenerator.h"
#include "rrLogger.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrCapability.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#include "rrc_api.h" 		  // Need to include this before the support header..
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

#if defined(_WIN32)
#include <windef.h>
#define RR_MAX_PATH MAX_PATH
#elif defined(__unix__) || defined(__APPLE__)
#include <limits.h>
#define RR_MAX_PATH PATH_MAX
#endif

//---------------------------------------------------------------------------

namespace rrc
{
using namespace std;
using namespace rr;

RRHandle rrCallConv createRRInstance()
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

RRHandle rrCallConv createRRInstanceEx(const char* tempFolder)
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

RRInstanceListHandle rrCallConv createRRInstances(int count)
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

bool rrCallConv freeRRInstances(RRInstanceListHandle rrList)
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

char* rrCallConv getInstallFolder()
{
    if(!gInstallFolder[0])
    {
        strncpy(gInstallFolder, "/usr/local", RR_MAX_PATH);
    }
    return gInstallFolder;
}

bool rrCallConv setInstallFolder(const char* folder)
{
    try
    {
		gInstallFolder = new char[2048];
	    return strcpy(gInstallFolder, folder) != NULL ? true : false;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

char* rrCallConv getAPIVersion()
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

char* rrCallConv getCPPAPIVersion()
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

char* rrCallConv getRRCAPILocation()
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

char* rrCallConv getCopyright()
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

char* rrCallConv getInfo(RRHandle handle)
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

char* rrCallConv getExtendedAPIInfo()
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

char* rrCallConv getlibSBMLVersion(RRHandle handle)
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

char* rrCallConv getCurrentSBML(RRHandle handle)
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
bool rrCallConv setComputeAndAssignConservationLaws(RRHandle handle, const bool OnOrOff)
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

bool rrCallConv setTempFolder(RRHandle handle, const char* folder)
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

char* rrCallConv getTempFolder(RRHandle handle)
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

bool rrCallConv setCompiler(RRHandle handle, const char* fName)
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

bool rrCallConv setCompilerLocation(RRHandle handle, const char* folder)
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

char* rrCallConv getCompilerLocation(RRHandle handle)
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

bool rrCallConv setSupportCodeFolder(RRHandle handle,const char* folder)
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

char* rrCallConv getSupportCodeFolder(RRHandle handle)
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


char* rrCallConv getWorkingDirectory()
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

bool rrCallConv loadSBMLFromFile(RRHandle _handle, const char* fileName)
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

bool rrCallConv loadSBMLFromFileE(RRHandle _handle, const char* fileName, bool forceRecompile)
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

bool rrCallConv loadSBML(RRHandle handle, const char* sbml)
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

bool rrCallConv loadSBMLE(RRHandle handle, const char* sbml, bool forceRecompilation)
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


bool rrCallConv loadSimulationSettings(RRHandle handle, const char* fileName)
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

char* rrCallConv getSBML(RRHandle handle)
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

bool rrCallConv isModelLoaded(RRHandle handle)
{
	try
    {
      	RoadRunner* rri = castFrom(handle);
        return rri->isModelLoaded();
    }
    catch_bool_macro
}

bool rrCallConv unLoadModel(RRHandle handle)
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

bool rrCallConv setTimeStart(RRHandle handle, const double timeStart)
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

bool rrCallConv setTimeEnd(RRHandle handle, const double timeEnd)
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

bool rrCallConv setNumPoints(RRHandle handle, const int nrPoints)
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

bool rrCallConv getTimeStart(RRHandle handle, double* timeStart)
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

bool rrCallConv getTimeEnd(RRHandle handle, double* timeEnd)
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

bool rrCallConv getNumPoints(RRHandle handle, int* numPoints)
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

bool rrCallConv setTimeCourseSelectionList(RRHandle handle, const char* list)
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

bool rrCallConv createTimeCourseSelectionList(RRHandle handle)
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

RRStringArrayHandle rrCallConv getTimeCourseSelectionList(RRHandle handle)
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

RRDataHandle rrCallConv simulate(RRHandle handle)
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

RRDataHandle rrCallConv getSimulationResult(RRHandle handle)
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


RRDataHandle rrCallConv simulateEx(RRHandle handle, const double timeStart, const double timeEnd, const int numberOfPoints)
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

RRStringArrayHandle rrCallConv getReactionIds(RRHandle handle)
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

RRVectorHandle rrCallConv getRatesOfChange(RRHandle handle)
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

RRStringArrayHandle rrCallConv getRatesOfChangeIds(RRHandle handle)
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

RRMatrixHandle rrCallConv getUnscaledElasticityMatrix(RRHandle handle)
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

RRMatrixHandle rrCallConv getScaledElasticityMatrix(RRHandle handle)
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

bool rrCallConv getValue(RRHandle handle, const char* symbolId, double *value)
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

bool rrCallConv setValue(RRHandle handle, const char* symbolId, const double value)
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


RRMatrixHandle rrCallConv getStoichiometryMatrix(RRHandle handle)
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

RRMatrixHandle rrCallConv getConservationMatrix(RRHandle handle)
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

RRMatrixHandle rrCallConv getLinkMatrix(RRHandle handle)
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

RRMatrixHandle rrCallConv getL0Matrix(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);

        DoubleMatrix *tempMat = rri->getL0Matrix();

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

RRMatrixHandle rrCallConv getNrMatrix(RRHandle handle)
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
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

C_DECL_SPEC bool rrCallConv hasError()
{
    return (gLastError != NULL) ? true : false;
}

char* rrCallConv getLastError()
{
    if(!gLastError)
    {
        gLastError = createText("No Error");
    }
    return gLastError;
}

bool rrCallConv reset(RRHandle handle)
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

int rrCallConv getNumberOfReactions(RRHandle handle)
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

bool rrCallConv getReactionRate(RRHandle handle, const int rateNr, double* value)
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

RRVectorHandle rrCallConv getReactionRates(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);

        vector<double> vec =  rri->getReactionRates();

        RRVector* aVec = createVectorFromVector_double(vec);
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

int rrCallConv getNumberOfBoundarySpecies(RRHandle handle)
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

RRStringArrayHandle rrCallConv getBoundarySpeciesIds(RRHandle handle)
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

int rrCallConv getNumberOfFloatingSpecies(RRHandle handle)
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

RRStringArrayHandle rrCallConv getFloatingSpeciesIds(RRHandle handle)
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

int rrCallConv getNumberOfGlobalParameters(RRHandle handle)
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

RRStringArrayHandle rrCallConv getGlobalParameterIds(RRHandle handle)
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

RRVectorHandle rrCallConv getFloatingSpeciesConcentrations(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        vector<double> vec =  rri->getFloatingSpeciesConcentrations();
        RRVector* aVec = createVectorFromVector_double(vec);
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

RRVectorHandle rrCallConv getBoundarySpeciesConcentrations(RRHandle handle)
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
        RRVector* aVec = createVectorFromVector_double(vec);
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


RRVectorHandle rrCallConv getFloatingSpeciesInitialConcentrations(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);


        vector<double> vec =  rri->getFloatingSpeciesInitialConcentrations();
        RRVector* aVec = createVectorFromVector_double(vec);
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

bool rrCallConv setFloatingSpeciesByIndex (RRHandle handle, const int index, const double value)
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

bool rrCallConv setBoundarySpeciesByIndex (RRHandle handle, const int index, const double value)
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

bool rrCallConv setGlobalParameterByIndex(RRHandle handle, const int index, const double value)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        rri->setGlobalParameterByIndex(index, value);
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

bool rrCallConv setFloatingSpeciesInitialConcentrations(RRHandle handle, const RRVector* vec)
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

bool rrCallConv setFloatingSpeciesConcentrations(RRHandle handle, const RRVector* vec)
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

bool rrCallConv setBoundarySpeciesConcentrations(RRHandle handle, const RRVector* vec)
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

bool rrCallConv oneStep(RRHandle handle, const double currentTime, const double stepSize, double *value)
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

RRVectorHandle rrCallConv getGlobalParameterValues(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> vec =  rri->getGlobalParameterValues();
        RRVector* aVec = createVectorFromVector_double(vec);
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

RRListHandle rrCallConv getAvailableTimeCourseSymbols(RRHandle handle)
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

RRListHandle rrCallConv getAvailableSteadyStateSymbols(RRHandle handle)
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

bool rrCallConv getBoundarySpeciesByIndex (RRHandle handle, const int index, double* value)
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

bool rrCallConv getFloatingSpeciesByIndex (RRHandle handle, const int index, double *value)
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

bool rrCallConv getGlobalParameterByIndex (RRHandle handle, const int index, double *value)
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

bool rrCallConv getuCC (RRHandle handle, const char* variable, const char* parameter, double *value)
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


bool rrCallConv getCC (RRHandle handle, const char* variable, const char* parameter, double *value)
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

bool rrCallConv getuEE(RRHandle handle, const char* name, const char* species, double* value)
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

bool rrCallConv getEE(RRHandle handle, const char* name, const char* species, double *value)
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

int rrCallConv getNumberOfDependentSpecies(RRHandle handle)
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

int rrCallConv getNumberOfIndependentSpecies(RRHandle handle)
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


bool rrCallConv steadyState(RRHandle handle, double* value)
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

bool rrCallConv evalModel(RRHandle handle)
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

char* rrCallConv getParamPromotedSBML(RRHandle handle, const char* sArg)
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

RRVectorHandle rrCallConv computeSteadyStateValues(RRHandle handle)
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

        RRVector* aVec = createVectorFromVector_double(vec);
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

bool rrCallConv setSteadyStateSelectionList(RRHandle handle, const char* list)
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

RRStringArrayHandle rrCallConv getSteadyStateSelectionList(RRHandle handle)
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

RRMatrixHandle rrCallConv getFullJacobian(RRHandle handle)
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

RRMatrixHandle rrCallConv getReducedJacobian(RRHandle handle)
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

RRMatrixHandle rrCallConv getEigenvalues(RRHandle handle)
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

//Todo: this routine should not need a roadrunner handle
RRMatrixHandle rrCallConv getEigenvaluesMatrix (RRHandle handle, const RRMatrixHandle mat)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        if (mat == NULL) {
             stringstream msg;
            msg<<"RoadRunner exception: "<< "Matrix argument to getEigenvaluesMAtrix is NULL" <<endl;
            setError(msg.str());
            return NULL;
        }

        // Convert RRMatrixHandle mat to a DoubleMatrix
        DoubleMatrix dmat (mat->RSize, mat->CSize);
        double value;
        for (int i=0; i<mat->RSize; i++)
        {
            for (int j=0; j<mat->CSize; j++)
            {
                getMatrixElement (mat, i, j, &value);
                dmat(i,j) = value;
            }
        }
        DoubleMatrix tempMat = rri->getEigenvaluesFromMatrix (dmat);
        // Convert the DoubleMatrix result to a RRMatrixHandle type
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

char* rrCallConv getCSourceFileName(RRHandle handle)
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

RRCCode* rrCallConv getCCode(RRHandle handle)
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
bool rrCallConv setCodeGenerationMode (int _mode)
{
    return false;
}

//NOM forwarded functions
int rrCallConv getNumberOfRules(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri->getNOM())
        {
            Log(lWarning)<<"NOM is not allocated.";
            return -1;
        }
        int value = rri->getNOM()->getNumRules();
        return value;
    }
    catch_int_macro
}

char* rrcCallConv getModelName(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri->getNOM())
        {
            Log(lWarning)<<"NOM is not allocated.";
        	return NULL;
        }
        return createText(rri->getNOM()->getModelName());
    }
    catch_ptr_macro
}

bool rrCallConv getScaledFloatingSpeciesElasticity(RRHandle handle, const char* reactionId, const char* speciesId, double *value)
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

RRStringArrayHandle rrCallConv getFloatingSpeciesInitialConditionIds(RRHandle handle)
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

RRVectorHandle rrCallConv getRatesOfChangeEx(RRHandle handle, const RRVectorHandle vec)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        vector<double> tempList = createVectorFromRRVector(vec);
        tempList = rri->getRatesOfChangeEx(tempList);
        return createVectorFromVector_double (tempList);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRVectorHandle rrCallConv getReactionRatesEx(RRHandle handle, const RRVectorHandle vec)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        vector<double> tempList = createVectorFromRRVector(vec);
        tempList = rri->getReactionRatesEx(tempList);
        return createVectorFromVector_double(tempList);;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

RRListHandle rrCallConv getElasticityCoefficientIds(RRHandle handle)
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

bool rrCallConv setCapabilities(RRHandle handle, const char* caps)
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

char* rrCallConv getCapabilities(RRHandle handle)
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

RRStringArrayHandle rrCallConv getListOfCapabilities(RRHandle handle)
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
RRStringArrayHandle rrCallConv getEigenvalueIds(RRHandle handle)
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

RRListHandle rrCallConv getFluxControlCoefficientIds(RRHandle handle)
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

RRMatrixHandle rrCallConv getUnscaledConcentrationControlCoefficientMatrix(RRHandle handle)
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

RRMatrixHandle rrCallConv getScaledConcentrationControlCoefficientMatrix(RRHandle handle)
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

RRMatrixHandle rrCallConv getUnscaledFluxControlCoefficientMatrix(RRHandle handle)
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

RRMatrixHandle rrCallConv getScaledFluxControlCoefficientMatrix(RRHandle handle)
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

RRListHandle rrCallConv getUnscaledFluxControlCoefficientIds(RRHandle handle)
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

RRList* rrCallConv getConcentrationControlCoefficientIds(RRHandle handle)
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

RRListHandle rrCallConv getUnscaledConcentrationControlCoefficientIds(RRHandle handle)
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

int rrCallConv getNumberOfCompartments(RRHandle handle)
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

bool rrCallConv getCompartmentByIndex(RRHandle handle, const int index, double *value)
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

bool rrCallConv setCompartmentByIndex (RRHandle handle, const int index, const double value)
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

RRStringArrayHandle rrCallConv getCompartmentIds(RRHandle handle)
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

bool rrCallConv getRateOfChange(RRHandle handle, const int index, double* value)
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


// LOGGING ROUTINES
void rrCallConv logMsg(CLogLevel lvl, const char* msg)
{
    try
    {
        Log((LogLevel) lvl)<<msg;
    }
    catch(const Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
}

bool rrCallConv enableLoggingToConsole()
{
    try
    {
        LogOutput::mLogToConsole = true;
        return true;
    }
    catch(const Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrCallConv enableLoggingToFile(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        char* tempFolder = getTempFolder(handle);
		string logFile = joinPath(tempFolder, "RoadRunner.log") ;
        freeText(tempFolder);

        gLog.Init("", gLog.GetLogLevel(), new LogFile(logFile.c_str()));
        return true;
    }
    catch(const Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

char* rrCallConv testString (char* testStr)
{
    return testStr;
}

bool rrCallConv setLogLevel(const char* _lvl)
{
    try
    {
        LogLevel lvl = GetLogLevel(_lvl);
        gLog.SetCutOffLogLevel(lvl);
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

char* rrCallConv getLogLevel()
{
    try
    {
        string level = gLog.GetCurrentLogLevel();
        char* lvl = createText(level.c_str());
        return lvl;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return NULL;
    }
}

char* rrCallConv getLogFileName()
{
    try
    {
        return createText(gLog.GetLogFileName().c_str());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return NULL;
    }
}

char* rrCallConv getBuildDate()
{
    return createText(__DATE__);
}

char* rrCallConv getBuildTime()
{
    return createText(__TIME__);
}

char* rrCallConv getBuildDateTime()
{
    return createText(string(__DATE__) + string(" ") + string(__TIME__));
}

int rrCallConv getInstanceCount(RRInstanceListHandle iList)
{
    return iList->Count;
}

RRHandle rrCallConv getRRHandle(RRInstanceListHandle iList, int index)
{
    return iList->Handle[index];
}

bool rrCallConv freeRRInstance(RRHandle handle)
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


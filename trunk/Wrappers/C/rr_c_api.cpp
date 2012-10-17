/**
 * @file rr_c_api.cpp
 * @brief roadRunner C API 2012
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerwork/ for more details.
 *
 * Copyright (C) 2012
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

#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
//---------------------------------------------------------------------------
#include <windows.h>
#include <sstream>
#include "rrRoadRunner.h"
#include "rrCGenerator.h"
#include "rrLogger.h"           //Might be useful for debugging later on
#include "rr_c_api.h"
#include "rr_c_api_support.h"   //Support functions, not exposed as api functions and or data
#include "rrException.h"
#include "rr_version_info.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
//---------------------------------------------------------------------------

//We only need to give the linker the folder where libs are
//using the pragma comment. Works for MSVC and codegear

#if defined(CG_IDE)
#pragma comment(lib, "rr_c_api.lib")
#pragma comment(lib, "roadrunner-static.lib")
//#pragma comment(lib, "sundials_cvode.lib")
//#pragma comment(lib, "sundials_nvecserial.lib")
#pragma comment(lib, "pugi-static.lib")
//#pragma comment(lib, "nleq-static.lib")
//#pragma comment(lib, "rr-libstruct-static.lib")
//#pragma comment(lib, "libsbml-static.lib")
//#pragma comment(lib, "libxml2_xe.lib")
//#pragma comment(lib, "blas.lib")
//#pragma comment(lib, "lapack.lib")
//#pragma comment(lib, "libf2c.lib")
//#pragma comment(lib, "unit_test-static.lib")
#endif

using namespace std;
using namespace rr;
using namespace rr_c_api;
namespace rr_c_api
{
static  rr::RoadRunner*     gRRHandle       = NULL;
char*                       gLastError      = NULL;
}

bool rrCallConv enableLogging()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

	    LogOutput::mLogToConsole = true;
        char* tempFolder = getTempFolder();
		string logFile = JoinPath(tempFolder, "RoadRunner.log") ;
        freeText(tempFolder);
        gLog.Init("", gLog.GetLogLevel(), unique_ptr<LogFile>(new LogFile(logFile.c_str())));

        char* buffer = new char[1024];
        // Get the current working directory:
        if( (buffer = _getcwd( buffer, MAXPATH )) == NULL )
        {
            perror( "getcwd error" );
        }
        else
        {
            printf( "Current cwd = %s \nLength: %d\n", buffer, strlen(buffer) );
            Log(lInfo)<<"Current working folder is :"<<buffer;
            delete [] buffer;
        }

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

bool rrCallConv setLogLevel(const char* _lvl)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }
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
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        string level = gLog.GetCurrentLogLevel();
        char* lvl = createText(level.c_str());
    	return lvl;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return false;
    }
}

char* rrCallConv getLogFileName()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }
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
    char* date = new char[strlen(__DATE__) + 1];
    strcpy(date, __DATE__);
    return date;
}

char* rrCallConv getVersion()
{
    return createText(RR_VERSION);
}

RRHandle rrCallConv getRRInstance()
{
	try
    {
        if(!gRRHandle)
        {
            gRRHandle = new rr::RoadRunner();
        }
    	return gRRHandle;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
		return NULL;
    }
}

char* rrCallConv getCopyright()
{
	try
    {
        char* text = NULL;
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }
        else
        {
            text = new char[gRRHandle->getCopyright().size() + 1];
            strcpy(text, gRRHandle->getCopyright().c_str());
        }
        return text;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

char* rrCallConv getlibSBMLVersion()
{
	try
    {
        char* text = NULL;
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }
        else
        {
            string _text = gRRHandle->getlibSBMLVersion();
            text = new char[_text.size() + 1];
            strcpy(text, _text.c_str());
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

char* rrCallConv getCurrentSBML()
{
	try
    {
        char* text = NULL;
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }
        else
        {
            string sbml = gRRHandle->writeSBML();
            text = new char[sbml.size() + 1];
            strcpy(text, sbml.c_str());
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
bool rrCallConv setComputeAndAssignConservationLaws(const bool& OnOrOff)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        gRRHandle->ComputeAndAssignConservationLaws(OnOrOff);
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

bool rrCallConv setTempFolder(const char* folder)
{
	try
    {
    	if(!gRRHandle)
    	{
        	setError(ALLOCATE_API_ERROR_MSG);
        	return false;
    	}
	    return gRRHandle->SetTempFileFolder(folder);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
  	return false;
}

char* rrCallConv getTempFolder()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

	    char* text = new char[gRRHandle->GetTempFileFolder().size() + 1];
    	strcpy(text, gRRHandle->GetTempFileFolder().c_str());
	    return text;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

char* rrCallConv getWorkingDirectory()
{
	try
    {
		string cwd = rr::getWorkingDirectory();
	    char* text = new char[cwd.size() + 1];
    	strcpy(text, cwd.c_str());
	    return text;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

bool rrCallConv loadSBMLFromFile(const char* fileName)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        //Check if file exists first
        if(!FileExists(fileName))
        {
            stringstream msg;
            msg<<"The file "<<fileName<<" was not found";
            setError(msg.str());
            return false;
        }

        if(!gRRHandle->loadSBMLFromFile(fileName))
        {
            setError("Failed to load SBML semantics");	//There are many wasy loading a model can fail, look at logFile to know more
            return false;
        }
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

bool rrCallConv loadSBML(const char* sbml)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        if(!gRRHandle->loadSBML(sbml))
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
    }
  	return false;
}

char* rrCallConv getSBML()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        string sbml = gRRHandle->getSBML();

        return createText(sbml);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
  	return NULL;
}

bool rrCallConv setTimeStart(const double& timeStart)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        gRRHandle->setTimeStart(timeStart);
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

bool rrCallConv setTimeEnd(const double& timeEnd)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        gRRHandle->setTimeEnd(timeEnd);
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

bool rrCallConv setNumPoints(const int& nrPoints)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        gRRHandle->setNumPoints(nrPoints);
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

bool rrCallConv getTimeStart(double& timeStart)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

		timeStart = gRRHandle->getTimeStart();
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


bool rrCallConv getTimeEnd(double& timeEnd)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

		timeEnd = gRRHandle->getTimeEnd();
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


bool rrCallConv getNumPoints(int& numPoints)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

		numPoints = gRRHandle->getNumPoints();
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


bool  rrCallConv setTimeCourseSelectionList(const char* list)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        gRRHandle->setTimeCourseSelectionList(list);
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

RRStringArrayHandle rrCallConv getTimeCourseSelectionList()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        StringList sNames = gRRHandle->getTimeCourseSelectionList();

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
    }
    return NULL;
}

RRResultHandle rrCallConv simulate()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        if(!gRRHandle->Simulate())
        {
            return NULL;
        }

        SimulationData result = gRRHandle->GetSimulationResult();

        //Extract the data and return struct..
        RRResult* aResult  = new RRResult;
        aResult->ColumnHeaders = new char*[result.GetNrOfCols()];
        for(int i = 0; i < result.GetNrOfCols(); i++)
        {
            aResult->ColumnHeaders[i] = new char(32);
            strcpy(aResult->ColumnHeaders[i], result.GetColumnNames()[i].c_str());
        }

        aResult->RSize = result.GetNrOfRows();
        aResult->CSize = result.GetNrOfCols();
        int size = aResult->RSize*aResult->CSize;
        aResult->Data = new double[size];

        int index = 0;
        //The data layout is simple row after row, in one single long row...
        for(int row = 0; row < aResult->RSize; row++)
        {
            for(int col = 0; col < aResult->CSize; col++)
            {
                aResult->Data[index++] = result(row, col);
            }
        }
	    return aResult;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

RRResultHandle rrCallConv simulateEx (const double& timeStart, const double& timeEnd, const int& numberOfPoints)
{
	try
    {
        setTimeStart(timeStart);
        setTimeEnd (timeEnd);
        setNumPoints(numberOfPoints);
	  	return simulate();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

RRStringArrayHandle rrCallConv getReactionIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        StringList rNames = gRRHandle->getReactionIds();

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

RRVectorHandle rrCallConv getRatesOfChange()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        vector<double> rates = gRRHandle->getRatesOfChange();

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
            return list;
        }
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

RRStringArrayHandle rrCallConv getRatesOfChangeIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        StringList rNames = gRRHandle->getRateOfChangeIds();

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

bool rrCallConv getValue(const char* symbolId, double& value)
{
	try
    {
    	if(!gRRHandle)
    	{
        	setError(ALLOCATE_API_ERROR_MSG);
        	return false;
    	}
	    value = gRRHandle->getValue(symbolId);
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


RRMatrixHandle rrCallConv getUnScaledElasticityMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

		ls::DoubleMatrix tempMat = gRRHandle->getUnscaledElasticityMatrix();

        RRMatrixHandle matrix = createMatrix(&tempMat);
	    return matrix;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

RRMatrixHandle rrCallConv getScaledElasticityMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        ls::DoubleMatrix tempMat = gRRHandle->getScaledElasticityMatrix();


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

bool rrCallConv setValue(const char* symbolId, const double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
	    return gRRHandle->setValue(symbolId, value);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

RRMatrixHandle rrCallConv getStoichiometryMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        ls::DoubleMatrix tempMat = gRRHandle->getStoichiometryMatrix();

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
    }
	return false;
}

RRMatrixHandle rrCallConv getConservationMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        ls::DoubleMatrix tempMat = gRRHandle->getConservationMatrix();

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
    }
	return false;
}

RRMatrixHandle rrCallConv getLinkMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        ls::DoubleMatrix *tempMat = gRRHandle->getLinkMatrix();

		return createMatrix(tempMat);
	}
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return false;
}

RRMatrixHandle rrCallConv getL0Matrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        ls::DoubleMatrix *tempMat = gRRHandle->getL0Matrix();
        
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

RRMatrixHandle rrCallConv getNrMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        ls::DoubleMatrix *tempMat = gRRHandle->getNrMatrix();

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
    return gLastError;
}

bool rrCallConv reset()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        gRRHandle->reset();
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

int rrCallConv getNumberOfReactions()
{
 	try
    {
        if(!gRRHandle)
        {
           setError(ALLOCATE_API_ERROR_MSG);
           return -1;
        }
        return gRRHandle->getNumberOfReactions();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return -1;
}

bool rrCallConv getReactionRate(const int& rateNr, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        value = gRRHandle->getReactionRate(rateNr);
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

RRVectorHandle rrCallConv getReactionRates()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        vector<double> vec =  gRRHandle->getReactionRates();

        RRVector* aVec = createVectorFromVector_double(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

int rrCallConv getNumberOfBoundarySpecies()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }
        return gRRHandle->getNumberOfBoundarySpecies();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
  	return -1;
}

RRStringArrayHandle rrCallConv getBoundarySpeciesIds()         
{
	try
    {

        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        StringList bNames = gRRHandle->getBoundarySpeciesIds();

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
    }
   	return NULL;
}

int rrCallConv getNumberOfFloatingSpecies()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }
        return gRRHandle->getNumberOfFloatingSpecies();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
   	return -1;
}

RRStringArrayHandle rrCallConv getFloatingSpeciesIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        StringList fNames = gRRHandle->getFloatingSpeciesIds();

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
    }
    return false;
}

int rrCallConv getNumberOfGlobalParameters()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }
        return gRRHandle->getNumberOfGlobalParameters();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
   	return -1;
}

RRStringArrayHandle rrCallConv getGlobalParameterIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        StringList pNames = gRRHandle->getGlobalParameterIds();

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
    }
   	return NULL;
}

RRVectorHandle rrCallConv getFloatingSpeciesConcentrations()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        vector<double> vec =  gRRHandle->getFloatingSpeciesConcentrations();
        RRVector* aVec = createVectorFromVector_double(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

RRVectorHandle rrCallConv getBoundarySpeciesConcentrations()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        vector<double> vec =  gRRHandle->getBoundarySpeciesConcentrations();
        RRVector* aVec = createVectorFromVector_double(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}


RRVectorHandle rrCallConv getFloatingSpeciesInitialConcentrations()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        vector<double> vec =  gRRHandle->getFloatingSpeciesInitialConcentrations();
        RRVector* aVec = createVectorFromVector_double(vec);
        return aVec;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

bool rrCallConv setFloatingSpeciesByIndex (const int& index, const double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        gRRHandle->setFloatingSpeciesByIndex(index, value);
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

bool rrCallConv setBoundarySpeciesByIndex (const int& index, const double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        gRRHandle->setBoundarySpeciesByIndex(index, value);
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

bool rrCallConv setGlobalParameterByIndex(const int& index, const double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        gRRHandle->setGlobalParameterByIndex(index, value);
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

bool rrCallConv setFloatingSpeciesInitialConcentrations(const RRVector* vec)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        vector<double> tempVec;
        copyVector(vec, tempVec);
        gRRHandle->changeInitialConditions(tempVec);
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



bool rrCallConv setFloatingSpeciesConcentrations(const RRVector* vec)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        vector<double> tempVec;
        copyVector(vec, tempVec);
        gRRHandle->setFloatingSpeciesConcentrations(tempVec);

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


bool rrCallConv setBoundarySpeciesConcentrations(const RRVector* vec)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        vector<double> tempVec;
        copyVector(vec, tempVec);
        gRRHandle->setBoundarySpeciesConcentrations(tempVec);

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


bool rrCallConv oneStep(const double& currentTime, const double& stepSize, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        value = gRRHandle->oneStep(currentTime, stepSize);
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

RRVectorHandle rrCallConv getGlobalParameterValues()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        vector<double> vec =  gRRHandle->getGlobalParameterValues();
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

RRListHandle rrCallConv getAvailableTimeCourseSymbols()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        ArrayList slSymbols = gRRHandle->getAvailableTimeCourseSymbols();
        //cout<<"Got "<<slSymbols;
		return createList(slSymbols);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    	return NULL;
    }
}

RRListHandle rrCallConv getAvailableSteadyStateSymbols()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        ArrayList slSymbols = gRRHandle->getAvailableSteadyStateSymbols();
        //cout<<"Got "<<slSymbols;
		return createList(slSymbols);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    	return NULL;
    }
}

bool rrCallConv getBoundarySpeciesByIndex (const int& index, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        value = gRRHandle->getBoundarySpeciesByIndex(index);
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

bool rrCallConv getFloatingSpeciesByIndex (const int& index, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        value = gRRHandle->getFloatingSpeciesByIndex(index);
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

bool rrCallConv getGlobalParameterByIndex (const int& index, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        value = gRRHandle->getGlobalParameterByIndex(index);
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



bool rrCallConv getuCC (const char* variable, const char* parameter, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        value = gRRHandle->getuCC(variable, parameter);
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


bool rrCallConv getCC (const char* variable, const char* parameter, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        value = gRRHandle->getCC(variable, parameter);
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


bool rrCallConv getuEE(const char* name, const char* species, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        value = gRRHandle->getuEE(name, species);
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


bool rrCallConv getEE(const char* name, const char* species, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        value = gRRHandle->getEE(name, species);
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

int rrCallConv getNumberOfDependentSpecies()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }

        return gRRHandle->getNumberOfDependentSpecies();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
  	return -1;
}

int rrCallConv getNumberOfIndependentSpecies()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }

        return gRRHandle->getNumberOfIndependentSpecies();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
  	return -1;
}

bool rrCallConv steadyState(double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
	   	value = gRRHandle->steadyState();
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

bool rrCallConv evalModel()
{
	try
	{
		if(!gRRHandle)
		{
			setError(ALLOCATE_API_ERROR_MSG);
		}
		gRRHandle->EvalModel();
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

char* rrCallConv getParamPromotedSBML(const char* sArg)
{
	try
	{
		if(!gRRHandle)
		{
			setError(ALLOCATE_API_ERROR_MSG);
			return NULL;
		}

		string param =  gRRHandle->getParamPromotedSBML(sArg);

		char* text = createText(param.c_str());
		return text;
	}
	catch(Exception& ex)
	{
		stringstream msg;
		msg<<"RoadRunner exception: "<<ex.what()<<endl;
		setError(msg.str());
	}
	return NULL;
}

RRVectorHandle rrCallConv computeSteadyStateValues()
{
	try
	{
		if(!gRRHandle)
		{
			setError(ALLOCATE_API_ERROR_MSG);
			return NULL;
		}
		vector<double> vec =  gRRHandle->computeSteadyStateValues();

		RRVector* aVec = createVectorFromVector_double(vec);
		return aVec;
	}
	catch(Exception& ex)
	{
		stringstream msg;
		msg<<"RoadRunner exception: "<<ex.what()<<endl;
		setError(msg.str());
	}
	return NULL;
}

bool rrCallConv setSteadyStateSelectionList(const char* list)
{
	try
	{
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        StringList aList(list, " ,");
        gRRHandle->setSteadyStateSelectionList(aList);
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

RRListHandle rrCallConv getSteadyStateSelectionList()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        ArrayList sNames = gRRHandle->getSteadyStateSelectionList();

        if(!sNames.Count())
        {
            return NULL;
        }

		// ********** HMS TO BE ADDED BACK LATER
        return createList(sNames);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

RRMatrixHandle rrCallConv getFullJacobian()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        ls::DoubleMatrix tempMat = gRRHandle->getFullJacobian();
        return createMatrix(&tempMat);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}

RRMatrixHandle rrCallConv getReducedJacobian()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        ls::DoubleMatrix tempMat = gRRHandle->getReducedJacobian();
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


RRMatrixHandle rrCallConv getEigenValues()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

		ls::DoubleMatrix tempMat = gRRHandle->getEigenvalues();
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

char* rrCallConv getCSourceFileName()
{
	try
    {
    	if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        CGenerator* generator = gRRHandle->GetCGenerator();
        if(!generator)
        {
            return NULL;
        }

        char* fName;
        string fNameS = generator->GetSourceCodeFileName();

        fNameS = ExtractFileNameNoExtension(fNameS);

        if(fNameS.size())
        {
            fName = new char[fNameS.size() + 1];
            strcpy(fName, fNameS.c_str());

        }
		return fName;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
		return NULL;
    }
}

RRCCode* rrCallConv getCCode()
{
	try
    {
    	if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        CGenerator* generator = gRRHandle->GetCGenerator();
        if(!generator)
        {
            return NULL;
        }

        RRCCode* cCode = new RRCCode;
		cCode->Header = NULL;
		cCode->Source = NULL;
        string header = generator->GetHeaderCode();
        string source = generator->GetSourceCode();

        if(header.size())
        {
            cCode->Header = new char[header.size() + 1];
            strcpy(cCode->Header, header.c_str());
        }

        if(source.size())
        {
            cCode->Source = new char[source.size() + 1];
            strcpy(cCode->Source, source.c_str());
        }
        return cCode;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
	return NULL;
}


// *******  Not yet implemented  ********
// codeGenerationMode = 0 if mode is C code generation
// codeGenerationMode = 1 ig mode is internal math interpreter
bool rrCallConv setCodeGenerationMode (int _mode)
{
	return false;
}


//NOM forwarded functions
int rrCallConv getNumberOfRules()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return -1;
        }
        if(!gRRHandle->getNOM())
        {
            Log(lWarning)<<"NOM is not allocated.";
        	return -1;
        }
        int value = gRRHandle->getNOM()->getNumRules();
        return value;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return -1;
    }
}

bool rrCallConv getScaledFloatingSpeciesElasticity(const char* reactionId, const char* speciesId, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }
        value = gRRHandle->getScaledFloatingSpeciesElasticity(reactionId, speciesId);
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

RRStringArrayHandle rrCallConv getFloatingSpeciesInitialConditionIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        StringList aList = gRRHandle->getFloatingSpeciesInitialConditionIds();
        return createList(aList);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRVectorHandle rrCallConv getRatesOfChangeEx(const RRVectorHandle vec)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        vector<double> tempList = createVectorFromRRVector(vec);
        tempList = gRRHandle->getRatesOfChangeEx(tempList);
        return createVectorFromVector_double (tempList);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRVectorHandle rrCallConv getReactionRatesEx(const RRVectorHandle vec)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        vector<double> tempList = createVectorFromRRVector(vec);
        tempList = gRRHandle->getReactionRatesEx(tempList);
        return createVectorFromVector_double(tempList);;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRList* rrCallConv getElasticityCoefficientIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        ArrayList aList = gRRHandle->getElasticityCoefficientIds();
        return createList(aList);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}


bool rrCallConv setCapabilities(const char* caps)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return false;
        }

        if(!caps)
        {
            return false;
        }
        gRRHandle->setCapabilities(caps);
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

char* rrCallConv getCapabilities()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        return createText(gRRHandle->getCapabilities());
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RRStringArrayHandle rrCallConv getEigenValueIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        StringList aList = gRRHandle->getEigenValueIds();
        return createList(aList);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRListHandle rrCallConv getFluxControlCoefficientIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        return createList(gRRHandle->getFluxControlCoefficientIds());
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRMatrixHandle rrCallConv getUnscaledConcentrationControlCoefficientMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        return createMatrix(&(gRRHandle->getUnscaledConcentrationControlCoefficientMatrix()));
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}


RRMatrixHandle rrCallConv getScaledConcentrationControlCoefficientMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        return createMatrix(&(gRRHandle->getScaledConcentrationControlCoefficientMatrix()));
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}


RRMatrixHandle rrCallConv getUnscaledFluxControlCoefficientMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        return createMatrix(&(gRRHandle->getUnscaledFluxControlCoefficientMatrix()));
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}


RRMatrixHandle rrCallConv getScaledFluxControlCoefficientMatrix()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }

        return createMatrix(&(gRRHandle->getScaledFluxControlCoefficientMatrix()));
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRListHandle rrCallConv getUnscaledFluxControlCoefficientIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
		ArrayList arrList = gRRHandle->getUnscaledFluxControlCoefficientIds();
        return createList(arrList);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRList* rrCallConv getConcentrationControlCoefficientIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        ArrayList list = gRRHandle->getConcentrationControlCoefficientIds();
        return createList(list);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

RRListHandle rrCallConv getUnscaledConcentrationControlCoefficientIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        return createList(gRRHandle->getUnscaledConcentrationControlCoefficientIds());
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

int rrCallConv getNumberOfCompartments()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        return gRRHandle->getNumberOfCompartments();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return -1;
}

bool rrCallConv getCompartmentByIndex(const int& index, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        value = gRRHandle->getCompartmentByIndex(index);
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

bool rrCallConv setCompartmentByIndex (const int& index, const double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        gRRHandle->setCompartmentByIndex(index, value);
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

RRStringArrayHandle rrCallConv getCompartmentIds()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        return createList(gRRHandle->getCompartmentIds());
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

bool rrCallConv getRateOfChange(const int& index, double& value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
            return NULL;
        }
        value = gRRHandle->getRateOfChange(index);
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

//Print functions ==========================================================

char* rrCallConv stringArrayToString (const RRStringArrayHandle list)
{
	try
    {
        if(!list)
        {
            return NULL;
        }

		stringstream resStr;
	    for(int i = 0; i < list->Count; i++)
        {
        	resStr<<list->String[i];;
            if(i < list->Count -1)
            {
            	resStr <<" ";//endl;
            }
        }

		string strTmp = resStr.str();
    	char* resultChar = new char[strTmp.size() + 1];
        strcpy(resultChar, strTmp.c_str());
        return resultChar;

    }
    catch(Exception& ex)
    {
        stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}


char* rrCallConv resultToString(const RRResultHandle result)
{
	try
    {
        if(!result)
        {
            return NULL;
        }
		stringstream resStr;
		//RRResult is a 2D matrix, and column headers (strings)
        //First header....
	    for(int i = 0; i < result->CSize; i++)
        {
        	resStr<<result->ColumnHeaders[i];
            if(i < result->CSize -1)
            {
            	resStr <<"\t";
            }
        }
        resStr<<endl;

        //Then the data
        int index = 0;
	    for(int j = 0; j < result->RSize; j++)
   	    {
		    for(int i = 0; i < result->CSize; i++)
    	    {
        		resStr<<result->Data[index++];
	            if(i < result->CSize -1)
    	        {
        	    	resStr <<"\t";
            	}
            }
	    	resStr <<"\n";
        }
		string strTmp = resStr.str();
    	char* resultChar = new char[strTmp.size() + 1];
        strcpy(resultChar, strTmp.c_str());
        return resultChar;

    }
    catch(Exception& ex)
    {
        stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

char* rrCallConv matrixToString(const RRMatrixHandle matrixHandle)
{
	try
    {
        if(!matrixHandle)
        {
            return NULL;
        }

        RRMatrix& mat = *matrixHandle;
        stringstream ss;
        ss<<"matrix dimension: "<<mat.RSize<<"x"<<mat.CSize<<" --\n";

        int index = 0;
        for(int row = 0; row < mat.RSize; row++)
        {
            for(int col = 0; col < mat.CSize; col++)
            {
                ss<<mat.Data[index++];
                if(col < mat.CSize + 1)
                {
                    ss<<"\t";
                }
            }
            ss<<endl;
        }
        string msg(ss.str());
        return createText(msg);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

char* rrCallConv vectorToString(RRVectorHandle vecHandle)
{
	try
    {
        if(!vecHandle)
        {
            setError("Null vector in printMatrix...");
            return NULL;
        }

        RRVector& vec = *vecHandle;

        stringstream ss;
        ss<<"vector dimension: "<<vec.Count<<" \n";

        for(int index = 0; index < vec.Count; index++)
        {
            ss<<vec.Data[index];
            if(index < vec.Count + 1)
            {
                ss<<"\t";
            }
        }
        ss<<endl;
        return createText(ss.str());
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
    return NULL;
}

//Free Functions =====================================================
bool rrCallConv freeRRInstance(RRHandle handle)
{
	try
    {
        delete gRRHandle;
        gRRHandle = NULL;
        handle = NULL;
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

bool rrCallConv freeMatrix(RRMatrixHandle matrix)
{
	try
    {
        if(matrix)
        {
            delete [] (matrix->Data);
            delete matrix;
        }
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

bool rrCallConv freeResult(RRResultHandle handle)
{
	try
    {
        delete handle;
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

bool rrCallConv freeText(char* text)
{
	try
    {
        if(text != ALLOCATE_API_ERROR_MSG)
        {
            delete [] text;
        }
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


bool rrCallConv freeStringArray(RRStringArrayHandle sl)
{
	try
    {
    	delete sl;
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


bool rrCallConv freeVector(RRVectorHandle vector)
{
	try
    {
        if(vector)
        {
    	   delete [] vector->Data;
        }
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

bool rrCallConv freeCCode(RRCCodeHandle code)
{
	try
    {
        if(code)
        {
            delete code->Header;
            delete code->Source;
        }
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

/////////////////////////////////////////////////////////////
void rrCallConv Pause()
{
    rr::Pause(true, "Hit any key to continue..\n");
}

RRVectorHandle rrCallConv createVector (int size)
{
   RRVectorHandle list = new RRVector;
   list->Count = size;
   list->Data = new double[list->Count];
   return list;
}

int rrCallConv getVectorLength (RRVectorHandle vector)
{
	if (vector == NULL)
		return -1;
	else
		return vector->Count;
}

bool rrCallConv getVectorElement (RRVectorHandle vector, int index, double& value)
{
	if (vector == NULL)
		return false;
	if ((index < 0) || (index >= vector->Count))
		return false;
	value = vector->Data[index];
	return true;
}

bool rrCallConv setVectorElement (RRVectorHandle vector, int index, double value)
{
	if (vector == NULL)
		return false;
	if ((index < 0) || (index >= vector->Count))
		return false;
	vector->Data[index] = value;
	return true;
}

int rrCallConv getMatrixNumRows (RRMatrixHandle m)
{
	if (m == NULL)
		return -1;
	return m->RSize;
}

int  rrCallConv getMatrixNumCols (RRMatrixHandle m)
{
	if (m == NULL)
		return -1;
	return m->CSize;
}

bool rrCallConv getMatrixElement (RRMatrixHandle m, int r, int c, double& value)
{
	if (m == NULL)
		return false;
	if ((r < 0) || (c < 0) || (r >= m->RSize) || (c >= m->CSize))
		return false;
	value = m->Data[r*m->CSize + c];
	return true;
}

int rrCallConv  getResultNumRows (RRResultHandle result)
{
	if (result == NULL)
		return -1;
	return result->RSize;
}

int  rrCallConv  getResultNumCols (RRResultHandle result)
{
	if (result == NULL)
		return -1;
	return result->CSize;
}

bool  rrCallConv getResultElement(RRResultHandle result, int r, int c, double& value)
{
	if (result == NULL)
    {
		return false;
    }

	if ((r < 0) || (c < 0) || (r >= result->RSize) || (c >= result->CSize))
    {
		return false;
    }

	value = result->Data[r*result->CSize + c];
	return true;
}

char*  rrCallConv getResultColumnLabel (RRResultHandle result, int column)
{
	if (result == NULL)
    {
		return NULL;
    }

	if ((column < 0) || (column >= result->CSize))
		return NULL;
	return result->ColumnHeaders[column];
}

char* rrCallConv getCCodeHeader(RRCCodeHandle code)
{
	if (code == NULL)
    {
		return NULL;
    }
	return code->Header;
}

char* rrCallConv getCCodeSource(RRCCodeHandle code)
{
	if (code == NULL)
    {
		return NULL;
    }
	return code->Source;
}

// -------------------------------------------------------------------
// List Routines
// -------------------------------------------------------------------
RRListHandle rrCallConv createRRList()
{
	RRListHandle list = new RRList;
	list->Count = 0;
	list->Items = NULL;
	return list;
}

void rrCallConv freeRRList (RRListHandle theList)
{
	if(!theList)
    {
    	return;
    }
    int itemCount = theList->Count;
    for(int i = 0; i < itemCount; i++)
    {
        if(theList->Items[i]->ItemType == litString)
        {
              delete [] theList->Items[i]->data.sValue;
        }
        if(theList->Items[i]->ItemType == litList)
        {
            freeRRList ((RRList *) theList->Items[i]->data.lValue);
        }
        delete theList->Items[i];
    }
	delete [] theList->Items;
    delete theList;
    theList = NULL;
}

RRListItemHandle rrCallConv createIntegerItem (int value)
{
	RRListItemHandle item =  new RRListItem;
	item->ItemType = litInteger;
	item->data.iValue = value;
	return item;
}

RRListItemHandle rrCallConv createDoubleItem (double value)
{
	RRListItemHandle item = new RRListItem;
	item->ItemType = litDouble;
	item->data.dValue = value;
	return item;
}

RRListItemHandle rrCallConv createStringItem (char* value)
{
	RRListItemHandle item = new RRListItem;
	item->ItemType = litString;
	item->data.sValue = new char[strlen(value) + 1];
	strcpy(item->data.sValue, value);
	return item;
}

RRListItemHandle rrCallConv createListItem (RRList* value)
{
	RRListItemHandle item = new RRListItem;
	item->ItemType = litList;
	item->data.lValue = value;
	return item;
}

// Add an item to a given list, returns the index to
// the item in the list. Returns -1 if it fails.
int rrCallConv addItem (RRListHandle list, RRListItemHandle *item)
{
	int n = list->Count;

	RRListItemHandle *newItems = new RRListItemHandle [n+1];
    if(!newItems)
    {
    	setError("Failed allocating memory in addItem()");
    	return -1;
    }

    for(int i = 0; i < n; i++)
    {
    	newItems[i] = list->Items[i];
    }

    newItems[n] = *item;
    RRListItemHandle *oldItems = list->Items;
    list->Items = newItems;

    delete [] oldItems;

	list->Count = n+1;
	return n;
}

bool rrCallConv isListItemInteger (RRListItemHandle item)
{
	return (item->ItemType == litInteger) ? true : false;
}

bool rrCallConv isListItemDouble (RRListItemHandle item)
{
	return (item->ItemType == litDouble) ? true : false;
}

bool rrCallConv isListItemString (RRListItemHandle item)
{
	return (item->ItemType == litString) ? true : false;
}

bool rrCallConv isListItemList (RRListItemHandle item)
{
	return (item->ItemType == litList) ? true : false;
}

RRListItemHandle rrCallConv getListItem (RRListHandle list, int index)
{
	return (index >= list->Count) ? NULL : list->Items[index];
}

bool rrCallConv getIntegerListItem (RRListItemHandle item, int &value)
{
    if (item->ItemType == litInteger)
    {
        value = item->data.iValue;
        return true;
    }
    return false;
}

bool rrCallConv getDoubleListItem (RRListItemHandle item, double &value)
{
    if (item->ItemType == litDouble)
    {
    	value = item->data.dValue;
     	return true;
    }

    return false;
}

char* rrCallConv getStringListItem (RRListItemHandle item)
{
	return (item->ItemType == litString) ? item->data.sValue : NULL;
}

RRListHandle rrCallConv getList (RRListItemHandle item)
{
	return (item->ItemType == litList) ? item->data.lValue : NULL;
}

bool rrCallConv isListItem (RRListItemHandle item, ListItemType itemType)
{
	return  (item->ItemType == itemType) ? true : false;
}

int rrCallConv getListLength (RRListHandle myList)
{
	return myList->Count;
}

char* rrCallConv listToString (RRListHandle list)
{
	try
    {
        if(!list)
        {
            return NULL;
        }

        //Types of list items
        char*           cVal;
        int             intVal;
        double          dVal;
        RRList*        lVal; 		//list is nested list
		stringstream resStr;
        resStr<<"{";
	    for(int i = 0; i < list->Count; i++)
        {
			switch(list->Items[i]->ItemType)
            {
                case litString:
					cVal = list->Items[i]->data.sValue;
                    resStr<<"\"";
                    if(cVal)
                    {
                    	resStr<<cVal;
                    }
                    resStr<<"\"";
                break;

                case litInteger:
					intVal = list->Items[i]->data.iValue;
                    resStr<< (intVal);
                break;

                case litDouble:
					dVal =  list->Items[i]->data.dValue;
                    resStr<< (dVal);
                break;

                case litList:
					lVal = list->Items[i]->data.lValue;
                    if(lVal)
                    {
                    	char* text = listToString(lVal);
                    	resStr<<text;
                        freeText(text);
                    }
                    else
                    {
						resStr<<"{}";
                    }
                break;
            }

            if(i < list->Count -1)
            {
                resStr<<",";
            }
        }
        resStr<<"}";
		string strTmp = resStr.str();
    	char* resultChar = new char[strTmp.size() + 1];
        strcpy(resultChar, strTmp.c_str());
        return resultChar;

    }
    catch(Exception& ex)
    {
        stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}


#pragma hdrstop
#include <stdexcept>
#include "telAutoTelluriumInterface.h"
//#include "rr/rrRoadRunner.h"
#include "rr/rrExecutableModel.h"
#include "../libAuto/vsAuto.h"
#include "telLogger.h"
#include "telStringList.h"
#include "telUtils.h"
#include "telAutoUtils.h"

#include "rrplugins/core/tel_api.h"
#include "../../../wrappers/C/rrc_types.h"

namespace telauto
{
using namespace tlp;
using namespace autolib;

//Statics
rrc::RRHandle AutoTellurimInterface::mRR = NULL;		//#RoadRunner*     AutoTellurimInterface::mRR  = NULL;
rrc::THostInterface *AutoTellurimInterface::mHostInterface = NULL;

Properties*     AutoTellurimInterface::mProperties              = NULL;
AutoConstants   AutoTellurimInterface::mAutoConstants;
string          AutoTellurimInterface::mPCPParameterName        = gEmptyString;
StringList      AutoTellurimInterface::mModelParameters         = StringList();
StringList      AutoTellurimInterface::mModelBoundarySpecies    = StringList();

AutoTellurimInterface::AutoTellurimInterface(rrc::RRHandle rr)	//#AutoTellurimInterface::AutoTellurimInterface(RoadRunner* rr)
{
    mRR = rr;
}

AutoTellurimInterface::~AutoTellurimInterface()
{}

void AutoTellurimInterface::assignRoadRunner(rrc::RRHandle rrInstance,rrc::THostInterface* tHostInterface)//#::assignRoadRunner(RoadRunner* rrInstance)
{
    mRR = rrInstance;
	mHostInterface = tHostInterface;
}

void AutoTellurimInterface::assignProperties(Properties* props) 
{
    mProperties = props;
    if(mProperties)
    {
        mAutoConstants.populateFrom(mProperties);
    }
    else
    {
        mAutoConstants.reset();
    }
}

bool AutoTellurimInterface::selectParameter(const string& para)
{
    mPCPParameterName = para;
    return false;
}

bool AutoTellurimInterface::setScanDirection(ScanDirection val)
{

    if(val == sdPositive)
    {
        mAutoConstants.DS = fabs(mAutoConstants.DS);
    }
    else
    {
        mAutoConstants.DS = -1 * fabs(mAutoConstants.DS);
    }

    mAutoConstants.mScanDirection = val;
    return true;
}

bool AutoTellurimInterface::setTempFolder(const string& fldr)
{
    if(folderExists(fldr))
    {
        mTempFolder = fldr;
        return true;
    }
    else
    {
        return false;
    }
}

string AutoTellurimInterface::getTempFolder()
{
    return mTempFolder;
}

void AutoTellurimInterface::setInitialPCPValue()
{
	double value = (mAutoConstants.mScanDirection == sdPositive) ? mAutoConstants.RL0 : mAutoConstants.RL1;

	if(mModelBoundarySpecies.contains(mPCPParameterName))
	{
		int index = mModelBoundarySpecies.indexOf(mPCPParameterName);
		
		mHostInterface->setBoundarySpeciesByIndex(mRR,index, value);
	}
	else
	{
		mHostInterface->setValue(mRR,mPCPParameterName.c_str(), value);
	}


	if(mAutoConstants.PreSimulation)
	{
		/*rr::BasicDictionary opt;
		opt.setItem("start", mAutoConstants.PreSimulationStart);
		opt.setItem("duration", mAutoConstants.PreSimulationDuration);
		opt.setItem("steps", mAutoConstants.PreSimulationSteps);
		opt.setItem("stiff", true);
		mRR->simulate(&opt);			// TODO: Why are two simulate calls necessary? If there is only a single call, simulation does not tend towards steady-state, even with long simulation times.
		mRR->simulate(&opt);*/

		// Two simulations gives better results. 
		mHostInterface->simulateEx(mRR, mAutoConstants.PreSimulationStart, mAutoConstants.PreSimulationDuration, mAutoConstants.PreSimulationSteps);
		mHostInterface->simulateEx(mRR, mAutoConstants.PreSimulationStart, mAutoConstants.PreSimulationDuration, mAutoConstants.PreSimulationSteps);
	}
	
	double* val= new double;		//just to make the call compatible else no use
	mHostInterface->steadyState(mRR,val);
	delete val;
}

void AutoTellurimInterface::run()
{
	if(!mRR)
    {
		throw(Exception("Roadrunner is NULL in AutoTelluriumInterface function run()"));
    }
	
    if(!setupUsingCurrentModel())
    {
		throw(Exception("Failed in Setup AutoTelluriumInterface"));
    }
    //Create fort.2 file
    string temp = getConstantsAsString();
	autolib::createFort2File(temp.c_str(), joinPath(getTempFolder(), "fort.2"));
    //Run AUTO
    CallAuto(getTempFolder());

}

bool AutoTellurimInterface::setupUsingCurrentModel()
{
    //mAutoConstants.NDIM = mRR->getModel()->getNumIndFloatingSpecies() + mRR->getModel()->getNumRateRules();
	mAutoConstants.NDIM = mHostInterface->_getNumIndFloatingSpecies(mRR) + mHostInterface->_getNumRateRules(mRR);
    //k1,k2 etc
	rrc::RRStringArrayPtr lists= mHostInterface->getGlobalParameterIds(mRR);
	StringList list1(lists->String,lists->Count);
	//mModelParameters        = mRR->getGlobalParameterIds(rrHandle);
	mModelParameters = list1;

	lists= mHostInterface->getBoundarySpeciesIds(mRR);
	StringList list2(lists->String, lists->Count);
    //Boundary species can be used as PCP as well
    mModelBoundarySpecies   = list2;

    //Set initial value of Primary continuation parameter
    setInitialPCPValue();
    setCallbackStpnt(ModelInitializationCallback);	///*************************** Check
    setCallbackFunc2(ModelFunctionCallback);
    return true;
}

//Called by Auto
int autoCallConv AutoTellurimInterface::ModelInitializationCallback(long ndim, double t, double* u, double* par)
{
	//rr::ExecutableModel* theModel = mRR->getModel();

	//The continuation parameter can be a 'parameter' or a boundary species
	int numBoundaries(0), numParameters(0);

	if(mModelBoundarySpecies.indexOf(mPCPParameterName) != -1)
	{
		numBoundaries = 1;
	}

	if(mModelParameters.indexOf(mPCPParameterName) != -1)
	{
		numParameters = 1;
	}

	vector<double> boundaryValues(numBoundaries);
	vector<double> globalParameters(numParameters);

	if(numBoundaries > 0)
	{
		double* value = new double;
		for (int i = 0; i < numBoundaries; i++)
		{
			int selSpecieIndex      = mModelBoundarySpecies.indexOf(mPCPParameterName);
			mHostInterface->getBoundarySpeciesByIndex(mRR,selSpecieIndex,value);
			boundaryValues[i] = *value;
		}
		delete value;
	}

	if(numParameters > 0)
	{
		double* value=new double;
		for (int i = 0; i < numParameters; i++)
		{
			int selParameter    = mModelParameters.indexOf(mPCPParameterName);
			mHostInterface->getGlobalParameterByIndex(mRR,selParameter,value);
			globalParameters[i] = *value;
		}
		delete value;
	}

	int oParaSize = numBoundaries + numParameters;
	vector<double> parameterValues(oParaSize);

	for(int i = 0; i < numBoundaries; i++)
	{
		parameterValues[i] = boundaryValues[i];
	}

	for(int i = 0; i < numParameters; i++)
	{
		parameterValues[numBoundaries + i] = globalParameters[i];
	}

	for(int i = 0; i < oParaSize; i++)
	{
		par[i] = parameterValues[i];
	}

	int    nrIndFloatingSpecies = mHostInterface->_getNumIndFloatingSpecies(mRR)+mHostInterface->_getNumRateRules(mRR);
	double* floatCon            = new double[nrIndFloatingSpecies];

	//theModel->getFloatingSpeciesConcentrations(nrIndFloatingSpecies, NULL, floatCon);
	floatCon = (mHostInterface->getFloatingSpeciesConcentrations(mRR))->Data;

	int nMin = min(nrIndFloatingSpecies, ndim);
	for(int i = 0; i < nMin; i++)
	{
		u[i] = floatCon[i];
	}

	delete[] floatCon;
	return 0;
}

void autoCallConv AutoTellurimInterface::ModelFunctionCallback(const double* oVariables, const double* par, double* oResult)
{
	//The continuation parameter can be a 'parameter' OR a boundary species
	int numBoundaries(0);
	int numParameters(0);

	if(mModelBoundarySpecies.indexOf(mPCPParameterName) != -1)
	{
		numBoundaries = 1;
	}

	if(mModelParameters.indexOf(mPCPParameterName) != -1)
	{
		numParameters = 1;
	}

	//rr::ExecutableModel* theModel = mRR->getModel();
	if (numBoundaries > 0)
	{
		vector<double> oBoundary(numBoundaries);
		for(int i = 0; i < numBoundaries; i++)
		{
			oBoundary[i] = par[i];
		}

		for (int i = 0; i < numBoundaries; i++)
		{
			double val = oBoundary[i];
			int selSpecieIndex      = mModelBoundarySpecies.indexOf(mPCPParameterName);
			mHostInterface->setBoundarySpeciesByIndex(mRR,selSpecieIndex, val);
		}
	}

	if (numParameters > 0)
	{
		//Todo memory leak
		vector<double> oParameters(numParameters);
		for(int i = 0; i < numParameters; i++)
		{
			oParameters[i] = par[i];
		}

		//Currently only one variable is supported
		mHostInterface->setValue(mRR, mPCPParameterName.c_str(), oParameters[0]);
	}
		
	//vector<rr::SelectionRecord>  selRecs = mRR->getSteadyStateSelectionList(rrHandle);	//incompatible type
	
	//vector<rr::SelectionRecord>  selRecs;
	rrc::RRStringArrayPtr temp= mHostInterface->getSteadyStateSelectionList(mRR);
	tlp::StringList selRecs(temp->String, temp->Count);
	/*for (int i = 0; i < temp->Count; ++i) {							//recheck***********************************
		rr::SelectionRecord record(*(temp->String+i));
		selRecs.push_back(record);
	}

	tlp::StringList              selList = getRecordsAsStrings(selRecs);*/
	tlp::StringList              selList = selRecs;
	vector<double> variableTemp(selList.size());
	int ndim = mAutoConstants.NDIM;
	int nMin = min(selList.size(), ndim);

	for (int i = 0; i < nMin; i++)
	{
		variableTemp[i] = oVariables[i];
	}

	int  numIndFloatingSpecies  = mHostInterface->_getNumIndFloatingSpecies(mRR) + mHostInterface->_getNumRateRules(mRR);
	double* tempConc            = new double[numIndFloatingSpecies];
	if(!tempConc)
	{
		throw std::runtime_error("Failed to allocate memory in AutoTellurimInterface ModelFunction CallBack.");
	}

	for(int i = 0; i < numIndFloatingSpecies; i++)
	{
		if(i < variableTemp.size())
		{
			tempConc[i] = variableTemp[i];
		}
		else
		{
			throw("Big Problem");
		}
	}


	//theModel->setFloatingSpeciesConcentrations(numIndFloatingSpecies, NULL, tempConc);

	rrc::RRVectorPtr intermediate= new rrc::RRVector;
	intermediate->Count = numIndFloatingSpecies;
	intermediate->Data = tempConc;
	mHostInterface->setFloatingSpeciesConcentrations(mRR, intermediate);


	delete [] tempConc;

	//theModel->convertToAmounts();

	// 1) Check* Check the values and check and they are identical*************************************************************
	// 2) Replace the commented line
	// 3) 

	
	double  time            = mHostInterface->_getTime(mRR);
	int     stateVecSize    = mHostInterface->_getStateVector(mRR);
	double* dydts           = new double[stateVecSize];

	mHostInterface->_getStateVectorRate(mRR,time,dydts);
	nMin = min(stateVecSize, ndim);

	for(int i = 0; i < nMin; i++)
	{
		oResult[i] = dydts[i];
	}
	delete[] dydts;

	/*
	oResult=(mHostInterface->getRatesOfChange(mRR))->Data;
	int size = (mHostInterface->getRatesOfChange(mRR))->Count;

	RRPLOG(lError) << "ORESULTS:\n";
	for (int i = 0; i < size; ++i) {
		RRPLOG(lError) << oResult[i] << " ";
	}*/



}

string AutoTellurimInterface::getConstantsAsString()
{
    return mAutoConstants.getConstantsAsString();
}

}

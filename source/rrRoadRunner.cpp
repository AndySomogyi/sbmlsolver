#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iostream>
#include "Poco/File.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrModelGenerator.h"
#include "rrCompiler.h"
#include "rrStreamWriter.h"
#include "rrLogger.h"
#include "rrModelGeneratorFactory.h"
#include "rrUtils.h"
#include "rrExecutableModel.h"
#include "rrSBMLModelSimulation.h"
#include "rr-libstruct/lsLA.h"
#include "rr-libstruct/lsLibla.h"
#include "rrModelState.h"
#include "rrArrayList2.h"
#include "rrCapabilities.h"
#include "rrConstants.h"
#include "rrVersionInfo.h"
//---------------------------------------------------------------------------

namespace rr
{
using namespace std;
using namespace ls;


//The instance count increases/decreases as instances are created/destroyed.
int                   RoadRunner::mInstanceCount = 0;
Mutex                 RoadRunner::mLibSBMLMutex;

int RoadRunner::getInstanceCount()
{
    return mInstanceCount;
}

int RoadRunner::getInstanceID()
{
    return mInstanceID;
}

RoadRunner::RoadRunner(const string& tempFolder, const string& supportCodeFolder, const string& compiler)
:
mUseKinsol(false),
mDiffStepSize(0.05),
mModelFolder("models"),
mSteadyStateThreshold(1.E-2),
mSimulation(NULL),
mCurrentSBMLFileName(""),
mCVode(NULL),
mSteadyStateSolver(NULL),
mComputeAndAssignConservationLaws("Conservation", false, "enables (=true) or disables \
(=false) the conservation analysis \
of models for timecourse simulations."),
mTimeStart(0),
mTimeEnd(10),
mNumPoints(21),
mModel(NULL),
mCurrentSBML(""),
mPluginManager(joinPath(getParentFolder(supportCodeFolder), "plugins")),
mConservedTotalChanged(false),
mCapabilities("RoadRunner", "RoadRunner Capabilities"),
mRRCoreCapabilities("Road Runner Core", "", "Core RoadRunner Parameters")
{
	//Roadrunner is a "single" capability with many parameters
	mRRCoreCapabilities.addParameter(&mComputeAndAssignConservationLaws);

    mCapabilities.add(mRRCoreCapabilities);
    Log(lDebug4)<<"In RoadRunner ctor";

    // for now, dump out who we are
    Log(lDebug3) << "RoadRunner::RoadRunner(...), running refactored modelgen NOMFix\n";

    mModelGenerator = ModelGeneratorFactory::createModelGenerator("CModelGenerator", tempFolder, supportCodeFolder, compiler);

    setTempFileFolder(tempFolder);
    mPluginManager.setRoadRunnerInstance(this);

	//Increase instance count..
	mInstanceCount++;
    mInstanceID = mInstanceCount;

	//Setup additonal objects
    mCVode = new CvodeInterface(this, NULL);

    if(mCVode)
    {
    	mCapabilities.add(mCVode->getCapability());
    }

	mSteadyStateSolver = getNLEQInterface();
    if(mSteadyStateSolver)
    {
    	mCapabilities.add(mSteadyStateSolver->getCapability());
    }
}

RoadRunner::~RoadRunner()
{
    Log(lDebug4)<<"In RoadRunner DTOR";

    Log(lDebug3) << "In " << __FUNC__ << "\n";

    delete mModelGenerator;
    delete mModel;
    delete mCVode;

    //delete mLS;
    mInstanceCount--;
}

ExecutableModel* RoadRunner::getModel()
{
    return mModel;
}

vector<SelectionRecord> RoadRunner::getSelectionList()
{
	return mSelectionList;
}

string RoadRunner::getInfo()
{
	stringstream info;
    info<<"Model Loaded: "<<(mModel == NULL ? "false" : "true")<<endl;
    if(mModel)
    {
    	info<<"ModelName: "			<<  mModel->getModelName()<<endl;
//        info<<"Model DLL Loaded: "	<< (mModel->mDLL.isLoaded() ? "true" : "false")	<<endl;
//        info<<"Initialized: "		<< (mModel->mIsInitialized ? "true" : "false")	<<endl;
    }
    info<<"ConservationAnalysis: "	<<	(mComputeAndAssignConservationLaws.getValue() ? "true" : "false")<<endl;
    info<<"libSBML version: "		<<	getlibSBMLVersion()<<endl;
    info<<"Temporary folder: "		<<	getTempFolder()<<endl;
    info<<"Compiler location: "		<<	getCompiler()->getCompilerLocation()<<endl;
    info<<"Support Code Folder: "	<<	getCompiler()->getSupportCodeFolder()<<endl;
    info<<"Working Directory: "		<<	getCWD()<<endl;
	return info.str();
}

string RoadRunner::getExtendedVersionInfo()
{
	stringstream info;
    info<<"libSBML version: "		<<	getlibSBMLVersion()<<endl;
    info<<"Temporary folder: "		<<	getTempFolder()<<endl;
    info<<"Compiler location: "		<<	getCompiler()->getCompilerLocation()<<endl;
    info<<"Support Code Folder: "	<<	getCompiler()->getSupportCodeFolder()<<endl;
    info<<"Working Directory: "		<<	getCWD()<<endl;
	return info.str();
}

PluginManager&    RoadRunner::getPluginManager()
{
    return mPluginManager;
}

NOMSupport* RoadRunner::getNOM()
{
    return &mNOM;
}

Compiler* RoadRunner::getCompiler()
{
    return mModelGenerator ? mModelGenerator->getCompiler() : 0;
}

CvodeInterface* RoadRunner::getCVodeInterface()
{
    if(!mCVode)// && mModel != NULL)
    {
        mCVode = new CvodeInterface(this, mModel);
    }
    return mCVode;
}

bool RoadRunner::setCompiler(const string& compiler)
{
    return mModelGenerator ? mModelGenerator->setCompiler(compiler) : false;
}

NLEQInterface* RoadRunner::getNLEQInterface()
{
    if(!mSteadyStateSolver)// && mModel != NULL)
    {
        mSteadyStateSolver = new NLEQInterface(mModel);
    }
    return dynamic_cast<NLEQInterface*>(mSteadyStateSolver);
}

bool RoadRunner::isModelLoaded()
{
    return mModel ? true : false;
}

bool RoadRunner::useSimulationSettings(SimulationSettings& settings)
{
    mSettings   = settings;
    mTimeStart  = mSettings.mStartTime;
    mTimeEnd    = mSettings.mEndTime;
    mNumPoints  = mSettings.mSteps + 1;
    return true;
}

bool RoadRunner::computeAndAssignConservationLaws()
{
	return mComputeAndAssignConservationLaws.getValue();
}

bool RoadRunner::setTempFileFolder(const string& folder)
{
	if(folderExists(folder))
	{
		Log(lDebug)<<"Setting temp file folder to "<<folder;
	    mModelGenerator->setTemporaryDirectory(folder);
		mTempFileFolder = folder;
		return true;
	}
	else
	{
    	stringstream msg;
        msg<<"The folder: "<<folder<<" don't exist...";
		Log(lError)<<msg.str();

		CoreException e(msg.str());
        throw(e);
	}
}

string RoadRunner::getTempFolder()
{
	return mTempFileFolder;
}

int RoadRunner::createDefaultTimeCourseSelectionList()
{
    StringList theList;
    StringList oFloating  = getFloatingSpeciesIds();

	theList.add("time");
    for(int i = 0; i < oFloating.Count(); i++)
    {
        theList.add(oFloating[i]);
    }

    setTimeCourseSelectionList(theList);

    Log(lDebug)<<"The following is selected:";
    for(int i = 0; i < mSelectionList.size(); i++)
    {
        Log(lDebug)<<mSelectionList[i];
    }
    return mSelectionList.size();
}

int RoadRunner::createTimeCourseSelectionList()
{

    StringList theList = getSelectionListFromSettings(mSettings);

    if(theList.Count() < 2)
    {
        //AutoSelect
        theList.add("Time");

        //Get All floating species
       StringList oFloating  = getFloatingSpeciesIds();
       for(int i = 0; i < oFloating.Count(); i++)
       {
            theList.add(oFloating[i]);
       }
    }

    setTimeCourseSelectionList(theList);

    Log(lDebug)<<"The following is selected:";
    for(int i = 0; i < mSelectionList.size(); i++)
    {
        Log(lDebug)<<mSelectionList[i];
    }

    if(mSelectionList.size() < 2)
    {
        Log(lWarning)<<"You have not made a selection. No data is selected";
        return 0;
    }
    return mSelectionList.size();
}

ModelGenerator* RoadRunner::getModelGenerator()
{
    return mModelGenerator;
}

//NOM exposure ====================================================
string RoadRunner::getParamPromotedSBML(const string& sArg)
{
    return NOMSupport::getParamPromotedSBML(sArg);
}


bool RoadRunner::initializeModel()
{
    if(mModel)
    {
        mConservedTotalChanged = false;
        mModel->setCompartmentVolumes();
        mModel->initializeInitialConditions();
        mModel->setParameterValues();
        mModel->setCompartmentVolumes();
        mModel->setBoundaryConditions();
        mModel->setInitialConditions();
        mModel->convertToAmounts();
        mModel->evalInitialAssignments();

        mModel->computeRules(mModel->getModelData().y, mModel->getModelData().ySize);
        mModel->convertToAmounts();

        if (mComputeAndAssignConservationLaws.getValue())
        {
            mModel->computeConservedTotals();
        }

        if(mCVode)
        {
            delete mCVode;
        }
        mCVode = new CvodeInterface(this, mModel);
        mModel->assignCVodeInterface(mCVode);

        // reset the simulation state
        reset();
        return true;
    }
    else
    {
        return false;
    }
}

RoadRunnerData RoadRunner::getSimulationResult()
{
    return mRoadRunnerData;
}

double RoadRunner::getValueForRecord(const SelectionRecord& record)
{
    double dResult;

    switch (record.selectionType)
    {
        case SelectionType::clFloatingSpecies:
            dResult = mModel->getConcentration(record.index);
        break;

        case SelectionType::clBoundarySpecies:
            dResult = mModel->getModelData().bc[record.index];
        break;

        case SelectionType::clFlux:
            dResult = mModel->getModelData().rates[record.index];
        break;

        case SelectionType::clRateOfChange:
            dResult = mModel->getModelData().dydt[record.index];
        break;

        case SelectionType::clVolume:
            dResult = mModel->getModelData().c[record.index];
        break;

        case SelectionType::clParameter:
            {
                if (record.index > ((mModel->getModelData().gpSize) - 1))
                {
                    dResult = mModel->getModelData().ct[record.index - (mModel->getModelData().gpSize)];
                }
                else
                {
                    dResult = mModel->getModelData().gp[record.index];
                }
            }
        break;

        case SelectionType::clFloatingAmount:
            dResult = mModel->getModelData().amounts[record.index];
        break;

        case SelectionType::clBoundaryAmount:
            int nIndex;
            if (mModel->getCompartments().find(mModel->getBoundarySpecies()[record.index].compartmentName, nIndex))
            {
                dResult = mModel->getModelData().bc[record.index] * mModel->getModelData().c[nIndex];
            }
            else
            {
                dResult = 0.0;
            }
        break;

        case SelectionType::clElasticity:
            dResult = getEE(record.p1, record.p2, false);
        break;

        case SelectionType::clUnscaledElasticity:
            dResult = getuEE(record.p1, record.p2, false);
        break;

        // ********  Todo: Enable this.. ***********
        case SelectionType::clEigenValue:
//            vector< complex<double> >oComplex = LA.GetEigenValues(getReducedJacobian());
//            if (oComplex.Length > record.index)
//            {
//                dResult = oComplex[record.index].Real;
//            }
//            else
//                dResult = Double.NaN;
                dResult = 0.0;
        break;

        case SelectionType::clStoichiometry:
            dResult = mModel->getModelData().sr[record.index];
        break;

        default:
            dResult = 0.0;
        break;
    }
    return dResult;
}

double RoadRunner::getNthSelectedOutput(const int& index, const double& dCurrentTime)
{
    SelectionRecord record = mSelectionList[index];

    if (record.selectionType == SelectionType::clTime)
    {
        return dCurrentTime;
    }
    else
    {
        return getValueForRecord(record);
    }
}

void RoadRunner::addNthOutputToResult(DoubleMatrix& results, int nRow, double dCurrentTime)
{
    stringstream msg;
    for (u_int j = 0; j < mSelectionList.size(); j++)
    {
        double out =  getNthSelectedOutput(j, dCurrentTime);
        results(nRow,j) = out;
        msg<<gTab<<out;
    }
    Log(lDebug1)<<"Added result row\t"<<nRow<<" : "<<msg.str();
}

vector<double> RoadRunner::buildModelEvalArgument()
{
    vector<double> dResult;
    dResult.resize((mModel->getModelData().amountsSize) + (mModel->getModelData().rateRulesSize) );

    vector<double> dCurrentRuleValues = mModel->getCurrentValues();

    for(int i = 0; i < (mModel->getModelData().rateRulesSize); i++)
    {
        dResult[i] = dCurrentRuleValues[i];
    }

    for(int i = 0; i < (mModel->getModelData().amountsSize); i++)
    {
        dResult[i + (mModel->getModelData().rateRulesSize)] = mModel->getModelData().amounts[i];
    }

    return dResult;
}

DoubleMatrix RoadRunner::runSimulation()
{
    if (mNumPoints <= 1)
    {
        mNumPoints = 2;
    }

    double hstep = (mTimeEnd - mTimeStart) / (mNumPoints - 1);
    int nrCols = mSelectionList.size();
    if(!nrCols)
    {
        nrCols = createDefaultTimeCourseSelectionList();
    }

    DoubleMatrix results(mNumPoints, nrCols);

    if(!mModel)
    {
        return results;
    }

    vector<double> y;
    y = buildModelEvalArgument();
    mModel->evalModel(mTimeStart, y);
    addNthOutputToResult(results, 0, mTimeStart);

    //Todo: Don't understand this code.. MTK
    if (mCVode->haveVariables())
    {
        mCVode->reStart(mTimeStart, mModel);
    }

    double tout = mTimeStart;

    //The simulation is executed right here..
    Log(lDebug)<<"Will run the OneStep function "<<mNumPoints<<" times";
    for (int i = 1; i < mNumPoints; i++)
    {
        Log(lDebug)<<"Step "<<i;
        mCVode->oneStep(tout, hstep);
        tout = mTimeStart + i * hstep;
        addNthOutputToResult(results, i, tout);
    }
    Log(lDebug)<<"Simulation done..";
    Log(lDebug2)<<"Result: (point, time, value)";
    if(results.size())
    {
        for (int i = 0; i < mNumPoints; i++)
        {
            Log(lDebug2)<<i<<gTab<<results(i,0)<<gTab<<setprecision(16)<<results(i,1);
        }
    }
    return results;
}

bool RoadRunner::simulateSBMLFile(const string& fileName, const bool& useConservationLaws)
{
    computeAndAssignConservationLaws(useConservationLaws);

    string mModelXMLFileName = fileName;
    ifstream fs(mModelXMLFileName.c_str());
    if(!fs)
    {
        throw(Exception("Failed to open the model file:" + mModelXMLFileName));
    }

    Log(lInfo)<<"\n\n ===== Reading model file: " <<mModelXMLFileName <<" ==============";
    string sbml((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    Log(lDebug5)<<"Loading SBML. SBML model code size: "<<sbml.size();
    mCurrentSBMLFileName = fileName;
    loadSBML(sbml);

    mRawRoadRunnerData = simulate();

    StringList list = getTimeCourseSelectionList();
    return true;
}

bool RoadRunner::loadSBMLFromFile(const string& fileName, const bool& forceReCompile)
{
	if(!fileExists(fileName))
    {
        stringstream msg;
        msg<<"File: "<<fileName<<" don't exist";
        Log(lError)<<msg.str();
        return false;
    }

    ifstream ifs(fileName.c_str());
    if(!ifs)
    {
        stringstream msg;
        msg<<"Failed opening file: "<<fileName;
        Log(lError)<<msg.str();
        return false;
    }

    std::string sbml((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    ifs.close();
    Log(lDebug5)<<"Read SBML content from file:\n "<<sbml \
                << "\n============ End of SBML "<<endl;

    mCurrentSBMLFileName = fileName;
    return loadSBML(sbml, forceReCompile);
}

bool RoadRunner::loadSBMLIntoNOM(const string& sbml)
{
    string sASCII = NOMSupport::convertTime(sbml, "time");

    Log(lDebug4)<<"Loading SBML into NOM";
    mNOM.loadSBML(sASCII.c_str(), "time");
    return true;
}

bool RoadRunner::loadSBMLIntoLibStruct(const string& sbml)
{
    Log(lDebug3)<<"Loading sbml into StructAnalysis";
    string msg = mLS.loadSBML(sbml);            //the ls loadSBML load call took SASCII before.. does it need to?
    Log(lDebug1)<<"Message from StructAnalysis.LoadSBML function\n"<<msg;
    return msg.size() ? true : false;
}

string RoadRunner::createModelName(const string& mCurrentSBMLFileName)
{
    //Generate source code for the model
    string modelName;
    if(mCurrentSBMLFileName.size())
    {
    	modelName = getFileNameNoExtension(mCurrentSBMLFileName);
    }
    else
    {
		modelName = toString(mInstanceID);
    }
    return modelName;
}

bool cleanFolder(const string& folder, const string& baseName, const StringList& extensions)
{
    for(int i = 0; i < extensions.Count(); i++)
       {
    	string aFName = joinPath(folder, baseName) + "." + extensions[i];
        Poco::File aFile(aFName);
        if(aFile.exists())
        {
            aFile.remove();
        }
    }
    return true;
}

bool RoadRunner::loadSBML(const string& sbml, const bool& forceReCompile)
{
    mCurrentSBML = sbml;

    //clear temp folder of roadrunner generated files, only if roadRunner instance == 1
    Log(lDebug)<<"Loading SBML into simulator";
    if (!sbml.size())
    {
        throw(CoreException("SBML string is empty!"));
    }

    loadSBMLIntoLibStruct(sbml);
    {    //Scope for Mutex
           Mutex::ScopedLock lock(mLibSBMLMutex);
        loadSBMLIntoNOM(sbml);    //There is something in here that is not threadsafe... causes crash with multiple threads, without mutex
    }

    delete mModel;
    mModel = mModelGenerator->createModel(sbml, &mLS, &mNOM, forceReCompile, computeAndAssignConservationLaws());

    //Finally intitilaize the model..
    if(!initializeModel())
    {
        Log(lError)<<"Failed Initializing C Model";
        return false;
    }

    createDefaultSelectionLists();
    return true;
}

bool RoadRunner::createDefaultSelectionLists()
{
    bool result = true;

    //Create a default timecourse selectionlist
    if(!createDefaultTimeCourseSelectionList())
    {
        Log(lDebug)<<"Failed creating default timecourse selectionList.";
        result = false;
    }
    else
    {
        Log(lDebug)<<"Created default TimeCourse selection list.";
    }

    //Create a defualt steady state selectionlist
    if(!createDefaultSteadyStateSelectionList())
    {
        Log(lDebug)<<"Failed creating default steady state selectionList.";
        result = false;
    }
    else
    {
        Log(lDebug)<<"Created default SteadyState selection list.";
    }
    return result;
}

bool RoadRunner::loadSimulationSettings(const string& fName)
{
    if(!mSettings.LoadFromFile(fName))
    {
        Log(lError)<<"Failed loading settings from file:" <<fName;
        return false;
    }

    useSimulationSettings(mSettings);

    //This one creates the list of what we will look at in the result
     createTimeCourseSelectionList();
    return true;
}

bool RoadRunner::unLoadModel()
{
    // The model owns the shared library (if it exists), when the model is deleted,
    // its dtor unloads the shared lib.
    if(mModel)
    {
        delete mModel;
        mModel = NULL;
        return true;
    }
    return false;
}

//Reset the simulator back to the initial conditions specified in the SBML model
void RoadRunner::reset()
{
    if (mModel)
    {
        mModel->setTime(0.0);

        // Reset the event flags
        mModel->resetEvents();
        mModel->setCompartmentVolumes();
        mModel->setInitialConditions();
        mModel->convertToAmounts();

        // in case we have ODE rules we should assign those as initial values
        mModel->initializeRateRuleSymbols();
        mModel->initializeRates();

        // and of course initial assignments should override anything
        mModel->evalInitialAssignments();
        mModel->convertToAmounts();

        // also we might need to set some initial assignment rules.
        mModel->convertToConcentrations();
        mModel->computeRules(mModel->getModelData().y, mModel->getModelData().ySize);
        mModel->initializeRates();
        mModel->initializeRateRuleSymbols();
        mModel->evalInitialAssignments();
        mModel->computeRules(mModel->getModelData().y, mModel->getModelData().ySize);

        mModel->convertToAmounts();

        if (mComputeAndAssignConservationLaws.getValue() && !mConservedTotalChanged)
        {
            mModel->computeConservedTotals();
        }

        mCVode->assignNewVector(mModel, true);
        mCVode->testRootsAtInitialTime();

        mModel->setTime(0.0);
        mCVode->reStart(0.0, mModel);

        mCVode->mAssignments.clear();//Clear();

        try
        {
            mModel->testConstraints();
        }
        catch (const Exception& e)
        {
            Log(lWarning)<<"Constraint Violated at time = 0\n"<<e.Message();
        }
    }
}

DoubleMatrix RoadRunner::simulate()
{
    try
    {
        if (!mModel)
        {
            throw Exception(gEmptyModelMessage);
        }

        if (mTimeEnd <= mTimeStart)
        {
            throw Exception("Error: time end must be greater than time start");
        }
    	return runSimulation();
    }
    catch (const Exception& e)
    {
        Log(lWarning)<<"Problem in simulate: "<<e.Message();
    }

}

bool RoadRunner::simulate2()
{
    if(!mModel)
    {
        Log(lError)<<"No model is loaded, can't simulate..";
        throw(Exception("There is no model loaded, can't simulate"));
    }

 	mRawRoadRunnerData = simulateEx(mTimeStart, mTimeEnd, mNumPoints);

    //Populate simulation result
    populateResult();
    return true;
}

bool RoadRunner::simulate2Ex(const double& startTime, const double& endTime, const int& numberOfPoints)
{
    if(!mModel)
    {
        Log(lError)<<"No model is loaded, can't simulate..";
        throw(Exception("There is no model loaded, can't simulate"));
    }

 	mRawRoadRunnerData = simulateEx(startTime, endTime, numberOfPoints);

    //Populate simulation result
    populateResult();
    return true;
}

bool RoadRunner::populateResult()
{
    StringList list = getTimeCourseSelectionList();

    mRoadRunnerData.setColumnNames(list);
    mRoadRunnerData.setData(mRawRoadRunnerData);
    return true;
}

// Help("Extension method to simulate (time start, time end, number of points). This routine resets the model to its initial condition before running the simulation (unlike simulate())"
DoubleMatrix RoadRunner::simulateEx(const double& startTime, const double& endTime, const int& numberOfPoints)
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        reset(); // reset back to initial conditions

        if (endTime < 0 || startTime < 0 || numberOfPoints <= 0 || endTime <= startTime)
        {
            throw CoreException("Illegal input to simulateEx");
        }

        mTimeEnd            = endTime;
        mTimeStart          = startTime;
        mNumPoints          = numberOfPoints;
        mRawRoadRunnerData  = runSimulation();
        populateResult();
        return mRawRoadRunnerData;
    }
    catch(const Exception& e)
    {
        throw CoreException("Unexpected error from simulateEx()", e.Message());
    }
}

//Returns the currently selected columns that will be returned by calls to simulate() or simulateEx(,,).
StringList RoadRunner::getTimeCourseSelectionList()
{
    StringList oResult;

    if (!mModel)
    {
        oResult.add("time");
        return oResult;
    }

    StringList oFloating    = mModel->getFloatingSpeciesConcentrationNames();
    StringList oBoundary    = mModel->getBoundarySpeciesNames();
    StringList oFluxes      = mModel->getReactionNames();
    StringList oVolumes     = mModel->getCompartmentNames();
    StringList oRates       = getRateOfChangeIds();
    StringList oParameters  = getParameterIds();

    vector<SelectionRecord>::iterator iter;

    for(iter = mSelectionList.begin(); iter != mSelectionList.end(); iter++)
    {
        SelectionRecord record = (*iter);
        switch (record.selectionType)
        {
            case SelectionType::clTime:
                oResult.add("time");
                break;
            case SelectionType::clBoundaryAmount:
                oResult.add(format("[{0}]", oBoundary[record.index]));
                break;
            case SelectionType::clBoundarySpecies:
                oResult.add(oBoundary[record.index]);
                break;
            case SelectionType::clFloatingAmount:
                oResult.add(format("[{0}]", oFloating[record.index]));
                break;
            case SelectionType::clFloatingSpecies:
                oResult.add(oFloating[record.index]);
                break;
            case SelectionType::clVolume:
                oResult.add(oVolumes[record.index]);
                break;
            case SelectionType::clFlux:
                oResult.add(oFluxes[record.index]);
                break;
            case SelectionType::clRateOfChange:
                oResult.add(oRates[record.index]);
                break;
            case SelectionType::clParameter:
                oResult.add(oParameters[record.index]);
                break;
            case SelectionType::clEigenValue:
                oResult.add("eigen_" + record.p1);
                break;
            case SelectionType::clElasticity:
                oResult.add(format("EE:{0},{1}", record.p1, record.p2));
                break;
            case SelectionType::clUnscaledElasticity:
                oResult.add(format("uEE:{0},{1}", record.p1, record.p2));
                break;
            case SelectionType::clStoichiometry:
                oResult.add(record.p1);
                break;
        }
    }
    return oResult;
}

// Help("Compute the steady state of the model, returns the sum of squares of the solution")
double RoadRunner::steadyState()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (mUseKinsol)
    {
            //mSteadyStateSolver = NULL;//new KinSolveInterface(mModel);
            Log(lError)<<"Kinsol solver is not enabled...";
            return -1;
    }
    else
    {
        mSteadyStateSolver = new NLEQInterface(mModel);
    }

    //Get a std vector for the solver
    vector<double> someAmounts;
    copyCArrayToStdVector(mModel->getModelData().amounts, someAmounts, mModel->getNumIndependentVariables());

    double ss = mSteadyStateSolver->solve(someAmounts);
    if(ss < 0)
    {
        Log(lError)<<"Steady State solver failed...";
    }
    mModel->convertToConcentrations();

    delete mSteadyStateSolver;
    mSteadyStateSolver = NULL;

    return ss;
}

void RoadRunner::setParameterValue(const TParameterType& parameterType, const int& parameterIndex, const double& value)
{
    switch (parameterType)
    {
        case TParameterType::ptBoundaryParameter:
            mModel->getModelData().bc[parameterIndex] = value;
        break;

        case TParameterType::ptGlobalParameter:
            mModel->getModelData().gp[parameterIndex] = value;
        break;

        case TParameterType::ptFloatingSpecies:
            mModel->getModelData().y[parameterIndex] = value;
        break;

        case TParameterType::ptConservationParameter:
            mModel->getModelData().ct[parameterIndex] = value;
        break;

        case TParameterType::ptLocalParameter:
            throw Exception("Local parameters not permitted in setParameterValue (getCC, getEE)");
    }
}

double RoadRunner::getParameterValue(const TParameterType& parameterType, const int& parameterIndex)
{
    switch (parameterType)
    {
        case TParameterType::ptBoundaryParameter:
            return mModel->getModelData().bc[parameterIndex];

        case TParameterType::ptGlobalParameter:
            return mModel->getModelData().gp[parameterIndex];

        // Used when calculating elasticities
        case TParameterType::ptFloatingSpecies:
            return mModel->getModelData().y[parameterIndex];

        case TParameterType::ptConservationParameter:
            return mModel->getModelData().ct[parameterIndex];

        case TParameterType::ptLocalParameter:
            throw Exception("Local parameters not permitted in getParameterValue (getCC?)");

        default:
            return 0.0;
    }
}

// Help("This method turns on / off the computation and adherence to conservation laws."
//              + "By default roadRunner will discover conservation cycles and reduce the model accordingly.")
void RoadRunner::computeAndAssignConservationLaws(const bool& bValue)
{
	if(bValue == mComputeAndAssignConservationLaws.getValue())
    {
    	Log(lWarning)<<"The compute and assign conservation laws flag already set to : "<<toString(bValue);
    }

    mComputeAndAssignConservationLaws.setValue(bValue);

    if(mModel != NULL)
    {
        if(!loadSBML(mCurrentSBML, true))
        {
            throw( CoreException("Failed re-Loading model when setting computeAndAssignConservationLaws"));
        }
//        if(!generateModelCode())
//        {
//            throw("Failed generating model from SBML when trying to set computeAndAssignConservationLaws");
//        }
//
//        //We need no recompile the model if this flag changes..
//        if(!compileModel())
//        {
//            throw( CoreException("Failed compiling model when trying to set computeAndAssignConservationLaws"));
//        }
//
//        //Then we have to reinit the model..

    }
}

// Help("Returns the names given to the rate of change of the floating species")
StringList RoadRunner::getRateOfChangeIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    StringList sp = mModel->getFloatingSpeciesConcentrationNames(); // Reordered list
    for (int i = 0; i < sp.Count(); i++)
    {
        sp[i] = sp[i] + "'";
    }
    return sp;
}

// Help("Gets the list of compartment names")
StringList RoadRunner::getCompartmentIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getCompartmentNames();
}

StringList RoadRunner::getParameterIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    StringList sp = mModel->getGlobalParameterNames(); // Reordered list
    return sp;
}

// [Help("Get scaled elasticity coefficient with respect to a global parameter or species")]
double RoadRunner::getEE(const string& reactionName, const string& parameterName)
{
    return getEE(reactionName, parameterName, true);
}

// [Help("Get scaled elasticity coefficient with respect to a global parameter or species. Optionally the model is brought to steady state after the computation.")]
double RoadRunner::getEE(const string& reactionName, const string& parameterName, bool computeSteadyState)
{
    TParameterType parameterType;
    int reactionIndex;
    int parameterIndex;

    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    // Check the reaction name
    if (!mModel->getReactions().find(reactionName, reactionIndex))
    {
        throw CoreException(format("Unable to locate reaction name: [{0}]", reactionName));
    }

    // Find out what kind of parameter we are dealing with
    if (mModel->getFloatingSpeciesConcentrations().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptFloatingSpecies;
    }
    else if (mModel->getBoundarySpecies().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptBoundaryParameter;
    }
    else if (mModel->getGlobalParameters().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptGlobalParameter;
    }
    else if (mModel->getConservations().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptConservationParameter;
    }
    else
    {
        throw CoreException(format("Unable to locate variable: [{0}]", parameterName));
    }

    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
    double variableValue = mModel->getModelData().rates[reactionIndex];
    double parameterValue = getParameterValue(parameterType, parameterIndex);
    if (variableValue == 0)
    {
        variableValue = 1e-12;
    }
    return getuEE(reactionName, parameterName, computeSteadyState) * parameterValue / variableValue;
}

//        [Help("Get unscaled elasticity coefficient with respect to a global parameter or species")]
double RoadRunner::getuEE(const string& reactionName, const string& parameterName)
{
    return getuEE(reactionName, parameterName, true);
}

class aFinalizer
{
    private:
        TParameterType     mParameterType;
        int                 mParameterIndex;
        double             mOriginalParameterValue;
        bool             mComputeSteadyState;
        RoadRunner*     mRR;

    public:
                        aFinalizer(TParameterType& pType, const int& pIndex, const double& origValue, const bool& doWhat, RoadRunner* aRoadRunner)
                        :
                        mParameterType(pType),
                        mParameterIndex(pIndex),
                        mOriginalParameterValue(origValue),
                        mComputeSteadyState(doWhat),
                        mRR(aRoadRunner)
                        {}

                        ~aFinalizer()
                        {
                            //this is a finally{} code block
                            // What ever happens, make sure we restore the parameter level
                            mRR->setParameterValue(mParameterType, mParameterIndex, mOriginalParameterValue);
                            mRR->getModel()->computeReactionRates(mRR->getModel()->getTime(), mRR->getModel()->getModelData().y);
                            if (mComputeSteadyState)
                            {
                                mRR->steadyState();
                            }
                        }
};

//[Help("Get unscaled elasticity coefficient with respect to a global parameter or species. Optionally the model is brought to steady state after the computation.")]
double RoadRunner::getuEE(const string& reactionName, const string& parameterName, bool computeSteadystate)
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        TParameterType parameterType;
        double originalParameterValue;
        int reactionIndex;
        int parameterIndex;

        mModel->convertToConcentrations();
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);

        // Check the reaction name
        if (!mModel->getReactions().find(reactionName, reactionIndex))
        {
            throw CoreException("Unable to locate reaction name: [" + reactionName + "]");
        }

        // Find out what kind of parameter we are dealing with
        if (mModel->getFloatingSpeciesConcentrations().find(parameterName, parameterIndex))
        {
            parameterType = TParameterType::ptFloatingSpecies;
            originalParameterValue = mModel->getModelData().y[parameterIndex];
        }
        else if (mModel->getBoundarySpecies().find(parameterName, parameterIndex))
        {
            parameterType = TParameterType::ptBoundaryParameter;
            originalParameterValue = mModel->getModelData().bc[parameterIndex];
        }
        else if (mModel->getGlobalParameters().find(parameterName, parameterIndex))
        {
            parameterType = TParameterType::ptGlobalParameter;
            originalParameterValue = mModel->getModelData().gp[parameterIndex];
        }
        else if (mModel->getConservations().find(parameterName, parameterIndex))
        {
            parameterType = TParameterType::ptConservationParameter;
            originalParameterValue = mModel->getModelData().ct[parameterIndex];
        }
        else
        {
            throw CoreException("Unable to locate variable: [" + parameterName + "]");
        }

        double hstep = mDiffStepSize*originalParameterValue;
        if (fabs(hstep) < 1E-12)
        {
            hstep = mDiffStepSize;
        }

        aFinalizer a(parameterType, parameterIndex, originalParameterValue, mModel, this);
        mModel->convertToConcentrations();

        setParameterValue(parameterType, parameterIndex, originalParameterValue + hstep);
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        double fi = mModel->getModelData().rates[reactionIndex];

        setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        double fi2 = mModel->getModelData().rates[reactionIndex];

        setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        double fd = mModel->getModelData().rates[reactionIndex];

        setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        double fd2 = mModel->getModelData().rates[reactionIndex];

        // Use instead the 5th order approximation double unscaledValue = (0.5/hstep)*(fi-fd);
        // The following separated lines avoid small amounts of roundoff error
        double f1 = fd2 + 8*fi;
        double f2 = -(8*fd + fi2);

        return 1/(12*hstep)*(f1 + f2);
    }
    catch(const Exception& e)
    {
        throw CoreException("Unexpected error from getuEE(): " +  e.Message());
    }
}

// Help("Updates the model based on all recent changes")
void RoadRunner::evalModel()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    mModel->convertToAmounts();
    vector<double> args = mCVode->buildEvalArgument();
    mModel->evalModel(mModel->getTime(), args);
}

void RoadRunner::setTimeCourseSelectionList(const string& list)
{
    StringList aList(list,", ");
    setTimeCourseSelectionList(aList);
}

// Help("Set the columns to be returned by simulate() or simulateEx(), valid symbol names include" +
//              " time, species names, , volume, reaction rates and rates of change (speciesName')")
void RoadRunner::setTimeCourseSelectionList(const StringList& _selList)
{
    mSelectionList.clear();
    StringList newSelectionList(_selList);
    StringList fs = mModel->getFloatingSpeciesConcentrationNames();
    StringList bs = mModel->getBoundarySpeciesNames();
    StringList rs = mModel->getReactionNames();
    StringList vol= mModel->getCompartmentNames();
    StringList gp = mModel->getGlobalParameterNames();
//    StringList sr = mModelGenerator->ModifiableSpeciesReferenceList;

    for (int i = 0; i < _selList.Count(); i++)
    {
    	if (toUpper(newSelectionList[i]) == toUpper("time"))
        {
        	mSelectionList.push_back(SelectionRecord(0, clTime));
        }

        // Check for species
        for (int j = 0; j < fs.Count(); j++)
        {
            if (newSelectionList[i] == fs[j])
            {
               	mSelectionList.push_back(SelectionRecord(j, SelectionType::clFloatingSpecies));
                break;
            }

            if (newSelectionList[i] == "[" + fs[j] + "]")
            {
               	mSelectionList.push_back(SelectionRecord(j, clFloatingAmount));
                break;
            }

            // Check for species rate of change
            if (newSelectionList[i] == fs[j] + "'")
            {
                mSelectionList.push_back(SelectionRecord(j, clRateOfChange));
                break;
            }
        }

        // Check fgr boundary species
        for (int j = 0; j < bs.Count(); j++)
        {
            if (newSelectionList[i] == bs[j])
            {
                mSelectionList.push_back(SelectionRecord(j, clBoundarySpecies));
                break;
            }
            if (newSelectionList[i] == "[" + bs[j] + "]")
            {
                mSelectionList.push_back(SelectionRecord(j, clBoundaryAmount));
                break;
            }
        }

        for (int j = 0; j < rs.Count(); j++)
        {
            // Check for reaction rate
            if (newSelectionList[i] == rs[j])
            {
                mSelectionList.push_back(SelectionRecord(j, clFlux));
                break;
            }
        }

        for (int j = 0; j < vol.Count(); j++)
        {
            // Check for volume
            if (newSelectionList[i] == vol[j])
            {
                mSelectionList.push_back(SelectionRecord(j, clVolume));
                break;
            }

            if (newSelectionList[i] == "[" + vol[j] + "]")
            {
                mSelectionList.push_back(SelectionRecord(j, clVolume));
                break;
            }
        }

        for (int j = 0; j < gp.Count(); j++)
        {
            if (newSelectionList[i] == gp[j])
            {
                mSelectionList.push_back(SelectionRecord(j, clParameter));
                break;
            }
        }

        //((string)newSelectionList[i]).StartsWith("eigen_")
        string tmp = newSelectionList[i];
        if (startsWith(tmp, "eigen_"))
        {
            string species = tmp.substr(tmp.find_last_of("eigen_") + 1);
            mSelectionList.push_back(SelectionRecord(i, clEigenValue, species));
//            mSelectionList[i].selectionType = SelectionType::clEigenValue;
//            mSelectionList[i].p1 = species;
            int aIndex = fs.find(species);
            mSelectionList[i].index = aIndex;
            //mModelGenerator->floatingSpeciesConcentrationList.find(species, mSelectionList[i].index);
        }

//        if (((string)newSelectionList[i]).StartsWith("EE:"))
//        {
//            string parameters = ((string)newSelectionList[i]).Substring(3);
//            var p1 = parameters.Substring(0, parameters.IndexOf(","));
//            var p2 = parameters.Substring(parameters.IndexOf(",") + 1);
//            mSelectionList[i].selectionType = SelectionType::clElasticity;
//            mSelectionList[i].p1 = p1;
//            mSelectionList[i].p2 = p2;
//        }
//
//        if (((string)newSelectionList[i]).StartsWith("uEE:"))
//        {
//            string parameters = ((string)newSelectionList[i]).Substring(4);
//            var p1 = parameters.Substring(0, parameters.IndexOf(","));
//            var p2 = parameters.Substring(parameters.IndexOf(",") + 1);
//            mSelectionList[i].selectionType = SelectionType::clUnscaledElasticity;
//            mSelectionList[i].p1 = p1;
//            mSelectionList[i].p2 = p2;
//        }
//        if (((string)newSelectionList[i]).StartsWith("eigen_"))
//        {
//            var species = ((string)newSelectionList[i]).Substring("eigen_".Length);
//            mSelectionList[i].selectionType = SelectionType::clEigenValue;
//            mSelectionList[i].p1 = species;
//            mModelGenerator->floatingSpeciesConcentrationList.find(species, out mSelectionList[i].index);
//        }
//
//        int index;
//        if (sr.find((string)newSelectionList[i], out index))
//        {
//            mSelectionList[i].selectionType = SelectionType::clStoichiometry;
//            mSelectionList[i].index = index;
//            mSelectionList[i].p1 = (string) newSelectionList[i];
//        }
    }
}

// Help(
// "Carry out a single integration step using a stepsize as indicated in the method call (the intergrator is reset to take into account all variable changes). Arguments: double CurrentTime, double StepSize, Return Value: new CurrentTime."
//            )
double RoadRunner::oneStep(const double& currentTime, const double& stepSize)
{
    return oneStep(currentTime, stepSize, true);
}

//Help(
//   "Carry out a single integration step using a stepsize as indicated in the method call. Arguments: double CurrentTime, double StepSize, bool: reset integrator if true, Return Value: new CurrentTime."
//   )
double RoadRunner::oneStep(const double& currentTime, const double& stepSize, const bool& reset)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (reset)
    {
        mCVode->reStart(currentTime, mModel);
    }
    return mCVode->oneStep(currentTime, stepSize);
}

// Returns eigenvalues, first column real part, second column imaginary part
// -------------------------------------------------------------------------
DoubleMatrix RoadRunner::getEigenvalues()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        vector<Complex> vals = getEigenvaluesCpx();

        DoubleMatrix result(vals.size(), 2);

        for (int i = 0; i < vals.size(); i++)
        {
            result[i][0] = real(vals[i]);
            result[i][1] = imag(vals[i]);
        }
        return result;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getEigenvalues()", e.Message());
    }
}

// Returns eigenvalues, first column real part, second column imaginary part
// -------------------------------------------------------------------------
DoubleMatrix RoadRunner::getEigenvaluesFromMatrix (DoubleMatrix m)
{
    try
    {
        vector<Complex> vals = ls::getEigenValues(m);

        DoubleMatrix result(vals.size(), 2);

        for (int i = 0; i < vals.size(); i++)
        {
            result[i][0] = real(vals[i]);
            result[i][1] = imag(vals[i]);
        }
        return result;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getEigenvalues()", e.Message());
    }
}

vector< Complex > RoadRunner::getEigenvaluesCpx()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        DoubleMatrix mat;
		if (mComputeAndAssignConservationLaws.getValue())
        {
           mat = getReducedJacobian();
        }
        else
        {
           mat = getFullJacobian();
        }
        return ls::getEigenValues(mat);
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getEigenvalues()", e.Message());
    }
}

// Help("Compute the full Jacobian at the current operating point")
DoubleMatrix RoadRunner::getFullJacobian()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }
        DoubleMatrix uelast = getUnscaledElasticityMatrix();
        DoubleMatrix rsm;
		if (mComputeAndAssignConservationLaws.getValue())
        {
            rsm = getReorderedStoichiometryMatrix();
        }
        else
        {
            rsm = getStoichiometryMatrix();
        }
       return mult(rsm, uelast);

    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from fullJacobian()", e.Message());
    }
}

DoubleMatrix RoadRunner::getFullReorderedJacobian()
{
    try
    {
        if (mModel)
        {
            DoubleMatrix uelast = getUnscaledElasticityMatrix();
            DoubleMatrix rsm     = getStoichiometryMatrix();
            return mult(rsm, uelast);
        }
        throw CoreException(gEmptyModelMessage);
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from fullJacobian()", e.Message());
    }
}

// Help("Compute the reduced Jacobian at the current operating point")
DoubleMatrix RoadRunner::getReducedJacobian()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        if(mComputeAndAssignConservationLaws.getValue() == false)
        {
            throw CoreException("The reduced Jacobian matrix can only be computed if conservation law detection is enabled");
        }

        DoubleMatrix uelast = getUnscaledElasticityMatrix();
        if(!mLS.getNrMatrix())
        {
            return DoubleMatrix(0,0);
        }
        DoubleMatrix I1 = mult(*(mLS.getNrMatrix()), uelast);
        DoubleMatrix *linkMat = mLS.getLinkMatrix();
        return mult(I1, (*linkMat));
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getReducedJacobian(): ", e.Message());
    }
}

// ---------------------------------------------------------------------
// Start of Level 4 API Methods
// ---------------------------------------------------------------------
DoubleMatrix* RoadRunner::getLinkMatrix()
{
    try
    {
       if (!mModel)
       {
           throw CoreException(gEmptyModelMessage);
       }
       //return _L;
        return mLS.getLinkMatrix();
    }
    catch (const Exception& e)
    {
         throw CoreException("Unexpected error from getLinkMatrix()", e.Message());
    }
}

DoubleMatrix* RoadRunner::getNrMatrix()
{
    try
    {
       if (!mModel)
       {
            throw CoreException(gEmptyModelMessage);
       }
        //return _Nr;
        return mLS.getNrMatrix();
    }
    catch (const Exception& e)
    {
         throw CoreException("Unexpected error from getNrMatrix()", e.Message());
    }
}

DoubleMatrix* RoadRunner::getL0Matrix()
{
    try
    {
       if (!mModel)
       {
            throw CoreException(gEmptyModelMessage);
       }
          //return _L0;
       return mLS.getL0Matrix();
    }
    catch (const Exception& e)
    {
         throw CoreException("Unexpected error from getL0Matrix()", e.Message());
    }
}

// Help("Returns the stoichiometry matrix for the currently loaded model")
DoubleMatrix RoadRunner::getStoichiometryMatrix()
{
    try
    {
//        DoubleMatrix* aMat = mLS.getStoichiometryMatrix();
        DoubleMatrix* aMat = mLS.getReorderedStoichiometryMatrix();
        if (!mModel || !aMat)
        {
            throw CoreException(gEmptyModelMessage);
        }

        DoubleMatrix mat(aMat->numRows(), aMat->numCols());

        for(int row = 0; row < mat.RSize(); row++)
        {
            for(int col = 0; col < mat.CSize(); col++)
            {
                mat(row,col) = (*aMat)(row,col);
            }
        }
        return mat;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getStoichiometryMatrix()" + e.Message());
    }
}

// Help("Returns the stoichiometry matrix for the currently loaded model")
DoubleMatrix RoadRunner::getReorderedStoichiometryMatrix()
{
    try
    {
        DoubleMatrix* aMat = mLS.getReorderedStoichiometryMatrix();
        if (!mModel || !aMat)
        {
            throw CoreException(gEmptyModelMessage);
        }

        //Todo: Room to improve how matrices are handled across LibStruct/RoadRunner/C-API
        DoubleMatrix mat(aMat->numRows(), aMat->numCols());

        for(int row = 0; row < mat.RSize(); row++)
        {
            for(int col = 0; col < mat.CSize(); col++)
            {
                mat(row,col) = (*aMat)(row,col);
            }
        }
        return mat;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getStoichiometryMatrix()" + e.Message());
    }
}

// Help("Returns the stoichiometry matrix for the currently loaded model")
DoubleMatrix RoadRunner::getFullyReorderedStoichiometryMatrix()
{
    try
    {
        DoubleMatrix* aMat = mLS.getFullyReorderedStoichiometryMatrix();
        if (!mModel || !aMat)
        {
            throw CoreException(gEmptyModelMessage);
        }

        //Todo: Room to improve how matrices are handled across LibStruct/RoadRunner/C-API
        DoubleMatrix mat(aMat->numRows(), aMat->numCols());

        for(int row = 0; row < mat.RSize(); row++)
        {
            for(int col = 0; col < mat.CSize(); col++)
            {
                mat(row,col) = (*aMat)(row,col);
            }
        }
        return mat;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getStoichiometryMatrix()" + e.Message());
    }
}

DoubleMatrix RoadRunner::getConservationMatrix()
{
    DoubleMatrix mat;

    try
    {
       if (mModel)
       {
           DoubleMatrix* aMat = mLS.getGammaMatrix();
            if (aMat)
            {
                mat.resize(aMat->numRows(), aMat->numCols());
                for(int row = 0; row < mat.RSize(); row++)
                {
                    for(int col = 0; col < mat.CSize(); col++)
                    {
                        mat(row,col) = (*aMat)(row,col);
                    }
                }
            }
            return mat;

       }
       throw CoreException(gEmptyModelMessage);
    }
    catch (const Exception& e)
    {
         throw CoreException("Unexpected error from getConservationMatrix()", e.Message());
    }
}

// Help("Returns the number of dependent species in the model")
int RoadRunner::getNumberOfDependentSpecies()
{
    try
    {
        if (mModel)
        {
            //return mStructAnalysis.GetInstance()->getNumDepSpecies();
            return mLS.getNumDepSpecies();
        }

        throw CoreException(gEmptyModelMessage);
    }

    catch(Exception &e)
    {
        throw CoreException("Unexpected error from getNumberOfDependentSpecies()", e.Message());
    }
}

// Help("Returns the number of independent species in the model")
int RoadRunner::getNumberOfIndependentSpecies()
{
    try
    {
        if (mModel)
        {
            return mLS.getNumIndSpecies();
        }
        //return StructAnalysis.getNumIndSpecies();
        throw CoreException(gEmptyModelMessage);
    }
    catch (Exception &e)
    {
        throw CoreException("Unexpected error from getNumberOfIndependentSpecies()", e.Message());
    }
}

double RoadRunner::getVariableValue(const TVariableType& variableType, const int& variableIndex)
{
    switch (variableType)
    {
        case vtFlux:
            return mModel->getModelData().rates[variableIndex];

        case vtSpecies:
            return mModel->getModelData().y[variableIndex];

        default:
            throw CoreException("Unrecognised variable in getVariableValue");
    }
}

//  Help("Returns the Symbols of all Flux Control Coefficients.")
NewArrayList RoadRunner::getFluxControlCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList oReactions       = getReactionIds();
    StringList oParameters      = mModel->getGlobalParameterNames();
    StringList oBoundary        = mModel->getBoundarySpeciesNames();
    StringList oConservation    = mModel->getConservationNames();

    for(int i = 0; i < oReactions.Count(); i++)
    {
        string s = oReactions[i];

        NewArrayList oCCReaction;
        StringList oInner;
        oCCReaction.Add(s);

        for(int i = 0; i < oParameters.Count(); i++)
        {
            oInner.add("CC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.Count(); i++)
        {
            oInner.add("CC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.Count(); i++)
        {
            oInner.add("CC:" + s + "," + oConservation[i]);
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

//  Help("Returns the Symbols of all Unscaled Flux Control Coefficients.")
NewArrayList RoadRunner::getUnscaledFluxControlCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList oReactions = getReactionIds();
    StringList oParameters = mModel->getGlobalParameterNames();
    StringList oBoundary = mModel->getBoundarySpeciesNames();
    StringList oConservation = mModel->getConservationNames();

    for(int i = 0; i < oReactions.Count(); i++)
    {
        string s = oReactions[i];

        NewArrayList oCCReaction;
        StringList oInner;
        oCCReaction.Add(s);

        for(int i = 0; i < oParameters.Count(); i++)
        {
            oInner.add("uCC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.Count(); i++)
        {
            oInner.add("uCC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.Count(); i++)
        {
            oInner.add("uCC:" + s + "," + oConservation[i]);
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

// Help("Returns the Symbols of all Concentration Control Coefficients.")
NewArrayList RoadRunner::getConcentrationControlCoefficientIds()
{
    NewArrayList oResult;// = new ArrayList();
    if (!mModel)
    {
        return oResult;
    }

    StringList oFloating        = getFloatingSpeciesIds();
    StringList oParameters      = mModel->getGlobalParameterNames();
    StringList oBoundary        = mModel->getBoundarySpeciesNames();
    StringList oConservation    = mModel->getConservationNames();

    for(int i = 0; i < oFloating.Count(); i++)
    {
        string s = oFloating[i];
        NewArrayList oCCFloating;
        StringList oInner;
        oCCFloating.Add(s);

        for(int i = 0; i < oParameters.Count(); i++)
        {
            oInner.add("CC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.Count(); i++)
        {
            oInner.add("CC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.Count(); i++)
        {
            oInner.add("CC:" + s + "," + oConservation[i]);
        }

        oCCFloating.Add(oInner);
        oResult.Add(oCCFloating);
    }

    return oResult;
}

// Help("Returns the Symbols of all Unscaled Concentration Control Coefficients.")
NewArrayList RoadRunner::getUnscaledConcentrationControlCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList oFloating        = getFloatingSpeciesIds();
    StringList oParameters      = mModel->getGlobalParameterNames();
    StringList oBoundary        = mModel->getBoundarySpeciesNames();
    StringList oConservation    = mModel->getConservationNames();

    for(int i = 0; i < oFloating.Count(); i++)
    {
        string s = oFloating[i];
        NewArrayList oCCFloating;
        StringList oInner;
        oCCFloating.Add(s);

        for(int i = 0; i < oParameters.Count(); i++)
        {
            oInner.add("uCC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.Count(); i++)
        {
            oInner.add("uCC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.Count(); i++)
        {
            oInner.add("uCC:" + s + "," + oConservation[i]);
        }

        oCCFloating.Add(oInner);
        oResult.Add(oCCFloating);
    }

    return oResult;
}

// Help("Returns the Symbols of all Elasticity Coefficients.")
NewArrayList RoadRunner::getElasticityCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList reactionNames        = getReactionIds();
    StringList floatingSpeciesNames = mModel->getFloatingSpeciesConcentrationNames();
    StringList boundarySpeciesNames = mModel->getBoundarySpeciesNames();
    StringList conservationNames    = mModel->getConservationNames();
    StringList globalParameterNames = mModel->getGlobalParameterNames();

    for(int i = 0; i < reactionNames.Count(); i++)
    {
        string reac_name = reactionNames[i];
        NewArrayList oCCReaction;
        oCCReaction.Add(reac_name);
        StringList oInner;

        for(int j = 0; j < floatingSpeciesNames.Count(); j++)
        {
            oInner.add(format("EE:{0},{1}", reac_name, floatingSpeciesNames[j]));
        }

        for(int j = 0; j < boundarySpeciesNames.Count(); j++)
        {
            oInner.add(format("EE:{0},{1}", reac_name, boundarySpeciesNames[j]));
        }

        for(int j = 0; j < globalParameterNames.Count(); j++)
        {
            oInner.add(format("EE:{0},{1}", reac_name, globalParameterNames[j]));
        }

        for(int j = 0; j < conservationNames.Count(); j++)
        {
            oInner.add(format("EE:{0},{1}", reac_name, conservationNames[j]));
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

// Help("Returns the Symbols of all Unscaled Elasticity Coefficients.")
NewArrayList RoadRunner::getUnscaledElasticityCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList oReactions( getReactionIds() );
    StringList oFloating = mModel->getFloatingSpeciesConcentrationNames();
    StringList oBoundary = mModel->getBoundarySpeciesNames();
    StringList oGlobalParameters = mModel->getGlobalParameterNames();
    StringList oConservation = mModel->getConservationNames();

    for(int i = 0; i < oReactions.Count(); i++)
    {
        string reac_name = oReactions[i];
        NewArrayList oCCReaction;
        StringList oInner;
        oCCReaction.Add(reac_name);

        for(int j = 0; j < oFloating.Count(); j++)
        {
            string variable = oFloating[j];
            oInner.add(format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oBoundary.Count(); j++)
        {
            string variable = oBoundary[j];
            oInner.add(format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oGlobalParameters.Count(); j++)
        {
            string variable = oGlobalParameters[j];
            oInner.add(format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oConservation.Count(); j++)
        {
            string variable = oConservation[j];
            oInner.add(format("uEE:{0},{1}", reac_name, variable));
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

// Help("Returns the Symbols of all Floating Species Eigenvalues.")
StringList RoadRunner::getEigenvalueIds()
{
    if (!mModel)
    {
        return StringList();
    }

    StringList result;
    StringList floating = mModel->getFloatingSpeciesConcentrationNames();

    for(int i = 0; i < floating.Count(); i++)
    {
        result.add("eigen_" + floating[i]);
    }

    return result;
}

// Help(
//            "Returns symbols of the currently loaded model, that can be used for steady state analysis. format: array of arrays  { { \"groupname\", { \"item1\", \"item2\" ... } } }  or { { \"groupname\", { \"subgroup\", { \"item1\" ... } } } }."
//            )
NewArrayList RoadRunner::getAvailableSteadyStateSymbols()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    oResult.Add("Floating Species",                                 getFloatingSpeciesIds() );
    oResult.Add("Boundary Species",                                 getBoundarySpeciesIds() );
    oResult.Add("Floating Species (amount)",                         getFloatingSpeciesAmountIds() );
    oResult.Add("Boundary Species (amount)",                         getBoundarySpeciesAmountIds() );
    oResult.Add("Global Parameters",                                 getParameterIds() );
    oResult.Add("Volumes",                                             mModel->getCompartmentNames() );
    oResult.Add("Fluxes",                                             getReactionIds() );
    oResult.Add("Flux Control Coefficients",                         getFluxControlCoefficientIds() );
    oResult.Add("Concentration Control Coefficients",                 getConcentrationControlCoefficientIds() );
    oResult.Add("Unscaled Concentration Control Coefficients",        getUnscaledConcentrationControlCoefficientIds());
    oResult.Add("Elasticity Coefficients",                             getElasticityCoefficientIds() );
    oResult.Add("Unscaled Elasticity Coefficients",                 getUnscaledElasticityCoefficientIds() );
    oResult.Add("Eigenvalues",                                         getEigenvalueIds() );

    return oResult;
}

int RoadRunner::createDefaultSteadyStateSelectionList()
{
    mSteadyStateSelection.clear();
    // default should be species only ...
    StringList floatingSpecies = getFloatingSpeciesIds();
    mSteadyStateSelection.resize(floatingSpecies.Count());
    for (int i = 0; i < floatingSpecies.Count(); i++)
    {
        SelectionRecord aRec;
        aRec.selectionType = SelectionType::clFloatingSpecies;
        aRec.p1 = floatingSpecies[i];
        aRec.index = i;
        mSteadyStateSelection[i] = aRec;
    }
    return mSteadyStateSelection.size();
}

// Help("Returns the selection list as returned by computeSteadyStateValues().")
StringList RoadRunner::getSteadyStateSelectionList()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (mSteadyStateSelection.size() == 0)
    {
          createDefaultSteadyStateSelectionList();
    }

    StringList oFloating     = mModel->getFloatingSpeciesConcentrationNames();
    StringList oBoundary     = mModel->getBoundarySpeciesNames();
    StringList oFluxes       = mModel->getReactionNames();
    StringList oVolumes      = mModel->getCompartmentNames();
    StringList oRates        = getRateOfChangeIds();
    StringList oParameters   = getParameterIds();

    StringList result;
    for(int i = 0; i < mSteadyStateSelection.size(); i++)
    {
        SelectionRecord record = mSteadyStateSelection[i];
        switch (record.selectionType)
        {
            case SelectionType::clTime:
                result.add("time");
            break;
            case SelectionType::clBoundaryAmount:
                result.add(format("[{0}]", oBoundary[record.index]));
            break;
            case SelectionType::clBoundarySpecies:
                result.add(oBoundary[record.index]);
            break;
            case SelectionType::clFloatingAmount:
                result.add("[" + (string)oFloating[record.index] + "]");
            break;
            case SelectionType::clFloatingSpecies:
                result.add(oFloating[record.index]);
            break;
            case SelectionType::clVolume:
                result.add(oVolumes[record.index]);
            break;
            case SelectionType::clFlux:
                result.add(oFluxes[record.index]);
            break;
            case SelectionType::clRateOfChange:
                result.add(oRates[record.index]);
            break;
            case SelectionType::clParameter:
                result.add(oParameters[record.index]);
            break;
            case SelectionType::clEigenValue:
                result.add("eigen_" + record.p1);
            break;
            case SelectionType::clElasticity:
                result.add("EE:" + record.p1 + "," + record.p2);
            break;
            case SelectionType::clUnscaledElasticity:
                result.add("uEE:" + record.p1 + "," + record.p2);
            break;
            case SelectionType::clUnknown:
                result.add(record.p1);
                break;
        }
    }
    return result ;
}

vector<SelectionRecord> RoadRunner::getSteadyStateSelection(const StringList& newSelectionList)
{
    vector<SelectionRecord> steadyStateSelection;
    steadyStateSelection.resize(newSelectionList.Count());
    StringList fs = mModel->getFloatingSpeciesConcentrationNames();
    StringList bs = mModel->getBoundarySpeciesNames();
    StringList rs = mModel->getReactionNames();
    StringList vol = mModel->getCompartmentNames();
    StringList gp = mModel->getGlobalParameterNames();

    for (int i = 0; i < newSelectionList.Count(); i++)
    {
        bool set = false;
        // Check for species
        for (int j = 0; j < fs.Count(); j++)
        {
            if ((string)newSelectionList[i] == (string)fs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionType::clFloatingSpecies;
                set = true;
                break;
            }

            if ((string)newSelectionList[i] == "[" + (string)fs[j] + "]")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionType::clFloatingAmount;
                set = true;
                break;
            }

            // Check for species rate of change
            if ((string)newSelectionList[i] == (string)fs[j] + "'")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionType::clRateOfChange;
                set = true;
                break;
            }
        }

        if (set)
        {
            continue;
        }

        // Check fgr boundary species
        for (int j = 0; j < bs.Count(); j++)
        {
            if ((string)newSelectionList[i] == (string)bs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionType::clBoundarySpecies;
                set = true;
                break;
            }
            if ((string)newSelectionList[i] == "[" + (string)bs[j] + "]")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionType::clBoundaryAmount;
                set = true;
                break;
            }
        }

        if (set)
        {
            continue;
        }

        if ((string)newSelectionList[i] == "time")
        {
            steadyStateSelection[i].selectionType = SelectionType::clTime;
            set = true;
        }

        for (int j = 0; j < rs.Count(); j++)
        {
            // Check for reaction rate
            if ((string)newSelectionList[i] == (string)rs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionType::clFlux;
                set = true;
                break;
            }
        }

        for (int j = 0; j < vol.Count(); j++)
        {
            // Check for volume
            if ((string)newSelectionList[i] == (string)vol[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionType::clVolume;
                set = true;
                break;
            }
        }

        for (int j = 0; j < gp.Count(); j++)
        {
            // Check for volume
            if ((string)newSelectionList[i] == (string)gp[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionType::clParameter;
                set = true;
                break;
            }
        }

        if (set)
        {
            continue;
        }

        // it is another symbol
        steadyStateSelection[i].selectionType = SelectionType::clUnknown;
        steadyStateSelection[i].p1 = (string)newSelectionList[i];
    }
    return steadyStateSelection;
}

// Help("sets the selection list as returned by computeSteadyStateValues().")
void RoadRunner::setSteadyStateSelectionList(const StringList& newSelectionList)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    vector<SelectionRecord> ssSelection = getSteadyStateSelection(newSelectionList);
    mSteadyStateSelection = ssSelection;
}

// Help("performs steady state analysis, returning values as given by setSteadyStateSelectionList().")
vector<double> RoadRunner::computeSteadyStateValues()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    if(mSteadyStateSelection.size() == 0)
    {
        createDefaultSteadyStateSelectionList();
    }
    return computeSteadyStateValues(mSteadyStateSelection, true);
}

vector<double> RoadRunner::computeSteadyStateValues(const vector<SelectionRecord>& selection, const bool& computeSteadyState)
{
    if (computeSteadyState)
    {
        steadyState();
    }

    vector<double> result; //= new double[oSelection.Length];
    for (int i = 0; i < selection.size(); i++)
    {
        result.push_back(computeSteadyStateValue(selection[i]));
    }
    return result;

}

double RoadRunner::computeSteadyStateValue(const SelectionRecord& record)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (record.selectionType == SelectionType::clUnknown)
    {
        return computeSteadyStateValue(record.p1);
    }
    return getValueForRecord(record);
}

// Help("Returns the value of the given steady state identifier.")
double RoadRunner::computeSteadyStateValue(const string& sId)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    string tmp("CC:");
    if(sId.compare(0, tmp.size(), tmp) == 0)
    {
        string sList = sId.substr(tmp.size());
        string sVariable = sList.substr(0, sList.find_first_of(","));
        string sParameter = sList.substr(sVariable.size() + 1);
        return getCC(sVariable, sParameter);
    }

    tmp = "uCC:";
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string sList = sId.substr(tmp.size());
        string sVariable = sList.substr(0, sList.find_first_of(","));
        string sParameter = sList.substr(sVariable.size() + 1);
        return getuCC(sVariable, sParameter);
    }

    tmp = "EE:";
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string sList = sId.substr(tmp.size());
        string sReaction = sList.substr(0, sList.find_first_of(","));
        string sVariable = sList.substr(sReaction.size() + 1);
        return getEE(sReaction, sVariable);
    }

    tmp = "uEE:";
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string sList = sId.substr(tmp.size());
        string sReaction = sList.substr(0, sList.find_first_of(","));
        string sVariable = sList.substr(sReaction.size() + 1);
        return getuEE(sReaction, sVariable);
    }
    else
    {
        tmp = "eigen_";
        if (sId.compare(0, tmp.size(), tmp) == 0)
        {
            string sSpecies = sId.substr(tmp.size());
            int nIndex;
            if (mModel->getFloatingSpeciesConcentrations().find(sSpecies, nIndex))
            {
                //SBWComplex[] oComplex = SBW_CLAPACK.getEigenValues(getReducedJacobian());
                //LibLA LA;

                DoubleMatrix mat = getReducedJacobian();
                vector<Complex> oComplex = ls::getEigenValues(mat);

                if (oComplex.size() > nIndex)
                {
                    return oComplex[nIndex].Real;
                }
                return gDoubleNaN;
            }
			throw CoreException(format("Found unknown floating species '{0}' in computeSteadyStateValue()", sSpecies));
        }
        try
        {
            return getValue(sId);
        }
        catch (Exception& )
        {
            throw CoreException(format("Found unknown symbol '{0}' in computeSteadyStateValue()", sId));
        }
    }
}

string RoadRunner::getModelName()
{
    return mNOM.getModelName();
}

// TODO Looks like major problems here, this
// apears to modify the AFTER a model has been created from it.
string RoadRunner::writeSBML()
{
    NOMSupport& NOM = this->mNOM;

    NOM.loadSBML(NOM.getParamPromotedSBML(mCurrentSBML));

    ModelState state(*mModel);
//    var state = new ModelState(model);

    StringList array = getFloatingSpeciesIds();
    for (int i = 0; i < array.Count(); i++)
    {
        NOM.setValue((string)array[i], state.mFloatingSpeciesConcentrations[i]);
    }

    array = getBoundarySpeciesIds();
    for (int i = 0; i < array.Count(); i++)
    {
        NOM.setValue((string)array[i], state.mBoundarySpeciesConcentrations[i]);
    }

    array = getCompartmentIds();
    for (int i = 0; i < array.Count(); i++)
    {
        NOM.setValue((string)array[i], state.mCompartmentVolumes[i]);
    }

    array = getGlobalParameterIds();
    for (int i = 0; i < min((int) array.Count(), (int) state.mGlobalParameters.size()); i++)
    {
        NOM.setValue((string)array[i], state.mGlobalParameters[i]);
    }

    return NOM.getSBML();
}

// Get the number of local parameters for a given reaction
int RoadRunner::getNumberOfLocalParameters(const int& reactionId)
{
     if (!mModel)
     {
         throw CoreException(gEmptyModelMessage);
     }
     return mModel->getNumLocalParameters(reactionId);
}

// Returns the value of a global parameter by its index
// ***** SHOULD WE SUPPORT LOCAL PARAMETERS? ******** (Sept 2, 2012, HMS
double RoadRunner::getLocalParameterByIndex    (const int& reactionId, const int& index)
{
    if(!mModel)
    {
       throw CoreException(gEmptyModelMessage);
    }

    if(    reactionId >= 0 &&
        reactionId < mModel->getNumReactions() &&
        index >= 0 &&
        index < mModel->getNumLocalParameters(reactionId))
    {
        return -1;//mModel->getModelData().lp[reactionId][index];
    }
    else
    {
     	throw CoreException(format("Index in getLocalParameterByIndex out of range: [{0}]", index));
    }
}

// Help("Get the number of reactions")
int RoadRunner::getNumberOfReactions()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getNumReactions();
}

// Help("Returns the rate of a reaction by its index")
double RoadRunner::getReactionRate(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumReactions()))
    {
        mModel->convertToConcentrations();
        mModel->computeReactionRates(0.0, mModel->getModelData().y);
        return mModel->getModelData().rates[index];
    }
    else
    {
        throw CoreException(format("Index in getReactionRate out of range: [{0}]", index));
    }
}

// Help("Returns the rate of changes of a species by its index")
double RoadRunner::getRateOfChange(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumTotalVariables()))
    {
        mModel->computeAllRatesOfChange();
        return mModel->getModelData().dydt[index];
    }

    throw CoreException(format("Index in getRateOfChange out of range: [{0}]", index));
}

// Help("Returns the rates of changes given an array of new floating species concentrations")
vector<double> RoadRunner::getRatesOfChangeEx(const vector<double>& values)
{
    setFloatingSpeciesConcentrations(values);
    return getRatesOfChange();
}

// Help("Returns the rates of changes given an array of new floating species concentrations")
vector<double> RoadRunner::getReactionRatesEx(const vector<double>& values)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->computeReactionRates(0.0, createVector(values));
    return createVector(mModel->getModelData().rates, mModel->getModelData().ratesSize);
}

// Help("Get the number of compartments")
int RoadRunner::getNumberOfCompartments()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getNumCompartments();
}

// Help("Sets the value of a compartment by its index")
void RoadRunner::setCompartmentByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
         throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumCompartments()))
    {
        mModel->getModelData().c[index] = value;
    }
    else
    {
        throw CoreException(format("Index in getCompartmentByIndex out of range: [{0}]", index));
    }
}

// Help("Returns the value of a compartment by its index")
double RoadRunner::getCompartmentByIndex(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumCompartments()))
    {
        return mModel->getModelData().c[index];
    }

    throw CoreException(format("Index in getCompartmentByIndex out of range: [{0}]", index));
}

// Help("Get the number of boundary species")
int RoadRunner::getNumberOfBoundarySpecies()
{
    if (!mModel)
    {
        throw Exception(gEmptyModelMessage);
    }
    return mModel->getNumBoundarySpecies();
}

// Help("Sets the value of a boundary species by its index")
void RoadRunner::setBoundarySpeciesByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw Exception(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumBoundarySpecies()))
    {
        mModel->getModelData().bc[index] = value;
    }
    else
    {
        throw Exception(format("Index in getBoundarySpeciesByIndex out of range: [{0}]", index));
    }
}

// Help("Returns the value of a boundary species by its index")
double RoadRunner::getBoundarySpeciesByIndex(const int& index)
{
    if (!mModel)
    {
        throw Exception(gEmptyModelMessage);
    }
    if ((index >= 0) && (index < mModel->getNumBoundarySpecies()))
    {
        return mModel->getModelData().bc[index];
    }
    throw Exception(format("Index in getBoundarySpeciesByIndex out of range: [{0}]", index));
}

// Help("Returns an array of boundary species concentrations")
vector<double> RoadRunner::getBoundarySpeciesConcentrations()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->convertToConcentrations();
    return createVector(mModel->getModelData().bc, mModel->getModelData().bcSize);
}

// Help("Set the concentrations for all boundary species in the model")
//        void RoadRunner::setBoundarySpeciesConcentrations(double[] values)
//        {
//            if (!mModel) throw CoreException(gEmptyModelMessage);
//
//            mModel->getModelData().bc = values;
//        }

// Help("Gets the list of boundary species names")
StringList RoadRunner::getBoundarySpeciesIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getBoundarySpeciesNames();
}

// Help("Gets the list of boundary species amount names")
StringList RoadRunner::getBoundarySpeciesAmountIds()
{
    StringList result;// = new ArrayList();
    StringList list = getBoundarySpeciesIds();
//    foreach (string s in getBoundarySpeciesIds()) oResult.add("[" + s + "]");
    for(int item = 0; item < list.Count(); item++)// (object item in floatingSpeciesNames)
    {
        result.add(format("[{0}]", list[item]));
    }

    return result;
}

// Help("Get the number of floating species")
int RoadRunner::getNumberOfFloatingSpecies()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getNumTotalVariables();
}

// Help("Sets the value of a floating species by its index")
void RoadRunner::setFloatingSpeciesInitialConcentrationByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumTotalVariables()))
    {
        mModel->getModelData().init_y[index] = value;
        reset();
    }
    else
    {
        throw CoreException(format("Index in setFloatingSpeciesInitialConcentrationByIndex out of range: [{0}]", index));
    }
}

// Help("Sets the value of a floating species by its index")
void RoadRunner::setFloatingSpeciesByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumTotalVariables()))
    {
        mModel->setConcentration(index, value); // This updates the amount vector aswell
        if (!mConservedTotalChanged)
        {
            mModel->computeConservedTotals();
        }
    }
    else
    {
        throw CoreException(format("Index in setFloatingSpeciesByIndex out of range: [{0}]", index));
    }
}

// Help("Returns the value of a floating species by its index")
double RoadRunner::getFloatingSpeciesByIndex(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumTotalVariables()))
    {
        return mModel->getConcentration(index);
    }
    throw CoreException(format("Index in getFloatingSpeciesByIndex out of range: [{0}]", index));
}

// Help("Returns an array of floating species concentrations")
vector<double> RoadRunner::getFloatingSpeciesConcentrations()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->convertToConcentrations();
    return createVector(mModel->getModelData().y, mModel->getModelData().ySize);
}

// Help("returns an array of floating species initial conditions")
vector<double> RoadRunner::getFloatingSpeciesInitialConcentrations()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    vector<double> initYs;
    copyCArrayToStdVector(mModel->getModelData().init_y, initYs, mModel->getModelData().init_ySize);
    return initYs;
}

// Help("Sets the initial conditions for all floating species in the model")
void RoadRunner::setFloatingSpeciesInitialConcentrations(const vector<double>& values)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    for (int i = 0; i < values.size(); i++)
    {
        mModel->setConcentration(i, values[i]);
        if (mModel->getModelData().ySize > i)
        {
            mModel->getModelData().init_y[i] = values[i];
        }
    }

//    mModel->getModelData().init_y = values;
    reset();
}

// Help("Set the concentrations for all floating species in the model")
void RoadRunner::setFloatingSpeciesConcentrations(const vector<double>& values)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    for (int i = 0; i < values.size(); i++)
    {
        mModel->setConcentration(i, values[i]);
        if (mModel->getModelData().ySize > i)
        {
            mModel->getModelData().y[i] = values[i];
        }
    }
    mModel->convertToAmounts();
    if (!mConservedTotalChanged) mModel->computeConservedTotals();
}

// Help("Set the concentrations for all floating species in the model")
void RoadRunner::setBoundarySpeciesConcentrations(const vector<double>& values)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    for (int i = 0; i < values.size(); i++)
    {
        mModel->setConcentration(i, values[i]);
        if ((mModel->getModelData().bcSize) > i)
        {
            mModel->getModelData().bc[i] = values[i];
        }
    }
    mModel->convertToAmounts();
}

// This is a Level 1 method !
// Help("Returns a list of floating species names")
StringList RoadRunner::getFloatingSpeciesIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getFloatingSpeciesConcentrationNames(); // Reordered list
}

// Help("Returns a list of floating species initial condition names")
StringList RoadRunner::getFloatingSpeciesInitialConditionIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    StringList floatingSpeciesNames = mModel->getFloatingSpeciesConcentrationNames();
    StringList result;// = new ArrayList();
    for(int item = 0; item < floatingSpeciesNames.Count(); item++)// (object item in floatingSpeciesNames)
    {
        result.add(format("init({0})", floatingSpeciesNames[item]));
    }
    return result;
}

// Help("Returns the list of floating species amount names")
StringList RoadRunner::getFloatingSpeciesAmountIds()
{
    StringList oResult;
    StringList list = getFloatingSpeciesIds();

    for(int i = 0; i < list.Count(); i++)
    {
        oResult.add(format("[{0}]", list[i]));
    }
    return oResult;
}

// Help("Get the number of global parameters")
int RoadRunner::getNumberOfGlobalParameters()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getGlobalParameterNames().Count();
}

// Help("Sets the value of a global parameter by its index")
void RoadRunner::setGlobalParameterByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumGlobalParameters() + mModel->getModelData().ctSize))
    {
        if (index >= mModel->getNumGlobalParameters())
        {
            mModel->getModelData().ct[index - mModel->getNumGlobalParameters()] = value;
            mModel->updateDependentSpeciesValues(mModel->getModelData().y);
            mConservedTotalChanged = true;
        }
        else
        {
            mModel->getModelData().gp[index] = value;
        }
    }
    else
    {
        throw CoreException(format("Index in getNumGlobalParameters out of range: [{0}]", index));
    }
}

// Help("Returns the value of a global parameter by its index")
double RoadRunner::getGlobalParameterByIndex(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < (mModel->getNumGlobalParameters() + mModel->getModelData().ctSize)))
    {
        int arraySize = mModel->getModelData().gpSize + mModel->getModelData().ctSize;
        double* result = new double[arraySize];

        for(int i = 0; i < mModel->getModelData().gpSize; i++)
        {
            result[i] = mModel->getModelData().gp[i];
        }

        int tempIndex = 0;
        for(int i = mModel->getModelData().gpSize; i < arraySize; i++)
        {
            result[i] = mModel->getModelData().ct[tempIndex++];
        }

        return result[index];
    }

    throw CoreException(format("Index in getNumGlobalParameters out of range: [{0}]", index));
}

// Help("Set the values for all global parameters in the model")
//        void RoadRunner::setGlobalParameterValues(double[] values)
//        {
//            if (!mModel) throw CoreException(gEmptyModelMessage);
//            if (values.Length == mModel->getModelData().gp.Length)
//                mModel->getModelData().gp = values;
//            else
//            {
//                for (int i = 0; i < mModel->getModelData().gp.Length; i++)
//                {
//                    mModel->getModelData().gp[i] = values[i];
//                }
//                for (int i = 0; i < mModel->getModelData().ct.Length; i++)
//                {
//                    mModel->getModelData().gp[i] = values[i + mModel->getModelData().gp.Length];
//                    mConservedTotalChanged = true;
//                }
//                mModel->getModelData().updateDependentSpeciesValues(mModel->getModelData().y);
//            }
//        }

// Help("Get the values for all global parameters in the model")
vector<double> RoadRunner::getGlobalParameterValues()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (mModel->getModelData().ctSize > 0)
    {
        vector<double> result; //= new double[mModel->getModelData().gp.Length + mModel->getModelData().ct.Length];
        result.resize(mModel->getModelData().gpSize + mModel->getModelData().ctSize);

        //mModel->getModelData().gp.CopyTo(result, 0);
        copyValues(result,mModel->getModelData().gp, mModel->getModelData().gpSize, 0);

        //mModel->getModelData().ct.CopyTo(result, mModel->getModelData().gp.Length);
        copyValues(result, mModel->getModelData().ct, mModel->getModelData().ctSize, mModel->getModelData().gpSize);
        return result;
    }

    return createVector(mModel->getModelData().gp, mModel->getModelData().gpSize);
}

// Help("Gets the list of parameter names")
StringList RoadRunner::getGlobalParameterIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getGlobalParameterNames();
}

// Help("Returns a description of the module")
string RoadRunner::getDescription()
{
    return "Simulator API based on CVODE/NLEQ/C++ implementation";
}

//---------------- MCA functions......
//        [Help("Get unscaled control coefficient with respect to a global parameter")]
double RoadRunner::getuCC(const string& variableName, const string& parameterName)
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        TParameterType parameterType;
        TVariableType variableType;
        double originalParameterValue;
        int variableIndex;
        int parameterIndex;

        mModel->convertToConcentrations();
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);

        // Check the variable name
        if (mModel->getReactions().find(variableName, variableIndex))
        {
            variableType = TVariableType::vtFlux;
        }
        else if (mModel->getFloatingSpeciesConcentrations().find(variableName, variableIndex))
        {
            variableType = TVariableType::vtSpecies;
        }
        else
        {
            throw CoreException("Unable to locate variable: [" + variableName + "]");
        }

        // Check for the parameter name
        if (mModel->getGlobalParameters().find(parameterName, parameterIndex))
        {
            parameterType = TParameterType::ptGlobalParameter;
            originalParameterValue = mModel->getModelData().gp[parameterIndex];
        }
        else if (mModel->getBoundarySpecies().find(parameterName, parameterIndex))
        {
            parameterType = TParameterType::ptBoundaryParameter;
            originalParameterValue = mModel->getModelData().bc[parameterIndex];
        }
        else if (mModel->getConservations().find(parameterName, parameterIndex))
        {
            parameterType = TParameterType::ptConservationParameter;
            originalParameterValue = mModel->getModelData().ct[parameterIndex];
        }
        else
        {
            throw CoreException("Unable to locate parameter: [" + parameterName + "]");
        }

        // Get the original parameter value
        originalParameterValue = getParameterValue(parameterType, parameterIndex);

        double hstep = mDiffStepSize*originalParameterValue;
        if (fabs(hstep) < 1E-12)
        {
            hstep = mDiffStepSize;
        }

        try
        {
            mModel->convertToConcentrations();

            setParameterValue(parameterType, parameterIndex, originalParameterValue + hstep);
            steadyState();
            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
            double fi = getVariableValue(variableType, variableIndex);

            setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
            steadyState();
            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
            double fi2 = getVariableValue(variableType, variableIndex);

            setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
            steadyState();
            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
            double fd = getVariableValue(variableType, variableIndex);

            setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
            steadyState();
            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
            double fd2 = getVariableValue(variableType, variableIndex);

            // Use instead the 5th order approximation double unscaledValue = (0.5/hstep)*(fi-fd);
            // The following separated lines avoid small amounts of roundoff error
            double f1 = fd2 + 8*fi;
            double f2 = -(8*fd + fi2);

            // What ever happens, make sure we restore the parameter level
            setParameterValue(parameterType, parameterIndex, originalParameterValue);
            steadyState();

            return 1/(12*hstep)*(f1 + f2);
        }
        catch(...) //Catch anything... and do 'finalize'
        {
            // What ever happens, make sure we restore the parameter level
            setParameterValue(parameterType, parameterIndex, originalParameterValue);
            steadyState();
            throw;
        }
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getuCC ()", e.Message());
    }
}

//        [Help("Get scaled control coefficient with respect to a global parameter")]
double RoadRunner::getCC(const string& variableName, const string& parameterName)
{
    TVariableType variableType;
    TParameterType parameterType;
    int variableIndex;
    int parameterIndex;

    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    // Check the variable name
    if (mModel->getReactions().find(variableName, variableIndex))
    {
        variableType = TVariableType::vtFlux;
    }
    else if (mModel->getFloatingSpeciesConcentrations().find(variableName, variableIndex))
    {
        variableType = TVariableType::vtSpecies;
    }
    else
    {
        throw CoreException("Unable to locate variable: [" + variableName + "]");
    }

    // Check for the parameter name
    if (mModel->getGlobalParameters().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptGlobalParameter;
    }
    else if (mModel->getBoundarySpecies().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptBoundaryParameter;
    }
    else if (mModel->getConservations().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptConservationParameter;
    }
    else
    {
        throw CoreException("Unable to locate parameter: [" + parameterName + "]");
    }

    steadyState();

    double variableValue = getVariableValue(variableType, variableIndex);
    double parameterValue = getParameterValue(parameterType, parameterIndex);
    return getuCC(variableName, parameterName)*parameterValue/variableValue;
}

//[Ignore]
// Get a single species elasticity value
// IMPORTANT:
// Assumes that the reaction rates have been precomputed at the operating point !!
double RoadRunner::getUnscaledSpeciesElasticity(int reactionId, int speciesIndex)
{
    double originalParameterValue = mModel->getConcentration(speciesIndex);

    double hstep = mDiffStepSize*originalParameterValue;
    if (fabs(hstep) < 1E-12)
    {
        hstep = mDiffStepSize;
    }

    mModel->convertToConcentrations();
    mModel->setConcentration(speciesIndex, originalParameterValue + hstep);
    try
    {
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        double fi = mModel->getModelData().rates[reactionId];

        mModel->setConcentration(speciesIndex, originalParameterValue + 2*hstep);
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        double fi2 = mModel->getModelData().rates[reactionId];

        mModel->setConcentration(speciesIndex, originalParameterValue - hstep);
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        double fd = mModel->getModelData().rates[reactionId];

        mModel->setConcentration(speciesIndex, originalParameterValue - 2*hstep);
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        double fd2 = mModel->getModelData().rates[reactionId];

        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
        // The following separated lines avoid small amounts of roundoff error
        double f1 = fd2 + 8*fi;
        double f2 = -(8*fd + fi2);

        // What ever happens, make sure we restore the species level
        mModel->setConcentration(speciesIndex, originalParameterValue);
        return 1/(12*hstep)*(f1 + f2);
    }
    catch(const Exception& e)
    {
        Log(lError)<<"Something went wrong in "<<__FUNCTION__;
        Log(lError)<<"Exception "<<e.what()<< " thrown";
                // What ever happens, make sure we restore the species level
        mModel->setConcentration(speciesIndex, originalParameterValue);
        return gDoubleNaN;
    }
}


//        [Help("Compute the unscaled species elasticity matrix at the current operating point")]
DoubleMatrix RoadRunner::getUnscaledElasticityMatrix()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        DoubleMatrix uElastMatrix(mModel->getNumReactions(), mModel->getNumTotalVariables());
        mModel->convertToConcentrations();

        // Compute reaction velocities at the current operating point
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);

        for (int i = 0; i < mModel->getNumReactions(); i++)
        {
            for (int j = 0; j < mModel->getNumTotalVariables(); j++)
            {
                uElastMatrix[i][j] = getUnscaledSpeciesElasticity(i, j);
            }
        }

        return uElastMatrix;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from unscaledElasticityMatrix()", e.Message());
    }
}

//        [Help("Compute the unscaled elasticity matrix at the current operating point")]
DoubleMatrix RoadRunner::getScaledReorderedElasticityMatrix()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        DoubleMatrix uelast = getUnscaledElasticityMatrix();

        DoubleMatrix result(uelast.RSize(), uelast.CSize());// = new double[uelast.Length][];
        mModel->convertToConcentrations();
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
        vector<double> rates;
        if(!copyCArrayToStdVector(mModel->getModelData().rates, rates, mModel->getModelData().ratesSize))
        {
            throw CoreException("Failed to copy model->rates");
        }

        for (int i = 0; i < uelast.RSize(); i++)
        {
            // Rows are rates
            if (mModel->getModelData().ratesSize == 0 || rates[i] == 0)
            {
                string name;
                if(mModelGenerator && mModel->getReactions().size())
                {
                    name = mModel->getReactions()[i].name;
                }
                else
                {
                    name = "none";
                }

                throw CoreException("Unable to compute elasticity, reaction rate [" + name + "] set to zero");
            }

            for (int j = 0; j < uelast.CSize(); j++) // Columns are species
            {
                result[i][j] = uelast[i][j]*mModel->getConcentration(j)/rates[i];
            }
        }
        return result;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from scaledElasticityMatrix()", e.Message());
    }
}

//        [Help("Compute the scaled elasticity for a given reaction and given species")]
double RoadRunner::getScaledFloatingSpeciesElasticity(const string& reactionName, const string& speciesName)
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }
        int speciesIndex = 0;
        int reactionIndex = 0;

        mModel->convertToConcentrations();
        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);

        if (!mModel->getFloatingSpeciesConcentrations().find(speciesName, speciesIndex))
        {
            throw CoreException("Internal Error: unable to locate species name while computing unscaled elasticity");
        }
        if (!mModel->getReactions().find(reactionName, reactionIndex))
        {
            throw CoreException("Internal Error: unable to locate reaction name while computing unscaled elasticity");
        }

        return getUnscaledSpeciesElasticity(reactionIndex, speciesIndex)*
               mModel->getConcentration(speciesIndex)/mModel->getModelData().rates[reactionIndex];

    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from scaledElasticityMatrix()", e.Message());
    }
}

//        [Ignore]
//        // Changes a given parameter type by the given increment
//        void changeParameter(TParameterType parameterType, int reactionIndex, int parameterIndex,
//                                     double originalValue, double increment)
//        {
//            switch (parameterType)
//            {
//                case TParameterType::ptLocalParameter:
//                    mModel->getModelData().lp[reactionIndex][parameterIndex] = originalValue + increment;
//                    break;
//                case TParameterType::ptGlobalParameter:
//                    mModel->getModelData().gp[parameterIndex] = originalValue + increment;
//                    break;
//                case TParameterType::ptBoundaryParameter:
//                    mModel->getModelData().bc[parameterIndex] = originalValue + increment;
//                    break;
//                case TParameterType::ptConservationParameter:
//                    mModel->getModelData().ct[parameterIndex] = originalValue + increment;
//                    break;
//            }
//        }
//
//
//        [Help("Returns the unscaled elasticity for a named reaction with respect to a named parameter (local or global)"
//            )]
//        double getUnscaledParameterElasticity(string reactionName, string parameterName)
//        {
//            int reactionIndex;
//            int parameterIndex;
//            double originalParameterValue;
//            TParameterType parameterType;
//
//            if (!mModel) throw CoreException(gEmptyModelMessage);
//            mModel->convertToConcentrations();
//            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//            if (!mModel->getReactions().find(reactionName, out reactionIndex))
//                throw CoreException(
//                    "Internal Error: unable to locate reaction name while computing unscaled elasticity");
//
//            // Look for the parameter name, check local parameters first, then global
//            if (mModelGenerator->localParameterList[reactionIndex].find(reactionName, parameterName,
//                                                                               out parameterIndex))
//                parameterType = TParameterType::ptLocalParameter;
//            else if (mModel->getGlobalParameters().find(parameterName, out parameterIndex))
//                parameterType = TParameterType::ptGlobalParameter;
//            else if (mModel->getBoundarySpecies().find(parameterName, out parameterIndex))
//                parameterType = TParameterType::ptBoundaryParameter;
//            else if (mModel->getConservations().find(parameterName, out parameterIndex))
//                parameterType = TParameterType::ptConservationParameter;
//            else
//                return 0.0;
//
//            double f1, f2, fi, fi2, fd, fd2;
//            originalParameterValue = 0.0;
//            switch (parameterType)
//            {
//                case TParameterType::ptLocalParameter:
//                    originalParameterValue = mModel->getModelData().lp[reactionIndex][parameterIndex];
//                    break;
//                case TParameterType::ptGlobalParameter:
//                    originalParameterValue = mModel->getModelData().gp[parameterIndex];
//                    break;
//                case TParameterType::ptBoundaryParameter:
//                    originalParameterValue = mModel->getModelData().bc[parameterIndex];
//                    break;
//                case TParameterType::ptConservationParameter:
//                    originalParameterValue = mModel->getModelData().ct[parameterIndex];
//                    break;
//            }
//
//            double hstep = mDiffStepSize*originalParameterValue;
//            if (Math.Abs(hstep) < 1E-12)
//                hstep = mDiffStepSize;
//
//            try
//            {
//                changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, hstep);
//                mModel->convertToConcentrations();
//                mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                fi = mModel->getModelData().rates[reactionIndex];
//
//                changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, 2*hstep);
//                mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                fi2 = mModel->getModelData().rates[reactionIndex];
//
//                changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, -hstep);
//                mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                fd = mModel->getModelData().rates[reactionIndex];
//
//                changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, -2*hstep);
//                mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                fd2 = mModel->getModelData().rates[reactionIndex];
//
//                // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                // The following separated lines avoid small amounts of roundoff error
//                f1 = fd2 + 8*fi;
//                f2 = -(8*fd + fi2);
//            }
//            finally
//            {
//                // What ever happens, make sure we restore the species level
//                switch (parameterType)
//                {
//                    case TParameterType::ptLocalParameter:
//                        mModel->getModelData().lp[reactionIndex][parameterIndex] = originalParameterValue;
//                        break;
//                    case TParameterType::ptGlobalParameter:
//                        mModel->getModelData().gp[parameterIndex] = originalParameterValue;
//                        break;
//                    case TParameterType::ptBoundaryParameter:
//                        mModel->getModelData().bc[parameterIndex] = originalParameterValue;
//                        break;
//                    case TParameterType::ptConservationParameter:
//                        mModel->getModelData().ct[parameterIndex] = originalParameterValue;
//                        break;
//                }
//            }
//            return 1/(12*hstep)*(f1 + f2);
//        }
//
//
// Use the formula: ucc = -L Jac^-1 Nr
// [Help("Compute the matrix of unscaled concentration control coefficients")]
DoubleMatrix RoadRunner::getUnscaledConcentrationControlCoefficientMatrix()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        setTimeStart(0.0);
        setTimeEnd(50.0);
        setNumPoints(2);
        simulate(); //This will crash, because numpoints == 1, not anymore, numPoints = 2 if numPoints <= 1
        if (steadyState() > mSteadyStateThreshold)
        {
            if (steadyState() > 1E-2)
            {
                throw CoreException("Unable to locate steady state during control coefficient computation");
            }
        }

        // Compute the Jacobian first
        DoubleMatrix uelast     = getUnscaledElasticityMatrix();
        DoubleMatrix *Nr         = getNrMatrix();
        DoubleMatrix T1 = mult(*Nr, uelast);
        DoubleMatrix *LinkMatrix = getLinkMatrix();
        DoubleMatrix Jac = mult(T1, *LinkMatrix);

        // Compute -Jac
        DoubleMatrix T2 = Jac * (-1.0);

        ComplexMatrix temp(T2); //Get a complex matrix from a double one. Imag part is zero
        ComplexMatrix Inv = GetInverse(temp);

        // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
        // Sauro: mult which takes complex matrix need to be implemented
        DoubleMatrix T3 = mult(Inv, *Nr); // Compute ( - Jac)^-1 . Nr

        // Finally include the dependent set as well.
        DoubleMatrix T4 = mult(*LinkMatrix, T3); // Compute L (iwI - Jac)^-1 . Nr
        return T4;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getUnscaledConcentrationControlCoefficientMatrix()", e.Message());
    }
}

// [Help("Compute the matrix of scaled concentration control coefficients")]
DoubleMatrix RoadRunner::getScaledConcentrationControlCoefficientMatrix()
{
    try
    {
        if (mModel)
        {
            DoubleMatrix ucc = getUnscaledConcentrationControlCoefficientMatrix();

            if (ucc.size() > 0 )
            {
                mModel->convertToConcentrations();
                mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
                for (int i = 0; i < ucc.RSize(); i++)
                {
                    for (int j = 0; j < ucc.CSize(); j++)
                    {
                        if(mModel->getConcentration(i) != 0.0)
                        {
                            ucc[i][j] = ucc[i][j]*mModel->getModelData().rates[j]/mModel->getConcentration(i);
                        }
                        else
                        {
                            throw(Exception("Dividing with zero"));
                        }
                    }
                }
            }
            return ucc;
        }
        else
        {
            throw CoreException(gEmptyModelMessage);
        }
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getScaledConcentrationControlCoefficientMatrix()", e.Message());
    }
}

// Use the formula: ucc = elast CS + I
// [Help("Compute the matrix of unscaled flux control coefficients")]
DoubleMatrix RoadRunner::getUnscaledFluxControlCoefficientMatrix()
{
    try
    {
        if (mModel)
        {
            DoubleMatrix ucc = getUnscaledConcentrationControlCoefficientMatrix();
            DoubleMatrix uee = getUnscaledElasticityMatrix();

            DoubleMatrix T1 = mult(uee, ucc);

			// Add an identity matrix I to T1, that is add a 1 to every diagonal of T1
            for (int i=0; i<T1.RSize(); i++)
                T1[i][i] = T1[i][i] + 1;
            return T1;//Matrix.convertToDouble(T1);
        }
        else throw CoreException(gEmptyModelMessage);
    }
    catch (CoreException&)
    {
        throw;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getUnscaledFluxControlCoefficientMatrix()", e.Message());
    }
}

// [Help("Compute the matrix of scaled flux control coefficients")]
DoubleMatrix RoadRunner::getScaledFluxControlCoefficientMatrix()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        DoubleMatrix ufcc = getUnscaledFluxControlCoefficientMatrix();

        if (ufcc.RSize() > 0)
        {
            mModel->convertToConcentrations();
            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
            for (int i = 0; i < ufcc.RSize(); i++)
            {
                for (int j = 0; j < ufcc.CSize(); j++)
                {
                    if(mModel->getModelData().rates[i] !=0)
                    {
                        ufcc[i][j] = ufcc[i][j] * mModel->getModelData().rates[j]/mModel->getModelData().rates[i];
                    }
                    else
                    {
                        throw(Exception("Dividing with zero"));
                       }
                }
            }
        }
        return ufcc;
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()", e.Message());
    }
}

// Help("Returns the initially loaded model as SBML")
string RoadRunner::getSBML()
{
    return mCurrentSBML;
}

// Help("Set the time start for the simulation")
void RoadRunner::setTimeStart(const double& startTime)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (startTime < 0)
    {
        throw CoreException("Time Start most be greater than zero");
    }

    mTimeStart = startTime;
}

//Help("Set the time end for the simulation")
void RoadRunner::setTimeEnd(const double& endTime)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (endTime <= 0)
    {
        throw CoreException("Time End most be greater than zero");
    }

    mTimeEnd = endTime;
}

//Help("Set the number of points to generate during the simulation")
void RoadRunner::setNumPoints(const int& pts)
{
    if(!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mNumPoints = (pts <= 0) ? 2 : pts;
}

// [Help("get the currently set time start")]
double RoadRunner::getTimeStart()
{
    return mTimeStart;
}

// [Help("get the currently set time end")]
double RoadRunner::getTimeEnd()
{
   return mTimeEnd;
}

// [Help("get the currently set number of points")]
int RoadRunner::getNumPoints()
{
   return mNumPoints;
}

// Help(
//            "Change the initial conditions to another concentration vector (changes only initial conditions for floating Species)")
void RoadRunner::changeInitialConditions(const vector<double>& ic)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    for (int i = 0; i < ic.size(); i++)
    {
        mModel->setConcentration(i, ic[i]);
        if ((mModel->getModelData().init_ySize) > i)
        {
            mModel->getModelData().init_y[i] = ic[i];
        }
    }
    mModel->convertToAmounts();
    mModel->computeConservedTotals();
}

// Help("Returns the current vector of reactions rates")
vector<double> RoadRunner::getReactionRates()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    mModel->convertToConcentrations();
    mModel->computeReactionRates(0.0, mModel->getModelData().y);

    vector<double> _rates;
    copyCArrayToStdVector(mModel->getModelData().rates, _rates, mModel->getModelData().ratesSize);
    return _rates;
}

// Help("Returns the current vector of rates of change")
vector<double> RoadRunner::getRatesOfChange()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->computeAllRatesOfChange();
    vector<double> result;
    copyCArrayToStdVector(mModel->getModelData().dydt, result, mModel->getModelData().dydtSize);

    return result;
}

// Help("Returns a list of reaction names")
StringList RoadRunner::getReactionIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    return mModel->getReactionNames();
}

// ---------------------------------------------------------------------
// Start of Level 2 API Methods
// ---------------------------------------------------------------------
// Help("Get Simulator Capabilities")
string RoadRunner::getCapabilitiesAsXML()
{
    return mCapabilities.asXML();
}

Capability* RoadRunner::getCapability(const string& cap_name)
{
	return mCapabilities.get(cap_name);
}

StringList RoadRunner::getListOfCapabilities()
{
    return mCapabilities.asStringList();
}

bool RoadRunner::addCapability(Capability& cap)
{
	mCapabilities.add(cap);
	return true;
}

bool RoadRunner::addCapabilities(Capabilities& caps)
{
	for(int i = 0; i < caps.count(); i++)
    {
    	addCapability(*(caps[i]));
    }
    return true;
}

StringList RoadRunner::getListOfParameters(const string& cap)
{
	Capability *aCap = mCapabilities.get(cap);
    if(!aCap)
    {
    	stringstream msg;
    	msg<<"No such capability: "<<cap;
		throw(CoreException(msg.str()));
    }
    Parameters* paras = aCap->getParameters();
    if(paras)
    {
    	return paras->asStringList();
    }
}

void RoadRunner::setTolerances(const double& aTol, const double& rTol)
{
    if(mCVode)
    {
        mCVode->setTolerances(aTol, rTol);
    }
}

void RoadRunner::correctMaxStep()
{
    if(mCVode)
    {
        double maxStep = (mTimeEnd - mTimeStart) / (mNumPoints);
        maxStep = min(mCVode->mMaxStep, maxStep);
        mCVode->mMaxStep = maxStep;
    }
}

// Help("Set Simulator Capabilites")
void RoadRunner::setCapabilities(const string& capsStr)
{
//    var cs = new CapsSupport(capsStr);
//    cs.Apply();
//
//    //correctMaxStep();
//
//    if (mModel)
//    {
//        if(!mCVode)
//        {
//            mCVode = new CvodeInterface(model);
//        }
//        for (int i = 0; i < model.getNumIndependentVariables; i++)
//        {
//            mCVode->setAbsTolerance(i, CvodeInterface->absTol);
//        }
//        mCVode->reStart(0.0, model);
//    }
//
//    if (cs.HasSection("integration") && cs["integration"].HasCapability("usekinsol"))
//    {
//        CapsSupport.Capability cap = cs["integration", "usekinsol"];
//        mUseKinsol = cap.IntValue;
//    }
}

// Help("Sets the value of the given species or global parameter to the given value (not of local parameters)")
bool RoadRunner::setValue(const string& sId, const double& dValue)
{
    if (!mModel)
    {
        Log(lError)<<gEmptyModelMessage;
        return false;
    }

    int nIndex = -1;
    if (mModel->getGlobalParameters().find(sId, nIndex))
    {
        mModel->getModelData().gp[nIndex] = dValue;
        return true;
    }

    if (mModel->getBoundarySpecies().find(sId, nIndex))
    {
        mModel->getModelData().bc[nIndex] = dValue;
        return true;
    }

    if (mModel->getCompartments().find(sId, nIndex))
    {
        mModel->getModelData().c[nIndex] = dValue;
        return true;
    }

    if (mModel->getFloatingSpeciesConcentrations().find(sId, nIndex))
    {
        mModel->setConcentration(nIndex, dValue);
        mModel->convertToAmounts();
        if (!mConservedTotalChanged)
        {
            mModel->computeConservedTotals();
        }
        return true;
    }

    if (mModel->getConservations().find(sId, nIndex))
    {
        mModel->getModelData().ct[nIndex] = dValue;
        mModel->updateDependentSpeciesValues(mModel->getModelData().y);
        mConservedTotalChanged = true;
        return true;
    }

    StringList initialConditions;
    initialConditions = getFloatingSpeciesInitialConditionIds();

    if (initialConditions.Contains(sId))
    {
        int index = initialConditions.indexOf(sId);
        mModel->getModelData().init_y[index] = dValue;
        reset();
        return true;
    }

    Log(lError)<<format("Given Id: '{0}' not found.", sId) + "Only species and global parameter values can be set";
    return false;
}

// Help("Gets the Value of the given species or global parameter (not of local parameters)")
double RoadRunner::getValue(const string& sId)
{
    if (!mModel)
        throw CoreException(gEmptyModelMessage);

    int nIndex = 0;
    if (mModel->getGlobalParameters().find(sId, nIndex))
    {
        return mModel->getModelData().gp[nIndex];
    }
    if (mModel->getBoundarySpecies().find(sId, nIndex))
    {
        return mModel->getModelData().bc[nIndex];
    }

    if (mModel->getFloatingSpeciesConcentrations().find(sId, nIndex))
    {
        return mModel->getModelData().y[nIndex];
    }

    if (mModel->getFloatingSpeciesConcentrations().find(sId.substr(0, sId.size() - 1), nIndex))
    {
        mModel->computeAllRatesOfChange();

        //fs[j] + "'" will be interpreted as rate of change
        return mModel->getModelData().dydt[nIndex];
    }

    if (mModel->getCompartments().find(sId, nIndex))
    {
        return mModel->getModelData().c[nIndex];
    }
    if (mModel->getReactions().find(sId, nIndex))
    {
        return mModel->getModelData().rates[nIndex];
    }

    if (mModel->getConservations().find(sId, nIndex))
    {
        return mModel->getModelData().ct[nIndex];
    }

    StringList initialConditions = getFloatingSpeciesInitialConditionIds();
    if (initialConditions.Contains(sId))
    {
        int index = initialConditions.indexOf(sId);
        return mModel->getModelData().init_y[index];
    }

    string tmp("EE:");
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string parameters = sId.substr(3);
        string p1 = parameters.substr(0, parameters.find_first_of(","));
        string p2 = parameters.substr(parameters.find_first_of(",") + 1);
        return getEE(p1, p2, false);
    }

    tmp = ("uEE:");
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string parameters = sId.substr(4);
        string p1 = parameters.substr(0, parameters.find_first_of(","));
        string p2 = parameters.substr(parameters.find_first_of(",") + 1);
        return getuEE(p1, p2, false);
    }

    tmp = ("eigen_");
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string species = sId.substr(tmp.size());
        int index;
        mModel->getFloatingSpeciesConcentrations().find(species, index);

        //DoubleMatrix mat = getReducedJacobian();
        DoubleMatrix mat;
		if (mComputeAndAssignConservationLaws.getValue())
        {
           mat = getReducedJacobian();
        }
        else
        {
           mat = getFullJacobian();
        }

        vector<Complex> oComplex = ls::getEigenValues(mat);

        if(mSelectionList.size() == 0)
        {
            throw("Tried to access record in empty mSelectionList in getValue function: eigen_");
        }

        if (oComplex.size() > mSelectionList[index + 1].index) //Becuase first one is time !?
        {
            return oComplex[mSelectionList[index + 1].index].Real;
        }
        return std::numeric_limits<double>::quiet_NaN();
    }

    throw CoreException("Given Id: '" + sId + "' not found.",
                                      "Only species, global parameter values and fluxes can be returned");
}

// Help(
//            "Returns symbols of the currently loaded model,
//              that can be used for the selectionlist format array of arrays  { { \"groupname\", { \"item1\", \"item2\" ... } } }."
//            )
NewArrayList RoadRunner::getAvailableTimeCourseSymbols()
{
    NewArrayList oResult;

    if (!mModel)
    {
        return oResult;
    }

    oResult.Add("Floating Species",                 getFloatingSpeciesIds() );
    oResult.Add("Boundary Species",                 getBoundarySpeciesIds() );
    oResult.Add("Floating Species (amount)",        getFloatingSpeciesAmountIds() );
    oResult.Add("Boundary Species (amount)",        getBoundarySpeciesAmountIds() );
    oResult.Add("Global Parameters",                getParameterIds() );
    oResult.Add("Fluxes",                           getReactionIds() );
    oResult.Add("Rates of Change",                  getRateOfChangeIds() );
    oResult.Add("Volumes",                          mModel->getCompartmentNames() );
    oResult.Add("Elasticity Coefficients",          getElasticityCoefficientIds() );
    oResult.Add("Unscaled Elasticity Coefficients", getUnscaledElasticityCoefficientIds() );
    oResult.Add("Eigenvalues",                      getEigenvalueIds() );
    return oResult;
}

string RoadRunner::getVersion()
{
    return RR_VERSION;
}

string RoadRunner::getCopyright()
{
    return "(c) 2009-2012 HM. Sauro and FT. Bergmann, BSD Licence";
}

string RoadRunner::getURL()
{
    return "http://www.sys-bio.org";
}

string RoadRunner::getlibSBMLVersion()
{
    return mNOM.getlibSBMLVersion();
}

// =========================================== NON ENABLED FUNCTIONS BELOW.....


//        [Help("Compute the unscaled elasticity for a given reaction and given species")]
//        double getUnscaledFloatingSpeciesElasticity(string reactionName, string speciesName)
//        {
//            try
//            {
//                if (mModel)
//                {
//                    int speciesIndex = 0;
//                    int reactionIndex = 0;
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    if (!mModel->getFloatingSpeciesConcentrations().find(speciesName, out speciesIndex))
//                        throw CoreException(
//                            "Internal Error: unable to locate species name while computing unscaled elasticity");
//                    if (!mModel->getReactions().find(reactionName, out reactionIndex))
//                        throw CoreException(
//                            "Internal Error: unable to locate reaction name while computing unscaled elasticity");
//
//                    return getUnscaledSpeciesElasticity(reactionIndex, speciesIndex);
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from scaledElasticityMatrix()", e.Message());
//            }
//        }
//


//        [Help(
//            "Compute the value for a particular flux control coefficient, permitted parameters include global parameters, boundary conditions and conservation totals"
//            )]
//        double getUnscaledFluxControlCoefficient(string reactionName, string parameterName)
//        {
//            int fluxIndex;
//            int parameterIndex;
//            TParameterType parameterType;
//            double originalParameterValue;
//            double f1;
//            double f2;
//
//            try
//            {
//                if (mModel)
//                {
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    if (!mModel->getReactions().find(reactionName, out fluxIndex))
//                        throw CoreException(
//                            "Internal Error: unable to locate species name while computing unscaled control coefficient");
//
//                    if (mModel->getGlobalParameters().find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptGlobalParameter;
//                        originalParameterValue = mModel->getModelData().gp[parameterIndex];
//                    }
//                    else if (mModel->getBoundarySpecies().find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptBoundaryParameter;
//                        originalParameterValue = mModel->getModelData().bc[parameterIndex];
//                    }
//                    else if (mModel->getConservations().find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptConservationParameter;
//                        originalParameterValue = mModel->getModelData().ct[parameterIndex];
//                    }
//                    else throw CoreException("Unable to locate parameter: [" + parameterName + "]");
//
//                    double hstep = mDiffStepSize*originalParameterValue;
//                    if (Math.Abs(hstep) < 1E-12)
//                        hstep = mDiffStepSize;
//
//                    try
//                    {
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue + hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        double fi = mModel->getModelData().rates[fluxIndex];
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        double fi2 = mModel->getModelData().rates[fluxIndex];
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        double fd = mModel->getModelData().rates[fluxIndex];
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        double fd2 = mModel->getModelData().rates[fluxIndex];
//
//                        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                        // The following separated lines avoid small amounts of roundoff error
//                        f1 = fd2 + 8*fi;
//                        f2 = -(8*fd + fi2);
//                    }
//                    finally
//                    {
//                        // What ever happens, make sure we restore the species level
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue);
//                        steadyState();
//                    }
//                    return 1/(12*hstep)*(f1 + f2);
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help("Compute the value for a particular scaled flux control coefficients with respect to a local parameter")]
//        double getScaledFluxControlCoefficient(string reactionName, string localReactionName, string parameterName)
//        {
//            int parameterIndex;
//            int reactionIndex;
//
//            try
//            {
//                if (mModel)
//                {
//                    double ufcc = getUnscaledFluxControlCoefficient(reactionName, localReactionName, parameterName);
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    mModel->getReactions().find(reactionName, out reactionIndex);
//                    if (mModel->getGlobalParameters().find(parameterName, out parameterIndex))
//                        return ufccmModel->getModelData().gp[parameterIndex]/mModel->getModelData().rates[reactionIndex];
//                    else if (mModel->getBoundarySpecies().find(parameterName, out parameterIndex))
//                        return ufccmModel->getModelData().bc[parameterIndex]/mModel->getModelData().rates[reactionIndex];
//                    else if (mModel->getConservations().find(parameterName, out parameterIndex))
//                        return ufccmModel->getModelData().ct[parameterIndex]/mModel->getModelData().rates[reactionIndex];
//                    return 0.0;
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help(
//            "Compute the value for a particular scaled flux control coefficients with respect to a global or boundary species parameter"
//            )]
//        double getScaledFluxControlCoefficient(string reactionName, string parameterName)
//        {
//            int parameterIndex;
//            int reactionIndex;
//
//            try
//            {
//                if (mModel)
//                {
//                    double ufcc = getUnscaledFluxControlCoefficient(reactionName, parameterName);
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    mModel->getReactions().find(reactionName, out reactionIndex);
//                    if (mModel->getGlobalParameters().find(parameterName, out parameterIndex))
//                        return ufccmModel->getModelData().gp[parameterIndex]/mModel->getModelData().rates[reactionIndex];
//                    else if (mModel->getBoundarySpecies().find(parameterName, out parameterIndex))
//                        return ufccmModel->getModelData().bc[parameterIndex]/mModel->getModelData().rates[reactionIndex];
//                    else if (mModel->getConservations().find(parameterName, out parameterIndex))
//                        return ufccmModel->getModelData().ct[parameterIndex]/mModel->getModelData().rates[reactionIndex];
//                    return 0.0;
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//    }
//}


//        [Help(
//            "Compute the value for a particular unscaled concentration control coefficients with respect to a local parameter"
//            )]
//        double getUnscaledConcentrationControlCoefficient(string speciesName, string localReactionName, string parameterName)
//        {
//            int parameterIndex;
//            int reactionIndex;
//            int speciesIndex;
//            double f1;
//            double f2;
//
//            try
//            {
//                if (mModel)
//                {
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    if (!mModel->getReactions().find(localReactionName, out reactionIndex))
//                        throw CoreException(
//                            "Internal Error: unable to locate reaction name while computing unscaled control coefficient");
//
//                    if (!mModel->getFloatingSpeciesConcentrations().find(speciesName, out speciesIndex))
//                        throw CoreException(
//                            "Internal Error: unable to locate species name while computing unscaled control coefficient");
//
//                    // Look for the parameter name
//                    if (mModelGenerator->localParameterList[reactionIndex].find(parameterName,
//                                                                                       out parameterIndex))
//                    {
//                        double originalParameterValue = mModel->getModelData().lp[reactionIndex][parameterIndex];
//                        double hstep = mDiffStepSize*originalParameterValue;
//                        if (Math.Abs(hstep) < 1E-12)
//                            hstep = mDiffStepSize;
//
//                        try
//                        {
//                            mModel->convertToConcentrations();
//                            mModel->getModelData().lp[reactionIndex][parameterIndex] = originalParameterValue + hstep;
//                            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                            double fi = mModel->getConcentration(speciesIndex);
//
//                            mModel->getModelData().lp[reactionIndex][parameterIndex] = originalParameterValue + 2*hstep;
//                            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                            double fi2 = mModel->getConcentration(speciesIndex);
//
//                            mModel->getModelData().lp[reactionIndex][parameterIndex] = originalParameterValue - hstep;
//                            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                            double fd = mModel->getConcentration(speciesIndex);
//
//                            mModel->getModelData().lp[reactionIndex][parameterIndex] = originalParameterValue - 2*hstep;
//                            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                            double fd2 = mModel->getConcentration(speciesIndex);
//
//                            // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                            // The following separated lines avoid small amounts of roundoff error
//                            f1 = fd2 + 8*fi;
//                            f2 = -(8*fd + fi2);
//                        }
//                        finally
//                        {
//                            // What ever happens, make sure we restore the species level
//                            mModel->getModelData().lp[reactionIndex][parameterIndex] = originalParameterValue;
//                        }
//                        return 1/(12*hstep)*(f1 + f2);
//                    }
//                    else
//                        throw CoreException("Unable to locate local parameter [" + parameterName +
//                                                          "] in reaction [" + localReactionName + "]");
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help(
//            "Compute the value for a particular scaled concentration control coefficients with respect to a local parameter"
//            )]
//        double getScaledConcentrationControlCoefficient(string speciesName, string localReactionName, string parameterName)
//        {
//            int localReactionIndex;
//            int parameterIndex;
//            int speciesIndex;
//
//            try
//            {
//                if (mModel)
//                {
//                    double ucc = getUnscaledConcentrationControlCoefficient(speciesName, localReactionName,
//                                                                            parameterName);
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    mModel->getReactions().find(localReactionName, out localReactionIndex);
//                    mModel->getFloatingSpeciesConcentrations().find(localReactionName, out speciesIndex);
//                    mModelGenerator->localParameterList[localReactionIndex].find(parameterName,
//                                                                                        out parameterIndex);
//
//                    return uccmModel->getModelData().lp[localReactionIndex][parameterIndex]/mModel->getConcentration(speciesIndex);
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help(
//            "Compute the value for a particular concentration control coefficient, permitted parameters include global parameters, boundary conditions and conservation totals"
//            )]
//        double getUnscaledConcentrationControlCoefficient(string speciesName, string parameterName)
//        {
//            int speciesIndex;
//            int parameterIndex;
//            TParameterType parameterType;
//            double originalParameterValue;
//            double f1;
//            double f2;
//
//            try
//            {
//                if (mModel)
//                {
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    if (!mModel->getFloatingSpeciesConcentrations().find(speciesName, out speciesIndex))
//                        throw CoreException(
//                            "Internal Error: unable to locate species name while computing unscaled control coefficient");
//
//                    if (mModel->getGlobalParameters().find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptGlobalParameter;
//                        originalParameterValue = mModel->getModelData().gp[parameterIndex];
//                    }
//                    else if (mModel->getBoundarySpecies().find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptBoundaryParameter;
//                        originalParameterValue = mModel->getModelData().bc[parameterIndex];
//                    }
//                    else if (mModel->getConservations().find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptConservationParameter;
//                        originalParameterValue = mModel->getModelData().ct[parameterIndex];
//                    }
//                    else throw CoreException("Unable to locate parameter: [" + parameterName + "]");
//
//                    double hstep = mDiffStepSize*originalParameterValue;
//                    if (Math.Abs(hstep) < 1E-12)
//                        hstep = mDiffStepSize;
//
//                    try
//                    {
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue + hstep);
//                        steadyState();
//                        mModel->convertToConcentrations();
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        double fi = mModel->getConcentration(speciesIndex);
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        double fi2 = mModel->getConcentration(speciesIndex);
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        double fd = mModel->getConcentration(speciesIndex);
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        double fd2 = mModel->getConcentration(speciesIndex);
//
//                        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                        // The following separated lines avoid small amounts of roundoff error
//                        f1 = fd2 + 8*fi;
//                        f2 = -(8*fd + fi2);
//                    }
//                    finally
//                    {
//                        // What ever happens, make sure we restore the species level
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue);
//                        steadyState();
//                    }
//                    return 1/(12*hstep)*(f1 + f2);
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help(
//            "Compute the value for a particular scaled concentration control coefficients with respect to a global or boundary species parameter"
//            )]
//        double getScaledConcentrationControlCoefficient(string speciesName, string parameterName)
//        {
//            int parameterIndex;
//            int speciesIndex;
//
//            try
//            {
//                if (mModel)
//                {
//                    double ucc = getUnscaledConcentrationControlCoefficient(speciesName, parameterName);
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    mModel->getFloatingSpeciesConcentrations().find(speciesName, out speciesIndex);
//                    if (mModel->getGlobalParameters().find(parameterName, out parameterIndex))
//                        return uccmModel->getModelData().gp[parameterIndex]/mModel->getConcentration(speciesIndex);
//                    else if (mModel->getBoundarySpecies().find(parameterName, out parameterIndex))
//                        return uccmModel->getModelData().bc[parameterIndex]/mModel->getConcentration(speciesIndex);
//                    else if (mModel->getConservations().find(parameterName, out parameterIndex))
//                        return uccmModel->getModelData().ct[parameterIndex]/mModel->getConcentration(speciesIndex);
//                    return 0.0;
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        // ----------------------------------------------------------------------------------------------
//
//
//        [Help("Compute the value for a particular unscaled flux control coefficients with respect to a local parameter")
//        ]
//        double getUnscaledFluxControlCoefficient(string fluxName, string localReactionName, string parameterName)
//        {
//            int parameterIndex;
//            int localReactionIndex;
//            int fluxIndex;
//            double f1;
//            double f2;
//
//            try
//            {
//                if (mModel)
//                {
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//
//                    if (!mModel->getReactions().find(localReactionName, out localReactionIndex))
//                        throw CoreException(
//                            "Internal Error: unable to locate reaction name while computing unscaled control coefficient");
//
//                    if (!mModel->getReactions().find(fluxName, out fluxIndex))
//                        throw CoreException(
//                            "Internal Error: unable to locate reaction name while computing unscaled control coefficient");
//
//                    // Look for the parameter name
//                    if (mModelGenerator->localParameterList[localReactionIndex].find(parameterName,
//                                                                                            out parameterIndex))
//                    {
//                        double originalParameterValue = mModel->getModelData().lp[localReactionIndex][parameterIndex];
//                        double hstep = mDiffStepSize*originalParameterValue;
//                        if (Math.Abs(hstep) < 1E-12)
//                            hstep = mDiffStepSize;
//
//                        try
//                        {
//                            mModel->convertToConcentrations();
//                            mModel->getModelData().lp[localReactionIndex][parameterIndex] = originalParameterValue + hstep;
//                            steadyState();
//                            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                            double fi = mModel->getModelData().rates[fluxIndex];
//
//                            mModel->getModelData().lp[localReactionIndex][parameterIndex] = originalParameterValue + 2*hstep;
//                            steadyState();
//                            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                            double fi2 = mModel->getModelData().rates[fluxIndex];
//
//                            mModel->getModelData().lp[localReactionIndex][parameterIndex] = originalParameterValue - hstep;
//                            steadyState();
//                            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                            double fd = mModel->getModelData().rates[fluxIndex];
//
//                            mModel->getModelData().lp[localReactionIndex][parameterIndex] = originalParameterValue - 2*hstep;
//                            steadyState();
//                            mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                            double fd2 = mModel->getModelData().rates[fluxIndex];
//
//                            // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                            // The following separated lines avoid small amounts of roundoff error
//                            f1 = fd2 + 8*fi;
//                            f2 = -(8*fd + fi2);
//                        }
//                        finally
//                        {
//                            // What ever happens, make sure we restore the species level
//                            mModel->getModelData().lp[localReactionIndex][parameterIndex] = originalParameterValue;
//                            steadyState();
//                        }
//                        return 1/(12*hstep)*(f1 + f2);
//                    }
//                    else
//                        throw CoreException("Unable to locate local parameter [" + parameterName +
//                                                          "] in reaction [" + localReactionName + "]");
//                }
//                else throw CoreException(gEmptyModelMessage);
//            }
//            catch (CoreException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw CoreException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//

//        [Help(
//            "Returns the elasticity of a given reaction to a given parameter. Parameters can be boundary species or global parameters"
//            )]
//        double getUnScaledElasticity(string reactionName, string parameterName)
//        {
//            if (!mModel) throw CoreException(gEmptyModelMessage);
//            double f1, f2, fi, fi2, fd, fd2;
//            double hstep;
//
//            int reactionId = -1;
//            if (!(mModel->getReactions().find(reactionName, out reactionId)))
//                throw CoreException("Unrecognized reaction name in call to getUnScaledElasticity [" +
//                                                  reactionName + "]");
//
//            int index = -1;
//            // Find out what kind of parameter it is, species or global parmaeter
//            if (mModel->getBoundarySpecies().find(parameterName, out index))
//            {
//                double originalParameterValue = mModel->getModelData().bc[index];
//                hstep = mDiffStepSize*originalParameterValue;
//                if (Math.Abs(hstep) < 1E-12)
//                    hstep = mDiffStepSize;
//
//                try
//                {
//                    mModel->convertToConcentrations();
//                    mModel->getModelData().bc[index] = originalParameterValue + hstep;
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                    fi = mModel->getModelData().rates[reactionId];
//
//                    mModel->getModelData().bc[index] = originalParameterValue + 2*hstep;
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                    fi2 = mModel->getModelData().rates[reactionId];
//
//                    mModel->getModelData().bc[index] = originalParameterValue - hstep;
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                    fd = mModel->getModelData().rates[reactionId];
//
//                    mModel->getModelData().bc[index] = originalParameterValue - 2*hstep;
//                    mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                    fd2 = mModel->getModelData().rates[reactionId];
//
//                    // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                    // The following separated lines avoid small amounts of roundoff error
//                    f1 = fd2 + 8*fi;
//                    f2 = -(8*fd + fi2);
//                }
//                finally
//                {
//                    mModel->getModelData().bc[index] = originalParameterValue;
//                }
//            }
//            else
//            {
//                if (mModel->getGlobalParameters().find(parameterName, out index))
//                {
//                    double originalParameterValue = mModel->getModelData().gp[index];
//                    hstep = mDiffStepSize*originalParameterValue;
//                    if (Math.Abs(hstep) < 1E-12)
//                        hstep = mDiffStepSize;
//
//                    try
//                    {
//                        mModel->convertToConcentrations();
//
//                        mModel->getModelData().gp[index] = originalParameterValue + hstep;
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        fi = mModel->getModelData().rates[reactionId];
//
//                        mModel->getModelData().gp[index] = originalParameterValue + 2*hstep;
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        fi2 = mModel->getModelData().rates[reactionId];
//
//                        mModel->getModelData().gp[index] = originalParameterValue - hstep;
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        fd = mModel->getModelData().rates[reactionId];
//
//                        mModel->getModelData().gp[index] = originalParameterValue - 2*hstep;
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        fd2 = mModel->getModelData().rates[reactionId];
//
//                        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                        // The following separated lines avoid small amounts of roundoff error
//                        f1 = fd2 + 8*fi;
//                        f2 = -(8*fd + fi2);
//                    }
//                    finally
//                    {
//                        mModel->getModelData().gp[index] = originalParameterValue;
//                    }
//                }
//                else if (mModel->getConservations().find(parameterName, out index))
//                {
//                    double originalParameterValue = mModel->getModelData().gp[index];
//                    hstep = mDiffStepSize*originalParameterValue;
//                    if (Math.Abs(hstep) < 1E-12)
//                        hstep = mDiffStepSize;
//
//                    try
//                    {
//                        mModel->convertToConcentrations();
//
//                        mModel->getModelData().ct[index] = originalParameterValue + hstep;
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        fi = mModel->getModelData().rates[reactionId];
//
//                        mModel->getModelData().ct[index] = originalParameterValue + 2*hstep;
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        fi2 = mModel->getModelData().rates[reactionId];
//
//                        mModel->getModelData().ct[index] = originalParameterValue - hstep;
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        fd = mModel->getModelData().rates[reactionId];
//
//                        mModel->getModelData().ct[index] = originalParameterValue - 2*hstep;
//                        mModel->computeReactionRates(mModel->getTime(), mModel->getModelData().y);
//                        fd2 = mModel->getModelData().rates[reactionId];
//
//                        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                        // The following separated lines avoid small amounts of roundoff error
//                        f1 = fd2 + 8*fi;
//                        f2 = -(8*fd + fi2);
//                    }
//                    finally
//                    {
//                        mModel->getModelData().ct[index] = originalParameterValue;
//                    }
//                }
//                else
//                    throw CoreException("Unrecognized parameter name in call to getUnScaledElasticity [" +
//                                                      parameterName + "]");
//            }
//            return 1/(12*hstep)*(f1 + f2);
//        }

// Help("Returns the value of a compartment by its index")
//        void setCompartmentVolumes(double[] values)
//        {
//            if (!mModel)
//                throw CoreException(gEmptyModelMessage);
//            if (values.Length < mModel->getNumCompartments)
//                mModel->getModelData().c = values;
//            else
//                throw (new CoreException(String.format("Size of vector out not in range in setCompartmentValues: [{0}]", values.Length)));
//        }
//

// Help("Sets the value of a global parameter by its index")
//        void RoadRunner::setLocalParameterByIndex(int reactionId, int index, double value)
//        {
//            if (!mModel) throw CoreException(gEmptyModelMessage);
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions) &&
//                (index >= 0) && (index < mModel->getNumLocalParameters(reactionId)))
//                mModel->getModelData().lp[reactionId][index] = value;
//            else
//                throw CoreException(string.format("Index in setLocalParameterByIndex out of range: [{0}]", index));
//        }
//


// Help("Returns the values selected with setTimeCourseSelectionList() for the current model time / timestep")
vector<double> RoadRunner::getSelectedValues()
{
    if (!mModel)
    {
    	throw CoreException(gEmptyModelMessage);
    }

    vector<double> result;
    result.resize(mSelectionList.size());

    for (int i = 0; i < mSelectionList.size(); i++)
    {
    	result[i] = getNthSelectedOutput(i, mModel->getModelData().time);
    }
    return result;
}


// Help("When turned on, this method will cause rates, event assignments, rules and such to be multiplied " +
//              "with the compartment volume, if species are defined as initialAmounts. By default this behavior is off.")
//
//        void RoadRunner::reMultiplyCompartments(bool bValue)
//        {
//            _ReMultiplyCompartments = bValue;
//        }
//
// Help("Performs a steady state parameter scan with the given parameters returning all elments from the mSelectionList: (format: symnbol, startValue, endValue, stepSize)")
//        double[][] RoadRunner::steadyStateParameterScan(string symbol, double startValue, double endValue, double stepSize)
//        {
//            var results = new List<double[]>();
//
//            double initialValue = getValue(symbol);
//            double current = startValue;
//
//            while (current < endValue)
//            {
//                setValue(symbol, current);
//                try
//                {
//                    steadyState();
//                }
//                catch (Exception)
//                {
//                    //
//                }
//
//                var currentRow = new List<double> {current};
//                currentRow.AddRange(getSelectedValues());
//
//                results.add(currentRow.ToArray());
//                current += stepSize;
//            }
//            setValue(symbol, initialValue);
//
//            return results.ToArray();
//        }
//
//

// Help("Set the values for all global parameters in the model")
//        void RoadRunner::setLocalParameterValues(int reactionId, double[] values)
//        {
//            if (!mModel) throw CoreException(gEmptyModelMessage);
//
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions))
//                mModel->getModelData().lp[reactionId] = values;
//            else
//                throw CoreException(String.format("Index in setLocalParameterValues out of range: [{0}]", reactionId));
//        }
//
// Help("Get the values for all global parameters in the model")
//        double[] RoadRunner::getLocalParameterValues(int reactionId)
//        {
//            if (!mModel)
//                throw CoreException(gEmptyModelMessage);
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions))
//                return mModel->getModelData().lp[reactionId];
//            throw CoreException(String.format("Index in getLocalParameterValues out of range: [{0}]", reactionId));
//        }
//
// Help("Gets the list of parameter names")
//        ArrayList RoadRunner::getLocalParameterNames(int reactionId)
//        {
//            if (!mModel)
//                throw CoreException(gEmptyModelMessage);
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions))
//                return mModelGenerator->getLocalParameterList(reactionId);
//            throw (new CoreException("reaction Id out of range in call to getLocalParameterNames"));
//        }
//
// Help("Returns a list of global parameter tuples: { {parameter Name, value},...")
//        ArrayList RoadRunner::getAllLocalParameterTupleList()
//        {
//            if (!mModel)
//                throw CoreException(gEmptyModelMessage);
//
//            var tupleList = new ArrayList();
//            for (int i = 0; i < mModelGenerator->getNumberOfReactions(); i++)
//            {
//                var tuple = new ArrayList();
//                ArrayList lpList = mModelGenerator->getLocalParameterList(i);
//                tuple.add(i);
//                for (int j = 0; j < lpList.Count; j++)
//                {
//                    tuple.add(lpList[j]);
//                    tuple.Add(mModel->mData.lp[i][j]);
//                }
//                tupleList.add(tuple);
//            }
//            return tupleList;
//        }
//


}//namespace

//We only need to give the linker the folder where libs are
//using the pragma comment. Automatic lining works for MSVC and codegear

#if defined(CG_IDE)
#pragma comment(lib, "sundials_cvode.lib")
#pragma comment(lib, "sundials_nvecserial.lib")
#pragma comment(lib, "nleq-static.lib")
#pragma comment(lib, "pugi-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "libsbml-static.lib")
#pragma comment(lib, "libxml2_xe.lib")
#pragma comment(lib, "blas.lib")
#pragma comment(lib, "lapack.lib")
#pragma comment(lib, "libf2c.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#endif

#if defined(_WIN32)
#pragma comment(lib, "IPHLPAPI.lib") //Becuase of poco needing this
#endif



#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iostream>
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
#include "rrCapabilities.h"
#include "rrConstants.h"
#include "rrVersionInfo.h"
#include "rrCVODEInterface.h"
#include "rrNLEQInterface.h"
#include "c/rrNOMSupport.h"
#include "Poco/File.h"
#include "Poco/Mutex.h"
#include <sbml/common/libsbml-version.h>
//---------------------------------------------------------------------------

namespace rr
{
using namespace std;
using namespace ls;
using Poco::Mutex;


// we can write a single function to pick the string lists out
// of the model instead of duplicating it 6 times with
// fun ptrs.
typedef string (ExecutableModel::*GetNameFuncPtr)(int);
typedef int (ExecutableModel::*GetNumFuncPtr)();

// make this static here, hide our implementation...
static vector<string> createModelStringList(ExecutableModel *model,
        GetNumFuncPtr numFunc, GetNameFuncPtr nameFunc)
{
    if (!model)
    {
        throw CoreException(gEmptyModelMessage);
    }
    const int num = (model->*numFunc)();
    vector<string> strings(num);

    for(int i = 0; i < num; i++)
    {
        strings[i] = (model->*nameFunc)(i);
    }

    return strings;
}


//The instance count increases/decreases as instances are created/destroyed.
int                   RoadRunner::mInstanceCount = 0;


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
mCapabilities("RoadRunner", "RoadRunner Capabilities"),
mRRCoreCapabilities("Road Runner Core", "", "Core RoadRunner Parameters"),
mSteadyStateThreshold(1.E-2),
mCurrentSBMLFileName(""),
mSimulation(NULL),
mCVode(NULL),
mComputeAndAssignConservationLaws("Conservation", false, "enables (=true) or disables "
                                  "(=false) the conservation analysis "
                                  "of models for timecourse simulations."),
mTimeStart(0),
mTimeEnd(10),
mNumPoints(21),
mModel(NULL),
mCurrentSBML(""),
mPluginManager(joinPath(getParentFolder(supportCodeFolder), "plugins"))


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
    mCVode = new CvodeInterface(NULL);

    if(mCVode)
    {
        mCapabilities.add(mCVode->getCapability());
    }

    // we currently use NLEQInterface as the only steady state solver.
    // should this change in the future, this should be replaced
    // with a factory pattern.
    NLEQInterface ss = NLEQInterface();
    mCapabilities.add(ss.getCapability());
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
        info<<"ModelName: "            <<  mModel->getModelName()<<endl;
//        info<<"Model DLL Loaded: "    << (mModel->mDLL.isLoaded() ? "true" : "false")    <<endl;
//        info<<"Initialized: "        << (mModel->mIsInitialized ? "true" : "false")    <<endl;
    }
    info<<"ConservationAnalysis: "    <<    (mComputeAndAssignConservationLaws.getValue() ? "true" : "false")<<endl;
    info<<"libSBML version: "        <<    getlibSBMLVersion()<<endl;
    info<<"Temporary folder: "        <<    getTempFolder()<<endl;
    info<<"Compiler location: "        <<    getCompiler()->getCompilerLocation()<<endl;
    info<<"Support Code Folder: "    <<    getCompiler()->getSupportCodeFolder()<<endl;
    info<<"Working Directory: "        <<    getCWD()<<endl;
    return info.str();
}

string RoadRunner::getExtendedVersionInfo()
{
    stringstream info;
    info << "Version: "                << getVersion() << endl;
    info<<"libSBML version: "          << getlibSBMLVersion() << endl;
    info<<"Working Directory: "        << getCWD() << endl;
    return info.str();
}

PluginManager&    RoadRunner::getPluginManager()
{
    return mPluginManager;
}


LibStructural* RoadRunner::getLibStruct()
{
    return &mLS;
}

Compiler* RoadRunner::getCompiler()
{
    return mModelGenerator ? mModelGenerator->getCompiler() : 0;
}


bool RoadRunner::setCompiler(const string& compiler)
{
    return mModelGenerator ? mModelGenerator->setCompiler(compiler) : false;
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
    return mModelGenerator ? mModelGenerator->setTemporaryDirectory(folder) : false;
}

string RoadRunner::getTempFolder()
{
    return mModelGenerator ? mModelGenerator->getTemporaryDirectory() : "";
}

int RoadRunner::createDefaultTimeCourseSelectionList()
{
    vector<string> theList;
    vector<string> oFloating  = getFloatingSpeciesIds();

    theList.push_back("time");
    for(int i = 0; i < oFloating.size(); i++)
    {
        theList.push_back(oFloating[i]);
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
    vector<string> theList = getSelectionListFromSettings(mSettings);

    if(theList.size() < 2)
    {
        //AutoSelect
        theList.push_back("Time");

       //Get All floating species
       vector<string> oFloating  = getFloatingSpeciesIds();
       for(int i = 0; i < oFloating.size(); i++)
       {
            theList.push_back(oFloating[i]);
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
        mModel->setConservedSumChanged(false);
        mModel->evalInitialConditions();

        if(mCVode)
        {
            delete mCVode;
        }
        mCVode = new CvodeInterface(mModel);

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

RoadRunnerData* RoadRunner::getRoadRunnerData()
{
    return &mRoadRunnerData;
}

double RoadRunner::getValueForRecord(const SelectionRecord& record)
{
    double dResult;

    switch (record.selectionType)
    {
    case SelectionRecord::clFloatingSpecies:

        dResult = 0;
        mModel->getFloatingSpeciesConcentrations(1, &record.index, &dResult);
        break;

    case SelectionRecord::clBoundarySpecies:
        mModel->getBoundarySpeciesConcentrations(1, &record.index, &dResult);
        break;

    case SelectionRecord::clFlux:
        dResult = 0;
        mModel->getReactionRates(1, &record.index, &dResult);
        break;

    case SelectionRecord::clRateOfChange:
        dResult = 0;
        mModel->getFloatingSpeciesAmountRates(1, &record.index, &dResult);
        break;

    case SelectionRecord::clVolume:
        mModel->getCompartmentVolumes(1, &record.index, &dResult);
        break;

    case SelectionRecord::clParameter:
    {
        if (record.index > ((mModel->getNumGlobalParameters()) - 1))
        {
            int index = record.index - mModel->getNumGlobalParameters();
            mModel->getConservedSums(1, &index, &dResult);
        }
        else
        {
            mModel->getGlobalParameterValues(1, &record.index, &dResult);
        }
    }
    break;

    case SelectionRecord::clFloatingAmount:
        mModel->getFloatingSpeciesAmounts(1, &record.index, &dResult);
        break;

    case SelectionRecord::clBoundaryAmount:
        mModel->getBoundarySpeciesAmounts(1, &record.index, &dResult);
        break;

    case SelectionRecord::clElasticity:
        dResult = getEE(record.p1, record.p2, false);
        break;

    case SelectionRecord::clUnscaledElasticity:
        dResult = getuEE(record.p1, record.p2, false);
        break;

        // ********  Todo: Enable this.. ***********
    case SelectionRecord::clEigenValue:
        //            vector< complex<double> >oComplex = LA.GetEigenValues(getReducedJacobian());
        //            if (oComplex.Length > record.index)
            //            {
            //                dResult = oComplex[record.index].Real;
            //            }
        //            else
            //                dResult = Double.NaN;
        dResult = 0.0;
        break;

    case SelectionRecord::clStoichiometry:
        dResult = mModel->getStoichiometry(record.index);
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

    if (record.selectionType == SelectionRecord::clTime)
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

    // evalute the model with its current state
    mModel->evalModel(mTimeStart, 0, 0);

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

    // why is this here???
    vector<string> list = getTimeCourseSelectionList();
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

bool RoadRunner::loadSBML(const string& sbml, const bool& forceReCompile)
{
    static Mutex libSBMLMutex;
    mCurrentSBML = sbml;

    //clear temp folder of roadrunner generated files, only if roadRunner instance == 1
    Log(lDebug)<<"Loading SBML into simulator";
    if (!sbml.size())
    {
        throw(CoreException("SBML string is empty!"));
    }

    {
        //Scope for Mutex
        //There is something in here that is not threadsafe... causes crash with multiple threads, without mutex
        Mutex::ScopedLock lock(libSBMLMutex);
        loadSBMLIntoLibStruct(sbml);
    }

    delete mModel;
    uint options = 0;
    options |= (forceReCompile ? ModelGenerator::ForceReCompile : 0);
    options |= (computeAndAssignConservationLaws()
            ? ModelGenerator::ComputeAndAssignConsevationLaws : 0);
    mModel = mModelGenerator->createModel(sbml, options);

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
        mModel->reset();

        if (mComputeAndAssignConservationLaws.getValue() && !mModel->getConservedSumChanged())
        {
            mModel->computeConservedTotals();
        }

        mCVode->assignNewVector(mModel, true);
        mCVode->testRootsAtInitialTime();

        mModel->setTime(0.0);
        mCVode->reStart(0.0, mModel);

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
        stringstream msg;
        msg<<"Problem in simulate: "<<e.Message();
        Log(lError)<<msg.str();

        throw(CoreException(msg.str()));
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
    vector<string> list = getTimeCourseSelectionList();

    mRoadRunnerData.setColumnNames(list);
    mRoadRunnerData.setData(mRawRoadRunnerData);
    return true;
}


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

vector<string> RoadRunner::getTimeCourseSelectionList()
{
    vector<string> oResult;

    if (!mModel)
    {
        oResult.push_back("time");
        return oResult;
    }

    vector<string> oFloating    = getFloatingSpeciesIds();
    vector<string> oBoundary    = getBoundarySpeciesIds();
    vector<string> oFluxes      = getReactionIds();
    vector<string> oVolumes     = getCompartmentIds();
    vector<string> oRates       = getRateOfChangeIds();
    vector<string> oParameters  = getParameterIds();

    vector<SelectionRecord>::iterator iter;

    for(iter = mSelectionList.begin(); iter != mSelectionList.end(); iter++)
    {
        SelectionRecord record = (*iter);
        switch (record.selectionType)
        {
            case SelectionRecord::clTime:
                oResult.push_back("time");
                break;
            case SelectionRecord::clBoundaryAmount:
                oResult.push_back(format("[{0}]", oBoundary[record.index]));
                break;
            case SelectionRecord::clBoundarySpecies:
                oResult.push_back(oBoundary[record.index]);
                break;
            case SelectionRecord::clFloatingAmount:
                oResult.push_back(format("[{0}]", oFloating[record.index]));
                break;
            case SelectionRecord::clFloatingSpecies:
                oResult.push_back(oFloating[record.index]);
                break;
            case SelectionRecord::clVolume:
                oResult.push_back(oVolumes[record.index]);
                break;
            case SelectionRecord::clFlux:
                oResult.push_back(oFluxes[record.index]);
                break;
            case SelectionRecord::clRateOfChange:
                oResult.push_back(oRates[record.index]);
                break;
            case SelectionRecord::clParameter:
                oResult.push_back(oParameters[record.index]);
                break;
            case SelectionRecord::clEigenValue:
                oResult.push_back("eigen_" + record.p1);
                break;
            case SelectionRecord::clElasticity:
                oResult.push_back(format("EE:{0},{1}", record.p1, record.p2));
                break;
            case SelectionRecord::clUnscaledElasticity:
                oResult.push_back(format("uEE:{0},{1}", record.p1, record.p2));
                break;
            case SelectionRecord::clStoichiometry:
                oResult.push_back(record.p1);
                break;
            default:
                // return empty list
                break;
        }
    }
    return oResult;
}


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

    NLEQInterface steadyStateSolver(mModel);

    //Get a std vector for the solver
    vector<double> someAmounts(mModel->getNumIndependentSpecies(), 0);
    mModel->getFloatingSpeciesAmounts(someAmounts.size(), 0, &someAmounts[0]);

    double ss = steadyStateSolver.solve(someAmounts);
    if(ss < 0)
    {
        Log(lError)<<"Steady State solver failed...";
    }
    mModel->convertToConcentrations();

    return ss;
}

void RoadRunner::setParameterValue(const TParameterType::TParameterType parameterType,
        const int parameterIndex, const double value)
{
    switch (parameterType)
    {
        case TParameterType::ptBoundaryParameter:
            mModel->setBoundarySpeciesConcentrations(1, &parameterIndex, &value);
        break;

        case TParameterType::ptGlobalParameter:
            mModel->setGlobalParameterValues(1, &parameterIndex, &value);
        break;

        case TParameterType::ptFloatingSpecies:
            mModel->setFloatingSpeciesConcentrations(1, &parameterIndex, &value);
        break;

        case TParameterType::ptConservationParameter:
            mModel->setConservedSums(1, &parameterIndex, &value);
        break;

        case TParameterType::ptLocalParameter:
            throw Exception("Local parameters not permitted in setParameterValue (getCC, getEE)");
    }
}

double RoadRunner::getParameterValue(const TParameterType::TParameterType parameterType,
        const int parameterIndex)
{
    switch (parameterType)
    {
    case TParameterType::ptBoundaryParameter:
    {
        double result = 0;
        mModel->getBoundarySpeciesConcentrations(1, &parameterIndex, &result);
        return result;
    }
    break;
    case TParameterType::ptGlobalParameter:
    {
        double result = 0;
        mModel->getGlobalParameterValues(1, &parameterIndex, &result);
        return result;
    }
    break;

    // Used when calculating elasticities
    case TParameterType::ptFloatingSpecies:
    {
        double result = 0;
        mModel->getFloatingSpeciesConcentrations(1, &parameterIndex, &result);
        return result;
    }
    break;

    case TParameterType::ptConservationParameter:
    {
        double result = 0;
        mModel->getConservedSums(1, &parameterIndex, &result);
        return result;
    }
    break;

    case TParameterType::ptLocalParameter:
        throw Exception("Local parameters not permitted in getParameterValue (getCC?)");
        break;

    default:
        return 0.0;
        break;
    }
    return 0;
}

// Help("This method turns on / off the computation and adherence to conservation laws."
//              + "By default roadRunner will discover conservation cycles and reduce the model accordingly.")
void RoadRunner::computeAndAssignConservationLaws(const bool& bValue)
{
    if(bValue == mComputeAndAssignConservationLaws.getValue())
    {
        Log(lDebug)<<"The compute and assign conservation laws flag already set to : "<<toString(bValue);
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
vector<string> RoadRunner::getRateOfChangeIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    vector<string> sp = getFloatingSpeciesIds(); // Reordered list
    for (int i = 0; i < sp.size(); i++)
    {
        sp[i] = sp[i] + "'";
    }
    return sp;
}

// Help("Gets the list of compartment names")
vector<string> RoadRunner::getCompartmentIds()
{
    return createModelStringList(mModel, &ExecutableModel::getNumCompartments,
            &ExecutableModel::getCompartmentId);
}

vector<string> RoadRunner::getParameterIds()
{
    return createModelStringList(mModel, &ExecutableModel::getNumGlobalParameters,
            &ExecutableModel::getGlobalParameterId);
}

// [Help("Get scaled elasticity coefficient with respect to a global parameter or species")]
double RoadRunner::getEE(const string& reactionName, const string& parameterName)
{
    return getEE(reactionName, parameterName, true);
}

double RoadRunner::getEE(const string& reactionName, const string& parameterName, bool computeSteadyState)
{
    TParameterType::TParameterType parameterType;
    int reactionIndex;
    int parameterIndex;

    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    // Check the reaction name
    if ((reactionIndex = mModel->getReactionIndex(reactionName)) < 0)
    {
        throw CoreException(format("Unable to locate reaction name: [{0}]", reactionName));
    }

    // Find out what kind of parameter we are dealing with
    if (( parameterIndex = mModel->getFloatingSpeciesIndex(parameterName)) >= 0)
    {
        parameterType = TParameterType::ptFloatingSpecies;
    }
    else if ((parameterIndex = mModel->getBoundarySpeciesIndex(parameterName)) >= 0)
    {
        parameterType = TParameterType::ptBoundaryParameter;
    }
    else if ((parameterIndex = mModel->getGlobalParameterIndex(parameterName)) >= 0)
    {
        parameterType = TParameterType::ptGlobalParameter;
    }
    else if ((parameterIndex = mModel->getConservedSumIndex(parameterName)) >= 0)
    {
        parameterType = TParameterType::ptConservationParameter;
    }
    else
    {
        throw CoreException(format("Unable to locate variable: [{0}]", parameterName));
    }

    mModel->evalReactionRates();

    double variableValue = 0;
    mModel->getReactionRates(1, &reactionIndex, &variableValue);
    double parameterValue = getParameterValue(parameterType, parameterIndex);
    if (variableValue == 0)
    {
        variableValue = 1e-12;
    }
    return getuEE(reactionName, parameterName, computeSteadyState) * parameterValue / variableValue;
}


double RoadRunner::getuEE(const string& reactionName, const string& parameterName)
{
    return getuEE(reactionName, parameterName, true);
}

class aFinalizer
{
private:
    TParameterType::TParameterType    mParameterType;
    int    mParameterIndex;
    double    mOriginalParameterValue;
    bool    mComputeSteadyState;
    RoadRunner*    mRR;

public:
    aFinalizer(TParameterType::TParameterType& pType, const int& pIndex,
            const double& origValue, const bool& doWhat, RoadRunner* aRoadRunner)
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
        mRR->getModel()->evalReactionRates();
        if (mComputeSteadyState)
        {
            mRR->steadyState();
        }
    }
};

double RoadRunner::getuEE(const string& reactionName, const string& parameterName, bool computeSteadystate)
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        TParameterType::TParameterType parameterType;
        double originalParameterValue;
        int reactionIndex;
        int parameterIndex;

        mModel->convertToConcentrations();
        mModel->evalReactionRates();

        // Check the reaction name
        if ((reactionIndex = mModel->getReactionIndex(reactionName)) < 0)
        {
            throw CoreException("Unable to locate reaction name: [" + reactionName + "]");
        }

        // Find out what kind of parameter we are dealing with
        if ((parameterIndex = mModel->getFloatingSpeciesIndex(parameterName)) >= 0)
        {
            parameterType = TParameterType::ptFloatingSpecies;
            originalParameterValue = 0;
            mModel->getFloatingSpeciesConcentrations(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getBoundarySpeciesIndex(parameterName)) >= 0)
        {
            parameterType = TParameterType::ptBoundaryParameter;
            originalParameterValue = 0;
            mModel->getBoundarySpeciesConcentrations(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getGlobalParameterIndex(parameterName)) >= 0)
        {
            parameterType = TParameterType::ptGlobalParameter;
            originalParameterValue = 0;
            mModel->getGlobalParameterValues(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getConservedSumIndex(parameterName)) >= 0)
        {
            parameterType = TParameterType::ptConservationParameter;
            originalParameterValue = 0;
            mModel->getConservedSums(1, &parameterIndex, &originalParameterValue);
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
        mModel->evalReactionRates();
        double fi = 0;
        mModel->getReactionRates(1, &reactionIndex, &fi);

        setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
        mModel->evalReactionRates();
        double fi2 = 0;
        mModel->getReactionRates(1, &reactionIndex, &fi2);

        setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
        mModel->evalReactionRates();
        double fd = 0;
        mModel->getReactionRates(1, &reactionIndex, &fd);

        setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
        mModel->evalReactionRates();
        double fd2 = 0;
        mModel->getReactionRates(1, &reactionIndex, &fd2);

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
    mModel->evalModel(mModel->getTime(), 0);
}

void RoadRunner::setTimeCourseSelectionList(const string& list)
{
    StringList aList(list,", ");
    setTimeCourseSelectionList(aList);
}

// Help("Set the columns to be returned by simulate() or simulateEx(), valid symbol names include" +
//              " time, species names, , volume, reaction rates and rates of change (speciesName')")
void RoadRunner::setTimeCourseSelectionList(const vector<string>& _selList)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mSelectionList.clear();
    vector<string> newSelectionList(_selList);
    vector<string> fs = getFloatingSpeciesIds();
    vector<string> bs = getBoundarySpeciesIds();
    vector<string> rs = getReactionIds();
    vector<string> vol= getCompartmentIds();
    vector<string> gp = getGlobalParameterIds();
//    StringList sr = mModelGenerator->ModifiableSpeciesReferenceList;

    for (int i = 0; i < _selList.size(); i++)
    {
        if (toUpper(newSelectionList[i]) == toUpper("time"))
        {
            mSelectionList.push_back(SelectionRecord(0, SelectionRecord::clTime));
        }

        // Check for species
        for (int j = 0; j < fs.size(); j++)
        {
            if (newSelectionList[i] == fs[j])
            {
                   mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clFloatingSpecies));
                break;
            }

            if (newSelectionList[i] == "[" + fs[j] + "]")
            {
                   mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clFloatingAmount));
                break;
            }

            // Check for species rate of change
            if (newSelectionList[i] == fs[j] + "'")
            {
                mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clRateOfChange));
                break;
            }
        }

        // Check fgr boundary species
        for (int j = 0; j < bs.size(); j++)
        {
            if (newSelectionList[i] == bs[j])
            {
                mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clBoundarySpecies));
                break;
            }
            if (newSelectionList[i] == "[" + bs[j] + "]")
            {
                mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clBoundaryAmount));
                break;
            }
        }

        for (int j = 0; j < rs.size(); j++)
        {
            // Check for reaction rate
            if (newSelectionList[i] == rs[j])
            {
                mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clFlux));
                break;
            }
        }

        for (int j = 0; j < vol.size(); j++)
        {
            // Check for volume
            if (newSelectionList[i] == vol[j])
            {
                mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clVolume));
                break;
            }

            if (newSelectionList[i] == "[" + vol[j] + "]")
            {
                mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clVolume));
                break;
            }
        }

        for (int j = 0; j < gp.size(); j++)
        {
            if (newSelectionList[i] == gp[j])
            {
                mSelectionList.push_back(SelectionRecord(j, SelectionRecord::clParameter));
                break;
            }
        }

        //((string)newSelectionList[i]).StartsWith("eigen_")
        string tmp = newSelectionList[i];
        if (startsWith(tmp, "eigen_"))
        {
            string species = tmp.substr(tmp.find_last_of("eigen_") + 1);
            mSelectionList.push_back(SelectionRecord(i, SelectionRecord::clEigenValue, species));
//            mSelectionList[i].selectionType = SelectionRecord::clEigenValue;
//            mSelectionList[i].p1 = species;
            int aIndex = indexOf(fs, species);
            mSelectionList[i].index = aIndex;
            //mModelGenerator->floatingSpeciesConcentrationList.find(species, mSelectionList[i].index);
        }

//        if (((string)newSelectionList[i]).StartsWith("EE:"))
//        {
//            string parameters = ((string)newSelectionList[i]).Substring(3);
//            var p1 = parameters.Substring(0, parameters.IndexOf(","));
//            var p2 = parameters.Substring(parameters.IndexOf(",") + 1);
//            mSelectionList[i].selectionType = SelectionRecord::clElasticity;
//            mSelectionList[i].p1 = p1;
//            mSelectionList[i].p2 = p2;
//        }
//
//        if (((string)newSelectionList[i]).StartsWith("uEE:"))
//        {
//            string parameters = ((string)newSelectionList[i]).Substring(4);
//            var p1 = parameters.Substring(0, parameters.IndexOf(","));
//            var p2 = parameters.Substring(parameters.IndexOf(",") + 1);
//            mSelectionList[i].selectionType = SelectionRecord::clUnscaledElasticity;
//            mSelectionList[i].p1 = p1;
//            mSelectionList[i].p2 = p2;
//        }
//        if (((string)newSelectionList[i]).StartsWith("eigen_"))
//        {
//            var species = ((string)newSelectionList[i]).Substring("eigen_".Length);
//            mSelectionList[i].selectionType = SelectionRecord::clEigenValue;
//            mSelectionList[i].p1 = species;
//            mModelGenerator->floatingSpeciesConcentrationList.find(species, out mSelectionList[i].index);
//        }
//
//        int index;
//        if (sr.find((string)newSelectionList[i], out index))
//        {
//            mSelectionList[i].selectionType = SelectionRecord::clStoichiometry;
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
//DoubleMatrix RoadRunner::getEigenvaluesFromMatrix (DoubleMatrix m)
//{
//    try
//    {
//        vector<Complex> vals = ls::getEigenValues(m);
//
//        DoubleMatrix result(vals.size(), 2);
//
//        for (int i = 0; i < vals.size(); i++)
//        {
//            result[i][0] = real(vals[i]);
//            result[i][1] = imag(vals[i]);
//        }
//        return result;
//    }
//    catch (const Exception& e)
//    {
//        throw CoreException("Unexpected error from getEigenvalues()", e.Message());
//    }
//}

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

double RoadRunner::getVariableValue(const TVariableType::TVariableType variableType,
        const int variableIndex)
{
    switch (variableType)
    {
    case TVariableType::vtFlux:
    {
        double result = 0;
        mModel->getReactionRates(1, &variableIndex, &result);
    }
    break;

    case TVariableType::vtSpecies:
    {
        double result = 0;
        mModel->getFloatingSpeciesConcentrations(1, &variableIndex, &result);
        return result;
    }
    break;


    default:
        throw CoreException("Unrecognised variable in getVariableValue");
        break;
    }
    return 0;
}

//  Help("Returns the Symbols of all Flux Control Coefficients.")
NewArrayList RoadRunner::getFluxControlCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    vector<string> oReactions       = getReactionIds();
    vector<string> oParameters      = getGlobalParameterIds();
    vector<string> oBoundary        = getBoundarySpeciesIds();
    vector<string> oConservation    = getConservedSumIds();

    for(int i = 0; i < oReactions.size(); i++)
    {
        string s = oReactions[i];

        NewArrayList oCCReaction;
        StringList oInner;
        oCCReaction.Add(s);

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

    vector<string> oReactions = getReactionIds();
    vector<string> oParameters = getGlobalParameterIds();
    vector<string> oBoundary = getBoundarySpeciesIds();
    vector<string> oConservation = getConservedSumIds();

    for(int i = 0; i < oReactions.size(); i++)
    {
        string s = oReactions[i];

        NewArrayList oCCReaction;
        vector<string> oInner;
        oCCReaction.Add(s);

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

    vector<string> oFloating        = getFloatingSpeciesIds();
    vector<string> oParameters      = getGlobalParameterIds();
    vector<string> oBoundary        = getBoundarySpeciesIds();
    vector<string> oConservation    = getConservedSumIds();

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
NewArrayList RoadRunner::getUnscaledConcentrationControlCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    vector<string> oFloating        = getFloatingSpeciesIds();
    vector<string> oParameters      = getGlobalParameterIds();
    vector<string> oBoundary        = getBoundarySpeciesIds();
    vector<string> oConservation    = getConservedSumIds();

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
NewArrayList RoadRunner::getElasticityCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    vector<string> reactionNames        = getReactionIds();
    vector<string> floatingSpeciesNames = getFloatingSpeciesIds();
    vector<string> boundarySpeciesNames = getBoundarySpeciesIds();
    vector<string> conservationNames    = getConservedSumIds();
    vector<string> globalParameterNames = getGlobalParameterIds();

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
NewArrayList RoadRunner::getUnscaledElasticityCoefficientIds()
{
    NewArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    vector<string> oReactions( getReactionIds() );
    vector<string> oFloating = getFloatingSpeciesIds();
    vector<string> oBoundary = getBoundarySpeciesIds();
    vector<string> oGlobalParameters = getGlobalParameterIds();
    vector<string> oConservation = getConservedSumIds();

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

// Help("Returns the Symbols of all Floating Species Eigenvalues.")
vector<string> RoadRunner::getEigenvalueIds()
{
    if (!mModel)
    {
        return vector<string>();
    }

    vector<string> result;
    vector<string> floating = getFloatingSpeciesIds();

    for(int i = 0; i < floating.size(); i++)
    {
        result.push_back("eigen_" + floating[i]);
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
    oResult.Add("Floating Species (amount)",                        getFloatingSpeciesAmountIds() );
    oResult.Add("Boundary Species (amount)",                        getBoundarySpeciesAmountIds() );
    oResult.Add("Global Parameters",                                getParameterIds() );
    oResult.Add("Volumes",                                          getCompartmentIds() );
    oResult.Add("Fluxes",                                           getReactionIds() );
    oResult.Add("Flux Control Coefficients",                        getFluxControlCoefficientIds() );
    oResult.Add("Concentration Control Coefficients",               getConcentrationControlCoefficientIds() );
    oResult.Add("Unscaled Concentration Control Coefficients",      getUnscaledConcentrationControlCoefficientIds());
    oResult.Add("Elasticity Coefficients",                          getElasticityCoefficientIds() );
    oResult.Add("Unscaled Elasticity Coefficients",                 getUnscaledElasticityCoefficientIds() );
    oResult.Add("Eigenvalues",                                      getEigenvalueIds() );

    return oResult;
}

int RoadRunner::createDefaultSteadyStateSelectionList()
{
    mSteadyStateSelection.clear();
    // default should be species only ...
    vector<string> floatingSpecies = getFloatingSpeciesIds();
    mSteadyStateSelection.resize(floatingSpecies.size());
    for (int i = 0; i < floatingSpecies.size(); i++)
    {
        SelectionRecord aRec;
        aRec.selectionType = SelectionRecord::clFloatingSpecies;
        aRec.p1 = floatingSpecies[i];
        aRec.index = i;
        mSteadyStateSelection[i] = aRec;
    }
    return mSteadyStateSelection.size();
}

// Help("Returns the selection list as returned by computeSteadyStateValues().")
vector<string> RoadRunner::getSteadyStateSelectionList()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (mSteadyStateSelection.size() == 0)
    {
          createDefaultSteadyStateSelectionList();
    }

    vector<string> oFloating     = getFloatingSpeciesIds();
    vector<string> oBoundary     = getBoundarySpeciesIds();
    vector<string> oFluxes       = getReactionIds();
    vector<string> oVolumes      = getCompartmentIds();
    vector<string> oRates        = getRateOfChangeIds();
    vector<string> oParameters   = getParameterIds();

    vector<string> result;
    for(int i = 0; i < mSteadyStateSelection.size(); i++)
    {
        SelectionRecord record = mSteadyStateSelection[i];
        switch (record.selectionType)
        {
            case SelectionRecord::clTime:
                result.push_back("time");
            break;
            case SelectionRecord::clBoundaryAmount:
                result.push_back(format("[{0}]", oBoundary[record.index]));
            break;
            case SelectionRecord::clBoundarySpecies:
                result.push_back(oBoundary[record.index]);
            break;
            case SelectionRecord::clFloatingAmount:
                result.push_back("[" + (string)oFloating[record.index] + "]");
            break;
            case SelectionRecord::clFloatingSpecies:
                result.push_back(oFloating[record.index]);
            break;
            case SelectionRecord::clVolume:
                result.push_back(oVolumes[record.index]);
            break;
            case SelectionRecord::clFlux:
                result.push_back(oFluxes[record.index]);
            break;
            case SelectionRecord::clRateOfChange:
                result.push_back(oRates[record.index]);
            break;
            case SelectionRecord::clParameter:
                result.push_back(oParameters[record.index]);
            break;
            case SelectionRecord::clEigenValue:
                result.push_back("eigen_" + record.p1);
            break;
            case SelectionRecord::clElasticity:
                result.push_back("EE:" + record.p1 + "," + record.p2);
            break;
            case SelectionRecord::clUnscaledElasticity:
                result.push_back("uEE:" + record.p1 + "," + record.p2);
            break;
            case SelectionRecord::clUnknown:
                result.push_back(record.p1);
            break;
            default:
                // empty list
            break;
        }
    }
    return result ;
}

vector<SelectionRecord> RoadRunner::getSteadyStateSelection(const vector<string>& newSelectionList)
{
    vector<SelectionRecord> steadyStateSelection;
    steadyStateSelection.resize(newSelectionList.size());
    vector<string> fs = getFloatingSpeciesIds();
    vector<string> bs = getBoundarySpeciesIds();
    vector<string> rs = getReactionIds();
    vector<string> vol = getCompartmentIds();
    vector<string> gp = getGlobalParameterIds();

    for (int i = 0; i < newSelectionList.size(); i++)
    {
        bool set = false;
        // Check for species
        for (int j = 0; j < fs.size(); j++)
        {
            if ((string)newSelectionList[i] == (string)fs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionRecord::clFloatingSpecies;
                set = true;
                break;
            }

            if ((string)newSelectionList[i] == "[" + (string)fs[j] + "]")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionRecord::clFloatingAmount;
                set = true;
                break;
            }

            // Check for species rate of change
            if ((string)newSelectionList[i] == (string)fs[j] + "'")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionRecord::clRateOfChange;
                set = true;
                break;
            }
        }

        if (set)
        {
            continue;
        }

        // Check fgr boundary species
        for (int j = 0; j < bs.size(); j++)
        {
            if ((string)newSelectionList[i] == (string)bs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionRecord::clBoundarySpecies;
                set = true;
                break;
            }
            if ((string)newSelectionList[i] == "[" + (string)bs[j] + "]")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionRecord::clBoundaryAmount;
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
            steadyStateSelection[i].selectionType = SelectionRecord::clTime;
            set = true;
        }

        for (int j = 0; j < rs.size(); j++)
        {
            // Check for reaction rate
            if ((string)newSelectionList[i] == (string)rs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionRecord::clFlux;
                set = true;
                break;
            }
        }

        for (int j = 0; j < vol.size(); j++)
        {
            // Check for volume
            if ((string)newSelectionList[i] == (string)vol[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionRecord::clVolume;
                set = true;
                break;
            }
        }

        for (int j = 0; j < gp.size(); j++)
        {
            // Check for volume
            if ((string)newSelectionList[i] == (string)gp[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = SelectionRecord::clParameter;
                set = true;
                break;
            }
        }

        if (set)
        {
            continue;
        }

        // it is another symbol
        steadyStateSelection[i].selectionType = SelectionRecord::clUnknown;
        steadyStateSelection[i].p1 = (string)newSelectionList[i];
    }
    return steadyStateSelection;
}

// Help("sets the selection list as returned by computeSteadyStateValues().")
void RoadRunner::setSteadyStateSelectionList(const vector<string>& newSelectionList)
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

    if (record.selectionType == SelectionRecord::clUnknown)
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
            if ((nIndex = mModel->getFloatingSpeciesIndex(sSpecies)) >= 0)
            {
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
    return mModel ? mModel->getModelName() : "";
}

// TODO Looks like major problems here, this
// apears to modify the AFTER a model has been created from it.
string RoadRunner::writeSBML()
{
    NOMSupport NOM;

    NOM.loadSBML(NOMSupport::getParamPromotedSBML(mCurrentSBML));

    vector<string> array = getFloatingSpeciesIds();
    for (int i = 0; i < array.size(); i++)
    {
        double value = 0;
        mModel->getFloatingSpeciesAmounts(1, &i, &value);
        NOM.setValue((string)array[i], value);
    }

    array = getBoundarySpeciesIds();
    for (int i = 0; i < array.size(); i++)
    {
        double value = 0;
        mModel->getBoundarySpeciesConcentrations(1, &i, &value);
        NOM.setValue((string)array[i], value);
    }

    array = getCompartmentIds();
    for (int i = 0; i < array.size(); i++)
    {
        double value = 0;
        mModel->getCompartmentVolumes(1, &i, &value);
        NOM.setValue((string)array[i], value);
    }

    array = getGlobalParameterIds();
    for (int i = 0; i < mModel->getNumGlobalParameters(); i++)
    {
        double value = 0;
        mModel->getGlobalParameterValues(1, &i, &value);
        NOM.setValue((string)array[i], value);
    }

    return NOM.getSBML();
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
        mModel->evalReactionRates();
        double result = 0;
        mModel->getReactionRates(1, &index, &result);
        return result;
    }
    else
    {
        throw CoreException(format("Index in getReactionRate out of range: [{0}]", index));
    }
}


double RoadRunner::getRateOfChange(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumFloatingSpecies()))
    {
        mModel->computeAllRatesOfChange();
        double value = 0;
        mModel->getFloatingSpeciesAmountRates(1, &index, &value);
        return value;
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

    mModel->evalReactionRates();
    vector<double> result(mModel->getNumReactions());
    mModel->getReactionRates(result.size(), 0, &result[0]);
    return result;
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


void RoadRunner::setCompartmentByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
         throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumCompartments()))
    {
        mModel->setCompartmentVolumes(1, &index, &value);
    }
    else
    {
        throw CoreException(format("Index in getCompartmentByIndex out of range: [{0}]", index));
    }
}

double RoadRunner::getCompartmentByIndex(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumCompartments()))
    {
        double result = 0;
        mModel->getCompartmentVolumes(1, &index, &result);
        return result;
    }

    throw CoreException(format("Index in getCompartmentByIndex out of range: [{0}]", index));
}

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
        mModel->setBoundarySpeciesConcentrations(1, &index, &value);
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
        double result = 0;
        mModel->getBoundarySpeciesConcentrations(1, &index, &result);
        return result;
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
    vector<double> result(mModel->getNumBoundarySpecies(), 0);
    mModel->getBoundarySpeciesConcentrations(result.size(), 0, &result[0]);
    return result;
}



// Help("Gets the list of boundary species names")
vector<string> RoadRunner::getBoundarySpeciesIds()
{
    return createModelStringList(mModel, &ExecutableModel::getNumBoundarySpecies,
            &ExecutableModel::getBoundarySpeciesId);
}

vector<string> RoadRunner::getConservedSumIds()
{
    return createModelStringList(mModel, &ExecutableModel::getNumConservedSums,
            &ExecutableModel::getConservedSumId);
}


// Help("Gets the list of boundary species amount names")
vector<string> RoadRunner::getBoundarySpeciesAmountIds()
{
    vector<string> result;// = new ArrayList();
    vector<string> list = getBoundarySpeciesIds();
//    foreach (string s in getBoundarySpeciesIds()) oResult.add("[" + s + "]");
    for(int item = 0; item < list.size(); item++)// (object item in floatingSpeciesNames)
    {
        result.push_back(format("[{0}]", list[item]));
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
    return mModel->getNumFloatingSpecies();
}

double RoadRunner::getFloatingSpeciesInitialConcentrationByIndex(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumFloatingSpecies()))
    {
        double result = 0;
        mModel->getFloatingSpeciesInitConcentrations(1, &index, &result);
        return result;
    }
    else
    {
        throw CoreException(format("Index in setFloatingSpeciesInitialConcentrationByIndex out of range: [{0}]", index));
    }
}

// Help("Sets the value of a floating species by its index")
void RoadRunner::setFloatingSpeciesInitialConcentrationByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumFloatingSpecies()))
    {
        mModel->setFloatingSpeciesInitConcentrations(1, &index, &value);
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

    if ((index >= 0) && (index < mModel->getNumFloatingSpecies()))
    {
        mModel->setFloatingSpeciesConcentrations(1, &index, &value); // This updates the amount vector aswell
        if (!mModel->getConservedSumChanged())
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

    if ((index >= 0) && (index < mModel->getNumFloatingSpecies()))
    {
        double result = 0;
        return mModel->getFloatingSpeciesConcentrations(1, &index, &result);
        return result;
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


    vector<double> result(mModel->getNumFloatingSpecies(), 0);

    mModel->convertToConcentrations();
    mModel->getFloatingSpeciesConcentrations(result.size(), 0, &result[0]);
    return result;
}

// Help("returns an array of floating species initial conditions")
vector<double> RoadRunner::getFloatingSpeciesInitialConcentrations()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    vector<double> initYs(mModel->getNumFloatingSpecies());
    mModel->getFloatingSpeciesInitConcentrations(initYs.size(), 0, &initYs[0]);
    return initYs;
}

// Help("Sets the initial conditions for all floating species in the model")
void RoadRunner::setFloatingSpeciesInitialConcentrations(const vector<double>& values)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->setFloatingSpeciesConcentrations(values.size(), 0, &values[0]);
    mModel->setFloatingSpeciesInitConcentrations(values.size(), 0, &values[0]);

    reset();
}

// Help("Set the concentrations for all floating species in the model")
void RoadRunner::setFloatingSpeciesConcentrations(const vector<double>& values)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->setFloatingSpeciesConcentrations(values.size(), 0, &values[0]);
    mModel->convertToAmounts();

    if (!mModel->getConservedSumChanged()) mModel->computeConservedTotals();
}

// Help("Set the concentrations for all floating species in the model")
void RoadRunner::setBoundarySpeciesConcentrations(const vector<double>& values)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->setBoundarySpeciesConcentrations(values.size(), 0, &values[0]);

    // TODO: No idea whatsoever why boundary species concentrations was also
    // setting the floating species concentrations???
    //
    //    for (int i = 0; i < values.size(); i++)
    //    {
    //        mModel->setConcentration(i, values[i]);
    //        if ((mModel->getModelData().numBoundarySpecies) > i)
    //        {
    //            mModel->getModelData().boundarySpeciesConcentrations[i] = values[i];
    //        }
    //    }
    mModel->convertToAmounts();
}

// This is a Level 1 method !
// Help("Returns a list of floating species names")
vector<string> RoadRunner::getFloatingSpeciesIds()
{
    return createModelStringList(mModel, &ExecutableModel::getNumFloatingSpecies,
            &ExecutableModel::getFloatingSpeciesId);
}

// Help("Returns a list of floating species initial condition names")
vector<string> RoadRunner::getFloatingSpeciesInitialConditionIds()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    vector<string> floatingSpeciesNames = getFloatingSpeciesIds();
    vector<string> result;// = new ArrayList();
    for(int item = 0; item < floatingSpeciesNames.size(); item++)// (object item in floatingSpeciesNames)
    {
        result.push_back(format("init({0})", floatingSpeciesNames[item]));
    }
    return result;
}

// Help("Returns the list of floating species amount names")
vector<string> RoadRunner::getFloatingSpeciesAmountIds()
{
    vector<string> oResult;
    vector<string> list = getFloatingSpeciesIds();

    for(int i = 0; i < list.size(); i++)
    {
        oResult.push_back(format("[{0}]", list[i]));
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
    return getGlobalParameterIds().size();
}

// Help("Sets the value of a global parameter by its index")
void RoadRunner::setGlobalParameterByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->setGlobalParameterValues(1, &index, &value);

    if ((mModel->getNumGlobalParameters()) &&
            (index < mModel->getNumGlobalParameters() +
                    mModel->getNumDependentSpecies()))
    {
        mModel->setConservedSumChanged(true);
    }
}

// Help("Returns the value of a global parameter by its index")
double RoadRunner::getGlobalParameterByIndex(const int& index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < (mModel->getNumGlobalParameters() + mModel->getNumDependentSpecies())))
    {
        int arraySize = mModel->getNumGlobalParameters() + mModel->getNumDependentSpecies();
        double* data = new double[arraySize];

        mModel->getGlobalParameterValues(mModel->getNumGlobalParameters(), 0, data);

        mModel->getConservedSums(mModel->getNumDependentSpecies(), 0, data + mModel->getNumGlobalParameters());

        double result = data[index];
        delete[] data;
        return result;
    }

    throw CoreException(format("Index in getNumGlobalParameters out of range: [{0}]", index));
}

// Help("Get the values for all global parameters in the model")
vector<double> RoadRunner::getGlobalParameterValues()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (mModel->getNumDependentSpecies() > 0)
    {
        vector<double> result(mModel->getNumGlobalParameters() +
                mModel->getNumDependentSpecies());

        mModel->getGlobalParameterValues(
                mModel->getNumGlobalParameters(), 0, &result[0]);

        mModel->getConservedSums(
                mModel->getNumDependentSpecies(), 0,
                &result[mModel->getNumGlobalParameters()]);

        return result;
    }

    vector<double> result(mModel->getNumGlobalParameters());
    mModel->getGlobalParameterValues(result.size(), 0, &result[0]);
    return result;
}

// Help("Gets the list of parameter names")
vector<string> RoadRunner::getGlobalParameterIds()
{
    return createModelStringList(mModel, &ExecutableModel::getNumGlobalParameters,
            &ExecutableModel::getGlobalParameterId);
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

        TParameterType::TParameterType parameterType;
        TVariableType::TVariableType variableType;
        double originalParameterValue;
        int variableIndex;
        int parameterIndex;

        mModel->convertToConcentrations();
        mModel->evalReactionRates();

        // Check the variable name
        if ((variableIndex = mModel->getReactionIndex(variableName)) >= 0)
        {
            variableType = TVariableType::vtFlux;
        }
        else if ((variableIndex = mModel->getFloatingSpeciesIndex(variableName)) >= 0)
        {
            variableType = TVariableType::vtSpecies;
        }
        else
        {
            throw CoreException("Unable to locate variable: [" + variableName + "]");
        }

        // Check for the parameter name
        if ((parameterIndex = mModel->getGlobalParameterIndex(parameterName)) >= 0)
        {
            parameterType = TParameterType::ptGlobalParameter;
            originalParameterValue = 0;
            mModel->getGlobalParameterValues(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getBoundarySpeciesIndex(parameterName)) >= 0)
        {
            parameterType = TParameterType::ptBoundaryParameter;
            originalParameterValue = 0;
            mModel->getBoundarySpeciesConcentrations(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getConservedSumIndex(parameterName)) >= 0)
        {
            parameterType = TParameterType::ptConservationParameter;
            originalParameterValue = 0;
            mModel->getConservedSums(1, &parameterIndex, &originalParameterValue);
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
            mModel->evalReactionRates();
            double fi = getVariableValue(variableType, variableIndex);

            setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
            steadyState();
            mModel->evalReactionRates();
            double fi2 = getVariableValue(variableType, variableIndex);

            setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
            steadyState();
            mModel->evalReactionRates();
            double fd = getVariableValue(variableType, variableIndex);

            setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
            steadyState();
            mModel->evalReactionRates();
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
    TVariableType::TVariableType variableType;
    TParameterType::TParameterType parameterType;
    int variableIndex;
    int parameterIndex;

    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    // Check the variable name
    if ((variableIndex = mModel->getReactionIndex(variableName)) >= 0)
    {
        variableType = TVariableType::vtFlux;
    }
    else if ((variableIndex = mModel->getFloatingSpeciesIndex(variableName)) >= 0)
    {
        variableType = TVariableType::vtSpecies;
    }
    else
    {
        throw CoreException("Unable to locate variable: [" + variableName + "]");
    }

    // Check for the parameter name
    if ((parameterIndex = mModel->getGlobalParameterIndex(parameterName)) >= 0)
    {
        parameterType = TParameterType::ptGlobalParameter;
    }
    else if ((parameterIndex = mModel->getBoundarySpeciesIndex(parameterName)) >= 0)
    {
        parameterType = TParameterType::ptBoundaryParameter;
    }
    else if ((parameterIndex = mModel->getConservedSumIndex(parameterName)) >= 0)
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
    double value;
    double originalParameterValue = 0;
    mModel->getFloatingSpeciesConcentrations(1, &speciesIndex, &originalParameterValue);

    double hstep = mDiffStepSize*originalParameterValue;
    if (fabs(hstep) < 1E-12)
    {
        hstep = mDiffStepSize;
    }

    mModel->convertToConcentrations();

    value = originalParameterValue + hstep;
    mModel->setFloatingSpeciesConcentrations(1, &speciesIndex, &value);

    try
    {
        mModel->evalReactionRates();
        double fi = 0;
        mModel->getReactionRates(1, &reactionId, &fi);

        value = originalParameterValue + 2*hstep;
        mModel->setFloatingSpeciesConcentrations(1, &speciesIndex, &value);
        mModel->evalReactionRates();
        double fi2 = 0;
        mModel->getReactionRates(1, &reactionId, &fi2);

        value = originalParameterValue - hstep;
        mModel->setFloatingSpeciesConcentrations(1, &speciesIndex, &value);
        mModel->evalReactionRates();
        double fd = 0;
        mModel->getReactionRates(1, &reactionId, &fd);

        value = originalParameterValue - 2*hstep;
        mModel->setFloatingSpeciesConcentrations(1, &speciesIndex, &value);
        mModel->evalReactionRates();
        double fd2 = 0;
        mModel->getReactionRates(1, &reactionId, &fd2);

        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
        // The following separated lines avoid small amounts of roundoff error
        double f1 = fd2 + 8*fi;
        double f2 = -(8*fd + fi2);

        // What ever happens, make sure we restore the species level
        mModel->setFloatingSpeciesConcentrations(1, &speciesIndex, &originalParameterValue);
        return 1/(12*hstep)*(f1 + f2);
    }
    catch(const Exception& e)
    {
        Log(lError)<<"Something went wrong in "<<__FUNCTION__;
        Log(lError)<<"Exception "<<e.what()<< " thrown";
                // What ever happens, make sure we restore the species level
        mModel->setFloatingSpeciesConcentrations(1, &speciesIndex, &originalParameterValue);
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

        DoubleMatrix uElastMatrix(mModel->getNumReactions(), mModel->getNumFloatingSpecies());
        mModel->convertToConcentrations();

        // Compute reaction velocities at the current operating point
        mModel->evalReactionRates();

        for (int i = 0; i < mModel->getNumReactions(); i++)
        {
            for (int j = 0; j < mModel->getNumFloatingSpecies(); j++)
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
        mModel->evalReactionRates();
        vector<double> rates(mModel->getNumReactions());
        mModel->getReactionRates(rates.size(), 0, &rates[0]);

        for (int i = 0; i < uelast.RSize(); i++)
        {
            // Rows are rates
            if (mModel->getNumReactions() == 0 || rates[i] == 0)
            {
                string name;
                if(mModelGenerator && mModel->getNumReactions())
                {
                    name = mModel->getReactionId(i);
                }
                else
                {
                    name = "none";
                }

                throw CoreException("Unable to compute elasticity, reaction rate [" + name + "] set to zero");
            }

            for (int j = 0; j < uelast.CSize(); j++) // Columns are species
            {
                double concentration = 0;
                mModel->getFloatingSpeciesConcentrations(1, &j, &concentration);

                result[i][j] = uelast[i][j]*concentration/rates[i];
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
        mModel->evalReactionRates();

        if ((speciesIndex = mModel->getFloatingSpeciesIndex(speciesName)) < 0)
        {
            throw CoreException("Internal Error: unable to locate species name while computing unscaled elasticity");
        }
        if ((reactionIndex = mModel->getReactionIndex(reactionName)) < 0)
        {
            throw CoreException("Internal Error: unable to locate reaction name while computing unscaled elasticity");
        }

        double concentration = 0;
        mModel->getFloatingSpeciesConcentrations(1, &speciesIndex, &concentration);

        double reactionRate = 0;
        mModel->getReactionRates(1, &reactionIndex, &reactionRate);
        return getUnscaledSpeciesElasticity(reactionIndex, speciesIndex) *
            concentration / reactionRate;

    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from scaledElasticityMatrix()", e.Message());
    }
}


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
                mModel->evalReactionRates();
                for (int i = 0; i < ucc.RSize(); i++)
                {
                    for (int j = 0; j < ucc.CSize(); j++)
                    {
                        double conc = 0;
                        mModel->getFloatingSpeciesConcentrations(1, &i, &conc);
                        if(conc != 0.0)
                        {
                            double rate = 0;
                            mModel->getReactionRates(1, &j, &rate);
                            ucc[i][j] = ucc[i][j] * rate / conc;
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
            mModel->evalReactionRates();
            for (int i = 0; i < ufcc.RSize(); i++)
            {
                for (int j = 0; j < ufcc.CSize(); j++)
                {
                    double irate = 0;
                    mModel->getReactionRates(1, &i, &irate);
                    if(irate !=0)
                    {
                        double jrate = 0;
                        mModel->getReactionRates(1, &j, &jrate);
                        ufcc[i][j] = ufcc[i][j] * jrate / irate;
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

    mModel->setFloatingSpeciesConcentrations(ic.size(), 0, &ic[0]);
    mModel->setFloatingSpeciesInitConcentrations(ic.size(), 0, &ic[0]);
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
    mModel->evalReactionRates();

    vector<double> rates(mModel->getNumReactions());
    mModel->getReactionRates(rates.size(), 0, &rates[0]);
    return rates;
}

// Help("Returns the current vector of rates of change")
vector<double> RoadRunner::getRatesOfChange()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    mModel->computeAllRatesOfChange();
    vector<double> result(mModel->getNumFloatingSpecies());
    mModel->getFloatingSpeciesAmountRates(result.size(), 0, &result[0]);
    return result;
}

// Help("Returns a list of reaction names")
vector<string> RoadRunner::getReactionIds()
{
    return createModelStringList(mModel, &ExecutableModel::getNumReactions,
            &ExecutableModel::getReactionId);
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

vector<string> RoadRunner::getListOfCapabilities()
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

vector<string> RoadRunner::getListOfParameters(const string& cap)
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
    return vector<string>();
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
    if ((nIndex = mModel->getGlobalParameterIndex(sId)) >= 0)
    {
        mModel->setGlobalParameterValues(1, &nIndex, &dValue);
        return true;
    }

    if ((nIndex = mModel->getBoundarySpeciesIndex(sId)) >= 0)
    {
        mModel->setBoundarySpeciesConcentrations(1, &nIndex, &dValue);
        return true;
    }

    if ((nIndex = mModel->getCompartmentIndex(sId)) >= 0)
    {
        mModel->setCompartmentVolumes(1, &nIndex, &dValue);
        return true;
    }

    if ((nIndex = mModel->getFloatingSpeciesIndex(sId)) >= 0)
    {
        mModel->setFloatingSpeciesConcentrations(1, &nIndex, &dValue);
        mModel->convertToAmounts();
        if (!mModel->getConservedSumChanged())
        {
            mModel->computeConservedTotals();
        }
        return true;
    }

    if ((nIndex = mModel->getConservedSumIndex(sId)) >= 0)
    {
        mModel->setConservedSums(1, &nIndex, &dValue);
        mModel->updateDependentSpeciesValues();
        mModel->setConservedSumChanged(true);
        return true;
    }

    StringList initialConditions;
    initialConditions = getFloatingSpeciesInitialConditionIds();

    if (initialConditions.Contains(sId))
    {
        int index = initialConditions.indexOf(sId);
        mModel->setFloatingSpeciesInitConcentrations(1, &index, &dValue);
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
    if (( nIndex = mModel->getGlobalParameterIndex(sId)) >= 0)
    {
        double result = 0;
        mModel->getGlobalParameterValues(1, &nIndex, &result);
        return result;
    }
    if ((nIndex = mModel->getBoundarySpeciesIndex(sId)) >= 0)
    {
        double result = 0;
        mModel->getBoundarySpeciesConcentrations(1, &nIndex, &result);
        return result;
    }

    if ((nIndex = mModel->getFloatingSpeciesIndex(sId)) >= 0)
    {
        double result = 0;
        mModel->getFloatingSpeciesConcentrations(1, &nIndex, &result);
        return result;
    }

    if ((nIndex = mModel->getFloatingSpeciesIndex(sId.substr(0, sId.size() - 1))) >= 0)
    {
        mModel->computeAllRatesOfChange();

        //fs[j] + "'" will be interpreted as rate of change
        double result = 0;
        mModel->getFloatingSpeciesAmountRates(1, &nIndex, &result);
        return result;
    }

    if ((nIndex = mModel->getCompartmentIndex(sId)) >= 0)
    {
        double result = 0;
        mModel->getCompartmentVolumes(1, &nIndex, &result);
        return result;
    }
    if ((nIndex = mModel->getReactionIndex(sId)) >= 0)
    {
        double result = 0;
        mModel->getReactionRates(1, &nIndex, &result);
        return result;
    }

    if ((nIndex = mModel->getConservedSumIndex(sId)) >= 0)
    {
        double result = 0;
        return mModel->getConservedSums(1, &nIndex, &result);
        return result;
    }

    StringList initialConditions = getFloatingSpeciesInitialConditionIds();
    if (initialConditions.Contains(sId))
    {
        int index = initialConditions.indexOf(sId);
        double result = 0;
        mModel->getFloatingSpeciesInitConcentrations(1, &index, &result);
        return result;
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
        int index = mModel->getFloatingSpeciesIndex(species);


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
    oResult.Add("Volumes",                          getCompartmentIds() );
    oResult.Add("Elasticity Coefficients",          getElasticityCoefficientIds() );
    oResult.Add("Unscaled Elasticity Coefficients", getUnscaledElasticityCoefficientIds() );
    oResult.Add("Eigenvalues",                      getEigenvalueIds() );
    return oResult;
}

string RoadRunner::getVersion()
{
    return string(RR_VERSION) + string(", compiled with ") + string(RR_COMPILER);
}

string RoadRunner::getCopyright()
{
    return "(c) 2009-2013 HM. Sauro, FT. Bergmann, Totte Karlsson and Andy Somogyi, BSD Licence";
}

string RoadRunner::getURL()
{
    return "http://www.sys-bio.org";
}

string RoadRunner::getlibSBMLVersion()
{
    return libsbml::getLibSBMLDottedVersion();
}


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
        result[i] = getNthSelectedOutput(i, mModel->getTime());
    }
    return result;
}



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



#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop

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
#include "rrConstants.h"
#include "rrVersionInfo.h"
#include "rrCVODEInterface.h"
#include "rrNLEQInterface.h"
#include "c/rrNOMSupport.h"

#include <iostream>
#include <assert.h>
#include <rr-libstruct/lsLibStructural.h>
#include <Poco/File.h>
#include <Poco/Mutex.h>
#include <sbml/common/libsbml-version.h>

namespace rr
{
using namespace std;
using namespace ls;
using Poco::Mutex;

static Mutex roadRunnerMutex;


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
        return vector<string>(0);
    }

    const int num = (model->*numFunc)();
    vector<string> strings(num);

    for(int i = 0; i < num; i++)
    {
        strings[i] = (model->*nameFunc)(i);
    }

    return strings;
}

/**
 * convert the names from the sbml settings file into the
 * RoadRunner selection syntax.
 */
static std::vector<std::string> createSelectionList(const SimulateOptions& o);


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

RoadRunner::RoadRunner(const string& _compiler, const string& _tempDir,
        const string& _supportCodeDir) :
        mUseKinsol(false),
        mDiffStepSize(0.05),

        mSteadyStateThreshold(1.E-2),
        mRawRoadRunnerData(),
        mRoadRunnerData(),
        mCurrentSBMLFileName(""),
        mCVode(0),
        mSelectionList(),
        mModelGenerator(0),
        mComputeAndAssignConservationLaws(false),
        mSteadyStateSelection(),
        mModel(0),
        mCurrentSBML(),
        mLS(0),
        mSettings()
{


#if defined(BUILD_LLVM)
    string compiler = _compiler.empty() ? "LLVM" : _compiler;
#else
    string compiler = _compiler.empty() ? gDefaultCompiler : _compiler;
#endif

    string tempDir = _tempDir.empty() ? gDefaultTempFolder : _tempDir;
    string supportCodeDir = _supportCodeDir.empty() ?
            gDefaultSupportCodeFolder : _supportCodeDir;

    // for now, dump out who we are
    Log(Logger::PRIO_DEBUG) << __FUNC__ << "compiler: " << compiler <<
            ", tempDir:" << tempDir << ", supportCodeDir: " <<
            supportCodeDir;

    mModelGenerator = ModelGeneratorFactory::createModelGenerator(compiler,
            tempDir, supportCodeDir);

    setTempFileFolder(tempDir);

    //Increase instance count..
    mInstanceCount++;
    mInstanceID = mInstanceCount;
}

RoadRunner::~RoadRunner()
{
    Log(lDebug4)<<"In RoadRunner DTOR";

    Log(lDebug3) << "In " << __FUNC__ << "\n";

    delete mModelGenerator;
    delete mModel;
    delete mCVode;
    delete mLS;
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
    info<<"ConservationAnalysis: "    <<    mComputeAndAssignConservationLaws << endl;
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



LibStructural* RoadRunner::getLibStruct()
{
    Mutex::ScopedLock lock(roadRunnerMutex);

    if (mLS)
    {
        return mLS;
    }
    else if (!mCurrentSBML.empty())
    {
        mLS = new ls::LibStructural(mCurrentSBML);
        Log(Logger::PRIO_INFORMATION) << "created structural analysis, messages: "
                << mLS->getAnalysisMsg();
        return mLS;
    }
    else
    {
        throw Exception("could not create structural analysis with no loaded sbml");
        return 0;
    }
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

void RoadRunner::setSimulateOptions(const SimulateOptions& settings)
{
    mSettings = settings;
}

SimulateOptions& RoadRunner::getSimulateOptions()
{
    return mSettings;
}

bool RoadRunner::getConservationAnalysis()
{
    return mComputeAndAssignConservationLaws;
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
        theList.push_back("[" + oFloating[i] + "]");
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
    // make a list out of the values in the settings,
    // will always have at least a "time" at the first item.
    vector<string> settingsList = createSelectionList(mSettings);

    assert(settingsList.size() >= 1 && "selection list from SimulateOptions does does not have time");

    // if settings has any selections, this overrides the selection list.
    if (settingsList.size() > 1)
    {
        Log(Logger::PRIO_INFORMATION) << "overriding selection list with values from SimulateOptions.";
        setTimeCourseSelectionList(settingsList);
    }

    for(int i = 0; i < mSelectionList.size(); i++)
    {
        Log(Logger::PRIO_DEBUG) << "Selection Value [" << i << "]: " << mSelectionList[i].to_repr();
    }

    if(mSelectionList.size() < 2)
    {
        Log(Logger::PRIO_WARNING) << "No values selected for simulation result";
        return 0;
    }

    return mSelectionList.size();
}

ModelGenerator* RoadRunner::getModelGenerator()
{
    return mModelGenerator;
}

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

        mCVode = new CvodeInterface(mModel, &mSettings);

        // reset the simulation state
        reset();
        return true;
    }
    else
    {
        return false;
    }
}

RoadRunnerData *RoadRunner::getSimulationResult()
{
    return &mRoadRunnerData;
}

double RoadRunner::getSelectionValue(const SelectionRecord& record)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    double dResult;

    switch (record.selectionType)
    {
    case SelectionRecord::FLOATING_CONCENTRATION:

        dResult = 0;
        mModel->getFloatingSpeciesConcentrations(1, &record.index, &dResult);
        break;

    case SelectionRecord::BOUNDARY_CONCENTRATION:
        mModel->getBoundarySpeciesConcentrations(1, &record.index, &dResult);
        break;

    case SelectionRecord::REACTION_RATE:
        dResult = 0;
        mModel->getReactionRates(1, &record.index, &dResult);
        break;

    case SelectionRecord::FLOATING_AMOUNT_RATE:
        dResult = 0;
        mModel->computeAllRatesOfChange();
        mModel->getFloatingSpeciesAmountRates(1, &record.index, &dResult);
        break;

    case SelectionRecord::COMPARTMENT:
        mModel->getCompartmentVolumes(1, &record.index, &dResult);
        break;

    case SelectionRecord::GLOBAL_PARAMETER:
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

    case SelectionRecord::FLOATING_AMOUNT:
        mModel->getFloatingSpeciesAmounts(1, &record.index, &dResult);
        break;

    case SelectionRecord::BOUNDARY_AMOUNT:
        mModel->getBoundarySpeciesAmounts(1, &record.index, &dResult);
        break;

    case SelectionRecord::ELASTICITY:
        dResult = getEE(record.p1, record.p2, false);
        break;

    case SelectionRecord::UNSCALED_ELASTICITY:
        dResult = getuEE(record.p1, record.p2, false);
        break;
    case SelectionRecord::EIGENVALUE:
    {
        string species = record.p1;
        int index = mModel->getFloatingSpeciesIndex(species);

        DoubleMatrix mat;
        if (mComputeAndAssignConservationLaws)
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
            throw("Tried to access record in empty mSelectionList in getValue function: eigen");
        }

        if (oComplex.size() > mSelectionList[index + 1].index) //Becuase first one is time !?
        {
            return oComplex[mSelectionList[index + 1].index].Real;
        }
        return std::numeric_limits<double>::quiet_NaN();
    }
    break;
    case SelectionRecord::INITIAL_CONCENTRATION:
        mModel->getFloatingSpeciesInitConcentrations(1, &record.index, &dResult);
        break;
    case SelectionRecord::STOICHIOMETRY:
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

    if (record.selectionType == SelectionRecord::TIME)
    {
        return dCurrentTime;
    }
    else
    {
        return getSelectionValue(record);
    }
}

void RoadRunner::addNthOutputToResult(DoubleMatrix& results, int nRow, double dCurrentTime)
{
    for (u_int j = 0; j < mSelectionList.size(); j++)
    {
        double out =  getNthSelectedOutput(j, dCurrentTime);
        results(nRow,j) = out;
    }
}



bool RoadRunner::loadSBMLFromFile(const string& fileName, const LoadSBMLOptions *options)
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

    return loadSBML(sbml, options);
}

vector<double> RoadRunner::getConservedSums()
{
    return getLibStruct()->getConservedSums();
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

bool RoadRunner::loadSBML(const string& sbml, const LoadSBMLOptions *options)
{
    Mutex::ScopedLock lock(roadRunnerMutex);

    mCurrentSBML = sbml;

    //clear temp folder of roadrunner generated files, only if roadRunner instance == 1
    Log(lDebug)<<"Loading SBML into simulator";
    if (!sbml.size())
    {
        throw(CoreException("SBML string is empty!"));
    }


    delete mModel;

    if (options)
    {
        mComputeAndAssignConservationLaws = options->modelGeneratorOpt
                & LoadSBMLOptions::ConservationAnalysis;
        mModel = mModelGenerator->createModel(sbml, options->modelGeneratorOpt);
    }
    else
    {
        LoadSBMLOptions opt;
        opt.modelGeneratorOpt = getConservationAnalysis() ?
                opt.modelGeneratorOpt | LoadSBMLOptions::ConservationAnalysis :
                opt.modelGeneratorOpt & ~LoadSBMLOptions::ConservationAnalysis;
        mModel = mModelGenerator->createModel(sbml, opt.modelGeneratorOpt);
    }

    //Finally intitilaize the model..
    if(!initializeModel())
    {
        Log(lError)<<"Failed Initializing C Model";
        return false;
    }

    if (!options || !(options->loadFlags & LoadSBMLOptions::NO_DEFAULT_SELECTIONS))
    {
        createDefaultSelectionLists();
    }

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

        if (mComputeAndAssignConservationLaws && !mModel->getConservedSumChanged())
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

bool RoadRunner::populateResult()
{
    vector<string> list(mSelectionList.size());

    for(int i = 0; i < mSelectionList.size(); ++i)
    {
        list[i] = mSelectionList[i].to_string();
    }

    mRoadRunnerData.setColumnNames(list);
    mRoadRunnerData.setData(mRawRoadRunnerData);
    return true;
}


std::vector<rr::SelectionRecord>& RoadRunner::getSelections()
{
    return mSelectionList;
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

void RoadRunner::setParameterValue(const ParameterType::ParameterType parameterType,
        const int parameterIndex, const double value)
{
    switch (parameterType)
    {
        case ParameterType::ptBoundaryParameter:
            mModel->setBoundarySpeciesConcentrations(1, &parameterIndex, &value);
        break;

        case ParameterType::ptGlobalParameter:
            mModel->setGlobalParameterValues(1, &parameterIndex, &value);
        break;

        case ParameterType::ptFloatingSpecies:
            mModel->setFloatingSpeciesConcentrations(1, &parameterIndex, &value);
        break;

        case ParameterType::ptConservationParameter:
            mModel->setConservedSums(1, &parameterIndex, &value);
        break;

        case ParameterType::ptLocalParameter:
            throw Exception("Local parameters not permitted in setParameterValue (getCC, getEE)");
    }
}

double RoadRunner::getParameterValue(const ParameterType::ParameterType parameterType,
        const int parameterIndex)
{
    switch (parameterType)
    {
    case ParameterType::ptBoundaryParameter:
    {
        double result = 0;
        mModel->getBoundarySpeciesConcentrations(1, &parameterIndex, &result);
        return result;
    }
    break;
    case ParameterType::ptGlobalParameter:
    {
        double result = 0;
        mModel->getGlobalParameterValues(1, &parameterIndex, &result);
        return result;
    }
    break;

    // Used when calculating elasticities
    case ParameterType::ptFloatingSpecies:
    {
        double result = 0;
        mModel->getFloatingSpeciesConcentrations(1, &parameterIndex, &result);
        return result;
    }
    break;

    case ParameterType::ptConservationParameter:
    {
        double result = 0;
        mModel->getConservedSums(1, &parameterIndex, &result);
        return result;
    }
    break;

    case ParameterType::ptLocalParameter:
        throw Exception("Local parameters not permitted in getParameterValue (getCC?)");
        break;

    default:
        return 0.0;
        break;
    }
    return 0;
}


void RoadRunner::setConservationAnalysis(bool bValue)
{
    if(bValue == mComputeAndAssignConservationLaws)
    {
        Log(lDebug)<<"The compute and assign conservation laws flag already set to : "<<toString(bValue);
    }

    mComputeAndAssignConservationLaws = bValue;

    if(mModel != NULL)
    {
        LoadSBMLOptions opt;

        // set the comput and assign cons flag
        opt.modelGeneratorOpt = bValue ?
                opt.modelGeneratorOpt | LoadSBMLOptions::ConservationAnalysis :
                opt.modelGeneratorOpt & ~LoadSBMLOptions::ConservationAnalysis;

        // have to reload
        opt.modelGeneratorOpt = opt.modelGeneratorOpt | LoadSBMLOptions::ForceReCompile;

        if(!loadSBML(mCurrentSBML, &opt))
        {
            throw( CoreException("Failed re-Loading model when setting computeAndAssignConservationLaws"));
        }
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
    ParameterType::ParameterType parameterType;
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
        parameterType = ParameterType::ptFloatingSpecies;
    }
    else if ((parameterIndex = mModel->getBoundarySpeciesIndex(parameterName)) >= 0)
    {
        parameterType = ParameterType::ptBoundaryParameter;
    }
    else if ((parameterIndex = mModel->getGlobalParameterIndex(parameterName)) >= 0)
    {
        parameterType = ParameterType::ptGlobalParameter;
    }
    else if ((parameterIndex = mModel->getConservedSumIndex(parameterName)) >= 0)
    {
        parameterType = ParameterType::ptConservationParameter;
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
    ParameterType::ParameterType    mParameterType;
    int    mParameterIndex;
    double    mOriginalParameterValue;
    bool    mComputeSteadyState;
    RoadRunner*    mRR;

public:
    aFinalizer(ParameterType::ParameterType& pType, const int& pIndex,
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

        ParameterType::ParameterType parameterType;
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
            parameterType = ParameterType::ptFloatingSpecies;
            originalParameterValue = 0;
            mModel->getFloatingSpeciesConcentrations(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getBoundarySpeciesIndex(parameterName)) >= 0)
        {
            parameterType = ParameterType::ptBoundaryParameter;
            originalParameterValue = 0;
            mModel->getBoundarySpeciesConcentrations(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getGlobalParameterIndex(parameterName)) >= 0)
        {
            parameterType = ParameterType::ptGlobalParameter;
            originalParameterValue = 0;
            mModel->getGlobalParameterValues(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getConservedSumIndex(parameterName)) >= 0)
        {
            parameterType = ParameterType::ptConservationParameter;
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


// Help("Set the columns to be returned by simulate() or simulateEx(), valid symbol names include" +
//              " time, species names, , volume, reaction rates and rates of change (speciesName')")
void RoadRunner::setTimeCourseSelectionList(const vector<string>& _selList)
{
    mSelectionList.clear();

    for(int i = 0; i < _selList.size(); ++i)
    {
        mSelectionList.push_back(createSelection(_selList[i]));
    }
}


double RoadRunner::oneStep(const double currentTime, const double stepSize, const bool reset)
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



vector< Complex > RoadRunner::getEigenvaluesCpx()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        DoubleMatrix mat;
        if (mComputeAndAssignConservationLaws)
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
        if (mComputeAndAssignConservationLaws)
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

DoubleMatrix RoadRunner::getReducedJacobian()
{
    try
    {
        if (!mModel)
        {
            throw CoreException(gEmptyModelMessage);
        }

        if(mComputeAndAssignConservationLaws == false)
        {
            throw CoreException("The reduced Jacobian matrix can only be computed if conservation law detection is enabled");
        }

        ls::LibStructural &ls = *getLibStruct();

        DoubleMatrix uelast = getUnscaledElasticityMatrix();
        if(!ls.getNrMatrix())
        {
            return DoubleMatrix(0,0);
        }
        DoubleMatrix I1 = mult(*(ls.getNrMatrix()), uelast);
        DoubleMatrix *linkMat = ls.getLinkMatrix();
        return mult(I1, (*linkMat));
    }
    catch (const Exception& e)
    {
        throw CoreException("Unexpected error from getReducedJacobian(): ", e.Message());
    }
}

DoubleMatrix* RoadRunner::getLinkMatrix()
{
    try
    {
       if (!mModel)
       {
           throw CoreException(gEmptyModelMessage);
       }
       //return _L;
        return getLibStruct()->getLinkMatrix();
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
        return getLibStruct()->getNrMatrix();
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
       return getLibStruct()->getL0Matrix();
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
        DoubleMatrix* aMat = getLibStruct()->getReorderedStoichiometryMatrix();
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
        DoubleMatrix* aMat = getLibStruct()->getReorderedStoichiometryMatrix();
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
        DoubleMatrix* aMat = getLibStruct()->getFullyReorderedStoichiometryMatrix();
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
           DoubleMatrix* aMat = getLibStruct()->getGammaMatrix();
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
            return getLibStruct()->getNumDepSpecies();
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
            return getLibStruct()->getNumIndSpecies();
        }
        //return StructAnalysis.getNumIndSpecies();
        throw CoreException(gEmptyModelMessage);
    }
    catch (Exception &e)
    {
        throw CoreException("Unexpected error from getNumberOfIndependentSpecies()", e.Message());
    }
}

double RoadRunner::getVariableValue(const VariableType::VariableType variableType,
        const int variableIndex)
{
    switch (variableType)
    {
    case VariableType::vtFlux:
    {
        double result = 0;
        mModel->getReactionRates(1, &variableIndex, &result);
    }
    break;

    case VariableType::vtSpecies:
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
        result.push_back("eigen(" + floating[i] + ")");
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
        aRec.selectionType = SelectionRecord::FLOATING_CONCENTRATION;
        aRec.p1 = floatingSpecies[i];
        aRec.index = i;
        mSteadyStateSelection[i] = aRec;
    }
    return mSteadyStateSelection.size();
}

// Help("Returns the selection list as returned by computeSteadyStateValues().")
vector<string> RoadRunner::getSteadyStateSelections()
{
    vector<string> result;

    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (mSteadyStateSelection.size() == 0)
    {
          createDefaultSteadyStateSelectionList();
    }

    result.resize(mSteadyStateSelection.size());

    for(int i = 0; i < mSteadyStateSelection.size(); ++i)
    {
        result[i] = mSteadyStateSelection[i].to_string();
    }


    return result;
}

vector<SelectionRecord> RoadRunner::getSteadyStateSelection(const vector<string>& newSelectionList)
{
    vector<SelectionRecord> steadyStateSelection;
    steadyStateSelection.resize(newSelectionList.size());

    for (int i = 0; i < newSelectionList.size(); ++i)
    {
        steadyStateSelection[i] = createSelection(newSelectionList[i]);
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

vector<double> RoadRunner::computeSteadyStateValues(const vector<SelectionRecord>& selection, bool computeSteadyState)
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

    if (record.selectionType == SelectionRecord::UNKNOWN)
    {
        return computeSteadyStateValue(record.p1);
    }
    return getSelectionValue(record);
}


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
            return getSelectionValue(sId);
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

void RoadRunner::setGlobalParameterByIndex(const int index, const double value)
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

        ParameterType::ParameterType parameterType;
        VariableType::VariableType variableType;
        double originalParameterValue;
        int variableIndex;
        int parameterIndex;

        mModel->convertToConcentrations();
        mModel->evalReactionRates();

        // Check the variable name
        if ((variableIndex = mModel->getReactionIndex(variableName)) >= 0)
        {
            variableType = VariableType::vtFlux;
        }
        else if ((variableIndex = mModel->getFloatingSpeciesIndex(variableName)) >= 0)
        {
            variableType = VariableType::vtSpecies;
        }
        else
        {
            throw CoreException("Unable to locate variable: [" + variableName + "]");
        }

        // Check for the parameter name
        if ((parameterIndex = mModel->getGlobalParameterIndex(parameterName)) >= 0)
        {
            parameterType = ParameterType::ptGlobalParameter;
            originalParameterValue = 0;
            mModel->getGlobalParameterValues(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getBoundarySpeciesIndex(parameterName)) >= 0)
        {
            parameterType = ParameterType::ptBoundaryParameter;
            originalParameterValue = 0;
            mModel->getBoundarySpeciesConcentrations(1, &parameterIndex, &originalParameterValue);
        }
        else if ((parameterIndex = mModel->getConservedSumIndex(parameterName)) >= 0)
        {
            parameterType = ParameterType::ptConservationParameter;
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
    VariableType::VariableType variableType;
    ParameterType::ParameterType parameterType;
    int variableIndex;
    int parameterIndex;

    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    // Check the variable name
    if ((variableIndex = mModel->getReactionIndex(variableName)) >= 0)
    {
        variableType = VariableType::vtFlux;
    }
    else if ((variableIndex = mModel->getFloatingSpeciesIndex(variableName)) >= 0)
    {
        variableType = VariableType::vtSpecies;
    }
    else
    {
        throw CoreException("Unable to locate variable: [" + variableName + "]");
    }

    // Check for the parameter name
    if ((parameterIndex = mModel->getGlobalParameterIndex(parameterName)) >= 0)
    {
        parameterType = ParameterType::ptGlobalParameter;
    }
    else if ((parameterIndex = mModel->getBoundarySpeciesIndex(parameterName)) >= 0)
    {
        parameterType = ParameterType::ptBoundaryParameter;
    }
    else if ((parameterIndex = mModel->getConservedSumIndex(parameterName)) >= 0)
    {
        parameterType = ParameterType::ptConservationParameter;
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

        mSettings.start = 0;
        mSettings.duration = 50.0;
        mSettings.steps = 1;

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


Integrator* RoadRunner::getIntegrator()
{
    return mCVode;
}

void RoadRunner::correctMaxStep()
{
    if(mCVode)
    {
        double maxStep = (mSettings.duration) / (mSettings.steps + 1);
        maxStep = min(mCVode->mMaxStep, maxStep);
        mCVode->mMaxStep = maxStep;
    }
}

bool RoadRunner::setValue(const string& sId, double dValue)
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
double RoadRunner::getSelectionValue(const string& sel)
{
    return getSelectionValue(createSelection(sel));
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
    return string(RR_VERSION) + string(", compiled with ") + string(RR_COMPILER)
            + " on date " + string( __DATE__ ) + ", " + string(__TIME__);
}

string RoadRunner::getCopyright()
{
    return "(c) 2009-2013 HM. Sauro, FT. Bergmann, Totte Karlsson and Andy Somogyi, BSD Licence";
}


string RoadRunner::getlibSBMLVersion()
{
    return libsbml::getLibSBMLDottedVersion();
}

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

const RoadRunnerData* RoadRunner::simulate(const SimulateOptions* _options)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if (_options)
    {
        this->mSettings = *_options;
    }

    //This one creates the list of what we will look at in the result
    createTimeCourseSelectionList();

    if (mCVode)
    {
        mCVode->setTolerances(mSettings.relative, mSettings.absolute);
    }

    if (mSettings.flags & SimulateOptions::ResetModel)
    {
        reset(); // reset back to initial conditions
    }

    if (mSettings.duration < 0 || mSettings.start < 0
            || mSettings.steps <= 0 )
    {
        throw CoreException("Illegal input to simulate");
    }

    double timeEnd = mSettings.duration - mSettings.start;
    double timeStart = mSettings.start;
    int numPoints = mSettings.steps + 1;

    if (numPoints <= 1)
    {
        numPoints = 2;
    }

    double hstep = (timeEnd - timeStart) / (numPoints - 1);
    int nrCols = mSelectionList.size();

    if(!nrCols)
    {
        assert(0 && "time couse selection list is empty");
        nrCols = createDefaultTimeCourseSelectionList();
    }

    // ignored if same
    mRawRoadRunnerData.resize(mSettings.steps + 1, nrCols);

    // evalute the model with its current state
    mModel->evalModel(timeStart, 0, 0);

    addNthOutputToResult(mRawRoadRunnerData, 0, timeStart);

    //Todo: Don't understand this code.. MTK
    if (mCVode->haveVariables())
    {
        mCVode->reStart(timeStart, mModel);
    }

    double tout = timeStart;

    //The simulation is executed right here..
    Log(Logger::PRIO_DEBUG)<<"Will run the OneStep function "<< mSettings.steps + 1 <<" times";
    for (int i = 1; i < mSettings.steps + 1; i++)
    {
        Log(Logger::PRIO_DEBUG)<<"Step "<<i;
        mCVode->oneStep(tout, hstep);
        tout = timeStart + i * hstep;
        addNthOutputToResult(mRawRoadRunnerData, i, tout);
    }
    Log(Logger::PRIO_DEBUG)<<"Simulation done..";

    // set the data into the RoadRunnerData struct
    populateResult();

    return &mRoadRunnerData;
}


static std::vector<std::string> createSelectionList(const SimulateOptions& o)
{
    //read from settings the variables found in the amounts and concentrations lists
    std::vector<std::string> theList;
    SelectionRecord record;

    theList.push_back("time");

    int nrOfVars = o.variables.size();

    for(int i = 0; i < o.amounts.size(); i++)
    {
        theList.push_back(o.amounts[i]);        //In the setSelection list below, the [] selects the correct 'type'
    }

    for(int i = 0; i < o.concentrations.size(); i++)
    {
        theList.push_back("[" + o.concentrations[i] + "]");
    }

    //We may have variables
    //A variable 'exists' only in "variables", not in the amount or concentration section

    for(int i = 0; i < o.variables.size(); i++)
    {
        string aVar = o.variables[i];

        if ((find(o.amounts.begin(), o.amounts.end(), aVar) == o.amounts.end()) &&
                (find(o.concentrations.begin(), o.concentrations.end(), aVar)
                        == o.concentrations.end()))
        {
            theList.push_back(o.variables[i]);
        }
    }

    return theList;
}


_xmlNode *RoadRunner::createConfigNode()
{
    // the top level capabilities
    _xmlNode *capies = Configurable::createCapabilitiesNode("RoadRunner",
            "RoadRunner Capabilities");

    // capability for this class
    _xmlNode *caps = Configurable::createCapabilityNode("RoadRunner Core", "",
            "Core RoadRunner Capability");

    Configurable::addChild(caps, Configurable::createParameterNode("Conservation",
                    "enables (=true) or disables (=false) the conservation analysis of models for timecourse simulations.",
                    (int) mComputeAndAssignConservationLaws));

    Configurable::addChild(capies, caps);

    // capability for child objects
    if (this->mCVode)
    {
        Configurable::addChild(capies, mCVode->createConfigNode());
    }

    return capies;
}

void RoadRunner::loadConfig(const _xmlDoc* doc)
{
}

std::string RoadRunner::getConfigurationXML()
{
    return Configurable::xmlFromConfigNode(createConfigNode());
}

void RoadRunner::setConfigurationXML(const std::string& xml)
{
    Configurable::loadXmlConfig(xml, this);
}

SelectionRecord RoadRunner::createSelection(const std::string& str)
{
    if (!mModel)
    {
        throw Exception("Can not create selection without a model");
    }

    SelectionRecord sel(str);

    if (sel.selectionType == SelectionRecord::UNKNOWN)
    {
        throw Exception("invalid selection string " + str);
    }

    // check to see that we have valid selection ids
    switch(sel.selectionType)
    {
    case SelectionRecord::TIME:
        // time is always fine...
        break;
    case SelectionRecord::UNKNOWN_ELEMENT:
        // check for sbml element types

        if ((sel.index = mModel->getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            sel.selectionType = SelectionRecord::FLOATING_AMOUNT;
            break;
        }
        else if ((sel.index = mModel->getBoundarySpeciesIndex(sel.p1)) >= 0)
        {
            sel.selectionType = SelectionRecord::BOUNDARY_AMOUNT;
            break;
        }
        else if ((sel.index = mModel->getCompartmentIndex(sel.p1)) >= 0)
        {
            sel.selectionType = SelectionRecord::COMPARTMENT;
            break;
        }
        else if ((sel.index = mModel->getGlobalParameterIndex(sel.p1)) >= 0)
        {
            sel.selectionType = SelectionRecord::GLOBAL_PARAMETER;
            break;
        }
        else if ((sel.index = mModel->getReactionIndex(sel.p1)) >= 0)
        {
            sel.selectionType = SelectionRecord::REACTION_RATE;
            break;
        }
        else
        {
            throw Exception("No sbml element exists for symbol '" + str + "'");
            break;
        }
    case SelectionRecord::UNKNOWN_CONCENTRATION:
        if ((sel.index = mModel->getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            sel.selectionType = SelectionRecord::FLOATING_CONCENTRATION;
            break;
        }
        else if ((sel.index = mModel->getBoundarySpeciesIndex(sel.p1)) >= 0)
        {
            sel.selectionType = SelectionRecord::BOUNDARY_CONCENTRATION;
            break;
        }
        else
        {
            string msg = "No sbml element exists for concentration selection '" + str + "'";
            Log(Logger::PRIO_ERROR) << msg;
            throw Exception(msg);
            break;
        }
    case SelectionRecord::FLOATING_AMOUNT_RATE:
        if ((sel.index = mModel->getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            break;
        }
        else
        {
            throw Exception("Invalid id '" + str + "' for floating amount rate");
            break;
        }
    case SelectionRecord::ELASTICITY:
    case SelectionRecord::UNSCALED_ELASTICITY:
    case SelectionRecord::CONTROL:
    case SelectionRecord::UNSCALED_CONTROL:
        // check that the control coef args are valid
        if (mModel->getReactionIndex(sel.p1) >= 0 ||
                mModel->getFloatingSpeciesIndex(sel.p1) >= 0)

        {
            if (mModel->getGlobalParameterIndex(sel.p2) >= 0 ||
                    mModel->getBoundarySpeciesIndex(sel.p2) >= 0 ||
                    mModel->getConservedSumIndex(sel.p2) >= 0)
            {
                Log(Logger::PRIO_INFORMATION) <<
                        "Valid metabolic control selection: " << sel.to_repr();
            }
            else
            {
                throw Exception("The second argument to a metabolic control "
                        "coefficient selection, " + sel.p2 + ", must be either "
                        "a global parameter, boundary species or conserved sum");
            }
        }
        else
        {
            throw Exception("The first argument to a metabolic control "
                    "coefficient selection, " + sel.p1 + ", must be either "
                    "a reaction, floating species or conserved sum");
        }

        break;
    case SelectionRecord::EIGENVALUE:
        Log(Logger::PRIO_WARNING) << "syntactically valid eigen, however not enabled yet";
        break;
    case SelectionRecord::STOICHIOMETRY:
        Log(Logger::PRIO_WARNING) << "syntactically valid stoichiometry, however not enabled yet";
        break;
    case SelectionRecord::INITIAL_CONCENTRATION:
        if ((sel.index = mModel->getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            break;
        }
        else
        {
            throw Exception("Invalid id '" + sel.p1 + "' for floating initial concentration");
            break;
        }
    default:
        Log(Logger::PRIO_ERROR) << "A new SelectionRecord should not have this value: "
            << sel.to_repr();
        break;
    }

    return sel;
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



#pragma hdrstop
// on Windows, this needs to go first to get M_PI
#define _USE_MATH_DEFINES

#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrModelGenerator.h"
#include "rrCompiler.h"
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
#include "rrSBMLReader.h"
#include "c/rrNOMSupport.h"

#include <iostream>
#include <math.h>
#include <assert.h>
#include <rr-libstruct/lsLibStructural.h>
#include <Poco/File.h>
#include <Poco/Mutex.h>


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

/**
 * Some functions needed for the freq. analysis code
 * From Frank Bergmann code, ported to C++ by Totte Karlson
 * Generate log spaced array, algorithm from matlab, note operator precedence
 * y = (10).^ [d1+(0:n-2)*(d2-d1)/(floor(n)-1), d2];
 *
 * d1 = starting value (10^d1)
 * d2 = ending value (10^d2)
 * n = number of values in the generated series
 */
static vector<double>  logspace(const double& startW, const double& d2, const int& n);
static double          phase(Complex& val);
static double          getAdjustment(Complex& z);



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
    Log(Logger::LOG_DEBUG) << __FUNC__ << "compiler: " << compiler <<
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
    info<<"libSBML version: "         <<    getVersionStr(VERSIONSTR_LIBSBML) << endl;
    info<<"Temporary folder: "        <<    getTempFolder()<<endl;
    info<<"Compiler location: "       <<    getCompiler()->getCompilerLocation() << endl;
    info<<"Support Code Folder: "     <<    getCompiler()->getSupportCodeFolder() << endl;
    info<<"Working Directory: "       <<    getCWD() << endl;
    return info.str();
}

string RoadRunner::getExtendedVersionInfo()
{
    stringstream info;
    info << getVersionStr(VERSIONSTR_BASIC | VERSIONSTR_COMPILER | VERSIONSTR_DATE | VERSIONSTR_LIBSBML) << endl;
    info << "Working Directory: " << getCWD() << endl;
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
        Log(Logger::LOG_INFORMATION) << "created structural analysis, messages: "
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

bool RoadRunner::getConservedMoietyAnalysis()
{
    return mComputeAndAssignConservationLaws;
}

bool RoadRunner::setTempFileFolder(const string& folder)
{
    return mModelGenerator ? mModelGenerator->setTemporaryDirectory(folder) : false;
}

string RoadRunner::getTempFolder()
{
    return mModelGenerator ? mModelGenerator->getTemporaryDirectory() : string("");
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

    setSelections(theList);

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
        Log(Logger::LOG_INFORMATION) << "overriding selection list with values from SimulateOptions.";
        setSelections(settingsList);
    }

    for(int i = 0; i < mSelectionList.size(); i++)
    {
        Log(Logger::LOG_DEBUG) << "Selection Value [" << i << "]: " << mSelectionList[i].to_repr();
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


double RoadRunner::getValue(const SelectionRecord& record)
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
            mModel->getConservedMoietyValues(1, &index, &dResult);
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

    case SelectionRecord::CONTROL:
        dResult = this->getCC(record.p1, record.p2);
        break;

    case SelectionRecord::UNSCALED_CONTROL:
        dResult = this->getuCC(record.p1, record.p2);
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

        if (oComplex.size() > index) //Becuase first one is time !?
        {
            return std::real(oComplex[index]);
        }
        return std::numeric_limits<double>::quiet_NaN();
    }
    break;
    case SelectionRecord::INITIAL_FLOATING_CONCENTRATION:
        mModel->getFloatingSpeciesInitConcentrations(1, &record.index, &dResult);
        break;
    case SelectionRecord::STOICHIOMETRY:
    {
        int speciesIndex = mModel->getFloatingSpeciesIndex(record.p1);
        int reactionIndex = mModel->getReactionIndex(record.p2);
        return mModel->getStoichiometry(speciesIndex, reactionIndex);
    }
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
        return getValue(record);
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


vector<double> RoadRunner::getConservedMoietyValues()
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

bool RoadRunner::load(const string& uriOrSbml, const LoadSBMLOptions *options)
{
    Mutex::ScopedLock lock(roadRunnerMutex);

    mCurrentSBML = SBMLReader::read(uriOrSbml);

    //clear temp folder of roadrunner generated files, only if roadRunner instance == 1
    Log(lDebug)<<"Loading SBML into simulator";
    if (!mCurrentSBML.size())
    {
        throw(CoreException("SBML string is empty!"));
    }

    delete mModel;
    mModel = 0;

    if (options)
    {
        mComputeAndAssignConservationLaws = options->modelGeneratorOpt
                & LoadSBMLOptions::CONSERVED_MOIETIES;
        mModel = mModelGenerator->createModel(mCurrentSBML, options->modelGeneratorOpt);
    }
    else
    {
        LoadSBMLOptions opt;
        opt.modelGeneratorOpt = getConservedMoietyAnalysis() ?
                opt.modelGeneratorOpt | LoadSBMLOptions::CONSERVED_MOIETIES :
                opt.modelGeneratorOpt & ~LoadSBMLOptions::CONSERVED_MOIETIES;
        mModel = mModelGenerator->createModel(mCurrentSBML, opt.modelGeneratorOpt);
    }

    //Finally intitilaize the model..
    if(!initializeModel())
    {
        Log(Logger::LOG_ERROR)<<"Failed Initializing ExecutableModel";
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
            Log(Logger::LOG_ERROR)<<"Kinsol solver is not enabled...";
            return -1;
    }

    NLEQInterface steadyStateSolver(mModel);

    if (configurationXML.length() > 0)
    {
        Configurable::loadXmlConfig(configurationXML, &steadyStateSolver);
    }

    //Get a std vector for the solver
    vector<double> someAmounts(mModel->getNumIndFloatingSpecies(), 0);
    mModel->getFloatingSpeciesAmounts(someAmounts.size(), 0, &someAmounts[0]);

    double ss = steadyStateSolver.solve(someAmounts);
    if(ss < 0)
    {
        Log(Logger::LOG_ERROR)<<"Steady State solver failed...";
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
            mModel->setConservedMoietyValues(1, &parameterIndex, &value);
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
        mModel->getConservedMoietyValues(1, &parameterIndex, &result);
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


void RoadRunner::setConservedMoietyAnalysis(bool bValue)
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
                opt.modelGeneratorOpt | LoadSBMLOptions::CONSERVED_MOIETIES :
                opt.modelGeneratorOpt & ~LoadSBMLOptions::CONSERVED_MOIETIES;

        // have to reload
        opt.modelGeneratorOpt = opt.modelGeneratorOpt | LoadSBMLOptions::RECOMPILE;

        if(!load(mCurrentSBML, &opt))
        {
            throw( CoreException("Failed re-Loading model when setting computeAndAssignConservationLaws"));
        }
    }
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
    else if ((parameterIndex = mModel->getConservedMoietyIndex(parameterName)) >= 0)
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
        else if ((parameterIndex = mModel->getConservedMoietyIndex(parameterName)) >= 0)
        {
            parameterType = ParameterType::ptConservationParameter;
            originalParameterValue = 0;
            mModel->getConservedMoietyValues(1, &parameterIndex, &originalParameterValue);
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
        DoubleMatrix *rsm;
        LibStructural *ls = getLibStruct();
        if (mComputeAndAssignConservationLaws)
        {
            rsm = ls->getReorderedStoichiometryMatrix();
        }
        else
        {
            rsm = ls->getStoichiometryMatrix();
        }
       return mult(*rsm, uelast);

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
            LibStructural *ls = getLibStruct();
            DoubleMatrix uelast = getUnscaledElasticityMatrix();
            DoubleMatrix *rsm = ls->getStoichiometryMatrix();
            return mult(*rsm, uelast);
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

vector<SelectionRecord>& RoadRunner::getSteadyStateSelections()
{
    return mSteadyStateSelection;
}

vector<double> RoadRunner::getSteadyStateValues()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    if(mSteadyStateSelection.size() == 0)
    {
        createDefaultSteadyStateSelectionList();
    }

    steadyState();

    vector<double> result; //= new double[oSelection.Length];
    for (int i = 0; i < mSteadyStateSelection.size(); i++)
    {
        result.push_back(getValue(mSteadyStateSelection[i]));
    }
    return result;
}

string RoadRunner::getModelName()
{
    return mModel ? mModel->getModelName() : string("");
}

// TODO Looks like major problems here, this
// apears to modify the AFTER a model has been created from it.
string RoadRunner::getCurrentSBML()
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






// Help("Get the number of floating species")
int RoadRunner::getNumberOfFloatingSpecies()
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    return mModel->getNumFloatingSpecies();
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
void RoadRunner::setFloatingSpeciesByIndex(int index, double value)
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
double RoadRunner::getFloatingSpeciesByIndex(const int index)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    if ((index >= 0) && (index < mModel->getNumFloatingSpecies()))
    {
        double result = 0;
        mModel->getFloatingSpeciesConcentrations(1, &index, &result);
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
                    mModel->getNumDepFloatingSpecies()))
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

    if ((index >= 0) && (index < (mModel->getNumGlobalParameters() + mModel->getNumDepFloatingSpecies())))
    {
        int arraySize = mModel->getNumGlobalParameters() + mModel->getNumDepFloatingSpecies();
        double* data = new double[arraySize];

        mModel->getGlobalParameterValues(mModel->getNumGlobalParameters(), 0, data);

        mModel->getConservedMoietyValues(mModel->getNumDepFloatingSpecies(), 0, data + mModel->getNumGlobalParameters());

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

    if (mModel->getNumDepFloatingSpecies() > 0)
    {
        vector<double> result(mModel->getNumGlobalParameters() +
                mModel->getNumDepFloatingSpecies());

        mModel->getGlobalParameterValues(
                mModel->getNumGlobalParameters(), 0, &result[0]);

        mModel->getConservedMoietyValues(
                mModel->getNumDepFloatingSpecies(), 0,
                &result[mModel->getNumGlobalParameters()]);

        return result;
    }

    vector<double> result(mModel->getNumGlobalParameters());
    mModel->getGlobalParameterValues(result.size(), 0, &result[0]);
    return result;
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
        else if ((parameterIndex = mModel->getConservedMoietyIndex(parameterName)) >= 0)
        {
            parameterType = ParameterType::ptConservationParameter;
            originalParameterValue = 0;
            mModel->getConservedMoietyValues(1, &parameterIndex, &originalParameterValue);
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
    else if ((parameterIndex = mModel->getConservedMoietyIndex(parameterName)) >= 0)
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
        Log(Logger::LOG_ERROR)<<"Something went wrong in "<<__FUNCTION__;
        Log(Logger::LOG_ERROR)<<"Exception "<<e.what()<< " thrown";
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

DoubleMatrix RoadRunner::getScaledElasticityMatrix()
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
        Log(Logger::LOG_ERROR)<<gEmptyModelMessage;
        return false;
    }

    try
    {
        mModel->setValue(sId, dValue);
    }
    catch (std::exception& ex)
    {
        Log(Logger::LOG_ERROR) << "setValue(" << sId << ", " << dValue << ") failed, "
                << ex.what();
    }

    int nIndex = 0;

    if ((nIndex = mModel->getConservedMoietyIndex(sId)) >= 0)
    {
        mModel->setConservedMoietyValues(1, &nIndex, &dValue);
        mModel->updateDependentSpeciesValues();
        mModel->setConservedSumChanged(true);
        return true;
    }

    SelectionRecord sel(sId);

    if (sel.selectionType == SelectionRecord::INITIAL_FLOATING_AMOUNT ||
            sel.selectionType == SelectionRecord::INITIAL_FLOATING_CONCENTRATION)
    {
        reset();
    }

    return true;
}


double RoadRunner::getValue(const std::string& sel)
{
    return getValue(createSelection(sel));
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

    if (mSettings.flags & SimulateOptions::RESET_MODEL)
    {
        reset(); // reset back to initial conditions
    }

    if (mSettings.duration < 0 || mSettings.start < 0
            || mSettings.steps <= 0 )
    {
        throw CoreException("Illegal input to simulate");
    }

    // set how the result should be returned to python
    mRoadRunnerData.structuredResult = mSettings.flags & SimulateOptions::STRUCTURED_RESULT;

    double timeEnd = mSettings.duration + mSettings.start;
    double timeStart = mSettings.start;
    int numPoints = mSettings.steps + 1;

    if (numPoints <= 1)
    {
        numPoints = 2;
    }

    double hstep = (timeEnd - timeStart) / (numPoints - 1);
    int nrCols = mSelectionList.size();

    Log(Logger::LOG_DEBUG) << "starting simulation with " << nrCols << " selected columns";

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
    Log(Logger::LOG_DEBUG)<<"Will run the OneStep function "<< mSettings.steps + 1 <<" times";
    for (int i = 1; i < mSettings.steps + 1; i++)
    {
        Log(Logger::LOG_DEBUG)<<"Step "<<i;
        mCVode->oneStep(tout, hstep);
        tout = timeStart + i * hstep;
        addNthOutputToResult(mRawRoadRunnerData, i, tout);
    }
    Log(Logger::LOG_DEBUG)<<"Simulation done..";

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
    if (mCVode)
    {
        Configurable::addChild(capies, mCVode->createConfigNode());
    }
    else
    {
        CvodeInterface tmp(0, &mSettings);
        Configurable::addChild(capies, tmp.createConfigNode());
    }

    // nleq only exists during steadyState, so we need to create a tmp
    // one and load it with the xml if we given.
    NLEQInterface nleq(0);

    if (configurationXML.length() > 0)
    {
        Configurable::loadXmlConfig(configurationXML, &nleq);
    }

    Configurable::addChild(capies, nleq.createConfigNode());

    return capies;
}

void RoadRunner::loadConfig(const _xmlDoc* doc)
{
    if (mCVode)
    {
        mCVode->loadConfig(doc);
    }
}

std::string RoadRunner::getConfigurationXML()
{
    return Configurable::xmlFromConfigNode(createConfigNode());
}

void RoadRunner::setConfigurationXML(const std::string& xml)
{
    this->configurationXML = xml;
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
            Log(Logger::LOG_ERROR) << msg;
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
                    mModel->getConservedMoietyIndex(sel.p2) >= 0)
            {
                Log(Logger::LOG_INFORMATION) <<
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
        if ((sel.index = mModel->getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            break;
        }
        else
        {
            throw Exception("eigen argument '" + sel.p1 + "' is not a floating species.");
        }
        break;
    case SelectionRecord::STOICHIOMETRY:
        if (mModel->getFloatingSpeciesIndex(sel.p1) >= 0)
        {
            if (mModel->getReactionIndex(sel.p2) >= 0)
            {
                break;
            }
            else
            {
                throw Exception("second argument to stoich '" + sel.p2 + "' is not a reaction id.");
            }
        }
        else
        {
            throw Exception("first argument to stoich '" + sel.p1 + "' is not a floating species id.");
        }
        break;
    case SelectionRecord::INITIAL_FLOATING_CONCENTRATION:
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
        Log(Logger::LOG_ERROR) << "A new SelectionRecord should not have this value: "
            << sel.to_repr();
        break;
    }

    return sel;
}

void RoadRunner::setSelections(const vector<string>& _selList)
{
    mSelectionList.clear();

    for(int i = 0; i < _selList.size(); ++i)
    {
        mSelectionList.push_back(createSelection(_selList[i]));
    }
}

void RoadRunner::setSelections(const std::vector<rr::SelectionRecord>& ss)
{
    mSelectionList = ss;
}

void RoadRunner::setSteadyStateSelections(const vector<string>& ss)
{
    mSteadyStateSelection.clear();

    for(int i = 0; i < ss.size(); ++i)
    {
        mSteadyStateSelection.push_back(createSelection(ss[i]));
    }
}

void RoadRunner::setSteadyStateSelections(const std::vector<rr::SelectionRecord>& ss)
{
    mSteadyStateSelection = ss;
}



//Compute the frequency response, startW, Number Of Decades, Number of Points, parameterName, variableName
Matrix<double> RoadRunner::getFrequencyResponse(double startFrequency,
        int numberOfDecades, int numberOfPoints,
        const string& parameterName, const string& variableName,
        bool useDB, bool useHz)
{
    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }

    try
    {
        vector<string> reactionNames = getReactionIds();
        vector<string> speciesNames = getFloatingSpeciesIds();

        // Prepare the dv/dp array
        Matrix< Complex > dvdp(reactionNames.size(), 1);

        //Guess we don't need to simulate here?? (TK)
        //        SimulateOptions opt;
        //        opt.start = 0;
        //        opt.duration = 50.0;
        //        opt.steps = 1;
        //        simulate(&opt);
        if(steadyState() > 1E-2)
        {
            throw Exception("Unable to locate steady state during frequency response computation");
        }

        ComplexMatrix uelast(getUnscaledElasticityMatrix());
        ComplexMatrix Nr( *(getNrMatrix()));
        ComplexMatrix LinkMatrix( *(getLinkMatrix()));

        // Compute dv/dp
        for (int j = 0; j < reactionNames.size(); j++)
        {
            double val = getUnscaledParameterElasticity(reactionNames[j], parameterName);
            dvdp(j, 0) = Complex(val, 0.0);
            Log(lDebug)<<"dv/dp: " << dvdp(j, 0);
        }

        // Compute the Jacobian first
        ComplexMatrix T1(Nr.RSize(), uelast.CSize());
        T1 = mult(Nr, uelast);
        Log(lInfo)<<T1;

        ComplexMatrix Jac(Nr.RSize(), LinkMatrix.CSize());
        Jac = mult(T1, LinkMatrix);

        ComplexMatrix Id(Nr.RSize(), Nr.RSize());

        //Create Identity matrix
        for(int i = 0; i < Id.RSize(); i++)
        {
            Id(i,i) = Complex(1,0);
        }

        ComplexMatrix T2(Nr.RSize(), LinkMatrix.CSize());   // Stores iwI - Jac  and (iwI - Jac)^-1
        ComplexMatrix T3(LinkMatrix.RSize(), 1);            // Stores (iwI - Jac)^-1 . Nr
        ComplexMatrix T4(Nr.RSize(), Nr.CSize());
        ComplexMatrix T5(LinkMatrix.RSize(), 1);
        ComplexMatrix Inv(Nr.RSize(), LinkMatrix.CSize());

        // resultArray stores frequency, gain and phase
        Matrix<double> resultArray(numberOfPoints, 3);

        // Main loop, generate log spaced frequency numbers and compute the gain and phase at each frequency
        vector<double> w(logspace(startFrequency, numberOfDecades, numberOfPoints));

        for (int i = 0; i < numberOfPoints; i++)
        {
            Complex diagVal(0.0, w[i]);

            T1 = multDiag(Id, diagVal); // Compute iwI
            T2 = subtract(T1, Jac);     // Compute iwI - Jac //// Compute (iwI - Jac)^-1
            Inv = GetInverse(T2);
            T3  = mult(Inv, Nr);        // Compute (iwI - Jac)^-1 . Nr

            for (int j = 0; j < reactionNames.size(); j++)
            {
                double realPart = getUnscaledParameterElasticity(reactionNames[j], parameterName);
                dvdp(j, 0) = ls::Complex(realPart, 0.0);
            }

            T4 = mult(T3, dvdp);    // Compute(iwI - Jac)^-1 . Nr . dvdp

            // Finally include the dependent set as well.
            T5 = mult(LinkMatrix, T4); // Compute L (iwI - Jac)^-1 . Nr . dvdp

            for (int j = 0; j < speciesNames.size(); j++)
            {
                if (speciesNames[j] == variableName)
                {
                    double dw = abs(T5(j, 0));
                    if (useDB)
                    {
                        dw = 20.0*log10(dw);
                    }
                    resultArray(i,1) = dw;
                    Complex val(T5(j, 0));

                    double phase = (180.0/M_PI) * rr::phase(val) + getAdjustment(val);
                    resultArray(i,2) = phase;
                    break;
                }
            }
            if (useHz)
            {
                // Store frequency, convert to Hz by dividing by 2Pi
                resultArray[i][0] = w[i]/(2.*M_PI);
            }
            else
            {
                // Store frequency, leave as rad/sec
                resultArray[i][0] = w[i];
            }
        }
        return resultArray;
    }
    catch(const Exception& e)
    {
      throw Exception("Unexpected error in getFrequencyResponse(): " +  e.Message());
    }
}



double RoadRunner::getUnscaledParameterElasticity(const string& reactionName, const string& parameterName)
{
    int parameterIndex;

    if (!mModel)
    {
        throw CoreException(gEmptyModelMessage);
    }
    try
    {
        mModel->convertToConcentrations();
        mModel->evalReactionRates();

        int reactionIndex = mModel->getReactionIndex(reactionName);

        if(reactionIndex == -1)
        {
            throw CoreException("Internal Error: unable to locate reaction name while computing unscaled elasticity");
        }

        // Look for the parameter name, check local parameters first, then global
        ParameterType::ParameterType parameterType;

//        if (1==2)//(ModelGenerator.Instance.localParameterList[reactionIndex].find(reactionName, parameterName, out parameterIndex))
//        {
//    //        parameterType = TParameterType.ptLocalParameter;
//        }
        if(mModel->getGlobalParameterIndex(parameterName) != -1)
        {
            parameterIndex = mModel->getGlobalParameterIndex(parameterName);
            parameterType  = ParameterType::ptGlobalParameter;
        }
        else if(mModel->getBoundarySpeciesIndex(parameterName) != -1)
        {
            parameterIndex = mModel->getBoundarySpeciesIndex(parameterName);
            parameterType  = ParameterType::ptBoundaryParameter;
        }
        else if(mModel->getConservedMoietyIndex(parameterName) != -1)
        {
            parameterIndex = mModel->getConservedMoietyIndex(parameterName);
            parameterType = ParameterType::ptConservationParameter;
        }
        else
        {
            return 0.0;
        }

        double originalParameterValue = 0.0;
        double result = 0;
        switch (parameterType)
        {
            case ParameterType::ptGlobalParameter:
                originalParameterValue = getGlobalParameterByIndex(parameterIndex);
            break;
            case ParameterType::ptBoundaryParameter:
                originalParameterValue = getBoundarySpeciesByIndex(parameterIndex);
            break;
            case ParameterType::ptConservationParameter:
                //originalParameterValue = mModel->getC ;//model.ct[parameterIndex];
                mModel->getConservedMoietyValues(1, &parameterIndex, &result);
                originalParameterValue = result;
            break;
            default:
                throw(Exception("This parameterType is not supported in getUnscaledParameterElasticity"));
        }

        double hstep = mDiffStepSize*originalParameterValue;
        if (fabs(hstep) < 1E-12)
        {
            hstep = mDiffStepSize;
        }

        double f1, f2, fi, fi2, fd, fd2;
        changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, hstep);
        mModel->convertToConcentrations();
        mModel->evalReactionRates();
        mModel->getReactionRates(1, &reactionIndex, &fi);

        changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, 2.0*hstep);
        mModel->evalReactionRates();
        mModel->getReactionRates(1, &reactionIndex, &fi2);

        changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, -hstep);
        mModel->evalReactionRates();
        mModel->getReactionRates(1, &reactionIndex, &fd);

        changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, -2.0*hstep);
        mModel->evalReactionRates();
        mModel->getReactionRates(1, &reactionIndex, &fd2);

        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
        // The following separated lines avoid small amounts of roundoff error
        f1 = fd2 + 8.0*fi;
        f2 = -(8.0*fd + fi2);

        // What ever happens, make sure we restore the species level
        changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, 0);
        return 1.0/(12.0*hstep)*(f1 + f2);
    }
    catch(const Exception& ex)
    {
        throw(Exception("Problem in getUnscaledParameterElasticity():" + ex.getMessage()));
    }
}

// Changes a given parameter type by the given increment
void RoadRunner::changeParameter(ParameterType::ParameterType parameterType, int reactionIndex, int parameterIndex,
                                    double originalValue, double increment)
{
    setParameterValue(parameterType, parameterIndex, originalValue + increment);
}


vector<double> logspace(const double& startW, const double& d2, const int& n)
{
    double d1 = 0;
    vector<double> y(n);
    for (int i = 0; i <= n - 2; i++)
    {
        y[i] = i*(d2 - d1);
        y[i] = y[i]/(n - 1);
        y[i] = d1 + y[i];
        y[i] = pow(10, y[i]) * startW;
    }
    y[n - 1] = pow(10, d2)*startW;
    return y;
}

double phase(Complex& val)
{
    if ((real(val) == 0.0) && (imag(val) == 0.0))
    {
        return 0.0;
    }
    else
    {
        return atan2(imag(val), real(val));
    }
}

//This function is used in roadrunners freq analysis code...
double getAdjustment(Complex& z)
{
    double adjustment;
    if (real(z) >= 0 && imag(z) >= 0)
    {
        adjustment = 0;
    }
    else if (real(z) >= 0 && imag(z) < 0)
    {
        adjustment = 360;
    }
    else if (real(z) < 0 && imag(z) >= 0)
    {
        adjustment = 0;
    }
    else
    {
        adjustment = 360;
    }
    return adjustment;
}


/************************ Selection Ids Species Section ***********************/
#if (1) /**********************************************************************/
/******************************************************************************/

void RoadRunner::getIds(int types, std::list<std::string>& ids)
{
    if (mModel)
    {
        mModel->getIds(types, ids);

        if (types & SelectionRecord::EIGENVALUE)
        {
            std::list<std::string> eigen;
            mModel->getIds(SelectionRecord::FLOATING_AMOUNT, eigen);

            for (std::list<std::string>::const_iterator i =
                    eigen.begin(); i != eigen.end(); ++i)
            {
                ids.push_back("eigen(" + *i + ")");
            }
        }
    }
}

int RoadRunner::getSupportedIdTypes()
{
    int types = mModel ? mModel->getSupportedIdTypes() : 0;

    return types | SelectionRecord::ELASTICITY |
            SelectionRecord::UNSCALED_ELASTICITY |
            SelectionRecord::CONTROL |
            SelectionRecord::UNSCALED_CONTROL |
            SelectionRecord::EIGENVALUE;
}

vector<string> RoadRunner::getRateOfChangeIds()
{
    std::list<std::string> list;

    if (mModel) {
        mModel->getIds(SelectionRecord::FLOATING_AMOUNT_RATE, list);
    }

    return std::vector<std::string>(list.begin(), list.end());
}

vector<string> RoadRunner::getCompartmentIds()
{
    std::list<std::string> list;

    if (mModel) {
        mModel->getIds(SelectionRecord::COMPARTMENT, list);
    }

    return std::vector<std::string>(list.begin(), list.end());
}

vector<string> RoadRunner::getGlobalParameterIds()
{
    std::list<std::string> list;

    if (mModel) {
        mModel->getIds(SelectionRecord::GLOBAL_PARAMETER, list);
    }

    return std::vector<std::string>(list.begin(), list.end());
}

vector<string> RoadRunner::getBoundarySpeciesIds()
{
    std::list<std::string> list;

    if (mModel) {
        mModel->getIds(SelectionRecord::BOUNDARY_AMOUNT, list);
    }

    return std::vector<std::string>(list.begin(), list.end());
}

vector<string> RoadRunner::getConservedMoietyIds()
{
    return createModelStringList(mModel, &ExecutableModel::getNumConservedMoieties,
            &ExecutableModel::getConservedMoietyId);
}

vector<string> RoadRunner::getFloatingSpeciesIds()
{
    std::list<std::string> list;

    if (mModel) {
        mModel->getIds(SelectionRecord::FLOATING_AMOUNT, list);
    }

    return std::vector<std::string>(list.begin(), list.end());
}

vector<string> RoadRunner::getFloatingSpeciesInitialConditionIds()
{
    std::list<std::string> list;

    if (mModel) {
        mModel->getIds(SelectionRecord::INITIAL_FLOATING_CONCENTRATION, list);
    }

    return std::vector<std::string>(list.begin(), list.end());
}

vector<string> RoadRunner::getReactionIds()
{
    std::list<std::string> list;

    if (mModel) {
        mModel->getIds(SelectionRecord::REACTION_RATE, list);
    }

    return std::vector<std::string>(list.begin(), list.end());
}

vector<string> RoadRunner::getEigenvalueIds()
{
    std::list<std::string> list;

    getIds(SelectionRecord::EIGENVALUE, list);

    return std::vector<std::string>(list.begin(), list.end());
}


/************************ End Selection Ids Species Section *******************/
#endif /***********************************************************************/
/******************************************************************************/




}//namespace


#if defined(_WIN32)
#pragma comment(lib, "IPHLPAPI.lib") //Becuase of poco needing this
#endif



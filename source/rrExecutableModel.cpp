#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iostream>
#include "rrException.h"
#include "rrLogger.h"
#include "rrExecutableModel.h"
#include "rrCGenerator.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------
using namespace std;
namespace rr
{

ExecutableModel::ExecutableModel(CGenerator& generator, ModelSharedLibrary& dll)
:
mDummyInt(0),
mDummyDouble(0),
mDummyDoubleArray(new double[1]),
mData(),
mCG(generator),
mNOM(generator.mNOM),
mLibStruct(generator.mLibStruct),
mIsInitialized(false),
mDLL(dll)
{
	//Zero data structure..
	memset(&mData, 0, sizeof(ModelData));
    mDummyDoubleArray[0] = 1;

    if(mDLL.isLoaded())
    {
        setupDLLFunctions();
        setupModelData();
    }
    else
    {
    	Log(lError)<<"The Model DLL is not loaded in ExecutableModel ctor..";
    }
}

ExecutableModel::~ExecutableModel()
{
    delete [] mDummyDoubleArray;
}

string ExecutableModel::getModelName()
{
	return mData.modelName;
}

void ExecutableModel::assignCVodeInterface(CvodeInterface* cvodeI)
{
    mCvodeInterface = cvodeI;
}

void ExecutableModel::setTime(double _time)
{
    mData.time = _time;
}

double ExecutableModel::getTime()
{
	return mData.time;
}

/////////////////// The following used to be in IModel
int ExecutableModel::getNumIndependentVariables()
{
    return mData.numIndependentVariables;
}

int ExecutableModel::getNumDependentVariables()
{
    return mData.numDependentVariables;
}

int ExecutableModel::getNumTotalVariables()
{
    return mData.numTotalVariables;
}

int ExecutableModel::getNumBoundarySpecies()
{
    return mData.numBoundaryVariables;    //Todos: bad naming - is Variables/Species, choose one..
}

int ExecutableModel::getNumGlobalParameters()
{
    return mData.numGlobalParameters;
}

int ExecutableModel::getNumCompartments()
{
    return mData.numCompartments;
}

int ExecutableModel::getNumReactions()
{
    return mData.numReactions;
}

int ExecutableModel::getNumRules()
{
    return mData.numRules;
}

int ExecutableModel::getNumEvents()
{
    return mData.numEvents;
}

double ExecutableModel::getAmounts(const int& i)
{
    return (mData.amounts ) ? mData.amounts[i] : -1;
}

bool ExecutableModel::setupDLLFunctions()
{
    //Exported functions in the dll need to be assigned to a function pointer here..
    if(!mDLL.isLoaded())
    {
        Log(lError)<<"DLL handle not valid in SetupModel function";
        return false;
    }

    //Load functions..
    cInitModel                          = (c_int_MDS)         			   mDLL.getSymbol("InitModel");
    cInitModelData                      = (c_int_MDS)         			   mDLL.getSymbol("InitModelData");
    cinitializeInitialConditions        = (c_void_MDS)                     mDLL.getSymbol("initializeInitialConditions");
    csetParameterValues                 = (c_void_MDS)                     mDLL.getSymbol("setParameterValues");
    csetCompartmentVolumes              = (c_void_MDS)                     mDLL.getSymbol("setCompartmentVolumes");
    cgetNumLocalParameters              = (c_int_MDS_int)                  mDLL.getSymbol("getNumLocalParameters");
    csetBoundaryConditions              = (c_void_MDS)                     mDLL.getSymbol("setBoundaryConditions");
    csetInitialConditions               = (c_void_MDS)                     mDLL.getSymbol("setInitialConditions");
    cevalInitialAssignments             = (c_void_MDS)                     mDLL.getSymbol("evalInitialAssignments");
    ccomputeRules                       = (c_void_MDS_doubleStar)          mDLL.getSymbol("computeRules");
    cconvertToAmounts                   = (c_void_MDS)                     mDLL.getSymbol("convertToAmounts");
    ccomputeConservedTotals             = (c_void_MDS)                     mDLL.getSymbol("computeConservedTotals");
    cgetConcentration                   = (c_double_MDS_int)               mDLL.getSymbol("getConcentration");
    cGetCurrentValues                   = (c_doubleStar_MDS)               mDLL.getSymbol("GetCurrentValues");
    cevalModel                          = (c_void_MDS_double_doubleStar)   mDLL.getSymbol("__evalModel");
    cconvertToConcentrations            = (c_void_MDS)                     mDLL.getSymbol("convertToConcentrations");
    cevalEvents                         = (c_void_MDS_double_doubleStar)   mDLL.getSymbol("evalEvents");
    cupdateDependentSpeciesValues       = (c_void_MDS_doubleStar)          mDLL.getSymbol("updateDependentSpeciesValues");
    ccomputeAllRatesOfChange            = (c_void_MDS)                     mDLL.getSymbol("computeAllRatesOfChange");
    cAssignRates_a                      = (c_void_MDS)                     mDLL.getSymbol("AssignRatesA");
    cAssignRates_b                      = (c_void_MDS_doubleStar)          mDLL.getSymbol("AssignRatesB");
    ctestConstraints                    = (c_void_MDS)                     mDLL.getSymbol("testConstraints");
    cresetEvents                        = (c_void_MDS)                     mDLL.getSymbol("resetEvents");
    cInitializeRateRuleSymbols          = (c_void_MDS)                     mDLL.getSymbol("InitializeRateRuleSymbols");
    cInitializeRates                    = (c_void_MDS)                     mDLL.getSymbol("InitializeRates");
    csetConcentration                   = (c_void_MDS_int_double)          mDLL.getSymbol("setConcentration");
    cComputeReactionRates               = (c_void_MDS_double_doubleStar)   mDLL.getSymbol("computeReactionRates");
    ccomputeEventPriorities             = (c_void_MDS)                     mDLL.getSymbol("computeEventPriorities");
    return true;
}

bool ExecutableModel::setupModelData()
{
	//See CGenerator writeInitModelDataFunction to see how this initialization was done in the DLL before
	mData.modelName 					= createText(mNOM.getModelName());

    //Variables...
	mData.numIndependentVariables 		= mCG.mNumIndependentSpecies;
	mData.numDependentVariables   		= mCG.mNumDependentSpecies;
	mData.numTotalVariables				= mCG.mNumFloatingSpecies;        //???
    mData.numBoundaryVariables			= mCG.mNumBoundarySpecies;
    mData.numGlobalParameters			= mCG.mGlobalParameterList.size();
	mData.numCompartments				= mCG.mCompartmentList.size();
    mData.numReactions					= mCG.mReactionList.size();
    mData.numEvents						= mCG.mNumEvents;

	mData.amountsSize					= mCG.mFloatingSpeciesConcentrationList.Count();
    mData.amounts 						= new double[mData.amountsSize];

    mData.dydtSize						= mCG.mFloatingSpeciesConcentrationList.size();
	mData.dydt							= new double[mData.dydtSize];

    mData.rateRulesSize					= mCG.mMapRateRule.size();
	mData.rateRules						= new double[mData.rateRulesSize];

    mData.ySize							= mCG.mFloatingSpeciesConcentrationList.size();
    mData.y								= new double[mData.ySize];

    mData.ratesSize						= mCG.mNumReactions;
    mData.rates							= new double[mData.ratesSize];

    mData.ctSize						= mCG.mNumDependentSpecies;
    mData.ct							= new double[mData.ctSize];

    mData.init_ySize					= mCG.mFloatingSpeciesConcentrationList.Count();
    mData.init_y                        = new double[mData.init_ySize];

    mData.gpSize						=  mCG.mNumGlobalParameters + mCG.mTotalLocalParmeters;
    mData.gp							= new double[mData.gpSize];

    mData.cSize							= mCG.mNumCompartments;
    mData.c								= new double[mData.cSize];

    mData.bcSize						= mCG.mNumBoundarySpecies;
 	mData.bc							= new double[mData.bcSize];

    mData.lpSize 						= mCG.mNumReactions;
	mData.lp							= new double[mData.lpSize];

    mData.srSize  						= mCG.mNumModifiableSpeciesReferences;
    mData.sr							= new double[mData.srSize];

	mData.localParameterDimensionsSize	= mCG.mNumReactions;
	mData.localParameterDimensions		= new int[mData.localParameterDimensionsSize];

    mData.eventPrioritiesSize 			= mCG.mNumEvents;
    mData.eventPriorities				= new double[mData.eventPrioritiesSize];

    mData.eventStatusArraySize			= mCG.mNumEvents;
    mData.eventStatusArray				= new bool[mData.eventStatusArraySize];

    mData.previousEventStatusArraySize	= mCG.mNumEvents;
    mData.previousEventStatusArray		= new bool[mData.previousEventStatusArraySize];

    mData.eventPersistentTypeSize		= mCG.mNumEvents;
    mData.eventPersistentType			= new bool[mData.eventPersistentTypeSize];

    mData.eventTestsSize				= mCG.mNumEvents;
 	mData.eventTests					= new double[mData.eventTestsSize];

    mData.eventTypeSize					= mCG.mNumEvents;
    mData.eventType						= new bool[mData.eventTypeSize];

    if(mData.numEvents > 0)
    {
    //Event function pointer stuff
//    if(mDLL.hasSymbol("Get_eventAssignments"))
//    {
//        c_TEventAssignmentDelegateStar Get_eventAssignments;
//        Get_eventAssignments  	= (c_TEventAssignmentDelegateStar) mDLL.getSymbol("Get_eventAssignments");
//        mData.eventAssignments 		= Get_eventAssignments();
//        source<<Append("\tmd->eventAssignments = (TEventAssignmentDelegate*) malloc(sizeof(TEventAssignmentDelegate)*md->numEvents);" , NL());
        mData.eventAssignments 		= (TEventAssignmentDelegate*)  new TEventAssignmentDelegate[mData.numEvents];
//    }
//    else
//    {
//	    mData.eventAssignments = NULL;
//    }
//
//    if(mDLL.hasSymbol("Get_computeEventAssignments"))
//    {
//        c_TComputeEventAssignmentDelegateStar     Get_computeEventAssignments;
//        Get_computeEventAssignments      = (c_TComputeEventAssignmentDelegateStar) mDLL.getSymbol("Get_computeEventAssignments");
//        mData.computeEventAssignments = Get_computeEventAssignments();
//        source<<Append("\tmd->computeEventAssignments = (TComputeEventAssignmentDelegate*) malloc(sizeof(TComputeEventAssignmentDelegate)*md->numEvents);" , NL());
        mData.computeEventAssignments 	= (TComputeEventAssignmentDelegate*)  new TComputeEventAssignmentDelegate[mData.numEvents];
//    }
//    else
//    {
//		mData.computeEventAssignments = NULL;
//    }
//
//    if(mDLL.hasSymbol("Get_performEventAssignments"))
//    {
//     	c_TPerformEventAssignmentDelegateStar Get_performEventAssignments;
//    	Get_performEventAssignments  = (c_TPerformEventAssignmentDelegateStar) mDLL.getSymbol("Get_performEventAssignments");
//        mData.performEventAssignments = Get_performEventAssignments();
//        source<<Append("\tmd->performEventAssignments = (TPerformEventAssignmentDelegate*) malloc(sizeof(TPerformEventAssignmentDelegate)*md->numEvents);" , NL());
        mData.performEventAssignments = (TPerformEventAssignmentDelegate*)  new TPerformEventAssignmentDelegate[mData.numEvents];
//    }
//    else
//    {
//        mData.performEventAssignments = NULL;
//    }


//    if(mDLL.hasSymbol("GetEventDelays"))
//    {
//        c_GetEventDelayDelegatesStar GetEventDelays;
//        GetEventDelays  = (c_GetEventDelayDelegatesStar) mDLL.getSymbol("GetEventDelays");
//        mData.eventDelays = GetEventDelays();
//        mData.eventDelays =
		mData.eventDelays = new TEventDelayDelegate[mData.numEvents];
//    }
//    else
//    {
//		mData.eventDelays = NULL;
//    }
	}
    if(cInitModel)
    {
	    cInitModel(&mData);
    }
    return true;
}

void ExecutableModel::setCompartmentVolumes()
{
    if(!csetCompartmentVolumes)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetCompartmentVolumes(&mData);
}

void  ExecutableModel::setConcentration(int index, double value)
{
    if(!csetConcentration)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetConcentration(&mData, index, value);
}

void  ExecutableModel::computeReactionRates (double time, double* y)
{
	if(!cComputeReactionRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

	cComputeReactionRates(&mData, time, y);
}

vector<double> ExecutableModel::getCurrentValues()
{
    vector<double> vals;
    if(!cGetCurrentValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return vals;
    }

    double* values = cGetCurrentValues(&mData);     //The size of this double* is mMapRateRule.size(); ??

    int count = mCG.numAdditionalRates();
    if(values)
    {
        for(int i = 0; i < count; i++)
        {
            vals.push_back(values[i]);
        }
    }

    return vals;
}

double ExecutableModel::getConcentration(int index)
{
    if(!cgetConcentration)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return 0;
    }

    return cgetConcentration(&mData, index);
}

int ExecutableModel::getNumLocalParameters(int reactionId)
{
    if(!cgetNumLocalParameters)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return 0;
    }

    return cgetNumLocalParameters(&mData, reactionId);
}

void ExecutableModel::initializeInitialConditions()
{
    if(!cinitializeInitialConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cinitializeInitialConditions(&mData);
}

//void ExecutableModel::setInitialConditions(){}
void ExecutableModel::setParameterValues()
{
    if(!csetParameterValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    csetParameterValues(&mData);
}

void ExecutableModel::setBoundaryConditions()
{
    if(!csetBoundaryConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    csetBoundaryConditions(&mData);
}

void ExecutableModel::initializeRates()
{
    if(!cInitializeRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cInitializeRates(&mData);
}

void ExecutableModel::assignRates()
{
    if(!cAssignRates_a)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cAssignRates_a(&mData);
}

void ExecutableModel::assignRates(vector<double>& _rates)
{
    if(!cAssignRates_b)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    double* local_rates = CreateVector(_rates);

    cAssignRates_b(&mData, local_rates);
    delete [] local_rates;
}

void ExecutableModel::computeConservedTotals()
{
    if(!ccomputeConservedTotals)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeConservedTotals(&mData);
}

void ExecutableModel::computeEventPriorites()
{
    if(!ccomputeEventPriorities)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeEventPriorities(&mData);
}

void ExecutableModel::convertToAmounts()
{
    if(!cconvertToAmounts)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cconvertToAmounts(&mData);
}

void ExecutableModel::convertToConcentrations()
{
    if(!cconvertToConcentrations)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cconvertToConcentrations(&mData);
}

void ExecutableModel::updateDependentSpeciesValues(double* y_vec)
{
    if(!cupdateDependentSpeciesValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cupdateDependentSpeciesValues(&mData, y_vec);
}

void ExecutableModel::computeRules(vector<double>& arr)
{
    double* cArr = CreateVector(arr);
    computeRules(cArr, arr.size());
    delete [] cArr;

}
void ExecutableModel::computeRules(double* y, int size)
{
    if(!ccomputeRules)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    ccomputeRules(&mData, y);
}

void ExecutableModel::setInitialConditions()
{
    if(!csetInitialConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetInitialConditions(&mData);
}

//void ExecutableModel::computeReactionRates(double time, vector<double>& y){}
void ExecutableModel::computeAllRatesOfChange()
{
    if(!ccomputeAllRatesOfChange)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeAllRatesOfChange(&mData);
}

void ExecutableModel::evalModel(const double& timein, const vector<double>& y)
{
    if(!cevalModel)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

	double *oAmounts = CreateVector(y);
    cevalModel(&mData, timein, oAmounts);
    delete [] oAmounts;
}

void ExecutableModel::evalEvents(const double& timeIn, const vector<double>& y)
{
    if(!cevalEvents)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
	double *oAmounts = CreateVector(y);
    cevalEvents(&mData, timeIn, oAmounts);
    delete [] oAmounts;
}

void ExecutableModel::resetEvents()
{
    if(!cresetEvents)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cresetEvents(&mData);
}

void ExecutableModel::evalInitialAssignments()
{
    if(!cevalInitialAssignments)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cevalInitialAssignments(&mData);
}

void ExecutableModel::testConstraints()
{
    if(!ctestConstraints)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        throw(Exception("Problem in testConstraints"));
    }

    ctestConstraints(&mData);
}

void ExecutableModel::initializeRateRuleSymbols()
{
    if(!cInitializeRateRuleSymbols)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cInitializeRateRuleSymbols(&mData);
}

} //Namespace rr

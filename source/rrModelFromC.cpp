#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iostream>
#include "rrLogger.h"
#include "rrModelFromC.h"
#include "rrCGenerator.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------
using namespace std;
namespace rr
{

ModelFromC::ModelFromC(CGenerator* generator, ModelSharedLibrary& dll)
:
mDummyInt(0),
mDummyDouble(0),
mDummyDoubleArray(new double[1]),
numIndependentVariables(&mDummyInt),
numDependentVariables(&mDummyInt),
numTotalVariables(&mDummyInt),
numBoundaryVariables(&mDummyInt),
numGlobalParameters(&mDummyInt),
numCompartments(&mDummyInt),
numReactions(&mDummyInt),
numRules(&mDummyInt),
numEvents(&mDummyInt),
mModelName("NoNameSet"),
time(0),
mCodeGenerator(generator),
mIsInitialized(false),
mDLL(dll)
{
    mDummyDoubleArray[0] = 1;

    if(mDLL.isLoaded())
    {
        setupDLLFunctions();
        setupDLLData();
    }
}

ModelFromC::~ModelFromC()
{
    delete [] mDummyDoubleArray;
}

void ModelFromC::assignCVodeInterface(CvodeInterface* cvodeI)
{
    mCvodeInterface = cvodeI;
}

void ModelFromC::setTime(double _time)
{
    *time = _time;
}

double ModelFromC::getTime()
{
	return *time;
}

/////////////////// The following used to be in IModel
int ModelFromC::getNumIndependentVariables()
{
    return *numIndependentVariables;
}

int ModelFromC::getNumDependentVariables()
{
    return *numDependentVariables;
}

int ModelFromC::getNumTotalVariables()
{
    return *numTotalVariables;
}

int ModelFromC::getNumBoundarySpecies()
{
    return *numBoundaryVariables;    //Todos: bad naming - is Variables/Species, choose one..
}

int ModelFromC::getNumGlobalParameters()
{
    return *numGlobalParameters;
}

int ModelFromC::getNumCompartments()
{
    return *numCompartments;
}

int ModelFromC::getNumReactions()
{
    return *numReactions;
}

int ModelFromC::getNumRules()
{
    return *numRules;
}

int ModelFromC::getNumEvents()
{
    return *numEvents;
}

double ModelFromC::getAmounts(const int& i)
{
    return (amounts ) ? amounts[i] : -1;
}

bool ModelFromC::setupDLLFunctions()
{
    //Exported functions in the dll need to be assigned to a function pointer here..
    if(!mDLL.isLoaded())
    {
        Log(lError)<<"DLL handle not valid in SetupModel function";
        return false;
    }

    //Load functions..
    cInitModel                          = (c_int)                      mDLL.getSymbol("InitModel");//GetFunctionPtr("InitModel");
    cGetModelName                       = (c_charStar)                 mDLL.getSymbol("GetModelName");
    cinitializeInitialConditions        = (c_void)                     mDLL.getSymbol("initializeInitialConditions");
    csetParameterValues                 = (c_void)                     mDLL.getSymbol("setParameterValues");
    csetCompartmentVolumes              = (c_void)                     mDLL.getSymbol("setCompartmentVolumes");
    cgetNumLocalParameters              = (c_int_int)                  mDLL.getSymbol("getNumLocalParameters");
    csetBoundaryConditions              = (c_void)                     mDLL.getSymbol("setBoundaryConditions");
    csetInitialConditions               = (c_void)                     mDLL.getSymbol("setInitialConditions");
    cevalInitialAssignments             = (c_void)                     mDLL.getSymbol("evalInitialAssignments");
    ccomputeRules                       = (c_void_doubleStar)          mDLL.getSymbol("computeRules");
    cconvertToAmounts                   = (c_void)                     mDLL.getSymbol("convertToAmounts");
    ccomputeConservedTotals             = (c_void)                     mDLL.getSymbol("computeConservedTotals");
    cgetConcentration                   = (c_double_int)               mDLL.getSymbol("getConcentration");
    cGetCurrentValues                   = (c_doubleStar)               mDLL.getSymbol("GetCurrentValues");
    cevalModel                          = (c_void_double_doubleStar)   mDLL.getSymbol("__evalModel");
    cconvertToConcentrations            = (c_void)                     mDLL.getSymbol("convertToConcentrations");
    cevalEvents                         = (c_void_double_doubleStar)   mDLL.getSymbol("evalEvents");
    cupdateDependentSpeciesValues       = (c_void_doubleStar)          mDLL.getSymbol("updateDependentSpeciesValues");
    ccomputeAllRatesOfChange            = (c_void)                     mDLL.getSymbol("computeAllRatesOfChange");
    cAssignRates_a                      = (c_void)                     mDLL.getSymbol("AssignRatesA");
    cAssignRates_b                      = (c_void_doubleStar)          mDLL.getSymbol("AssignRatesB");
    ctestConstraints                    = (c_void)                     mDLL.getSymbol("testConstraints");
    cresetEvents                        = (c_void)                     mDLL.getSymbol("resetEvents");
    cInitializeRateRuleSymbols          = (c_void)                     mDLL.getSymbol("InitializeRateRuleSymbols");
    cInitializeRates                    = (c_void)                     mDLL.getSymbol("InitializeRates");
    csetConcentration                   = (c_void_int_double)          mDLL.getSymbol("setConcentration");
    cComputeReactionRates               = (c_void_double_doubleStar)   mDLL.getSymbol("computeReactionRates");
    ccomputeEventPriorities             = (c_void)                     mDLL.getSymbol("computeEventPriorities");
    return true;
}

bool ModelFromC::setupDLLData()
{
    if(!cInitModel)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return false;
    }

    //This setup up data in the DLL...
    if(cInitModel() != 0)
    {
        Log(lError)<<"Failed to InitModel in "<<__FUNCTION__;
        return false;
    }

    if(!cGetModelName)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return false;
    }

    char* modelName = cGetModelName();
    if(modelName)
    {
        mModelName = modelName;
    }

    //Simple variables...
    numIndependentVariables         = mDLL.hasSymbol("numIndependentVariables") ?   (int*)      mDLL.getSymbol("numIndependentVariables")   : NULL;
    numDependentVariables 	        = mDLL.hasSymbol("numDependentVariables")   ?   (int*)      mDLL.getSymbol("numDependentVariables")     : NULL;
    numTotalVariables 		        = mDLL.hasSymbol("numTotalVariables")       ?   (int*)      mDLL.getSymbol("numTotalVariables")         : NULL;
    numBoundaryVariables 	        = mDLL.hasSymbol("numBoundaryVariables")    ?   (int*)      mDLL.getSymbol("numBoundaryVariables")      : NULL;
    numGlobalParameters 	        = mDLL.hasSymbol("numGlobalParameters")     ?   (int*)      mDLL.getSymbol("numGlobalParameters")       : NULL;
    numCompartments 		        = mDLL.hasSymbol("numCompartments")         ?   (int*)      mDLL.getSymbol("numCompartments")           : NULL;
    numReactions 			        = mDLL.hasSymbol("numReactions")            ?   (int*)      mDLL.getSymbol("numReactions")              : NULL;
    numEvents 				        = mDLL.hasSymbol("numEvents")               ?   (int*)      mDLL.getSymbol("numEvents")                 : NULL;
    amounts  				        = mDLL.hasSymbol("_amounts")                ?   (double*) 	mDLL.getSymbol("_amounts")                  : NULL;
    amountsSize  			        = mDLL.hasSymbol("_amountsSize")            ?   (int*) 	    mDLL.getSymbol("_amountsSize")              : NULL;
    dydt  					        = mDLL.hasSymbol("_dydt")                   ?   (double*) 	mDLL.getSymbol("_dydt")                     : NULL;
    dydtSize  				        = mDLL.hasSymbol("_dydtSize")               ?   (int*) 	    mDLL.getSymbol("_dydtSize")                 : NULL;
    rateRules  				        = mDLL.hasSymbol("_rateRules")              ?   (double*) 	mDLL.getSymbol("_rateRules")                : NULL;
    rateRulesSize 			        = mDLL.hasSymbol("_rateRulesSize")          ?   (int*) 	    mDLL.getSymbol("_rateRulesSize")            : NULL;
    y  						        = mDLL.hasSymbol("_y")                      ?   (double*) 	mDLL.getSymbol("_y")                        : NULL;
    ySize  					        = mDLL.hasSymbol("_ySize")                  ?   (int*) 	    mDLL.getSymbol("_ySize")                    : NULL;
    rates  					        = mDLL.hasSymbol("_rates")                  ?   (double*) 	mDLL.getSymbol("_rates")                    : NULL;
    ratesSize  				        = mDLL.hasSymbol("_ratesSize")              ?   (int*) 	    mDLL.getSymbol("_ratesSize")                : NULL;
    ct  					        = mDLL.hasSymbol("_ct")                     ?   (double*) 	mDLL.getSymbol("_ct")                       : NULL;
    ctSize  				        = mDLL.hasSymbol("_ctSize")                 ?   (int*) 	    mDLL.getSymbol("_ctSize")                   : NULL;
    time       				        = mDLL.hasSymbol("mTime")                   ?   (double*) 	mDLL.getSymbol("mTime")                     : NULL;
    init_y       			        = mDLL.hasSymbol("_init_y")                 ?   (double*) 	mDLL.getSymbol("_init_y")                   : NULL;
    init_ySize       		        = mDLL.hasSymbol("_init_ySize")             ?   (int*) 	    mDLL.getSymbol("_init_ySize")               : NULL;
    gp       				        = mDLL.hasSymbol("_gp")                     ?   (double*) 	mDLL.getSymbol("_gp")                       : NULL;
    gpSize       			        = mDLL.hasSymbol("_gpSize")                 ?   (int*) 	    mDLL.getSymbol("_gpSize")                   : NULL;
    c       				        = mDLL.hasSymbol("_c")                      ?   (double*) 	mDLL.getSymbol("_c")                        : NULL;
    cSize       			        = mDLL.hasSymbol("_cSize")                  ?   (int*) 	    mDLL.getSymbol("_cSize")                    : NULL;
    bc       				        = mDLL.hasSymbol("_bc")                     ?   (double*) 	mDLL.getSymbol("_bc")                       : NULL;
    bcSize       			        = mDLL.hasSymbol("_bcSize")                 ?   (int*) 	    mDLL.getSymbol("_bcSize")                   : NULL;
    lp       				        = mDLL.hasSymbol("_lp")						?	(double*) 	mDLL.getSymbol("_lp")						: mDummyDoubleArray;
    lpSize       			        = mDLL.hasSymbol("_lpSize") 	                   ?	(int*) 		mDLL.getSymbol("_lpSize") 	        : &mDummyInt;
    sr       				        = mDLL.hasSymbol("_sr")  		                   ?	(double*) 	mDLL.getSymbol("_sr")  		        : mDummyDoubleArray;
    srSize       			        = mDLL.hasSymbol("_srSize") 	                   ?	(int*)	    mDLL.getSymbol("_srSize") 	        : &mDummyInt;
    eventPriorities   		        = mDLL.hasSymbol("_eventPriorities")               ?	(double*) 	mDLL.getSymbol("_eventPriorities") 	: NULL;
    eventStatusArray   		        = mDLL.hasSymbol("mEventStatusArray")              ?	(bool*) 	mDLL.getSymbol("mEventStatusArray")	: NULL;
    eventStatusArraySize            = mDLL.hasSymbol("mEventStatusArraySize")          ?	(int*) 		mDLL.getSymbol("mEventStatusArraySize") 	: NULL;
    previousEventStatusArray        = mDLL.hasSymbol("_previousEventStatusArray")      ?	(bool*) 	mDLL.getSymbol("_previousEventStatusArray") : NULL;
    previousEventStatusArraySize    = mDLL.hasSymbol("_previousEventStatusArraySize")  ?	(int*) 		mDLL.getSymbol("_previousEventStatusArraySize") :  &mDummyInt;
    eventPersistentType   			= mDLL.hasSymbol("_eventPersistentType")           ?	(bool*) 	mDLL.getSymbol("_eventPersistentType") 		: NULL;
    eventPersistentTypeSize      	= mDLL.hasSymbol("_eventPersistentTypeSize")       ?	(int*) 		mDLL.getSymbol("_eventPersistentTypeSize") 	: NULL;
    eventTests   					= mDLL.hasSymbol("mEventTests")                    ?	(double*) 	mDLL.getSymbol("mEventTests") 				: NULL;
    eventTestsSize      			= mDLL.hasSymbol("mEventTestsSize") 			   ?	(int*) 		mDLL.getSymbol("mEventTestsSize") 			: &mDummyInt;
    eventType   					= mDLL.hasSymbol("_eventType")                     ?	(bool*) 	mDLL.getSymbol("_eventType")                : NULL;
    eventTypeSize      				= mDLL.hasSymbol("_eventTypeSize")				   ?	(int*) 		mDLL.getSymbol("_eventTypeSize") 			: &mDummyInt;

    //Event function pointer stuff
    if(mDLL.hasSymbol("Get_eventAssignments"))
    {
        c_TEventAssignmentDelegateStar Get_eventAssignments;
        Get_eventAssignments  	= (c_TEventAssignmentDelegateStar) mDLL.getSymbol("Get_eventAssignments");
        eventAssignments 		= Get_eventAssignments();
    }
    else
    {
	    eventAssignments = NULL;
    }

    if(mDLL.hasSymbol("Get_computeEventAssignments"))
    {
        c_TComputeEventAssignmentDelegateStar     Get_computeEventAssignments;
        Get_computeEventAssignments      = (c_TComputeEventAssignmentDelegateStar) mDLL.getSymbol("Get_computeEventAssignments");
        computeEventAssignments = Get_computeEventAssignments();
    }
    else
    {
		computeEventAssignments = NULL;
    }

    if(mDLL.hasSymbol("Get_performEventAssignments"))
    {
     	c_TPerformEventAssignmentDelegateStar Get_performEventAssignments;
    	Get_performEventAssignments  = (c_TPerformEventAssignmentDelegateStar) mDLL.getSymbol("Get_performEventAssignments");
        performEventAssignments = Get_performEventAssignments();
    }
    else
    {
        performEventAssignments = NULL;
    }

    if(mDLL.hasSymbol("GetEventDelays"))
    {
        c_GetEventDelayDelegatesStar GetEventDelays;
        GetEventDelays  = (c_GetEventDelayDelegatesStar) mDLL.getSymbol("GetEventDelays");
        eventDelays = GetEventDelays();
    }
    else
    {
		eventDelays = NULL;
    }

    return true;
}

void ModelFromC::setCompartmentVolumes()
{
    if(!csetCompartmentVolumes)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetCompartmentVolumes();
}

void  ModelFromC::setConcentration(int index, double value)
{
    if(!csetConcentration)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetConcentration(index, value);
}

void  ModelFromC::computeReactionRates (double time, double* y)
{
	if(!cComputeReactionRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

	cComputeReactionRates (time, y);
}

vector<double> ModelFromC::getCurrentValues()
{
    vector<double> vals;
    if(!cGetCurrentValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return vals;
    }

    double* values = cGetCurrentValues();     //The size of this double* is mMapRateRule.size(); ??

    int count = mCodeGenerator->NumAdditionalRates();
    if(values)
    {
        for(int i = 0; i < count; i++)
        {
            vals.push_back(values[i]);
        }
    }

    return vals;
}

double ModelFromC::getConcentration(int index)
{
    if(!cgetConcentration)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return 0;
    }

    return cgetConcentration(index);
}

int ModelFromC::getNumLocalParameters(int reactionId)
{
    if(!cgetNumLocalParameters)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return 0;
    }

    return cgetNumLocalParameters(reactionId);
}

void ModelFromC::initializeInitialConditions()
{
    if(!cinitializeInitialConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cinitializeInitialConditions();
}

//void ModelFromC::setInitialConditions(){}
void ModelFromC::setParameterValues()
{
    if(!csetParameterValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    csetParameterValues();
}

void ModelFromC::setBoundaryConditions()
{
    if(!csetBoundaryConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    csetBoundaryConditions();
}

void ModelFromC::initializeRates()
{
    if(!cInitializeRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cInitializeRates();
}

void ModelFromC::assignRates()
{
    if(!cAssignRates_a)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cAssignRates_a();
}

void ModelFromC::assignRates(vector<double>& _rates)
{
    if(!cAssignRates_b)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    double* local_rates = CreateVector(_rates);

    cAssignRates_b(local_rates);
    delete [] local_rates;
}

void ModelFromC::computeConservedTotals()
{
    if(!ccomputeConservedTotals)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeConservedTotals();
}

void ModelFromC::computeEventPriorites()
{
    if(!ccomputeEventPriorities)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeEventPriorities();
}

void ModelFromC::convertToAmounts()
{
    if(!cconvertToAmounts)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cconvertToAmounts();
}

void ModelFromC::convertToConcentrations()
{
    if(!cconvertToConcentrations)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cconvertToConcentrations();
}

void ModelFromC::updateDependentSpeciesValues(double* y_vec)
{
    if(!cupdateDependentSpeciesValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cupdateDependentSpeciesValues(y_vec);
}

void ModelFromC::computeRules(vector<double>& arr)
{
    double* cArr = CreateVector(arr);
    computeRules(cArr, arr.size());
    delete [] cArr;

}
void ModelFromC::computeRules(double* y, int size)
{
    if(!ccomputeRules)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    ccomputeRules(y);
}

void ModelFromC::setInitialConditions()
{
    if(!csetInitialConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetInitialConditions();
}

//void ModelFromC::computeReactionRates(double time, vector<double>& y){}
void ModelFromC::computeAllRatesOfChange()
{
    if(!ccomputeAllRatesOfChange)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeAllRatesOfChange();
}

void ModelFromC::evalModel(const double& timein, const vector<double>& y)
{
    if(!cevalModel)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

	double *oAmounts = CreateVector(y);
    cevalModel(timein, oAmounts);
    delete [] oAmounts;
}

void ModelFromC::evalEvents(const double& timeIn, const vector<double>& y)
{
    if(!cevalEvents)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
	double *oAmounts = CreateVector(y);
    cevalEvents(timeIn, oAmounts);
    delete [] oAmounts;
}

void ModelFromC::resetEvents()
{
    if(!cresetEvents)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cresetEvents();
}

void ModelFromC::evalInitialAssignments()
{
    if(!cevalInitialAssignments)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cevalInitialAssignments();
}

void ModelFromC::testConstraints()
{
    if(!ctestConstraints)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    ctestConstraints();
}

void ModelFromC::initializeRateRuleSymbols()
{
    if(!cInitializeRateRuleSymbols)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cInitializeRateRuleSymbols();
}

} //Namespace rr

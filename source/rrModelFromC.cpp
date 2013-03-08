#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iostream>
#include "rrException.h"
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
//numIndependentVariables(&mDummyInt),
//numDependentVariables(&mDummyInt),
//numTotalVariables(&mDummyInt),
//numBoundaryVariables(&mDummyInt),
//numGlobalParameters(&mDummyInt),
//numCompartments(&mDummyInt),
//numReactions(&mDummyInt),
//numRules(&mDummyInt),
//numEvents(&mDummyInt),
mData(),
time(0),
mCodeGenerator(generator),
mIsInitialized(false),
mDLL(dll)
{
	memset(&mData, 0, sizeof(ModelData));
    mDummyDoubleArray[0] = 1;

    if(mDLL.isLoaded())
    {
        setupDLLFunctions();
        setupModelData();
    }
    else
    {
    	Log(lError)<<"The Model DLL is not loaded in ModelFromC ctor..";
    }
}

ModelFromC::~ModelFromC()
{
    delete [] mDummyDoubleArray;
}

string ModelFromC::getModelName()
{
	return mData.modelName;
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
    return mData.numIndependentVariables;
}

int ModelFromC::getNumDependentVariables()
{
    return mData.numDependentVariables;
}

int ModelFromC::getNumTotalVariables()
{
    return mData.numTotalVariables;
}

int ModelFromC::getNumBoundarySpecies()
{
    return mData.numBoundaryVariables;    //Todos: bad naming - is Variables/Species, choose one..
}

int ModelFromC::getNumGlobalParameters()
{
    return mData.numGlobalParameters;
}

int ModelFromC::getNumCompartments()
{
    return mData.numCompartments;
}

int ModelFromC::getNumReactions()
{
    return mData.numReactions;
}

int ModelFromC::getNumRules()
{
    return mData.numRules;
}

int ModelFromC::getNumEvents()
{
    return mData.numEvents;
}

double ModelFromC::getAmounts(const int& i)
{
    return (mData.amounts ) ? mData.amounts[i] : -1;
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
    cInitModel                          = (c_int_MDS)         mDLL.getSymbol("InitModel");
//    cgetModelName                       = (c_charStar_MDS)        mDLL.getSymbol("getModelName");
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

bool ModelFromC::setupModelData()
{
//    if(!cInitModel)
//    {
//        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
//        return false;
//    }
//
//    //This setup up data in the DLL...
//    if(cInitModel(&mData) != 0)
//    {
//        Log(lError)<<"Failed to InitModel in "<<__FUNCTION__;
//        return false;
//    }
//
//    if(!cgetModelName)
//    {
//        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
//        return false;
//    }

    //Simple variables...
//    numIndependentVariables         = mDLL.hasSymbol("numIndependentVariables")        ?   (int*)       mDLL.getSymbol("numIndependentVariables")       : NULL;
//    numDependentVariables 	        = mDLL.hasSymbol("numDependentVariables")          ?   (int*)       mDLL.getSymbol("numDependentVariables")         : NULL;
//    numTotalVariables 		        = mDLL.hasSymbol("numTotalVariables")              ?   (int*)       mDLL.getSymbol("numTotalVariables")             : NULL;
//    numBoundaryVariables 	        = mDLL.hasSymbol("numBoundaryVariables")           ?   (int*)       mDLL.getSymbol("numBoundaryVariables")          : NULL;
//    numGlobalParameters 	        = mDLL.hasSymbol("numGlobalParameters")            ?   (int*)       mDLL.getSymbol("numGlobalParameters")           : NULL;
//    numCompartments 		        = mDLL.hasSymbol("numCompartments")                ?   (int*)       mDLL.getSymbol("numCompartments")               : NULL;
//    numReactions 			        = mDLL.hasSymbol("numReactions")                   ?   (int*)       mDLL.getSymbol("numReactions")                  : NULL;
//    numEvents 				        = mDLL.hasSymbol("numEvents")                      ?   (int*)       mDLL.getSymbol("numEvents")                     : NULL;
//    amounts  				        = mDLL.hasSymbol("_amounts")                       ?   (double*) 	mDLL.getSymbol("_amounts")                      : NULL;
//    amountsSize  			        = mDLL.hasSymbol("_amountsSize")                   ?   (int*) 	    mDLL.getSymbol("_amountsSize")                  : NULL;
//    dydt  					        = mDLL.hasSymbol("_dydt")                          ?   (double*) 	mDLL.getSymbol("_dydt")                         : NULL;
//    dydtSize  				        = mDLL.hasSymbol("_dydtSize")                      ?   (int*) 	    mDLL.getSymbol("_dydtSize")                     : NULL;
//    rateRules  				        = mDLL.hasSymbol("_rateRules")                     ?   (double*) 	mDLL.getSymbol("_rateRules")                    : NULL;
//    rateRulesSize 			        = mDLL.hasSymbol("_rateRulesSize")                 ?   (int*) 	    mDLL.getSymbol("_rateRulesSize")                : NULL;
//    y  						        = mDLL.hasSymbol("_y")                             ?   (double*) 	mDLL.getSymbol("_y")                            : NULL;
//    ySize  					        = mDLL.hasSymbol("_ySize")                         ?   (int*) 	    mDLL.getSymbol("_ySize")                        : NULL;
//    rates  					        = mDLL.hasSymbol("_rates")                         ?   (double*) 	mDLL.getSymbol("_rates")                        : NULL;
//    ratesSize  				        = mDLL.hasSymbol("_ratesSize")                     ?   (int*) 	    mDLL.getSymbol("_ratesSize")                    : NULL;
//    ct  					        = mDLL.hasSymbol("_ct")                            ?   (double*) 	mDLL.getSymbol("_ct")                           : NULL;
//    ctSize  				        = mDLL.hasSymbol("_ctSize")                        ?   (int*) 	    mDLL.getSymbol("_ctSize")                       : NULL;
//    time       				        = mDLL.hasSymbol("mTime")                          ?   (double*) 	mDLL.getSymbol("mTime")                         : NULL;
//    init_y       			        = mDLL.hasSymbol("_init_y")                        ?   (double*) 	mDLL.getSymbol("_init_y")                       : NULL;
//    init_ySize       		        = mDLL.hasSymbol("_init_ySize")                    ?   (int*) 	    mDLL.getSymbol("_init_ySize")                   : NULL;
//    gp       				        = mDLL.hasSymbol("_gp")                            ?   (double*) 	mDLL.getSymbol("_gp")                           : NULL;
//    gpSize       			        = mDLL.hasSymbol("_gpSize")                        ?   (int*) 	    mDLL.getSymbol("_gpSize")                       : NULL;
//    c       				        = mDLL.hasSymbol("_c")                             ?   (double*) 	mDLL.getSymbol("_c")                            : NULL;
//    cSize       			        = mDLL.hasSymbol("_cSize")                         ?   (int*) 	    mDLL.getSymbol("_cSize")                        : NULL;
//    bc       				        = mDLL.hasSymbol("_bc")                            ?   (double*) 	mDLL.getSymbol("_bc")                           : NULL;
//    bcSize       			        = mDLL.hasSymbol("_bcSize")                        ?   (int*) 	    mDLL.getSymbol("_bcSize")                       : NULL;
//    lp       				        = mDLL.hasSymbol("_lp")						       ?   (double*) 	mDLL.getSymbol("_lp")						    : mDummyDoubleArray;
//    lpSize       			        = mDLL.hasSymbol("_lpSize") 	                   ?   (int*) 		mDLL.getSymbol("_lpSize") 	                    : &mDummyInt;
//    sr       				        = mDLL.hasSymbol("_sr")  		                   ?   (double*) 	mDLL.getSymbol("_sr")  		                    : mDummyDoubleArray;
//    srSize       			        = mDLL.hasSymbol("_srSize") 	                   ?   (int*)	    mDLL.getSymbol("_srSize") 	                    : &mDummyInt;
//    eventPriorities   		        = mDLL.hasSymbol("_eventPriorities")               ?   (double*) 	mDLL.getSymbol("_eventPriorities") 	            : NULL;
//    eventStatusArray   		        = mDLL.hasSymbol("mEventStatusArray")              ?   (bool*) 		mDLL.getSymbol("mEventStatusArray")	            : NULL;
//    eventStatusArraySize            = mDLL.hasSymbol("mEventStatusArraySize")          ?   (int*) 		mDLL.getSymbol("mEventStatusArraySize") 	    : NULL;
//    previousEventStatusArray        = mDLL.hasSymbol("_previousEventStatusArray")      ?   (bool*) 		mDLL.getSymbol("_previousEventStatusArray")     : NULL;
//    previousEventStatusArraySize    = mDLL.hasSymbol("_previousEventStatusArraySize")  ?   (int*) 		mDLL.getSymbol("_previousEventStatusArraySize") :  &mDummyInt;
//    eventPersistentType   			= mDLL.hasSymbol("_eventPersistentType")           ?   (bool*) 		mDLL.getSymbol("_eventPersistentType") 		    : NULL;
//    eventPersistentTypeSize      	= mDLL.hasSymbol("_eventPersistentTypeSize")       ?   (int*) 		mDLL.getSymbol("_eventPersistentTypeSize") 	    : NULL;
//    eventTests   					= mDLL.hasSymbol("mEventTests")                    ?   (double*) 	mDLL.getSymbol("mEventTests") 				    : NULL;
//    eventTestsSize      			= mDLL.hasSymbol("mEventTestsSize") 			   ?   (int*) 		mDLL.getSymbol("mEventTestsSize") 			    : &mDummyInt;
//    eventType   					= mDLL.hasSymbol("_eventType")                     ?   (bool*) 		mDLL.getSymbol("_eventType")                    : NULL;
//    eventTypeSize      				= mDLL.hasSymbol("_eventTypeSize")				   ?   (int*) 		mDLL.getSymbol("_eventTypeSize") 			    : &mDummyInt;

    //Event function pointer stuff
    if(mDLL.hasSymbol("Get_eventAssignments"))
    {
        c_TEventAssignmentDelegateStar Get_eventAssignments;
        Get_eventAssignments  	= (c_TEventAssignmentDelegateStar) mDLL.getSymbol("Get_eventAssignments");
        mData.eventAssignments 		= Get_eventAssignments();
    }
    else
    {
	    mData.eventAssignments = NULL;
    }

    if(mDLL.hasSymbol("Get_computeEventAssignments"))
    {
        c_TComputeEventAssignmentDelegateStar     Get_computeEventAssignments;
        Get_computeEventAssignments      = (c_TComputeEventAssignmentDelegateStar) mDLL.getSymbol("Get_computeEventAssignments");
        mData.computeEventAssignments = Get_computeEventAssignments();
    }
    else
    {
		mData.computeEventAssignments = NULL;
    }

    if(mDLL.hasSymbol("Get_performEventAssignments"))
    {
     	c_TPerformEventAssignmentDelegateStar Get_performEventAssignments;
    	Get_performEventAssignments  = (c_TPerformEventAssignmentDelegateStar) mDLL.getSymbol("Get_performEventAssignments");
        mData.performEventAssignments = Get_performEventAssignments();
    }
    else
    {
        mData.performEventAssignments = NULL;
    }

    if(mDLL.hasSymbol("GetEventDelays"))
    {
        c_GetEventDelayDelegatesStar GetEventDelays;
        GetEventDelays  = (c_GetEventDelayDelegatesStar) mDLL.getSymbol("GetEventDelays");
        mData.eventDelays = GetEventDelays();
    }
    else
    {
		mData.eventDelays = NULL;
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

    csetCompartmentVolumes(&mData);
}

void  ModelFromC::setConcentration(int index, double value)
{
    if(!csetConcentration)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetConcentration(&mData, index, value);
}

void  ModelFromC::computeReactionRates (double time, double* y)
{
	if(!cComputeReactionRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

	cComputeReactionRates(&mData, time, y);
}

vector<double> ModelFromC::getCurrentValues()
{
    vector<double> vals;
    if(!cGetCurrentValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return vals;
    }

    double* values = cGetCurrentValues(&mData);     //The size of this double* is mMapRateRule.size(); ??

    int count = mCodeGenerator->numAdditionalRates();
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

    return cgetConcentration(&mData, index);
}

int ModelFromC::getNumLocalParameters(int reactionId)
{
    if(!cgetNumLocalParameters)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return 0;
    }

    return cgetNumLocalParameters(&mData, reactionId);
}

void ModelFromC::initializeInitialConditions()
{
    if(!cinitializeInitialConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cinitializeInitialConditions(&mData);
}

//void ModelFromC::setInitialConditions(){}
void ModelFromC::setParameterValues()
{
    if(!csetParameterValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    csetParameterValues(&mData);
}

void ModelFromC::setBoundaryConditions()
{
    if(!csetBoundaryConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    csetBoundaryConditions(&mData);
}

void ModelFromC::initializeRates()
{
    if(!cInitializeRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cInitializeRates(&mData);
}

void ModelFromC::assignRates()
{
    if(!cAssignRates_a)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cAssignRates_a(&mData);
}

void ModelFromC::assignRates(vector<double>& _rates)
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

void ModelFromC::computeConservedTotals()
{
    if(!ccomputeConservedTotals)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeConservedTotals(&mData);
}

void ModelFromC::computeEventPriorites()
{
    if(!ccomputeEventPriorities)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeEventPriorities(&mData);
}

void ModelFromC::convertToAmounts()
{
    if(!cconvertToAmounts)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cconvertToAmounts(&mData);
}

void ModelFromC::convertToConcentrations()
{
    if(!cconvertToConcentrations)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cconvertToConcentrations(&mData);
}

void ModelFromC::updateDependentSpeciesValues(double* y_vec)
{
    if(!cupdateDependentSpeciesValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cupdateDependentSpeciesValues(&mData, y_vec);
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

    ccomputeRules(&mData, y);
}

void ModelFromC::setInitialConditions()
{
    if(!csetInitialConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetInitialConditions(&mData);
}

//void ModelFromC::computeReactionRates(double time, vector<double>& y){}
void ModelFromC::computeAllRatesOfChange()
{
    if(!ccomputeAllRatesOfChange)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeAllRatesOfChange(&mData);
}

void ModelFromC::evalModel(const double& timein, const vector<double>& y)
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

void ModelFromC::evalEvents(const double& timeIn, const vector<double>& y)
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

void ModelFromC::resetEvents()
{
    if(!cresetEvents)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cresetEvents(&mData);
}

void ModelFromC::evalInitialAssignments()
{
    if(!cevalInitialAssignments)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cevalInitialAssignments(&mData);
}

void ModelFromC::testConstraints()
{
    if(!ctestConstraints)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        throw(Exception("Problem in testConstraints"));
    }

    ctestConstraints(&mData);
}

void ModelFromC::initializeRateRuleSymbols()
{
    if(!cInitializeRateRuleSymbols)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cInitializeRateRuleSymbols(&mData);
}

} //Namespace rr

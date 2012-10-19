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

ModelFromC::ModelFromC(CGenerator* generator, HINSTANCE dllHandle)
:
mCodeGenerator(generator),
mIsInitialized(false),
mDLLHandle(dllHandle),
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
time(0),
mModelName("NoNameSet")
{
    mDummyDoubleArray[0] = 1;

    if(mDLLHandle)
    {
        SetupDLLFunctions();
        SetupDLLData();
    }
}

ModelFromC::~ModelFromC()
{
    delete [] mDummyDoubleArray;
}

void ModelFromC::AssignCVodeInterface(CvodeInterface* cvodeI)
{
    mCvodeInterface = cvodeI;
}

void ModelFromC::SetTime(double _time)
{
    *time = _time;
}

double ModelFromC::GetTime()
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

double ModelFromC::GetAmounts(const int& i)
{
    return (amounts ) ? amounts[i] : -1;
}

bool ModelFromC::SetupDLLFunctions()
{
    //Exported functions in the dll need to be assigned to a function pointer here..
    if(!mDLLHandle)
    {
        Log(lError)<<"DLL handle not valid in SetupModel function";
        return false;
    }

    //Load functions..
    cInitModel                          = (c_int)                      GetFunctionPtr("InitModel");
    cGetModelName                       = (c_charStar)                 GetFunctionPtr("GetModelName");
    cinitializeInitialConditions        = (c_void)                     GetFunctionPtr("initializeInitialConditions");
    csetParameterValues                 = (c_void)                     GetFunctionPtr("setParameterValues");
    csetCompartmentVolumes              = (c_void)                     GetFunctionPtr("setCompartmentVolumes");
    cgetNumLocalParameters              = (c_int_int)                  GetFunctionPtr("getNumLocalParameters");
    csetBoundaryConditions              = (c_void)                     GetFunctionPtr("setBoundaryConditions");
    csetInitialConditions               = (c_void)                     GetFunctionPtr("setInitialConditions");
    cevalInitialAssignments             = (c_void)                     GetFunctionPtr("evalInitialAssignments");
    ccomputeRules                       = (c_void_doubleStar)          GetFunctionPtr("computeRules");
    cconvertToAmounts                   = (c_void)                     GetFunctionPtr("convertToAmounts");
    ccomputeConservedTotals             = (c_void)                     GetFunctionPtr("computeConservedTotals");
    cgetConcentration                   = (c_double_int)               GetFunctionPtr("getConcentration");
    cGetCurrentValues                   = (c_doubleStar)               GetFunctionPtr("GetCurrentValues");
    cevalModel                          = (c_void_double_doubleStar)   GetFunctionPtr("evalModel");
    cconvertToConcentrations            = (c_void)                     GetFunctionPtr("convertToConcentrations");
    cevalEvents                         = (c_void_double_doubleStar)   GetFunctionPtr("evalEvents");
    cupdateDependentSpeciesValues       = (c_void_doubleStar)          GetFunctionPtr("updateDependentSpeciesValues");
    ccomputeAllRatesOfChange            = (c_void)                     GetFunctionPtr("computeAllRatesOfChange");
    cAssignRates_a                      = (c_void)                     GetFunctionPtr("AssignRatesA");
    cAssignRates_b                      = (c_void_doubleStar)          GetFunctionPtr("AssignRatesB");
    ctestConstraints                    = (c_void)                     GetFunctionPtr("testConstraints");
    cresetEvents                        = (c_void)                     GetFunctionPtr("resetEvents");
    cInitializeRateRuleSymbols          = (c_void)                     GetFunctionPtr("InitializeRateRuleSymbols");
    cInitializeRates                    = (c_void)                     GetFunctionPtr("InitializeRates");
    csetConcentration                   = (c_void_int_double)          GetFunctionPtr("setConcentration");
    cComputeReactionRates               = (c_void_double_doubleStar)   GetFunctionPtr("computeReactionRates");
    ccomputeEventPriorities             = (c_void)                     GetFunctionPtr("computeEventPriorities");
    return true;
}

bool ModelFromC::SetupDLLData()
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

    char* modelName = cGetModelName();
    if(modelName)
    {
        mModelName = modelName;
    }

    //Simple variables...
    int *test = (int*) GetProcAddress((HMODULE) mDLLHandle, "numIndependentVariables");
    numIndependentVariables = test;

    test = (int*) GetProcAddress((HMODULE) mDLLHandle, "numDependentVariables");
    numDependentVariables = test;

    test = (int*) GetProcAddress((HMODULE) mDLLHandle, "numTotalVariables");
    numTotalVariables = test;

    test = (int*) GetProcAddress((HMODULE) mDLLHandle, "numBoundaryVariables");
    numBoundaryVariables = test;

    test = (int*) GetProcAddress((HMODULE) mDLLHandle, "numGlobalParameters");
    numGlobalParameters = test;

    test = (int*) GetProcAddress((HMODULE) mDLLHandle, "numCompartments");
    numCompartments = test;

    test = (int*) GetProcAddress((HMODULE) mDLLHandle, "numReactions");
    numReactions = test;

    test = (int*) GetProcAddress((HMODULE) mDLLHandle, "numEvents");
    numEvents = test;

    amounts  = (double*) GetProcAddress((HMODULE) mDLLHandle, "_amounts");
    if(!amounts)
    {
        Log(lDebug)<<"Failed to assign to amounts";
    }

    amountsSize  = (int*) GetProcAddress((HMODULE) mDLLHandle, "_amountsSize");
    if(!amountsSize)
    {
        Log(lDebug)<<"Failed to assign to amountsSize";
    }

    dydt  = (double*) GetProcAddress((HMODULE) mDLLHandle, "_dydt");
    if(!dydt)
    {
        Log(lDebug)<<"Failed to assign to dydt";
    }

    dydtSize  = (int*) GetProcAddress((HMODULE) mDLLHandle, "_dydtSize");
    if(!dydtSize)
    {
        Log(lDebug)<<"Failed to assign to dydtSize";
    }

    rateRules  = (double*) GetProcAddress((HMODULE) mDLLHandle, "_rateRules");
    if(!rateRules)
    {
        Log(lDebug)<<"Failed to assign to rateRules";
    }

    rateRulesSize = (int*) GetProcAddress((HMODULE) mDLLHandle, "_rateRulesSize");
    if(!rateRulesSize)
    {
        Log(lDebug)<<"Failed to assign to rateRulesSize";
    }

    y  = (double*) GetProcAddress((HMODULE) mDLLHandle, "_y");
    if(!y)
    {
        Log(lDebug)<<"Failed to assign to mY";
    }

    ySize  = (int*) GetProcAddress((HMODULE) mDLLHandle, "_ySize");
    if(!ySize)
    {
        Log(lDebug)<<"Failed to assign to ySize";
    }

    rates  = (double*) GetProcAddress((HMODULE) mDLLHandle, "_rates");
    if(!rates)
    {
        Log(lDebug)<<"Failed to assign to rates";
    }

    ratesSize  = (int*) GetProcAddress((HMODULE) mDLLHandle, "_ratesSize");
    if(!ratesSize)
    {
        Log(lDebug)<<"Failed to assign to ratesSize";
    }

    ct  = (double*) GetProcAddress((HMODULE) mDLLHandle, "_ct");
    if(!ct)
    {
        Log(lDebug)<<"Failed to assign to ct";
    }

    ctSize  = (int*) GetProcAddress((HMODULE) mDLLHandle, "_ctSize");
    if(!ctSize)
    {
        Log(lDebug)<<"Failed to assign to ctSize";
    }

    time       = (double*) GetProcAddress((HMODULE) mDLLHandle, "mTime");
    if(!time)
    {
        Log(lDebug)<<"Failed to assign to time";
    }

    init_y       = (double*) GetProcAddress((HMODULE) mDLLHandle, "_init_y");
    if(!init_y)
    {
        Log(lDebug)<<"Failed to assign to init_y";
    }

    init_ySize       = (int*) GetProcAddress((HMODULE) mDLLHandle, "_init_ySize");
    if(!init_ySize)
    {
        Log(lDebug)<<"Failed to assign to init_ySize";
    }

    gp       = (double*) GetProcAddress((HMODULE) mDLLHandle, "_gp");
    if(!gp)
    {
        Log(lDebug)<<"Failed to assign to gp";
    }

    gpSize       = (int*) GetProcAddress((HMODULE) mDLLHandle, "_gpSize");
    if(!gpSize)
    {
        Log(lDebug)<<"Failed to assign to gpSize";
    }

    c       = (double*) GetProcAddress((HMODULE) mDLLHandle, "_c");
    if(!c)
    {
        Log(lDebug)<<"Failed to assign to mC";
    }

    cSize       = (int*) GetProcAddress((HMODULE) mDLLHandle, "_cSize");
    if(!cSize)
    {
        Log(lDebug)<<"Failed to assign to cSize";
    }

    bc       = (double*) GetProcAddress((HMODULE) mDLLHandle, "_bc");
    if(!bc)
    {
        Log(lDebug)<<"Failed to assign to bc";
    }

    bcSize       = (int*) GetProcAddress((HMODULE) mDLLHandle, "_bcSize");
    if(!bcSize)
    {
        Log(lDebug)<<"Failed to assign to bcSize";
    }

    lp       = (double*) GetProcAddress((HMODULE) mDLLHandle, "_lp");
    if(!lp)
    {
        Log(lDebug)<<"Failed to assign to lp";
        lp = mDummyDoubleArray;
    }

    lpSize       = (int*) GetProcAddress((HMODULE) mDLLHandle, "_lpSize");
    if(!srSize)
    {
        Log(lDebug)<<"Failed to assign to lpSize";
        lpSize = &mDummyInt;
    }

    sr       = (double*) GetProcAddress((HMODULE) mDLLHandle, "_sr");
    if(!sr)
    {
        Log(lDebug)<<"Failed to assign to sr";
        sr = mDummyDoubleArray;
    }

    srSize       = (int*) GetProcAddress((HMODULE) mDLLHandle, "_srSize");
    if(!srSize)
    {
        Log(lDebug)<<"Failed to assign to srSize";
        srSize = &mDummyInt;
    }

    eventPriorities   = (double*) GetProcAddress((HMODULE) mDLLHandle, "_eventPriorities");
    if(!eventPriorities)
    {
        Log(lDebug)<<"Failed to assign to eventPriorities";
        eventPriorities = NULL;
    }

    eventStatusArray   = (bool*) GetProcAddress((HMODULE) mDLLHandle, "mEventStatusArray");
    if(!eventStatusArray)
    {
        Log(lDebug)<<"Failed to assign to eventStatusArray";
        eventStatusArray = NULL;
    }

    eventStatusArraySize      = (int*) GetProcAddress((HMODULE) mDLLHandle, "mEventStatusArraySize");
    if(!eventStatusArraySize)
    {
        Log(lDebug)<<"Failed to assign to eventStatusArraySize";
    }

    previousEventStatusArray   = (bool*) GetProcAddress((HMODULE) mDLLHandle, "_previousEventStatusArray");
    if(!previousEventStatusArray)
    {
        Log(lDebug)<<"Failed to assign to previousEventStatusArray";
        previousEventStatusArray = NULL;
    }

    previousEventStatusArraySize      = (int*) GetProcAddress((HMODULE) mDLLHandle, "_previousEventStatusArraySize");
    if(!previousEventStatusArraySize)
    {
        Log(lDebug)<<"Failed to assign to previousEventStatusArraySize";
        previousEventStatusArraySize = &mDummyInt;
    }

    eventPersistentType   = (bool*) GetProcAddress((HMODULE) mDLLHandle, "_eventPersistentType");
    if(!eventPersistentType)
    {
        Log(lDebug)<<"Failed to assign to eventPersistentType";
        eventPersistentType = NULL;
    }

    eventPersistentTypeSize      = (int*) GetProcAddress((HMODULE) mDLLHandle, "_eventPersistentTypeSize");
    if(!eventPersistentTypeSize)
    {
        Log(lDebug)<<"Failed to assign to eventPersistentTypeSize";
    }

    eventTests   = (double*) GetProcAddress((HMODULE) mDLLHandle, "mEventTests");
    if(!eventTests)
    {
        Log(lDebug)<<"Failed to assign to eventTests";
        eventTests = NULL;
    }

    eventTestsSize      = (int*) GetProcAddress((HMODULE) mDLLHandle, "mEventTestsSize");
    if(!eventTestsSize)
    {
        Log(lDebug)<<"Failed to assign to eventTestsSize";
        eventTestsSize = & mDummyInt;
    }

    eventType   = (bool*) GetProcAddress((HMODULE) mDLLHandle, "_eventType");
    if(!eventType)
    {
        Log(lDebug)<<"Failed to assign to eventType";
        eventType = NULL;
    }

    eventTypeSize      = (int*) GetProcAddress((HMODULE) mDLLHandle, "_eventTypeSize");
    if(!eventTypeSize)
    {
        Log(lDebug)<<"Failed to assign to eventTypeSize";
        eventTypeSize = & mDummyInt;
    }

    //Event function pointer stuff
    c_TEventAssignmentDelegateStar     Get_eventAssignments;
    Get_eventAssignments  = (c_TEventAssignmentDelegateStar) GetProcAddress((HMODULE) mDLLHandle, "Get_eventAssignments");
    if(!Get_eventAssignments)
    {
        Log(lDebug)<<"Failed to assign to Get_eventAssignments";
        eventAssignments = NULL;
    }
    else
    {
        eventAssignments = Get_eventAssignments();
    }

    c_TComputeEventAssignmentDelegateStar     Get_computeEventAssignments;
    Get_computeEventAssignments      = (c_TComputeEventAssignmentDelegateStar) GetProcAddress((HMODULE) mDLLHandle, "Get_computeEventAssignments");
    if(!Get_computeEventAssignments)
    {
        Log(lDebug)<<"Failed to assign to Get_computeEventAssignments";
    }
    else
    {
        computeEventAssignments = Get_computeEventAssignments();
    }

    c_TPerformEventAssignmentDelegateStar     Get_performEventAssignments;
    Get_performEventAssignments  = (c_TPerformEventAssignmentDelegateStar) GetProcAddress((HMODULE) mDLLHandle, "Get_performEventAssignments");
    if(!Get_performEventAssignments)
    {
        Log(lDebug)<<"Failed to assign to Get_performEventAssignments";
        performEventAssignments = NULL;
    }
    else
    {
        performEventAssignments = Get_performEventAssignments();
    }

    c_GetEventDelayDelegatesStar     GetEventDelays;
    GetEventDelays  = (c_GetEventDelayDelegatesStar) GetProcAddress((HMODULE) mDLLHandle, "GetEventDelays");
    if(!GetEventDelays)
    {
        Log(lDebug)<<"Failed to assign to Get_computeEventAssignments";
    }
    else
    {
        eventDelays = GetEventDelays();
    }


    return true;
}

FARPROC ModelFromC::GetFunctionPtr(const string& funcName)
{
    FARPROC handle = GetProcAddress((HMODULE) mDLLHandle, funcName.c_str());
    if(handle == NULL)
    {
        Log(lError) << "Unable to load the function: " << funcName;
        return NULL;
    }
    Log(lDebug3)<<"Loaded function " << funcName;
    return handle;
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

vector<double> ModelFromC::GetCurrentValues()
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

void ModelFromC::InitializeRates()
{
    if(!cInitializeRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cInitializeRates();
}

void ModelFromC::AssignRates()
{
    if(!cAssignRates_a)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cAssignRates_a();
}

void ModelFromC::AssignRates(vector<double>& _rates)
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

    //copy y values to amounts
//    for(u_int i = 0; i < y.size(); i++)
//    {
//        amounts[i] = y[i];
//    }
//
//    cevalModel(timein, amounts);

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

//    cevalEvents(timeIn, amounts);
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

void ModelFromC::InitializeRateRuleSymbols()
{
    if(!cInitializeRateRuleSymbols)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cInitializeRateRuleSymbols();
}

} //Namespace rr

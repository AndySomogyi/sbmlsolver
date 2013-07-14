#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iostream>
#include "rrException.h"
#include "rrLogger.h"
#include "rrCompiledExecutableModel.h"
#include "rrCModelGenerator.h"
#include "rrUtils.h"
#include "rrCompiledModelState.h"
//---------------------------------------------------------------------------
using namespace std;
namespace rr
{

CompiledExecutableModel::CompiledExecutableModel(const ModelSymbols& symbols, ModelSharedLibrary* dll) :
mDummyInt(0),
mDummyDouble(0),
mDummyDoubleArray(new double[1]),
mData(),
ms(symbols),
mIsInitialized(false),
mDLL(dll)
{
    //Zero data structure..
    initModelData(mData);
    mDummyDoubleArray[0] = 1;

    if(mDLL->isLoaded())
    {
        setupDLLFunctions();
        setupModelData();
    }
    else
    {
        Log(lError)<<"The Model DLL is not loaded in CompiledExecutableModel ctor..";
    }
}

CompiledExecutableModel::~CompiledExecutableModel()
{
    Log(lDebug3) << "in " << __FUNC__ << "\n";
    // only free buffers, mData is stack allocated.
    freeModelDataBuffers(mData);
    delete [] mDummyDoubleArray;
    if(mDLL)
    {
        mDLL->unload();
    }
    delete mDLL;

    while (!modelStates.empty())
    {
        CompiledModelState *state = modelStates.top();
        modelStates.pop();
        delete state;
    }
}

string CompiledExecutableModel::getModelName()
{
    return mData.modelName;
}

void CompiledExecutableModel::setTime(double _time)
{
    mData.time = _time;
}

double CompiledExecutableModel::getTime()
{
    return mData.time;
}

/////////////////// The following used to be in IModel
int CompiledExecutableModel::getNumIndependentSpecies()
{
    return mData.numIndependentSpecies;
}

int CompiledExecutableModel::getNumDependentSpecies()
{
    return mData.numDependentSpecies;
}

int CompiledExecutableModel::getNumFloatingSpecies()
{
    return mData.numFloatingSpecies;
}

int CompiledExecutableModel::getNumBoundarySpecies()
{
    return mData.numBoundarySpecies;
}

int CompiledExecutableModel::getNumGlobalParameters()
{
    return mData.numGlobalParameters;
}

int CompiledExecutableModel::getNumCompartments()
{
    return mData.numCompartments;
}

int CompiledExecutableModel::getNumReactions()
{
    return mData.numReactions;
}

int CompiledExecutableModel::getNumEvents()
{
    return mData.numEvents;
}

double CompiledExecutableModel::getAmounts(const int& i)
{
    return (mData.floatingSpeciesAmounts ) ? mData.floatingSpeciesAmounts[i] : -1;
}

int CompiledExecutableModel::getReactionIndex(const std::string& reactionName)
{
    int result = -1;
    return ms.mReactionList.find(reactionName, result) ? result : -1;
}

std::string CompiledExecutableModel::getReactionName(int index)
{
    return ms.mReactionList[index].name;
}

int CompiledExecutableModel::getGlobalParameterIndex(const std::string& name)
{
    int result = -1;
    return ms.mGlobalParameterList.find(name, result) ? result : -1;
}

string CompiledExecutableModel::getGlobalParameterName(int index)
{
    return ms.mGlobalParameterList[index].name;
}

int CompiledExecutableModel::getBoundarySpeciesIndex(const string& name)
{
    int result = -1;
    return ms.mBoundarySpeciesList.find(name, result) ? result : -1;
}

string CompiledExecutableModel::getBoundarySpeciesName(int index)
{
    return ms.mBoundarySpeciesList[index].name;
}

int CompiledExecutableModel::getBoundarySpeciesCompartmentIndex(int index)
{
    // mModel->getCompartments().find(mModel->getBoundarySpecies()[record.index].compartmentName, nIndex))
    string compartmentName = ms.mBoundarySpeciesList[index].compartmentName;
    int compartmentIndex = -1;
    return ms.mCompartmentList.find(compartmentName, compartmentIndex) ? compartmentIndex : -1;
}

int CompiledExecutableModel::getCompartmentIndex(const string& name)
{
    int result = -1;
    return ms.mCompartmentList.find(name, result) ? result : -1;
}

string CompiledExecutableModel::getCompartmentName(int index)
{
    return ms.mCompartmentList[index].name;
}

int CompiledExecutableModel::getFloatingSpeciesIndex(const string& name)
{
    int result = -1;
    return ms.mFloatingSpeciesConcentrationList.find(name, result) ? result : -1;
}

string CompiledExecutableModel::getFloatingSpeciesName(int index)
{
    return ms.mFloatingSpeciesConcentrationList[index].name;
}

double CompiledExecutableModel::getGlobalParameterValue(int index)
{
    if ((index >= 0) && (index < getNumGlobalParameters() + getModelData().numDependentSpecies))
    {
        if (index >= getNumGlobalParameters())
        {
            return getModelData().dependentSpeciesConservedSums[index - getNumGlobalParameters()];
        }
        else
        {
            return getModelData().globalParameters[index];
        }
    }
    else
    {
        throw CoreException(format("Index in getNumGlobalParameters out of range: [{0}]", index));
    }
}

void CompiledExecutableModel::setGlobalParameterValue(int index, double value)
{
    if ((index >= 0) && (index < getNumGlobalParameters() + getModelData().numDependentSpecies))
    {
        if (index >= getNumGlobalParameters())
        {
            getModelData().dependentSpeciesConservedSums[index - getNumGlobalParameters()] = value;
            updateDependentSpeciesValues(getModelData().floatingSpeciesConcentrations);
        }
        else
        {
            getModelData().globalParameters[index] = value;
        }
    }
    else
    {
        throw CoreException(format("Index in getNumGlobalParameters out of range: [{0}]", index));
    }
}

int CompiledExecutableModel::pushState(unsigned options)
{
    CompiledModelState *state = new CompiledModelState(*this);
    modelStates.push(state);
    return modelStates.size();
}

int CompiledExecutableModel::popState(unsigned options)
{
    if (modelStates.size() > 0)
    {
        CompiledModelState *state = modelStates.top();
        modelStates.pop();
        if (!(options && PopDiscard))
        {
            state->AssignToModel(*this);
        }
        delete state;
    }
    return modelStates.size();
}

bool CompiledExecutableModel::setupDLLFunctions()
{
    //Exported functions in the dll need to be assigned to a function pointer here..
    if(!mDLL->isLoaded())
    {
        Log(lError)<<"DLL handle not valid in SetupModel function";
        return false;
    }

    //Load functions..
    cInitModel                          = (c_int_MDS)                      mDLL->getSymbol("InitModel");
    cInitModelData                      = (c_int_MDS)                      mDLL->getSymbol("InitModelData");
    cinitializeInitialConditions        = (c_void_MDS)                     mDLL->getSymbol("initializeInitialConditions");
    csetParameterValues                 = (c_void_MDS)                     mDLL->getSymbol("setParameterValues");
    csetCompartmentVolumes              = (c_void_MDS)                     mDLL->getSymbol("setCompartmentVolumes");
    cgetNumLocalParameters              = (c_int_MDS_int)                  mDLL->getSymbol("getNumLocalParameters");
    csetBoundaryConditions              = (c_void_MDS)                     mDLL->getSymbol("setBoundaryConditions");
    csetInitialConditions               = (c_void_MDS)                     mDLL->getSymbol("setInitialConditions");
    cevalInitialAssignments             = (c_void_MDS)                     mDLL->getSymbol("evalInitialAssignments");
    ccomputeRules                       = (c_void_MDS_doubleStar)          mDLL->getSymbol("computeRules");
    cconvertToAmounts                   = (c_void_MDS)                     mDLL->getSymbol("convertToAmounts");
    ccomputeConservedTotals             = (c_void_MDS)                     mDLL->getSymbol("computeConservedTotals");
    cgetConcentration                   = (c_double_MDS_int)               mDLL->getSymbol("getConcentration");
    cGetCurrentValues                   = (c_doubleStar_MDS)               mDLL->getSymbol("GetCurrentValues");
    cevalModel                          = (c_void_MDS_double_doubleStar)   mDLL->getSymbol("__evalModel");
    cconvertToConcentrations            = (c_void_MDS)                     mDLL->getSymbol("convertToConcentrations");
    cevalEvents                         = (c_void_MDS_double_doubleStar)   mDLL->getSymbol("evalEvents");
    cupdateDependentSpeciesValues       = (c_void_MDS_doubleStar)          mDLL->getSymbol("updateDependentSpeciesValues");
    ccomputeAllRatesOfChange            = (c_void_MDS)                     mDLL->getSymbol("computeAllRatesOfChange");
    cAssignRates_a                      = (c_void_MDS)                     mDLL->getSymbol("AssignRatesA");
    cAssignRates_b                      = (c_void_MDS_doubleStar)          mDLL->getSymbol("AssignRatesB");
    ctestConstraints                    = (c_void_MDS)                     mDLL->getSymbol("testConstraints");
    cresetEvents                        = (c_void_MDS)                     mDLL->getSymbol("resetEvents");
    cInitializeRateRuleSymbols          = (c_void_MDS)                     mDLL->getSymbol("InitializeRateRuleSymbols");
    cInitializeRates                    = (c_void_MDS)                     mDLL->getSymbol("InitializeRates");
    csetConcentration                   = (c_void_MDS_int_double)          mDLL->getSymbol("setConcentration");
    cComputeReactionRates               = (c_void_MDS_double_doubleStar)   mDLL->getSymbol("computeReactionRates");
    ccomputeEventPriorities             = (c_void_MDS)                     mDLL->getSymbol("computeEventPriorities");
    return true;
}

bool CompiledExecutableModel::setupModelData()
{
    // zero out the structure
    initModelData(mData);

    // set the buffer sizes
    mData.numIndependentSpecies         = ms.mNumIndependentSpecies;
    mData.numDependentSpecies           = ms.mNumDependentSpecies;
    mData.numGlobalParameters           = ms.mGlobalParameterList.size();
    mData.numReactions                  = ms.mReactionList.size();
    mData.numEvents                     = ms.mNumEvents;
    mData.numFloatingSpecies            = ms.mFloatingSpeciesConcentrationList.size();
    mData.numRateRules                  = ms.mRateRules.size();
    mData.numCompartments               = ms.mCompartmentList.size();
    mData.numBoundarySpecies            = ms.mNumBoundarySpecies;
    mData.srSize                        = ms.mNumModifiableSpeciesReferences;
    mData.eventPrioritiesSize           = ms.mNumEvents;
    mData.eventStatusArraySize          = ms.mNumEvents;
    mData.previousEventStatusArraySize  = ms.mNumEvents;
    mData.eventPersistentTypeSize       = ms.mNumEvents;
    mData.eventTestsSize                = ms.mNumEvents;
    mData.eventTypeSize                 = ms.mNumEvents;

    // allocate the data buffers
    string test = ms.mModelName;
    allocModelDataBuffers(mData, test);

    if(cInitModel)
    {
        cInitModel(&mData);
    }
    return true;
}

void CompiledExecutableModel::setCompartmentVolumes()
{
    if(!csetCompartmentVolumes)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetCompartmentVolumes(&mData);
}

void  CompiledExecutableModel::setConcentration(int index, double value)
{
    if(!csetConcentration)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetConcentration(&mData, index, value);
}

void  CompiledExecutableModel::computeReactionRates (double time, double* y)
{
    if(!cComputeReactionRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cComputeReactionRates(&mData, time, y);
}

void CompiledExecutableModel::getRateRuleValues(double *rateRuleValues)
{
    vector<double> vals;
    if(!cGetCurrentValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
    }

    // in CModelGenerator::writeComputeRules, in effect, the following
    // line is writen in the generated code:
    // dResult = (double*) calloc( numAdditionalRates() sizeof(double))
    double* values = cGetCurrentValues(&mData);

    int count = ms.mRateRules.size();
    if(values)
    {
        for(int i = 0; i < count; i++)
        {
            rateRuleValues[i] = values[i];
        }
    }

    // allocated in C, free'd here
    // for now, we'll just leak in Windows.
#if defined (__unix__) || defined(__APPLE__)
    free(values);
#endif

}

double CompiledExecutableModel::getConcentration(int index)
{
    if(!cgetConcentration)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return 0;
    }

    return cgetConcentration(&mData, index);
}

int CompiledExecutableModel::getNumLocalParameters(int reactionId)
{
    if(!cgetNumLocalParameters)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return 0;
    }

    return cgetNumLocalParameters(&mData, reactionId);
}

void CompiledExecutableModel::initializeInitialConditions()
{
    if(!cinitializeInitialConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cinitializeInitialConditions(&mData);
}

//void CompiledExecutableModel::setInitialConditions(){}
void CompiledExecutableModel::setParameterValues()
{
    if(!csetParameterValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    csetParameterValues(&mData);
}

void CompiledExecutableModel::setBoundaryConditions()
{
    if(!csetBoundaryConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    csetBoundaryConditions(&mData);
}

void CompiledExecutableModel::initializeRates()
{
    if(!cInitializeRates)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cInitializeRates(&mData);
}


void CompiledExecutableModel::setRateRuleValues(const double *_rates)
{
    if(!cAssignRates_b)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cAssignRates_b(&mData, _rates);
}

void CompiledExecutableModel::computeConservedTotals()
{
    if(!ccomputeConservedTotals)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeConservedTotals(&mData);
}

void CompiledExecutableModel::computeEventPriorites()
{
    if(!ccomputeEventPriorities)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeEventPriorities(&mData);
}

void CompiledExecutableModel::convertToAmounts()
{
    if(!cconvertToAmounts)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cconvertToAmounts(&mData);
}

void CompiledExecutableModel::convertToConcentrations()
{
    if(!cconvertToConcentrations)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cconvertToConcentrations(&mData);
}

void CompiledExecutableModel::updateDependentSpeciesValues(double* y_vec)
{
    if(!cupdateDependentSpeciesValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cupdateDependentSpeciesValues(&mData, y_vec);
}

void CompiledExecutableModel::computeRules(vector<double>& arr)
{
    double* cArr = createVector(arr);
    computeRules(cArr, arr.size());
    delete [] cArr;

}
void CompiledExecutableModel::computeRules(double* y, int size)
{
    if(!ccomputeRules)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    ccomputeRules(&mData, y);
}

void CompiledExecutableModel::setInitialConditions()
{
    if(!csetInitialConditions)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    csetInitialConditions(&mData);
}

//void CompiledExecutableModel::computeReactionRates(double time, vector<double>& y){}
void CompiledExecutableModel::computeAllRatesOfChange()
{
    if(!ccomputeAllRatesOfChange)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    ccomputeAllRatesOfChange(&mData);
}

void CompiledExecutableModel::evalModel(double timein, const double *y)
{
    if(!cevalModel)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cevalModel(&mData, timein, y);
}

void CompiledExecutableModel::evalEvents(const double& timeIn, const vector<double>& y)
{
    if(!cevalEvents)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    double *oAmounts = createVector(y);
    cevalEvents(&mData, timeIn, oAmounts);
    delete [] oAmounts;
}

void CompiledExecutableModel::resetEvents()
{
    if(!cresetEvents)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cresetEvents(&mData);
}

void CompiledExecutableModel::evalInitialAssignments()
{
    if(!cevalInitialAssignments)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cevalInitialAssignments(&mData);
}

void CompiledExecutableModel::testConstraints()
{
    if(!ctestConstraints)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        throw(Exception("Problem in testConstraints"));
    }

    ctestConstraints(&mData);
}

void CompiledExecutableModel::initializeRateRuleSymbols()
{
    if(!cInitializeRateRuleSymbols)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }

    cInitializeRateRuleSymbols(&mData);
}

string CompiledExecutableModel::getInfo()
{
    stringstream info;
    info << "CompiledExecutableModel" << endl;
    info << "ModelName: "             <<  getModelName()<<endl;
    info << "Model DLL Loaded: "      << (mDLL->isLoaded() ? "true" : "false")    <<endl;
    info << "Initialized: "           << (mIsInitialized ? "true" : "false")    <<endl;
    return info.str();
}

ModelData& CompiledExecutableModel::getModelData()
{
    return mData;
}

const SymbolList &CompiledExecutableModel::getConservations()
{
    return ms.mConservationList;
}

const StringList CompiledExecutableModel::getConservationNames()
{
    StringList tmp; // = new ArrayList();
    for (int i = 0; i < ms.mConservationList.Count(); i++)
    {
        tmp.add(ms.mConservationList[i].name);
    }
    return tmp;
}


} //Namespace rr

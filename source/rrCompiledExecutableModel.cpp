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
//---------------------------------------------------------------------------
using namespace std;
namespace rr
{

CompiledExecutableModel::CompiledExecutableModel(CModelGenerator& generator, ModelSharedLibrary* dll) :
mDummyInt(0),
mDummyDouble(0),
mDummyDoubleArray(new double[1]),
mData(),
mCG(generator),
mNOM(*generator.mNOM),
mLibStruct(*generator.mLibStruct),
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
    cout << "in " << __FUNC__ << "\n";
    // only free buffers, mData is stack allocated.
    freeModelDataBuffers(mData);
    delete [] mDummyDoubleArray;
    if(mDLL)
    {
        mDLL->unload();
    }
    delete mDLL;
}

string CompiledExecutableModel::getModelName()
{
    return mData.modelName;
}

void CompiledExecutableModel::assignCVodeInterface(CvodeInterface* cvodeI)
{
    mCvodeInterface = cvodeI;
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
int CompiledExecutableModel::getNumIndependentVariables()
{
    return mData.numIndependentVariables;
}

int CompiledExecutableModel::getNumDependentVariables()
{
    return mData.numDependentVariables;
}

int CompiledExecutableModel::getNumTotalVariables()
{
    return mData.numTotalVariables;
}

int CompiledExecutableModel::getNumBoundarySpecies()
{
    return mData.numBoundaryVariables;    //Todos: bad naming - is Variables/Species, choose one..
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

int CompiledExecutableModel::getNumRules()
{
    return mData.numRules;
}

int CompiledExecutableModel::getNumEvents()
{
    return mData.numEvents;
}

double CompiledExecutableModel::getAmounts(const int& i)
{
    return (mData.amounts ) ? mData.amounts[i] : -1;
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
    mData.numIndependentVariables       = mCG.mNumIndependentSpecies;
    mData.numDependentVariables         = mCG.mNumDependentSpecies;
    mData.numTotalVariables             = mCG.mNumFloatingSpecies;        //???
    mData.numBoundaryVariables          = mCG.mNumBoundarySpecies;
    mData.numGlobalParameters           = mCG.mGlobalParameterList.size();
    mData.numCompartments               = mCG.mCompartmentList.size();
    mData.numReactions                  = mCG.mReactionList.size();
    mData.numEvents                     = mCG.mNumEvents;
    mData.amountsSize                   = mCG.mFloatingSpeciesConcentrationList.Count();
    mData.dydtSize                      = mCG.mFloatingSpeciesConcentrationList.size();
    mData.rateRulesSize                 = mCG.mMapRateRule.size();
    mData.ySize                         = mCG.mFloatingSpeciesConcentrationList.size();
    mData.ratesSize                     = mCG.mNumReactions;
    mData.ctSize                        = mCG.mNumDependentSpecies;
    mData.init_ySize                    = mCG.mFloatingSpeciesConcentrationList.Count();
    mData.gpSize                        = mCG.mNumGlobalParameters + mCG.mTotalLocalParmeters;
    mData.cSize                         = mCG.mNumCompartments;
    mData.bcSize                        = mCG.mNumBoundarySpecies;
    mData.lpSize                        = mCG.mNumReactions;
    mData.srSize                        = mCG.mNumModifiableSpeciesReferences;
    mData.localParameterDimensionsSize  = mCG.mNumReactions;
    mData.eventPrioritiesSize           = mCG.mNumEvents;
    mData.eventStatusArraySize          = mCG.mNumEvents;
    mData.previousEventStatusArraySize  = mCG.mNumEvents;
    mData.eventPersistentTypeSize       = mCG.mNumEvents;
    mData.eventTestsSize                = mCG.mNumEvents;
    mData.eventTypeSize                 = mCG.mNumEvents;

    // allocate the data buffers
    allocModelDataBuffers(mData, mNOM.getModelName());

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

vector<double> CompiledExecutableModel::getCurrentValues()
{
    vector<double> vals;
    if(!cGetCurrentValues)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return vals;
    }

    // in CModelGenerator::writeComputeRules, in effect, the following
    // line is writen in the generated code:
    // dResult = (double*) calloc( numAdditionalRates() sizeof(double))
    double* values = cGetCurrentValues(&mData);

    int count = mCG.numAdditionalRates();
    if(values)
    {
        for(int i = 0; i < count; i++)
        {
            vals.push_back(values[i]);
        }
    }

    // allocated in C, free'd here
    free(values);

    return vals;
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

void CompiledExecutableModel::assignRates()
{
    if(!cAssignRates_a)
    {
        Log(lError)<<"Tried to call NULL function in "<<__FUNCTION__;
        return;
    }
    cAssignRates_a(&mData);
}

void CompiledExecutableModel::assignRates(vector<double>& _rates)
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
    double* cArr = CreateVector(arr);
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

void CompiledExecutableModel::evalModel(const double& timein, const vector<double>& y)
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

void CompiledExecutableModel::evalEvents(const double& timeIn, const vector<double>& y)
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

CvodeInterface* CompiledExecutableModel::getCvodeInterface() {
    return mCvodeInterface;
}

SymbolList &CompiledExecutableModel::getReactions() {
    return mCG.mReactionList;
}

SymbolList &CompiledExecutableModel::getGlobalParameters()
{
    return mCG.mGlobalParameterList;
}

SymbolList &CompiledExecutableModel::getBoundarySpecies()
{
    return mCG.mBoundarySpeciesList;
}

SymbolList &CompiledExecutableModel::getCompartments()
{
    return mCG.mCompartmentList;
}

SymbolList &CompiledExecutableModel::getConservations()
{
    return mCG.mConservationList;
}

SymbolList &CompiledExecutableModel::getFloatingSpeciesAmounts()
{
    return mCG.mFloatingSpeciesAmountsList;
}

SymbolList &CompiledExecutableModel::getFloatingSpeciesConcentrations()
{
    return mCG.mFloatingSpeciesConcentrationList;
}

StringList CompiledExecutableModel::getCompartmentNames()
{
    StringList tmp;
    for (u_int i = 0; i < mCG.mCompartmentList.size(); i++)
    {
        tmp.Add(mCG.mCompartmentList[i].name);
    }
    return tmp;
}

StringList CompiledExecutableModel::getConservationNames()
{
    StringList tmp; // = new ArrayList();
    for (int i = 0; i < mCG.mConservationList.Count(); i++)
    {
        tmp.Add(mCG.mConservationList[i].name);
    }
    return tmp;
}

StringList CompiledExecutableModel::getGlobalParameterNames()
{
    StringList tmp;
    for (int i = 0; i < mCG.mGlobalParameterList.size(); i++)
    {
        tmp.Add(mCG.mGlobalParameterList[i].name);
    }

    for (int i = 0; i < mCG.mConservationList.Count(); i++)
    {
        tmp.Add(mCG.mConservationList[i].name);
    }

    return tmp;
}

StringList CompiledExecutableModel::getReactionNames()
{
    StringList tmp;
    for (int i = 0; i < mCG.mReactionList.size(); i++)
    {
        tmp.Add(mCG.mReactionList[i].name);
    }
    return tmp;
}

StringList CompiledExecutableModel::getFloatingSpeciesConcentrationNames()
{
    StringList tmp;
    for (int i = 0; i < mCG.mFloatingSpeciesConcentrationList.size(); i++)
    {
        tmp.Add(mCG.mFloatingSpeciesConcentrationList[i].name);
    }
    return tmp;
}

StringList CompiledExecutableModel::getBoundarySpeciesNames()
{
    StringList tmp;
    for (int i = 0; i < mCG.mBoundarySpeciesList.size(); i++)
    {
        tmp.Add(mCG.mBoundarySpeciesList[i].name);
    }
    return tmp;
}


} //Namespace rr

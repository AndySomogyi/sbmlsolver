/*
 * rrModelSymbols.cpp
 *
 *  Created on: May 31, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "rrModelSymbols.h"
#include "rrStringUtils.h"
#include "sbml/Model.h"
#include "sbml/SBMLDocument.h"
#include "rrLogger.h"
#include "rrException.h"
#include "rrOSSpecifics.h"
#include "rrRule.h"
#include <sstream>

using namespace std;
using namespace libsbml;
using namespace ls;

namespace rr
{

const string ModelSymbols::mDoubleFormat("%.19G");

SymbolList test1() {
    return SymbolList();
}

SymbolList test2(SymbolList& s) {
    return s;
}

//string CModelGenerator::generateModelCode(const string& sbmlStr, const bool& _computeAndAssignConsevationLaws)
//{
//    //This function now assume that the sbml already been loaded into NOM and libstruct..
//    mComputeAndAssignConsevationLaws  = _computeAndAssignConsevationLaws;
//    Log(lDebug2)<<"Entering CModelGenerators generateModelCode function";
//    StringList  Warnings;
//    CodeBuilder ignore;     //The Write functions below are inherited with a CodeBuilder in the
//                            //prototype that is not to be used..
//
//    //Clear header and source file objects...
//    mHeader.Clear();
//    mSource.Clear();
//
//    mModelName = mNOM->getModelName();
//    if(!mModelName.size())
//    {
//        Log(lWarning)<<"Model name is empty. ModelName is assigned 'NameNotSet'.";
//        mModelName = "NameNotSet";
//    }
//
//    Log(lDebug1)<<"Processing model: "<<mModelName;
//    mNumReactions  = mNOM->getNumReactions();
//
//    Log(lDebug3)<<"Number of reactions:"<<mNumReactions;
//
//    mGlobalParameterList.Clear();
//    mModifiableSpeciesReferenceList.Clear();
//    mLocalParameterList.reserve(mNumReactions);
//    mReactionList.Clear();
//    mBoundarySpeciesList.Clear();
//    mFloatingSpeciesConcentrationList.Clear();
//    mFloatingSpeciesAmountsList.Clear();
//    mCompartmentList.Clear();
//    mConservationList.Clear();
//    mFunctionNames.empty();
//    mFunctionParameters.empty();
//
//    if(mComputeAndAssignConsevationLaws)
//    {
//        mNumIndependentSpecies     = mLibStruct->getNumIndSpecies();
//        mIndependentSpeciesList = mLibStruct->getIndependentSpecies();
//        mDependentSpeciesList   = mLibStruct->getDependentSpecies();
//    }
//    else
//    {
//        mNumIndependentSpecies = mLibStruct->getNumSpecies();
//        mIndependentSpeciesList = mLibStruct->getSpecies();
//    }
//
//    // Load the compartment array (name and value)
//    mNumCompartments            = readCompartments();
//
//    // Read FloatingSpecies
//    mNumFloatingSpecies         = readFloatingSpecies();
//    mNumDependentSpecies        = mNumFloatingSpecies - mNumIndependentSpecies;
//
//    // Load the boundary species array (name and value)
//    mNumBoundarySpecies     = readBoundarySpecies();
//
//    // Get all the parameters into a list, global and local
//    mNumGlobalParameters     = readGlobalParameters();
//    mNumModifiableSpeciesReferences = readModifiableSpeciesReferences();
//
//    // Load up local parameters next
//    readLocalParameters(mNumReactions, mLocalParameterDimensions, mTotalLocalParmeters);
//    mNumEvents = mNOM->getNumEvents();
//
//    //Write model to String builder...
//    writeClassHeader(ignore);
//    writeOutVariables(ignore);
//    writeOutSymbolTables(ignore);
//
//    ///// Write non exports
//    mHeader.NewLine("\n//NON - EXPORTS ========================================");
//    mHeader.AddFunctionProto("void", "InitializeDelays(ModelData* md)");
//
//    ///// Start of exported functions
//    mHeader.NewLine("\n//EXPORTS ========================================");
//    mHeader.AddFunctionExport("int", "InitModelData(ModelData* md)");
//    mHeader.AddFunctionExport("int", "InitModel(ModelData* md)");
//
//    mHeader.AddFunctionExport("char*", "getModelName(ModelData* md)");
//    ///////////////
//
//    writeResetEvents(ignore, mNumEvents);
//    writeSetConcentration(ignore);
//    writeGetConcentration(ignore);
//    writeConvertToAmounts(ignore);
//    writeConvertToConcentrations(ignore);
//    writeProperties(ignore);
//    writeAccessors(ignore);
//    writeUserDefinedFunctions(ignore);
//    writeSetInitialConditions(ignore, mNumFloatingSpecies);
//    writeSetBoundaryConditions(ignore);
//    writeSetCompartmentVolumes(ignore);
//    writeSetParameterValues(ignore, mNumReactions);
//
//    //fills mConservationList
//    writeComputeConservedTotals(ignore, mNumFloatingSpecies, mNumDependentSpecies);
//
//    // Get the L0 matrix
//    int nrRows;
//    int nrCols;
//
//    ls::DoubleMatrix* aL0 = initializeL0(nrRows, nrCols);     //Todo: What is this doing? answer.. it is used below..
//    writeUpdateDependentSpecies(ignore, mNumIndependentSpecies, mNumDependentSpecies, *aL0);
//    int numOfRules = writeComputeRules(ignore, mNumReactions);
//
//    writeComputeAllRatesOfChange(ignore, mNumIndependentSpecies, mNumDependentSpecies, *aL0);
//    delete aL0;
//    writeComputeReactionRates(ignore, mNumReactions);
//    writeEvalModel(ignore, mNumReactions, mNumIndependentSpecies, mNumFloatingSpecies, numOfRules);
//    writeEvalEvents(ignore, mNumEvents, mNumFloatingSpecies);
//    writeEventAssignments(ignore, mNumReactions, mNumEvents);
//    writeEvalInitialAssignments(ignore, mNumReactions);
//    writeTestConstraints(ignore);
//
//    writeInitModelDataFunction(mHeader, mSource);
//    writeInitFunction(mHeader, mSource);
//
//    mHeader<<"\n\n#endif //modelH"<<NL();
//    string modelCode = mHeader.ToString() + mSource.ToString();
//
//    Log(lDebug5)<<" ------ Model Code --------\n"
//            <<modelCode
//            <<" ----- End of Model Code -----\n";
//
//    return modelCode;
//}

static StringList readIndependentSpeciesList(const LibStructural &libs, bool mComputeAndAssignConsevationLaws);
static StringList readDependentSpeciesList(const LibStructural &libs, bool mComputeAndAssignConsevationLaws);
static SymbolList readCompartments(NOMSupport &nom);
static SymbolList readBoundarySpecies(NOMSupport &nom, const SymbolList &compartmentList);
static SymbolList readConservationList(const int& numDependentSpecies);
static SymbolList readFloatingSpeciesConcentrationList(NOMSupport& nom, ls::LibStructural &libs,
        bool mComputeAndAssignConsevationLaws, const SymbolList &mCompartmentList);
static SymbolList readGlobalParameters(NOMSupport &nom);
static SymbolList readModifiableSpeciesReferences(NOMSupport &nom);
static SymbolList readReactionList(NOMSupport &nom);
static vector<int> readLocalParameterDimensions(NOMSupport &nom);
static vector<SymbolList> readLocalParameterList(NOMSupport &nom);
static StringList readFunctionNames(NOMSupport &nom);
static StringList readFunctionParameters(NOMSupport &nom);
static string readModelName(const NOMSupport &nom);
static IntStringHashTable readRateRules(NOMSupport &nom,
        const SymbolList &floatingSpeciesConcentrationList,
        const SymbolList &globalParameterList,
        const SymbolList &boundarySpeciesList,
        const SymbolList &compartmentList,
        const SymbolList &modifiableSpeciesReferenceList);

ModelSymbols::ModelSymbols() :
    mModelName("NameNotSet"),
    mBoundarySpeciesList(),
    mNumBoundarySpecies(0),
    mCompartmentList(),
    mNumCompartments(0),
    mTotalLocalParmeters(0),
    mNumEvents(0),
    mNumGlobalParameters(0),
    mNumIndependentSpecies(0),
    mNumReactions(0),
    mNumModifiableSpeciesReferences(0),
    mFloatingSpeciesConcentrationList(),
    mNumFloatingSpecies(0),
    mNumDependentSpecies(0),
    mGlobalParameterList(),
    mModifiableSpeciesReferenceList(),
    mReactionList(),
    mConservationList(),
    mComputeAndAssignConsevationLaws(false),
    mNumRules(0)
{
}

ModelSymbols& rr::ModelSymbols::operator=(const ModelSymbols& right)
{
    if (this == &right) return *this;
    this->~ModelSymbols();
    new (this) ModelSymbols(right);
    return *this;
}

ModelSymbols::ModelSymbols(NOMSupport& nom, ls::LibStructural& libs, bool computeAndAssignConsevationLaws) :
    mModelName(readModelName(nom)),
    mNumReactions(nom.getNumReactions()),
    mComputeAndAssignConsevationLaws(computeAndAssignConsevationLaws),
    mIndependentSpeciesList(readIndependentSpeciesList(libs,  computeAndAssignConsevationLaws)),
    mNumIndependentSpecies(mIndependentSpeciesList.size()),
    mDependentSpeciesList(readDependentSpeciesList(libs, computeAndAssignConsevationLaws)),
    mCompartmentList(readCompartments(nom)),
    mNumCompartments(mCompartmentList.size()),
    mFloatingSpeciesConcentrationList(readFloatingSpeciesConcentrationList(nom, libs, computeAndAssignConsevationLaws, mCompartmentList)),
    mNumFloatingSpecies(mFloatingSpeciesConcentrationList.size()),
    mNumDependentSpecies(mNumFloatingSpecies - mNumIndependentSpecies),
    mBoundarySpeciesList(readBoundarySpecies(nom, mCompartmentList)),
    mNumBoundarySpecies(mBoundarySpeciesList.size()),
    mGlobalParameterList(readGlobalParameters(nom)),
    mNumGlobalParameters(mGlobalParameterList.size()),
    mModifiableSpeciesReferenceList(readModifiableSpeciesReferences(nom)),
    mNumModifiableSpeciesReferences(mModifiableSpeciesReferenceList.size()),
    mReactionList(readReactionList(nom)),
    mLocalParameterDimensions(readLocalParameterDimensions(nom)),
    mLocalParameterList(readLocalParameterList(nom)),
    mConservationList(readConservationList(mNumDependentSpecies)),
    mTotalLocalParmeters(0), // TODO always set to zero before, is this right???
    mNumEvents(nom.getNumEvents()),
    mFunctionNames(readFunctionNames(nom)),
    mFunctionParameters(readFunctionParameters(nom)),
    mRateRules(readRateRules(nom,
        mFloatingSpeciesConcentrationList,
        mGlobalParameterList,
        mBoundarySpeciesList,
        mCompartmentList,
        mModifiableSpeciesReferenceList)),
   mNumRules(nom.getNumRules())
{
    // nothing to do here, all done in initialization list
}

ModelSymbols::~ModelSymbols()
{
    // nothing to do here, we allocate no heap memory
}

//void ModelSymbols::print()
//{
//
//    cout << "mModelName: " << mModelName << "\n";
//    cout << "mNumReactions: "  << mNumReactions << "\n";
//    cout << "mIndependentSpeciesList:\n";
//    for (int i = 0; i < mIndependentSpeciesList.size(); i++) {
//        cout << "\t" << mIndependentSpeciesList[i] << "\n";
//    }
//    cout << "mNumIndependentSpecies: " <<  mNumIndependentSpecies << "\n";
//    cout << "mDependentSpeciesList\n";
//    for (int i = 0; i <  mDependentSpeciesList.size(); i++) {
//        cout << "\t" << mDependentSpeciesList[i] << "\n";
//    }
//    cout << "mCompartmentList: \n";
//    for (int i = 0; i < mCompartmentList.size(); i++) {
//        cout << "\t" << mCompartmentList[i] << "\n";
//    }
//    /*
//    int mNumCompartments;
//    SymbolList mFloatingSpeciesConcentrationList;
//    int mNumFloatingSpecies;
//    int mNumDependentSpecies;
//    SymbolList mBoundarySpeciesList;
//    int mNumBoundarySpecies;
//    SymbolList mGlobalParameterList;
//    int mNumGlobalParameters;
//    SymbolList mModifiableSpeciesReferenceList;
//    int mNumModifiableSpeciesReferences;
//    SymbolList mReactionList;
//    vector<int> mLocalParameterDimensions;
//    vector<SymbolList> mLocalParameterList;
//    SymbolList mConservationList;
//    int mTotalLocalParmeters;
//    int mNumEvents;
//    StringList mFunctionNames;
//    StringList mFunctionParameters;
//    IntStringHashTable                  mMapRateRule;
//    SymbolList mFloatingSpeciesAmountsList;
//    */
//
//}

static string readModelName(const NOMSupport &nom)
{
    string modelName = nom.getModelName();
    if(!modelName.size())
    {
        Log(lWarning)<<"Model name is empty. ModelName is assigned 'NameNotSet'.";
        modelName = "NameNotSet";
    }
    return modelName;
}




static SymbolList readCompartments(NOMSupport &nom)
{
    // cout << __FUNC__ << "\n";
    SymbolList compartmentList;
    const int numCompartments = nom.getNumCompartments();
    for (u_int i = 0; i < numCompartments; i++)
    {
        string sCompartmentId = nom.getNthCompartmentId(i);
        double value = nom.getValue(sCompartmentId);

        if(isNaN(value))
        {
            value = 1;
        }
        compartmentList.Add(Symbol(sCompartmentId, value));
    }
    return compartmentList;
}



static SymbolList readBoundarySpecies(NOMSupport &nom, const SymbolList &compartmentList)
{
    // cout << __FUNC__ << "\n";
    StringListContainer oBoundarySpecies = nom.getListOfBoundarySpecies();
    const int numBoundarySpecies = oBoundarySpecies.Count(); // sp1.size();
    SymbolList boundarySpeciesList;
    for (int i = 0; i < numBoundarySpecies; i++)
    {
        StringList oTempList     = oBoundarySpecies[i];
        string sName             = oTempList[0];
        string compartmentName     = nom.getNthBoundarySpeciesCompartmentName(i);
        bool bIsConcentration     = toBool(oTempList[2]);
        double dValue             = toDouble(oTempList[1]);
        if (isNaN(dValue))
        {
            dValue = 0;
        }

        Symbol symbol;
        if (bIsConcentration)
        {
            symbol = Symbol(sName, dValue, compartmentName);
        }
        else
        {
            int nCompartmentIndex;
            double dVolume;
            if(compartmentList.find(compartmentName, nCompartmentIndex))
            {
                dVolume = compartmentList[nCompartmentIndex].value;
            }
            else
            {
                if (isNaN(dVolume))
                {
                    dVolume = 1;
                }
            }
            stringstream formula;
            formula<<toString(dValue, ModelSymbols::mDoubleFormat)<<"/ md->compartmentVolumes["<<nCompartmentIndex<<"]";
            symbol = Symbol(sName,
                                dValue / dVolume,
                                compartmentName,
                                formula.str());
        }

        if(nom.getModel())
        {
            const Species* species = nom.getModel()->getSpecies(sName);
            if(species)
            {
                symbol.hasOnlySubstance = species->getHasOnlySubstanceUnits();
                symbol.constant = species->getConstant();
            }
        }
        else
        {
            //TODO: How to report error...?
            //Log an error...
            symbol.hasOnlySubstance = false;

        }
        boundarySpeciesList.Add(symbol);
    }
    return boundarySpeciesList;
}

//int CModelGenerator::readBoundarySpecies()
//{
//    int numBoundarySpecies;
//    StringListContainer oBoundarySpecies = mNOM->getListOfBoundarySpecies();
//    numBoundarySpecies = oBoundarySpecies.Count(); // sp1.size();
//    for (int i = 0; i < numBoundarySpecies; i++)
//    {
//        StringList oTempList     = oBoundarySpecies[i];
//        string sName             = oTempList[0];
//        string compartmentName     = mNOM->getNthBoundarySpeciesCompartmentName(i);
//        bool bIsConcentration     = toBool(oTempList[2]);
//        double dValue             = toDouble(oTempList[1]);
//        if (isNaN(dValue))
//        {
//            dValue = 0;
//        }
//
//        Symbol *symbol = NULL;
//        if (bIsConcentration)
//        {
//            //Todo: memoryleak
//            symbol = new Symbol(sName, dValue, compartmentName);
//        }
//        else
//        {
//            int nCompartmentIndex;
//            double dVolume;
//            if(ms.mCompartmentList.find(compartmentName, nCompartmentIndex))
//            {
//                dVolume = ms.mCompartmentList[nCompartmentIndex].value;
//            }
//            else
//            {
//                if (isNaN(dVolume))
//                {
//                    dVolume = 1;
//                }
//            }
//            stringstream formula;
//            formula<<toString(dValue, ms.mDoubleFormat)<<"/ md->c["<<nCompartmentIndex<<"]";
//            symbol = new Symbol(sName,
//                                dValue / dVolume,
//                                compartmentName,
//                                formula.str());
//        }
//
//        if(mNOM->getModel())
//        {
//            Species* species = mNOM->getModel()->getSpecies(sName);
//            if(species)
//            {
//                symbol->hasOnlySubstance = species->getHasOnlySubstanceUnits();
//                symbol->constant = species->getConstant();
//            }
//        }
//        else
//        {
//            //TODO: How to report error...?
//            //Log an error...
//            symbol->hasOnlySubstance = false;
//
//        }
//        ms.mBoundarySpeciesList.Add(*symbol);
//    }
//    return numBoundarySpecies;
//}


static SymbolList readConservationList(const int& numDependentSpecies)
{
    // cout << __FUNC__ << "\n";
    SymbolList conservationList;
    for (int i = 0; i < numDependentSpecies; i++)
    {
        conservationList.Add(Symbol("CSUM" + toString(i))); //TODO: how to deal with this?
    }
    return conservationList;
}


static StringList readIndependentSpeciesList(const LibStructural &libs, bool mComputeAndAssignConsevationLaws)
{
    // cout << __FUNC__ << "\n";
    //    if(mComputeAndAssignConsevationLaws)
    //    {
    //        mNumIndependentSpecies     = mLibStruct->getNumIndSpecies();
    //        mIndependentSpeciesList = mLibStruct->getIndependentSpecies();
    //        mDependentSpeciesList   = mLibStruct->getDependentSpecies();
    //    }
    //    else
    //    {
    //        mNumIndependentSpecies = mLibStruct->getNumSpecies();
    //        mIndependentSpeciesList = mLibStruct->getSpecies();
    //    }
    return mComputeAndAssignConsevationLaws ? libs.getIndependentSpecies() : libs.getSpecies();
}

static StringList readDependentSpeciesList(const LibStructural &libs, bool mComputeAndAssignConsevationLaws)
{
    // cout << __FUNC__ << "\n";
    return mComputeAndAssignConsevationLaws ? StringList(libs.getDependentSpecies()) : StringList();
}

static SymbolList readFloatingSpeciesConcentrationList(NOMSupport& nom, LibStructural &libs,
        bool mComputeAndAssignConsevationLaws, const SymbolList &mCompartmentList)
{
    // cout << __FUNC__ << "\n";
    IntStringHashTable mapVariables;
    const int numOfRules = nom.getNumRules();

    SymbolList floatingSpeciesConcentrationList;

    // Load a reordered list into the variable list.
    StringList reOrderedList;

    if (mComputeAndAssignConsevationLaws)
    {
        reOrderedList = libs.getReorderedSpecies();
    }
    else
    {
        reOrderedList = libs.getSpecies();
    }

    StringListContainer oFloatingSpecies = nom.getListOfFloatingSpecies();

    for (int i = 0; i < reOrderedList.size(); i++)
    {
        for (int j = 0; j < oFloatingSpecies.Count(); j++)
        {
            StringList oTempList = oFloatingSpecies[j];
            if (reOrderedList[i] != (const string&) oTempList[0])
            {
                continue;
            }

            string compartmentName = nom.getNthFloatingSpeciesCompartmentName(j);
            bool bIsConcentration = toBool(oTempList[2]);
            double dValue = toDouble(oTempList[1]);
            if (isNaN(dValue))
            {
                dValue = 0;
            }

            Symbol symbol;
            if (bIsConcentration)
            {
                symbol = Symbol(reOrderedList[i], dValue, compartmentName);
            }
            else
            {
                int nCompartmentIndex;
                mCompartmentList.find(compartmentName, nCompartmentIndex);

                double dVolume = mCompartmentList[nCompartmentIndex].value;
                if (isNaN(dVolume))
                {
                    dVolume = 1;
                }

                stringstream formula;
                formula << toString(dValue, ModelSymbols::mDoubleFormat)
                            << "/ md->compartmentVolumes[" << nCompartmentIndex << "]";

                symbol = Symbol(reOrderedList[i], dValue / dVolume,
                        compartmentName, formula.str());
            }

            if (nom.getModel())
            {
                const Species *aSpecies = nom.getModel()->getSpecies(reOrderedList[i]);
                if (aSpecies)
                {
                    symbol.hasOnlySubstance = aSpecies->getHasOnlySubstanceUnits();
                    symbol.constant = aSpecies->getConstant();
                }
            }
            else
            {
                //TODO: How to report error...?
                //Log an error...
                symbol.hasOnlySubstance = false;
            }
            Log(lDebug5) << "Adding symbol to floatingSpeciesConcentrationList:" << (symbol);
            floatingSpeciesConcentrationList.Add(symbol);
            break;
        }
    }


    for (int i = 0; i < numOfRules; i++)
    {
        try
        {
            string ruleType = nom.getNthRuleType(i);

            // We only support assignment and ode rules at the moment
            string eqnRule = nom.getNthRule(i);
            RRRule aRule(eqnRule, ruleType);
            string varName = trim(aRule.GetLHS());

            bool isRateRule = false;

            if (aRule.GetType() == rtRate)
            {
                isRateRule = true;
                int index;
                if (floatingSpeciesConcentrationList.find(varName, index))
                {
                    floatingSpeciesConcentrationList[index].rateRule = true;
                }
                else
                {
                    //leftSideRule = "\n\tmd->rateRuleRates[" + toString(numRateRules) + "]";
                    //ms.mRateRules[numRateRules] = findSymbol(varName);
                    //mapVariables[numRateRules] = varName;
                    //numRateRules++;
                }
            }
        }
        catch (const Exception& e)
        {
            throw CoreException("Error while trying to get Rule #" + toString(i) + e.Message());
        }
    }




    return floatingSpeciesConcentrationList;
}
//
//int CModelGenerator::readFloatingSpecies()
//{
//    // Load a reordered list into the variable list.
//    StringList reOrderedList;
//
//    if(mComputeAndAssignConsevationLaws)
//    {
//       reOrderedList = mLibStruct->getReorderedSpecies();
//    }
//    else
//    {
//        reOrderedList = mLibStruct->getSpecies();
//    }
//
//    StringListContainer oFloatingSpecies = mNOM->getListOfFloatingSpecies();
//
//    for (int i = 0; i < reOrderedList.Count(); i++)
//    {
//        for (int j = 0; j < oFloatingSpecies.Count(); j++)
//        {
//            StringList oTempList = oFloatingSpecies[j];
//              if(reOrderedList[i] != (const string&) oTempList[0])
//              {
//                  continue;
//              }
//
//            string compartmentName = mNOM->getNthFloatingSpeciesCompartmentName(j);
//            bool bIsConcentration  = toBool(oTempList[2]);
//            double dValue = toDouble(oTempList[1]);
//            if (isNaN(dValue))
//            {
//                  dValue = 0;
//            }
//
//            Symbol *symbol = NULL;
//            if (bIsConcentration)
//            {
//              symbol = new Symbol(reOrderedList[i], dValue, compartmentName);
//            }
//            else
//            {
//              int nCompartmentIndex;
//              ms.mCompartmentList.find(compartmentName, nCompartmentIndex);
//
//              double dVolume = ms.mCompartmentList[nCompartmentIndex].value;
//              if (isNaN(dVolume))
//              {
//                dVolume = 1;
//              }
//
//              stringstream formula;
//              formula<<toString(dValue,ms.mDoubleFormat)<<"/ md->c["<<nCompartmentIndex<<"]";
//
//              symbol = new Symbol(reOrderedList[i],
//                  dValue / dVolume,
//                  compartmentName,
//                  formula.str());
//            }
//
//            if(mNOM->getModel())
//            {
//                Species *aSpecies = mNOM->getModel()->getSpecies(reOrderedList[i]);
//                if(aSpecies)
//                {
//                    symbol->hasOnlySubstance = aSpecies->getHasOnlySubstanceUnits();
//                    symbol->constant = aSpecies->getConstant();
//                }
//            }
//            else
//            {
//                //TODO: How to report error...?
//                //Log an error...
//                symbol->hasOnlySubstance = false;
//            }
//            Log(lDebug5)<<"Adding symbol to ms.mFloatingSpeciesConcentrationList:"<<(*symbol);
//            ms.mFloatingSpeciesConcentrationList.Add(*(symbol));
//            delete symbol;
//            break;
//          }
//          //throw RRException("Reordered Species " + reOrderedList[i] + " not found.");
//      }
//      return oFloatingSpecies.Count();
//}
//


static string findSymbol(const string& varName,
                         const SymbolList &floatingSpeciesConcentrationList,
                         const SymbolList &globalParameterList,
                         const SymbolList &boundarySpeciesList,
                         const SymbolList &compartmentList,
                         const SymbolList &modifiableSpeciesReferenceList)
{
      int index = 0;
      if (floatingSpeciesConcentrationList.find(varName, index))
      {
          return format("md->floatingSpeciesConcentrations[{0}]", index);
      }
      else if (globalParameterList.find(varName, index))
      {
          return format("md->globalParameters[{0}]", index);
      }
      else if (boundarySpeciesList.find(varName, index))
      {
          return format("md->boundarySpeciesConcentrations[{0}]", index);
      }
      else if (compartmentList.find(varName, index))
      {
          return format("md->compartmentVolumes[{0}]", index);
      }
      else if (modifiableSpeciesReferenceList.find(varName, index))
      {
          return format("md->sr[{0}]", index);
      }
      else
      {
          throw Exception(format("Unable to locate lefthand side symbol in assignment[{0}]", varName));
      }
}

static IntStringHashTable readRateRules(NOMSupport &nom,
        const SymbolList &floatingSpeciesConcentrationList,
        const SymbolList &globalParameterList,
        const SymbolList &boundarySpeciesList,
        const SymbolList &compartmentList,
        const SymbolList &modifiableSpeciesReferenceList)
{
    IntStringHashTable rateRules;
    const int numOfRules = nom.getNumRules();
    int numRateRules = 0;

    for (int i = 0; i < numOfRules; i++)
    {
        try
        {
            string ruleType = nom.getNthRuleType(i);

            // We only support assignment and ode rules at the moment
            string eqnRule = nom.getNthRule(i);
            RRRule aRule(eqnRule, ruleType);
            string varName = trim(aRule.GetLHS());

            bool isRateRule = false;

            if (aRule.GetType() == rtRate)
            {
                isRateRule = true;
                int index;
                if (!floatingSpeciesConcentrationList.find(varName, index))
                {
                    rateRules[numRateRules++] = findSymbol(varName,
                            floatingSpeciesConcentrationList,
                            globalParameterList, boundarySpeciesList,
                            compartmentList, modifiableSpeciesReferenceList);
                }
            }
        }
        catch (const Exception& e)
        {
            throw CoreException("Error while trying to get Rule #" + toString(i) + e.Message());
        }
    }

    return rateRules;
}




static SymbolList readGlobalParameters(NOMSupport &nom)
{
    // cout << __FUNC__ << "\n";
    SymbolList gblobalParameterList;

    int numGlobalParameters;
    StringListContainer oParameters = nom.getListOfParameters();
    numGlobalParameters = oParameters.Count();
    for (u_int i = 0; i < numGlobalParameters; i++)
    {
        StringList parameter = oParameters[i];

        string name = parameter[0];
        double value = toDouble(parameter[1]);
        Symbol aSymbol(name, value);
        Log(lDebug5) << "Adding symbol" << aSymbol
                << " to global parameters";

        gblobalParameterList.Add(aSymbol);
    }
    return gblobalParameterList;
}

//int ModelGenerator::readGlobalParameters()
//{
//    CHECK_LIB_NOM();
//
//    int numGlobalParameters;
//    ArrayList oParameters = mNOM->getListOfParameters();
//    numGlobalParameters = oParameters.Count();
//    for (u_int i = 0; i < numGlobalParameters; i++)
//    {
//        StringList parameter = oParameters[i];
//
//        string name     = parameter[0];
//        double value     = toDouble(parameter[1]);
//        Symbol aSymbol(name, value);
//        Log(lDebug5)<<"Adding symbol"<<aSymbol<<" to global parameters";
//
//        ms.mGlobalParameterList.Add(aSymbol);
//    }
//    return numGlobalParameters;
//}


static SymbolList readModifiableSpeciesReferences(NOMSupport &nom)
{
    // cout << __FUNC__ << "\n";
    SymbolList modifiableSpeciesReferenceList;

    if(!nom.getSBMLDocument())
    {
        throw Exception("nom.getSBMLDocument() is NULL");
    }
    SBMLDocument &SBMLDoc = *nom.getSBMLDocument();
    Model &SbmlModel  = *nom.getModel();

    if(nom.getSBMLDocument()->getLevel() < 3)
    {
        // return empty list
        return modifiableSpeciesReferenceList;
    }

    string id;
    double value;
    int numReactions = SbmlModel.getNumReactions();
    for (u_int i = 0; i < numReactions; i++)
    {
        Reaction &reaction = *(SbmlModel.getReaction(i));
        for (u_int j = 0; j < reaction.getNumReactants(); j++)
        {
            SpeciesReference &reference = *(reaction.getReactant(j));
            id = reference.getId();
            if (!(id.size()))
            {
                continue;
            }
            value = reference.getStoichiometry();
            if (isNaN(value))
                value = 1;

            if (reference.isSetId())
            {
                modifiableSpeciesReferenceList.Add(Symbol(id, value));
            }
        }
        for (u_int j = 0; j < reaction.getNumProducts(); j++)
        {
            SpeciesReference &reference = *(reaction.getProduct(j));
            id = reference.getId();
            if (isNullOrEmpty(id))
            {
                continue;
            }
            value = reference.getStoichiometry();
            if (isNaN(value))
            {
                value = 1;
            }

            if (reference.isSetId())
            {
                modifiableSpeciesReferenceList.Add(Symbol(id, value));
            }
        }
    }
    return modifiableSpeciesReferenceList;
}


// readLocalParameters(mNumReactions, mLocalParameterDimensions, mTotalLocalParmeters);
//Todo: totalLocalParmeters is not used
static SymbolList readReactionList(NOMSupport &nom)
{
    // cout << __FUNC__ << "\n";
    SymbolList reactionList;
    const int numReactions = nom.getNumReactions();
    for (int i = 0; i < numReactions; i++)
    {
        reactionList.Add(Symbol(nom.getNthReactionId(i), 0.0));
    }
    return reactionList;
}

static vector<int> readLocalParameterDimensions(NOMSupport &nom)
{
    // cout << __FUNC__ << "\n";
    vector<int> localParameterDimensions;
    const int numReactions = nom.getNumReactions();
    localParameterDimensions.resize(numReactions);
    for (int i = 0; i < numReactions; i++)
    {
        localParameterDimensions[i] = nom.getNumParameters(i);
    }
    return localParameterDimensions;
}

static vector<SymbolList> readLocalParameterList(NOMSupport &nom)
{
    // cout << __FUNC__ << "\n";
    vector<SymbolList> localParameterList;
    const int numReactions = nom.getNumReactions();
    localParameterList.resize(numReactions);

    for (int i = 0; i < numReactions; i++)
    {
        const int numLocalParameters = nom.getNumParameters(i);
        string reactionName = nom.getNthReactionId(i);
        SymbolList newList;
        for (u_int j = 0; j < numLocalParameters; j++)
        {
            string name = nom.getNthParameterId(i, j);
            double value = nom.getNthParameterValue(i, j);
            newList.Add(Symbol(reactionName, name, value));
        }
        localParameterList.push_back(newList);
    }
    return localParameterList;
}

////Todo: totalLocalParmeters is not used
//void ModelGenerator::readLocalParameters(const int& numReactions,  vector<int>& localParameterDimensions, int& totalLocalParmeters)
//{
//    CHECK_LIB_NOM();
//
//    string name;
//    double value;
//    int numLocalParameters;
//    totalLocalParmeters = 0;
//    string reactionName;
//    localParameterDimensions.resize(numReactions);
//    for (int i = 0; i < numReactions; i++)
//    {
//        numLocalParameters = mNOM->getNumParameters(i);
//        reactionName = mNOM->getNthReactionId(i);
//        ms.mReactionList.Add(Symbol(reactionName, 0.0));
//        SymbolList newList;
//        for (u_int j = 0; j < numLocalParameters; j++)
//        {
//            localParameterDimensions[i] = numLocalParameters;
//            name = mNOM->getNthParameterId(i, j);
//            value = mNOM->getNthParameterValue(i, j);
//            newList.Add(Symbol(reactionName, name, value));
//        }
//        ms.mLocalParameterList.push_back(newList);
//    }
//}



StringList readFunctionNames(NOMSupport &nom)
{
    // cout << __FUNC__ << "\n";
    StringList mFunctionNames;
    for (int i = 0; i < nom.getNumFunctionDefinitions(); i++)
    {
        try
        {
            StringListContainer oList = nom.getNthFunctionDefinition(i);
            StringList aList = oList[0];

            string sName = aList[0];
            mFunctionNames.add(sName);
        }
        catch (const Exception& ex)
        {
            std::stringstream ss;
            ss << "Error while trying to get Function Definition # ";
            ss << i << ", what: " << ex.what();
            throw Exception(ss.str());
        }
    }
    return mFunctionNames;
}

StringList readFunctionParameters(NOMSupport &nom)
{
    // cout << __FUNC__ << "\n";
    StringList mFunctionParameters;
    for (int i = 0; i < nom.getNumFunctionDefinitions(); i++)
    {
        try
        {
            StringListContainer oList = nom.getNthFunctionDefinition(i);
            StringList oArguments = oList[1];

            for (int j = 0; j < oArguments.size(); j++)
            {
                mFunctionParameters.add((string) oArguments[j]);
            }
        }
        catch (const Exception& ex)
        {
            std::stringstream ss;
            ss << "Error while trying to get Function Definition # ";
            ss << i << ", what: " << ex.what();
            throw Exception(ss.str());
        }
    }
    return mFunctionParameters;
}

// for the time being, copy these here to have quick look to see if anything is wrong.

//int ModelGenerator::readCompartments()
//{
//    CHECK_LIB_NOM();
//
//    int numCompartments = mNOM->getNumCompartments();
//    for (u_int i = 0; i < numCompartments; i++)
//    {
//        string sCompartmentId = mNOM->getNthCompartmentId(i);
//        double value = mNOM->getValue(sCompartmentId);
//
//        if(isNaN(value))
//        {
//            value = 1;
//        }
//        ms.mCompartmentList.Add(Symbol(sCompartmentId, value));
//    }
//    return numCompartments;
//}




} /* namespace rr */

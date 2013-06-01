/*
 * rrModelSymbols.cpp
 *
 *  Created on: May 31, 2013
 *      Author: andy
 */

#include "rrModelSymbols.h"
#include "rrStringUtils.h"
#include "sbml/Model.h"
#include "sbml/SBMLDocument.h"
#include "rrLogger.h"
#include "rrException.h"
#include "rrOSSpecifics.h"
#include <sstream>

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

static StringList readIndependentSpeciesList(ls::LibStructural &libs, bool mComputeAndAssignConsevationLaws);
static StringList readDependentSpeciesList(ls::LibStructural &libs, bool mComputeAndAssignConsevationLaws);
static SymbolList readCompartments(NOMSupport &nom);
static SymbolList readBoundarySpecies(NOMSupport &nom, SymbolList &compartmentList);
static SymbolList readConservationList(const int& numDependentSpecies);
static SymbolList readFloatingSpeciesConcentrationList(NOMSupport& nom, ls::LibStructural &libs,
        bool mComputeAndAssignConsevationLaws, SymbolList &mCompartmentList);
static SymbolList readGlobalParameters(NOMSupport &nom);
static SymbolList readModifiableSpeciesReferences(NOMSupport &nom);
static SymbolList readReactionList(NOMSupport &nom);
static vector<int> readLocalParameterDimensions(NOMSupport &nom);
static vector<SymbolList> readLocalParameterList(NOMSupport &nom);
static StringList readFunctionNames(NOMSupport &nom);
static StringList readFunctionParameters(NOMSupport &nom);

ModelSymbols::ModelSymbols() :
    mNumBoundarySpecies(0),
    mNumCompartments(0),
    mTotalLocalParmeters(0),
    mNumEvents(0),
    mNumGlobalParameters(0),
    mNumIndependentSpecies(0),
    mNumReactions(0),
    mNumModifiableSpeciesReferences(0),
    mNumFloatingSpecies(0),
    mNumDependentSpecies(0)
{
}

ModelSymbols::ModelSymbols(NOMSupport& nom, ls::LibStructural& libs, bool computeAndAssignConsevationLaws) :
    mModelName(nom.getModelName()),
    mNumReactions(nom.getNumReactions()),
    mIndependentSpeciesList(readIndependentSpeciesList(libs,  computeAndAssignConsevationLaws)),
    mNumIndependentSpecies(mIndependentSpeciesList.Count()),
    mDependentSpeciesList(readDependentSpeciesList(libs, computeAndAssignConsevationLaws)),
    mCompartmentList(readCompartments(nom)),
    mNumCompartments(mCompartmentList.Count()),
    mFloatingSpeciesConcentrationList(readFloatingSpeciesConcentrationList(nom, libs, computeAndAssignConsevationLaws, mCompartmentList)),
    mNumFloatingSpecies(mFloatingSpeciesConcentrationList.Count()),
    mNumDependentSpecies(mNumFloatingSpecies - mNumIndependentSpecies),
    mBoundarySpeciesList(readBoundarySpecies(nom, mCompartmentList)),
    mNumBoundarySpecies(mBoundarySpeciesList.Count()),
    mGlobalParameterList(readGlobalParameters(nom)),
    mNumGlobalParameters(mGlobalParameterList.Count()),
    mModifiableSpeciesReferenceList(readModifiableSpeciesReferences(nom)),
    mNumModifiableSpeciesReferences(mModifiableSpeciesReferenceList.Count()),
    mReactionList(readReactionList(nom)),
    mLocalParameterDimensions(readLocalParameterDimensions(nom)),
    mLocalParameterList(readLocalParameterList(nom)),
    mConservationList(readConservationList(mNumDependentSpecies)),
    mTotalLocalParmeters(0), // TODO always set to zero before, is this right???
    mNumEvents(nom.getNumEvents()),
    mFunctionNames(readFunctionNames(nom)),
    mFunctionParameters(readFunctionParameters(nom))
{
    // TODO Auto-generated constructor stub
    //mNumDependentSpecies(mDependentSpeciesList.Count()),
    int i = 0;
    int j = i + 1;
    cout << j;

    cout << __FUNC__ << "\n";

}

ModelSymbols::~ModelSymbols()
{
    // TODO Auto-generated destructor stub
}

void ModelSymbols::print()
{

    cout << "mModelName: " << mModelName << "\n";
    cout << "mNumReactions: "  << mNumReactions << "\n";
    cout << "mIndependentSpeciesList:\n";
    for (int i = 0; i < mIndependentSpeciesList.Count(); i++) {
        cout << "\t" << mIndependentSpeciesList[i] << "\n";
    }
    cout << "mNumIndependentSpecies: " <<  mNumIndependentSpecies << "\n";
    cout << "mDependentSpeciesList\n";
    for (int i = 0; i <  mDependentSpeciesList.Count(); i++) {
        cout << "\t" << mDependentSpeciesList[i] << "\n";
    }
    cout << "mCompartmentList: \n";
    for (int i = 0; i < mCompartmentList.Count(); i++) {
        cout << "\t" << mCompartmentList[i] << "\n";
    }
    /*
    int mNumCompartments;
    SymbolList mFloatingSpeciesConcentrationList;
    int mNumFloatingSpecies;
    int mNumDependentSpecies;
    SymbolList mBoundarySpeciesList;
    int mNumBoundarySpecies;
    SymbolList mGlobalParameterList;
    int mNumGlobalParameters;
    SymbolList mModifiableSpeciesReferenceList;
    int mNumModifiableSpeciesReferences;
    SymbolList mReactionList;
    vector<int> mLocalParameterDimensions;
    vector<SymbolList> mLocalParameterList;
    SymbolList mConservationList;
    int mTotalLocalParmeters;
    int mNumEvents;
    StringList mFunctionNames;
    StringList mFunctionParameters;
    IntStringHashTable                  mMapRateRule;
    SymbolList mFloatingSpeciesAmountsList;
    */

}



static SymbolList readCompartments(NOMSupport &nom)
{
    cout << __FUNC__ << "\n";
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



static SymbolList readBoundarySpecies(NOMSupport &nom, SymbolList &compartmentList)
{
    cout << __FUNC__ << "\n";
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
            formula<<toString(dValue, ModelSymbols::mDoubleFormat)<<"/ md->c["<<nCompartmentIndex<<"]";
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


static SymbolList readConservationList(const int& numDependentSpecies)
{
    cout << __FUNC__ << "\n";
    SymbolList conservationList;
    for (int i = 0; i < numDependentSpecies; i++)
    {
        conservationList.Add(Symbol("CSUM" + toString(i))); //TODO: how to deal with this?
    }
    return conservationList;
}


static StringList readIndependentSpeciesList(ls::LibStructural &libs, bool mComputeAndAssignConsevationLaws)
{
    cout << __FUNC__ << "\n";
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

static StringList readDependentSpeciesList(ls::LibStructural &libs, bool mComputeAndAssignConsevationLaws)
{
    cout << __FUNC__ << "\n";
    return mComputeAndAssignConsevationLaws ? libs.getDependentSpecies() : StringList();
}

static SymbolList readFloatingSpeciesConcentrationList(NOMSupport& nom, ls::LibStructural &libs,
        bool mComputeAndAssignConsevationLaws, SymbolList &mCompartmentList)
{
    cout << __FUNC__ << "\n";
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

    for (int i = 0; i < reOrderedList.Count(); i++)
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

            Symbol symbol;;
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
                            << "/ md->c[" << nCompartmentIndex << "]";

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
    return floatingSpeciesConcentrationList;
}

static SymbolList readGlobalParameters(NOMSupport &nom)
{
    cout << __FUNC__ << "\n";
    SymbolList gblobalParameterList;

    int numGlobalParameters;
    ArrayList oParameters = nom.getListOfParameters();
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


static SymbolList readModifiableSpeciesReferences(NOMSupport &nom)
{
    cout << __FUNC__ << "\n";
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
    cout << __FUNC__ << "\n";
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
    cout << __FUNC__ << "\n";
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
    cout << __FUNC__ << "\n";
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

StringList readFunctionNames(NOMSupport &nom)
{
    cout << __FUNC__ << "\n";
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
    cout << __FUNC__ << "\n";
    StringList mFunctionParameters;
    for (int i = 0; i < nom.getNumFunctionDefinitions(); i++)
    {
        try
        {
            StringListContainer oList = nom.getNthFunctionDefinition(i);
            StringList oArguments = oList[1];

            for (int j = 0; j < oArguments.Count(); j++)
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




} /* namespace rr */

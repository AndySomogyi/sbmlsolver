/*
 * rrModelSymbols.h
 *
 *  Created on: May 31, 2013
 *      Author: andy
 */

#ifndef RRMODELSYMBOLS_H_
#define RRMODELSYMBOLS_H_

#include "rrSymbolList.h"
#include "rrHashTable.h"
#include "rrNOMSupport.h"
#include "rr-libstruct/lsLibStructural.h"

namespace rr
{

class ModelSymbols
{
public:
    ModelSymbols();
    ModelSymbols(NOMSupport& nom, ls::LibStructural& libs,
            bool computeAndAssignConsevationLaws);
    virtual ~ModelSymbols();

    string mModelName;
    int mNumReactions;
    StringList mIndependentSpeciesList;
    int mNumIndependentSpecies;
    StringList mDependentSpeciesList;
    SymbolList mCompartmentList;
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
    IntStringHashTable                  mRateRules;
    SymbolList mFloatingSpeciesAmountsList;
    static const string                 mDoubleFormat;



    void print();

};

} /* namespace rr */
#endif /* RRMODELSYMBOLS_H_ */

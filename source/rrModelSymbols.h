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

class RR_DECLSPEC ModelSymbols
{
public:
    ModelSymbols();
    ModelSymbols(NOMSupport& nom, ls::LibStructural& libs,
                 bool computeAndAssignConsevationLaws);

    /**
     * have to define assignment operator if you want
     * assignable types with const member vars.
     */
    ModelSymbols& operator=(const ModelSymbols&);

    virtual ~ModelSymbols();

    /**
     * ! CRITICALLY IMPORTANT !
     * THE ORDER OF THE MEMBER VARIABLE DEFINITIONS BELOW HAVE TO BE IN THIS ORDER.
     *
     * This class is created entierly in the initialization list, and the initialization
     * list order is determined by the order of variable definitions. Many variables
     * depend on previous variables. A little squirly, but thats the price to pay
     * for having all const member vars.
     */

    /**
     * name of the model
     */
    const string mModelName;

    const int mNumReactions;

    /**
     * if we computeAndAssignConsevationLaws, this is the actual Independent
     * species, otherwise, its just species.
     *
     * Independent species are the linearly independent species in the
     * stochiometry matrix.
     */
    const StringList mIndependentSpeciesList;
    const int mNumIndependentSpecies;

    /**
     * copied straight from the NOM
     */
    const StringList mDependentSpeciesList;

    const SymbolList mCompartmentList;
    const int mNumCompartments;

    const SymbolList mFloatingSpeciesConcentrationList;
    const int mNumFloatingSpecies;

    const int mNumDependentSpecies;

    const SymbolList mBoundarySpeciesList;
    const int mNumBoundarySpecies;

    const SymbolList mGlobalParameterList;
    const int mNumGlobalParameters;

    const SymbolList mModifiableSpeciesReferenceList;
    const int mNumModifiableSpeciesReferences;

    const SymbolList mReactionList;

    const vector<int> mLocalParameterDimensions;
    const vector<SymbolList> mLocalParameterList;

    const SymbolList mConservationList;

    const int mTotalLocalParmeters;
    const int mNumEvents;

    const StringList mFunctionNames;
    const StringList mFunctionParameters;

    const IntStringHashTable mRateRules;

    static const string mDoubleFormat;

};

} /* namespace rr */
#endif /* RRMODELSYMBOLS_H_ */

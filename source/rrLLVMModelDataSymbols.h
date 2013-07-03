/*
 * rrLLVMModelDataSymbols.h
 *
 * Created on: Jun 28, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef LLVMModelDataSymbolsH
#define LLVMModelDataSymbolsH

#include "rrModelData.h"
#include "rrExecutableModel.h"

#include <map>

namespace libsbml { class Model; }

namespace rr
{

enum ModelDataFields {
    Size = 0,
    ModelName,
    Time,
    NumIndependentSpecies,
    NumDependentSpecies,
    DependentSpeciesConservedSums,
    NumGlobalParameters,
    GlobalParameters,
    NumReactions,
    ReactionRates,
    LocalParametersOffsets,
    LocalParametersNum,
    LocalParameters,
    NumFloatingSpecies,
    FloatingSpeciesConcentrations,
    FloatingSpeciesInitConcentrations,
    FloatingSpeciesConcentrationRates,
    FloatingSpeciesAmounts,
    FloatingSpeciesCompartments,
    NumBoundarySpecies,
    BoundarySpeciesConcentrations,
    BoundarySpeciesCompartments,
    NumCompartments,
    CompartmentVolumes,
    NumRateRules,
    RateRules,
    NumEvents,
    EventTypeSize,
    EventType,
    EventPersistentTypeSize,
    EventPersistentType,
    EventTestsSize,
    EventTests,
    EventPrioritiesSize,
    EventPriorities,
    EventStatusArraySize,
    EventStatusArray,
    PreviousEventStatusArraySize,
    PreviousEventStatusArray,
    EventDelays,
    EventAssignments,
    ComputeEventAssignments,
    PerformEventAssignments,
    VariableTable,
    BoundaryTable,
    GlobalParameterTable,
    SrSize,
    Sr
};

class LLVMModelDataSymbols
{
public:

    typedef std::map<std::string, int> StringIntMap;
    typedef std::pair<std::string, int> StringIntPair;

    LLVMModelDataSymbols();

    LLVMModelDataSymbols(libsbml::Model const* model,
            bool computeAndAssignConsevationLaws);

    virtual ~LLVMModelDataSymbols();



    int getCompartmentIndex(std::string const&) const;
    int getFloatingSpeciesIndex(std::string const&) const;
    int getBoundarySpeciesIndex(std::string const&) const;
    int getFloatingSpeciesCompartmentIndex(std::string const&) const;
    int getBoundarySpeciesCompartmentIndex(std::string const&) const;
    int getGlobalParameterIndex(std::string const&) const;
    std::vector<std::string> getGlobalParameterIds() const;
    std::vector<std::string> getFloatingSpeciesIds() const;
    std::vector<std::string> getCompartmentIds() const;
    std::vector<std::string> getBoundarySpeciesIds() const;




//     /**
//      * number of linearly independent rows in the stochiometry matrix.
//      */
//     int                                 numIndependentSpecies;
//
//     /**
//      * number of linerly dependent rows in the stoichiometry matrix.
//      *
//      * numIndependentVariables + numDependentVariables had better
//      * be equal to numFloatingSpecies
//      */
//     int                                 numDependentSpecies;
//     double*                             dependentSpeciesConservedSums;
//
//     /**
//      * number of global parameters
//      */
//     int                                 numGlobalParameters;
//     double*                             globalParameters;
//
//     /**
//      * number of reactions, same as ratesSize.
//      */
//     int                                 numReactions;
//     double*                             reactionRates;
//
//     /**
//      * LLVM specific
//      * C version does not support local parameters
//      * This is the offset, or starting index of the local parameters
//      * for reaction i. Length is numReactions.
//      *
//      * Rationale: It is simple more effecient to store all the local
//      * parameters in a single dimensional array with offsets, as the
//      * offsets can be computed at compile time, whereas if we used
//      * a array of arrays, it would require an additional memory access
//      * to determine the location of the parameter.
//      */
//     int*                                localParametersOffsets;
//
//     /**
//      * the number of local parameters for each reaction,
//      * so legnth is numReactions. This is an array of counts,
//      * hence it is named differently than the rest of the num*** fields.
//      */
//     int*                                localParametersNum;
//
//     /**
//      * All local parameters are stored in this array. This has
//      * length sum(localParameterNum).
//      */
//     double*                             localParameters;
//
//     /**
//      * The total ammounts of the floating species, i.e.
//      * concentration * compartment volume.
//      * Everything named floatingSpecies??? has length numFloatingSpecies.
//      *
//      * Note, the floating species consist of BOTH independent AND dependent
//      * species. Indexes [0,numIndpendentSpecies) values are the indenpendent
//      * species, and the [numIndependentSpecies,numIndendentSpecies+numDependentSpecies)
//      * contain the dependent species.
//      */
//     int                                 numFloatingSpecies;
//
//     /**
//      * number of floating species and floating species concentrations.
//      */
//     double*                             floatingSpeciesConcentrations;
//
//     /**
//      * initial concentration values for floating species.
//      */
//     double*                             floatingSpeciesInitConcentrations;
//
//     /**
//      * concentration rates of change for floating species.
//      */
//     double*                             floatingSpeciesConcentrationRates;
//
//     /**
//      * The total amount of a species in a compartment.
//      */
//     double*                             floatingSpeciesAmounts;
//
//     /**
//      * compartment index for each floating species,
//      * e.g. the volume of the i'th species is
//      * md->compartmentVolumes[md->floatingSpeciesCompartments[i]]
//      */
//     int*                                floatingSpeciesCompartments;
//
//     /**
//      * number of boundary species and boundary species concentrations.
//      * units: either
//      * Mass Percent = (Mass of Solute) / (Mass of Solution) x 100%
//      * Volume Percent= (Volume of Solute) / (Volume of Solution) x 100%
//      * Mass/Volume Percent= (Mass of Solute) / (Volume of Solution) x 100%
//      */
//     int                                 numBoundarySpecies;
//     double*                             boundarySpeciesConcentrations;
//
//     /**
//      * compartment index for each boundary species,
//      * e.g. the volume of the i'th species is
//      * md->compartmentVolumes[md->boundarySpeciesCompartments[i]]
//      */
//     int*                                boundarySpeciesCompartments;
//
//     /**
//      * number of compartments, and compartment volumes.
//      * units: volume
//      */
//     int                                 numCompartments;
//     double*                             compartmentVolumes;
//
//
//     int                                 numRateRules;
//     double*                             rateRules;
//
//
//     //Event stuff
//     int                                 numEvents;
//     int                                 eventTypeSize;
//     bool*                               eventType;
//
//     int                                 eventPersistentTypeSize;
//     bool*                               eventPersistentType;
//
//     int                                 eventTestsSize;
//     double*                             eventTests;
//
//     int                                 eventPrioritiesSize;
//     double*                             eventPriorities;
//
//     int                                 eventStatusArraySize;
//     bool*                               eventStatusArray;
//
//     int                                 previousEventStatusArraySize;
//     bool*                               previousEventStatusArray;
//
//     TEventDelayDelegate*                eventDelays;
//     TEventAssignmentDelegate*           eventAssignments;
//
//     TComputeEventAssignmentDelegate*    computeEventAssignments;
//     TPerformEventAssignmentDelegate*    performEventAssignments;
//
//     /**
//      * Is set by the model to the names of the FloatingSpeciesConcentrationList.
//      * The model should set each variableTable[i] to a static null terminated string.
//      * allocModelDataBuffers should allocate space for numTotalVariables.
//      * strings.
//      */
//     char**                              variableTable;
//
//     /**
//      * names of boundary table species, set by the model to a static string.
//      * allocModelDataBuffers should allocate numBoundaryVariables length char** array.
//      */
//     char**                              boundaryTable;
//
//     /**
//      * names of global parameters. populated by the model.
//      * allocModelDataBuffers should allocate length numGlobalParameters  char** array.
//      */
//     char**                              globalParameterTable;
//
//     /**
//      * C species references,
//      * not working correctly...
//      */
//     int                                 srSize;
//     double*                             sr;
//
//     /**
//      * Looks like these were going to be C local variables, but were
//      * never implemented...
//      */
//     //int                                 lpSize;
//     //double*                             lp;
//     //int                                 localParameterDimensionsSize;
//     //int*                                localParameterDimensions;


    void initAllocModelDataBuffers(ModelData& m) const;

    void print() const;



private:

    std::string modelName;
    StringIntMap floatingSpeciesMap;
    StringIntMap boundarySpeciesMap;
    StringIntMap compartmentsMap;
    StringIntMap globalParametersMap;
    std::vector<int> floatingSpeciesCompartments;
    std::vector<int> boundarySpeciesCompartments;


};

} /* namespace rr */
#endif /* RRLLVMMODELDATASYMBOLS_H_ */

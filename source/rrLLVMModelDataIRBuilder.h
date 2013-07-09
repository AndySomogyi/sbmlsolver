/*
 * rrLLVMModelDataSymbols.h
 *
 * Created on: Jun 28, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef rrLLVMModelDataIRBuilderH
#define rrLLVMModelDataIRBuilderH

#include "rrLLVMIncludes.h"
#include "rrLLVMModelDataSymbols.h"

#include <map>

namespace libsbml { class Model; }

namespace rr
{

class LLVMModelDataIRBuilder
{
public:

    LLVMModelDataIRBuilder(LLVMModelDataSymbols const&,
            llvm::IRBuilder<> * = 0);

    virtual ~LLVMModelDataIRBuilder();

    llvm::Value *createFloatSpeciesConcGEP(llvm::Value *s, const std::string &id);

    llvm::Value *createFloatSpeciesConcStore(llvm::Value *s, const std::string &id,
            llvm::Value *value);

    llvm::Value *createFloatSpeciesAmtGEP(llvm::Value *s, const std::string &id);

    llvm::Value *createFloatSpeciesAmtStore(llvm::Value *modelData, const std::string &id, llvm::Value *value);
    llvm::Value *createFloatSpeciesConcFromAmtLoad(llvm::Value *s, const std::string &id);
    llvm::Value *createFloatSpeciesAmtFromConcLoad(llvm::Value *s, const std::string &id);

    llvm::Value *createFloatSpeciesCompGEP(llvm::Value *s, const std::string &id);

    llvm::Value *createGlobalParamGEP(llvm::Value *s, const std::string &id);

    llvm::Value *createGEP(llvm::Value *, ModelDataFields field);

    /**
     * create a GEP for an array which belongs to the ModelData struct,
     * only valid for arrays, will crash on non-array fields.
     */
    llvm::Value *createGEP(llvm::Value *, ModelDataFields field, unsigned index);

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

    void createAccessors(llvm::Module *module);

    std::pair<llvm::Function*, llvm::Function*> createFloatingSpeciesAccessors(llvm::Module *module,
            const std::string id);

    void test(llvm::Module *module,
            llvm::IRBuilder<> *build, llvm::ExecutionEngine * engine);

    /**
     * get the struct type for the CSR sparse storage struct.
     */
    static llvm::StructType *getCSRSparseStructType(llvm::Module *module, llvm::ExecutionEngine *engine = 0);


    static llvm::StructType *getStructType(llvm::Module *module, llvm::ExecutionEngine *engine = 0);


    static const char* ModelDataName;
    static const char* dcsr_matrixName;



private:
    llvm::IRBuilder<> *builder;

    LLVMModelDataSymbols const& symbols;



    /**
     * check to see that s is a ModelData struct pointer,
     * throw an exception if not.
     */
    void validateStruct(llvm::Value *s, const char* funcName);

    /**
     * expensive to always validate the struct, so cache a pointer to it,
     * so just commpare.
     */
    llvm::Value *cachedStructValue;
};

} /* namespace rr */
#endif /* rrLLVMModelDataIRBuilderH */

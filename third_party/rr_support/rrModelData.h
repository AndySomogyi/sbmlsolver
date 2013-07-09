#ifndef rrModelDataH
#define rrModelDataH

/**
 * This file is included by the generated C sbml model code, so
 * BE AWARE OF THIS AND BE VERY CAREFULL MODIFYING IT!
 */

#if defined __cplusplus
namespace rr
{
#endif
typedef struct   SModelData *ModelDataP;
typedef double   (*TEventDelayDelegate)(ModelDataP);
typedef double*  (*TComputeEventAssignmentDelegate)(ModelDataP);
typedef void     (*TPerformEventAssignmentDelegate)(ModelDataP, double*);
typedef void     (*TEventAssignmentDelegate)();



/**
 * sparse storage compressed row format matrix.
 *
 * This should eventually get replaced when we use a numeric library
 * which support sparse storage.
 *
 * structure layout based on  Mark Hoemmen's BeBop sparse conversion lib.
 */
typedef struct dcsr_matrix_t
{
    /**
     * number of rows
     */
    int m;

    /**
     * number of columns
     */
    int n;

    /**
     * number of stored (nonzero) entries.
     */
    int nnz;

    /**
     * array of stored (nonzero) entries of the matrix
     * length: nnz
     *
     */
    double* values;

    /**
     * array of column indices of the stored (nonzero) entries of the matrix,
     * length: nnz
     */
    int* colidx;

    /**
     * array of indices into the colidx and values arrays, for each column,
     * length: m+1
     */
    int* rowptr;

} dcsr_matrix;


/**
 * A data structure that is that allows data to be exchanged
 * with running SBML models. In the case of CExecutableModels, A pointer to
 * this struct is given to the compiled shared library, and the C code
 * there modifies the buffers of this structure.
 *
 * There are some functions in ExecutableModel.h that manage ModelData
 * memory. These would have made more sense here, but in order to prevent
 * any issues with generated code interacting with them, they were placed
 * there.
 *
 * Basic Nomencalture
 * Compartments: A well stirred compartment which contains one or more species.
 * the volume of a compartment can change durring the course of a simulation.
 *
 * Floating Species: these are chemical species who's values (ammount / concentration)
 * change over time.
 *
 * Boundary Species: chemical species that who's values are fixed to their initial conditions,
 * these function as boundary conditions.
 *
 * @see ExecutableModel.h
 */
typedef struct SModelData
{
    /**
     * sizeof this struct, make sure we use the correct
     * size in LLVM land.
     */
    unsigned                            size;

    unsigned                            flags;

    /**
     * current time.
     */
    double                              time;

    /**
     * number of linearly independent rows in the stochiometry matrix.
     */
    int                                 numIndependentSpecies;

    /**
     * number of linerly dependent rows in the stoichiometry matrix.
     *
     * numIndependentVariables + numDependentVariables had better
     * be equal to numFloatingSpecies
     */
    int                                 numDependentSpecies;
    double*                             dependentSpeciesConservedSums;

    /**
     * number of global parameters
     */
    int                                 numGlobalParameters;
    double*                             globalParameters;

    /**
     * number of reactions, same as ratesSize.
     * These are the calcuated reaction rates, not the
     * species rates.
     */
    int                                 numReactions;
    double*                             reactionRates;

    int                                 numRateRules;
    double*                             rateRules;

    /**
     * everything that has a rate (anything that changes) is
     * stored in this single array. This way, a pointer to this
     * array can be given to the integrator and it can integrate
     * it in a single shot.
     *
     * All the other rate arrays are aliases into an offset in this
     * array.
     */
    int                                 numRates;
    double*                             rates;

    /**
     * LLVM specific
     * C version does not support local parameters
     * This is the offset, or starting index of the local parameters
     * for reaction i. Length is numReactions.
     *
     * Rationale: It is simple more effecient to store all the local
     * parameters in a single dimensional array with offsets, as the
     * offsets can be computed at compile time, whereas if we used
     * a array of arrays, it would require an additional memory access
     * to determine the location of the parameter.
     */
    int*                                localParametersOffsets;

    /**
     * the number of local parameters for each reaction,
     * so legnth is numReactions. This is an array of counts,
     * hence it is named differently than the rest of the num*** fields.
     */
    int*                                localParametersNum;

    /**
     * All local parameters are stored in this array. This has
     * length sum(localParameterNum).
     */
    double*                             localParameters;

    /**
     * The total ammounts of the floating species, i.e.
     * concentration * compartment volume.
     * Everything named floatingSpecies??? has length numFloatingSpecies.
     *
     * Note, the floating species consist of BOTH independent AND dependent
     * species. Indexes [0,numIndpendentSpecies) values are the indenpendent
     * species, and the [numIndependentSpecies,numIndendentSpecies+numDependentSpecies)
     * contain the dependent species.
     */
    int                                 numFloatingSpecies;

    /**
     * number of floating species and floating species concentrations.
     */
    double*                             floatingSpeciesConcentrations;

    /**
     * initial concentration values for floating species.
     */
    double*                             floatingSpeciesInitConcentrations;

    /**
     * concentration rates of change for floating species.
     */
    double*                             floatingSpeciesConcentrationRates;

    /**
     * The total amount of a species in a compartment.
     */
    double*                             floatingSpeciesAmounts;

    /**
     * compartment index for each floating species,
     * e.g. the volume of the i'th species is
     * md->compartmentVolumes[md->floatingSpeciesCompartments[i]]
     */
    int*                                floatingSpeciesCompartments;

    /**
     * number of boundary species and boundary species concentrations.
     * units: either
     * Mass Percent = (Mass of Solute) / (Mass of Solution) x 100%
     * Volume Percent= (Volume of Solute) / (Volume of Solution) x 100%
     * Mass/Volume Percent= (Mass of Solute) / (Volume of Solution) x 100%
     */
    int                                 numBoundarySpecies;
    double*                             boundarySpeciesConcentrations;
    double*                             boundarySpeciesAmounts;

    /**
     * compartment index for each boundary species,
     * e.g. the volume of the i'th species is
     * md->compartmentVolumes[md->boundarySpeciesCompartments[i]]
     */
    int*                                boundarySpeciesCompartments;

    /**
     * number of compartments, and compartment volumes.
     * units: volume
     */
    int                                 numCompartments;
    double*                             compartmentVolumes;

    /**
     * stoichiometry matrix
     */
    dcsr_matrix                         stoichiometry;


    //Event stuff
    int                                 numEvents;
    int                                 eventTypeSize;
    bool*                               eventType;

    int                                 eventPersistentTypeSize;
    bool*                               eventPersistentType;

    int                                 eventTestsSize;
    double*                             eventTests;

    int                                 eventPrioritiesSize;
    double*                             eventPriorities;

    int                                 eventStatusArraySize;
    bool*                               eventStatusArray;

    int                                 previousEventStatusArraySize;
    bool*                               previousEventStatusArray;

    /**
     * Work area for model implementations. The data stored here is entirely
     * model implementation specific and should not be accessed
     * anywhere else.
     *
     * allocated by allocModelDataBuffers based on the value of workSize;
     */
    int                                 workSize;
    double*                             work;

    TEventDelayDelegate*                eventDelays;
    TEventAssignmentDelegate*           eventAssignments;

    TComputeEventAssignmentDelegate*    computeEventAssignments;
    TPerformEventAssignmentDelegate*    performEventAssignments;

    /**
     * model name
     */
    char*                               modelName;


    /**
     * Is set by the model to the names of the FloatingSpeciesConcentrationList.
     * The model should set each variableTable[i] to a static null terminated string.
     * allocModelDataBuffers should allocate space for numTotalVariables.
     * strings.
     */
    char**                              variableTable;

    /**
     * names of boundary table species, set by the model to a static string.
     * allocModelDataBuffers should allocate numBoundaryVariables length char** array.
     */
    char**                              boundaryTable;

    /**
     * names of global parameters. populated by the model.
     * allocModelDataBuffers should allocate length numGlobalParameters  char** array.
     */
    char**                              globalParameterTable;

    /**
     * C species references,
     * not working correctly...
     */
    int                                 srSize;
    double*                             sr;

    /**
     * Looks like these were going to be C local variables, but were
     * never implemented...
     */
    //int                                 lpSize;
    //double*                             lp;
    //int                                 localParameterDimensionsSize;
    //int*                                localParameterDimensions;

} ModelData;
//#pragma pack(pop)

#if defined __cplusplus
}
#endif



#endif

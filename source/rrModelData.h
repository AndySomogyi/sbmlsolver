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
    double                              time;

    /**
     * number of linearly independent rows in the stochiometry matrix.
     */
    int                                 numIndependentVariables;

    /**
     * number of linerly dependent rows in the stoichiometry matrix.
     */
    int                                 numDependentVariables;

    /**
     * total number of rows in the stochimetry matrix, this should be
     * the same as the number of floating species.
     */
    int                                 numTotalVariables;

    /**
     * number of global parameters, same as gpSize.
     * TODO: clean up whatever uses this.
     */
    int                                 numGlobalParameters;



    /**
     * number of reactions, same as ratesSize.
     * TODO: clean up whatever code that uses this.
     */
    int                                 numReactions;

    /**
     * number of rate rules, same as rateRulesSize.
     * TODO: clean up whatever code that uses this.
     */
    int                                 numRules;
    int                                 numEvents;

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
     * model name
     */
    char*                               modelName;

    /**
     * number of floating species and floating species concentrations.
     */
    int                                 ySize;
    double*                             y;

    /**
     * number of global parameters and global parameter values.
     * global parameters are all the parameters in a model, including
     * the kinetic law parameters.
     */
    int                                 gpSize;
    double*                             gp;

    /**
     * number of modifiable species references and
     * modifiable species reference values.
     */
    int                                 srSize;
    double*                             sr;

    /**
     * reactions???
     * TODO: figure out if this is ever used.
     */
    int                                 lpSize;
    double*                             lp;

    /**
     * initial concentration values for floating species.
     */
    int                                 init_ySize;
    double*                             init_y;

    /**
     * The total ammounts of the floating species, i.e.
     * concentration * compartment volume
     */
    int                                 numFloatingSpecies;
    double*                             amounts;

    /**
     * number of boundary species and boundary species concentrations.
     * units: either
     * Mass Percent = (Mass of Solute) / (Mass of Solution) x 100%
     * Volume Percent= (Volume of Solute) / (Volume of Solution) x 100%
     * Mass/Volume Percent= (Mass of Solute) / (Volume of Solution) x 100%
     */
    int                                 numBoundarySpecies;
    double*                             boundarySpeciesConcentrations;

    /**
     * number of compartments, and compartment volumes.
     * units: volume
     */
    int                                 numCompartments;
    double*                             compartmentVolumes;

    /**
     * concentration rates of change for floating species.
     */
    int                                 dydtSize;
    double*                             dydt;

    /**
     * number of reactions and the reaction rates
     */
    int                                 ratesSize;
    double*                             rates;

    int                                 rateRulesSize;
    double*                             rateRules;

    int                                 ctSize;
    double*                             ct;

    int                                 localParameterDimensionsSize;
    int*                                localParameterDimensions;

    //Event stuff
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

    TEventDelayDelegate*                eventDelays;
    TEventAssignmentDelegate*           eventAssignments;

    TComputeEventAssignmentDelegate*    computeEventAssignments;
    TPerformEventAssignmentDelegate*    performEventAssignments;

} ModelData;
//#pragma pack(pop)

#if defined __cplusplus
}
#endif



#endif

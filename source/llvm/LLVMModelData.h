/*
 * LLVMModelData.h
 *
 *  Created on: Aug 8, 2013
 *      Author: andy
 */

#ifndef RRLLVMMODELDATA_H_
#define RRLLVMMODELDATA_H_

#include "rrSparse.h"
#include <string>
#include <ostream>

namespace rrllvm
{

struct LLVMModelData;


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
struct LLVMModelData
{
    /**
     * sizeof this struct, make sure we use the correct
     * size in LLVM land.
     */
    unsigned                            size;                             // 0

    unsigned                            flags;                            // 1

    /**
     * current time.
     */
    double                              time;                             // 2

    /**
     * number of linearly independent rows in the stochiometry matrix.
     */
    unsigned                            numIndependentSpecies;            // 3

    /**
     * number of linerly dependent rows in the stoichiometry matrix.
     *
     * numIndependentVariables + numDependentVariables had better
     * be equal to numFloatingSpecies
     */
    unsigned                            numDependentSpecies;              // 4
    double*                             dependentSpeciesConservedSums;    // 5

    /**
     * number of global parameters
     */
    unsigned                            numGlobalParameters;              // 6
    double*                             globalParameters;                 // 7

    /**
     * number of reactions, same as ratesSize.
     * These are the calcuated reaction rates, not the
     * species rates.
     */
    unsigned                            numReactions;                     // 8
    double*                             reactionRates;                    // 9

    unsigned                            numRateRules;                     // 10

    /**
     * All of the elelments which have a rate rule are stored here.
     *
     * As the integrator runs, this pointer can simply point to an offset
     * in the integrator's state vector.
     *
     * Only used in the LLVM version.
     */
    double*                             rateRuleValues;                   // 11

    /**
     * the rate of change of all elements who's dynamics are determined
     * by rate rules.
     *
     * In the LLVM version, this is just a pointer to a data block
     * owned by the integrator. In the C version, lots of strange
     * stuff goes on here, but the C version does allocate this block,
     * and do all sorts of copying back and forth between the integrator's
     * rate rule block.
     */
    double*                             rateRuleRates;                    // 12

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
    unsigned                            numFloatingSpecies;               // 13

    /**
     * amount rates of change for floating species.
     */
    double*                             floatingSpeciesAmountRates;       // 14

    /**
     * The total amount of a species in a compartment.
     */
    double*                             floatingSpeciesAmounts;           // 15

    /**
     * number of boundary species and boundary species concentrations.
     * units: either
     * Mass Percent = (Mass of Solute) / (Mass of Solution) x 100%
     * Volume Percent= (Volume of Solute) / (Volume of Solution) x 100%
     * Mass/Volume Percent= (Mass of Solute) / (Volume of Solution) x 100%
     */
    unsigned                            numBoundarySpecies;               // 16
    double*                             boundarySpeciesAmounts;           // 17

    /**
     * number of compartments, and compartment volumes.
     * units: volume
     */
    unsigned                            numCompartments;                  // 18
    double*                             compartmentVolumes;               // 19

    /**
     * stoichiometry matrix
     */
    rr::csr_matrix*                         stoichiometry;                    // 20


    //Event stuff
    unsigned                            numEvents;                        // 21

    /**
     * number of items in the state vector.
     */
    unsigned                            stateVectorSize;                  // 22

    /**
     * the state vector, this is usually a pointer to a block of data
     * owned by the integrator.
     */
    double*                             stateVector;                      // 23

    /**
     * the rate of change of the state vector, this is usually a pointer to
     * a block of data owned by the integrator.
     */
    double*                             stateVectorRate;                  // 24


    static void init(LLVMModelData&);

    static void allocBuffers(LLVMModelData &data, const std::string& modelName);

    static void freeBuffers(LLVMModelData&);
};

std::ostream& operator <<(std::ostream& os, const LLVMModelData& data);

}



#endif /* RRLLVMMODELDATA_H_ */

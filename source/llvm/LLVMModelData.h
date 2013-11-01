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

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4200 )
#endif


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

    unsigned                            numIndCompartments;               // 3

    /**
     * The total ammounts of the floating species, i.e.
     * concentration * compartment volume.
     * Everything named floatingSpecies??? has length numFloatingSpecies.
     *
     * Note, the floating species consist of BOTH independent AND dependent
     * species. Indexes [0,numIndpendentSpecies) values are the indenpendent
     * species, and the [numIndFloatingSpecies,numIndendentSpecies+numDependentSpecies)
     * contain the dependent species.
     */
    unsigned                            numIndFloatingSpecies;            // 4

    /**
     * \conservation
     *
     * conserved floating species require initial conditions to be set.
     */
    unsigned                            numConservedSpecies;              // 5


    /**
     * number of boundary species and boundary species concentrations.
     * units: either
     * Mass Percent = (Mass of Solute) / (Mass of Solution) x 100%
     * Volume Percent= (Volume of Solute) / (Volume of Solution) x 100%
     * Mass/Volume Percent= (Mass of Solute) / (Volume of Solution) x 100%
     */
    unsigned                            numIndBoundarySpecies;            // 6

    /**
     * number of global parameters
     */
    unsigned                            numIndGlobalParameters;           // 7


    /**
     * all rate rules are by definition dependent
     */
    unsigned                            numRateRules;                     // 8

    /**
     * number of reactions, same as ratesSize.
     * These are the calcuated reaction rates, not the
     * species rates.
     */
    unsigned                            numReactions;                     // 9

    /**
     * stoichiometry matrix
     */
    rr::csr_matrix*                     stoichiometry;                    // 10


    //Event stuff
    unsigned                            numEvents;                        // 11

    /**
     * number of items in the state vector.
     * should be numIndFloatingSpecies + numRateRules
     */
    unsigned                            stateVectorSize;                  // 12

    /**
     * the state vector, this is usually a pointer to a block of data
     * owned by the integrator.
     */
    double*                             stateVector;                      // 13

    /**
     * the rate of change of the state vector, this is usually a pointer to
     * a block of data owned by the integrator.
     */
    double*                             stateVectorRate;                  // 14

    /**
     * the rate of change of all elements who's dynamics are determined
     * by rate rules.
     *
     * This is just a pointer to a data block
     * owned by the integrator.
     *
     * Normally NULL, only valid durring an evalModel call.
     */
    double*                             rateRuleRates;                    // 15



    /**
     * amount rates of change for floating species.
     *
     * This pointer is ONLY valid during an evalModel call, otherwise it is
     * zero. TODO, this needs be be moved to a parameter.
     */
    double*                             floatingSpeciesAmountRates;       // 16

    // permanent data section


    /**
     * number of compartments, and compartment volumes.
     * units: volume
     */

    double*                             compartmentVolumesAlias;          // 17
    double*                             compartmentVolumesInitAlias;      // 18


    /**
     * \conservation
     *
     * length numIndFloatingSpecies
     */
    double*                             floatingSpeciesAmountsInitAlias;  // 19

    double*                             conservedSpeciesAmountsInitAlias; // 20

    double*                             boundarySpeciesAmountsAlias;      // 21
    double*                             boundarySpeciesAmountsInitAlias;  // 22

    double*                             globalParametersAlias;            // 23
    double*                             globalParametersInitAlias;        // 24

    double*                             reactionRatesAlias;               // 25

    /**
     * All of the elelments which have a rate rule are stored here.
     *
     * As the integrator runs, this pointer can simply point to an offset
     * in the integrator's state vector.
     *
     * This pointer is part of the state vector. When any function is called by
     * CVODE, this is actually a pointer to a CVODE owned memory block.
     * Otherwise, this points to the alocated rateRuleValues block at the end
     * of this struct.
     *
     */
    double*                             rateRuleValuesAlias;              // 26




    /**
     * has length numIndFloatingSpecies
     *
     * This pointer is part of the state vector. When any function is called by
     * CVODE, this is actually a pointer to a CVODE owned memory block.
     */
    double*                             floatingSpeciesAmountsAlias;      // 27

    /**
     * binary data layout:
     *
     * compartmentVolumes                [numIndCompartmentVolumes]       // 28
     * compartmentVolumesInit            [numIndCompartmentVolumes]       // 29
     * floatingSpeciesAmountsInit        [numIndFloatingSpecies]          // 30
     * conservedSpeciesAmountsInit       [numConservedSpecies]            // 31
     * boundarySpeciesAmounts            [numIndBoundarySpecies]          // 32
     * boundarySpeciesAmountsInit        [numIndBoundarySpecies]          // 33
     * globalParameters                  [numIndGlobalParameters]         // 34
     * globalParametersInit              [numIndGlobalParameters]         // 35
     * reactionRates                     [numReactions]                   // 36
     *
     * rateRuleValues                    [numRateRules]                   // 37
     * floatingSpeciesAmounts            [numIndFloatingSpecies]          // 38
     */
    double                              data[0];                          // not listed
};

void LLVMModelData_free(LLVMModelData*);

#ifdef _MSC_VER
#pragma warning( pop )
#endif



std::ostream& operator <<(std::ostream& os, const LLVMModelData& data);

}



#endif /* RRLLVMMODELDATA_H_ */

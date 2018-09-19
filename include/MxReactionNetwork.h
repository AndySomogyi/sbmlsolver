/**
 * MxReactionNetwork.h
 *
 *  Created on: Sep 18, 2018
 *      Author: andy
 */

/**
 * Interface definitions for the reaction network solver.
 */

#include <mx_port.h>

/**
 * symbol Ids are 32 bit signed integers. Valid Ids are non-negative,
 * invalid or reserved ids are negative
 */
typedef uint32_t SYMBOLID;

enum ReactionNetworkVariableKind {

    CRN_DEPENDENT_FLOATING_SPECIES = (0x1 << 0),  // => 0x00000001
    CRN_FLOATING_SPECIES           = (0x1 << 1),  // => 0x00000010
    CRN_BOUNDARY_SPECIES           = (0x1 << 2),  // => 0x00000100
    CRN_GLOBAL_PARAMETER           = (0x1 << 3),  // => 0x00001000
    CRN_COMPARTMENT                = (0x1 << 4),  // => 0x00010000
    CRN_CONSERVED_MOIETY           = (0x1 << 5),  // => 0x00100000
    CRN_RATE_RULE                  = (0x1 << 6),  // => 0x01000000
    CRN_REACTION                   = (0x1 << 7),  // => 0x10000000
    CRN_ALL                        = 0xffffffff
};



/**
 * symbol Ids are 32 bit signed integers. Valid Ids are non-negative,
 * invalid or reserved ids are negative
 */


/**
 * An interface implemented by a hosting compartment. A host compartment
 * represents some volume of space where the reaction network is hosted in.
 *
 * The reaction network calls methods on the compartment boundary to
 * determine the boundary conditions of the reaction network.
 */
struct ICompartmentBoundary {
    /**
     * gets the volume of the compartment and stores it in result
     */
    HRESULT getVolume( double* result) = 0;

    /**
     * gets the surface area of the compartment and stores it in result
     */
    HRESULT getArea( double* result) = 0;

    /**
     * Get the number of names that this object has, i.e. the number of
     * symbols in this network.
     *
     * @param kinds: a bitmask of the kinds of variables to get. Pass in
     * CRN_ALL
     */
    virtual HRESULT getVariableCount(uint32_t kinds, uint32_t *result);

    /**
     * Copies the name of a symbol into a user supplied buffer.
     *
     * @param maxLen the size of the user suplied buffer.
     */
    virtual HRESULT getVariableName(SYMBOLID id, uint32_t maxLen, char* buffer);


    virtual HRESULT getVariableKind(SYMBOLID id, uint32_t *kind);


    virtual HRESULT getVariableId(const char* name, SYMBOLID *id);

    /**
     * read the mean value of each of the given symbol ids at the
     * boundary, store the results in the result array
     */
    virtual HRESULT getMeanValues(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result) = 0;

    /**
     * set the flux rates at the boundary.
     *
     * This method provides a way for a chemical network to set
     * the flux rates (usually secretion and uptake) rates at
     * the boundary of the compartments.
     *
     * Units of flux are amount per unit time.
     */
    virtual HRESULT setFluxRates(
            const SYMBOLID *symbols,
            const double *fluxRates,
            uint32_t count) = 0;
};


/**
 * The IScalarSystem interface provides a way to access an
 * sbml model that was compiled, JIT'd or interpreted
 * as executable (runnable) module.
 *
 * This interface provides a set of methods to inspect, read and modify
 * a reaction network (typically a chemical reaction network).
 *
 * This interface also provides ways to access and modify model
 * initial conditions, rules, functions and whatever other semantic
 * information that was specified in the sbml model.
 */
struct IScalarSystem {


    /**
     * Get the number of names that this object has, i.e. the number of
     * symbols in this network.
     *
     * @param kinds: a bitmask of the kinds of variables to get. Pass in
     * CRN_ALL
     */
    virtual HRESULT getVariableCount(uint32_t kinds, uint32_t *result);

    /**
     * Copies the name of a symbol into a user supplied buffer.
     *
     * @param maxLen the size of the user suplied buffer.
     */
    virtual HRESULT getVariableName(SYMBOLID id, uint32_t maxLen, char* buffer);


    virtual HRESULT getVariableKind(SYMBOLID id, uint32_t *kind);

    virtual HRESULT getVariableId(const char* name, SYMBOLID *id);

    /**
     * amounts
     */
    virtual HRESULT getVariableAmounts(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result) = 0;

    virtual HRESULT setVariableAmounts(
            const SYMBOLID *symbols,
            uint32_t count,
            const double *values) = 0;

    virtual HRESULT getVariableInitialAmounts(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result) = 0;

    virtual HRESULT setVariableInitialAmounts(
            const SYMBOLID *symbols,
            uint32_t count,
            const double *values) = 0;

    virtual HRESULT getVariableAmountRates(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result) = 0;


    /**
     * concentrations
     */
    virtual HRESULT getVariableConcentrations(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result) = 0;

    virtual HRESULT setVariableConcentrations(
            const SYMBOLID *symbols,
            uint32_t count,
            const double *values) = 0;

    virtual HRESULT getVariableInitialConcentrations(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result) = 0;

    virtual HRESULT setVariableInitialConcentrations(
            const SYMBOLID *symbols,
            uint32_t count,
            const double *values) = 0;

    virtual HRESULT getVariableConcentrationRates(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result) = 0;


    /**
     * Get the name of this system
     */
    virtual HRESULT getModelName(uint32_t maxLen, char* buffer) = 0;

    virtual HRESULT setTime(double time) = 0;

    virtual HRESULT getTime(double *time) = 0;

    /**
     * Loads the initial conditions into the current model state.
     *
     * Initial conditions may have been updated at any time externally.
     */
    virtual HRESULT reset() = 0;


    virtual HRESULT setCompartmentBoundary(ICompartmentBoundary *boundary);

};



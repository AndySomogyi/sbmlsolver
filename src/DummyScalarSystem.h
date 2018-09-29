/*
 * DummyScalarSystem.h
 *
 *  Created on: Sep 29, 2018
 *      Author: andy
 */

#ifndef SRC_DUMMYSCALARSYSTEM_H_
#define SRC_DUMMYSCALARSYSTEM_H_

#include "MxReactionNetwork.h"

class DummyScalarSystem : public IScalarSystem
{
public:

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
            double *result);

    virtual HRESULT setVariableAmounts(
            const SYMBOLID *symbols,
            uint32_t count,
            const double *values);

    virtual HRESULT getVariableInitialAmounts(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result);

    virtual HRESULT setVariableInitialAmounts(
            const SYMBOLID *symbols,
            uint32_t count,
            const double *values);

    virtual HRESULT getVariableAmountRates(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result);


    /**
     * concentrations
     */
    virtual HRESULT getVariableConcentrations(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result);

    virtual HRESULT setVariableConcentrations(
            const SYMBOLID *symbols,
            uint32_t count,
            const double *values);

    virtual HRESULT getVariableInitialConcentrations(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result);

    virtual HRESULT setVariableInitialConcentrations(
            const SYMBOLID *symbols,
            uint32_t count,
            const double *values);

    virtual HRESULT getVariableConcentrationRates(
            const SYMBOLID *symbols,
            uint32_t count,
            double *result);


    /**
     * Get the name of this system
     */
    virtual HRESULT getModelName(uint32_t maxLen, char* buffer);

    virtual HRESULT setTime(double time);

    virtual HRESULT getTime(double *time);

    /**
     * Loads the initial conditions into the current model state.
     *
     * Initial conditions may have been updated at any time externally.
     */
    virtual HRESULT reset();


    virtual HRESULT setCompartmentBoundary(ICompartmentBoundary *boundary);
};

#endif /* SRC_DUMMYSCALARSYSTEM_H_ */

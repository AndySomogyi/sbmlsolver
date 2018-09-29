/*
 * DummyScalarSystem.cpp
 *
 *  Created on: Sep 29, 2018
 *      Author: andy
 */

#include <DummyScalarSystem.h>

HRESULT DummyScalarSystem::getVariableCount(uint32_t kinds, uint32_t* result)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableName(SYMBOLID id, uint32_t maxLen,
        char* buffer)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableKind(SYMBOLID id, uint32_t* kind)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableId(const char* name, SYMBOLID* id)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableAmounts(const SYMBOLID* symbols,
        uint32_t count, double* result)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::setVariableAmounts(const SYMBOLID* symbols,
        uint32_t count, const double* values)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableInitialAmounts(const SYMBOLID* symbols,
        uint32_t count, double* result)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::setVariableInitialAmounts(const SYMBOLID* symbols,
        uint32_t count, const double* values)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableAmountRates(const SYMBOLID* symbols,
        uint32_t count, double* result)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableConcentrations(const SYMBOLID* symbols,
        uint32_t count, double* result)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::setVariableConcentrations(const SYMBOLID* symbols,
        uint32_t count, const double* values)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableInitialConcentrations(
        const SYMBOLID* symbols, uint32_t count, double* result)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::setVariableInitialConcentrations(
        const SYMBOLID* symbols, uint32_t count, const double* values)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getVariableConcentrationRates(
        const SYMBOLID* symbols, uint32_t count, double* result)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getModelName(uint32_t maxLen, char* buffer)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::setTime(double time)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::getTime(double* time)
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::reset()
{
    return E_NOTIMPL;
}

HRESULT DummyScalarSystem::setCompartmentBoundary(
        ICompartmentBoundary* boundary)
{
    return E_NOTIMPL;
}

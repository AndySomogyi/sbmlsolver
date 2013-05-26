#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrSteadyStateSolver.h"


namespace rr
{

SteadyStateSolver::SteadyStateSolver(const string& funcName, const string& description)
:
mCapability("SteadyState", funcName, description)
{

}

SteadyStateSolver::~SteadyStateSolver()

{

}

Capability&	SteadyStateSolver::getCapability()
{
	return mCapability;
}

}

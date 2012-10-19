#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrCapability.h"
#include "rrCapabilitiesSection.h"
//---------------------------------------------------------------------------

namespace rr
{

CapabilitiesSection::CapabilitiesSection(const string& name, const string& method, const string& description)
:
mName(name),
mMethod(method),
mDescription(description)
{}

CapabilitiesSection::CapabilitiesSection(const CapabilitiesSection& from)
:
mName(from.mName),
mMethod(from.mMethod),
mDescription(from.mDescription),
mCapabilities(from.mCapabilities)
{}


}
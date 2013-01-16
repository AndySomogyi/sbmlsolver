//---------------------------------------------------------------------------

#pragma hdrstop
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rr
{

Plugin::Plugin(const std::string& name)
:
mName(name)
{

}

Plugin::~Plugin()
{

}

bool Plugin::Load()
{
}

bool Plugin::Unload()
{

}

}


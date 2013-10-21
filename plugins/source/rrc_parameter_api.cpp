#pragma hdrstop
#include <string>
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrc_parameter_api.h"
#include "rrLogger.h"
#include "rrc_api.h"
#include "rrc_utilities.h"           //Support functions, not exposed as api functions and or data
#include "rrc_cpp_support.h"           //Support functions, not exposed as api functions and or data
#include "rrc_plugin_api.h"
#include "rrplugins_cpp_support.h"
#include "rrParameter.h"
#include "rrBaseParameter.h"
//---------------------------------------------------------------------------

namespace rrc
{
using namespace std;
using namespace rr;



bool rrcCallConv setParameter(RRParameterHandle handle, const char* value)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        para->setValueFromString(value);
        return true;
    }
    catch_bool_macro
}

char* rrcCallConv getParameterValueAsString(RRParameterHandle handle)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        string val = para->getValueAsString();
        return rr::createText(val);
    }
    catch_ptr_macro
}

void* rrcCallConv getParameterValueAsPointer(RRParameterHandle handle)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        return para->getValueAsPointer();
    }
    catch_ptr_macro
}

char* rrcCallConv getParameterName(RRParameterHandle handle)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        return rr::createText(para->getName());
    }
    catch_ptr_macro
}

char* rrcCallConv getParameterHint(RRParameterHandle handle)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        return rr::createText(para->getHint());
    }
    catch_ptr_macro
}

//////////////////////////////////////////////
}//namespace

#pragma hdrstop
#include <string>
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrp_parameter_api.h"
#include "rrLogger.h"
#include "rrc_api.h"
#include "rrc_utilities.h"           //Support functions, not exposed as api functions and or data
#include "rrc_cpp_support.h"           //Support functions, not exposed as api functions and or data
#include "rrp_api.h"
#include "rrp_cpp_support.h"
#include "rrParameter.h"
#include "rrBaseParameter.h"
//---------------------------------------------------------------------------

namespace rrp
{
using namespace std;
using namespace rr;


bool rrp_cc setParameter(RRParameterHandle handle, const char* value)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        para->setValueFromString(value);
        return true;
    }
    catch_bool_macro
}

char* rrp_cc getParameterValueAsString(RRParameterHandle handle)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        string val = para->getValueAsString();
        return rr::createText(val);
    }
    catch_ptr_macro
}

void* rrp_cc getParameterValueAsPointer(RRParameterHandle handle)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        return para->getValueAsPointer();
    }
    catch_ptr_macro
}

char* rrp_cc getParameterName(RRParameterHandle handle)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        return rr::createText(para->getName());
    }
    catch_ptr_macro
}

char* rrp_cc getParameterHint(RRParameterHandle handle)
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

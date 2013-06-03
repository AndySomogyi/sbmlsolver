#pragma hdrstop
#include <string>
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrc_parameter_api.h"
#include "rrLogger.h"
#include "rrc_api.h"
#include "rrc_utilities.h"   		//Support functions, not exposed as api functions and or data
#include "rrc_cpp_support.h"   		//Support functions, not exposed as api functions and or data
//---------------------------------------------------------------------------

namespace rrc
{
using namespace std;
using namespace rr;

RRStringArrayHandle rrcCallConv getListOfParameters(RRHandle handle, const char* cap_name)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        StringList list = rri->getListOfParameters(cap_name);
        return createList(list);
    }
    catch_ptr_macro
}

RRParameterHandle rrcCallConv getParameter(RRHandle handle, const char* cap_name, const char* parName)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        Capability* cap = rri->getCapability(cap_name);
		if(!cap)
        {
        	return NULL;
        }
        return cap->getParameter(parName);
    }
    catch_ptr_macro
}

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
        return createText(val);
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
        return createText(para->getName());
    }
    catch_ptr_macro
}

char* rrcCallConv getParameterHint(RRParameterHandle handle)
{
	try
    {
        BaseParameter* para = castToParameter(handle);
        return createText(para->getHint());
    }
    catch_ptr_macro
}

//////////////////////////////////////////////
}//namespace
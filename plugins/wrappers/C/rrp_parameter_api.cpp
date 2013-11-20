#pragma hdrstop
#include <string>
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrp_parameter_api.h"
#include "rrLogger.h"
#include "rrc_api.h"
#include "rrc_utilities.h"              //Support functions, not exposed as api functions and or data
#include "rrc_cpp_support.h"            //Support functions, not exposed as api functions and or data
#include "rrp_api.h"
#include "rrp_cpp_support.h"
#include "rrParameter.h"
#include "rrBaseParameter.h"
//---------------------------------------------------------------------------

namespace rrp
{
using namespace std;
using namespace rr;

bool rrp_cc addParameter(RRParametersHandle handle, RRParameterHandle para)
{
    start_try
        Parameters* paras   = castToParameters(handle);
        BaseParameter* bPara = castToParameter(para);
        paras->add(bPara, false);
        return true;
    }
    catch_bool_macro
}

char* rrp_cc getParameterInfo(RRParameterHandle handle)
{
    start_try
        BaseParameter* para = castToParameter(handle);
        stringstream s;
        s<<"Name="<<para->getName()<<"\tType="<<para->getType()<<"\tHint="<<para->getHint();
        return createText(s.str());
    }
    catch_ptr_macro
}


bool rrp_cc setParameterByString(RRParameterHandle handle, const char* value)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        para->setValueFromString(string(value));
        return true;
    }
    catch_bool_macro
}

bool rrp_cc setIntParameter(RRParameterHandle handle, int value)
{
    try
    {
        Parameter<int>* para = castToIntParameter(handle);
        para->setValue(value);
        return true;
    }
    catch_bool_macro
}

bool rrp_cc setDoubleParameter(RRParameterHandle handle, double value)
{
    try
    {
        Parameter<double>* para = castToDoubleParameter(handle);
        para->setValue(value);
        return true;
    }
    catch_bool_macro
}

bool rrp_cc setStringParameter(RRParameterHandle handle, char* value)
{
    try
    {
        Parameter<char*>* para = castToStringParameter(handle);
        para->setValue(value);
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

char* rrp_cc getParameterType(RRParameterHandle handle)
{
    try
    {
        BaseParameter* para = castToParameter(handle);
        return rr::createText(para->getType());
    }
    catch_ptr_macro
}

}//namespace

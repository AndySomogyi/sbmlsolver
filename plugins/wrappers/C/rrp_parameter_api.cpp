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
#include "rrPluginParameter.h"
//---------------------------------------------------------------------------

namespace rrp
{
using namespace std;
using namespace rr;

RRParameterHandle rrp_cc createParameter(const char* label, const char* type, void* value)
{
    start_try
        if(string(type) == string("double"))
        {
            double iniVal  = 0;
            if(value != NULL)
            {
                double* dVal = (double*) value;
                iniVal = (*dVal);
            }
            Parameter<double> *para = new Parameter<double>(label, iniVal);
            return para;
        }

        return NULL;
    catch_ptr_macro
}

bool rrp_cc addParameterToList(RRParametersHandle handle, RRParameterHandle para)
{
    start_try
        Parameters* paras   = castToParameters(handle);
        PluginParameter* bPara = castToParameter(para);
        paras->add(bPara, false);
        return true;
    catch_bool_macro
}

char* rrp_cc getParameterInfo(RRParameterHandle handle)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        stringstream s;
        s<<"Name="<<para->getName()<<"\tType="<<para->getType()<<"\tHint="<<para->getHint();
        return createText(s.str());
    catch_ptr_macro
}


bool rrp_cc setParameterByString(RRParameterHandle handle, const char* value)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        para->setValueFromString(string(value));
        return true;
    catch_bool_macro
}

bool rrp_cc setIntParameter(RRParameterHandle handle, int value)
{
    start_try
        Parameter<int>* para = castToIntParameter(handle);
        para->setValue(value);
        return true;
    catch_bool_macro
}

bool rrp_cc setDoubleParameter(RRParameterHandle handle, double value)
{
    start_try
        Parameter<double>* para = castToDoubleParameter(handle);
        para->setValue(value);
        return true;
    catch_bool_macro
}

bool rrp_cc setStringParameter(RRParameterHandle handle, char* value)
{
    start_try
        Parameter<char*>* para = castToStringParameter(handle);
        para->setValue(value);
        return true;
    catch_bool_macro
}

char* rrp_cc getParameterValueAsString(RRParameterHandle handle)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        string val = para->getValueAsString();
        return rr::createText(val);
    catch_ptr_macro
}

void* rrp_cc getParameterValueHandle(RRParameterHandle handle)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        return para->getValueHandle();
    catch_ptr_macro
}

char* rrp_cc getParameterName(RRParameterHandle handle)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        return rr::createText(para->getName());
    catch_ptr_macro
}

char* rrp_cc getParameterHint(RRParameterHandle handle)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        return rr::createText(para->getHint());
    catch_ptr_macro
}

char* rrp_cc getParameterType(RRParameterHandle handle)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        return rr::createText(para->getType());
    catch_ptr_macro
}

}//namespace

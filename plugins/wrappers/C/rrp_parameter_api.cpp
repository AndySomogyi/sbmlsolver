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

RRParameterHandle rrp_cc createParameter(const char* label, const char* type, const char* _hint, void* value)
{
    start_try
        char* hint = (char*) _hint;
        if(!hint)
        {
            hint = "";
        }


        if(string(type) == string("bool"))
        {
            bool iniVal  = false;
            if(value != NULL)
            {
                //cast it
                bool* val = (bool*) value;
                iniVal = (*val);
            }
            Parameter<bool> *para = new Parameter<bool>(label, iniVal, hint);
            return para;
        }

        if(string(type) == string("int"))
        {
            int iniVal  = 0;
            if(value != NULL)
            {
                //cast it
                int* val = (int*) value;
                iniVal = (*val);
            }
            Parameter<int> *para = new Parameter<int>(label, iniVal, hint);
            return para;
        }

        //Don't support type 'float', it just causes problems. Make it a double
        if(string(type) == string("float"))
        {
            double iniVal  = 0;
            if(value != NULL)
            {
                //cast it
                double* dVal = (double*) value;
                iniVal = (*dVal);
            }
            Parameter<double> *para = new Parameter<double>(label, iniVal, hint);
            return para;
        }

        if(string(type) == string("double"))
        {
            double iniVal  = 0;
            if(value != NULL)
            {
                //cast it
                double* dVal = (double*) value;
                iniVal = (*dVal);
            }
            Parameter<double> *para = new Parameter<double>(label, iniVal, hint);
            return para;
        }

        if(string(type) == string("string"))
        {
            char* iniVal  = "";
            if(value != NULL)
            {
                //cast it
                char* *val = (char**) value;
                iniVal = (*val);
            }
            Parameter<char*> *para = new Parameter<char*>(label, iniVal, hint);
            return para;
        }

        if(string(type) == string("std::string"))
        {
            string iniVal  = "";
            if(value != NULL)
            {
                //cast it
                string* val = (string*) value;
                iniVal = (*val);
            }
            Parameter<string> *para = new Parameter<string>(label, iniVal, hint);
            return para;
        }

        if(string(type) == string("listOfParameters"))
        {
            Parameters iniVal;
            if(value != NULL)
            {
                //cast it
                Parameters* val = (Parameters*) value;
                iniVal = (*val);
            }

            Parameter<Parameters> *para = new Parameter<Parameters>(label, iniVal, hint);
            return para;
        }

        if(string(type) == string("roadRunnerData"))
        {
            RoadRunnerData iniVal;
            if(value != NULL)
            {
                //cast it
                RoadRunnerData* val = (RoadRunnerData*) value;
                iniVal = (*val);
            }

            Parameter<RoadRunnerData> *para = new Parameter<RoadRunnerData>(label, iniVal, hint);
            return para;
        }

        return NULL;
    catch_ptr_macro
}

bool rrp_cc freeParameter(RRParameterHandle paraHandle)
{
    start_try
        PluginParameter* para   = castToParameter(paraHandle);
        delete para;
        return true;
    catch_bool_macro
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
        s<<"Name="<<para->getName()<<"\tType="<<para->getType()<<"\tDescription="<<para->getDescription()<<"\tHint="<<para->getHint();
        return createText(s.str());
    catch_ptr_macro
}

bool rrp_cc setParameterDescription(RRParameterHandle handle, const char* value)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        para->setDescription(string(value));        
        return true;
    catch_bool_macro
}

bool rrp_cc setParameterHint(RRParameterHandle handle, const char* value)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        para->setHint(string(value));
        return true;
    catch_bool_macro
}

bool rrp_cc setParameterByString(RRParameterHandle handle, const char* value)
{
    start_try
        PluginParameter* para = castToParameter(handle);
        para->setValueFromString(string(value));
        return true;
    catch_bool_macro
}

bool rrp_cc setBoolParameter(RRParameterHandle handle, bool value)
{
    start_try
        Parameter<bool>* para = castToBoolParameter(handle);
        para->setValue(value);
        return true;
    catch_bool_macro
}

bool rrp_cc getBoolParameter(RRParameterHandle handle, bool* value)
{
    start_try
        Parameter<bool>* para = castToBoolParameter(handle);
        (*value) = para->getValue();
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

bool rrp_cc getIntParameter(RRParameterHandle handle, int *value)
{
    start_try
        Parameter<int>* para = castToIntParameter(handle);
        (*value) = para->getValue();
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

bool rrp_cc getDoubleParameter(RRParameterHandle handle, double *value)
{
    start_try
        Parameter<double>* para = castToDoubleParameter(handle);
        (*value) = para->getValue();
        return true;
    catch_bool_macro
}

bool rrp_cc setStringParameter(RRParameterHandle handle, char* value)
{
    start_try
        Parameter<string>* para = castToStringParameter(handle);
        para->setValue(value);
        return true;
    catch_bool_macro
}

bool rrp_cc getStringParameter(RRParameterHandle handle, const char* (*value))
{
    start_try
        Parameter<string>* para = castToStringParameter(handle);
                
        (*value) = para->getValue().c_str();
        return true;
    catch_bool_macro
}

bool rrp_cc setListParameter(RRParameterHandle handle, void* value)
{
    start_try
        Parameter<Parameters>* para = castToParametersParameter(handle);
        para->setValue((Parameters*)(value));
        return true;
    catch_bool_macro
}

bool rrp_cc getListParameter(RRParameterHandle handle, void* (value))
{
    start_try
        Parameter<Parameters>* para = castToParametersParameter(handle);
        //Parameters* assignTo = castToParameters(value);
        //
        (value) = (void*) &(para->getValue());
        return true;
    catch_bool_macro
}

bool rrp_cc setRoadRunnerDataParameter(RRParameterHandle handle, void* value)
{
    start_try
        Parameter<RoadRunnerData*>* para = castToRoadRunnerDataParameter(handle);
        para->setValue((RoadRunnerData*) value);
        return true;
    catch_bool_macro
}

bool rrp_cc getRoadRunnerDataParameter(RRParameterHandle handle, void* value)
{
    start_try
        Parameter<RoadRunnerData*>* para = castToRoadRunnerDataParameter(handle);
        RoadRunnerData* assignTo = castToRoadRunnerData(value);
        (assignTo) = para->getValue();
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

RRParameterHandle rrp_cc getFirstParameter(RRParametersHandle handle)
{
    start_try
        Parameters *paras = castToParameters(handle);
        return paras->getFirst();
    catch_ptr_macro
}

RRParameterHandle rrp_cc getNextParameter(RRParametersHandle handle)
{
    start_try
        Parameters *paras = castToParameters(handle);
        return paras->getNext();
    catch_ptr_macro
}

RRParameterHandle rrp_cc getPreviousParameter(RRParametersHandle handle)
{
    start_try
        Parameters *paras = castToParameters(handle);
        return paras->getPrevious();
    catch_ptr_macro
}

RRParameterHandle rrp_cc getCurrentParameter(RRParametersHandle handle)
{
    start_try
        Parameters *paras = castToParameters(handle);
        return paras->getCurrent();
    catch_ptr_macro
}

bool rrp_cc clearParameterList(RRParametersHandle handle)
{
    start_try
        Parameters* paras = castToParameters(handle);
        return paras->clear();
        catch_bool_macro
}

char* rrp_cc getNamesFromParameterList(RRParametersHandle handle)
{
    start_try
        Parameters* paras = castToParameters(handle);    
        StringList aList;
        for(int i = 0; i < paras->count(); i++)
        {
            aList.add((*paras)[i]->getName());
        }
        return createText(aList.AsString().c_str());

    catch_ptr_macro
}


}//namespace

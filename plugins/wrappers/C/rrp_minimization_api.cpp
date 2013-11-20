#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrMinimizationData.h"
#include "rrp_minimization_api.h"
#include "rrc_utilities.h"              //Support functions, not exposed as api functions and or data
#include "rrc_cpp_support.h"            //Support functions, not exposed as api functions and or data
#include "rrp_cpp_support.h"            //Support functions, not exposed as api functions and or data

//---------------------------------------------------------------------------

namespace rrp
{
using namespace std;
using namespace rr;

//bool rrp_cc addDoubleParameter(RRMinimizationDataHandle handle, const char* name, double value)
//{
//    try
//    {
//        MinimizationData* data = castToMinimizationData(handle);
//        data->addParameter(name, value);
//        return true;
//    }
//    catch_bool_macro
//}
//
//bool rrp_cc setMinimizationObservedDataSelectionList(RRMinimizationDataHandle handle, const char* selections)
//{
//    try
//    {
////        MinimizationData* data = castToMinimizationData(handle);
////        data->setObservedDataSelectionList(selections);
//        return true;
//    }
//    catch_bool_macro
//}

//bool rrp_cc setMinimizationModelDataSelectionList(RRMinimizationDataHandle handle, const char* selections)
//{
//    try
//    {
//        MinimizationData* data = castToMinimizationData(handle);
//        data->setModelDataSelectionList(selections);
//        return true;
//    }
//    catch_bool_macro
//}

char* rrp_cc getMinimizationDataReport(RRMinimizationDataHandle handle)
{
    try
    {
        MinimizationData* data = castToMinimizationData(handle);
        char* info = createText(data->getReport().c_str());
        return info;
    }
    catch_ptr_macro

}

}

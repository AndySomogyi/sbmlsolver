#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrc_nom_api.h"
#include "rrc_cpp_support.h"
#include "rrc_macros.h"
//---------------------------------------------------------------------------
namespace rrc
{
using namespace rr;
//NOM forwarded functions

char* rrcCallConv getModelName(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri->getNOM())
        {
            Log(lWarning)<<"NOM is not allocated.";
            return NULL;
        }
        return createText(rri->getNOM()->getModelName());
    }
    catch_ptr_macro
}

int rrcCallConv getNumberOfRules(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        if(!rri->getNOM())
        {
            Log(lWarning)<<"NOM is not allocated.";
            return -1;
        }
        int value = rri->getNOM()->getNumRules();
        return value;
    }
    catch_int_macro
}

} //namespace rrc

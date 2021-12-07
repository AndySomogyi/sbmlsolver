#pragma hdrstop
#include "telplugins_utilities.h"
#include "telplugins_cpp_support.h"
//---------------------------------------------------------------------------


char*  tpCreateText(const char* str)
{
    char* textHandle = tlpc::tpCreateText(string(str));
    return (char*) tlpc::gHM.registerHandle(textHandle, typeid(textHandle).name());
}


#ifndef rrTEventAssignmentDelegateH
#define rrTEventAssignmentDelegateH

#include "rrExporter.h"

namespace rr
{
    typedef void                         (rrCallConv *TEventAssignmentDelegate)();          //FuncPointer taking no args and returning void
    typedef TEventAssignmentDelegate*    (rrCallConv *c_TEventAssignmentDelegateStar)();    //Array of function pointers
}

#endif


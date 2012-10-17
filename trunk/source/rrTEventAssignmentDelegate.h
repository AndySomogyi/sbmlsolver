#ifndef rrTEventAssignmentDelegateH
#define rrTEventAssignmentDelegateH

namespace rr
{
    typedef void                         (__cdecl *TEventAssignmentDelegate)();            //FuncPointer taking no args and returning void
    typedef TEventAssignmentDelegate*     (__cdecl *c_TEventAssignmentDelegateStar)();    //Array of function pointers
}

#endif
//c#
//namespace LibRoadRunner
//{
//    public delegate void TEventAssignmentDelegate();
//}
//



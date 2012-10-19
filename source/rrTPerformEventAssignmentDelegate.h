#ifndef rrTPerformEventAssignmentDelegateH
#define rrTPerformEventAssignmentDelegateH
#include <vector>

using std::vector;

namespace rr
{
    typedef void                                 (__cdecl *TPerformEventAssignmentDelegate)(double* values);        //FuncPointer taking no args and returning void
    typedef TPerformEventAssignmentDelegate*     (__cdecl *c_TPerformEventAssignmentDelegateStar)();                //Array of function pointers
}

#endif



//C#
//namespace LibRoadRunner
//{
//    public delegate void TPerformEventAssignmentDelegate(double[] values);
//}

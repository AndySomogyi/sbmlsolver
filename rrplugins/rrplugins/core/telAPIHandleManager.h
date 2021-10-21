#ifndef telAPIHandleManagerH
#define telAPIHandleManagerH
#include <map>
//#include "telplugins_types.h"
//---------------------------------------------------------------------------
//Class keeping track of all handles that are created in the Tellurium C API

//using namespace tlpc;
using std::map;
typedef map<void*, const char*>   HandleMap;

class APIHandleManager
{
    public:
                                        APIHandleManager();
                                       ~APIHandleManager();
        void*                       registerHandle(void* handle,    const char* typeOf);
        bool                            unRegisterHandle(void* handle);
        void*                       validate(void* handle,          const char* typeOf, const char* fnc);

    protected:
        HandleMap                       mHandles;
        void*                       searchFor(void* handle);

};
#endif



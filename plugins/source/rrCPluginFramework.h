#ifndef rrCPluginFrameworkH
#define rrCPluginFrameworkH
#include "rrp_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rr
{
using namespace rrc;
extern "C"
{
#endif

/*!@brief C structure to hold C Plugin data */
typedef struct
{
    RRHandle   	    mRR;			//This is a handle to the roadRunner instance, creating the plugin

} *RRPluginDataPtr, RRPluginData;



#if defined( __cplusplus)
}
}//rr namespace
#endif

#endif

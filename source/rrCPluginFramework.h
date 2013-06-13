#ifndef rrCPluginFrameworkH
#define rrCPluginFrameworkH
#include "..\Wrappers\C\rrc_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rr
{
using namespace rrc;
extern "C"
{
#endif

/*!@brief C structure to hold C Plugin data */
typedef struct RRPluginData
{
    RRHandle	           	    mRR;			//This is a handle to the roadRunner instance, creating the plugin

} *RRPluginDataHandle;



#if defined( __cplusplus)
}
}//namespace

#endif


#endif

#ifndef rrp_cpp_supportH
#define rrp_cpp_supportH

#include "rrp_minimization_api.h"
#include "rrp_api.h"

namespace rrp {
class BaseParameter;
class MinimizationData;


/*!
 \brief Cast a handle to RoadRunner BaseParameter pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a baseparameter instance
 \ingroup cpp_support
*/
BaseParameter*                        castToParameter(rrp::RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner MinimizationData pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner minimization data instance
 \return Pointer to a minimization data instance
 \ingroup cpp_support
*/
MinimizationData*                  castToMinimizationData(rrp::RRMinimizationDataHandle handle);

}


#endif

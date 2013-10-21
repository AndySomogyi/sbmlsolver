#ifndef RRPLUGINS_CPP_SUPPORT_H_
#define RRPLUGINS_CPP_SUPPORT_H_

#include "rrc_minimization_api.h"
#include "rrp_api.h"

namespace rr {
class BaseParameter;
class MinimizationData;
}


/*!
 \brief Cast a handle to RoadRunner BaseParameter pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a baseparameter instance
 \ingroup cpp_support
*/
rr::BaseParameter*                        castToParameter(rrc::RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner MinimizationData pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner minimization data instance
 \return Pointer to a minimization data instance
 \ingroup cpp_support
*/
rr::MinimizationData*                  castToMinimizationData(rrc::RRMinimizationDataHandle handle);




#endif /* RRPLUGINS_CPP_SUPPORT_H_ */

#ifndef rrp_cpp_supportH
#define rrp_cpp_supportH
#include "rrp_api.h"

namespace rrp
{
template <class T>
class Parameter;
class Parameters;
class BaseParameter;
class MinimizationData;

/*!
 \brief Cast a handle to RoadRunner Parameters pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameters instance
 \return Pointer to a parameters instance
 \ingroup cpp_support
*/
Parameters*                         castToParameters(RRParametersHandle handle);

/*!
 \brief Cast a handle to RoadRunner BaseParameter pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a baseparameter instance
 \ingroup cpp_support
*/
BaseParameter*                      castToParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<int> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<int>* instance
 \ingroup cpp_support
*/
Parameter< int >*                   castToIntParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<double> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<double>* instance
 \ingroup cpp_support
*/
Parameter<double>*                  castToDoubleParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner Parameter<char*> pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner parameter instance
 \return Pointer to a Parameter<double>* instance
 \ingroup cpp_support
*/
Parameter<char*>*                   castToStringParameter(RRParameterHandle handle);

/*!
 \brief Cast a handle to RoadRunner MinimizationData pointer, throws if it fails
 \param[in] handle  A handle to a roadrunner minimization data instance
 \return Pointer to a minimization data instance
 \ingroup cpp_support
*/
MinimizationData*                   castToMinimizationData(RRMinimizationDataHandle handle);

}

#endif

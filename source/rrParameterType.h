#ifndef rrTParameterTypeH
#define rrTParameterTypeH

namespace rr
{
namespace ParameterType
{

/**
 * The type of sbml element that the RoadRunner::setParameterValue
 * and RoadRunner::getParameterValue method operate on.
 *
 * @deprecated use the ExecutableModel methods directly.
 */
enum ParameterType
{
    ptGlobalParameter = 0,
    ptLocalParameter,
    ptBoundaryParameter,
    ptConservationParameter,
    ptFloatingSpecies
};
}
}

#endif

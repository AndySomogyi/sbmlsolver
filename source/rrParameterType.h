#ifndef rrTParameterTypeH
#define rrTParameterTypeH

namespace rr
{
namespace ParameterType
{
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

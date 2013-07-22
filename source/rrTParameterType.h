#ifndef rrTParameterTypeH
#define rrTParameterTypeH

namespace rr
{
namespace TParameterType
{
enum TParameterType
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

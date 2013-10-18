/*
 * ConservationParameterPlugin.cpp
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#include "conservation/ConservationParameterPlugin.h"


namespace rr { namespace conservation {


rr::conservation::ConservationParameterPlugin::ConservationParameterPlugin(
        const std::string& uri, const std::string& prefix,
        ConservationPkgNamespaces* consns)
: SBasePlugin(uri, prefix, consns)
{
}

rr::conservation::ConservationParameterPlugin::ConservationParameterPlugin(
        const ConservationParameterPlugin& orig)
: SBasePlugin(orig)
{
}

rr::conservation::ConservationParameterPlugin::~ConservationParameterPlugin()
{
}

ConservationParameterPlugin& rr::conservation::ConservationParameterPlugin::operator =(
        const ConservationParameterPlugin& orig)
{
    return *this;
}

ConservationParameterPlugin* rr::conservation::ConservationParameterPlugin::clone() const
{
    return 0;
}


} } // namespace rr } namespace conservation }

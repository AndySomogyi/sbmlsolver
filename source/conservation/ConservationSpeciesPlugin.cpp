/*
 * ConservationSpeciesPlugin.cpp
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#include "conservation/ConservationSpeciesPlugin.h"

namespace rr { namespace conservation {


rr::conservation::ConservationSpeciesPlugin::ConservationSpeciesPlugin(
        const std::string& uri, const std::string& prefix,
        ConservationPkgNamespaces* consns)
: SBasePlugin(uri, prefix, consns)
{
}

rr::conservation::ConservationSpeciesPlugin::ConservationSpeciesPlugin(
        const ConservationSpeciesPlugin& orig)
: SBasePlugin(orig)
{
}

rr::conservation::ConservationSpeciesPlugin::~ConservationSpeciesPlugin()
{
}

ConservationSpeciesPlugin& rr::conservation::ConservationSpeciesPlugin::operator =(
        const ConservationSpeciesPlugin& orig)
{
    return *this;
}

ConservationSpeciesPlugin* rr::conservation::ConservationSpeciesPlugin::clone() const
{
    return 0;
}


} } // namespace rr } namespace conservation }

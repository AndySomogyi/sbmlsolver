/*
 * ConservationDocumentPlugin.cpp
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#include "conservation/ConservationDocumentPlugin.h"

namespace rr { namespace conservation {


rr::conservation::ConservationDocumentPlugin::ConservationDocumentPlugin(
        const std::string& uri, const std::string& prefix,
        ConservationPkgNamespaces* consns)
: SBMLDocumentPlugin(uri, prefix, consns)
{
}

rr::conservation::ConservationDocumentPlugin::ConservationDocumentPlugin(
        const ConservationDocumentPlugin& orig)
: SBMLDocumentPlugin(orig)
{
}

rr::conservation::ConservationDocumentPlugin::~ConservationDocumentPlugin()
{
}

ConservationDocumentPlugin& rr::conservation::ConservationDocumentPlugin::operator =(
        const ConservationDocumentPlugin& orig)
{
    return *this;
}

ConservationDocumentPlugin* rr::conservation::ConservationDocumentPlugin::clone() const
{
    return 0;
}


} } // namespace rr } namespace conservation }

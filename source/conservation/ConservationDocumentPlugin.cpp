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
: libsbml::SBMLDocumentPlugin(uri, prefix, consns)
{
}

rr::conservation::ConservationDocumentPlugin::ConservationDocumentPlugin(
        const ConservationDocumentPlugin& orig)
: libsbml::SBMLDocumentPlugin(orig)
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
    return new ConservationDocumentPlugin(*this);
}


} } // namespace rr } namespace conservation }

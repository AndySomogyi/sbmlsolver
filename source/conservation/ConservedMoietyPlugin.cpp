/*
 * ConservedMoietyPlugin.cpp
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#include "conservation/ConservedMoietyPlugin.h"

#include <stdexcept>


namespace rr { namespace conservation {


rr::conservation::ConservedMoietyPlugin::ConservedMoietyPlugin(
        const std::string& uri, const std::string& prefix,
        ConservationPkgNamespaces* consns)
: libsbml::SBasePlugin(uri, prefix, consns),
  conservedMoiety(false)
{
}

rr::conservation::ConservedMoietyPlugin::ConservedMoietyPlugin(
        const ConservedMoietyPlugin& orig)
: libsbml::SBasePlugin(orig),
  conservedMoiety(false)
{
}

rr::conservation::ConservedMoietyPlugin::~ConservedMoietyPlugin()
{
}

ConservedMoietyPlugin& rr::conservation::ConservedMoietyPlugin::operator =(
        const ConservedMoietyPlugin& orig)
{
    return *this;
}

ConservedMoietyPlugin* rr::conservation::ConservedMoietyPlugin::clone() const
{
    return new ConservedMoietyPlugin(*this);
}

bool ConservedMoietyPlugin::getConservedMoiety() const
{
    return conservedMoiety;
}

void ConservedMoietyPlugin::setConservedMoiety(bool value)
{
    conservedMoiety = value;
}

const std::string& ConservedMoietyPlugin::getConservedQuantity() const
{
    return conservedQuantity;
}

void ConservedMoietyPlugin::setConservedQuantity(const std::string& id)
{
    conservedQuantity = id;
}

}
} // namespace rr } namespace conservation }

void rr::conservation::ConservedMoietyPlugin::readAttributes(
        const libsbml::XMLAttributes& attributes,
        const libsbml::ExpectedAttributes& expectedAttributes)
{
    for (int i = 0; i < attributes.getLength(); ++i)
    {
        std::cout << "name: " << attributes.getName(i) << ", value: " << attributes.getValue(i) << ", uri: " << attributes.getURI(i) << std::endl;

    }
    if(attributes.hasAttribute("conservedMoiety", mURI))
    {
        if (!attributes.readInto("conservedMoiety", conservedMoiety))
        {
            std::string value = attributes.getValue("conservedMoiety");
            throw std::invalid_argument("conservedMoiety attribute with value " + value
                                        + " can not be converted to a boolean");
        }
    }
    else
    {
        conservedMoiety = false;
    }
    if(attributes.hasAttribute("conservedQuantity", mURI))
    {
        if (!attributes.readInto("conservedQuantity", conservedMoiety))
        {
            std::string value = attributes.getValue("conservedQuantity");
            throw std::invalid_argument("conservedQuantity attribute with value " + value
                                        + " can not be converted to a string");
        }
    }
}

void rr::conservation::ConservedMoietyPlugin::writeAttributes(
        libsbml::XMLOutputStream& stream) const
{

    libsbml::XMLTriple triple1("conservedMoiety",   mURI, mPrefix);
    libsbml::XMLTriple triple2("conservedQuantity", mURI, mPrefix);

    stream.writeAttribute(triple1, conservedMoiety);
    stream.writeAttribute(triple2, conservedQuantity);
}

/*
 * Configurable.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: andy
 */

#include "Configurable.h"
#include "rrStringUtils.h"
#include <libxml2/libxml/tree.h>

#define xmlchar(str) (const xmlChar*)(str)

namespace rr
{


void Configurable::loadXmlConfig(const std::string& xml,
        Configurable* configurable)
{
}

std::string Configurable::xmlFromConfigNode(const xmlNode* config)
{
    return "";
}

xmlNode* Configurable::createCapabilityNode(const std::string& name,
        const std::string& method, const std::string& desc)
{
    xmlNodePtr node = xmlNewNode(0, xmlchar("capability"));
    xmlSetProp(node, xmlchar("name"), xmlchar(name.c_str()));
    xmlSetProp(node, xmlchar("description"), xmlchar(desc.c_str()));
    return node;
}

xmlNode* Configurable::createCapabilitiesNode(const std::string& name,
        const std::string& desc)
{
    xmlNodePtr node = xmlNewNode(0, xmlchar("capabilities"));
    xmlSetProp(node, xmlchar("name"), xmlchar(name.c_str()));
    xmlSetProp(node, xmlchar("description"), xmlchar(desc.c_str()));
    return node;
}

xmlNode* Configurable::addChild(xmlNode* parent, xmlNode* cur)
{
    return 0;
}

xmlNode* Configurable::createParameterNode(const std::string& name,
        const std::string& hint, const std::string& value)
{
    xmlNodePtr node = xmlNewNode(0, xmlchar("parameter"));
    xmlSetProp(node, xmlchar("name"), xmlchar(name.c_str()));
    xmlSetProp(node, xmlchar("hint"), xmlchar(hint.c_str()));
    xmlSetProp(node, xmlchar("type"), xmlchar("string"));
    xmlSetProp(node, xmlchar("value"), xmlchar(value.c_str()));
    return node;
}

xmlNode* Configurable::createParameterNode(const std::string& name,
        const std::string& hint, int value)
{
    xmlNodePtr node = xmlNewNode(0, xmlchar("parameter"));
    xmlSetProp(node, xmlchar("name"), xmlchar(name.c_str()));
    xmlSetProp(node, xmlchar("hint"), xmlchar(hint.c_str()));
    xmlSetProp(node, xmlchar("type"), xmlchar("int"));
    xmlSetProp(node, xmlchar("value"), xmlchar(toString(value).c_str()));
    return node;
}

xmlNode* Configurable::createParameterNode(const std::string& name,
        const std::string& hint, double value)
{
    xmlNodePtr node = xmlNewNode(0, xmlchar("parameter"));
    xmlSetProp(node, xmlchar("name"), xmlchar(name.c_str()));
    xmlSetProp(node, xmlchar("hint"), xmlchar(hint.c_str()));
    xmlSetProp(node, xmlchar("type"), xmlchar("double"));
    xmlSetProp(node, xmlchar("value"), xmlchar(toString(value).c_str()));
    return node;
}

std::string Configurable::getParameterStringValue(const xmlNode* configRoot,
        const std::string& capabilityName, const std::string& parameterName)
{
    return "";
}

int Configurable::getParameterIntValue(const xmlNode* configRoot,
        const std::string& capabilityName, const std::string& parameterName)
{
    return 0;
}

double Configurable::getParameterDoubleValue(const xmlNode* configRoot,
        const std::string& capabilityName, const std::string& parameterName)
{
    return 0;
}

} /* namespace rr */

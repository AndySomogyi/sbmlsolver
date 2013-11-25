/*
 * Configurable.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "Configurable.h"
#include "rrStringUtils.h"
#include "rrLogger.h"
#include "rrOSSpecifics.h"

#include <stdexcept>
#include <cstring>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#define xmlchar(str) (const xmlChar*)(str)

namespace rr
{

void Configurable::loadXmlConfig(const std::string& xml,
        Configurable* configurable)
{
    xmlDocPtr doc = xmlReadDoc(xmlchar(xml.c_str()), 0, 0, 0);
    configurable->loadConfig(doc);
    xmlFreeDoc(doc);
}

std::string Configurable::xmlFromConfigNode(xmlNode* config)
{
    if (config == 0)
    {
        return "";
    }

    xmlDocPtr doc = NULL; /* document pointer */

    LIBXML_TEST_VERSION;

    /*
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(xmlchar("1.0"));

    xmlDocSetRootElement(doc, config);

    xmlChar *xmlbuff;
    int buffersize;

    /*
     * Dump the document to a buffer and print it
     * for demonstration purposes.
     */
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    printf("%s", (char *) xmlbuff);

    std::string result = (const char*) xmlbuff;

    /*
     * Free associated memory.
     */
    xmlFree(xmlbuff);
    xmlFreeDoc(doc);

    return result;
}

xmlNode* Configurable::createCapabilityNode(const std::string& name,
        const std::string& method, const std::string& desc)
{
    xmlNodePtr node = xmlNewNode(0, xmlchar("capability"));
    xmlSetProp(node, xmlchar("name"), xmlchar(name.c_str()));
    xmlSetProp(node, xmlchar("method"), xmlchar(method.c_str()));
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
    if (parent == 0 || cur == 0)
    {
        return 0;
    }

    const char* parentName = (const char*) parent->name;
    const char* childName = (const char*) cur->name;
    if (strcmp(parentName, "capabilities") == 0)
    {
        if (strcmp(childName, "capabilities") == 0)
        {
            xmlNode *result = 0;
            cur = cur->xmlChildrenNode;
            while (cur != NULL)
            {
                if (xmlStrcmp(cur->name, xmlchar("capability")) == 0)
                {
                    result = xmlAddChild(parent, cur);
                }
                cur = cur->next;
            }
            return result;
        }
        else if (strcmp(childName, "capability") == 0)
        {
            return xmlAddChild(parent, cur);
        }
        else if (strcmp(childName, "parameter") == 0)
        {
            // TODO error
        }
        else
        {
            // TODO error
        }
    }
    else if (strcmp(parentName, "capability") == 0)
    {
        if (strcmp(childName, "parameter") == 0)
        {
            return xmlAddChild(parent, cur);
        }
        else
        {
            // TODO error
        }
    }
    else if (strcmp(parentName, "parameter") == 0)
    {
        // TODO log error
    }
    else
    {
        // TODO log unknown element
    }
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
    xmlSetProp(node, xmlchar("value"), xmlchar(toString(value, "%e").c_str()));
    return node;
}

std::string Configurable::getParameterStringValue(const _xmlDoc *doc,
        const std::string& capabilityName, const std::string& parameterName)
{
    if (doc == 0)
    {
        throw std::invalid_argument(std::string(__FUNC__) + ", doc is NULL" );
    }

    std::string xpath = "/capabilities/capability[@name=\"" + capabilityName
            + "\"]/parameter[@name=\"" + parameterName + "\"]";

    xmlXPathContextPtr context = 0;
    xmlXPathObjectPtr xpathObj = 0;

    context = xmlXPathNewContext(const_cast<xmlDoc*>(doc));
    if (context == NULL)
    {
        printf("Error in xmlXPathNewContext\n");
        return NULL;
    }
    xpathObj = xmlXPathEvalExpression(xmlchar(xpath.c_str()), context);
    xmlXPathFreeContext(context);

    if (xpathObj == NULL)
    {
        printf("Error in xmlXPathEvalExpression\n");
        return NULL;
    }

    if (xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
    {
        xmlXPathFreeObject(xpathObj);
        printf("No result\n");
        return NULL;
    }

    if (xpathObj->nodesetval->nodeNr > 1)
    {
        // TODO log warning
    }

    xmlNode *node = xpathObj->nodesetval->nodeTab[0];

    xmlXPathFreeObject(xpathObj);

    //assert(node && "xmlNode from nodeTab is NULL");

    xmlChar* xmlStr = xmlGetProp(node, xmlchar("value"));

    std::string result = (const char*)xmlStr;

    xmlFree(xmlStr);

    return result;
}

int Configurable::getParameterIntValue(const _xmlDoc *doc,
        const std::string& capabilityName, const std::string& parameterName)
{
    std::string str = getParameterStringValue(doc, capabilityName, parameterName);
    return strToInt(str);
}

double Configurable::getParameterDoubleValue(const _xmlDoc *doc,
        const std::string& capabilityName, const std::string& parameterName)
{
    std::string str = getParameterStringValue(doc, capabilityName, parameterName);
    return strToDbl(str);
}

} /* namespace rr */

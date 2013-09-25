/*
 * Configurable.h
 *
 *  Created on: Sep 24, 2013
 *      Author: andy
 */

#ifndef CONFIGURABLE_H_
#define CONFIGURABLE_H_

#include <string>

struct _xmlNode;
struct _xmlDoc;

namespace rr
{

/**
 * The RoadRunner configuration system allows a collection of potentially
 * disparate objects to have all of thier configuration parameters collected
 * and assembled into a single xml document.
 *
 * The hierarchy of Configurable maintians no additional state other than the
 * local configuration parameters of each terminal node and the parent - child
 * relationships. The xml document / nodes exists only for the lifetime of the
 * createConfigNode / loadConfig calls. No xml objects should ever be maintined
 * outside these method calls.
 *
 * There are two method in the Configurable interface, one returns a configuration
 * as an xml node the other allows the object to load or read its configuration
 * from an xml document.
 *
 * Classes that implment this interface must return either a capabilities,
 * capability or parameter node. The static addChild function will automatically
 * determine what type the child and parents are and add it in the appropriate
 * location. So for example, say the both the parent and child nodes are
 * 'cababities', in this case, the list of 'cabability' elements is pulled
 * from the child and appended to the parent's capabitiles node as child
 * nodes. This situation is encountered when a PluginManager is used. RoadRunner
 * does not require nor is even aware of a PluginManager. RoadRunner considers
 * itself a top level object thus returns a 'capabilities' node. The PluginManager
 * contains a RoadRunner object, so when the PluginManager creates a 'cababilities'
 * node, asks all of its plugins for a config, these are typically a 'cabability'
 * which are added to the 'cababities' node that the PluginManager created. The
 * PluginManager also asks RoadRunner for its capabilities, so in this case,
 * RoadRunner's set of cabability nodes are automatically appended to the PluginManager's
 * cababity nodes and a single combined document is returned.
 */
class Configurable
{
public:

    /**
     * creates a new xml element that represent the current state of this
     * Configurable object and all if its child objects.
     */
    virtual _xmlNode *createConfigNode() = 0;

    /**
     * Given an xml element, the Configurable object should pick its needed
     * values that are stored in the element and use them to set its
     * internal configuration state.
     */
    virtual void loadConfig(const _xmlDoc* doc) = 0;

    /**
     * given an xml string, this loads the string into a document and calls
     * loadConfig on the given configurable pointer with the root element.
     *
     * Also checks that the root element is a "capabilties" element.
     */
    static void loadXmlConfig(const std::string& xml,
            Configurable* configurable);

    /**
     * Consumes an xmlNode and creates a new document and sets this as
     * the root node, The document takes ownership of the xmlNode.
     *
     * The document is then written out to a string which is returned,
     * and the document is then finally freed.
     */
    static std::string xmlFromConfigNode(_xmlNode* config);

    /**
     * create a "capability" node with "name", "method" and "description"
     * atttibutes.
     */
    static _xmlNode *createCapabilityNode(const std::string& name,
            const std::string& method, const std::string& desc);

    /**
     * create a "capabilities" node with "name" and "description"
     * attributes.
     */
    static _xmlNode *createCapabilitiesNode(const std::string& name,
            const std::string& desc);

    /**
     * essentially just calls xmlAddChild, but performs some checking to
     * verify that a parameter can only be added to a capability and a
     * capabilty can only be added to a capabilities node.
     *
     * as usual, the parent takes ownership of the child node.
     */
    static _xmlNode *addChild(_xmlNode *parent, _xmlNode *cur);

    /**
     * create a "parameter" node with a 'string' value.
     */
    static _xmlNode *createParameterNode(const std::string& name,
            const std::string& hint, const std::string& value);

    /**
     * create a "parameter" node with a 'integer' value.
     */
    static _xmlNode *createParameterNode(const std::string& name,
            const std::string& hint, int value);

    /**
     * create a "parameter" node with a 'double' value.
     */
    static _xmlNode *createParameterNode(const std::string& name,
            const std::string& hint, double value);

    /**
     * find the parameter node for the given capability / parameter in the
     * xml configuration document and return its value.
     *
     * @throws std::exception on failure.
     */
    static std::string getParameterStringValue(const _xmlDoc *doc,
            const std::string& capabilityName,
            const std::string& parameterName);

    /**
     * find the parameter node for the given capability / parameter in the
     * xml configuration document and coerce the value to an integer.
     *
     * @throws std::exception on failure.
     */
    static int getParameterIntValue(const _xmlDoc *doc,
            const std::string& capabilityName,
            const std::string& parameterName);

    /**
     * find the parameter node for the given capability / parameter in the
     * xml configuration document and coerce the value to a double.
     *
     * @throws std::exception on failure.
     */
    static double getParameterDoubleValue(const _xmlDoc *doc,
            const std::string& capabilityName,
            const std::string& parameterName);

};

} /* namespace rr */
#endif /* CONFIGURABLE_H_ */

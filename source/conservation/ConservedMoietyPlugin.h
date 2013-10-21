/*
 * ConservedMoietyPlugin.h
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#ifndef ConservedMoietyPlugin_H_
#define ConservedMoietyPlugin_H_

#include "rrExporter.h"
#include "ConservationExtension.h"
#include <sbml/extension/SBasePlugin.h>

namespace rr
{
namespace conservation
{

class RR_DECLSPEC ConservedMoietyPlugin: public libsbml::SBasePlugin
{
public:

    /**
     * Constructor
     */
    ConservedMoietyPlugin(const std::string &uri, const std::string &prefix,
            ConservationPkgNamespaces *consns);

    /**
     * Copy constructor. Creates a copy of this SBase object.
     */
    ConservedMoietyPlugin(const ConservedMoietyPlugin& orig);

    /**
     * Destroy this object.
     */
    virtual ~ConservedMoietyPlugin();

    /**
     * Assignment operator for ConservedMoietyPlugin.
     */
    ConservedMoietyPlugin& operator=(const ConservedMoietyPlugin& orig);

    /**
     * Creates and returns a deep copy of this ConservedMoietyPlugin object.
     *
     * @return a (deep) copy of this SBase object
     */
    virtual ConservedMoietyPlugin* clone() const;

    bool getConservedMoiety();

    void setConservedMoiety(bool value);

};

}
} // namespace rr } namespace conservation }
#endif /* ConservedMoietyPlugin_H_ */

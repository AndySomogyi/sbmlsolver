/*
 * ConservationSpeciesPlugin.h
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#ifndef CONSERVATIONSPECIESPLUGIN_H_
#define CONSERVATIONSPECIESPLUGIN_H_

#include "ConservationExtension.h"
#include <sbml/extension/SBasePlugin.h>


namespace rr { namespace conservation {


class ConservationSpeciesPlugin: public libsbml::SBasePlugin
{
public:

  /**
   * Constructor
   */
  ConservationSpeciesPlugin (const std::string &uri, const std::string &prefix,
          ConservationPkgNamespaces *consns);


  /**
   * Copy constructor. Creates a copy of this SBase object.
   */
  ConservationSpeciesPlugin(const ConservationSpeciesPlugin& orig);


  /**
   * Destroy this object.
   */
  virtual ~ConservationSpeciesPlugin ();


  /**
   * Assignment operator for ConservationSpeciesPlugin.
   */
  ConservationSpeciesPlugin& operator=(const ConservationSpeciesPlugin& orig);


  /**
   * Creates and returns a deep copy of this ConservationSpeciesPlugin object.
   *
   * @return a (deep) copy of this SBase object
   */
  virtual ConservationSpeciesPlugin* clone () const;
};

} } // namespace rr } namespace conservation }

#endif /* CONSERVATIONSPECIESPLUGIN_H_ */

/*
 * ConservationParameterPlugin.h
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#ifndef CONSERVATIONPARAMETERPLUGIN_H_
#define CONSERVATIONPARAMETERPLUGIN_H_


#include "ConservationExtension.h"
#include <sbml/extension/SBasePlugin.h>


namespace rr { namespace conservation {


class ConservationParameterPlugin: public libsbml::SBasePlugin
{
public:

  /**
   * Constructor
   */
  ConservationParameterPlugin (const std::string &uri, const std::string &prefix,
          ConservationPkgNamespaces *consns);


  /**
   * Copy constructor. Creates a copy of this SBase object.
   */
  ConservationParameterPlugin(const ConservationParameterPlugin& orig);


  /**
   * Destroy this object.
   */
  virtual ~ConservationParameterPlugin ();


  /**
   * Assignment operator for ConservationParameterPlugin.
   */
  ConservationParameterPlugin& operator=(const ConservationParameterPlugin& orig);


  /**
   * Creates and returns a deep copy of this ConservationParameterPlugin object.
   *
   * @return a (deep) copy of this SBase object
   */
  virtual ConservationParameterPlugin* clone () const;
};

} } // namespace rr } namespace conservation }
#endif /* CONSERVATIONPARAMETERPLUGIN_H_ */

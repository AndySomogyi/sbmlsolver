/*
 * ConservationDocumentPlugin.h
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#ifndef ConservationDocumentPlugin_H_
#define ConservationDocumentPlugin_H_

#include "rrExporter.h"
#include "ConservationExtension.h"
#include <sbml/extension/SBasePlugin.h>
#include <sbml/extension/SBMLDocumentPlugin.h>


namespace rr { namespace conservation {


class RR_DECLSPEC ConservationDocumentPlugin: public libsbml::SBMLDocumentPlugin
{
public:

  /**
   * Constructor
   */
  ConservationDocumentPlugin (const std::string &uri, const std::string &prefix,
          ConservationPkgNamespaces *consns);


  /**
   * Copy constructor. Creates a copy of this SBase object.
   */
  ConservationDocumentPlugin(const ConservationDocumentPlugin& orig);


  /**
   * Destroy this object.
   */
  virtual ~ConservationDocumentPlugin ();


  /**
   * Assignment operator for ConservationDocumentPlugin.
   */
  ConservationDocumentPlugin& operator=(const ConservationDocumentPlugin& orig);


  /**
   * Creates and returns a deep copy of this ConservationDocumentPlugin object.
   *
   * @return a (deep) copy of this SBase object
   */
  virtual ConservationDocumentPlugin* clone () const;
};

} } // namespace rr } namespace conservation }

#endif /* ConservationDocumentPlugin_H_ */

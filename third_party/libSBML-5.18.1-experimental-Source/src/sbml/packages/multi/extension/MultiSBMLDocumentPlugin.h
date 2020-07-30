/**
 * @file:   MultiSBMLDocumentPlugin.h
 * @brief:  Implementation of the MultiSBMLDocumentPlugin class
 * @author: SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class MultiSBMLDocumentPlugin
 * @sbmlbrief{multi} Extension of SBMLDocument for the "multi" package.
 *
 * The MultiSBMLDocumentPlugin class inherits from the SBMLDocumentPlugin
 * class, and codifies the extensions to the SBMLDocument class defined in
 * the SBML Level&nbsp;3 @ref multi ("multi") package.  This extension
 * defines a required flag named "required", which indicates whether "multi"
 * constructs can be used to change the core mathematical interpretation of
 * the Model defined in the SBML input.  Since "multi" constructs can, this
 * attribute must be set to the value @c "true".
 */

#ifndef MultiSBMLDocumentPlugin_H__
#define MultiSBMLDocumentPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBMLDocumentPlugin.h>
#include <sbml/packages/multi/extension/MultiExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN MultiSBMLDocumentPlugin : public SBMLDocumentPlugin
{
public:

  /**
   * Creates a new MultiSBMLDocumentPlugin object.
   *
   * @copydetails doc_what_are_xmlnamespaces
   *
   * @copydetails doc_what_are_sbmlnamespaces
   *
   * @param uri the URI of the SBML Level&nbsp;3 package implemented by
   * this libSBML package extension.
   *
   * @param prefix the XML namespace prefix being used for the package.
   *
   * @param multins the namespaces object for the package.
   */
  MultiSBMLDocumentPlugin(const std::string& uri, const std::string& prefix,
                          MultiPkgNamespaces* multins);


  /**
   * Copy constructor for MultiSBMLDocumentPlugin.
   *
   * @param orig the MultiSBMLDocumentPlugin instance to copy.
   */
  MultiSBMLDocumentPlugin(const MultiSBMLDocumentPlugin& orig);


  /**
   * Assignment operator for MultiSBMLDocumentPlugin.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  MultiSBMLDocumentPlugin& operator=(const MultiSBMLDocumentPlugin& rhs);


  /**
   * Creates and returns a deep copy of this MultiSBMLDocumentPlugin object.
   *
   * @return a (deep) copy of this MultiSBMLDocumentPlugin object.
   */
  virtual MultiSBMLDocumentPlugin* clone () const;


   /**
   * Destructor for MultiSBMLDocumentPlugin.
   */
  virtual ~MultiSBMLDocumentPlugin();


 #ifndef SWIG

  /** @cond doxygenLibsbmlInternal */
  /**
   * Reads the attributes of corresponding package in SBMLDocument element
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);
  /** @endcond */


#endif // SWIG

  /** @cond doxygenLibsbmlInternal */
  /**
   * Returns boolean based on whether flattening of a comp model has been implemented.
   *
   * @returns @c true if flattening for composed models has been implemented,
   * false otherwise.
   */
  virtual bool isCompFlatteningImplemented() const;
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Check consistency function.
   */
  virtual unsigned int checkConsistency();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Accepts the SBMLVisitor.
   */
//  virtual bool accept(SBMLVisitor& v) const;
   /** @endcond */


protected:

  /** @cond doxygenLibsbmlInternal */

  /** @endcond */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* MultiSBMLDocumentPlugin_H__ */



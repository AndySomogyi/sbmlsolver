/**
 * @file:   MultiReactionPlugin.h
 * @brief:  Implementation of the MultiReactionPlugin class
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
 */


#ifndef MultiReactionPlugin_H__
#define MultiReactionPlugin_H__


#include <sbml/common/extern.h>


#ifdef __cplusplus


#include <sbml/extension/SBasePlugin.h>
#include <sbml/packages/multi/extension/MultiExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN MultiReactionPlugin : public SBasePlugin
{
public:

  /**
   * Creates a new MultiReactionPlugin
   */
  MultiReactionPlugin(const std::string& uri, const std::string& prefix, 
                                 MultiPkgNamespaces* multins);


  /**
   * Copy constructor for MultiReactionPlugin.
   *
   * @param orig; the MultiReactionPlugin instance to copy.
   */
  MultiReactionPlugin(const MultiReactionPlugin& orig);


   /**
   * Assignment operator for MultiReactionPlugin.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  MultiReactionPlugin& operator=(const MultiReactionPlugin& rhs);


   /**
   * Creates and returns a deep copy of this MultiReactionPlugin object.
   *
   * @return a (deep) copy of this MultiReactionPlugin object.
   */
  virtual MultiReactionPlugin* clone () const;


   /**
   * Destructor for MultiReactionPlugin.
   */
  virtual ~MultiReactionPlugin();


   //---------------------------------------------------------------
  //
  // overridden virtual functions for read/write/check
  //
  //---------------------------------------------------------------

  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses must override this method to create, store, and then
   * return an SBML object corresponding to the next XMLToken in the
   * XMLInputStream if they have their specific elements.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses must override this method to write out their contained
   * SBML objects as XML elements if they have their specific elements.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */


  /**
   * Checks if this plugin object has all the required elements.
   *
   * Subclasses must override this method 
   * if they have their specific elements.
   *
   * @return true if this plugin object has all the required elements
   * otherwise false will be returned.
   */
  virtual bool hasRequiredElements () const;


  //---------------------------------------------------------------


  //---------------------------------------------------------------
  //
  // Functions for interacting with the members of the plugin
  //
  //---------------------------------------------------------------

  /**
   * Returns the value of the "isIntraSpeciesReaction" attribute of this CompPlugin.
   *
   * @return the value of the "isIntraSpeciesReaction" attribute of this CompPlugin as a boolean.
   */
  virtual bool getIsIntraSpeciesReaction() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * CompPlugin's "isIntraSpeciesReaction" attribute has been set.
   *
   * @return @c true if this CompPlugin's "isIntraSpeciesReaction" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetIsIntraSpeciesReaction() const;


  /**
   * Sets the value of the "isIntraSpeciesReaction" attribute of this CompPlugin.
   *
   * @param isIntraSpeciesReaction; bool value of the "isIntraSpeciesReaction" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setIsIntraSpeciesReaction(bool isIntraSpeciesReaction);


  /**
   * Unsets the value of the "isIntraSpeciesReaction" attribute of this CompPlugin.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetIsIntraSpeciesReaction();



  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitary depth.
   *
   * @return a List* of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  virtual void connectToParent (SBase* sbase);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  virtual bool accept (SBMLVisitor& v) const;

  /** @endcond doxygenLibsbmlInternal */


protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Read values from the given XMLAttributes set into their specific fields.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write values of XMLAttributes to the output stream.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */

  /** @cond doxygenLibsbmlInternal */

  bool          mIsIntraSpeciesReaction;
  bool          mIsSetIsIntraSpeciesReaction;

  /** @endcond doxygenLibsbmlInternal */


};




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */
#endif /* MultiReactionPlugin_H__ */



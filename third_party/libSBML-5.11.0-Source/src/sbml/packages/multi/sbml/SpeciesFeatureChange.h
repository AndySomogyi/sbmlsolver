/**
 * @file:   SpeciesFeatureChange.h
 * @brief:  Implementation of the SpeciesFeatureChange class
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


#ifndef SpeciesFeatureChange_H__
#define SpeciesFeatureChange_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/multi/common/multifwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/multi/extension/MultiExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN SpeciesFeatureChange : public SBase
{

protected:

  std::string   mId;
  std::string   mReactantSpeciesFeature;
  std::string   mProductSpeciesFeature;


public:

  /**
   * Creates a new SpeciesFeatureChange with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this SpeciesFeatureChange
   *
   * @param version an unsigned int, the SBML Version to assign to this SpeciesFeatureChange
   *
   * @param pkgVersion an unsigned int, the SBML Multi Version to assign to this SpeciesFeatureChange
   */
  SpeciesFeatureChange(unsigned int level      = MultiExtension::getDefaultLevel(),
                       unsigned int version    = MultiExtension::getDefaultVersion(),
                       unsigned int pkgVersion = MultiExtension::getDefaultPackageVersion());


  /**
   * Creates a new SpeciesFeatureChange with the given MultiPkgNamespaces object.
   *
   * @param multins the MultiPkgNamespaces object
   */
  SpeciesFeatureChange(MultiPkgNamespaces* multins);


   /**
   * Copy constructor for SpeciesFeatureChange.
   *
   * @param orig; the SpeciesFeatureChange instance to copy.
   */
  SpeciesFeatureChange(const SpeciesFeatureChange& orig);


   /**
   * Assignment operator for SpeciesFeatureChange.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SpeciesFeatureChange& operator=(const SpeciesFeatureChange& rhs);


   /**
   * Creates and returns a deep copy of this SpeciesFeatureChange object.
   *
   * @return a (deep) copy of this SpeciesFeatureChange object.
   */
  virtual SpeciesFeatureChange* clone () const;


   /**
   * Destructor for SpeciesFeatureChange.
   */
  virtual ~SpeciesFeatureChange();


   /**
   * Returns the value of the "id" attribute of this SpeciesFeatureChange.
   *
   * @return the value of the "id" attribute of this SpeciesFeatureChange as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SpeciesFeatureChange's "id" attribute has been set.
   *
   * @return @c true if this SpeciesFeatureChange's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this SpeciesFeatureChange.
   *
   * @param id; const std::string& value of the "id" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setId(const std::string& id);


  /**
   * Unsets the value of the "id" attribute of this SpeciesFeatureChange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetId();


  /**
   * Returns the value of the "reactantSpeciesFeature" attribute of this SpeciesFeatureChange.
   *
   * @return the value of the "reactantSpeciesFeature" attribute of this SpeciesFeatureChange as a string.
   */
  virtual const std::string& getReactantSpeciesFeature() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SpeciesFeatureChange's "reactantSpeciesFeature" attribute has been set.
   *
   * @return @c true if this SpeciesFeatureChange's "reactantSpeciesFeature" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetReactantSpeciesFeature() const;


  /**
   * Sets the value of the "reactantSpeciesFeature" attribute of this SpeciesFeatureChange.
   *
   * @param reactantSpeciesFeature; const std::string& value of the "reactantSpeciesFeature" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setReactantSpeciesFeature(const std::string& reactantSpeciesFeature);


  /**
   * Unsets the value of the "reactantSpeciesFeature" attribute of this SpeciesFeatureChange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetReactantSpeciesFeature();


  /**
   * Returns the value of the "productSpeciesFeature" attribute of this SpeciesFeatureChange.
   *
   * @return the value of the "productSpeciesFeature" attribute of this SpeciesFeatureChange as a string.
   */
  virtual const std::string& getProductSpeciesFeature() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SpeciesFeatureChange's "productSpeciesFeature" attribute has been set.
   *
   * @return @c true if this SpeciesFeatureChange's "productSpeciesFeature" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetProductSpeciesFeature() const;


  /**
   * Sets the value of the "productSpeciesFeature" attribute of this SpeciesFeatureChange.
   *
   * @param productSpeciesFeature; const std::string& value of the "productSpeciesFeature" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setProductSpeciesFeature(const std::string& productSpeciesFeature);


  /**
   * Unsets the value of the "productSpeciesFeature" attribute of this SpeciesFeatureChange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetProductSpeciesFeature();


  /**
   * Renames all the @c SIdRef attributes on this element, including any
   * found in MathML content (if such exists).
   *
   * This method works by looking at all attributes and (if appropriate)
   * mathematical formulas, comparing the identifiers to the value of @p
   * oldid.  If any matches are found, the matching identifiers are replaced
   * with @p newid.  The method does @em not descend into child elements.
   *
   * @param oldid the old identifier
   * @param newid the new identifier
   */
   virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * Returns the XML element name of this object, which for SpeciesFeatureChange, is
   * always @c "speciesFeatureChange".
   *
   * @return the name of this element, i.e. @c "speciesFeatureChange".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   *
   * @return the SBML type code for this object, or
   * @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes
   * for this SpeciesFeatureChange object have been set.
   *
   * @note The required attributes for a SpeciesFeatureChange object are:
   * @li "reactantSpeciesFeature"
   * @li "productSpeciesFeature"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor.
   */
  virtual bool accept (SBMLVisitor& v) const;


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Enables/Disables the given package with this element.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
               const std::string& pkgPrefix, bool flag);


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



};

class LIBSBML_EXTERN ListOfSpeciesFeatureChanges : public ListOf
{

public:

  /**
   * Creates a new ListOfSpeciesFeatureChanges with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this ListOfSpeciesFeatureChanges
   *
   * @param version an unsigned int, the SBML Version to assign to this ListOfSpeciesFeatureChanges
   *
   * @param pkgVersion an unsigned int, the SBML Multi Version to assign to this ListOfSpeciesFeatureChanges
   */
  ListOfSpeciesFeatureChanges(unsigned int level      = MultiExtension::getDefaultLevel(),
                              unsigned int version    = MultiExtension::getDefaultVersion(),
                              unsigned int pkgVersion = MultiExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfSpeciesFeatureChanges with the given MultiPkgNamespaces object.
   *
   * @param multins the MultiPkgNamespaces object
   */
  ListOfSpeciesFeatureChanges(MultiPkgNamespaces* multins);


   /**
   * Creates and returns a deep copy of this ListOfSpeciesFeatureChanges object.
   *
   * @return a (deep) copy of this ListOfSpeciesFeatureChanges object.
   */
  virtual ListOfSpeciesFeatureChanges* clone () const;


   /**
   * Get a SpeciesFeatureChange from the ListOfSpeciesFeatureChanges.
   *
   * @param n the index number of the SpeciesFeatureChange to get.
   *
   * @return the nth SpeciesFeatureChange in this ListOfSpeciesFeatureChanges.
   *
   * @see size()
   */
  virtual SpeciesFeatureChange* get(unsigned int n);


  /**
   * Get a SpeciesFeatureChange from the ListOfSpeciesFeatureChanges.
   *
   * @param n the index number of the SpeciesFeatureChange to get.
   *
   * @return the nth SpeciesFeatureChange in this ListOfSpeciesFeatureChanges.
   *
   * @see size()
   */
  virtual const SpeciesFeatureChange* get(unsigned int n) const;


  /**
   * Get a SpeciesFeatureChange from the ListOfSpeciesFeatureChanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the SpeciesFeatureChange to get.
   *
   * @return SpeciesFeatureChange in this ListOfSpeciesFeatureChanges
   * with the given id or NULL if no such
   * SpeciesFeatureChange exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual SpeciesFeatureChange* get(const std::string& sid);


  /**
   * Get a SpeciesFeatureChange from the ListOfSpeciesFeatureChanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the SpeciesFeatureChange to get.
   *
   * @return SpeciesFeatureChange in this ListOfSpeciesFeatureChanges
   * with the given id or NULL if no such
   * SpeciesFeatureChange exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual const SpeciesFeatureChange* get(const std::string& sid) const;


  /**
   * Removes the nth SpeciesFeatureChange from this ListOfSpeciesFeatureChanges
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the SpeciesFeatureChange to remove.
   *
   * @see size()
   */
  virtual SpeciesFeatureChange* remove(unsigned int n);


  /**
   * Removes the SpeciesFeatureChange from this ListOfSpeciesFeatureChanges with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the SpeciesFeatureChange to remove.
   *
   * @return the SpeciesFeatureChange removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SpeciesFeatureChange* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for ListOfSpeciesFeatureChanges, is
   * always @c "listOfSpeciesFeatureChanges".
   *
   * @return the name of this element, i.e. @c "listOfSpeciesFeatureChanges".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   *
   * @return the SBML type code for this object, or
   * @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getTypeCode () const;


  /**
   * Returns the libSBML type code for the SBML objects
   * contained in this ListOf object
   * 
   * @if clike LibSBML attaches an identifying code to every kind of SBML
   * object.  These are known as <em>SBML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SBMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SBML_. @endif@if java LibSBML attaches an identifying code to every
   * kind of SBML object.  These are known as <em>SBML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSBML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if python LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the Python language interface for libSBML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SBML_. @endif@if csharp LibSBML attaches an identifying
   * code to every kind of SBML object.  These are known as <em>SBML type
   * codes</em>.  In the C# language interface for libSBML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SBML_. @endif
   *
   * @return the SBML type code for the objects in this ListOf instance, or
   * @link SBMLTypeCode_t#SBML_UNKNOWN SBML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getItemTypeCode () const;


protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates a new SpeciesFeatureChange in this ListOfSpeciesFeatureChanges
   */
  virtual SBase* createObject(XMLInputStream& stream);


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write the namespace for the Multi package.
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;


  /** @endcond doxygenLibsbmlInternal */



};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

LIBSBML_EXTERN
SpeciesFeatureChange_t *
SpeciesFeatureChange_create(unsigned int level, unsigned int version,
                            unsigned int pkgVersion);


LIBSBML_EXTERN
void
SpeciesFeatureChange_free(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
SpeciesFeatureChange_t *
SpeciesFeatureChange_clone(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
char *
SpeciesFeatureChange_getId(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
char *
SpeciesFeatureChange_getReactantSpeciesFeature(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
char *
SpeciesFeatureChange_getProductSpeciesFeature(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
int
SpeciesFeatureChange_isSetId(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
int
SpeciesFeatureChange_isSetReactantSpeciesFeature(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
int
SpeciesFeatureChange_isSetProductSpeciesFeature(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
int
SpeciesFeatureChange_setId(SpeciesFeatureChange_t * sfc, const char * id);


LIBSBML_EXTERN
int
SpeciesFeatureChange_setReactantSpeciesFeature(SpeciesFeatureChange_t * sfc, const char * reactantSpeciesFeature);


LIBSBML_EXTERN
int
SpeciesFeatureChange_setProductSpeciesFeature(SpeciesFeatureChange_t * sfc, const char * productSpeciesFeature);


LIBSBML_EXTERN
int
SpeciesFeatureChange_unsetId(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
int
SpeciesFeatureChange_unsetReactantSpeciesFeature(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
int
SpeciesFeatureChange_unsetProductSpeciesFeature(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
int
SpeciesFeatureChange_hasRequiredAttributes(SpeciesFeatureChange_t * sfc);


LIBSBML_EXTERN
SpeciesFeatureChange_t *
ListOfSpeciesFeatureChanges_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
SpeciesFeatureChange_t *
ListOfSpeciesFeatureChanges_removeById(ListOf_t * lo, const char * sid);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SpeciesFeatureChange_H__  */


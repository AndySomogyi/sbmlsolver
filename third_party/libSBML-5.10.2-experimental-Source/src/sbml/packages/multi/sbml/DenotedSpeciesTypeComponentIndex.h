/**
 * @file:   DenotedSpeciesTypeComponentIndex.h
 * @brief:  Implementation of the DenotedSpeciesTypeComponentIndex class
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
 * @class DenotedSpeciesTypeComponentIndex
 *
 */


#ifndef DenotedSpeciesTypeComponentIndex_H__
#define DenotedSpeciesTypeComponentIndex_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/multi/common/multifwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/multi/extension/MultiExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN DenotedSpeciesTypeComponentIndex : public SBase
{

protected:

  std::string   mSpeciesTypeComponentIndex;


public:

  /**
   * Creates a new DenotedSpeciesTypeComponentIndex with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this DenotedSpeciesTypeComponentIndex
   *
   * @param version an unsigned int, the SBML Version to assign to this DenotedSpeciesTypeComponentIndex
   *
   * @param pkgVersion an unsigned int, the SBML Multi Version to assign to this DenotedSpeciesTypeComponentIndex
   */
  DenotedSpeciesTypeComponentIndex(unsigned int level      = MultiExtension::getDefaultLevel(),
                                   unsigned int version    = MultiExtension::getDefaultVersion(),
                                   unsigned int pkgVersion = MultiExtension::getDefaultPackageVersion());


  /**
   * Creates a new DenotedSpeciesTypeComponentIndex with the given MultiPkgNamespaces object.
   *
   * @param multins the MultiPkgNamespaces object
   */
  DenotedSpeciesTypeComponentIndex(MultiPkgNamespaces* multins);


   /**
   * Copy constructor for DenotedSpeciesTypeComponentIndex.
   *
   * @param orig; the DenotedSpeciesTypeComponentIndex instance to copy.
   */
  DenotedSpeciesTypeComponentIndex(const DenotedSpeciesTypeComponentIndex& orig);


   /**
   * Assignment operator for DenotedSpeciesTypeComponentIndex.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  DenotedSpeciesTypeComponentIndex& operator=(const DenotedSpeciesTypeComponentIndex& rhs);


   /**
   * Creates and returns a deep copy of this DenotedSpeciesTypeComponentIndex object.
   *
   * @return a (deep) copy of this DenotedSpeciesTypeComponentIndex object.
   */
  virtual DenotedSpeciesTypeComponentIndex* clone () const;


   /**
   * Destructor for DenotedSpeciesTypeComponentIndex.
   */
  virtual ~DenotedSpeciesTypeComponentIndex();


   /**
   * Returns the value of the "speciesTypeComponentIndex" attribute of this DenotedSpeciesTypeComponentIndex.
   *
   * @return the value of the "speciesTypeComponentIndex" attribute of this DenotedSpeciesTypeComponentIndex as a string.
   */
  virtual const std::string& getSpeciesTypeComponentIndex() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * DenotedSpeciesTypeComponentIndex's "speciesTypeComponentIndex" attribute has been set.
   *
   * @return @c true if this DenotedSpeciesTypeComponentIndex's "speciesTypeComponentIndex" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetSpeciesTypeComponentIndex() const;


  /**
   * Sets the value of the "speciesTypeComponentIndex" attribute of this DenotedSpeciesTypeComponentIndex.
   *
   * @param speciesTypeComponentIndex; const std::string& value of the "speciesTypeComponentIndex" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setSpeciesTypeComponentIndex(const std::string& speciesTypeComponentIndex);


  /**
   * Unsets the value of the "speciesTypeComponentIndex" attribute of this DenotedSpeciesTypeComponentIndex.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetSpeciesTypeComponentIndex();


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
   * Returns the XML element name of this object, which for DenotedSpeciesTypeComponentIndex, is
   * always @c "denotedSpeciesTypeComponentIndex".
   *
   * @return the name of this element, i.e. @c "denotedSpeciesTypeComponentIndex".
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
   * for this DenotedSpeciesTypeComponentIndex object have been set.
   *
   * @note The required attributes for a DenotedSpeciesTypeComponentIndex object are:
   * @li "speciesTypeComponentIndex"
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

class LIBSBML_EXTERN ListOfDenotedSpeciesTypeComponentIndexes : public ListOf
{

public:

  /**
   * Creates a new ListOfDenotedSpeciesTypeComponentIndexes with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this ListOfDenotedSpeciesTypeComponentIndexes
   *
   * @param version an unsigned int, the SBML Version to assign to this ListOfDenotedSpeciesTypeComponentIndexes
   *
   * @param pkgVersion an unsigned int, the SBML Multi Version to assign to this ListOfDenotedSpeciesTypeComponentIndexes
   */
  ListOfDenotedSpeciesTypeComponentIndexes(unsigned int level      = MultiExtension::getDefaultLevel(),
                                          unsigned int version    = MultiExtension::getDefaultVersion(),
                                          unsigned int pkgVersion = MultiExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfDenotedSpeciesTypeComponentIndexes with the given MultiPkgNamespaces object.
   *
   * @param multins the MultiPkgNamespaces object
   */
  ListOfDenotedSpeciesTypeComponentIndexes(MultiPkgNamespaces* multins);


   /**
   * Creates and returns a deep copy of this ListOfDenotedSpeciesTypeComponentIndexes object.
   *
   * @return a (deep) copy of this ListOfDenotedSpeciesTypeComponentIndexes object.
   */
  virtual ListOfDenotedSpeciesTypeComponentIndexes* clone () const;


   /**
   * Get a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes.
   *
   * @param n the index number of the DenotedSpeciesTypeComponentIndex to get.
   *
   * @return the nth DenotedSpeciesTypeComponentIndex in this ListOfDenotedSpeciesTypeComponentIndexes.
   *
   * @see size()
   */
  virtual DenotedSpeciesTypeComponentIndex* get(unsigned int n);


  /**
   * Get a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes.
   *
   * @param n the index number of the DenotedSpeciesTypeComponentIndex to get.
   *
   * @return the nth DenotedSpeciesTypeComponentIndex in this ListOfDenotedSpeciesTypeComponentIndexes.
   *
   * @see size()
   */
  virtual const DenotedSpeciesTypeComponentIndex* get(unsigned int n) const;


  /**
   * Get a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DenotedSpeciesTypeComponentIndex to get.
   *
   * @return DenotedSpeciesTypeComponentIndex in this ListOfDenotedSpeciesTypeComponentIndexes
   * with the given id or NULL if no such
   * DenotedSpeciesTypeComponentIndex exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual DenotedSpeciesTypeComponentIndex* get(const std::string& sid);


  /**
   * Get a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DenotedSpeciesTypeComponentIndex to get.
   *
   * @return DenotedSpeciesTypeComponentIndex in this ListOfDenotedSpeciesTypeComponentIndexes
   * with the given id or NULL if no such
   * DenotedSpeciesTypeComponentIndex exists.
   *
   * @see get(unsigned int n)   *
   * @see size()
   */
  virtual const DenotedSpeciesTypeComponentIndex* get(const std::string& sid) const;


  /**
   * Removes the nth DenotedSpeciesTypeComponentIndex from this ListOfDenotedSpeciesTypeComponentIndexes
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the DenotedSpeciesTypeComponentIndex to remove.
   *
   * @see size()
   */
  virtual DenotedSpeciesTypeComponentIndex* remove(unsigned int n);


  /**
   * Removes the DenotedSpeciesTypeComponentIndex from this ListOfDenotedSpeciesTypeComponentIndexes with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the DenotedSpeciesTypeComponentIndex to remove.
   *
   * @return the DenotedSpeciesTypeComponentIndex removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual DenotedSpeciesTypeComponentIndex* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for ListOfDenotedSpeciesTypeComponentIndexes, is
   * always @c "listOfDenotedSpeciesTypeComponentIndexes".
   *
   * @return the name of this element, i.e. @c "listOfDenotedSpeciesTypeComponentIndexes".
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
   * Creates a new DenotedSpeciesTypeComponentIndex in this ListOfDenotedSpeciesTypeComponentIndexes
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
DenotedSpeciesTypeComponentIndex_t *
DenotedSpeciesTypeComponentIndex_create(unsigned int level, unsigned int version,
                                        unsigned int pkgVersion);


LIBSBML_EXTERN
void
DenotedSpeciesTypeComponentIndex_free(DenotedSpeciesTypeComponentIndex_t * dstci);


LIBSBML_EXTERN
DenotedSpeciesTypeComponentIndex_t *
DenotedSpeciesTypeComponentIndex_clone(DenotedSpeciesTypeComponentIndex_t * dstci);


LIBSBML_EXTERN
char *
DenotedSpeciesTypeComponentIndex_getSpeciesTypeComponentIndex(DenotedSpeciesTypeComponentIndex_t * dstci);


LIBSBML_EXTERN
int
DenotedSpeciesTypeComponentIndex_isSetSpeciesTypeComponentIndex(DenotedSpeciesTypeComponentIndex_t * dstci);


LIBSBML_EXTERN
int
DenotedSpeciesTypeComponentIndex_setSpeciesTypeComponentIndex(DenotedSpeciesTypeComponentIndex_t * dstci, const char * speciesTypeComponentIndex);


LIBSBML_EXTERN
int
DenotedSpeciesTypeComponentIndex_unsetSpeciesTypeComponentIndex(DenotedSpeciesTypeComponentIndex_t * dstci);


LIBSBML_EXTERN
int
DenotedSpeciesTypeComponentIndex_hasRequiredAttributes(DenotedSpeciesTypeComponentIndex_t * dstci);


LIBSBML_EXTERN
DenotedSpeciesTypeComponentIndex_t *
ListOfDenotedSpeciesTypeComponentIndexes_getById(ListOf_t * lo, const char * sid);


LIBSBML_EXTERN
DenotedSpeciesTypeComponentIndex_t *
ListOfDenotedSpeciesTypeComponentIndexes_removeById(ListOf_t * lo, const char * sid);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  DenotedSpeciesTypeComponentIndex_H__  */


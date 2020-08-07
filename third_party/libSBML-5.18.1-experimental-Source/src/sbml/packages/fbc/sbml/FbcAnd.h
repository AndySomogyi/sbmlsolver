/**
 * @file   FbcAnd.h
 * @brief  Implementation of the FbcAnd class
 * @author SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
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
 * @class FbcAnd
 * @sbmlbrief{fbc} An &ldquo;and&rdquo; relationship for gene products
 *
 * FbcAnd class is used in Version 2 of the SBML Level&nbsp;3 @ref fbc
 * (&ldquo;fbc&rdquo;) package to represent an "and" relationship between two
 * or more child FbcAssociation objects.  In other words, it indicates that
 * all of the child objects are included.  Note that since the FbcAssociation
 * class is the parent class of GeneProductRef, FbcAnd and FbcOr, a given
 * FbcAnd can potentially include nested "and"/"or" combinations of gene
 * products.
 *
 * @copydetails doc_note_fbcv2_annotation_replacement
 *
 * @see FbcAssociation
 * @see FbcOr
 * @see GeneProductRef
 */


#ifndef FbcAnd_H__
#define FbcAnd_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/fbc/common/fbcfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/fbc/extension/FbcExtension.h>
#include <sbml/packages/fbc/sbml/FbcAssociation.h>

#include <sbml/packages/fbc/sbml/FbcAssociation.h>

LIBSBML_CPP_NAMESPACE_BEGIN



class LIBSBML_EXTERN FbcAnd : public FbcAssociation
{

protected:

  /** @cond doxygenLibsbmlInternal */
  ListOfFbcAssociations   mAssociations;
  /** @endcond */


public:

  /**
   * Creates a new FbcAnd with the given SBML Level, Version, and
   * &ldquo;fbc&rdquo;package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this FbcAnd.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * FbcAnd.
   *
   * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to
   * this FbcAnd.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  FbcAnd(unsigned int level      = FbcExtension::getDefaultLevel(),
         unsigned int version    = FbcExtension::getDefaultVersion(),
         unsigned int pkgVersion = FbcExtension::getDefaultPackageVersion());


  /**
   * Creates a new FbcAnd with the given FbcPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param fbcns the FbcPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  FbcAnd(FbcPkgNamespaces* fbcns);


   /**
   * Copy constructor for FbcAnd.
   *
   * @param orig the FbcAnd instance to copy.
   */
  FbcAnd(const FbcAnd& orig);


   /**
   * Assignment operator for FbcAnd.
   *
   * @param rhs the object whose values are used as the basis
   * of the assignment.
   */
  FbcAnd& operator=(const FbcAnd& rhs);


   /**
   * Creates and returns a deep copy of this FbcAnd object.
   *
   * @return a (deep) copy of this FbcAnd object.
   */
  virtual FbcAnd* clone () const;


   /**
   * Destructor for FbcAnd.
   */
  virtual ~FbcAnd();


   /**
   * Returns the ListOfFbcAssociations in this FbcAnd object.
   *
   * @return the ListOfFbcAssociations child of this FbcAnd.
   */
  const ListOfFbcAssociations* getListOfAssociations() const;


  /**
   * Returns the ListOfFbcAssociations in this FbcAnd object.
   *
   * @return the ListOfFbcAssociations child of this FbcAnd.
   */
  ListOfFbcAssociations* getListOfAssociations();


  /**
   * Get the nth object from the ListOfFbcAssociations.
   *
   * @param n the index number of the FbcAssociation to get.
   *
   * @return the nth FbcAssociation in the ListOfFbcAssociations within this
   * FbcAnd.
   * If the index @p n is invalid, @c NULL is returned.
   *
   * @see getNumAssociations()
   */
  FbcAssociation* getAssociation(unsigned int n);


  /**
   * Get the nth object from the ListOfFbcAssociations.
   *
   * @param n the index number of the FbcAssociation to get.
   *
   * @return the nth FbcAssociation in the ListOfFbcAssociations within this
   * FbcAnd.
   * If the index @p n is invalid, @c NULL is returned.
   *
   * @see getNumAssociations()
   */
  const FbcAssociation* getAssociation(unsigned int n) const;


  /**
   * Get an FbcAssociation from the ListOfFbcAssociations
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the FbcAssociation to get.
   *
   * @return the FbcAssociation in the ListOfFbcAssociations
   * with the given id or @c NULL if no such
   * FbcAssociation exists.
   *
   * @see getAssociation(unsigned int n)
   *
   * @see getNumAssociations()
   */
  FbcAssociation* getAssociation(const std::string& sid);


  /**
   * Get an FbcAssociation from the ListOfFbcAssociations
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the FbcAssociation to get.
   *
   * @return the FbcAssociation in the ListOfFbcAssociations
   * with the given id or @c NULL if no such
   * FbcAssociation exists.
   *
   * @see getAssociation(unsigned int n)
   *
   * @see getNumAssociations()
   */
  const FbcAssociation* getAssociation(const std::string& sid) const;


  /**
   * Adds a copy the given FbcAssociation to this FbcAnd.
   *
   * @param fa the FbcAssociation object to add.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_NAMESPACES_MISMATCH, OperationReturnValues_t}
   */
  int addAssociation(const FbcAssociation* fa);


  /**
   * Get the number of FbcAssociation objects in this FbcAnd.
   *
   * @return the number of FbcAssociation objects in this FbcAnd.
   */
  unsigned int getNumAssociations() const;


  /**
  * Converts this FbcAssociation object into an infix string representation.
  *
  * @return the association as infix string.
  */
  virtual std::string toInfix(bool usingId=false) const;


  /**
   * Creates a new FbcAnd object, adds it to this FbcAnd's
   * ListOfFbcAssociations and returns the FbcAnd object created. 
   *
   * @return a new FbcAnd object instance.
   *
   * @see addAssociation(const FbcAssociation* fa)
   */
  FbcAnd* createAnd();


  /**
   * Creates a new FbcOr object, adds it to this FbcAnd's
   * ListOfFbcAssociations and returns the FbcOr object created. 
   *
   * @return a new FbcOr object instance.
   *
   * @see addAssociation(const FbcAssociation* fa)
   */
  FbcOr* createOr();


  /**
   * Creates a new GeneProductRef object, adds it to this FbcAnd's
   * ListOfFbcAssociations and returns the GeneProductRef object created. 
   *
   * @return a new GeneProductRef object instance.
   *
   * @see addAssociation(const FbcAssociation* fa)
   */
  GeneProductRef* createGeneProductRef();


  /**
   * Removes the nth FbcAssociation from the ListOfFbcAssociations within this 
   * FbcAnd and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the FbcAssociation to remove.
   *
   * @see getNumAssociations()
   */
  FbcAssociation* removeAssociation(unsigned int n);


  /**
   * Removes the FbcAssociation with the given identifier from the 
   * ListOfFbcAssociations within this FbcAnd and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the FbcAssociation to remove.
   *
   * @return the FbcAssociation removed. As mentioned above, the caller owns the
   * returned item.
   */
  FbcAssociation* removeAssociation(const std::string& sid);


  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth.
   *
   * @return a List of pointers to all child objects.
   */
   virtual List* getAllElements(ElementFilter * filter = NULL);


  /**
   * Returns the XML element name of this object.
   *
   * For FbcAnd, the XML element name is always @c "fbcAnd".
   *
   * @return the name of this element, i.e. @c "fbcAnd".
   */
  virtual const std::string& getElementName () const;


  /**
   * Returns the libSBML type code for this SBML object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_FBC_AND, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Predicate returning @c true if all the required attributes
   * for this FbcAnd object have been set.  Will always return
   * @c true, since FbcOr has no required attributes.
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this FbcAnd object have been set.
   *
   * @note An FbcAnd object must have at least two FbcAssociation
   * children.
   *
   * @return a boolean value indicating whether all the required
   * elements for this object have been defined.
   */
  virtual bool hasRequiredElements() const;


  /** @cond doxygenLibsbmlInternal */

  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parent's
   * implementation of this method as well.
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor.
   */
  virtual bool accept (SBMLVisitor& v) const;


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Connects to child elements.
   */
  virtual void connectToChild ();


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Enables/Disables the given package with this element.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
               const std::string& pkgPrefix, bool flag);


  /** @endcond */



  #ifndef SWIG



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName, bool& value)
    const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName, int& value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           double& value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           unsigned int& value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           std::string& value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Predicate returning @c true if this FbcAnd's attribute "attributeName" is
   * set.
   *
   * @param attributeName, the name of the attribute to query.
   *
   * @return @c true if this FbcAnd's attribute "attributeName" has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetAttribute(const std::string& attributeName) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, bool value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, int value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, double value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName,
                           unsigned int value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName,
                           const std::string& value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Unsets the value of the "attributeName" attribute of this FbcAnd.
   *
   * @param attributeName, the name of the attribute to query.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetAttribute(const std::string& attributeName);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates and returns an new "elementName" object in this FbcAnd.
   *
   * @param elementName, the name of the element to create.
   *
   * @return pointer to the element created.
   */
  virtual SBase* createChildObject(const std::string& elementName);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the number of "elementName" in this FbcAnd.
   *
   * @param elementName, the name of the element to get number of.
   *
   * @return unsigned int number of elements.
   */
  virtual unsigned int getNumObjects(const std::string& elementName);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the nth object of "objectName" in this FbcAnd.
   *
   * @param elementName, the name of the element to get number of.
   *
   * @param index, unsigned int the index of the object to retrieve.
   *
   * @return pointer to the object.
   */
  virtual SBase* getObject(const std::string& elementName, unsigned int index);

  /** @endcond */




  #endif /* !SWIG */

protected:

  /** @cond doxygenLibsbmlInternal */

  /**
   * return the SBML object corresponding to next XMLToken.
   */
  virtual SBase* createObject(XMLInputStream& stream);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Read values from the given XMLAttributes set into their specific fields.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);


  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Write values of XMLAttributes to the output stream.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;


  /** @endcond */



};



LIBSBML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * Creates a new FbcAnd_t using the given SBML Level, Version and
 * &ldquo;fbc&rdquo; package version.
 *
 * @param level an unsigned int, the SBML Level to assign to this FbcAnd_t.
 *
 * @param version an unsigned int, the SBML Version to assign to this FbcAnd_t.
 *
 * @param pkgVersion an unsigned int, the SBML Fbc Version to assign to this
 * FbcAnd_t.
 *
 * @copydetails doc_note_setting_lv_pkg
 *
 * @copydetails doc_warning_returns_owned_pointer
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
FbcAnd_t *
FbcAnd_create(unsigned int level, unsigned int version,
              unsigned int pkgVersion);


/**
 * Frees this FbcAnd_t object.
 *
 * @param fa the FbcAnd_t structure.
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
void
FbcAnd_free(FbcAnd_t* fa);


/**
 * Creates and returns a deep copy of this FbcAnd_t object.
 *
 * @param fa the FbcAnd_t structure.
 *
 * @return a (deep) copy of this FbcAnd_t object.
 * pointer if a failure occurred.
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
FbcAnd_t*
FbcAnd_clone(const FbcAnd_t* fa);


/**
 * Adds a copy of the given FbcAssociation_t to this FbcAnd_t.
 *
 * @param fa the FbcAnd_t structure to which the FbcAssociation_t should be added.
 *
 * @param a the FbcAssociation_t object to add.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_PKG_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
int
FbcAnd_addAssociation(FbcAnd_t* fa, const FbcAssociation_t* a);


/**
 * Creates a new FbcAnd_t object, adds it to this FbcAnd_t object and returns
 * the FbcAnd_t object created.
 *
 * @param fa the FbcAnd_t structure to which the FbcAnd_t should be added.
 *
 * @return a new FbcAnd_t object instance.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
FbcAnd_t*
FbcAnd_createAnd(FbcAnd_t* fa);


/**
 * Creates a new FbcOr_t object, adds it to this FbcAnd_t object and returns
 * the FbcOr_t object created.
 *
 * @param fa the FbcAnd_t structure to which the FbcOr_t should be added.
 *
 * @return a new FbcOr_t object instance.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
FbcOr_t*
FbcAnd_createOr(FbcAnd_t* fa);


/**
 * Creates a new GeneProductRef_t object, adds it to this FbcAnd_t object and
 * returns the GeneProductRef_t object created.
 *
 * @param fa the FbcAnd_t structure to which the GeneProductRef_t should be
 * added.
 *
 * @return a new GeneProductRef_t object instance.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
GeneProductRef_t*
FbcAnd_createGeneProductRef(FbcAnd_t* fa);


/**
 * Returns a ListOf_t * containing FbcAssociation_t objects from this FbcAnd_t.
 *
 * @param fa the FbcAnd_t structure whose ListOfAssociations is sought.
 *
 * @return the ListOfAssociations from this FbcAnd_t as a ListOf_t *.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @see FbcAnd_addAssociation()
 * @see FbcAnd_getAssociationById()
 * @see FbcAnd_getAssociation()
 * @see FbcAnd_getNumAssociations()
 * @see FbcAnd_removeAssociationById()
 * @see FbcAnd_removeAssociation()
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
ListOf_t*
FbcAnd_getListOfFbcAssociations(FbcAnd_t* fa);


/**
 * Get an FbcAssociation_t from the FbcAnd_t.
 *
 * @param fa the FbcAnd_t structure to search.
 *
 * @param n an unsigned int representing the index of the FbcAssociation_t to
 * retrieve.
 *
 * @return the nth FbcAssociation_t in the ListOfAssociations within this FbcAnd.
 * If the index @p n is invalid, @c NULL is returned.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
FbcAssociation_t*
FbcAnd_getAssociation(FbcAnd_t* fa, unsigned int n);


/**
 * Get an FbcAssociation_t from the FbcAnd_t.
 *
 * @param fa the FbcAnd_t structure to search.
 *
 * @param sid the identifier of the FbcAssociation_t to
 * retrieve.
 *
 * @return the FbcAssociation_t with the given @p sid in the ListOfAssociations within this FbcAnd.
 *
 * @copydetails doc_returned_unowned_pointer
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
FbcAssociation_t *
FbcAnd_getAssociationById(FbcAnd_t * fa, const char * sid);


/**
 * Get the number of FbcAssociation_t objects in this FbcAnd_t.
 *
 * @param fa the FbcAnd_t structure to query.
 *
 * @return the number of FbcAssociation_t objects in this FbcAnd_t.
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
unsigned int
FbcAnd_getNumAssociations(FbcAnd_t* fa);


/**
 * Removes the nth FbcAssociation_t from this FbcAnd_t and returns a pointer to
 * it.
 *
 * @param fa the FbcAnd_t structure to search.
 *
 * @param n an unsigned int representing the index of the FbcAssociation_t to
 * remove.
 *
 * @return a pointer to the nth FbcAssociation_t in this FbcAnd_t.
 *
 * @copydetails doc_warning_returns_owned_pointer
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
FbcAssociation_t*
FbcAnd_removeAssociation(FbcAnd_t* fa, unsigned int n);


/**
 * Removes the FbcAssociation_t with the given identifier from this FbcAnd_t and returns a pointer to
 * it.
 *
 * @param fa the FbcAnd_t structure to search.
 *
 * @param sid the identifier of the FbcAssociation_t to
 * remove.
 *
 * @return a pointer to the nth FbcAssociation_t in this FbcAnd_t.
 *
 * @copydetails doc_warning_returns_owned_pointer
 *
 */
LIBSBML_EXTERN
FbcAssociation_t *
FbcAnd_removeAssociationById(FbcAnd_t * fa, const char * sid);


/**
 * Predicate returning @c 1 (true) if all the required attributes for this
 * FbcAnd_t object have been set.
 *
 * @param fa the FbcAnd_t structure.
 *
 * @return @c 1 (true) to indicate that all the required attributes of this
 * FbcAnd_t have been set, otherwise @c 0 (false) is returned.
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
int
FbcAnd_hasRequiredAttributes(const FbcAnd_t * fa);


/**
 * Predicate returning @c 1 (true) if all the required elements for this
 * FbcAnd_t object have been set.
 *
 * @param fa the FbcAnd_t structure.
 *
 * @return @c 1 (true) to indicate that all the required elements of this
 * FbcAnd_t have been set, otherwise @c 0 (false) is returned.
 *
 *
 * @note The required elements for the FbcAnd_t object are:
 *
 * @memberof FbcAnd_t
 */
LIBSBML_EXTERN
int
FbcAnd_hasRequiredElements(const FbcAnd_t * fa);




END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  FbcAnd_H__  */


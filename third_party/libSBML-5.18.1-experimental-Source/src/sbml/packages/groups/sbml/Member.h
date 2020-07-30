/**
 * @file Member.h
 * @brief Definition of the Member class.
 * @author SBMLTeam
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML. Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 * 3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 * Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 * 1. California Institute of Technology, Pasadena, CA, USA
 * 2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation. A copy of the license agreement is provided in the
 * file named "LICENSE.txt" included with this software distribution and also
 * available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class Member
 * @sbmlbrief{groups} The Member class defines what objects are parts of
 * a Group.
 *
 * The Member class has four optional attributes: id and name, which identify 
 * the element, and idRef and metaIdRef which reference the identifiers of 
 * other elements. There must be exactly one (and only one) method used to 
 * reference another element: either idRef or metaIdRef may be defined, but 
 * not both. (Multiple attributes are needed to account for the different types 
 * of identifiers that a given object may have.) The referenced object 
 * (including, potentially, another Group object) is thus made a member of the 
 * group in which the Member object is contained.
 * 
 * Since Member is derived from SBase and, as mentioned above, SBase provides 
 * both the ability to attach SBO terms as well as MIRIAM annotations, the 
 * semantics of a given member in a model can be made more precise by reference 
 * to external controlled vocabularies and ontologies.
 */


#ifndef Member_H__
#define Member_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/groups/common/groupsfwd.h>


#ifdef __cplusplus


#include <string>


#include <sbml/SBase.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Member : public SBase
{
protected:

  /** @cond doxygenLibsbmlInternal */

  std::string mIdRef;
  std::string mMetaIdRef;

  /** @endcond */

public:

  /**
   * Creates a new Member using the given SBML Level, Version and
   * &ldquo;groups&rdquo; package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this Member.
   *
   * @param version an unsigned int, the SBML Version to assign to this Member.
   *
   * @param pkgVersion an unsigned int, the SBML Groups Version to assign to
   * this Member.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Member(unsigned int level = GroupsExtension::getDefaultLevel(),
         unsigned int version = GroupsExtension::getDefaultVersion(),
         unsigned int pkgVersion =
           GroupsExtension::getDefaultPackageVersion());


  /**
   * Creates a new Member using the given GroupsPkgNamespaces object.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param groupsns the GroupsPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  Member(GroupsPkgNamespaces *groupsns);


  /**
   * Copy constructor for Member.
   *
   * @param orig the Member instance to copy.
   */
  Member(const Member& orig);


  /**
   * Assignment operator for Member.
   *
   * @param rhs the Member object whose values are to be used as the basis of
   * the assignment.
   */
  Member& operator=(const Member& rhs);


  /**
   * Creates and returns a deep copy of this Member object.
   *
   * @return a (deep) copy of this Member object.
   */
  virtual Member* clone() const;


  /**
   * Destructor for Member.
   */
  virtual ~Member();


  /**
   * Returns the value of the "id" attribute of this Member.
   *
   * @return the value of the "id" attribute of this Member as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Returns the value of the "name" attribute of this Member.
   *
   * @return the value of the "name" attribute of this Member as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Returns the value of the "idRef" attribute of this Member.
   *
   * @return the value of the "idRef" attribute of this Member as a string.
   */
  const std::string& getIdRef() const;


  /**
   * Returns the value of the "metaIdRef" attribute of this Member.
   *
   * @return the value of the "metaIdRef" attribute of this Member as a string.
   */
  const std::string& getMetaIdRef() const;


  /**
   * Predicate returning @c true if this Member's "id" attribute is set.
   *
   * @return @c true if this Member's "id" attribute has been set, otherwise
   * @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Predicate returning @c true if this Member's "name" attribute is set.
   *
   * @return @c true if this Member's "name" attribute has been set, otherwise
   * @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Predicate returning @c true if this Member's "idRef" attribute is set.
   *
   * @return @c true if this Member's "idRef" attribute has been set, otherwise
   * @c false is returned.
   */
  bool isSetIdRef() const;


  /**
   * Predicate returning @c true if this Member's "metaIdRef" attribute is set.
   *
   * @return @c true if this Member's "metaIdRef" attribute has been set,
   * otherwise @c false is returned.
   */
  bool isSetMetaIdRef() const;


  /**
   * Sets the value of the "id" attribute of this Member.
   *
   * @param id std::string& value of the "id" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE,
   * OperationReturnValues_t}
   *
   * Calling this function with @p id = @c NULL or an empty string is
   * equivalent to calling unsetId().
   */
  virtual int setId(const std::string& id);


  /**
   * Sets the value of the "name" attribute of this Member.
   *
   * @param name std::string& value of the "name" attribute to be set.
   *
   * @copydetails doc_returns_one_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   *
   * Calling this function with @p name = @c NULL or an empty string is
   * equivalent to calling unsetName().
   */
  virtual int setName(const std::string& name);


  /**
   * Sets the value of the "idRef" attribute of this Member.
   *
   * @param idRef std::string& value of the "idRef" attribute to be set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE,
   * OperationReturnValues_t}
   */
  int setIdRef(const std::string& idRef);


  /**
   * Sets the value of the "metaIdRef" attribute of this Member.
   *
   * @param metaIdRef std::string& value of the "metaIdRef" attribute to be
   * set.
   *
   * @copydetails doc_returns_one_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  int setMetaIdRef(const std::string& metaIdRef);


  /**
   * Unsets the value of the "id" attribute of this Member.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetId();


  /**
   * Unsets the value of the "name" attribute of this Member.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetName();


  /**
   * Unsets the value of the "idRef" attribute of this Member.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetIdRef();


  /**
   * Unsets the value of the "metaIdRef" attribute of this Member.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetMetaIdRef();


  /**
   * @copydoc doc_renamesidref_common
   */
  virtual void renameSIdRefs(const std::string& oldid,
                             const std::string& newid);


  /**
   * Returns the XML element name of this Member object.
   *
   * For Member, the XML element name is always @c "member".
   *
   * @return the name of this element, i.e. @c "member".
   */
  virtual const std::string& getElementName() const;


  /**
   * Returns the libSBML type code for this Member object.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_GROUPS_MEMBER, SBMLGroupsTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode() const;


  /**
   * Predicate returning @c true if all the required attributes for this Member
   * object have been set.
   *
   * @return @c true to indicate that all the required attributes of this
   * Member have been set, otherwise @c false is returned.
   */
  virtual bool hasRequiredAttributes() const;



  /** @cond doxygenLibsbmlInternal */

  /**
   * Write any contained elements
   */
  virtual void writeElements(XMLOutputStream& stream) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Accepts the given SBMLVisitor
   */
  virtual bool accept(SBMLVisitor& v) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the parent SBMLDocument
   */
  virtual void setSBMLDocument(SBMLDocument* d);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Enables/disables the given package with this element
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix,
                                     bool flag);

  /** @endcond */




  #ifndef SWIG



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the value of the "attributeName" attribute of this Member.
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
   * Returns the value of the "attributeName" attribute of this Member.
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
   * Returns the value of the "attributeName" attribute of this Member.
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
   * Returns the value of the "attributeName" attribute of this Member.
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
   * Returns the value of the "attributeName" attribute of this Member.
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
   * Predicate returning @c true if this Member's attribute "attributeName" is
   * set.
   *
   * @param attributeName, the name of the attribute to query.
   *
   * @return @c true if this Member's attribute "attributeName" has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetAttribute(const std::string& attributeName) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this Member.
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
   * Sets the value of the "attributeName" attribute of this Member.
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
   * Sets the value of the "attributeName" attribute of this Member.
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
   * Sets the value of the "attributeName" attribute of this Member.
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
   * Sets the value of the "attributeName" attribute of this Member.
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
   * Unsets the value of the "attributeName" attribute of this Member.
   *
   * @param attributeName, the name of the attribute to query.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetAttribute(const std::string& attributeName);

  /** @endcond */




  #endif /* !SWIG */

  /** @cond doxygenLibsbmlInternal */

  /**
   * Enables/disables the given package with this element
   */
  SBase* getReferencedElement();

  /** @endcond */



protected:


  /** @cond doxygenLibsbmlInternal */

  /**
   * Adds the expected attributes for this element
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Reads the expected attributes into the member data variables
   */
  virtual void readAttributes(const XMLAttributes& attributes,
                              const ExpectedAttributes& expectedAttributes);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Writes the attributes to the stream
   */
  virtual void writeAttributes(XMLOutputStream& stream) const;

  /** @endcond */


};



LIBSBML_CPP_NAMESPACE_END




#endif /* __cplusplus */




#ifndef SWIG




LIBSBML_CPP_NAMESPACE_BEGIN




BEGIN_C_DECLS


/**
 * Creates a new Member_t using the given SBML Level, Version and
 * &ldquo;groups&rdquo; package version.
 *
 * @param level an unsigned int, the SBML Level to assign to this Member_t.
 *
 * @param version an unsigned int, the SBML Version to assign to this Member_t.
 *
 * @param pkgVersion an unsigned int, the SBML Groups Version to assign to this
 * Member_t.
 *
 * @copydetails doc_note_setting_lv_pkg
 *
 * @copydetails doc_warning_returns_owned_pointer
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
Member_t *
Member_create(unsigned int level,
              unsigned int version,
              unsigned int pkgVersion);


/**
 * Creates and returns a deep copy of this Member_t object.
 *
 * @param m the Member_t structure.
 *
 * @return a (deep) copy of this Member_t object.
 *
 * @copydetails doc_warning_returns_owned_pointer
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
Member_t*
Member_clone(const Member_t* m);


/**
 * Frees this Member_t object.
 *
 * @param m the Member_t structure.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
void
Member_free(Member_t* m);


/**
 * Returns the value of the "id" attribute of this Member_t.
 *
 * @param m the Member_t structure whose id is sought.
 *
 * @return the value of the "id" attribute of this Member_t as a pointer to a
 * string.
 *
 * @copydetails doc_warning_returns_owned_char
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
char *
Member_getId(const Member_t * m);


/**
 * Returns the value of the "name" attribute of this Member_t.
 *
 * @param m the Member_t structure whose name is sought.
 *
 * @return the value of the "name" attribute of this Member_t as a pointer to a
 * string.
 *
 * @copydetails doc_warning_returns_owned_char
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
char *
Member_getName(const Member_t * m);


/**
 * Returns the value of the "idRef" attribute of this Member_t.
 *
 * @param m the Member_t structure whose idRef is sought.
 *
 * @return the value of the "idRef" attribute of this Member_t as a pointer to
 * a string.
 *
 * @copydetails doc_warning_returns_owned_char
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
char *
Member_getIdRef(const Member_t * m);


/**
 * Returns the value of the "metaIdRef" attribute of this Member_t.
 *
 * @param m the Member_t structure whose metaIdRef is sought.
 *
 * @return the value of the "metaIdRef" attribute of this Member_t as a pointer
 * to a string.
 *
 * @copydetails doc_warning_returns_owned_char
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
char *
Member_getMetaIdRef(const Member_t * m);


/**
 * Predicate returning @c 1 (true) if this Member_t's "id" attribute is set.
 *
 * @param m the Member_t structure.
 *
 * @return @c 1 (true) if this Member_t's "id" attribute has been set,
 * otherwise @c 0 (false) is returned.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_isSetId(const Member_t * m);


/**
 * Predicate returning @c 1 (true) if this Member_t's "name" attribute is set.
 *
 * @param m the Member_t structure.
 *
 * @return @c 1 (true) if this Member_t's "name" attribute has been set,
 * otherwise @c 0 (false) is returned.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_isSetName(const Member_t * m);


/**
 * Predicate returning @c 1 (true) if this Member_t's "idRef" attribute is set.
 *
 * @param m the Member_t structure.
 *
 * @return @c 1 (true) if this Member_t's "idRef" attribute has been set,
 * otherwise @c 0 (false) is returned.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_isSetIdRef(const Member_t * m);


/**
 * Predicate returning @c 1 (true) if this Member_t's "metaIdRef" attribute is
 * set.
 *
 * @param m the Member_t structure.
 *
 * @return @c 1 (true) if this Member_t's "metaIdRef" attribute has been set,
 * otherwise @c 0 (false) is returned.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_isSetMetaIdRef(const Member_t * m);


/**
 * Sets the value of the "id" attribute of this Member_t.
 *
 * @param m the Member_t structure.
 *
 * @param id const char * value of the "id" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * Calling this function with @p id = @c NULL or an empty string is equivalent
 * to calling Member_unsetId().
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_setId(Member_t * m, const char * id);


/**
 * Sets the value of the "name" attribute of this Member_t.
 *
 * @param m the Member_t structure.
 *
 * @param name const char * value of the "name" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * Calling this function with @p name = @c NULL or an empty string is
 * equivalent to calling Member_unsetName().
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_setName(Member_t * m, const char * name);


/**
 * Sets the value of the "idRef" attribute of this Member_t.
 *
 * @param m the Member_t structure.
 *
 * @param idRef const char * value of the "idRef" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_setIdRef(Member_t * m, const char * idRef);


/**
 * Sets the value of the "metaIdRef" attribute of this Member_t.
 *
 * @param m the Member_t structure.
 *
 * @param metaIdRef const char * value of the "metaIdRef" attribute to be set.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_setMetaIdRef(Member_t * m, const char * metaIdRef);


/**
 * Unsets the value of the "id" attribute of this Member_t.
 *
 * @param m the Member_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_unsetId(Member_t * m);


/**
 * Unsets the value of the "name" attribute of this Member_t.
 *
 * @param m the Member_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_unsetName(Member_t * m);


/**
 * Unsets the value of the "idRef" attribute of this Member_t.
 *
 * @param m the Member_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_unsetIdRef(Member_t * m);


/**
 * Unsets the value of the "metaIdRef" attribute of this Member_t.
 *
 * @param m the Member_t structure.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_unsetMetaIdRef(Member_t * m);


/**
 * Predicate returning @c 1 (true) if all the required attributes for this
 * Member_t object have been set.
 *
 * @param m the Member_t structure.
 *
 * @return @c 1 (true) to indicate that all the required attributes of this
 * Member_t have been set, otherwise @c 0 (false) is returned.
 *
 * @memberof Member_t
 */
LIBSBML_EXTERN
int
Member_hasRequiredAttributes(const Member_t * m);




END_C_DECLS




LIBSBML_CPP_NAMESPACE_END




#endif /* !SWIG */




#endif /* !Member_H__ */



/**
 * @file    Group.h
 * @brief   Definition of Group, the SBase derived class of groups package.
 * @author  Akiya Jouraku
 *
 * $Id: Group.h 13541 2011-04-08 22:16:45Z fbergmann $
 * $HeadURL: https://sbml.svn.sourceforge.net/svnroot/sbml/branches/libsbml-5/src/sbml/packages/groups/sbml/Group.h $
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


#ifndef Group_H__
#define Group_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/groups/common/groupsfwd.h>
LIBSBML_CPP_NAMESPACE_BEGIN

typedef enum
{
    GROUP_KIND_CLASSIFICATION
  , GROUP_KIND_PARTONOMY
  , GROUP_KIND_COLLECTION
  , GROUP_KIND_UNKNOWN
} GroupKind_t;

LIBSBML_CPP_NAMESPACE_END

#ifdef __cplusplus

#include <string>

#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>
#include <sbml/packages/groups/sbml/Member.h>
#include <sbml/packages/groups/sbml/MemberConstraint.h>

LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN Group : public SBase
{
protected:

  std::string   mId;
  std::string   mName;
  GroupKind_t   mKind;
  ListOfMembers mMembers;
  ListOfMemberConstraints   mMemberConstraints;

public:

  /**
   * Creates a new Group with the given level, version, and package version.
   *
   * @param level an unsigned int, the SBML Level to assign to this Group
   *
   * @param version an unsigned int, the SBML Version to assign to this Group
   *
   * @param pkgVersion an unsigned int, the SBML Groups Version to assign to this Group
   */
  Group(unsigned int level      = GroupsExtension::getDefaultLevel(),
        unsigned int version    = GroupsExtension::getDefaultVersion(),
        unsigned int pkgVersion = GroupsExtension::getDefaultPackageVersion());


  /**
   * Creates a new Group with the given GroupsPkgNamespaces object.
   *
   * @param groupsns the GroupsPkgNamespaces object
   */
  Group(GroupsPkgNamespaces* groupsns);


  /**
   * Copy constructor for Group.
   *
   * @param orig; the Group instance to copy.
   */
  Group(const Group& orig);


  /**
   * Assignment operator for Group.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  Group& operator=(const Group& rhs);


  /**
   * Creates and returns a deep copy of this Group object.
   *
   * @return a (deep) copy of this Group object.
   */
  virtual Group* clone () const;


  /**
   * Destructor for Group.
   */
  virtual ~Group();


  /**
   * Returns the value of the "id" attribute of this Group.
   *
   * @return the value of the "id" attribute of this Group as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Group's "id" attribute has been set.
   *
   * @return @c true if this Group's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this Group.
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
   * Unsets the value of the "id" attribute of this Group.
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
   * Returns the value of the "name" attribute of this Group.
   *
   * @return the value of the "name" attribute of this Group as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Group's "name" attribute has been set.
   *
   * @htmlinclude comment-set-methods.html
   * 
   * @return @c true if this Group's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;

  /**
   * Sets the value of the "name" attribute of this Group.
   *
   * The string in @p name is copied.
   *
   * @htmlinclude comment-set-methods.html
   *
   * @param name; const std::string& value of the "name" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setName(const std::string& name);


  /**
   * Unsets the value of the "name" attribute of this Group.
   *
   * @htmlinclude comment-set-methods.html
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetName();


  /**
   * Returns the value of the "kind" attribute of this Group.
   * 
   * @return the kind of this Group.
   */
  GroupKind_t getKind () const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * Group's "kind" attribute has been set.
   *
   * @htmlinclude comment-set-methods.html
   * 
   * @return @c true if this Group's "kind" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetKind() const;


  /**
   * Sets the value of the "kind" attribute of this Group.
   *
   * The string in @p kind is copied.
   *
   * @htmlinclude comment-set-methods.html
   *
   * @param kind the new kind for the Group
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  int setKind (const GroupKind_t kind);


  /**
   * Unsets the value of the "kind" attribute of this Group.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_OPERATION_FAILED
   */
  virtual int unsetKind();


  /**
   * Returns the ListOf object that holds all members.
   *
   * @return the ListOf object that holds all members.
   */ 
  const ListOfMembers* getListOfMembers () const;

  /**
   * Returns the member with the given index.
   * If the index is invalid, NULL is returned.
   *
   * @param n the index number of the Member to get.
   *
   * @return the nth Member in the ListOfMembers.
   */ 
  Member* getMember (unsigned int n);

  /**
   * Returns the member with the given index.
   * If the index is invalid, NULL is returned.
   *
   * @param n the index number of the Member to get.
   *
   * @return the nth Member in the ListOfMembers.
   */ 
  const Member* getMember (unsigned int n) const;

  /**
   * Returns the member with the given symbol.
   * If the index is invalid, NULL is returned.
   *
   * @param symbol a string representing the symbol attribute
   * of the Member to get.
   * 
   * @return Member in the ListOfMembers with the given symbol
   * or NULL if no such Member exists.
   */
  Member* getMember (const std::string& symbol);


  /**
   * Returns the member with the given symbol.
   * If the index is invalid, NULL is returned.
   *
   * @param symbol a string representing the symbol attribute
   * of the Member to get.
   * 
   * @return Member in the ListOfMembers with the given symbol
   * or NULL if no such Member exists.
   */
  const Member* getMember (const std::string& symbol) const;


  /**
   * Adds a copy of the given Member objcect to the list of members.
   *
   * @param member the Member object to be added to the list of 
   * members.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   */
  int addMember (const Member* member);


  /**
   * Returns the number of members for this group.
   *
   * @return the number of members for this group.
   */
  unsigned int getNumMembers () const;


  /**
   * Creates a Member object, adds it to the end of the
   * member objects list and returns a pointer to the newly
   * created object.
   *
   * @return a newly created Member object
   */
  Member* createMember ();


  /**
   * Removes the member with the given index from the group.
   * A pointer to the member that was removed is returned.
   * If no member has been removed, NULL is returned.
   *
   * @param n the index of the Member object to remove
   *
   * @return the Member object removed.  As mentioned above, 
   * the caller owns the returned object. NULL is returned if 
   * the given index is out of range.
   */
  Member* removeMember(unsigned int index);


  /**
   * Removes the member with the given symbol from the group.
   * A pointer to the member that was removed is returned.
   * If no member has been removed, NULL is returned.
   *
   * @param symbol the symbol attribute of the Member object to remove
   *
   * @return the Member object removed.  As mentioned above, 
   * the caller owns the returned object. NULL is returned if 
   * the given index is out of range.
   */
  Member* removeMember(const std::string& symbol);


  /**
   * Returns the  "ListOfMemberConstraints" in this Group object.
   *
   * @return the "ListOfMemberConstraints" attribute of this Group.
   */
  const ListOfMemberConstraints* getListOfMemberConstraints() const;


  /**
   * Returns the  "ListOfMemberConstraints" in this Group object.
   *
   * @return the "ListOfMemberConstraints" attribute of this Group.
   */
  ListOfMemberConstraints* getListOfMemberConstraints();


  /**
   * Get a MemberConstraint from the ListOfMemberConstraints.
   *
   * @param n the index number of the MemberConstraint to get.
   *
   * @return the nth MemberConstraint in the ListOfMemberConstraints within this Group.
   *
   * @see getNumMemberConstraints()
   */
  MemberConstraint* getMemberConstraint(unsigned int n);


  /**
   * Get a MemberConstraint from the ListOfMemberConstraints.
   *
   * @param n the index number of the MemberConstraint to get.
   *
   * @return the nth MemberConstraint in the ListOfMemberConstraints within this Group.
   *
   * @see getNumMemberConstraints()
   */
  const MemberConstraint* getMemberConstraint(unsigned int n) const;


  /**
   * Get a MemberConstraint from the ListOfMemberConstraints
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the MemberConstraint to get.
   *
   * @return the MemberConstraint in the ListOfMemberConstraints
   * with the given id or NULL if no such
   * MemberConstraint exists.
   *
   * @see getMemberConstraint(unsigned int n)
   *
   * @see getNumMemberConstraints()
   */
  MemberConstraint* getMemberConstraint(const std::string& sid);


  /**
   * Get a MemberConstraint from the ListOfMemberConstraints
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the MemberConstraint to get.
   *
   * @return the MemberConstraint in the ListOfMemberConstraints
   * with the given id or NULL if no such
   * MemberConstraint exists.
   *
   * @see getMemberConstraint(unsigned int n)
   *
   * @see getNumMemberConstraints()
   */
  const MemberConstraint* getMemberConstraint(const std::string& sid) const;


  /**
   * Adds a copy the given "MemberConstraint" to this Group.
   *
   * @param mc; the MemberConstraint object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSBML_OPERATION_SUCCESS
   * @li LIBSBML_INVALID_ATTRIBUTE_VALUE
   */
  int addMemberConstraint(const MemberConstraint* mc);


  /**
   * Get the number of MemberConstraint objects in this Group.
   *
   * @return the number of MemberConstraint objects in this Group
   */
  unsigned int getNumMemberConstraints() const;


  /**
   * Creates a new MemberConstraint object, adds it to this Groups
   * ListOfMemberConstraints and returns the MemberConstraint object created. 
   *
   * @return a new MemberConstraint object instance
   *
   * @see addMemberConstraint(const MemberConstraint* mc)
   */
  MemberConstraint* createMemberConstraint();


  /**
   * Removes the nth MemberConstraint from the ListOfMemberConstraints within this Group.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the MemberConstraint to remove.
   *
   * @see getNumMemberConstraints()
   */
  MemberConstraint* removeMemberConstraint(unsigned int n);


  /**
   * Removes the MemberConstraint with the given identifier from the ListOfMemberConstraints within this Group
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the MemberConstraint to remove.
   *
   * @return the MemberConstraint removed. As mentioned above, the caller owns the
   * returned item.
   */
  MemberConstraint* removeMemberConstraint(const std::string& sid);



  /**
   * Subclasses should override this method to return XML element name of
   * this SBML object.
   *
   * @return the string of the name of this element.
   */
  virtual const std::string& getElementName () const ;


  /**
   * @return the typecode (int) of this SBML object or SBML_UNKNOWN
   * (default).
   *
   * @see getElementName()
   */
  int getTypeCode () const;


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.  For example:
   *
   *   SBase::writeElements(stream);
   *   mReactans.write(stream);
   *   mProducts.write(stream);
   *   ...
   */
  virtual void writeElements (XMLOutputStream& stream) const;


  /**
   * Accepts the given SBMLVisitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  virtual bool accept (SBMLVisitor& v) const;
  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this SBML object.
   *
   * @param d the SBMLDocument object to use
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /**
   * Sets this SBML object to child SBML objects (if any).
   * (Creates a child-parent relationship by the parent)
   *
   * Subclasses must override this function if they define
   * one ore more child elements.
   * Basically, this function needs to be called in
   * constructor, copy constructor, assignment operator.
   *
   * @see setSBMLDocument
   * @see enablePackageInternal
   */
  virtual void connectToChild ();


  /**
   * Enables/Disables the given package with this element and child
   * elements (if any).
   * (This is an internal implementation for enablePakcage function)
   *
   * @note Subclasses in which one or more child elements are defined
   * must override this function.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);
  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */
  /* function returns true if component has all the required
   * attributes
   * needs to be overloaded for each component
   */
  virtual bool hasRequiredAttributes() const ;
  /** @endcond doxygenLibsbmlInternal */



  /** @cond doxygenLibsbmlInternal */
  /* function returns true if component has all the required
   * elements
   * needs to be overloaded for each component
   */
  virtual bool hasRequiredElements() const ;
  /** @endcond doxygenLibsbmlInternal */

    
protected:
  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase*
  createObject (XMLInputStream& stream);

  /**
   * Subclasses should override this method to get the list of
   * expected attributes.
   * This function is invoked from corresponding readAttributes()
   * function.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parents implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes, 
                               const ExpectedAttributes& expectedAttributes);

  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parents implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "id"  , mId   );
   *   stream.writeAttribute( "name", mName );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;

};

class LIBSBML_EXTERN ListOfGroups : public ListOf
{
public:

  /**
   * @return a (deep) copy of this ListOfGroups.
   */
  virtual ListOfGroups* clone () const;


  /**
   * Creates a new ListOfGroups with the given level, version, and package version.
   */
   ListOfGroups(unsigned int level      = GroupsExtension::getDefaultLevel(), 
                unsigned int version    = GroupsExtension::getDefaultVersion(), 
                unsigned int pkgVersion = GroupsExtension::getDefaultPackageVersion());


  /**
   * Creates a new ListOfGroups with the given GroupsPkgNamespaces object.
   */
   ListOfGroups(GroupsPkgNamespaces* groupsns);


  /**
   * Get a Group from the ListOfGroups.
   *
   * @param n the index number of the Group to get.
   * 
   * @return the nth Group in this ListOfGroups.
   *
   * @see size()
   */
  virtual Group* get(unsigned int n); 


  /**
   * Get a Group from the ListOfGroups.
   *
   * @param n the index number of the Group to get.
   * 
   * @return the nth Group in this ListOfGroups.
   *
   * @see size()
   */
  virtual const Group * get(unsigned int n) const; 


  /**
   * Get a Group from the ListOfGroups
   * based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the Group to get.
   * 
   * @return Group in this ListOfGroups
   * with the given id or NULL if no such
   * Group exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual Group* get (const std::string& sid);


  /**
   * Get a Group from the ListOfGroups
   * based on its identifier.
   *
   * @param sid a string representing the identifier 
   * of the Group to get.
   * 
   * @return Group in this ListOfGroups
   * with the given id or NULL if no such
   * Group exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const Group* get (const std::string& sid) const;


  /**
   * Removes the nth item from this ListOfGroups items and returns a pointer to
   * it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual Group* remove (unsigned int n);


  /**
   * Removes item in this ListOfGroups items with the given identifier.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then @c
   * NULL is returned.
   *
   * @param sid the identifier of the item to remove
   *
   * @return the item removed.  As mentioned above, the caller owns the
   * returned item.
   */
  virtual Group* remove (const std::string& sid);


  /**
   * @return the typecode (int) of SBML objects contained in this ListOf or
   * SBML_UNKNOWN (default).
   */
  virtual int getItemTypeCode () const;

  /**
   * Subclasses should override this method to return XML element name of
   * this SBML object.
   *
   * @return the string of the name of this element.
   */
  virtual const std::string& getElementName () const;


protected:

  /**
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);

  virtual void writeXMLNS (XMLOutputStream& stream) const;
};


LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

//
// C API will be added here.
//

LIBSBML_EXTERN
const char* 
GroupKind_toString(GroupKind_t kind);


LIBSBML_EXTERN
GroupKind_t 
GroupKind_fromString(const char* s);


LIBSBML_EXTERN
int 
GroupKind_isValidGroupKind(GroupKind_t kind);


LIBSBML_EXTERN
int 
GroupKind_isValidGroupKindString(const char* s);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif  /* !SWIG */
#endif  /* Group_H__ */

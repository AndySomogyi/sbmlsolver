/**
 * @file    Group.cpp
 * @brief   Implementation of Group, the SBase derived class of groups package.
 * @author  Akiya Jouraku
 *
 * $Id: Group.cpp 13541 2011-04-08 22:16:45Z fbergmann $
 * $HeadURL: https://sbml.svn.sourceforge.net/svnroot/sbml/branches/libsbml-5/src/sbml/packages/groups/sbml/Group.cpp $
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


#include <sbml/packages/groups/sbml/Group.h>
#include <sbml/packages/groups/validator/GroupsSBMLError.h>


using namespace std;


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new Group with the given level, version, and package version.
 */
Group::Group (unsigned int level, unsigned int version, unsigned int pkgVersion) 
  : SBase (level,version)
   ,mId("")
   ,mName("")
   ,mKind(GROUP_KIND_UNKNOWN)
   ,mMembers(level,version,pkgVersion)
   ,mMemberConstraints (level, version, pkgVersion)
{
  // set an SBMLNamespaces derived object of this package
  setSBMLNamespacesAndOwn(new GroupsPkgNamespaces(level, version, pkgVersion));

  // connect to child objects
  connectToChild();
}


/*
 * Creates a new Group with the given GroupsPkgNamespaces object.
 */
Group::Group(GroupsPkgNamespaces* groupsns)
 : SBase(groupsns)
  ,mId("")
  ,mName("")
  ,mKind(GROUP_KIND_UNKNOWN)
  ,mMembers(groupsns)
   ,mMemberConstraints (groupsns)
{
  // set the element namespace of this object
  setElementNamespace(groupsns->getURI());

  // connect to child objects
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(groupsns);
}


/*
 * Copy constructor for Group.
 */
Group::Group (const Group& orig)
  : SBase(orig)
{
  if (&orig == NULL)
  {
    throw SBMLConstructorException("Null argument to copy constructor");
  }
  else
  {
    mId  = orig.mId;
    mName  = orig.mName;
    mKind  = orig.mKind;
    mMembers  = orig.mMembers;
    mMemberConstraints  = orig.mMemberConstraints;

    // connect to child objects
    connectToChild();
  }
}


/*
 * Assignment for Group.
 */
Group&
Group::operator=(const Group& rhs)
{
  if (&rhs == NULL)
  {
    throw SBMLConstructorException("Null argument to assignment");
  }
  else if (&rhs != this)
  {
    SBase::operator=(rhs);
    mId  = rhs.mId;
    mName  = rhs.mName;
    mKind  = rhs.mKind;
    mMembers  = rhs.mMembers;
    mMemberConstraints  = rhs.mMemberConstraints;

    // connect to child objects
    connectToChild();
  }
  return *this;
}


/*
 * Clone for Group.
 */
Group*
Group::clone () const
{
  return new Group(*this);
}


/*
 * Destructor for Group.
 */
Group::~Group ()
{
}


/*
  * Returns the value of the "id" attribute of this Group.
  */
const std::string& 
Group::getId () const
{
  return mId;
}


/*
  * Predicate returning @c true or @c false depending on whether this
  * Group's "id" attribute has been set.
  */
bool 
Group::isSetId () const
{
  return (mId.empty() == false);
}

/*
  * Sets the value of the "id" attribute of this Group.
  */
int 
Group::setId (const std::string& id)
{
  return SyntaxChecker::checkAndSetSId(id,mId);
}


/*
  * Unsets the value of the "id" attribute of this Group.
  */
int 
Group::unsetId ()
{
  mId.erase();
  if (mId.empty())
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Sets the name of this SBML object to a copy of name.
 */
int
Group::setName (const std::string& name)
{
  if (&(name) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mName = name;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * @return the name of this SBML object.
 */
const string&
Group::getName () const
{
  return mName;
}


/*
 * @return true if the name of this SBML object has been set, false
 * otherwise.
 */
bool
Group::isSetName () const
{
  return (mName.empty() == false);
}


/*
 * Unsets the name of this SBML object.
 */
int
Group::unsetName ()
{
  mName.erase();

  if (mName.empty())
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Sets kind and returns value indicating success.
 */
int
Group::setKind(const GroupKind_t kind)
{
  if (GroupKind_isValidGroupKind(kind) == 0)
  {
    mKind = GROUP_KIND_UNKNOWN;
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mKind = kind;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * @return the kind of this SBML object.
 */
GroupKind_t
Group::getKind () const
{
  return mKind;
}

/*
 * @return true if the kind of this SBML object has been set, false
 * otherwise.
 */
bool
Group::isSetKind () const
{
  return (mKind != GROUP_KIND_UNKNOWN);
}


int
Group::unsetKind()
{
  mKind = GROUP_KIND_UNKNOWN;
  return LIBSBML_OPERATION_SUCCESS;
}

/*
 * Returns the member object that holds all members.
 */ 
const ListOfMembers*
Group::getListOfMembers () const
{
  return &this->mMembers;
}


/*
 * Remove the member with the given id.
 * A pointer to the removed member is returned.
 * If no member has been removed, NULL is returned.
 */
Member*
Group::removeMember(const std::string& symbol)
{
  return mMembers.remove(symbol);
}


/*
 * Remove the member with the given index.
 * A pointer to the removed member is returned.
 * If no member has been removed, NULL is returned.
 */
Member*
Group::removeMember(unsigned int index)
{
  return mMembers.remove(index);
}


/*
 * Returns the member with the given index.
 * If the index is invalid, NULL is returned.
 */ 
Member* 
Group::getMember (unsigned int index)
{
  return mMembers.get(index);
}

/*
 * Returns the member with the given index.
 * If the index is invalid, NULL is returned.
 */ 
const Member* 
Group::getMember (unsigned int index) const
{
  return mMembers.get(index);
}


/*
 * Returns the member that has the given id, or NULL if no
 * member has the id.
 */
Member*
Group::getMember (const std::string& symbol) 
{
  return mMembers.get(symbol);
}


/*
 * Returns the member that has the given id, or NULL if no
 * member has the id.
 */
const Member*
Group::getMember (const std::string& symbol) const
{
  return mMembers.get(symbol);
}


/*
 * Adds a member element
 */
int
Group::addMember (const Member* member)
{
  if (member == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
//  else if (!(member->hasRequiredAttributes()) || !(member->hasRequiredElements()))
//  {
//    return LIBSBML_INVALID_OBJECT;
//  }
  else if (getLevel() != member->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != member->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (member->isSetId() == true && getMember(member->getId()) != NULL)
  {
    // an object with this id already exists
    return LIBSBML_DUPLICATE_OBJECT_ID;
  }
  else if (matchesRequiredSBMLNamespacesForAddition
    (static_cast<const SBase *>(member)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    mMembers.append(member);

    return LIBSBML_OPERATION_SUCCESS;
  }

}


/*
 * Returns the number of members for the group.
 */
unsigned int
Group::getNumMembers () const
{
  return this->mMembers.size();
}


/*
 * Creates a Member object, adds it to the end of the member
 * objects list and returns a reference to the newly created object.
 */
Member*
Group::createMember ()
{
  Member* m = NULL;

  try
  {
    GROUPS_CREATE_NS(groupsns, getSBMLNamespaces());
    m = new Member(groupsns);
    delete groupsns;
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * do nothing
     */
  }

  if(m != NULL)
  {
    mMembers.appendAndOwn(m);
  }

  return m;
}


/*
 * Returns the  "ListOfMemberConstraints" in this Group object.
 */
const ListOfMemberConstraints*
Group::getListOfMemberConstraints() const
{
  return &mMemberConstraints;
}


/*
 * Returns the  "ListOfMemberConstraints" in this Group object.
 */
ListOfMemberConstraints*
Group::getListOfMemberConstraints()
{
  return &mMemberConstraints;
}


/*
 * Removes the nth MemberConstraint from the ListOfMemberConstraints.
 */
MemberConstraint*
Group::removeMemberConstraint(unsigned int n)
{
  return mMemberConstraints.remove(n);
}


/*
 * Removes the a MemberConstraint with given id from the ListOfMemberConstraints.
 */
MemberConstraint*
Group::removeMemberConstraint(const std::string& sid)
{
  return mMemberConstraints.remove(sid);
}


/*
 * Return the nth MemberConstraint in the ListOfMemberConstraints within this Group.
 */
MemberConstraint*
Group::getMemberConstraint(unsigned int n)
{
  return mMemberConstraints.get(n);
}


/*
 * Return the nth MemberConstraint in the ListOfMemberConstraints within this Group.
 */
const MemberConstraint*
Group::getMemberConstraint(unsigned int n) const
{
  return mMemberConstraints.get(n);
}


/*
 * Return a MemberConstraint from the ListOfMemberConstraints by id.
 */
MemberConstraint*
Group::getMemberConstraint(const std::string& sid)
{
  return mMemberConstraints.get(sid);
}


/*
 * Return a MemberConstraint from the ListOfMemberConstraints by id.
 */
const MemberConstraint*
Group::getMemberConstraint(const std::string& sid) const
{
  return mMemberConstraints.get(sid);
}


/*
 * Adds a copy the given "MemberConstraint" to this Group.
 */
int
Group::addMemberConstraint(const MemberConstraint* mc)
{
  if (mc == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (mc->hasRequiredAttributes() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != mc->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != mc->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (matchesRequiredSBMLNamespacesForAddition(static_cast<const SBase *>(mc)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    mMemberConstraints.append(mc);

    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Get the number of MemberConstraint objects in this Group.
 */
unsigned int
Group::getNumMemberConstraints() const
{
  return mMemberConstraints.size();
}


/*
 * Creates a new MemberConstraint object, adds it to this Groups
 */
MemberConstraint*
Group::createMemberConstraint()
{
  MemberConstraint* mc = NULL;

  try
  {
    GROUPS_CREATE_NS(groupsns, getSBMLNamespaces());
    mc = new MemberConstraint(groupsns);
    delete groupsns;
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * do nothing
     */
  }

  if(mc != NULL)
  {
    mMemberConstraints.appendAndOwn(mc);
  }

  return mc;
}



/*
 * Subclasses should override this method to return XML element name of
 * this SBML object.
 */
const std::string&
Group::getElementName () const
{
  static const std::string name = "group";
  return name;
}


/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase*
Group::createObject (XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase*        object = 0;

  if (name == "listOfMembers")
  {
    object = &mMembers;
  }
  else if (name == "listOfMemberConstraints")
  {
    object = &mMemberConstraints;
  }

  return object;
}

/*
 * Subclasses should override this method to get the list of
 * expected attributes.
 * This function is invoked from corresponding readAttributes()
 * function.
 */
void
Group::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("name");
  attributes.add("kind");
}


/*
 * Subclasses should override this method to read values from the given
 * XMLAttributes set into their specific fields.  Be sure to call your
 * parents implementation of this method as well.
 */
void
Group::readAttributes (const XMLAttributes& attributes,
                        const ExpectedAttributes& expectedAttributes)
{
  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  unsigned int numErrs;

  /* look to see whether an unknown attribute error was logged
   * during the read of the listOfGroups - which will have
   * happened immediately prior to this read
  */

  if (getErrorLog() != NULL &&
      static_cast<ListOfGroups*>(getParentSBMLObject())->size() < 2)
  {
    numErrs = getErrorLog()->getNumErrors();
    for (int n = numErrs-1; n >= 0; n--)
    {
      if (getErrorLog()->getError(n)->getErrorId() == UnknownPackageAttribute)
      {
        const std::string details =
              getErrorLog()->getError(n)->getMessage();
        getErrorLog()->remove(UnknownPackageAttribute);
        getErrorLog()->logPackageError("groups", GroupsUnknownError,
                  getPackageVersion(), sbmlLevel, sbmlVersion, details);
      }
      else if (getErrorLog()->getError(n)->getErrorId() == UnknownCoreAttribute)
      {
        const std::string details =
                   getErrorLog()->getError(n)->getMessage();
        getErrorLog()->remove(UnknownCoreAttribute);
        getErrorLog()->logPackageError("groups", UnknownError,
                  getPackageVersion(), sbmlLevel, sbmlVersion, details);
      }
    }
  }

  SBase::readAttributes(attributes, expectedAttributes);

  // look to see whether an unknown attribute error was logged
  if (getErrorLog() != NULL)
  {
    numErrs = getErrorLog()->getNumErrors();
    for (int n = numErrs-1; n >= 0; n--)
    {
      if (getErrorLog()->getError(n)->getErrorId() == UnknownPackageAttribute)
      {
        const std::string details =
                          getErrorLog()->getError(n)->getMessage();
        getErrorLog()->remove(UnknownPackageAttribute);
        getErrorLog()->logPackageError("groups", UnknownError,
                       getPackageVersion(), sbmlLevel, sbmlVersion, details);
      }
      else if (getErrorLog()->getError(n)->getErrorId() == UnknownCoreAttribute)
      {
        const std::string details =
                          getErrorLog()->getError(n)->getMessage();
        getErrorLog()->remove(UnknownCoreAttribute);
        getErrorLog()->logPackageError("groups", UnknownError,
                       getPackageVersion(), sbmlLevel, sbmlVersion, details);
      }
    }
  }

  bool assigned = false;

  //
  // id SId  ( use = "optional" )
  //
  assigned = attributes.readInto("id", mId);

   if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mId.empty() == true)
    {
      logEmptyString(mId, getLevel(), getVersion(), "<Group>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mId) == false)
    {
      logError(InvalidIdSyntax);
    }
  }

  //
  // name string   ( use = "optional" )
  //
  assigned = attributes.readInto("name", mName);

  if (assigned == true)
  {
    // check string is not empty

    if (mName.empty() == true)
    {
      logEmptyString(mName, getLevel(), getVersion(), "<Group>");
    }
  }


  //
  // kind string   ( use = "required" )
  //
  std::string kind;
  assigned = attributes.readInto("kind", kind);

  if (assigned == true)
  {
    // check string is not empty

    if (kind.empty() == true)
    {
      logEmptyString(kind, getLevel(), getVersion(), "<Group>");
    }
    else 
    {
       mKind = GroupKind_fromString( kind.c_str() );
       if (GroupKind_isValidGroupKind(mKind) == 0)
       {
         logError(NotSchemaConformant, getLevel(), getVersion(),
           "Invalid group kind.");
       }
    }
  }
  else
  {
    std::string message = "Groups attribute 'kind' is missing.";
    getErrorLog()->logPackageError("groups", UnknownError,
                   getPackageVersion(), sbmlLevel, sbmlVersion, message);
  }
}


/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.
 */
void
Group::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);

  if (isSetId() == true)
    stream.writeAttribute("id",   getPrefix(), mId);
  if (isSetName() == true)
    stream.writeAttribute("name", getPrefix(), mName);
  if (isSetKind() == true)
    stream.writeAttribute("kind", getPrefix(), 
                          GroupKind_toString(mKind));

  //
  // (EXTENSION)
  //
  SBase::writeExtensionAttributes(stream);
}


/*
 * Subclasses should override this method to write out their contained
 * SBML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.
 */
void
Group::writeElements (XMLOutputStream& stream) const
{
  SBase::writeElements(stream);

  if (getNumMembers() > 0)
  {
    mMembers.write(stream);
  }

  if (getNumMemberConstraints() > 0)
  {
    mMemberConstraints.write(stream);
  }

  //
  // (EXTENSION)
  //
  SBase::writeExtensionElements(stream);
}


/*
 * @return the typecode (int) of this SBML object or SBML_UNKNOWN
 * (default).
 *
 * @see getElementName()
 */
int
Group::getTypeCode () const
{
  return SBML_GROUPS_GROUP;
}


/*
 * Accepts the given SBMLVisitor.
 */
bool
Group::accept (SBMLVisitor& v) const
{
  v.visit(*this);

/* VISIT CHILDREN */

  v.leave(*this);

  return true;
}


/*
 * Sets the parent SBMLDocument of this SBML object.
 */
void
Group::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);

  mMembers.setSBMLDocument(d);
  mMemberConstraints.setSBMLDocument(d);
}


/*
 * Sets this SBML object to child SBML objects (if any).
 * (Creates a child-parent relationship by the parent)
  */
void
Group::connectToChild()
{
  SBase::connectToChild();
  mMembers.connectToParent(this);
  mMemberConstraints.connectToParent(this);
}


/*
 * Enables/Disables the given package with this element and child
 * elements (if any).
 * (This is an internal implementation for enablePakcage function)
 */
void
Group::enablePackageInternal(const std::string& pkgURI,
                             const std::string& pkgPrefix, bool flag)
{
  SBase::enablePackageInternal(pkgURI,pkgPrefix,flag);

  mMembers.enablePackageInternal(pkgURI,pkgPrefix,flag);
  mMemberConstraints.enablePackageInternal(pkgURI, pkgPrefix, flag);
}



/** @cond doxygenLibsbmlInternal */
bool
Group::hasRequiredAttributes() const
{
  bool allPresent = true;

  
  if (isSetKind() == false)
  {
    allPresent = false;
  }
  
  return allPresent;
}
/** @endcond doxygenLibsbmlInternal */


/** @cond doxygenLibsbmlInternal */
/* default for components that have no required elements */
bool
Group::hasRequiredElements() const
{
  bool allPresent = true;

  
  if (mMembers.size() < 1)
  {
    allPresent = false;
  }
  
  return allPresent;
}
/** @endcond doxygenLibsbmlInternal */


/*
 * Ctor.
 */
ListOfGroups::ListOfGroups(GroupsPkgNamespaces* groupsns)
 : ListOf(groupsns)
{
  //
  // set the element namespace of this object
  //
  setElementNamespace(groupsns->getURI());
}


/*
 * Ctor.
 */
ListOfGroups::ListOfGroups(unsigned int level, unsigned int version, unsigned int pkgVersion)
 : ListOf(level,version)
{
  setSBMLNamespacesAndOwn(new GroupsPkgNamespaces(level,version,pkgVersion));
};


/*
 * @return a (deep) copy of this ListOfUnitDefinitions.
 */
ListOfGroups*
ListOfGroups::clone () const
{
  return new ListOfGroups(*this);
}


/* return nth item in list */
Group *
ListOfGroups::get(unsigned int n)
{
  return static_cast<Group*>(ListOf::get(n));
}


/* return nth item in list */
const Group *
ListOfGroups::get(unsigned int n) const
{
  return static_cast<const Group*>(ListOf::get(n));
}


/* return item by id */
Group*
ListOfGroups::get (const std::string& sid)
{
  return const_cast<Group*>( 
    static_cast<const ListOfGroups&>(*this).get(sid) );
}


/* return item by id */
const Group*
ListOfGroups::get (const std::string& sid) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<Group>(sid) );
  return (result == mItems.end()) ? 0 : static_cast <Group*> (*result);
}


/* Removes the nth item from this list */
Group*
ListOfGroups::remove (unsigned int n)
{
   return static_cast<Group*>(ListOf::remove(n));
}


/* Removes item in this list by id */
Group*
ListOfGroups::remove (const std::string& sid)
{
  SBase* item = 0;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<Group>(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <Group*> (item);
}


/*
 * @return the typecode (int) of SBML objects contained in this ListOf or
 * SBML_UNKNOWN (default).
 */
int
ListOfGroups::getItemTypeCode () const
{
  return SBML_GROUPS_GROUP;
}


/*
 * Subclasses should override this method to return XML element name of
 * this SBML object.
 */
const std::string&
ListOfGroups::getElementName () const
{
  static const std::string name = "listOfGroups";
  return name;
}


/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase*
ListOfGroups::createObject (XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase*        object = 0;


  if (name == "group")
  {
    GROUPS_CREATE_NS(groupsns, this->getSBMLNamespaces());
    object = new Group(groupsns);
    appendAndOwn(object);
    delete groupsns;
    //mItems.push_back(object);
  }

  return object;
}


void 
ListOfGroups::writeXMLNS (XMLOutputStream& stream) const
{
  XMLNamespaces xmlns;

  std::string prefix = getPrefix();

  if (prefix.empty())
  {
    XMLNamespaces* thisxmlns = getNamespaces();
    if (thisxmlns && thisxmlns->hasURI(GroupsExtension::getXmlnsL3V1V1()))
    {
      xmlns.add(GroupsExtension::getXmlnsL3V1V1(),prefix);
    }
  }

  stream << xmlns;
}

static
const char* GROUP_KIND_STRINGS[] =
{
    "classification"
  , "partonomy"
  , "collection"
};


LIBSBML_EXTERN
const char* 
GroupKind_toString(GroupKind_t kind)
{
  int max = GROUP_KIND_UNKNOWN;

  if (kind < GROUP_KIND_CLASSIFICATION || kind >= max)
  {
      return NULL;
  }

  return GROUP_KIND_STRINGS[kind];
}


LIBSBML_EXTERN
GroupKind_t 
GroupKind_fromString(const char* s)
{
  if (s == NULL) 
  {
    return GROUP_KIND_UNKNOWN;
  }

  int max = GROUP_KIND_UNKNOWN;
  for (int i = 0; i < max; i++)
  {
    if (strcmp(GROUP_KIND_STRINGS[i], s) == 0)
      return (GroupKind_t)i;
  }
  return GROUP_KIND_UNKNOWN;
}


LIBSBML_EXTERN
int 
GroupKind_isValidGroupKind(GroupKind_t kind)
{
  int max = GROUP_KIND_UNKNOWN;

  if (kind < GROUP_KIND_CLASSIFICATION || kind >= max)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}


LIBSBML_EXTERN
int 
GroupKind_isValidGroupKindString(const char* s)
{
  return GroupKind_isValidGroupKind(GroupKind_fromString(s));
}


LIBSBML_CPP_NAMESPACE_END


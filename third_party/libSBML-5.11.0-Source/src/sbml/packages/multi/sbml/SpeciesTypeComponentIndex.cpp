/**
 * @file:   SpeciesTypeComponentIndex.cpp
 * @brief:  Implementation of the SpeciesTypeComponentIndex class
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


#include <sbml/packages/multi/sbml/SpeciesTypeComponentIndex.h>
#include <sbml/packages/multi/validator/MultiSBMLError.h>

#include <sbml/util/ElementFilter.h>

using namespace std;


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new SpeciesTypeComponentIndex with the given level, version, and package version.
 */
SpeciesTypeComponentIndex::SpeciesTypeComponentIndex (unsigned int level, unsigned int version, unsigned int pkgVersion)
  : SBase(level, version)
   ,mId ("")
   ,mComponent ("")
   ,mOccur (SBML_INT_MAX)
   ,mIsSetOccur (false)
   ,mIdentifyingParent ("")
   ,mDenotedSpeciesTypeComponentIndexes (level, version, pkgVersion)
{
  // set an SBMLNamespaces derived object of this package
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion));

  // connect to child objects
  connectToChild();
}


/*
 * Creates a new SpeciesTypeComponentIndex with the given MultiPkgNamespaces object.
 */
SpeciesTypeComponentIndex::SpeciesTypeComponentIndex (MultiPkgNamespaces* multins)
  : SBase(multins)
   ,mId ("")
   ,mComponent ("")
   ,mOccur (SBML_INT_MAX)
   ,mIsSetOccur (false)
   ,mIdentifyingParent ("")
   ,mDenotedSpeciesTypeComponentIndexes (multins)
{
  // set the element namespace of this object
  setElementNamespace(multins->getURI());

  // connect to child objects
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(multins);
}


/*
 * Copy constructor for SpeciesTypeComponentIndex.
 */
SpeciesTypeComponentIndex::SpeciesTypeComponentIndex (const SpeciesTypeComponentIndex& orig)
  : SBase(orig)
{
  if (&orig == NULL)
  {
    throw SBMLConstructorException("Null argument to copy constructor");
  }
  else
  {
    mId  = orig.mId;
    mComponent  = orig.mComponent;
    mOccur  = orig.mOccur;
    mIsSetOccur  = orig.mIsSetOccur;
    mIdentifyingParent  = orig.mIdentifyingParent;
    mDenotedSpeciesTypeComponentIndexes  = orig.mDenotedSpeciesTypeComponentIndexes;

    // connect to child objects
    connectToChild();
  }
}


/*
 * Assignment for SpeciesTypeComponentIndex.
 */
SpeciesTypeComponentIndex&
SpeciesTypeComponentIndex::operator=(const SpeciesTypeComponentIndex& rhs)
{
  if (&rhs == NULL)
  {
    throw SBMLConstructorException("Null argument to assignment");
  }
  else if (&rhs != this)
  {
    SBase::operator=(rhs);
    mId  = rhs.mId;
    mComponent  = rhs.mComponent;
    mOccur  = rhs.mOccur;
    mIsSetOccur  = rhs.mIsSetOccur;
    mIdentifyingParent  = rhs.mIdentifyingParent;
    mDenotedSpeciesTypeComponentIndexes  = rhs.mDenotedSpeciesTypeComponentIndexes;

    // connect to child objects
    connectToChild();
  }
  return *this;
}


/*
 * Clone for SpeciesTypeComponentIndex.
 */
SpeciesTypeComponentIndex*
SpeciesTypeComponentIndex::clone () const
{
  return new SpeciesTypeComponentIndex(*this);
}


/*
 * Destructor for SpeciesTypeComponentIndex.
 */
SpeciesTypeComponentIndex::~SpeciesTypeComponentIndex ()
{
}


/*
 * Returns the value of the "id" attribute of this SpeciesTypeComponentIndex.
 */
const std::string&
SpeciesTypeComponentIndex::getId() const
{
  return mId;
}


/*
 * Returns the value of the "component" attribute of this SpeciesTypeComponentIndex.
 */
const std::string&
SpeciesTypeComponentIndex::getComponent() const
{
  return mComponent;
}


/*
 * Returns the value of the "occur" attribute of this SpeciesTypeComponentIndex.
 */
unsigned int
SpeciesTypeComponentIndex::getOccur() const
{
  return mOccur;
}


/*
 * Returns the value of the "identifyingParent" attribute of this SpeciesTypeComponentIndex.
 */
const std::string&
SpeciesTypeComponentIndex::getIdentifyingParent() const
{
  return mIdentifyingParent;
}


/*
 * Returns true/false if id is set.
 */
bool
SpeciesTypeComponentIndex::isSetId() const
{
  return (mId.empty() == false);
}


/*
 * Returns true/false if component is set.
 */
bool
SpeciesTypeComponentIndex::isSetComponent() const
{
  return (mComponent.empty() == false);
}


/*
 * Returns true/false if occur is set.
 */
bool
SpeciesTypeComponentIndex::isSetOccur() const
{
  return mIsSetOccur;
}


/*
 * Returns true/false if identifyingParent is set.
 */
bool
SpeciesTypeComponentIndex::isSetIdentifyingParent() const
{
  return (mIdentifyingParent.empty() == false);
}


/*
 * Sets id and returns value indicating success.
 */
int
SpeciesTypeComponentIndex::setId(const std::string& id)
{
  return SyntaxChecker::checkAndSetSId(id, mId);
}


/*
 * Sets component and returns value indicating success.
 */
int
SpeciesTypeComponentIndex::setComponent(const std::string& component)
{
  if (&(component) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else if (!(SyntaxChecker::isValidInternalSId(component)))
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mComponent = component;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Sets occur and returns value indicating success.
 */
int
SpeciesTypeComponentIndex::setOccur(unsigned int occur)
{
  mOccur = occur;
  mIsSetOccur = true;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Sets identifyingParent and returns value indicating success.
 */
int
SpeciesTypeComponentIndex::setIdentifyingParent(const std::string& identifyingParent)
{
  if (&(identifyingParent) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else if (!(SyntaxChecker::isValidInternalSId(identifyingParent)))
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mIdentifyingParent = identifyingParent;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Unsets id and returns value indicating success.
 */
int
SpeciesTypeComponentIndex::unsetId()
{
  mId.erase();

  if (mId.empty() == true)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Unsets component and returns value indicating success.
 */
int
SpeciesTypeComponentIndex::unsetComponent()
{
  mComponent.erase();

  if (mComponent.empty() == true)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Unsets occur and returns value indicating success.
 */
int
SpeciesTypeComponentIndex::unsetOccur()
{
  mOccur = SBML_INT_MAX;
  mIsSetOccur = false;

  if (isSetOccur() == false)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Unsets identifyingParent and returns value indicating success.
 */
int
SpeciesTypeComponentIndex::unsetIdentifyingParent()
{
  mIdentifyingParent.erase();

  if (mIdentifyingParent.empty() == true)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Returns the  "ListOfDenotedSpeciesTypeComponentIndexes" in this SpeciesTypeComponentIndex object.
 */
const ListOfDenotedSpeciesTypeComponentIndexes*
SpeciesTypeComponentIndex::getListOfDenotedSpeciesTypeComponentIndexes() const
{
  return &mDenotedSpeciesTypeComponentIndexes;
}


/*
 * Returns the  "ListOfDenotedSpeciesTypeComponentIndexes" in this SpeciesTypeComponentIndex object.
 */
ListOfDenotedSpeciesTypeComponentIndexes*
SpeciesTypeComponentIndex::getListOfDenotedSpeciesTypeComponentIndexes()
{
  return &mDenotedSpeciesTypeComponentIndexes;
}


/*
 * Removes the nth DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes.
 */
DenotedSpeciesTypeComponentIndex*
SpeciesTypeComponentIndex::removeDenotedSpeciesTypeComponentIndex(unsigned int n)
{
  return mDenotedSpeciesTypeComponentIndexes.remove(n);
}


/*
 * Removes the a DenotedSpeciesTypeComponentIndex with given id from the ListOfDenotedSpeciesTypeComponentIndexes.
 */
DenotedSpeciesTypeComponentIndex*
SpeciesTypeComponentIndex::removeDenotedSpeciesTypeComponentIndex(const std::string& sid)
{
  return mDenotedSpeciesTypeComponentIndexes.remove(sid);
}


/*
 * Return the nth DenotedSpeciesTypeComponentIndex in the ListOfDenotedSpeciesTypeComponentIndexes within this SpeciesTypeComponentIndex.
 */
DenotedSpeciesTypeComponentIndex*
SpeciesTypeComponentIndex::getDenotedSpeciesTypeComponentIndex(unsigned int n)
{
  return mDenotedSpeciesTypeComponentIndexes.get(n);
}


/*
 * Return the nth DenotedSpeciesTypeComponentIndex in the ListOfDenotedSpeciesTypeComponentIndexes within this SpeciesTypeComponentIndex.
 */
const DenotedSpeciesTypeComponentIndex*
SpeciesTypeComponentIndex::getDenotedSpeciesTypeComponentIndex(unsigned int n) const
{
  return mDenotedSpeciesTypeComponentIndexes.get(n);
}


/*
 * Return a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes by id.
 */
DenotedSpeciesTypeComponentIndex*
SpeciesTypeComponentIndex::getDenotedSpeciesTypeComponentIndex(const std::string& sid)
{
  return mDenotedSpeciesTypeComponentIndexes.get(sid);
}


/*
 * Return a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes by id.
 */
const DenotedSpeciesTypeComponentIndex*
SpeciesTypeComponentIndex::getDenotedSpeciesTypeComponentIndex(const std::string& sid) const
{
  return mDenotedSpeciesTypeComponentIndexes.get(sid);
}


/*
 * Adds a copy the given "DenotedSpeciesTypeComponentIndex" to this SpeciesTypeComponentIndex.
 */
int
SpeciesTypeComponentIndex::addDenotedSpeciesTypeComponentIndex(const DenotedSpeciesTypeComponentIndex* dstci)
{
  if (dstci == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (dstci->hasRequiredAttributes() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != dstci->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != dstci->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (matchesRequiredSBMLNamespacesForAddition(static_cast<const SBase *>(dstci)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    mDenotedSpeciesTypeComponentIndexes.append(dstci);

    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Get the number of DenotedSpeciesTypeComponentIndex objects in this SpeciesTypeComponentIndex.
 */
unsigned int
SpeciesTypeComponentIndex::getNumDenotedSpeciesTypeComponentIndexes() const
{
  return mDenotedSpeciesTypeComponentIndexes.size();
}


/*
 * Creates a new DenotedSpeciesTypeComponentIndex object, adds it to this SpeciesTypeComponentIndexes
 */
DenotedSpeciesTypeComponentIndex*
SpeciesTypeComponentIndex::createDenotedSpeciesTypeComponentIndex()
{
  DenotedSpeciesTypeComponentIndex* dstci = NULL;

  try
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    dstci = new DenotedSpeciesTypeComponentIndex(multins);
    delete multins;
  }
  catch (...)
  {
    /* here we do not create a default object as the level/version must
     * match the parent object
     *
     * do nothing
     */
  }

  if(dstci != NULL)
  {
    mDenotedSpeciesTypeComponentIndexes.appendAndOwn(dstci);
  }

  return dstci;
}


/*
 * rename attributes that are SIdRefs or instances in math
 */
void
SpeciesTypeComponentIndex::renameSIdRefs(const std::string& oldid, const std::string& newid)
{
  if (isSetComponent() == true && mComponent == oldid)
  {
    setComponent(newid);
  }

  if (isSetIdentifyingParent() == true && mIdentifyingParent == oldid)
  {
    setIdentifyingParent(newid);
  }

}


List*
SpeciesTypeComponentIndex::getAllElements(ElementFilter* filter)
{
  List* ret = new List();
  List* sublist = NULL;


  ADD_FILTERED_FROM_PLUGIN(ret, sublist, filter);

  return ret;
}


/*
 * Returns the XML element name of this object
 */
const std::string&
SpeciesTypeComponentIndex::getElementName () const
{
  static const string name = "speciesTypeComponentIndex";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
SpeciesTypeComponentIndex::getTypeCode () const
{
  return SBML_MULTI_SPECIES_TYPE_COMPONENT_INDEX;
}


/*
 * check if all the required attributes are set
 */
bool
SpeciesTypeComponentIndex::hasRequiredAttributes () const
{
  bool allPresent = true;

  if (isSetId() == false)
    allPresent = false;

  if (isSetComponent() == false)
    allPresent = false;

  if (isSetOccur() == false)
    allPresent = false;

  return allPresent;
}


/*
 * check if all the required elements are set
 */
bool
SpeciesTypeComponentIndex::hasRequiredElements () const
{
  bool allPresent = true;

  return allPresent;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * write contained elements
 */
void
SpeciesTypeComponentIndex::writeElements (XMLOutputStream& stream) const
{
  SBase::writeElements(stream);

  if (getNumDenotedSpeciesTypeComponentIndexes() > 0)
  {
    mDenotedSpeciesTypeComponentIndexes.write(stream);
  }

  SBase::writeExtensionElements(stream);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Accepts the given SBMLVisitor.
 */
bool
SpeciesTypeComponentIndex::accept (SBMLVisitor& v) const
{
  v.visit(*this);

/* VISIT CHILDREN */

  v.leave(*this);

  return true;
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Sets the parent SBMLDocument.
 */
void
SpeciesTypeComponentIndex::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);
  mDenotedSpeciesTypeComponentIndexes.setSBMLDocument(d);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
   * Connects to child elements.
 */
void
SpeciesTypeComponentIndex::connectToChild()
{
  mDenotedSpeciesTypeComponentIndexes.connectToParent(this);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Enables/Disables the given package with this element.
 */
void
SpeciesTypeComponentIndex::enablePackageInternal(const std::string& pkgURI,
             const std::string& pkgPrefix, bool flag)
{
  SBase::enablePackageInternal(pkgURI, pkgPrefix, flag);
  mDenotedSpeciesTypeComponentIndexes.enablePackageInternal(pkgURI, pkgPrefix, flag);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * creates object.
 */
SBase*
SpeciesTypeComponentIndex::createObject(XMLInputStream& stream)
{
  const string& name = stream.peek().getName();
  SBase* object = NULL;

  MULTI_CREATE_NS(multins, getSBMLNamespaces());

  if (name == "listOfDenotedSpeciesTypeComponentIndexes")
  {
    object = &mDenotedSpeciesTypeComponentIndexes;
  }

  delete multins;
  return object;
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Get the list of expected attributes for this element.
 */
void
SpeciesTypeComponentIndex::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("component");
  attributes.add("occur");
  attributes.add("identifyingParent");
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Read values from the given XMLAttributes set into their specific fields.
 */
void
SpeciesTypeComponentIndex::readAttributes (const XMLAttributes& attributes,
                             const ExpectedAttributes& expectedAttributes)
{
  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  unsigned int numErrs;

  /* look to see whether an unknown attribute error was logged
   * during the read of the listOfSpeciesTypeComponentIndexes - which will have
   * happened immediately prior to this read
  */

  if (getErrorLog() != NULL &&
      static_cast<ListOfSpeciesTypeComponentIndexes*>(getParentSBMLObject())->size() < 2)
  {
    numErrs = getErrorLog()->getNumErrors();
    for (int n = numErrs-1; n >= 0; n--)
    {
      if (getErrorLog()->getError(n)->getErrorId() == UnknownPackageAttribute)
      {
        const std::string details =
              getErrorLog()->getError(n)->getMessage();
        getErrorLog()->remove(UnknownPackageAttribute);
        getErrorLog()->logPackageError("multi", MultiUnknownError,
                  getPackageVersion(), sbmlLevel, sbmlVersion, details);
      }
      else if (getErrorLog()->getError(n)->getErrorId() == UnknownCoreAttribute)
      {
        const std::string details =
                   getErrorLog()->getError(n)->getMessage();
        getErrorLog()->remove(UnknownCoreAttribute);
        getErrorLog()->logPackageError("multi", MultiUnknownError,
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
        getErrorLog()->logPackageError("multi", MultiUnknownError,
                       getPackageVersion(), sbmlLevel, sbmlVersion, details);
      }
      else if (getErrorLog()->getError(n)->getErrorId() == UnknownCoreAttribute)
      {
        const std::string details =
                          getErrorLog()->getError(n)->getMessage();
        getErrorLog()->remove(UnknownCoreAttribute);
        getErrorLog()->logPackageError("multi", MultiUnknownError,
                       getPackageVersion(), sbmlLevel, sbmlVersion, details);
      }
    }
  }

  bool assigned = false;

  //
  // id SId  ( use = "required" )
  //
  assigned = attributes.readInto("id", mId);

   if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mId.empty() == true)
    {
      logEmptyString(mId, getLevel(), getVersion(), "<SpeciesTypeComponentIndex>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mId) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute id='" + mId + "' does not conform.");
    }
  }
  else
  {
    std::string message = "Multi attribute 'id' is missing.";
    getErrorLog()->logPackageError("multi", MultiUnknownError,
                   getPackageVersion(), sbmlLevel, sbmlVersion, message);
  }

  //
  // component SIdRef   ( use = "required" )
  //
  assigned = attributes.readInto("component", mComponent);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mComponent.empty() == true)
    {
      logEmptyString(mComponent, getLevel(), getVersion(), "<SpeciesTypeComponentIndex>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mComponent) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute component='" + mComponent + "' does not conform.");
    }
  }
  else
  {
    std::string message = "Multi attribute 'component' is missing.";
    getErrorLog()->logPackageError("multi", MultiUnknownError,
                   getPackageVersion(), sbmlLevel, sbmlVersion, message);
  }

  //
  // occur unsigned int   ( use = "required" )
  //
  numErrs = getErrorLog()->getNumErrors();
  mIsSetOccur = attributes.readInto("occur", mOccur);

  if (mIsSetOccur == false)
  {
    if (getErrorLog() != NULL)
    {
      if (getErrorLog()->getNumErrors() == numErrs + 1 &&
              getErrorLog()->contains(XMLAttributeTypeMismatch))
      {
        getErrorLog()->remove(XMLAttributeTypeMismatch);
        getErrorLog()->logPackageError("multi", MultiUnknownError,
                     getPackageVersion(), sbmlLevel, sbmlVersion);
      }
      else
      {
        std::string message = "Multi attribute 'occur' is missing.";
        getErrorLog()->logPackageError("multi", MultiUnknownError,
                       getPackageVersion(), sbmlLevel, sbmlVersion, message);
      }
    }
  }

  //
  // identifyingParent SIdRef   ( use = "optional" )
  //
  assigned = attributes.readInto("identifyingParent", mIdentifyingParent);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mIdentifyingParent.empty() == true)
    {
      logEmptyString(mIdentifyingParent, getLevel(), getVersion(), "<SpeciesTypeComponentIndex>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mIdentifyingParent) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute identifyingParent='" + mIdentifyingParent 
        + "' does not conform.");
    }
  }

}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Write values of XMLAttributes to the output stream.
 */
  void
SpeciesTypeComponentIndex::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);

  if (isSetId() == true)
    stream.writeAttribute("id", getPrefix(), mId);

  if (isSetComponent() == true)
    stream.writeAttribute("component", getPrefix(), mComponent);

  if (isSetOccur() == true)
    stream.writeAttribute("occur", getPrefix(), mOccur);

  if (isSetIdentifyingParent() == true)
    stream.writeAttribute("identifyingParent", getPrefix(), mIdentifyingParent);

  SBase::writeExtensionAttributes(stream);

}


  /** @endcond doxygenLibsbmlInternal */


/*
 * Constructor 
 */
ListOfSpeciesTypeComponentIndexes::ListOfSpeciesTypeComponentIndexes(unsigned int level, 
                                   unsigned int version, 
                                   unsigned int pkgVersion)
 : ListOf(level, version)
{
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion)); 
}


/*
 * Constructor 
 */
ListOfSpeciesTypeComponentIndexes::ListOfSpeciesTypeComponentIndexes(MultiPkgNamespaces* multins)
  : ListOf(multins)
{
  setElementNamespace(multins->getURI());
}


/*
 * Returns a deep copy of this ListOfSpeciesTypeComponentIndexes 
 */
ListOfSpeciesTypeComponentIndexes* 
ListOfSpeciesTypeComponentIndexes::clone () const
 {
  return new ListOfSpeciesTypeComponentIndexes(*this);
}


/*
 * Get a SpeciesTypeComponentIndex from the ListOfSpeciesTypeComponentIndexes by index.
*/
SpeciesTypeComponentIndex*
ListOfSpeciesTypeComponentIndexes::get(unsigned int n)
{
  return static_cast<SpeciesTypeComponentIndex*>(ListOf::get(n));
}


/*
 * Get a SpeciesTypeComponentIndex from the ListOfSpeciesTypeComponentIndexes by index.
 */
const SpeciesTypeComponentIndex*
ListOfSpeciesTypeComponentIndexes::get(unsigned int n) const
{
  return static_cast<const SpeciesTypeComponentIndex*>(ListOf::get(n));
}


/*
 * Get a SpeciesTypeComponentIndex from the ListOfSpeciesTypeComponentIndexes by id.
 */
SpeciesTypeComponentIndex*
ListOfSpeciesTypeComponentIndexes::get(const std::string& sid)
{
  return const_cast<SpeciesTypeComponentIndex*>(
    static_cast<const ListOfSpeciesTypeComponentIndexes&>(*this).get(sid));
}


/*
 * Get a SpeciesTypeComponentIndex from the ListOfSpeciesTypeComponentIndexes by id.
 */
const SpeciesTypeComponentIndex*
ListOfSpeciesTypeComponentIndexes::get(const std::string& sid) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<SpeciesTypeComponentIndex>(sid) );
  return (result == mItems.end()) ? 0 : static_cast <SpeciesTypeComponentIndex*> (*result);
}


/*
 * Removes the nth SpeciesTypeComponentIndex from this ListOfSpeciesTypeComponentIndexes
 */
SpeciesTypeComponentIndex*
ListOfSpeciesTypeComponentIndexes::remove(unsigned int n)
{
  return static_cast<SpeciesTypeComponentIndex*>(ListOf::remove(n));
}


/*
 * Removes the SpeciesTypeComponentIndex from this ListOfSpeciesTypeComponentIndexes with the given identifier
 */
SpeciesTypeComponentIndex*
ListOfSpeciesTypeComponentIndexes::remove(const std::string& sid)
{
  SBase* item = NULL;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<SpeciesTypeComponentIndex>(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <SpeciesTypeComponentIndex*> (item);
}


/*
 * Returns the XML element name of this object
 */
const std::string&
ListOfSpeciesTypeComponentIndexes::getElementName () const
{
  static const string name = "listOfSpeciesTypeComponentIndexes";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
ListOfSpeciesTypeComponentIndexes::getTypeCode () const
{
  return SBML_LIST_OF;
}


/*
 * Returns the libSBML type code for the objects in this LIST_OF.
 */
int
ListOfSpeciesTypeComponentIndexes::getItemTypeCode () const
{
  return SBML_MULTI_SPECIES_TYPE_COMPONENT_INDEX;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * Creates a new SpeciesTypeComponentIndex in this ListOfSpeciesTypeComponentIndexes
 */
SBase*
ListOfSpeciesTypeComponentIndexes::createObject(XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase* object = NULL;

  if (name == "speciesTypeComponentIndex")
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    object = new SpeciesTypeComponentIndex(multins);
    appendAndOwn(object);
    delete multins;
  }

  return object;
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Write the namespace for the Multi package.
 */
void
ListOfSpeciesTypeComponentIndexes::writeXMLNS(XMLOutputStream& stream) const
{
  XMLNamespaces xmlns;

  std::string prefix = getPrefix();

  if (prefix.empty())
  {
    XMLNamespaces* thisxmlns = getNamespaces();
    if (thisxmlns && thisxmlns->hasURI(MultiExtension::getXmlnsL3V1V1()))
    {
      xmlns.add(MultiExtension::getXmlnsL3V1V1(),prefix);
    }
  }

  stream << xmlns;
}


  /** @endcond doxygenLibsbmlInternal */


/**
 * 
 */
LIBSBML_EXTERN
SpeciesTypeComponentIndex_t *
SpeciesTypeComponentIndex_create(unsigned int level, unsigned int version,
                                 unsigned int pkgVersion)
{
  return new SpeciesTypeComponentIndex(level, version, pkgVersion);
}


/**
 * 
 */
LIBSBML_EXTERN
void
SpeciesTypeComponentIndex_free(SpeciesTypeComponentIndex_t * stci)
{
  if (stci != NULL)
    delete stci;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesTypeComponentIndex_t *
SpeciesTypeComponentIndex_clone(SpeciesTypeComponentIndex_t * stci)
{
  if (stci != NULL)
  {
    return static_cast<SpeciesTypeComponentIndex_t*>(stci->clone());
  }
  else
  {
    return NULL;
  }
}


/**
 * 
 */
LIBSBML_EXTERN
char *
SpeciesTypeComponentIndex_getId(SpeciesTypeComponentIndex_t * stci)
{
  if (stci == NULL)
    return NULL;

  return stci->getId().empty() ? NULL : safe_strdup(stci->getId().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
char *
SpeciesTypeComponentIndex_getComponent(SpeciesTypeComponentIndex_t * stci)
{
  if (stci == NULL)
    return NULL;

  return stci->getComponent().empty() ? NULL : safe_strdup(stci->getComponent().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
unsigned int
SpeciesTypeComponentIndex_getOccur(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? stci->getOccur() : SBML_INT_MAX;
}


/**
 * 
 */
LIBSBML_EXTERN
char *
SpeciesTypeComponentIndex_getIdentifyingParent(SpeciesTypeComponentIndex_t * stci)
{
  if (stci == NULL)
    return NULL;

  return stci->getIdentifyingParent().empty() ? NULL : safe_strdup(stci->getIdentifyingParent().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_isSetId(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? static_cast<int>(stci->isSetId()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_isSetComponent(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? static_cast<int>(stci->isSetComponent()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_isSetOccur(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? static_cast<int>(stci->isSetOccur()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_isSetIdentifyingParent(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? static_cast<int>(stci->isSetIdentifyingParent()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_setId(SpeciesTypeComponentIndex_t * stci, const char * id)
{
  return (stci != NULL) ? stci->setId(id) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_setComponent(SpeciesTypeComponentIndex_t * stci, const char * component)
{
  return (stci != NULL) ? stci->setComponent(component) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_setOccur(SpeciesTypeComponentIndex_t * stci, unsigned int occur)
{
  return (stci != NULL) ? stci->setOccur(occur) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_setIdentifyingParent(SpeciesTypeComponentIndex_t * stci, const char * identifyingParent)
{
  return (stci != NULL) ? stci->setIdentifyingParent(identifyingParent) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_unsetId(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? stci->unsetId() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_unsetComponent(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? stci->unsetComponent() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_unsetOccur(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? stci->unsetOccur() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_unsetIdentifyingParent(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? stci->unsetIdentifyingParent() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesTypeComponentIndex_hasRequiredAttributes(SpeciesTypeComponentIndex_t * stci)
{
  return (stci != NULL) ? static_cast<int>(stci->hasRequiredAttributes()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesTypeComponentIndex_t *
ListOfSpeciesTypeComponentIndexes_getById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfSpeciesTypeComponentIndexes *>(lo)->get(sid) : NULL;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesTypeComponentIndex_t *
ListOfSpeciesTypeComponentIndexes_removeById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfSpeciesTypeComponentIndexes *>(lo)->remove(sid) : NULL;
}




LIBSBML_CPP_NAMESPACE_END



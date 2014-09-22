/**
 * @file:   SpeciesFeature.cpp
 * @brief:  Implementation of the SpeciesFeature class
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


#include <sbml/packages/multi/sbml/SpeciesFeature.h>
#include <sbml/packages/multi/validator/MultiSBMLError.h>

#include <sbml/util/ElementFilter.h>


using namespace std;


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new SpeciesFeature with the given level, version, and package version.
 */
SpeciesFeature::SpeciesFeature (unsigned int level, unsigned int version, unsigned int pkgVersion)
  : SBase(level, version)
   ,mId ("")
   ,mSpeciesFeatureType ("")
   ,mOccur (SBML_INT_MAX)
   ,mIsSetOccur (false)
   ,mComponent ("")
   ,mSpeciesFeatureValues (level, version, pkgVersion)
{
  // set an SBMLNamespaces derived object of this package
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion));

  // connect to child objects
  connectToChild();
}


/*
 * Creates a new SpeciesFeature with the given MultiPkgNamespaces object.
 */
SpeciesFeature::SpeciesFeature (MultiPkgNamespaces* multins)
  : SBase(multins)
   ,mId ("")
   ,mSpeciesFeatureType ("")
   ,mOccur (SBML_INT_MAX)
   ,mIsSetOccur (false)
   ,mComponent ("")
   ,mSpeciesFeatureValues (multins)
{
  // set the element namespace of this object
  setElementNamespace(multins->getURI());

  // connect to child objects
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(multins);
}


/*
 * Copy constructor for SpeciesFeature.
 */
SpeciesFeature::SpeciesFeature (const SpeciesFeature& orig)
  : SBase(orig)
{
  if (&orig == NULL)
  {
    throw SBMLConstructorException("Null argument to copy constructor");
  }
  else
  {
    mId  = orig.mId;
    mSpeciesFeatureType  = orig.mSpeciesFeatureType;
    mOccur  = orig.mOccur;
    mIsSetOccur  = orig.mIsSetOccur;
    mComponent  = orig.mComponent;
    mSpeciesFeatureValues  = orig.mSpeciesFeatureValues;

    // connect to child objects
    connectToChild();
  }
}


/*
 * Assignment for SpeciesFeature.
 */
SpeciesFeature&
SpeciesFeature::operator=(const SpeciesFeature& rhs)
{
  if (&rhs == NULL)
  {
    throw SBMLConstructorException("Null argument to assignment");
  }
  else if (&rhs != this)
  {
    SBase::operator=(rhs);
    mId  = rhs.mId;
    mSpeciesFeatureType  = rhs.mSpeciesFeatureType;
    mOccur  = rhs.mOccur;
    mIsSetOccur  = rhs.mIsSetOccur;
    mComponent  = rhs.mComponent;
    mSpeciesFeatureValues  = rhs.mSpeciesFeatureValues;

    // connect to child objects
    connectToChild();
  }
  return *this;
}


/*
 * Clone for SpeciesFeature.
 */
SpeciesFeature*
SpeciesFeature::clone () const
{
  return new SpeciesFeature(*this);
}


/*
 * Destructor for SpeciesFeature.
 */
SpeciesFeature::~SpeciesFeature ()
{
}


/*
 * Returns the value of the "id" attribute of this SpeciesFeature.
 */
const std::string&
SpeciesFeature::getId() const
{
  return mId;
}


/*
 * Returns the value of the "speciesFeatureType" attribute of this SpeciesFeature.
 */
const std::string&
SpeciesFeature::getSpeciesFeatureType() const
{
  return mSpeciesFeatureType;
}


/*
 * Returns the value of the "occur" attribute of this SpeciesFeature.
 */
const unsigned int
SpeciesFeature::getOccur() const
{
  return mOccur;
}


/*
 * Returns the value of the "component" attribute of this SpeciesFeature.
 */
const std::string&
SpeciesFeature::getComponent() const
{
  return mComponent;
}


/*
 * Returns true/false if id is set.
 */
bool
SpeciesFeature::isSetId() const
{
  return (mId.empty() == false);
}


/*
 * Returns true/false if speciesFeatureType is set.
 */
bool
SpeciesFeature::isSetSpeciesFeatureType() const
{
  return (mSpeciesFeatureType.empty() == false);
}


/*
 * Returns true/false if occur is set.
 */
bool
SpeciesFeature::isSetOccur() const
{
  return mIsSetOccur;
}


/*
 * Returns true/false if component is set.
 */
bool
SpeciesFeature::isSetComponent() const
{
  return (mComponent.empty() == false);
}


/*
 * Sets id and returns value indicating success.
 */
int
SpeciesFeature::setId(const std::string& id)
{
  return SyntaxChecker::checkAndSetSId(id, mId);
}


/*
 * Sets speciesFeatureType and returns value indicating success.
 */
int
SpeciesFeature::setSpeciesFeatureType(const std::string& speciesFeatureType)
{
  if (&(speciesFeatureType) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else if (!(SyntaxChecker::isValidInternalSId(speciesFeatureType)))
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mSpeciesFeatureType = speciesFeatureType;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Sets occur and returns value indicating success.
 */
int
SpeciesFeature::setOccur(unsigned int occur)
{
  mOccur = occur;
  mIsSetOccur = true;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Sets component and returns value indicating success.
 */
int
SpeciesFeature::setComponent(const std::string& component)
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
 * Unsets id and returns value indicating success.
 */
int
SpeciesFeature::unsetId()
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
 * Unsets speciesFeatureType and returns value indicating success.
 */
int
SpeciesFeature::unsetSpeciesFeatureType()
{
  mSpeciesFeatureType.erase();

  if (mSpeciesFeatureType.empty() == true)
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
SpeciesFeature::unsetOccur()
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
 * Unsets component and returns value indicating success.
 */
int
SpeciesFeature::unsetComponent()
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
 * Returns the  "ListOfSpeciesFeatureValues" in this SpeciesFeature object.
 */
const ListOfSpeciesFeatureValues*
SpeciesFeature::getListOfSpeciesFeatureValues() const
{
  return &mSpeciesFeatureValues;
}


/*
 * Returns the  "ListOfSpeciesFeatureValues" in this SpeciesFeature object.
 */
ListOfSpeciesFeatureValues*
SpeciesFeature::getListOfSpeciesFeatureValues()
{
  return &mSpeciesFeatureValues;
}


/*
 * Removes the nth SpeciesFeatureValue from the ListOfSpeciesFeatureValues.
 */
SpeciesFeatureValue*
SpeciesFeature::removeSpeciesFeatureValue(unsigned int n)
{
  return mSpeciesFeatureValues.remove(n);
}


/*
 * Removes the a SpeciesFeatureValue with given id from the ListOfSpeciesFeatureValues.
 */
SpeciesFeatureValue*
SpeciesFeature::removeSpeciesFeatureValue(const std::string& sid)
{
  return mSpeciesFeatureValues.remove(sid);
}


/*
 * Return the nth SpeciesFeatureValue in the ListOfSpeciesFeatureValues within this SpeciesFeature.
 */
SpeciesFeatureValue*
SpeciesFeature::getSpeciesFeatureValue(unsigned int n)
{
  return mSpeciesFeatureValues.get(n);
}


/*
 * Return the nth SpeciesFeatureValue in the ListOfSpeciesFeatureValues within this SpeciesFeature.
 */
const SpeciesFeatureValue*
SpeciesFeature::getSpeciesFeatureValue(unsigned int n) const
{
  return mSpeciesFeatureValues.get(n);
}


/*
 * Return a SpeciesFeatureValue from the ListOfSpeciesFeatureValues by id.
 */
SpeciesFeatureValue*
SpeciesFeature::getSpeciesFeatureValue(const std::string& sid)
{
  return mSpeciesFeatureValues.get(sid);
}


/*
 * Return a SpeciesFeatureValue from the ListOfSpeciesFeatureValues by id.
 */
const SpeciesFeatureValue*
SpeciesFeature::getSpeciesFeatureValue(const std::string& sid) const
{
  return mSpeciesFeatureValues.get(sid);
}


/*
 * Adds a copy the given "SpeciesFeatureValue" to this SpeciesFeature.
 */
int
SpeciesFeature::addSpeciesFeatureValue(const SpeciesFeatureValue* sfv)
{
  if (sfv == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (sfv->hasRequiredAttributes() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != sfv->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != sfv->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (matchesRequiredSBMLNamespacesForAddition(static_cast<const SBase *>(sfv)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    mSpeciesFeatureValues.append(sfv);

    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Get the number of SpeciesFeatureValue objects in this SpeciesFeature.
 */
unsigned int
SpeciesFeature::getNumSpeciesFeatureValues() const
{
  return mSpeciesFeatureValues.size();
}


/*
 * Creates a new SpeciesFeatureValue object, adds it to this SpeciesFeatures
 */
SpeciesFeatureValue*
SpeciesFeature::createSpeciesFeatureValue()
{
  SpeciesFeatureValue* sfv = NULL;

  try
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    sfv = new SpeciesFeatureValue(multins);
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

  if(sfv != NULL)
  {
    mSpeciesFeatureValues.appendAndOwn(sfv);
  }

  return sfv;
}


/*
 * rename attributes that are SIdRefs or instances in math
 */
void
SpeciesFeature::renameSIdRefs(const std::string& oldid, const std::string& newid)
{
  if (isSetSpeciesFeatureType() == true && mSpeciesFeatureType == oldid)
  {
    setSpeciesFeatureType(newid);
  }

  if (isSetComponent() == true && mComponent == oldid)
  {
    setComponent(newid);
  }

}


List*
SpeciesFeature::getAllElements(ElementFilter* filter)
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
SpeciesFeature::getElementName () const
{
  static const string name = "speciesFeature";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
SpeciesFeature::getTypeCode () const
{
  return SBML_MULTI_SPECIES_FEATURE;
}


/*
 * check if all the required attributes are set
 */
bool
SpeciesFeature::hasRequiredAttributes () const
{
  bool allPresent = true;

  if (isSetSpeciesFeatureType() == false)
    allPresent = false;

  if (isSetOccur() == false)
    allPresent = false;

  return allPresent;
}


/*
 * check if all the required elements are set
 */
bool
SpeciesFeature::hasRequiredElements () const
{
  bool allPresent = true;

  return allPresent;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * write contained elements
 */
void
SpeciesFeature::writeElements (XMLOutputStream& stream) const
{
  SBase::writeElements(stream);

  if (getNumSpeciesFeatureValues() > 0)
  {
    mSpeciesFeatureValues.write(stream);
  }

  SBase::writeExtensionElements(stream);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Accepts the given SBMLVisitor.
 */
bool
SpeciesFeature::accept (SBMLVisitor& v) const
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
SpeciesFeature::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);
  mSpeciesFeatureValues.setSBMLDocument(d);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
   * Connects to child elements.
 */
void
SpeciesFeature::connectToChild()
{
  mSpeciesFeatureValues.connectToParent(this);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Enables/Disables the given package with this element.
 */
void
SpeciesFeature::enablePackageInternal(const std::string& pkgURI,
             const std::string& pkgPrefix, bool flag)
{
  SBase::enablePackageInternal(pkgURI, pkgPrefix, flag);
  mSpeciesFeatureValues.enablePackageInternal(pkgURI, pkgPrefix, flag);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * creates object.
 */
SBase*
SpeciesFeature::createObject(XMLInputStream& stream)
{
  const string& name = stream.peek().getName();
  SBase* object = NULL;

  MULTI_CREATE_NS(multins, getSBMLNamespaces());

  if (name == "listOfSpeciesFeatureValues")
  {
    object = &mSpeciesFeatureValues;
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
SpeciesFeature::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("speciesFeatureType");
  attributes.add("occur");
  attributes.add("component");
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Read values from the given XMLAttributes set into their specific fields.
 */
void
SpeciesFeature::readAttributes (const XMLAttributes& attributes,
                             const ExpectedAttributes& expectedAttributes)
{
  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  unsigned int numErrs;

  /* look to see whether an unknown attribute error was logged
   * during the read of the listOfSpeciesFeatures - which will have
   * happened immediately prior to this read
  */

  if (getErrorLog() != NULL &&
      static_cast<ListOfSpeciesFeatures*>(getParentSBMLObject())->size() < 2)
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
  // id SId  ( use = "optional" )
  //
  assigned = attributes.readInto("id", mId);

   if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mId.empty() == true)
    {
      logEmptyString(mId, getLevel(), getVersion(), "<SpeciesFeature>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mId) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute id='" + mId + "' does not conform.");
    }
  }

  //
  // speciesFeatureType SIdRef   ( use = "required" )
  //
  assigned = attributes.readInto("speciesFeatureType", mSpeciesFeatureType);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mSpeciesFeatureType.empty() == true)
    {
      logEmptyString(mSpeciesFeatureType, getLevel(), getVersion(), "<SpeciesFeature>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mSpeciesFeatureType) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute speciesFeatureType='" + mSpeciesFeatureType 
        + "' does not conform.");
    }
  }
  else
  {
    std::string message = "Multi attribute 'speciesFeatureType' is missing.";
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
  // component SIdRef   ( use = "optional" )
  //
  assigned = attributes.readInto("component", mComponent);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mComponent.empty() == true)
    {
      logEmptyString(mComponent, getLevel(), getVersion(), "<SpeciesFeature>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mComponent) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute component='" + mComponent + "' does not conform.");
    }
  }

}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Write values of XMLAttributes to the output stream.
 */
  void
SpeciesFeature::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);

  if (isSetId() == true)
    stream.writeAttribute("id", getPrefix(), mId);

  if (isSetSpeciesFeatureType() == true)
    stream.writeAttribute("speciesFeatureType", getPrefix(), mSpeciesFeatureType);

  if (isSetOccur() == true)
    stream.writeAttribute("occur", getPrefix(), mOccur);

  if (isSetComponent() == true)
    stream.writeAttribute("component", getPrefix(), mComponent);

  SBase::writeExtensionAttributes(stream);

}


  /** @endcond doxygenLibsbmlInternal */


/*
 * Constructor 
 */
ListOfSpeciesFeatures::ListOfSpeciesFeatures(unsigned int level, 
                        unsigned int version, 
                        unsigned int pkgVersion)
 : ListOf(level, version)
 , mSubListOfSpeciesFeatures (NULL)
 , mRelation (MULTI_RELATION_UNKNOWN)
 , mIsSubList ( false)
{
  mSubListOfSpeciesFeatures = new List();

  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion)); 

  connectToChild();
}


/*
 * Constructor 
 */
ListOfSpeciesFeatures::ListOfSpeciesFeatures(MultiPkgNamespaces* multins)
  : ListOf(multins)
  , mSubListOfSpeciesFeatures (NULL)
  , mRelation (MULTI_RELATION_UNKNOWN )
  , mIsSubList (false )
{
  mSubListOfSpeciesFeatures = new List();
  setElementNamespace(multins->getURI());
  connectToChild();
}


/*
 * Returns a deep copy of this ListOfSpeciesFeatures 
 */
ListOfSpeciesFeatures* 
ListOfSpeciesFeatures::clone () const
 {
  return new ListOfSpeciesFeatures(*this);
}


/*
 * Get a SpeciesFeature from the ListOfSpeciesFeatures by index.
*/
SpeciesFeature*
ListOfSpeciesFeatures::get(unsigned int n)
{
  return static_cast<SpeciesFeature*>(ListOf::get(n));
}


/*
 * Get a SpeciesFeature from the ListOfSpeciesFeatures by index.
 */
const SpeciesFeature*
ListOfSpeciesFeatures::get(unsigned int n) const
{
  return static_cast<const SpeciesFeature*>(ListOf::get(n));
}


/*
 * Get a SpeciesFeature from the ListOfSpeciesFeatures by id.
 */
SpeciesFeature*
ListOfSpeciesFeatures::get(const std::string& sid)
{
  return const_cast<SpeciesFeature*>(
    static_cast<const ListOfSpeciesFeatures&>(*this).get(sid));
}


/*
 * Get a SpeciesFeature from the ListOfSpeciesFeatures by id.
 */
const SpeciesFeature*
ListOfSpeciesFeatures::get(const std::string& sid) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<SpeciesFeature>(sid) );
  return (result == mItems.end()) ? 0 : static_cast <SpeciesFeature*> (*result);
}


/*
 * Removes the nth SpeciesFeature from this ListOfSpeciesFeatures
 */
SpeciesFeature*
ListOfSpeciesFeatures::remove(unsigned int n)
{
  return static_cast<SpeciesFeature*>(ListOf::remove(n));
}


/*
 * Removes the SpeciesFeature from this ListOfSpeciesFeatures with the given identifier
 */
SpeciesFeature*
ListOfSpeciesFeatures::remove(const std::string& sid)
{
  SBase* item = NULL;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<SpeciesFeature>(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <SpeciesFeature*> (item);
}

int
ListOfSpeciesFeatures::addSubListOfSpeciesFeatures(ListOfSpeciesFeatures* losf)
{
  if (losf == NULL)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else
  {
    unsigned int num = getNumSubListOfSpeciesFeatures();
    static_cast<ListOfSpeciesFeatures*>(losf)->setIsSubList();
    mSubListOfSpeciesFeatures->add(losf);
    connectToChild();
    if (getNumSubListOfSpeciesFeatures() == num + 1)
    {
      return LIBSBML_OPERATION_SUCCESS;
    }
    else
    {
      return LIBSBML_OPERATION_FAILED;
    }
  }

}


unsigned int 
ListOfSpeciesFeatures::getNumSubListOfSpeciesFeatures() const
{
  return mSubListOfSpeciesFeatures->getSize();
}

 /*
 * Returns the value of the "relation" attribute of this ListOfSpeciesFeatures.
 */
Relation_t 
ListOfSpeciesFeatures::getRelation() const
{
  return mRelation;
}


/*
 * Predicate returning @c true or @c false depending on whether this
 * ListOfSpeciesFeatures's "relation" attribute has been set.
 */
bool 
ListOfSpeciesFeatures::isSetRelation() const
{
  return (mRelation != MULTI_RELATION_UNKNOWN);
}


/*
 * Sets the value of the "relation" attribute of this ListOfSpeciesFeatures.
 */
int 
ListOfSpeciesFeatures::setRelation(Relation_t relation)
{
  if (SpeciesFeature_isValidRelation(relation) == 0)
  {
    mRelation = MULTI_RELATION_UNKNOWN;
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mRelation = relation;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Unsets the value of the "relation" attribute of this ListOfSpeciesFeatures.
 */
int 
ListOfSpeciesFeatures::unsetRelation()
{
  mRelation = MULTI_RELATION_UNKNOWN;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Returns the XML element name of this object
 */
const std::string&
ListOfSpeciesFeatures::getElementName () const
{
  static const string name = "listOfSpeciesFeatures";
  static const string name1 = "subListOfSpeciesFeatures";
  if (getIsSubList() == false)
  {
    return name;
  }
  else
  {
    return name1;
  }
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
ListOfSpeciesFeatures::getTypeCode () const
{
  return SBML_LIST_OF;
}


/*
 * Returns the libSBML type code for the objects in this LIST_OF.
 */
int
ListOfSpeciesFeatures::getItemTypeCode () const
{
  return SBML_MULTI_SPECIES_FEATURE;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * Creates a new SpeciesFeature in this ListOfSpeciesFeatures
 */
SBase*
ListOfSpeciesFeatures::createObject(XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase* object = NULL;

  if (name == "speciesFeature")
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    object = new SpeciesFeature(multins);
    appendAndOwn(object);
    delete multins;
  }
  else if ( name == "subListOfSpeciesFeatures")
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    object = new ListOfSpeciesFeatures(multins);
    static_cast<ListOfSpeciesFeatures*>(object)->setIsSubList();
    mSubListOfSpeciesFeatures->add(object);
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
ListOfSpeciesFeatures::writeXMLNS(XMLOutputStream& stream) const
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

/** @cond doxygenLibsbmlInternal */
void
ListOfSpeciesFeatures::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("relation");

}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void 
ListOfSpeciesFeatures::readAttributes (const XMLAttributes& attributes,
                                  const ExpectedAttributes& expectedAttributes)
{
  SBase::readAttributes(attributes, expectedAttributes);
  
  //
  // bindingStatus string   ( use = "optional" )
  //
  std::string relation;
  bool assigned = attributes.readInto("relation", relation, 
                                   getErrorLog(), false);

  if (assigned == true)
  {
    // check string is not empty

    if (relation.empty() == true)
    {
      logEmptyString(relation, getLevel(), getVersion(),
                                    "<ListOfSpeciesFeatures>");
    }
    else 
    {
       mRelation = Relation_fromString( relation.c_str() );
       if (SpeciesFeature_isValidRelation(mRelation) == 0)
       {
          getErrorLog()->logPackageError("multi", MultiUnknownError,
                       getPackageVersion(), getLevel(), getVersion());
       }
    }
  }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void 
ListOfSpeciesFeatures::writeAttributes (XMLOutputStream& stream) const
{
  if (isSetRelation() == true)
    stream.writeAttribute("relation", getPrefix(), Relation_toString(mRelation));
}
/** @endcond */


/*
 * write elements
 */
void
ListOfSpeciesFeatures::writeElements (XMLOutputStream& stream) const
{
  ListOf::writeElements(stream);

  for (unsigned int i = 0; i < mSubListOfSpeciesFeatures->getSize(); i++)
  {
    static_cast<ListOfSpeciesFeatures*>(mSubListOfSpeciesFeatures->get(i))->
      SBase::write(stream);
  }

}


void
ListOfSpeciesFeatures::connectToChild()
{
  for (unsigned int i = 0; i < mSubListOfSpeciesFeatures->getSize(); i++)
  {
    static_cast<SBase*>(mSubListOfSpeciesFeatures->get(i))
      ->connectToParent(this);
  }
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesFeature_t *
SpeciesFeature_create(unsigned int level, unsigned int version,
                      unsigned int pkgVersion)
{
  return new SpeciesFeature(level, version, pkgVersion);
}


/**
 * 
 */
LIBSBML_EXTERN
void
SpeciesFeature_free(SpeciesFeature_t * sf)
{
  if (sf != NULL)
    delete sf;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesFeature_t *
SpeciesFeature_clone(SpeciesFeature_t * sf)
{
  if (sf != NULL)
  {
    return static_cast<SpeciesFeature_t*>(sf->clone());
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
SpeciesFeature_getId(SpeciesFeature_t * sf)
{
  if (sf == NULL)
    return NULL;

  return sf->getId().empty() ? NULL : safe_strdup(sf->getId().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
char *
SpeciesFeature_getSpeciesFeatureType(SpeciesFeature_t * sf)
{
  if (sf == NULL)
    return NULL;

  return sf->getSpeciesFeatureType().empty() ? NULL : safe_strdup(sf->getSpeciesFeatureType().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
unsigned int
SpeciesFeature_getOccur(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? sf->getOccur() : SBML_INT_MAX;
}


/**
 * 
 */
LIBSBML_EXTERN
char *
SpeciesFeature_getComponent(SpeciesFeature_t * sf)
{
  if (sf == NULL)
    return NULL;

  return sf->getComponent().empty() ? NULL : safe_strdup(sf->getComponent().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_isSetId(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? static_cast<int>(sf->isSetId()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_isSetSpeciesFeatureType(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? static_cast<int>(sf->isSetSpeciesFeatureType()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_isSetOccur(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? static_cast<int>(sf->isSetOccur()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_isSetComponent(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? static_cast<int>(sf->isSetComponent()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_setId(SpeciesFeature_t * sf, const char * id)
{
  return (sf != NULL) ? sf->setId(id) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_setSpeciesFeatureType(SpeciesFeature_t * sf, const char * speciesFeatureType)
{
  return (sf != NULL) ? sf->setSpeciesFeatureType(speciesFeatureType) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_setOccur(SpeciesFeature_t * sf, unsigned int occur)
{
  return (sf != NULL) ? sf->setOccur(occur) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_setComponent(SpeciesFeature_t * sf, const char * component)
{
  return (sf != NULL) ? sf->setComponent(component) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_unsetId(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? sf->unsetId() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_unsetSpeciesFeatureType(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? sf->unsetSpeciesFeatureType() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_unsetOccur(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? sf->unsetOccur() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_unsetComponent(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? sf->unsetComponent() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeature_hasRequiredAttributes(SpeciesFeature_t * sf)
{
  return (sf != NULL) ? static_cast<int>(sf->hasRequiredAttributes()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesFeature_t *
ListOfSpeciesFeatures_getById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfSpeciesFeatures *>(lo)->get(sid) : NULL;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesFeature_t *
ListOfSpeciesFeatures_removeById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfSpeciesFeatures *>(lo)->remove(sid) : NULL;
}

static
const char* RELATION_STRINGS[] =
{
    "and"
  , "or"
  , "not"
  , "unknown"
};


LIBSBML_EXTERN
int 
SpeciesFeature_isValidRelation(Relation_t relation)
{
  int max = MULTI_RELATION_UNKNOWN;

  if (relation < MULTI_RELATION_AND || relation >= max)
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
SpeciesFeature_isValidRelationString(const char* s)
{
  return SpeciesFeature_isValidRelation(Relation_fromString(s));
}




LIBSBML_EXTERN
const char* 
Relation_toString(Relation_t relation)
{
  int max = MULTI_RELATION_UNKNOWN;

  if (relation < MULTI_RELATION_AND || relation >= max)
  {
    return NULL;
  }

  return RELATION_STRINGS[relation];
}


LIBSBML_EXTERN
Relation_t 
Relation_fromString(const char* s)
{
  if (s == NULL) 
  {
    return MULTI_RELATION_UNKNOWN;
  }

  int max = MULTI_RELATION_UNKNOWN;
  for (int i = 0; i < max; i++)
  {
    if (strcmp(RELATION_STRINGS[i], s) == 0)
    {
      return (Relation_t)i;
    }
  }
  return MULTI_RELATION_UNKNOWN;
}





LIBSBML_CPP_NAMESPACE_END



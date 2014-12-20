/**
 * @file:   MultiSpeciesType.cpp
 * @brief:  Implementation of the MultiSpeciesType class
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


#include <sbml/packages/multi/sbml/MultiSpeciesType.h>
#include <sbml/packages/multi/validator/MultiSBMLError.h>

#include <sbml/util/ElementFilter.h>


using namespace std;


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new MultiSpeciesType with the given level, version, and package version.
 */
MultiSpeciesType::MultiSpeciesType (unsigned int level, unsigned int version, unsigned int pkgVersion)
  : SBase(level, version)
   ,mId ("")
   ,mName ("")
   ,mIsBindingSite (false)
   ,mIsSetIsBindingSite (false)
   ,mCompartment ("")
   ,mSpeciesFeatureTypes (level, version, pkgVersion)
   ,mSpeciesTypeInstances (level, version, pkgVersion)
   ,mSpeciesTypeComponentIndexes (level, version, pkgVersion)
   ,mInSpeciesTypeBonds (level, version, pkgVersion)
{
  // set an SBMLNamespaces derived object of this package
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion));

  // connect to child objects
  connectToChild();
}


/*
 * Creates a new MultiSpeciesType with the given MultiPkgNamespaces object.
 */
MultiSpeciesType::MultiSpeciesType (MultiPkgNamespaces* multins)
  : SBase(multins)
   ,mId ("")
   ,mName ("")
   ,mIsBindingSite (false)
   ,mIsSetIsBindingSite (false)
   ,mCompartment ("")
   ,mSpeciesFeatureTypes (multins)
   ,mSpeciesTypeInstances (multins)
   ,mSpeciesTypeComponentIndexes (multins)
   ,mInSpeciesTypeBonds (multins)
{
  // set the element namespace of this object
  setElementNamespace(multins->getURI());

  // connect to child objects
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(multins);
}


/*
 * Copy constructor for MultiSpeciesType.
 */
MultiSpeciesType::MultiSpeciesType (const MultiSpeciesType& orig)
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
    mIsBindingSite  = orig.mIsBindingSite;
    mIsSetIsBindingSite  = orig.mIsSetIsBindingSite;
    mCompartment  = orig.mCompartment;
    mSpeciesFeatureTypes  = orig.mSpeciesFeatureTypes;
    mSpeciesTypeInstances  = orig.mSpeciesTypeInstances;
    mSpeciesTypeComponentIndexes  = orig.mSpeciesTypeComponentIndexes;
    mInSpeciesTypeBonds  = orig.mInSpeciesTypeBonds;

    // connect to child objects
    connectToChild();
  }
}


/*
 * Assignment for MultiSpeciesType.
 */
MultiSpeciesType&
MultiSpeciesType::operator=(const MultiSpeciesType& rhs)
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
    mIsBindingSite  = rhs.mIsBindingSite;
    mIsSetIsBindingSite  = rhs.mIsSetIsBindingSite;
    mCompartment  = rhs.mCompartment;
    mSpeciesFeatureTypes  = rhs.mSpeciesFeatureTypes;
    mSpeciesTypeInstances  = rhs.mSpeciesTypeInstances;
    mSpeciesTypeComponentIndexes  = rhs.mSpeciesTypeComponentIndexes;
    mInSpeciesTypeBonds  = rhs.mInSpeciesTypeBonds;

    // connect to child objects
    connectToChild();
  }
  return *this;
}


/*
 * Clone for MultiSpeciesType.
 */
MultiSpeciesType*
MultiSpeciesType::clone () const
{
  return new MultiSpeciesType(*this);
}


/*
 * Destructor for MultiSpeciesType.
 */
MultiSpeciesType::~MultiSpeciesType ()
{
}


/*
 * Returns the value of the "id" attribute of this MultiSpeciesType.
 */
const std::string&
MultiSpeciesType::getId() const
{
  return mId;
}


/*
 * Returns the value of the "name" attribute of this MultiSpeciesType.
 */
const std::string&
MultiSpeciesType::getName() const
{
  return mName;
}


/*
 * Returns the value of the "isBindingSite" attribute of this MultiSpeciesType.
 */
bool
MultiSpeciesType::getIsBindingSite() const
{
  return mIsBindingSite;
}


/*
 * Returns the value of the "compartment" attribute of this MultiSpeciesType.
 */
const std::string&
MultiSpeciesType::getCompartment() const
{
  return mCompartment;
}


/*
 * Returns true/false if id is set.
 */
bool
MultiSpeciesType::isSetId() const
{
  return (mId.empty() == false);
}


/*
 * Returns true/false if name is set.
 */
bool
MultiSpeciesType::isSetName() const
{
  return (mName.empty() == false);
}


/*
 * Returns true/false if isBindingSite is set.
 */
bool
MultiSpeciesType::isSetIsBindingSite() const
{
  return mIsSetIsBindingSite;
}


/*
 * Returns true/false if compartment is set.
 */
bool
MultiSpeciesType::isSetCompartment() const
{
  return (mCompartment.empty() == false);
}


/*
 * Sets id and returns value indicating success.
 */
int
MultiSpeciesType::setId(const std::string& id)
{
  return SyntaxChecker::checkAndSetSId(id, mId);
}


/*
 * Sets name and returns value indicating success.
 */
int
MultiSpeciesType::setName(const std::string& name)
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
 * Sets isBindingSite and returns value indicating success.
 */
int
MultiSpeciesType::setIsBindingSite(bool isBindingSite)
{
  mIsBindingSite = isBindingSite;
  mIsSetIsBindingSite = true;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Sets compartment and returns value indicating success.
 */
int
MultiSpeciesType::setCompartment(const std::string& compartment)
{
  if (&(compartment) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else if (!(SyntaxChecker::isValidInternalSId(compartment)))
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mCompartment = compartment;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Unsets id and returns value indicating success.
 */
int
MultiSpeciesType::unsetId()
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
 * Unsets name and returns value indicating success.
 */
int
MultiSpeciesType::unsetName()
{
  mName.erase();

  if (mName.empty() == true)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Unsets isBindingSite and returns value indicating success.
 */
int
MultiSpeciesType::unsetIsBindingSite()
{
  mIsBindingSite = false;
  mIsSetIsBindingSite = false;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Unsets compartment and returns value indicating success.
 */
int
MultiSpeciesType::unsetCompartment()
{
  mCompartment.erase();

  if (mCompartment.empty() == true)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Returns the  "ListOfSpeciesFeatureTypes" in this MultiSpeciesType object.
 */
const ListOfSpeciesFeatureTypes*
MultiSpeciesType::getListOfSpeciesFeatureTypes() const
{
  return &mSpeciesFeatureTypes;
}


/*
 * Returns the  "ListOfSpeciesFeatureTypes" in this MultiSpeciesType object.
 */
ListOfSpeciesFeatureTypes*
MultiSpeciesType::getListOfSpeciesFeatureTypes()
{
  return &mSpeciesFeatureTypes;
}


/*
 * Removes the nth SpeciesFeatureType from the ListOfSpeciesFeatureTypes.
 */
SpeciesFeatureType*
MultiSpeciesType::removeSpeciesFeatureType(unsigned int n)
{
  return mSpeciesFeatureTypes.remove(n);
}


/*
 * Removes the a SpeciesFeatureType with given id from the ListOfSpeciesFeatureTypes.
 */
SpeciesFeatureType*
MultiSpeciesType::removeSpeciesFeatureType(const std::string& sid)
{
  return mSpeciesFeatureTypes.remove(sid);
}


/*
 * Return the nth SpeciesFeatureType in the ListOfSpeciesFeatureTypes within this MultiSpeciesType.
 */
SpeciesFeatureType*
MultiSpeciesType::getSpeciesFeatureType(unsigned int n)
{
  return mSpeciesFeatureTypes.get(n);
}


/*
 * Return the nth SpeciesFeatureType in the ListOfSpeciesFeatureTypes within this MultiSpeciesType.
 */
const SpeciesFeatureType*
MultiSpeciesType::getSpeciesFeatureType(unsigned int n) const
{
  return mSpeciesFeatureTypes.get(n);
}


/*
 * Return a SpeciesFeatureType from the ListOfSpeciesFeatureTypes by id.
 */
SpeciesFeatureType*
MultiSpeciesType::getSpeciesFeatureType(const std::string& sid)
{
  return mSpeciesFeatureTypes.get(sid);
}


/*
 * Return a SpeciesFeatureType from the ListOfSpeciesFeatureTypes by id.
 */
const SpeciesFeatureType*
MultiSpeciesType::getSpeciesFeatureType(const std::string& sid) const
{
  return mSpeciesFeatureTypes.get(sid);
}


/*
 * Adds a copy the given "SpeciesFeatureType" to this MultiSpeciesType.
 */
int
MultiSpeciesType::addSpeciesFeatureType(const SpeciesFeatureType* sft)
{
  if (sft == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (sft->hasRequiredAttributes() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != sft->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != sft->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (matchesRequiredSBMLNamespacesForAddition(static_cast<const SBase *>(sft)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    mSpeciesFeatureTypes.append(sft);

    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Get the number of SpeciesFeatureType objects in this MultiSpeciesType.
 */
unsigned int
MultiSpeciesType::getNumSpeciesFeatureTypes() const
{
  return mSpeciesFeatureTypes.size();
}


/*
 * Creates a new SpeciesFeatureType object, adds it to this MultiSpeciesTypes
 */
SpeciesFeatureType*
MultiSpeciesType::createSpeciesFeatureType()
{
  SpeciesFeatureType* sft = NULL;

  try
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    sft = new SpeciesFeatureType(multins);
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

  if(sft != NULL)
  {
    mSpeciesFeatureTypes.appendAndOwn(sft);
  }

  return sft;
}


/*
 * Returns the  "ListOfSpeciesTypeInstances" in this MultiSpeciesType object.
 */
const ListOfSpeciesTypeInstances*
MultiSpeciesType::getListOfSpeciesTypeInstances() const
{
  return &mSpeciesTypeInstances;
}


/*
 * Returns the  "ListOfSpeciesTypeInstances" in this MultiSpeciesType object.
 */
ListOfSpeciesTypeInstances*
MultiSpeciesType::getListOfSpeciesTypeInstances()
{
  return &mSpeciesTypeInstances;
}


/*
 * Removes the nth SpeciesTypeInstance from the ListOfSpeciesTypeInstances.
 */
SpeciesTypeInstance*
MultiSpeciesType::removeSpeciesTypeInstance(unsigned int n)
{
  return mSpeciesTypeInstances.remove(n);
}


/*
 * Removes the a SpeciesTypeInstance with given id from the ListOfSpeciesTypeInstances.
 */
SpeciesTypeInstance*
MultiSpeciesType::removeSpeciesTypeInstance(const std::string& sid)
{
  return mSpeciesTypeInstances.remove(sid);
}


/*
 * Return the nth SpeciesTypeInstance in the ListOfSpeciesTypeInstances within this MultiSpeciesType.
 */
SpeciesTypeInstance*
MultiSpeciesType::getSpeciesTypeInstance(unsigned int n)
{
  return mSpeciesTypeInstances.get(n);
}


/*
 * Return the nth SpeciesTypeInstance in the ListOfSpeciesTypeInstances within this MultiSpeciesType.
 */
const SpeciesTypeInstance*
MultiSpeciesType::getSpeciesTypeInstance(unsigned int n) const
{
  return mSpeciesTypeInstances.get(n);
}


/*
 * Return a SpeciesTypeInstance from the ListOfSpeciesTypeInstances by id.
 */
SpeciesTypeInstance*
MultiSpeciesType::getSpeciesTypeInstance(const std::string& sid)
{
  return mSpeciesTypeInstances.get(sid);
}


/*
 * Return a SpeciesTypeInstance from the ListOfSpeciesTypeInstances by id.
 */
const SpeciesTypeInstance*
MultiSpeciesType::getSpeciesTypeInstance(const std::string& sid) const
{
  return mSpeciesTypeInstances.get(sid);
}


/*
 * Adds a copy the given "SpeciesTypeInstance" to this MultiSpeciesType.
 */
int
MultiSpeciesType::addSpeciesTypeInstance(const SpeciesTypeInstance* sti)
{
  if (sti == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (sti->hasRequiredAttributes() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != sti->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != sti->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (matchesRequiredSBMLNamespacesForAddition(static_cast<const SBase *>(sti)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    mSpeciesTypeInstances.append(sti);

    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Get the number of SpeciesTypeInstance objects in this MultiSpeciesType.
 */
unsigned int
MultiSpeciesType::getNumSpeciesTypeInstances() const
{
  return mSpeciesTypeInstances.size();
}


/*
 * Creates a new SpeciesTypeInstance object, adds it to this MultiSpeciesTypes
 */
SpeciesTypeInstance*
MultiSpeciesType::createSpeciesTypeInstance()
{
  SpeciesTypeInstance* sti = NULL;

  try
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    sti = new SpeciesTypeInstance(multins);
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

  if(sti != NULL)
  {
    mSpeciesTypeInstances.appendAndOwn(sti);
  }

  return sti;
}


/*
 * Returns the  "ListOfSpeciesTypeComponentIndexes" in this MultiSpeciesType object.
 */
const ListOfSpeciesTypeComponentIndexes*
MultiSpeciesType::getListOfSpeciesTypeComponentIndexes() const
{
  return &mSpeciesTypeComponentIndexes;
}


/*
 * Returns the  "ListOfSpeciesTypeComponentIndexes" in this MultiSpeciesType object.
 */
ListOfSpeciesTypeComponentIndexes*
MultiSpeciesType::getListOfSpeciesTypeComponentIndexes()
{
  return &mSpeciesTypeComponentIndexes;
}


/*
 * Removes the nth SpeciesTypeComponentIndex from the ListOfSpeciesTypeComponentIndexes.
 */
SpeciesTypeComponentIndex*
MultiSpeciesType::removeSpeciesTypeComponentIndex(unsigned int n)
{
  return mSpeciesTypeComponentIndexes.remove(n);
}


/*
 * Removes the a SpeciesTypeComponentIndex with given id from the ListOfSpeciesTypeComponentIndexes.
 */
SpeciesTypeComponentIndex*
MultiSpeciesType::removeSpeciesTypeComponentIndex(const std::string& sid)
{
  return mSpeciesTypeComponentIndexes.remove(sid);
}


/*
 * Return the nth SpeciesTypeComponentIndex in the ListOfSpeciesTypeComponentIndexes within this MultiSpeciesType.
 */
SpeciesTypeComponentIndex*
MultiSpeciesType::getSpeciesTypeComponentIndex(unsigned int n)
{
  return mSpeciesTypeComponentIndexes.get(n);
}


/*
 * Return the nth SpeciesTypeComponentIndex in the ListOfSpeciesTypeComponentIndexes within this MultiSpeciesType.
 */
const SpeciesTypeComponentIndex*
MultiSpeciesType::getSpeciesTypeComponentIndex(unsigned int n) const
{
  return mSpeciesTypeComponentIndexes.get(n);
}


/*
 * Return a SpeciesTypeComponentIndex from the ListOfSpeciesTypeComponentIndexes by id.
 */
SpeciesTypeComponentIndex*
MultiSpeciesType::getSpeciesTypeComponentIndex(const std::string& sid)
{
  return mSpeciesTypeComponentIndexes.get(sid);
}


/*
 * Return a SpeciesTypeComponentIndex from the ListOfSpeciesTypeComponentIndexes by id.
 */
const SpeciesTypeComponentIndex*
MultiSpeciesType::getSpeciesTypeComponentIndex(const std::string& sid) const
{
  return mSpeciesTypeComponentIndexes.get(sid);
}


/*
 * Adds a copy the given "SpeciesTypeComponentIndex" to this MultiSpeciesType.
 */
int
MultiSpeciesType::addSpeciesTypeComponentIndex(const SpeciesTypeComponentIndex* stci)
{
  if (stci == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (stci->hasRequiredAttributes() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != stci->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != stci->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (matchesRequiredSBMLNamespacesForAddition(static_cast<const SBase *>(stci)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    mSpeciesTypeComponentIndexes.append(stci);

    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Get the number of SpeciesTypeComponentIndex objects in this MultiSpeciesType.
 */
unsigned int
MultiSpeciesType::getNumSpeciesTypeComponentIndexes() const
{
  return mSpeciesTypeComponentIndexes.size();
}


/*
 * Creates a new SpeciesTypeComponentIndex object, adds it to this MultiSpeciesTypes
 */
SpeciesTypeComponentIndex*
MultiSpeciesType::createSpeciesTypeComponentIndex()
{
  SpeciesTypeComponentIndex* stci = NULL;

  try
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    stci = new SpeciesTypeComponentIndex(multins);
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

  if(stci != NULL)
  {
    mSpeciesTypeComponentIndexes.appendAndOwn(stci);
  }

  return stci;
}


/*
 * Returns the  "ListOfInSpeciesTypeBonds" in this MultiSpeciesType object.
 */
const ListOfInSpeciesTypeBonds*
MultiSpeciesType::getListOfInSpeciesTypeBonds() const
{
  return &mInSpeciesTypeBonds;
}


/*
 * Returns the  "ListOfInSpeciesTypeBonds" in this MultiSpeciesType object.
 */
ListOfInSpeciesTypeBonds*
MultiSpeciesType::getListOfInSpeciesTypeBonds()
{
  return &mInSpeciesTypeBonds;
}


/*
 * Removes the nth InSpeciesTypeBond from the ListOfInSpeciesTypeBonds.
 */
InSpeciesTypeBond*
MultiSpeciesType::removeInSpeciesTypeBond(unsigned int n)
{
  return mInSpeciesTypeBonds.remove(n);
}


/*
 * Removes the a InSpeciesTypeBond with given id from the ListOfInSpeciesTypeBonds.
 */
InSpeciesTypeBond*
MultiSpeciesType::removeInSpeciesTypeBond(const std::string& sid)
{
  return mInSpeciesTypeBonds.remove(sid);
}


/*
 * Return the nth InSpeciesTypeBond in the ListOfInSpeciesTypeBonds within this MultiSpeciesType.
 */
InSpeciesTypeBond*
MultiSpeciesType::getInSpeciesTypeBond(unsigned int n)
{
  return mInSpeciesTypeBonds.get(n);
}


/*
 * Return the nth InSpeciesTypeBond in the ListOfInSpeciesTypeBonds within this MultiSpeciesType.
 */
const InSpeciesTypeBond*
MultiSpeciesType::getInSpeciesTypeBond(unsigned int n) const
{
  return mInSpeciesTypeBonds.get(n);
}


/*
 * Return a InSpeciesTypeBond from the ListOfInSpeciesTypeBonds by id.
 */
InSpeciesTypeBond*
MultiSpeciesType::getInSpeciesTypeBond(const std::string& sid)
{
  return mInSpeciesTypeBonds.get(sid);
}


/*
 * Return a InSpeciesTypeBond from the ListOfInSpeciesTypeBonds by id.
 */
const InSpeciesTypeBond*
MultiSpeciesType::getInSpeciesTypeBond(const std::string& sid) const
{
  return mInSpeciesTypeBonds.get(sid);
}


/*
 * Adds a copy the given "InSpeciesTypeBond" to this MultiSpeciesType.
 */
int
MultiSpeciesType::addInSpeciesTypeBond(const InSpeciesTypeBond* istb)
{
  if (istb == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (istb->hasRequiredAttributes() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != istb->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != istb->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (matchesRequiredSBMLNamespacesForAddition(static_cast<const SBase *>(istb)) == false)
  {
    return LIBSBML_NAMESPACES_MISMATCH;
  }
  else
  {
    mInSpeciesTypeBonds.append(istb);

    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Get the number of InSpeciesTypeBond objects in this MultiSpeciesType.
 */
unsigned int
MultiSpeciesType::getNumInSpeciesTypeBonds() const
{
  return mInSpeciesTypeBonds.size();
}


/*
 * Creates a new InSpeciesTypeBond object, adds it to this MultiSpeciesTypes
 */
InSpeciesTypeBond*
MultiSpeciesType::createInSpeciesTypeBond()
{
  InSpeciesTypeBond* istb = NULL;

  try
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    istb = new InSpeciesTypeBond(multins);
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

  if(istb != NULL)
  {
    mInSpeciesTypeBonds.appendAndOwn(istb);
  }

  return istb;
}


/*
 * rename attributes that are SIdRefs or instances in math
 */
void
MultiSpeciesType::renameSIdRefs(const std::string& oldid, const std::string& newid)
{
  if (isSetCompartment() == true && mCompartment == oldid)
  {
    setCompartment(newid);
  }

}


List*
MultiSpeciesType::getAllElements(ElementFilter* filter)
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
MultiSpeciesType::getElementName () const
{
  static const string name = "speciesType";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
MultiSpeciesType::getTypeCode () const
{
  return SBML_MULTI_SPECIES_TYPE;
}


/*
 * check if all the required attributes are set
 */
bool
MultiSpeciesType::hasRequiredAttributes () const
{
  bool allPresent = true;

  if (isSetId() == false)
    allPresent = false;

  if (isSetIsBindingSite() == false)
    allPresent = false;

  return allPresent;
}


/*
 * check if all the required elements are set
 */
bool
MultiSpeciesType::hasRequiredElements () const
{
  bool allPresent = true;

  return allPresent;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * write contained elements
 */
void
MultiSpeciesType::writeElements (XMLOutputStream& stream) const
{
  SBase::writeElements(stream);

  if (getNumSpeciesFeatureTypes() > 0)
  {
    mSpeciesFeatureTypes.write(stream);
  }

  if (getNumSpeciesTypeInstances() > 0)
  {
    mSpeciesTypeInstances.write(stream);
  }

  if (getNumSpeciesTypeComponentIndexes() > 0)
  {
    mSpeciesTypeComponentIndexes.write(stream);
  }

  if (getNumInSpeciesTypeBonds() > 0)
  {
    mInSpeciesTypeBonds.write(stream);
  }

  SBase::writeExtensionElements(stream);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Accepts the given SBMLVisitor.
 */
bool
MultiSpeciesType::accept (SBMLVisitor& v) const
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
MultiSpeciesType::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);
  mSpeciesFeatureTypes.setSBMLDocument(d);
  mSpeciesTypeInstances.setSBMLDocument(d);
  mSpeciesTypeComponentIndexes.setSBMLDocument(d);
  mInSpeciesTypeBonds.setSBMLDocument(d);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
   * Connects to child elements.
 */
void
MultiSpeciesType::connectToChild()
{
  mSpeciesFeatureTypes.connectToParent(this);
  mSpeciesTypeInstances.connectToParent(this);
  mSpeciesTypeComponentIndexes.connectToParent(this);
  mInSpeciesTypeBonds.connectToParent(this);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Enables/Disables the given package with this element.
 */
void
MultiSpeciesType::enablePackageInternal(const std::string& pkgURI,
             const std::string& pkgPrefix, bool flag)
{
  SBase::enablePackageInternal(pkgURI, pkgPrefix, flag);
  mSpeciesFeatureTypes.enablePackageInternal(pkgURI, pkgPrefix, flag);
  mSpeciesTypeInstances.enablePackageInternal(pkgURI, pkgPrefix, flag);
  mSpeciesTypeComponentIndexes.enablePackageInternal(pkgURI, pkgPrefix, flag);
  mInSpeciesTypeBonds.enablePackageInternal(pkgURI, pkgPrefix, flag);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * creates object.
 */
SBase*
MultiSpeciesType::createObject(XMLInputStream& stream)
{
  const string& name = stream.peek().getName();
  SBase* object = NULL;

  MULTI_CREATE_NS(multins, getSBMLNamespaces());

  if (name == "listOfSpeciesFeatureTypes")
  {
    object = &mSpeciesFeatureTypes;
  }
  else if (name == "listOfSpeciesTypeInstances")
  {
    object = &mSpeciesTypeInstances;
  }
  else if (name == "listOfSpeciesTypeComponentIndexes")
  {
    object = &mSpeciesTypeComponentIndexes;
  }
  else if (name == "listOfInSpeciesTypeBonds")
  {
    object = &mInSpeciesTypeBonds;
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
MultiSpeciesType::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("name");
  attributes.add("isBindingSite");
  attributes.add("compartment");
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Read values from the given XMLAttributes set into their specific fields.
 */
void
MultiSpeciesType::readAttributes (const XMLAttributes& attributes,
                             const ExpectedAttributes& expectedAttributes)
{
  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  unsigned int numErrs;

  /* look to see whether an unknown attribute error was logged
   * during the read of the listOfMultiSpeciesTypes - which will have
   * happened immediately prior to this read
  */

  if (getErrorLog() != NULL &&
      static_cast<ListOfMultiSpeciesTypes*>(getParentSBMLObject())->size() < 2)
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
      logEmptyString(mId, getLevel(), getVersion(), "<MultiSpeciesType>");
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
  // name string   ( use = "optional" )
  //
  assigned = attributes.readInto("name", mName);

  if (assigned == true)
  {
    // check string is not empty

    if (mName.empty() == true)
    {
      logEmptyString(mName, getLevel(), getVersion(), "<MultiSpeciesType>");
    }
  }

  //
  // isBindingSite bool   ( use = "required" )
  //
  numErrs = getErrorLog()->getNumErrors();
  mIsSetIsBindingSite = attributes.readInto("isBindingSite", mIsBindingSite);

  if (mIsSetIsBindingSite == false)
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
        std::string message = "Multi attribute 'isBindingSite' is missing.";
        getErrorLog()->logPackageError("multi", MultiUnknownError,
                       getPackageVersion(), sbmlLevel, sbmlVersion, message);
      }
    }
  }

  //
  // compartment SIdRef   ( use = "optional" )
  //
  assigned = attributes.readInto("compartment", mCompartment);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mCompartment.empty() == true)
    {
      logEmptyString(mCompartment, getLevel(), getVersion(), "<MultiSpeciesType>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mCompartment) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute compartment='" + mCompartment + "' does not conform.");
    }
  }

}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Write values of XMLAttributes to the output stream.
 */
  void
MultiSpeciesType::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);

  if (isSetId() == true)
    stream.writeAttribute("id", getPrefix(), mId);

  if (isSetName() == true)
    stream.writeAttribute("name", getPrefix(), mName);

  if (isSetIsBindingSite() == true)
    stream.writeAttribute("isBindingSite", getPrefix(), mIsBindingSite);

  if (isSetCompartment() == true)
    stream.writeAttribute("compartment", getPrefix(), mCompartment);

  SBase::writeExtensionAttributes(stream);

}


  /** @endcond doxygenLibsbmlInternal */


/*
 * Constructor 
 */
ListOfMultiSpeciesTypes::ListOfMultiSpeciesTypes(unsigned int level, 
                          unsigned int version, 
                          unsigned int pkgVersion)
 : ListOf(level, version)
{
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion)); 
}


/*
 * Constructor 
 */
ListOfMultiSpeciesTypes::ListOfMultiSpeciesTypes(MultiPkgNamespaces* multins)
  : ListOf(multins)
{
  setElementNamespace(multins->getURI());
}


/*
 * Returns a deep copy of this ListOfMultiSpeciesTypes 
 */
ListOfMultiSpeciesTypes* 
ListOfMultiSpeciesTypes::clone () const
 {
  return new ListOfMultiSpeciesTypes(*this);
}


/*
 * Get a MultiSpeciesType from the ListOfMultiSpeciesTypes by index.
*/
MultiSpeciesType*
ListOfMultiSpeciesTypes::get(unsigned int n)
{
  return static_cast<MultiSpeciesType*>(ListOf::get(n));
}


/*
 * Get a MultiSpeciesType from the ListOfMultiSpeciesTypes by index.
 */
const MultiSpeciesType*
ListOfMultiSpeciesTypes::get(unsigned int n) const
{
  return static_cast<const MultiSpeciesType*>(ListOf::get(n));
}


/*
 * Get a MultiSpeciesType from the ListOfMultiSpeciesTypes by id.
 */
MultiSpeciesType*
ListOfMultiSpeciesTypes::get(const std::string& sid)
{
  return const_cast<MultiSpeciesType*>(
    static_cast<const ListOfMultiSpeciesTypes&>(*this).get(sid));
}


/*
 * Get a MultiSpeciesType from the ListOfMultiSpeciesTypes by id.
 */
const MultiSpeciesType*
ListOfMultiSpeciesTypes::get(const std::string& sid) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<MultiSpeciesType>(sid) );
  return (result == mItems.end()) ? 0 : static_cast <MultiSpeciesType*> (*result);
}


/*
 * Removes the nth MultiSpeciesType from this ListOfMultiSpeciesTypes
 */
MultiSpeciesType*
ListOfMultiSpeciesTypes::remove(unsigned int n)
{
  return static_cast<MultiSpeciesType*>(ListOf::remove(n));
}


/*
 * Removes the MultiSpeciesType from this ListOfMultiSpeciesTypes with the given identifier
 */
MultiSpeciesType*
ListOfMultiSpeciesTypes::remove(const std::string& sid)
{
  SBase* item = NULL;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<MultiSpeciesType>(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <MultiSpeciesType*> (item);
}


/*
 * Returns the XML element name of this object
 */
const std::string&
ListOfMultiSpeciesTypes::getElementName () const
{
  static const string name = "listOfSpeciesTypes";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
ListOfMultiSpeciesTypes::getTypeCode () const
{
  return SBML_LIST_OF;
}


/*
 * Returns the libSBML type code for the objects in this LIST_OF.
 */
int
ListOfMultiSpeciesTypes::getItemTypeCode () const
{
  return SBML_MULTI_SPECIES_TYPE;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * Creates a new MultiSpeciesType in this ListOfMultiSpeciesTypes
 */
SBase*
ListOfMultiSpeciesTypes::createObject(XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase* object = NULL;

  if (name == "speciesType")
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    object = new MultiSpeciesType(multins);
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
ListOfMultiSpeciesTypes::writeXMLNS(XMLOutputStream& stream) const
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


/*
 * 
 */
LIBSBML_EXTERN
MultiSpeciesType_t *
MultiSpeciesType_create(unsigned int level, unsigned int version,
                        unsigned int pkgVersion)
{
  return new MultiSpeciesType(level, version, pkgVersion);
}


/*
 * 
 */
LIBSBML_EXTERN
void
MultiSpeciesType_free(MultiSpeciesType_t * mst)
{
  if (mst != NULL)
    delete mst;
}


/*
 *
 */
LIBSBML_EXTERN
MultiSpeciesType_t *
MultiSpeciesType_clone(MultiSpeciesType_t * mst)
{
  if (mst != NULL)
  {
    return static_cast<MultiSpeciesType_t*>(mst->clone());
  }
  else
  {
    return NULL;
  }
}


/*
 *
 */
LIBSBML_EXTERN
char *
MultiSpeciesType_getId(MultiSpeciesType_t * mst)
{
  if (mst == NULL)
    return NULL;

  return mst->getId().empty() ? NULL : safe_strdup(mst->getId().c_str());
}


/*
 *
 */
LIBSBML_EXTERN
char *
MultiSpeciesType_getName(MultiSpeciesType_t * mst)
{
  if (mst == NULL)
    return NULL;

  return mst->getName().empty() ? NULL : safe_strdup(mst->getName().c_str());
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_getIsBindingSite(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? static_cast<int>(mst->getIsBindingSite()) : 0;
}


/*
 *
 */
LIBSBML_EXTERN
char *
MultiSpeciesType_getCompartment(MultiSpeciesType_t * mst)
{
  if (mst == NULL)
    return NULL;

  return mst->getCompartment().empty() ? NULL : safe_strdup(mst->getCompartment().c_str());
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_isSetId(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? static_cast<int>(mst->isSetId()) : 0;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_isSetName(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? static_cast<int>(mst->isSetName()) : 0;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_isSetIsBindingSite(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? static_cast<int>(mst->isSetIsBindingSite()) : 0;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_isSetCompartment(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? static_cast<int>(mst->isSetCompartment()) : 0;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_setId(MultiSpeciesType_t * mst, const char * id)
{
  return (mst != NULL) ? mst->setId(id) : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_setName(MultiSpeciesType_t * mst, const char * name)
{
  return (mst != NULL) ? mst->setName(name) : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_setIsBindingSite(MultiSpeciesType_t * mst, int isBindingSite)
{
  return (mst != NULL) ? mst->setIsBindingSite(isBindingSite) : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_setCompartment(MultiSpeciesType_t * mst, const char * compartment)
{
  return (mst != NULL) ? mst->setCompartment(compartment) : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_unsetId(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? mst->unsetId() : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_unsetName(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? mst->unsetName() : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_unsetIsBindingSite(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? mst->unsetIsBindingSite() : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_unsetCompartment(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? mst->unsetCompartment() : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
MultiSpeciesType_hasRequiredAttributes(MultiSpeciesType_t * mst)
{
  return (mst != NULL) ? static_cast<int>(mst->hasRequiredAttributes()) : 0;
}


/*
 *
 */
LIBSBML_EXTERN
MultiSpeciesType_t *
ListOfMultiSpeciesTypes_getById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfMultiSpeciesTypes *>(lo)->get(sid) : NULL;
}


/*
 *
 */
LIBSBML_EXTERN
MultiSpeciesType_t *
ListOfMultiSpeciesTypes_removeById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfMultiSpeciesTypes *>(lo)->remove(sid) : NULL;
}




LIBSBML_CPP_NAMESPACE_END



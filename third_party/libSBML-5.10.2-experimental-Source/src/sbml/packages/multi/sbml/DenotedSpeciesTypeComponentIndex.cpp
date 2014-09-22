/**
 * @file:   DenotedSpeciesTypeComponentIndex.cpp
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
 */


#include <sbml/packages/multi/sbml/DenotedSpeciesTypeComponentIndex.h>
#include <sbml/packages/multi/validator/MultiSBMLError.h>


using namespace std;


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new DenotedSpeciesTypeComponentIndex with the given level, version, and package version.
 */
DenotedSpeciesTypeComponentIndex::DenotedSpeciesTypeComponentIndex (unsigned int level, unsigned int version, unsigned int pkgVersion)
  : SBase(level, version)
   ,mSpeciesTypeComponentIndex ("")
{
  // set an SBMLNamespaces derived object of this package
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion));
}


/*
 * Creates a new DenotedSpeciesTypeComponentIndex with the given MultiPkgNamespaces object.
 */
DenotedSpeciesTypeComponentIndex::DenotedSpeciesTypeComponentIndex (MultiPkgNamespaces* multins)
  : SBase(multins)
   ,mSpeciesTypeComponentIndex ("")
{
  // set the element namespace of this object
  setElementNamespace(multins->getURI());

  // load package extensions bound with this object (if any) 
  loadPlugins(multins);
}


/*
 * Copy constructor for DenotedSpeciesTypeComponentIndex.
 */
DenotedSpeciesTypeComponentIndex::DenotedSpeciesTypeComponentIndex (const DenotedSpeciesTypeComponentIndex& orig)
  : SBase(orig)
{
  if (&orig == NULL)
  {
    throw SBMLConstructorException("Null argument to copy constructor");
  }
  else
  {
    mSpeciesTypeComponentIndex  = orig.mSpeciesTypeComponentIndex;
  }
}


/*
 * Assignment for DenotedSpeciesTypeComponentIndex.
 */
DenotedSpeciesTypeComponentIndex&
DenotedSpeciesTypeComponentIndex::operator=(const DenotedSpeciesTypeComponentIndex& rhs)
{
  if (&rhs == NULL)
  {
    throw SBMLConstructorException("Null argument to assignment");
  }
  else if (&rhs != this)
  {
    SBase::operator=(rhs);
    mSpeciesTypeComponentIndex  = rhs.mSpeciesTypeComponentIndex;
  }
  return *this;
}


/*
 * Clone for DenotedSpeciesTypeComponentIndex.
 */
DenotedSpeciesTypeComponentIndex*
DenotedSpeciesTypeComponentIndex::clone () const
{
  return new DenotedSpeciesTypeComponentIndex(*this);
}


/*
 * Destructor for DenotedSpeciesTypeComponentIndex.
 */
DenotedSpeciesTypeComponentIndex::~DenotedSpeciesTypeComponentIndex ()
{
}


/*
 * Returns the value of the "speciesTypeComponentIndex" attribute of this DenotedSpeciesTypeComponentIndex.
 */
const std::string&
DenotedSpeciesTypeComponentIndex::getSpeciesTypeComponentIndex() const
{
  return mSpeciesTypeComponentIndex;
}


/*
 * Returns true/false if speciesTypeComponentIndex is set.
 */
bool
DenotedSpeciesTypeComponentIndex::isSetSpeciesTypeComponentIndex() const
{
  return (mSpeciesTypeComponentIndex.empty() == false);
}


/*
 * Sets speciesTypeComponentIndex and returns value indicating success.
 */
int
DenotedSpeciesTypeComponentIndex::setSpeciesTypeComponentIndex(const std::string& speciesTypeComponentIndex)
{
  if (&(speciesTypeComponentIndex) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else if (!(SyntaxChecker::isValidInternalSId(speciesTypeComponentIndex)))
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mSpeciesTypeComponentIndex = speciesTypeComponentIndex;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Unsets speciesTypeComponentIndex and returns value indicating success.
 */
int
DenotedSpeciesTypeComponentIndex::unsetSpeciesTypeComponentIndex()
{
  mSpeciesTypeComponentIndex.erase();

  if (mSpeciesTypeComponentIndex.empty() == true)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * rename attributes that are SIdRefs or instances in math
 */
void
DenotedSpeciesTypeComponentIndex::renameSIdRefs(const std::string& oldid, const std::string& newid)
{
  if (isSetSpeciesTypeComponentIndex() == true && mSpeciesTypeComponentIndex == oldid)
  {
    setSpeciesTypeComponentIndex(newid);
  }

}


/*
 * Returns the XML element name of this object
 */
const std::string&
DenotedSpeciesTypeComponentIndex::getElementName () const
{
  static const string name = "denotedSpeciesTypeComponentIndex";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
DenotedSpeciesTypeComponentIndex::getTypeCode () const
{
  return SBML_MULTI_DENOTED_SPECIES_TYPE_COMPONENT_INDEX;
}


/*
 * check if all the required attributes are set
 */
bool
DenotedSpeciesTypeComponentIndex::hasRequiredAttributes () const
{
  bool allPresent = true;

  if (isSetSpeciesTypeComponentIndex() == false)
    allPresent = false;

  return allPresent;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * write contained elements
 */
void
DenotedSpeciesTypeComponentIndex::writeElements (XMLOutputStream& stream) const
{
  SBase::writeElements(stream);

  SBase::writeExtensionElements(stream);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Accepts the given SBMLVisitor.
 */
bool
DenotedSpeciesTypeComponentIndex::accept (SBMLVisitor& v) const
{
  return v.visit(*this);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Sets the parent SBMLDocument.
 */
void
DenotedSpeciesTypeComponentIndex::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Enables/Disables the given package with this element.
 */
void
DenotedSpeciesTypeComponentIndex::enablePackageInternal(const std::string& pkgURI,
             const std::string& pkgPrefix, bool flag)
{
  SBase::enablePackageInternal(pkgURI, pkgPrefix, flag);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Get the list of expected attributes for this element.
 */
void
DenotedSpeciesTypeComponentIndex::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("speciesTypeComponentIndex");
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Read values from the given XMLAttributes set into their specific fields.
 */
void
DenotedSpeciesTypeComponentIndex::readAttributes (const XMLAttributes& attributes,
                             const ExpectedAttributes& expectedAttributes)
{
  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  unsigned int numErrs;

  /* look to see whether an unknown attribute error was logged
   * during the read of the listOfDenotedSpeciesTypeComponentIndexes - which will have
   * happened immediately prior to this read
  */

  if (getErrorLog() != NULL &&
      static_cast<ListOfDenotedSpeciesTypeComponentIndexes*>(getParentSBMLObject())->size() < 2)
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
  // speciesTypeComponentIndex SIdRef   ( use = "required" )
  //
  assigned = attributes.readInto("speciesTypeComponentIndex", mSpeciesTypeComponentIndex);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mSpeciesTypeComponentIndex.empty() == true)
    {
      logEmptyString(mSpeciesTypeComponentIndex, getLevel(), getVersion(), "<DenotedSpeciesTypeComponentIndex>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mSpeciesTypeComponentIndex) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute speciesTypeComponentIndex='" 
        + mSpeciesTypeComponentIndex + "' does not conform.");
    }
  }
  else
  {
    std::string message = "Multi attribute 'speciesTypeComponentIndex' is missing.";
    getErrorLog()->logPackageError("multi", MultiUnknownError,
                   getPackageVersion(), sbmlLevel, sbmlVersion, message);
  }

}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Write values of XMLAttributes to the output stream.
 */
  void
DenotedSpeciesTypeComponentIndex::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);

  if (isSetSpeciesTypeComponentIndex() == true)
    stream.writeAttribute("speciesTypeComponentIndex", getPrefix(), mSpeciesTypeComponentIndex);

  SBase::writeExtensionAttributes(stream);

}


  /** @endcond doxygenLibsbmlInternal */


/*
 * Constructor 
 */
ListOfDenotedSpeciesTypeComponentIndexes::ListOfDenotedSpeciesTypeComponentIndexes(unsigned int level, 
                                          unsigned int version, 
                                          unsigned int pkgVersion)
 : ListOf(level, version)
{
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion)); 
}


/*
 * Constructor 
 */
ListOfDenotedSpeciesTypeComponentIndexes::ListOfDenotedSpeciesTypeComponentIndexes(MultiPkgNamespaces* multins)
  : ListOf(multins)
{
  setElementNamespace(multins->getURI());
}


/*
 * Returns a deep copy of this ListOfDenotedSpeciesTypeComponentIndexes 
 */
ListOfDenotedSpeciesTypeComponentIndexes* 
ListOfDenotedSpeciesTypeComponentIndexes::clone () const
 {
  return new ListOfDenotedSpeciesTypeComponentIndexes(*this);
}


/*
 * Get a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes by index.
*/
DenotedSpeciesTypeComponentIndex*
ListOfDenotedSpeciesTypeComponentIndexes::get(unsigned int n)
{
  return static_cast<DenotedSpeciesTypeComponentIndex*>(ListOf::get(n));
}


/*
 * Get a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes by index.
 */
const DenotedSpeciesTypeComponentIndex*
ListOfDenotedSpeciesTypeComponentIndexes::get(unsigned int n) const
{
  return static_cast<const DenotedSpeciesTypeComponentIndex*>(ListOf::get(n));
}


/*
 * Get a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes by id.
 */
DenotedSpeciesTypeComponentIndex*
ListOfDenotedSpeciesTypeComponentIndexes::get(const std::string& sid)
{
  return const_cast<DenotedSpeciesTypeComponentIndex*>(
    static_cast<const ListOfDenotedSpeciesTypeComponentIndexes&>(*this).get(sid));
}


/*
 * Get a DenotedSpeciesTypeComponentIndex from the ListOfDenotedSpeciesTypeComponentIndexes by id.
 */
const DenotedSpeciesTypeComponentIndex*
ListOfDenotedSpeciesTypeComponentIndexes::get(const std::string& sid) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<DenotedSpeciesTypeComponentIndex>(sid) );
  return (result == mItems.end()) ? 0 : static_cast <DenotedSpeciesTypeComponentIndex*> (*result);
}


/*
 * Removes the nth DenotedSpeciesTypeComponentIndex from this ListOfDenotedSpeciesTypeComponentIndexes
 */
DenotedSpeciesTypeComponentIndex*
ListOfDenotedSpeciesTypeComponentIndexes::remove(unsigned int n)
{
  return static_cast<DenotedSpeciesTypeComponentIndex*>(ListOf::remove(n));
}


/*
 * Removes the DenotedSpeciesTypeComponentIndex from this ListOfDenotedSpeciesTypeComponentIndexes with the given identifier
 */
DenotedSpeciesTypeComponentIndex*
ListOfDenotedSpeciesTypeComponentIndexes::remove(const std::string& sid)
{
  SBase* item = NULL;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<DenotedSpeciesTypeComponentIndex>(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <DenotedSpeciesTypeComponentIndex*> (item);
}


/*
 * Returns the XML element name of this object
 */
const std::string&
ListOfDenotedSpeciesTypeComponentIndexes::getElementName () const
{
  static const string name = "listOfDenotedSpeciesTypeComponentIndexes";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
ListOfDenotedSpeciesTypeComponentIndexes::getTypeCode () const
{
  return SBML_LIST_OF;
}


/*
 * Returns the libSBML type code for the objects in this LIST_OF.
 */
int
ListOfDenotedSpeciesTypeComponentIndexes::getItemTypeCode () const
{
  return SBML_MULTI_DENOTED_SPECIES_TYPE_COMPONENT_INDEX;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * Creates a new DenotedSpeciesTypeComponentIndex in this ListOfDenotedSpeciesTypeComponentIndexes
 */
SBase*
ListOfDenotedSpeciesTypeComponentIndexes::createObject(XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase* object = NULL;

  if (name == "denotedSpeciesTypeComponentIndex")
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    object = new DenotedSpeciesTypeComponentIndex(multins);
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
ListOfDenotedSpeciesTypeComponentIndexes::writeXMLNS(XMLOutputStream& stream) const
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
DenotedSpeciesTypeComponentIndex_t *
DenotedSpeciesTypeComponentIndex_create(unsigned int level, unsigned int version,
                                        unsigned int pkgVersion)
{
  return new DenotedSpeciesTypeComponentIndex(level, version, pkgVersion);
}


/*
 * 
 */
LIBSBML_EXTERN
void
DenotedSpeciesTypeComponentIndex_free(DenotedSpeciesTypeComponentIndex_t * dstci)
{
  if (dstci != NULL)
    delete dstci;
}


/*
 *
 */
LIBSBML_EXTERN
DenotedSpeciesTypeComponentIndex_t *
DenotedSpeciesTypeComponentIndex_clone(DenotedSpeciesTypeComponentIndex_t * dstci)
{
  if (dstci != NULL)
  {
    return static_cast<DenotedSpeciesTypeComponentIndex_t*>(dstci->clone());
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
DenotedSpeciesTypeComponentIndex_getSpeciesTypeComponentIndex(DenotedSpeciesTypeComponentIndex_t * dstci)
{
  if (dstci == NULL)
    return NULL;

  return dstci->getSpeciesTypeComponentIndex().empty() ? NULL : safe_strdup(dstci->getSpeciesTypeComponentIndex().c_str());
}


/*
 *
 */
LIBSBML_EXTERN
int
DenotedSpeciesTypeComponentIndex_isSetSpeciesTypeComponentIndex(DenotedSpeciesTypeComponentIndex_t * dstci)
{
  return (dstci != NULL) ? static_cast<int>(dstci->isSetSpeciesTypeComponentIndex()) : 0;
}


/*
 *
 */
LIBSBML_EXTERN
int
DenotedSpeciesTypeComponentIndex_setSpeciesTypeComponentIndex(DenotedSpeciesTypeComponentIndex_t * dstci, const char * speciesTypeComponentIndex)
{
  return (dstci != NULL) ? dstci->setSpeciesTypeComponentIndex(speciesTypeComponentIndex) : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
DenotedSpeciesTypeComponentIndex_unsetSpeciesTypeComponentIndex(DenotedSpeciesTypeComponentIndex_t * dstci)
{
  return (dstci != NULL) ? dstci->unsetSpeciesTypeComponentIndex() : LIBSBML_INVALID_OBJECT;
}


/*
 *
 */
LIBSBML_EXTERN
int
DenotedSpeciesTypeComponentIndex_hasRequiredAttributes(DenotedSpeciesTypeComponentIndex_t * dstci)
{
  return (dstci != NULL) ? static_cast<int>(dstci->hasRequiredAttributes()) : 0;
}


/*
 *
 */
LIBSBML_EXTERN
DenotedSpeciesTypeComponentIndex_t *
ListOfDenotedSpeciesTypeComponentIndexes_getById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfDenotedSpeciesTypeComponentIndexes *>(lo)->get(sid) : NULL;
}


/*
 *
 */
LIBSBML_EXTERN
DenotedSpeciesTypeComponentIndex_t *
ListOfDenotedSpeciesTypeComponentIndexes_removeById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfDenotedSpeciesTypeComponentIndexes *>(lo)->remove(sid) : NULL;
}




LIBSBML_CPP_NAMESPACE_END



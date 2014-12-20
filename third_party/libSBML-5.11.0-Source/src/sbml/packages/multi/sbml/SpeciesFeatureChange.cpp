/**
 * @file:   SpeciesFeatureChange.cpp
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


#include <sbml/packages/multi/sbml/SpeciesFeatureChange.h>
#include <sbml/packages/multi/validator/MultiSBMLError.h>


using namespace std;


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new SpeciesFeatureChange with the given level, version, and package version.
 */
SpeciesFeatureChange::SpeciesFeatureChange (unsigned int level, unsigned int version, unsigned int pkgVersion)
  : SBase(level, version)
   ,mId ("")
   ,mReactantSpeciesFeature ("")
   ,mProductSpeciesFeature ("")
{
  // set an SBMLNamespaces derived object of this package
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion));
}


/*
 * Creates a new SpeciesFeatureChange with the given MultiPkgNamespaces object.
 */
SpeciesFeatureChange::SpeciesFeatureChange (MultiPkgNamespaces* multins)
  : SBase(multins)
   ,mId ("")
   ,mReactantSpeciesFeature ("")
   ,mProductSpeciesFeature ("")
{
  // set the element namespace of this object
  setElementNamespace(multins->getURI());

  // load package extensions bound with this object (if any) 
  loadPlugins(multins);
}


/*
 * Copy constructor for SpeciesFeatureChange.
 */
SpeciesFeatureChange::SpeciesFeatureChange (const SpeciesFeatureChange& orig)
  : SBase(orig)
{
  if (&orig == NULL)
  {
    throw SBMLConstructorException("Null argument to copy constructor");
  }
  else
  {
    mId  = orig.mId;
    mReactantSpeciesFeature  = orig.mReactantSpeciesFeature;
    mProductSpeciesFeature  = orig.mProductSpeciesFeature;
  }
}


/*
 * Assignment for SpeciesFeatureChange.
 */
SpeciesFeatureChange&
SpeciesFeatureChange::operator=(const SpeciesFeatureChange& rhs)
{
  if (&rhs == NULL)
  {
    throw SBMLConstructorException("Null argument to assignment");
  }
  else if (&rhs != this)
  {
    SBase::operator=(rhs);
    mId  = rhs.mId;
    mReactantSpeciesFeature  = rhs.mReactantSpeciesFeature;
    mProductSpeciesFeature  = rhs.mProductSpeciesFeature;
  }
  return *this;
}


/*
 * Clone for SpeciesFeatureChange.
 */
SpeciesFeatureChange*
SpeciesFeatureChange::clone () const
{
  return new SpeciesFeatureChange(*this);
}


/*
 * Destructor for SpeciesFeatureChange.
 */
SpeciesFeatureChange::~SpeciesFeatureChange ()
{
}


/*
 * Returns the value of the "id" attribute of this SpeciesFeatureChange.
 */
const std::string&
SpeciesFeatureChange::getId() const
{
  return mId;
}


/*
 * Returns the value of the "reactantSpeciesFeature" attribute of this SpeciesFeatureChange.
 */
const std::string&
SpeciesFeatureChange::getReactantSpeciesFeature() const
{
  return mReactantSpeciesFeature;
}


/*
 * Returns the value of the "productSpeciesFeature" attribute of this SpeciesFeatureChange.
 */
const std::string&
SpeciesFeatureChange::getProductSpeciesFeature() const
{
  return mProductSpeciesFeature;
}


/*
 * Returns true/false if id is set.
 */
bool
SpeciesFeatureChange::isSetId() const
{
  return (mId.empty() == false);
}


/*
 * Returns true/false if reactantSpeciesFeature is set.
 */
bool
SpeciesFeatureChange::isSetReactantSpeciesFeature() const
{
  return (mReactantSpeciesFeature.empty() == false);
}


/*
 * Returns true/false if productSpeciesFeature is set.
 */
bool
SpeciesFeatureChange::isSetProductSpeciesFeature() const
{
  return (mProductSpeciesFeature.empty() == false);
}


/*
 * Sets id and returns value indicating success.
 */
int
SpeciesFeatureChange::setId(const std::string& id)
{
  return SyntaxChecker::checkAndSetSId(id, mId);
}


/*
 * Sets reactantSpeciesFeature and returns value indicating success.
 */
int
SpeciesFeatureChange::setReactantSpeciesFeature(const std::string& reactantSpeciesFeature)
{
  if (&(reactantSpeciesFeature) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else if (!(SyntaxChecker::isValidInternalSId(reactantSpeciesFeature)))
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mReactantSpeciesFeature = reactantSpeciesFeature;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Sets productSpeciesFeature and returns value indicating success.
 */
int
SpeciesFeatureChange::setProductSpeciesFeature(const std::string& productSpeciesFeature)
{
  if (&(productSpeciesFeature) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else if (!(SyntaxChecker::isValidInternalSId(productSpeciesFeature)))
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mProductSpeciesFeature = productSpeciesFeature;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Unsets id and returns value indicating success.
 */
int
SpeciesFeatureChange::unsetId()
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
 * Unsets reactantSpeciesFeature and returns value indicating success.
 */
int
SpeciesFeatureChange::unsetReactantSpeciesFeature()
{
  mReactantSpeciesFeature.erase();

  if (mReactantSpeciesFeature.empty() == true)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Unsets productSpeciesFeature and returns value indicating success.
 */
int
SpeciesFeatureChange::unsetProductSpeciesFeature()
{
  mProductSpeciesFeature.erase();

  if (mProductSpeciesFeature.empty() == true)
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
SpeciesFeatureChange::renameSIdRefs(const std::string& oldid, const std::string& newid)
{
  if (isSetReactantSpeciesFeature() == true && mReactantSpeciesFeature == oldid)
  {
    setReactantSpeciesFeature(newid);
  }

  if (isSetProductSpeciesFeature() == true && mProductSpeciesFeature == oldid)
  {
    setProductSpeciesFeature(newid);
  }

}


/*
 * Returns the XML element name of this object
 */
const std::string&
SpeciesFeatureChange::getElementName () const
{
  static const string name = "speciesFeatureChange";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
SpeciesFeatureChange::getTypeCode () const
{
  return SBML_MULTI_SPECIES_FEATURE_CHANGE;
}


/*
 * check if all the required attributes are set
 */
bool
SpeciesFeatureChange::hasRequiredAttributes () const
{
  bool allPresent = true;

  if (isSetReactantSpeciesFeature() == false)
    allPresent = false;

  if (isSetProductSpeciesFeature() == false)
    allPresent = false;

  return allPresent;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * write contained elements
 */
void
SpeciesFeatureChange::writeElements (XMLOutputStream& stream) const
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
SpeciesFeatureChange::accept (SBMLVisitor& v) const
{
  return v.visit(*this);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Sets the parent SBMLDocument.
 */
void
SpeciesFeatureChange::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Enables/Disables the given package with this element.
 */
void
SpeciesFeatureChange::enablePackageInternal(const std::string& pkgURI,
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
SpeciesFeatureChange::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("reactantSpeciesFeature");
  attributes.add("productSpeciesFeature");
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Read values from the given XMLAttributes set into their specific fields.
 */
void
SpeciesFeatureChange::readAttributes (const XMLAttributes& attributes,
                             const ExpectedAttributes& expectedAttributes)
{
  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  unsigned int numErrs;

  /* look to see whether an unknown attribute error was logged
   * during the read of the listOfSpeciesFeatureChanges - which will have
   * happened immediately prior to this read
  */

  if (getErrorLog() != NULL &&
      static_cast<ListOfSpeciesFeatureChanges*>(getParentSBMLObject())->size() < 2)
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
      logEmptyString(mId, getLevel(), getVersion(), "<SpeciesFeatureChange>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mId) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute id='" + mId + "' does not conform.");
    }
  }

  //
  // reactantSpeciesFeature SIdRef   ( use = "required" )
  //
  assigned = attributes.readInto("reactantSpeciesFeature", mReactantSpeciesFeature);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mReactantSpeciesFeature.empty() == true)
    {
      logEmptyString(mReactantSpeciesFeature, getLevel(), getVersion(), "<SpeciesFeatureChange>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mReactantSpeciesFeature) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute reactantSpeciesFeature='" 
        + mReactantSpeciesFeature + "' does not conform.");
    }
  }
  else
  {
    std::string message = "Multi attribute 'reactantSpeciesFeature' is missing.";
    getErrorLog()->logPackageError("multi", MultiUnknownError,
                   getPackageVersion(), sbmlLevel, sbmlVersion, message);
  }

  //
  // productSpeciesFeature SIdRef   ( use = "required" )
  //
  assigned = attributes.readInto("productSpeciesFeature", mProductSpeciesFeature);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mProductSpeciesFeature.empty() == true)
    {
      logEmptyString(mProductSpeciesFeature, getLevel(), getVersion(), "<SpeciesFeatureChange>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mProductSpeciesFeature) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute productSpeciesFeature='" 
        + mProductSpeciesFeature + "' does not conform.");
    }
  }
  else
  {
    std::string message = "Multi attribute 'productSpeciesFeature' is missing.";
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
SpeciesFeatureChange::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);

  if (isSetId() == true)
    stream.writeAttribute("id", getPrefix(), mId);

  if (isSetReactantSpeciesFeature() == true)
    stream.writeAttribute("reactantSpeciesFeature", getPrefix(), mReactantSpeciesFeature);

  if (isSetProductSpeciesFeature() == true)
    stream.writeAttribute("productSpeciesFeature", getPrefix(), mProductSpeciesFeature);

  SBase::writeExtensionAttributes(stream);

}


  /** @endcond doxygenLibsbmlInternal */


/*
 * Constructor 
 */
ListOfSpeciesFeatureChanges::ListOfSpeciesFeatureChanges(unsigned int level, 
                              unsigned int version, 
                              unsigned int pkgVersion)
 : ListOf(level, version)
{
  setSBMLNamespacesAndOwn(new MultiPkgNamespaces(level, version, pkgVersion)); 
}


/*
 * Constructor 
 */
ListOfSpeciesFeatureChanges::ListOfSpeciesFeatureChanges(MultiPkgNamespaces* multins)
  : ListOf(multins)
{
  setElementNamespace(multins->getURI());
}


/*
 * Returns a deep copy of this ListOfSpeciesFeatureChanges 
 */
ListOfSpeciesFeatureChanges* 
ListOfSpeciesFeatureChanges::clone () const
 {
  return new ListOfSpeciesFeatureChanges(*this);
}


/*
 * Get a SpeciesFeatureChange from the ListOfSpeciesFeatureChanges by index.
*/
SpeciesFeatureChange*
ListOfSpeciesFeatureChanges::get(unsigned int n)
{
  return static_cast<SpeciesFeatureChange*>(ListOf::get(n));
}


/*
 * Get a SpeciesFeatureChange from the ListOfSpeciesFeatureChanges by index.
 */
const SpeciesFeatureChange*
ListOfSpeciesFeatureChanges::get(unsigned int n) const
{
  return static_cast<const SpeciesFeatureChange*>(ListOf::get(n));
}


/*
 * Get a SpeciesFeatureChange from the ListOfSpeciesFeatureChanges by id.
 */
SpeciesFeatureChange*
ListOfSpeciesFeatureChanges::get(const std::string& sid)
{
  return const_cast<SpeciesFeatureChange*>(
    static_cast<const ListOfSpeciesFeatureChanges&>(*this).get(sid));
}


/*
 * Get a SpeciesFeatureChange from the ListOfSpeciesFeatureChanges by id.
 */
const SpeciesFeatureChange*
ListOfSpeciesFeatureChanges::get(const std::string& sid) const
{
  vector<SBase*>::const_iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<SpeciesFeatureChange>(sid) );
  return (result == mItems.end()) ? 0 : static_cast <SpeciesFeatureChange*> (*result);
}


/*
 * Removes the nth SpeciesFeatureChange from this ListOfSpeciesFeatureChanges
 */
SpeciesFeatureChange*
ListOfSpeciesFeatureChanges::remove(unsigned int n)
{
  return static_cast<SpeciesFeatureChange*>(ListOf::remove(n));
}


/*
 * Removes the SpeciesFeatureChange from this ListOfSpeciesFeatureChanges with the given identifier
 */
SpeciesFeatureChange*
ListOfSpeciesFeatureChanges::remove(const std::string& sid)
{
  SBase* item = NULL;
  vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEq<SpeciesFeatureChange>(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <SpeciesFeatureChange*> (item);
}


/*
 * Returns the XML element name of this object
 */
const std::string&
ListOfSpeciesFeatureChanges::getElementName () const
{
  static const string name = "listOfSpeciesFeatureChanges";
  return name;
}


/*
 * Returns the libSBML type code for this SBML object.
 */
int
ListOfSpeciesFeatureChanges::getTypeCode () const
{
  return SBML_LIST_OF;
}


/*
 * Returns the libSBML type code for the objects in this LIST_OF.
 */
int
ListOfSpeciesFeatureChanges::getItemTypeCode () const
{
  return SBML_MULTI_SPECIES_FEATURE_CHANGE;
}


  /** @cond doxygenLibsbmlInternal */

/*
 * Creates a new SpeciesFeatureChange in this ListOfSpeciesFeatureChanges
 */
SBase*
ListOfSpeciesFeatureChanges::createObject(XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase* object = NULL;

  if (name == "speciesFeatureChange")
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    object = new SpeciesFeatureChange(multins);
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
ListOfSpeciesFeatureChanges::writeXMLNS(XMLOutputStream& stream) const
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
SpeciesFeatureChange_t *
SpeciesFeatureChange_create(unsigned int level, unsigned int version,
                            unsigned int pkgVersion)
{
  return new SpeciesFeatureChange(level, version, pkgVersion);
}


/**
 * 
 */
LIBSBML_EXTERN
void
SpeciesFeatureChange_free(SpeciesFeatureChange_t * sfc)
{
  if (sfc != NULL)
    delete sfc;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesFeatureChange_t *
SpeciesFeatureChange_clone(SpeciesFeatureChange_t * sfc)
{
  if (sfc != NULL)
  {
    return static_cast<SpeciesFeatureChange_t*>(sfc->clone());
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
SpeciesFeatureChange_getId(SpeciesFeatureChange_t * sfc)
{
  if (sfc == NULL)
    return NULL;

  return sfc->getId().empty() ? NULL : safe_strdup(sfc->getId().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
char *
SpeciesFeatureChange_getReactantSpeciesFeature(SpeciesFeatureChange_t * sfc)
{
  if (sfc == NULL)
    return NULL;

  return sfc->getReactantSpeciesFeature().empty() ? NULL : safe_strdup(sfc->getReactantSpeciesFeature().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
char *
SpeciesFeatureChange_getProductSpeciesFeature(SpeciesFeatureChange_t * sfc)
{
  if (sfc == NULL)
    return NULL;

  return sfc->getProductSpeciesFeature().empty() ? NULL : safe_strdup(sfc->getProductSpeciesFeature().c_str());
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_isSetId(SpeciesFeatureChange_t * sfc)
{
  return (sfc != NULL) ? static_cast<int>(sfc->isSetId()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_isSetReactantSpeciesFeature(SpeciesFeatureChange_t * sfc)
{
  return (sfc != NULL) ? static_cast<int>(sfc->isSetReactantSpeciesFeature()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_isSetProductSpeciesFeature(SpeciesFeatureChange_t * sfc)
{
  return (sfc != NULL) ? static_cast<int>(sfc->isSetProductSpeciesFeature()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_setId(SpeciesFeatureChange_t * sfc, const char * id)
{
  return (sfc != NULL) ? sfc->setId(id) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_setReactantSpeciesFeature(SpeciesFeatureChange_t * sfc, const char * reactantSpeciesFeature)
{
  return (sfc != NULL) ? sfc->setReactantSpeciesFeature(reactantSpeciesFeature) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_setProductSpeciesFeature(SpeciesFeatureChange_t * sfc, const char * productSpeciesFeature)
{
  return (sfc != NULL) ? sfc->setProductSpeciesFeature(productSpeciesFeature) : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_unsetId(SpeciesFeatureChange_t * sfc)
{
  return (sfc != NULL) ? sfc->unsetId() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_unsetReactantSpeciesFeature(SpeciesFeatureChange_t * sfc)
{
  return (sfc != NULL) ? sfc->unsetReactantSpeciesFeature() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_unsetProductSpeciesFeature(SpeciesFeatureChange_t * sfc)
{
  return (sfc != NULL) ? sfc->unsetProductSpeciesFeature() : LIBSBML_INVALID_OBJECT;
}


/**
 * 
 */
LIBSBML_EXTERN
int
SpeciesFeatureChange_hasRequiredAttributes(SpeciesFeatureChange_t * sfc)
{
  return (sfc != NULL) ? static_cast<int>(sfc->hasRequiredAttributes()) : 0;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesFeatureChange_t *
ListOfSpeciesFeatureChanges_getById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfSpeciesFeatureChanges *>(lo)->get(sid) : NULL;
}


/**
 * 
 */
LIBSBML_EXTERN
SpeciesFeatureChange_t *
ListOfSpeciesFeatureChanges_removeById(ListOf_t * lo, const char * sid)
{
  if (lo == NULL)
    return NULL;

  return (sid != NULL) ? static_cast <ListOfSpeciesFeatureChanges *>(lo)->remove(sid) : NULL;
}




LIBSBML_CPP_NAMESPACE_END



/**
 * @file:   MultiCompartmentPlugin.cpp
 * @brief:  Implementation of the MultiCompartmentPlugin class
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


#include <sbml/packages/multi/extension/MultiCompartmentPlugin.h>
#include <sbml/packages/multi/validator/MultiSBMLError.h>

#include <sbml/util/ElementFilter.h>
#include <sbml/Model.h>


using namespace std;


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new MultiCompartmentPlugin
 */
MultiCompartmentPlugin::MultiCompartmentPlugin(const std::string& uri,  
                                 const std::string& prefix, 
                               MultiPkgNamespaces* multins) :
    SBasePlugin(uri, prefix, multins)
  , mCompartmentReferences (multins)
   ,mCompartmentType ("")
   ,mIsType (false)
   ,mIsSetIsType (false)
{
}


/*
 * Copy constructor for MultiCompartmentPlugin.
 */
MultiCompartmentPlugin::MultiCompartmentPlugin(const MultiCompartmentPlugin& orig) :
    SBasePlugin(orig)
  , mCompartmentReferences ( orig.mCompartmentReferences)
{
  if (&orig == NULL)
  {
    mCompartmentType  = orig.mCompartmentType;
    mIsType  = orig.mIsType;
    mIsSetIsType  = orig.mIsSetIsType;
  }
}


/*
 * Assignment operator for MultiCompartmentPlugin.
 */
MultiCompartmentPlugin& 
MultiCompartmentPlugin::operator=(const MultiCompartmentPlugin& rhs)
{
  if (&rhs != this)
  {
    this->SBasePlugin::operator=(rhs);
    mCompartmentReferences = rhs.mCompartmentReferences;
    mCompartmentType  = rhs.mCompartmentType;
    mIsType  = rhs.mIsType;
    mIsSetIsType  = rhs.mIsSetIsType;
  }

  return *this;
}


/*
 * Creates and returns a deep copy of this MultiCompartmentPlugin object.
 */
MultiCompartmentPlugin* 
MultiCompartmentPlugin::clone () const
{
  return new MultiCompartmentPlugin(*this);
}


/*
 * Destructor for MultiCompartmentPlugin.
 */
MultiCompartmentPlugin::~MultiCompartmentPlugin()
{
}


//---------------------------------------------------------------
//
// overridden virtual functions for read/write/check
//
//---------------------------------------------------------------

/*
 * create object
 */
SBase*
MultiCompartmentPlugin::createObject (XMLInputStream& stream)
{
  SBase* object = NULL; 

  const std::string&      name   = stream.peek().getName(); 
  const XMLNamespaces&    xmlns  = stream.peek().getNamespaces(); 
  const std::string&      prefix = stream.peek().getPrefix(); 

  const std::string& targetPrefix = (xmlns.hasURI(mURI)) ? xmlns.getPrefix(mURI) : mPrefix;

  if (prefix == targetPrefix) 
  { 
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    if (name == "listOfCompartmentReferences" ) 
    { 
      object = &mCompartmentReferences;

      if (targetPrefix.empty() == true) 
      { 
        mCompartmentReferences.getSBMLDocument()->enableDefaultNS(mURI, true); 
      } 
    } 
    delete multins;
  } 

  return object; 
}


/*
 * write elements
 */
void
MultiCompartmentPlugin::writeElements (XMLOutputStream& stream) const
{
  if (getNumCompartmentReferences() > 0) 
  { 
    mCompartmentReferences.write(stream);
  } 
}


/*
 * Checks if this plugin object has all the required elements.
 */
bool
MultiCompartmentPlugin::hasRequiredElements () const
{
  bool allPresent = true; 

  // TO DO 

  return allPresent; 
}

  /** @cond doxygenLibsbmlInternal */

/*
 * Get the list of expected attributes for this element.
 */
void
MultiCompartmentPlugin::addExpectedAttributes(ExpectedAttributes& attributes)
{
  attributes.add("compartmentType");
  attributes.add("isType");
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Read values from the given XMLAttributes set into their specific fields.
 */
void
MultiCompartmentPlugin::readAttributes (const XMLAttributes& attributes,
                             const ExpectedAttributes& expectedAttributes)
{
  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  unsigned int numErrs;

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
  // compartmentType SIdRef   ( use = "optional" )
  //
  assigned = attributes.readInto("compartmentType", mCompartmentType);

  if (assigned == true)
  {
    // check string is not empty and correct syntax

    if (mCompartmentType.empty() == true)
    {
      logEmptyString(mCompartmentType, getLevel(), getVersion(), 
        getPackageVersion(), "<MultiCompartmentPlugin>");
    }
    else if (SyntaxChecker::isValidSBMLSId(mCompartmentType) == false && getErrorLog() != NULL)
    {
      getErrorLog()->logError(InvalidIdSyntax, getLevel(), getVersion(), 
        "The syntax of the attribute compartmentType='" + mCompartmentType + 
        "' does not conform.");
    }
  }

  //
  // isType bool   ( use = "required" )
  //
  numErrs = getErrorLog()->getNumErrors();
  mIsSetIsType = attributes.readInto("isType", mIsType);

  if (mIsSetIsType == false)
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
        std::string message = "Multi attribute 'isType' is missing.";
        getErrorLog()->logPackageError("multi", MultiUnknownError,
                       getPackageVersion(), sbmlLevel, sbmlVersion, message);
      }
    }
  }

}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Write values of XMLAttributes to the output stream.
 */
  void
MultiCompartmentPlugin::writeAttributes (XMLOutputStream& stream) const
{
  if (isSetCompartmentType() == true)
    stream.writeAttribute("compartmentType", getPrefix(), mCompartmentType);

  if (isSetIsType() == true)
    stream.writeAttribute("isType", getPrefix(), mIsType);
}


  /** @endcond doxygenLibsbmlInternal */



//---------------------------------------------------------------
//
// Functions for interacting with the members of the plugin
//
//---------------------------------------------------------------


/*
 * Returns the value of the "compartmentType" attribute of this MultiCompartmentPlugin.
 */
const std::string&
MultiCompartmentPlugin::getCompartmentType() const
{
  return mCompartmentType;
}


/*
 * Returns the value of the "isType" attribute of this MultiCompartmentPlugin.
 */
const bool
MultiCompartmentPlugin::getIsType() const
{
  return mIsType;
}


/*
 * Returns true/false if compartmentType is set.
 */
bool
MultiCompartmentPlugin::isSetCompartmentType() const
{
  return (mCompartmentType.empty() == false);
}


/*
 * Returns true/false if isType is set.
 */
bool
MultiCompartmentPlugin::isSetIsType() const
{
  return mIsSetIsType;
}


/*
 * Sets compartmentType and returns value indicating success.
 */
int
MultiCompartmentPlugin::setCompartmentType(const std::string& compartmentType)
{
  if (&(compartmentType) == NULL)
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else if (!(SyntaxChecker::isValidInternalSId(compartmentType)))
  {
    return LIBSBML_INVALID_ATTRIBUTE_VALUE;
  }
  else
  {
    mCompartmentType = compartmentType;
    return LIBSBML_OPERATION_SUCCESS;
  }
}


/*
 * Sets isType and returns value indicating success.
 */
int
MultiCompartmentPlugin::setIsType(bool isType)
{
  mIsType = isType;
  mIsSetIsType = true;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Unsets compartmentType and returns value indicating success.
 */
int
MultiCompartmentPlugin::unsetCompartmentType()
{
  mCompartmentType.erase();

  if (mCompartmentType.empty() == true)
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}


/*
 * Unsets isType and returns value indicating success.
 */
int
MultiCompartmentPlugin::unsetIsType()
{
  mIsType = false;
  mIsSetIsType = false;
  return LIBSBML_OPERATION_SUCCESS;
}



List*
MultiCompartmentPlugin::getAllElements(ElementFilter* filter)
{
  List* ret = new List();
  List* sublist = NULL;

  ADD_FILTERED_LIST(ret, sublist, mCompartmentReferences, filter);

  return ret;
}


/*
 * Returns the ListOfCompartmentReferences in this plugin object.
 */
const ListOfCompartmentReferences* 
MultiCompartmentPlugin::getListOfCompartmentReferences () const
{
  return &this->mCompartmentReferences;
}


/*
 * Returns the ListOfCompartmentReferences in this plugin object.
 */
ListOfCompartmentReferences* 
MultiCompartmentPlugin::getListOfCompartmentReferences ()
{
  return &this->mCompartmentReferences;
}


/*
 * Returns the CompartmentReference object that belongs to the given index.
 */
const CompartmentReference*
MultiCompartmentPlugin::getCompartmentReference(unsigned int n) const
{
  return static_cast<const CompartmentReference*>(mCompartmentReferences.get(n));
}


/*
 * Returns the CompartmentReference object that belongs to the given index.
 */
CompartmentReference*
MultiCompartmentPlugin::getCompartmentReference(unsigned int n)
{
  return static_cast<CompartmentReference*>(mCompartmentReferences.get(n));
}


/*
 * Returns the CompartmentReference object based on its identifier.
 */
const CompartmentReference*
MultiCompartmentPlugin::getCompartmentReference(const std::string& sid) const
{
  return static_cast<const CompartmentReference*>(mCompartmentReferences.get(sid));
}


/*
 * Returns the CompartmentReference object based on its identifier.
 */
CompartmentReference*
MultiCompartmentPlugin::getCompartmentReference(const std::string& sid)
{
  return static_cast<CompartmentReference*>(mCompartmentReferences.get(sid));
}


/*
 * Adds a copy of the given CompartmentReference to the ListOfCompartmentReferences in this plugin object.
 */
int
MultiCompartmentPlugin::addCompartmentReference (const CompartmentReference* compartmentReference)
{
  if (compartmentReference == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (compartmentReference->hasRequiredElements() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != compartmentReference->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != compartmentReference->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (getPackageVersion() != compartmentReference->getPackageVersion())
  {
    return LIBSBML_PKG_VERSION_MISMATCH;
  }
  else
  {
    mCompartmentReferences.append(compartmentReference);
  }

  return LIBSBML_OPERATION_SUCCESS;

}


/*
 * Creates a new CompartmentReference object and adds it to the ListOfCompartmentReferences in this plugin object.
 */
CompartmentReference* 
MultiCompartmentPlugin::createCompartmentReference ()
{
   CompartmentReference* cr = NULL;

  try
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    cr = new CompartmentReference(multins);
    delete multins;
  }
  catch(...)
  {
  }

  if (cr != NULL)
  {
    mCompartmentReferences.appendAndOwn(cr);
  }

  return cr;
}


/*
 * Removes the nth CompartmentReference object from this plugin object
 */
CompartmentReference* 
MultiCompartmentPlugin::removeCompartmentReference(unsigned int n)
{
  return static_cast<CompartmentReference*>(mCompartmentReferences.remove(n));
}


/*
 * Removes the CompartmentReference object with the given id from this plugin object
 */
CompartmentReference* 
MultiCompartmentPlugin::removeCompartmentReference(const std::string& sid)
{
  return static_cast<CompartmentReference*>(mCompartmentReferences.remove(sid));
}


/*
 * Returns the number of CompartmentReference objects in this plugin object.
 */
unsigned int 
MultiCompartmentPlugin::getNumCompartmentReferences () const
{
  return mCompartmentReferences.size();
}


//---------------------------------------------------------------


/*
 * Set the SBMLDocument.
 */
void
MultiCompartmentPlugin::setSBMLDocument(SBMLDocument* d)
{
  SBasePlugin::setSBMLDocument(d);

  mCompartmentReferences.setSBMLDocument(d);
}


/*
 * Connect to parent.
 */
void
MultiCompartmentPlugin::connectToParent(SBase* sbase)
{
  SBasePlugin::connectToParent(sbase);

  mCompartmentReferences.connectToParent(sbase);
}


/*
 * Enables the given package.
 */
void
MultiCompartmentPlugin::enablePackageInternal(const std::string& pkgURI,
                                   const std::string& pkgPrefix, bool flag)
{
  mCompartmentReferences.enablePackageInternal(pkgURI, pkgPrefix, flag);
}


/*
 * Accept the SBMLVisitor.
 */
bool
MultiCompartmentPlugin::accept(SBMLVisitor& v) const
{
  const Model * model = static_cast<const Model * >(this->getParentSBMLObject());

  v.visit(*model);
  v.leave(*model);

  for(unsigned int i = 0; i < getNumCompartmentReferences(); i++)
  {
    getCompartmentReference(i)->accept(v);
  }

  return true;
}




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */



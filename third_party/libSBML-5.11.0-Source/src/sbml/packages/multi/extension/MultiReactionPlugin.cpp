/**
 * @file:   MultiReactionPlugin.cpp
 * @brief:  Implementation of the MultiReactionPlugin class
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


#include <sbml/packages/multi/extension/MultiReactionPlugin.h>
#include <sbml/packages/multi/validator/MultiSBMLError.h>

#include <sbml/Model.h>


using namespace std;


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new MultiReactionPlugin
 */
MultiReactionPlugin::MultiReactionPlugin(const std::string& uri,  
                                 const std::string& prefix, 
                               MultiPkgNamespaces* multins) :
    SBasePlugin(uri, prefix, multins)
   ,mIsIntraSpeciesReaction (false)
   ,mIsSetIsIntraSpeciesReaction (false)
{
}


/*
 * Copy constructor for MultiReactionPlugin.
 */
MultiReactionPlugin::MultiReactionPlugin(const MultiReactionPlugin& orig) :
    SBasePlugin(orig)
{
  if (&orig == NULL)
  {
    mIsIntraSpeciesReaction  = orig.mIsIntraSpeciesReaction;
    mIsSetIsIntraSpeciesReaction  = orig.mIsSetIsIntraSpeciesReaction;
  }
}


/*
 * Assignment operator for MultiReactionPlugin.
 */
MultiReactionPlugin& 
MultiReactionPlugin::operator=(const MultiReactionPlugin& rhs)
{
  if (&rhs != this)
  {
    this->SBasePlugin::operator=(rhs);
    mIsIntraSpeciesReaction  = rhs.mIsIntraSpeciesReaction;
    mIsSetIsIntraSpeciesReaction  = rhs.mIsSetIsIntraSpeciesReaction;
  }

  return *this;
}


/*
 * Creates and returns a deep copy of this MultiReactionPlugin object.
 */
MultiReactionPlugin* 
MultiReactionPlugin::clone () const
{
  return new MultiReactionPlugin(*this);
}


/*
 * Destructor for MultiReactionPlugin.
 */
MultiReactionPlugin::~MultiReactionPlugin()
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
MultiReactionPlugin::createObject (XMLInputStream& stream)
{
  SBase* object = NULL; 

  //const std::string&      name   = stream.peek().getName();
  const XMLNamespaces&    xmlns  = stream.peek().getNamespaces(); 
  const std::string&      prefix = stream.peek().getPrefix(); 

  const std::string& targetPrefix = (xmlns.hasURI(mURI)) ? xmlns.getPrefix(mURI) : mPrefix;

  if (prefix == targetPrefix) 
  { 
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    delete multins;
  } 

  return object; 
}


/*
 * write elements
 */
void
MultiReactionPlugin::writeElements (XMLOutputStream& stream) const
{
}


/*
 * Checks if this plugin object has all the required elements.
 */
bool
MultiReactionPlugin::hasRequiredElements () const
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
MultiReactionPlugin::addExpectedAttributes(ExpectedAttributes& attributes)
{
  attributes.add("isIntraSpeciesReaction");
}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Read values from the given XMLAttributes set into their specific fields.
 */
void
MultiReactionPlugin::readAttributes (const XMLAttributes& attributes,
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

  //bool assigned = false;

  //
  // isIntraSpeciesReaction bool   ( use = "optional" )
  //
  numErrs = getErrorLog()->getNumErrors();
  mIsSetIsIntraSpeciesReaction = attributes.readInto("isIntraSpeciesReaction", 
                                                     mIsIntraSpeciesReaction);

  if (mIsSetIsIntraSpeciesReaction == false)
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
    }
  }

}


  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Write values of XMLAttributes to the output stream.
 */
  void
MultiReactionPlugin::writeAttributes (XMLOutputStream& stream) const
{
  if (isSetIsIntraSpeciesReaction() == true)
    stream.writeAttribute("isIntraSpeciesReaction", getPrefix(), mIsIntraSpeciesReaction);
}


  /** @endcond doxygenLibsbmlInternal */



//---------------------------------------------------------------
//
// Functions for interacting with the members of the plugin
//
//---------------------------------------------------------------

/*
 * Returns the value of the "isIntraSpeciesReaction" attribute of this MultiReactionPlugin.
 */
bool
MultiReactionPlugin::getIsIntraSpeciesReaction() const
{
  return mIsIntraSpeciesReaction;
}


/*
 * Returns true/false if isIntraSpeciesReaction is set.
 */
bool
MultiReactionPlugin::isSetIsIntraSpeciesReaction() const
{
  return mIsSetIsIntraSpeciesReaction;
}


/*
 * Sets isIntraSpeciesReaction and returns value indicating success.
 */
int
MultiReactionPlugin::setIsIntraSpeciesReaction(bool isIntraSpeciesReaction)
{
  mIsIntraSpeciesReaction = isIntraSpeciesReaction;
  mIsSetIsIntraSpeciesReaction = true;
  return LIBSBML_OPERATION_SUCCESS;
}


/*
 * Unsets isIntraSpeciesReaction and returns value indicating success.
 */
int
MultiReactionPlugin::unsetIsIntraSpeciesReaction()
{
  mIsIntraSpeciesReaction = false;
  mIsSetIsIntraSpeciesReaction = false;
  return LIBSBML_OPERATION_SUCCESS;
}


List*
MultiReactionPlugin::getAllElements(ElementFilter* filter)
{
  List* ret = new List();
  //List* sublist = NULL;


  return ret;
}


//---------------------------------------------------------------


/*
 * Set the SBMLDocument.
 */
void
MultiReactionPlugin::setSBMLDocument(SBMLDocument* d)
{
  SBasePlugin::setSBMLDocument(d);

}


/*
 * Connect to parent.
 */
void
MultiReactionPlugin::connectToParent(SBase* sbase)
{
  SBasePlugin::connectToParent(sbase);

}


/*
 * Enables the given package.
 */
void
MultiReactionPlugin::enablePackageInternal(const std::string& pkgURI,
                                   const std::string& pkgPrefix, bool flag)
{
}


/*
 * Accept the SBMLVisitor.
 */
bool
MultiReactionPlugin::accept(SBMLVisitor& v) const
{
  const Model * model = static_cast<const Model * >(this->getParentSBMLObject());

  v.visit(*model);
  v.leave(*model);

  return true;
}




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */



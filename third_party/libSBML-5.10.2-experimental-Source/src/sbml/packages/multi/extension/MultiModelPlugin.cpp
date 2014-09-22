/**
 * @file:   MultiModelPlugin.cpp
 * @brief:  Implementation of the MultiModelPlugin class
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


#include <sbml/packages/multi/extension/MultiModelPlugin.h>

#include <sbml/util/ElementFilter.h>
#include <sbml/Model.h>



using namespace std;


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new MultiModelPlugin
 */
MultiModelPlugin::MultiModelPlugin(const std::string& uri,  
                                 const std::string& prefix, 
                               MultiPkgNamespaces* multins) :
    SBasePlugin(uri, prefix, multins)
  , mMultiSpeciesTypes (multins)
{
}


/*
 * Copy constructor for MultiModelPlugin.
 */
MultiModelPlugin::MultiModelPlugin(const MultiModelPlugin& orig) :
    SBasePlugin(orig)
  , mMultiSpeciesTypes ( orig.mMultiSpeciesTypes)
{
}


/*
 * Assignment operator for MultiModelPlugin.
 */
MultiModelPlugin& 
MultiModelPlugin::operator=(const MultiModelPlugin& rhs)
{
  if (&rhs != this)
  {
    this->SBasePlugin::operator=(rhs);
    mMultiSpeciesTypes = rhs.mMultiSpeciesTypes;
  }

  return *this;
}


/*
 * Creates and returns a deep copy of this MultiModelPlugin object.
 */
MultiModelPlugin* 
MultiModelPlugin::clone () const
{
  return new MultiModelPlugin(*this);
}


/*
 * Destructor for MultiModelPlugin.
 */
MultiModelPlugin::~MultiModelPlugin()
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
MultiModelPlugin::createObject (XMLInputStream& stream)
{
  SBase* object = NULL; 

  const std::string&      name   = stream.peek().getName(); 
  const XMLNamespaces&    xmlns  = stream.peek().getNamespaces(); 
  const std::string&      prefix = stream.peek().getPrefix(); 

  const std::string& targetPrefix = (xmlns.hasURI(mURI)) ? xmlns.getPrefix(mURI) : mPrefix;

  if (prefix == targetPrefix) 
  { 
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    if (name == "listOfSpeciesTypes" ) 
    { 
      object = &mMultiSpeciesTypes;

      if (targetPrefix.empty() == true) 
      { 
        mMultiSpeciesTypes.getSBMLDocument()->enableDefaultNS(mURI, true); 
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
MultiModelPlugin::writeElements (XMLOutputStream& stream) const
{
  if (getNumMultiSpeciesTypes() > 0) 
  { 
    mMultiSpeciesTypes.write(stream);
  } 
}


/*
 * Checks if this plugin object has all the required elements.
 */
bool
MultiModelPlugin::hasRequiredElements () const
{
  bool allPresent = true; 

  // TO DO 

  return allPresent; 
}


//---------------------------------------------------------------
//
// Functions for interacting with the members of the plugin
//
//---------------------------------------------------------------

List*
MultiModelPlugin::getAllElements(ElementFilter* filter)
{
  List* ret = new List();
  List* sublist = NULL;

  ADD_FILTERED_LIST(ret, sublist, mMultiSpeciesTypes, filter);

  return ret;
}


/*
 * Returns the ListOfMultiSpeciesTypes in this plugin object.
 */
const ListOfMultiSpeciesTypes* 
MultiModelPlugin::getListOfMultiSpeciesTypes () const
{
  return &this->mMultiSpeciesTypes;
}


/*
 * Returns the ListOfMultiSpeciesTypes in this plugin object.
 */
ListOfMultiSpeciesTypes* 
MultiModelPlugin::getListOfMultiSpeciesTypes ()
{
  return &this->mMultiSpeciesTypes;
}


/*
 * Returns the MultiSpeciesType object that belongs to the given index.
 */
const MultiSpeciesType*
MultiModelPlugin::getMultiSpeciesType(unsigned int n) const
{
  return static_cast<const MultiSpeciesType*>(mMultiSpeciesTypes.get(n));
}


/*
 * Returns the MultiSpeciesType object that belongs to the given index.
 */
MultiSpeciesType*
MultiModelPlugin::getMultiSpeciesType(unsigned int n)
{
  return static_cast<MultiSpeciesType*>(mMultiSpeciesTypes.get(n));
}


/*
 * Returns the MultiSpeciesType object based on its identifier.
 */
const MultiSpeciesType*
MultiModelPlugin::getMultiSpeciesType(const std::string& sid) const
{
  return static_cast<const MultiSpeciesType*>(mMultiSpeciesTypes.get(sid));
}


/*
 * Returns the MultiSpeciesType object based on its identifier.
 */
MultiSpeciesType*
MultiModelPlugin::getMultiSpeciesType(const std::string& sid)
{
  return static_cast<MultiSpeciesType*>(mMultiSpeciesTypes.get(sid));
}


/*
 * Adds a copy of the given MultiSpeciesType to the ListOfMultiSpeciesTypes in this plugin object.
 */
int
MultiModelPlugin::addMultiSpeciesType (const MultiSpeciesType* multiSpeciesType)
{
  if (multiSpeciesType == NULL)
  {
    return LIBSBML_OPERATION_FAILED;
  }
  else if (multiSpeciesType->hasRequiredElements() == false)
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != multiSpeciesType->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != multiSpeciesType->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (getPackageVersion() != multiSpeciesType->getPackageVersion())
  {
    return LIBSBML_PKG_VERSION_MISMATCH;
  }
  else
  {
    mMultiSpeciesTypes.append(multiSpeciesType);
  }

  return LIBSBML_OPERATION_SUCCESS;

}


/*
 * Creates a new MultiSpeciesType object and adds it to the ListOfMultiSpeciesTypes in this plugin object.
 */
MultiSpeciesType* 
MultiModelPlugin::createMultiSpeciesType ()
{
   MultiSpeciesType* mst = NULL;

  try
  {
    MULTI_CREATE_NS(multins, getSBMLNamespaces());
    mst = new MultiSpeciesType(multins);
    delete multins;
  }
  catch(...)
  {
  }

  if (mst != NULL)
  {
    mMultiSpeciesTypes.appendAndOwn(mst);
  }

  return mst;
}


/*
 * Removes the nth MultiSpeciesType object from this plugin object
 */
MultiSpeciesType* 
MultiModelPlugin::removeMultiSpeciesType(unsigned int n)
{
  return static_cast<MultiSpeciesType*>(mMultiSpeciesTypes.remove(n));
}


/*
 * Removes the MultiSpeciesType object with the given id from this plugin object
 */
MultiSpeciesType* 
MultiModelPlugin::removeMultiSpeciesType(const std::string& sid)
{
  return static_cast<MultiSpeciesType*>(mMultiSpeciesTypes.remove(sid));
}


/*
 * Returns the number of MultiSpeciesType objects in this plugin object.
 */
unsigned int 
MultiModelPlugin::getNumMultiSpeciesTypes () const
{
  return mMultiSpeciesTypes.size();
}


//---------------------------------------------------------------


/*
 * Set the SBMLDocument.
 */
void
MultiModelPlugin::setSBMLDocument(SBMLDocument* d)
{
  SBasePlugin::setSBMLDocument(d);

  mMultiSpeciesTypes.setSBMLDocument(d);
}


/*
 * Connect to parent.
 */
void
MultiModelPlugin::connectToParent(SBase* sbase)
{
  SBasePlugin::connectToParent(sbase);

  mMultiSpeciesTypes.connectToParent(sbase);
}


/*
 * Enables the given package.
 */
void
MultiModelPlugin::enablePackageInternal(const std::string& pkgURI,
                                   const std::string& pkgPrefix, bool flag)
{
  mMultiSpeciesTypes.enablePackageInternal(pkgURI, pkgPrefix, flag);
}


/*
 * Accept the SBMLVisitor.
 */
bool
MultiModelPlugin::accept(SBMLVisitor& v) const
{
  const Model * model = static_cast<const Model * >(this->getParentSBMLObject());

  v.visit(*model);
  v.leave(*model);

  for(unsigned int i = 0; i < getNumMultiSpeciesTypes(); i++)
  {
    getMultiSpeciesType(i)->accept(v);
  }

  return true;
}




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */



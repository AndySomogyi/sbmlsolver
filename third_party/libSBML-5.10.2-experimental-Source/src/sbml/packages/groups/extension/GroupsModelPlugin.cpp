/**
 * @file    GroupsModelPlugin.cpp
 * @brief   Implementation of GroupsModelPlugin, the plugin class of
 *          groups package for the Model element.
 * @author  Akiya Jouraku
 *
 * $Id: GroupsModelPlugin.cpp 13541 2011-04-08 22:16:45Z fbergmann $
 * $HeadURL: https://sbml.svn.sourceforge.net/svnroot/sbml/branches/libsbml-5/src/sbml/packages/groups/extension/GroupsModelPlugin.cpp $
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

#include <sbml/packages/groups/extension/GroupsModelPlugin.h>
#include <sbml/packages/groups/sbml/Group.h>

#include <sbml/Model.h>


using namespace std;


#ifdef __cplusplus

LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new GroupsModelPlugin
 */
GroupsModelPlugin::GroupsModelPlugin(const std::string& uri,  
                                 const std::string& prefix, 
                               GroupsPkgNamespaces* groupsns) :
    SBasePlugin(uri, prefix, groupsns)
  , mGroups (groupsns)
{
}


/*
 * Copy constructor for GroupsModelPlugin.
 */
GroupsModelPlugin::GroupsModelPlugin(const GroupsModelPlugin& orig) :
    SBasePlugin(orig)
  , mGroups ( orig.mGroups)
{
}


/*
 * Destroy this object.
 */
GroupsModelPlugin::~GroupsModelPlugin () {}

/*
 * Assignment operator for GroupsModelPlugin.
 */
GroupsModelPlugin& 
GroupsModelPlugin::operator=(const GroupsModelPlugin& orig)
{
  if(&orig!=this)
  {
    this->SBasePlugin::operator =(orig);
    mGroups    = orig.mGroups;
  }    
  return *this;
}


/*
 * Creates and returns a deep copy of this GroupsModelPlugin object.
 * 
 * @return a (deep) copy of this SBase object
 */
GroupsModelPlugin* 
GroupsModelPlugin::clone () const
{
  return new GroupsModelPlugin(*this);  
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
GroupsModelPlugin::createObject(XMLInputStream& stream)
{
  SBase*        object = 0;

  const std::string&   name   = stream.peek().getName();
  const XMLNamespaces& xmlns  = stream.peek().getNamespaces();
  const std::string&   prefix = stream.peek().getPrefix();

  const std::string& targetPrefix = (xmlns.hasURI(mURI)) ? xmlns.getPrefix(mURI) : mPrefix;
  
  if (prefix == targetPrefix)
  {
    if ( name == "listOfGroups" ) 
    {
      //cout << "[DEBUG] GroupsModelPlugin::createObject create listOfGroups" << endl;
      object = &mGroups;
    
      if (targetPrefix.empty())
      {
        //
        // (NOTE)
        //
        // A top-level element (listOfGroups) of the groups extension is located 
        // in a default namespace, and thus xmlns=".." attribute must be added to 
        // the element.
        // This is done by invoking SBMLDocument::enableDefaultNS() function with 
        // the two arguments (the uri of this package and true value).
        //
        mGroups.getSBMLDocument()->enableDefaultNS(mURI,true);
      }
    }          
  }    

  return object;
}


/*
 *
 */
void
GroupsModelPlugin::writeElements (XMLOutputStream& stream) const
{
  if (getNumGroups() > 0)
  {
    mGroups.write(stream);
  }    
}


/* default for components that have no required elements */
bool
GroupsModelPlugin::hasRequiredElements() const
{
  bool allPresent = true;

  if (mGroups.size() < 1)
  {
    allPresent = false;    
  }
  return allPresent;
}



/*
 *
 *  (EXTENSION) Additional public functions
 *
 */  


/*
 * Returns the ListOf Groups for this Model.
 */
const ListOfGroups*
GroupsModelPlugin::getListOfGroups () const
{
  return &this->mGroups;
}


/*
 * Returns the ListOf Groups for this Model.
 */
ListOfGroups*
GroupsModelPlugin::getListOfGroups ()
{
  return &this->mGroups;
}


/*
 * Returns the groups object that belongs to the given index. If the index
 * is invalid, NULL is returned.
 */
const Group*
GroupsModelPlugin::getGroup (unsigned int index) const
{
  return static_cast<const Group*>( mGroups.get(index) );
}


/*
 * Returns the groups object that belongs to the given index. If the index
 * is invalid, NULL is returned.
 */
Group*
GroupsModelPlugin::getGroup (unsigned int index)
{
  return static_cast<Group*>( mGroups.get(index) );
}


/*
 * Returns the group object based on its identifier.
 */
Group*
GroupsModelPlugin::getGroup (const std::string& sid)
{
  return static_cast<Group*>( mGroups.get(sid) );
}


/*
 * Returns the group object based on its identifier.
 */
const Group* 
GroupsModelPlugin::getGroup (const std::string& sid) const
{
  return static_cast<const Group*>( mGroups.get(sid) );
}


/*
 * Returns the number of Group objects.
 */
unsigned int 
GroupsModelPlugin::getNumGroups() const
{
  return mGroups.size();
}



/*
 * Adds a copy of the groups object to the list of groupss.
 */ 
int
GroupsModelPlugin::addGroup (const Group* groups)
{
  if (!groups)
  {
    return LIBSBML_OPERATION_FAILED;
  }    
  else if (!groups->hasRequiredElements())
  {
    return LIBSBML_INVALID_OBJECT;
  }
  else if (getLevel() != groups->getLevel())
  {
    return LIBSBML_LEVEL_MISMATCH;
  }
  else if (getVersion() != groups->getVersion())
  {
    return LIBSBML_VERSION_MISMATCH;
  }
  else if (getPackageVersion() != groups->getPackageVersion())
  {
    return LIBSBML_PKG_VERSION_MISMATCH;
  }
  else
  {
    mGroups.append(groups);
  }

  return LIBSBML_OPERATION_SUCCESS;

}


/*
 * Creates a new groups object and adds it to the list of groups objects.
 * A reference to the newly created object is returned.
 */
Group*
GroupsModelPlugin::createGroup ()
{
  Group* g = 0;

  try
  {  
    GROUPS_CREATE_NS(groupsns, getSBMLNamespaces());
    g = new Group(groupsns);
    delete groupsns;
  }
  catch(...)
  {
    /* 
     * NULL will be returned if the mSBMLNS is invalid (basically this
     * should not happen) or some exception is thrown (e.g. std::bad_alloc)
     *
     * (Maybe this should be changed so that caller can detect what kind 
     *  of error happened in this function.)
     */
  }

  if (g != NULL)
  {
    mGroups.appendAndOwn(g);
  }

  return g;
}


/*
 * Removes the nth Group object from this Model object and
 * returns a pointer to it.
 */
Group* 
GroupsModelPlugin::removeGroup (unsigned int n)
{
  return static_cast<Group*>(mGroups.remove(n));
}


/*
 * Removes the Group object with the given id attribute from 
 * this plugin object and returns a pointer to it.
 */
Group* 
GroupsModelPlugin::removeGroup (const std::string& sid)
{
  return static_cast<Group*>(mGroups.remove(sid));
}


/*
 * Sets the parent SBMLDocument of this SBML object.
 *
 * @param d the SBMLDocument object to use
 */
void 
GroupsModelPlugin::setSBMLDocument (SBMLDocument* d)
{
  SBasePlugin::setSBMLDocument(d);

  mGroups.setSBMLDocument(d);  
}


/*
 * Sets the parent SBML object of this plugin object to
 * this object and child elements (if any).
 * (Creates a child-parent relationship by this plugin object)
 */
void
GroupsModelPlugin::connectToParent (SBase* sbase)
{
  SBasePlugin::connectToParent(sbase);

  mGroups.connectToParent(sbase);
}


/*
 * Enables/Disables the given package with child elements in this plugin
 * object (if any).
 */
void
GroupsModelPlugin::enablePackageInternal(const std::string& pkgURI,
                                        const std::string& pkgPrefix, bool flag)
{
  mGroups.enablePackageInternal(pkgURI,pkgPrefix,flag);
}


/*
 * Accept the SBMLVisitor.
 */
bool
GroupsModelPlugin::accept(SBMLVisitor& v) const
{
  const Model * model = static_cast<const Model * >(this->getParentSBMLObject());

  v.visit(*model);
  v.leave(*model);

  for(unsigned int i = 0; i < getNumGroups(); i++)
  {
    getGroup(i)->accept(v);
  }

  return true;
}




LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */

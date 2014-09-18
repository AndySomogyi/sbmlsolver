/**
 * @file    GroupsExtension.cpp
 * @brief   Implementation of GroupsExtension, the core module of groups package.
 * @author  Akiya Jouraku
 *
 * $Id: GroupsExtension.cpp 13541 2011-04-08 22:16:45Z fbergmann $
 * $HeadURL: https://sbml.svn.sourceforge.net/svnroot/sbml/branches/libsbml-5/src/sbml/packages/groups/extension/GroupsExtension.cpp $
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

#include <sbml/extension/SBMLExtensionRegister.h>
#include <sbml/extension/SBMLExtensionRegistry.h>
#include <sbml/extension/SBasePluginCreator.h>
#include <sbml/extension/SBMLDocumentPlugin.h>

#include <sbml/packages/groups/extension/GroupsExtension.h>
#include <sbml/packages/groups/extension/GroupsModelPlugin.h>
#include <sbml/packages/groups/extension/GroupsSBMLDocumentPlugin.h>
#include <sbml/packages/groups/validator/GroupsSBMLErrorTable.h>


#ifdef __cplusplus


#include <iostream>


LIBSBML_CPP_NAMESPACE_BEGIN


/*---------------------------------------------------------------
 *
 * This block is global initialization code which should be automatically
 * executed before invoking main() block.
 *
 */

/*------------------ (START) ----------------------------------*/

/*
 * Returns the package name of this extension.
 */
const std::string&
GroupsExtension::getPackageName ()
{
  static const std::string pkgName = "groups";
  return pkgName;
}


/*
 * Returns the default SBML Level this extension.
 */
unsigned int
GroupsExtension::getDefaultLevel ()
{
  return 3;
}


/*
 * Returns the default SBML Version this extension.
 */
unsigned int
GroupsExtension::getDefaultVersion ()
{
  return 1;
}


/*
 * Returns the default SBML version this extension.
 */
unsigned int
GroupsExtension::getDefaultPackageVersion ()
{
  return 1;
}


/*
 * XML namespaces of package.
 */
const std::string&
GroupsExtension::getXmlnsL3V1V1 ()
{
  static const std::string xmlns = "http://www.sbml.org/sbml/level3/version1/groups/version1";
  return xmlns;
}


/*
 * Adds this GroupsExtension object to the SBMLExtensionRegistry class.
 * GroupsExtension::init function is automatically invoked when this
 * object is instantiated
 */
static SBMLExtensionRegister<GroupsExtension> groupsExtensionRegistry;


static
const char* SBML_GROUPS_TYPECODE_STRINGS[] =
{
    "Member"
  , "MemberConstraint"
  , "Group"
};


/*
 * Instantiate SBMLExtensionNamespaces<GroupsExtension>
 * (GroupsPkgNamespaces) for DLL.
 */
template class LIBSBML_EXTERN SBMLExtensionNamespaces<GroupsExtension>;


/*------------------ (END) ----------------------------------*/

/*
 * Constructor
 */
GroupsExtension::GroupsExtension ()
{
}


/*
 * Copy constructor.
 */
GroupsExtension::GroupsExtension(const GroupsExtension& orig)
: SBMLExtension(orig)
{
}


/*
 * Destroy this object.
 */
GroupsExtension::~GroupsExtension ()
{
}


/*
 * Assignment operator for GroupsExtension.
 */
GroupsExtension& 
GroupsExtension::operator=(const GroupsExtension& orig)
{
  SBMLExtension::operator=(orig);
  return *this;
}


/*
 * Creates and returns a deep copy of this GroupsExtension object.
 * 
 * @return a (deep) copy of this SBase object
 */
GroupsExtension* 
GroupsExtension::clone () const
{
  return new GroupsExtension(*this);  
}


const std::string&
GroupsExtension::getName() const
{
  return getPackageName();
}


/*
 * Returns the URI (namespace) of the package corresponding to the combination of the given sbml level,
 * sbml version, and package version.
 * Empty string will be returned if no corresponding URI exists.
 *
 * @return a string of the package URI
 */
const std::string& 
GroupsExtension::getURI(unsigned int sbmlLevel, unsigned int sbmlVersion, unsigned int pkgVersion) const
{
  if (sbmlLevel == 3)
  {
    if (sbmlVersion == 1)
    {
      if (pkgVersion == 1)
      {
        return getXmlnsL3V1V1();
      }
    }
  }

  static std::string empty = "";

  return empty;
}


/*
 * Returns the SBML level with the given URI of this package.
 *
 *  (NOTICE) Package developers MUST OVERRIDE this pure virtual function in their derived class.
 *
 */
unsigned int 
GroupsExtension::getLevel(const std::string &uri) const
{
  if (uri == getXmlnsL3V1V1())
  {
    return 3;
  }
  
  return 0;
}


/*
 * Returns the SBML version with the given URI of this package.
 *
 *  (NOTICE) Package developers MUST OVERRIDE this pure virtual function in their derived class.
 *
 */
unsigned int 
GroupsExtension::getVersion(const std::string &uri) const
{
  if (uri == getXmlnsL3V1V1())
  {
    return 1;
  }

  return 0;
}


/*
 * Returns the package version with the given URI of this package.
 *
 *  (NOTICE) Package developers MUST OVERRIDE this pure virtual function in their derived class.
 *
 */
unsigned int
GroupsExtension::getPackageVersion(const std::string &uri) const
{
  if (uri == getXmlnsL3V1V1())
  {
    return 1;
  }

  return 0;
}


/*
 * Returns an SBMLExtensionNamespaces<class SBMLExtensionType> object 
 * (e.g. SBMLExtensionNamespaces<GroupsExtension> whose alias type is 
 * GroupsPkgNamespaces) corresponding to the given uri.
 * NULL will be returned if the given uri is not defined in the corresponding package.
 *
 *  (NOTICE) Package developers MUST OVERRIDE this pure virtual function in their derived class.
 *
 */
SBMLNamespaces*
GroupsExtension::getSBMLExtensionNamespaces(const std::string &uri) const
{
  GroupsPkgNamespaces* pkgns = 0;
  if ( uri == getXmlnsL3V1V1())
  {
    pkgns = new GroupsPkgNamespaces(3,1,1);    
  }  
  return pkgns;
}


/*
 * This method takes a type code of groups package and returns a string representing
 * the code.
 */
const char* 
GroupsExtension::getStringFromTypeCode(int typeCode) const
{
  int min = SBML_GROUPS_MEMBER;
  int max = SBML_GROUPS_GROUP;

  if ( typeCode < min || typeCode > max)
  {
    return "(Unknown SBML Groups Type)";  
  }

  return SBML_GROUPS_TYPECODE_STRINGS[typeCode - min];
}


/*
 *
 * Initialization function of groups extension module which is automatically invoked 
 * by SBMLExtensionRegister class before main() function invoked.
 *
 */
void 
GroupsExtension::init()
{
  //-------------------------------------------------------------------------
  //
  // 1. Checks if the groups pacakge has already been registered.
  //
  //-------------------------------------------------------------------------

  if (SBMLExtensionRegistry::getInstance().isRegistered(getPackageName()))
  {
    // do nothing;
    return;
  }

  //-------------------------------------------------------------------------
  //
  // 2. Creates an SBMLExtension derived object.
  //
  //-------------------------------------------------------------------------

  GroupsExtension groupsExtension;

  //-------------------------------------------------------------------------------------
  //
  // 3. Creates SBasePluginCreatorBase derived objects required for this 
  //    extension. The derived classes can be instantiated by using the following 
  //     template class.
  //
  //    temaplate<class SBasePluginType> class SBasePluginCreator
  //
  //    The constructor of the creator class has two arguments:
  //
  //        (1) SBaseExtensionPoint : extension point to which the plugin object connected
  //        (2) std::vector<std::string> : a std::vector object that contains a list of URI
  //                                       (package versions) supported by the plugin object.
  //
  //    For example, two plugin objects (plugged in SBMLDocument and Model elements) are 
  //    required for the groups extension.
  //
  //    Since only 'required' attribute is used in SBMLDocument by the groups package, existing
  //    SBMLDocumentPluginNotRequired class can be used as-is for the plugin.
  //
  //    Since the lists of supported package versions (currently only L3V1-groups-V1 supported )
  //    are equal in the both plugin objects, the same vector object is given to each 
  //    constructor.
  //
  //---------------------------------------------------------------------------------------

  std::vector<std::string> packageURIs;
  packageURIs.push_back(getXmlnsL3V1V1());

  SBaseExtensionPoint sbmldocExtPoint("core",SBML_DOCUMENT);
  SBaseExtensionPoint modelExtPoint("core",SBML_MODEL);

  SBasePluginCreator<GroupsSBMLDocumentPlugin, GroupsExtension> sbmldocPluginCreator(sbmldocExtPoint, packageURIs);
  SBasePluginCreator<GroupsModelPlugin,   GroupsExtension> modelPluginCreator(modelExtPoint,packageURIs);

  //--------------------------------------------------------------------------------------
  //
  // 3. Adds the above SBasePluginCreatorBase derived objects to the SBMLExtension derived object.
  //
  //--------------------------------------------------------------------------------------

  groupsExtension.addSBasePluginCreator(&sbmldocPluginCreator);
  groupsExtension.addSBasePluginCreator(&modelPluginCreator);

  //-------------------------------------------------------------------------
  //
  // 4. Registers the SBMLExtension derived object to SBMLExtensionRegistry
  //
  //-------------------------------------------------------------------------

  int result = SBMLExtensionRegistry::getInstance().addExtension(&groupsExtension);

  if (result != LIBSBML_OPERATION_SUCCESS)
  {
    std::cerr << "[Error] GroupsExtension::init() failed." << std::endl;
  }
}


  /** @cond doxygenLibsbmlInternal */

/*
 * Return error table entry. 
 */
packageErrorTableEntry
GroupsExtension::getErrorTable(unsigned int index) const
{
  return groupsErrorTable[index];
}

  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Return error table index for this id. 
 */
unsigned int
GroupsExtension::getErrorTableIndex(unsigned int errorId) const
{
  unsigned int tableSize = sizeof(groupsErrorTable)/sizeof(groupsErrorTable[0]);
  unsigned int index = 0;

  for(unsigned int i = 0; i < tableSize; i++)
  {
    if (errorId == groupsErrorTable[i].code)
    {
      index = i;
      break;
    }

  }

  return index;
}

  /** @endcond doxygenLibsbmlInternal */


  /** @cond doxygenLibsbmlInternal */

/*
 * Return error offset. 
 */
unsigned int
GroupsExtension::getErrorIdOffset() const
{
  return 4000000;
}

  /** @endcond doxygenLibsbmlInternal */




LIBSBML_CPP_NAMESPACE_END


#endif /* __cplusplus */



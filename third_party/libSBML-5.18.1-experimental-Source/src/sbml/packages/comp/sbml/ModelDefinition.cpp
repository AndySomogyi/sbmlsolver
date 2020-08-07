/**
 * @file    ModelDefinition.cpp
 * @brief   Implementation of ModelDefinition, the SBase-derived class of the comp package.
 * @author  Lucian Smith
 *
 *<!---------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 *------------------------------------------------------------------------- -->
 */

#include <iostream>

#include <sbml/SBMLVisitor.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLToken.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>

#include <sbml/packages/comp/extension/CompExtension.h>
#include <sbml/packages/comp/sbml/ModelDefinition.h>
#include <sbml/packages/comp/sbml/ListOfPorts.h>
#include <sbml/packages/comp/sbml/ListOfSubmodels.h>
#include <sbml/packages/comp/sbml/ListOfModelDefinitions.h>
#include <sbml/packages/comp/validator/CompSBMLError.h>


using namespace std;

LIBSBML_CPP_NAMESPACE_BEGIN
#ifdef __cplusplus

ModelDefinition::ModelDefinition (unsigned int level, unsigned int version, unsigned int pkgVersion) 
  : Model(level,version)
{
  // set an SBMLNamespaces derived object (CompPkgNamespaces) of this package.
  CompPkgNamespaces* moddef = new CompPkgNamespaces(level,version,pkgVersion);
  setSBMLNamespacesAndOwn(moddef);  
  //getSBMLDocument()->enableDefaultNS(mXpathURI,true);

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(moddef);
}


ModelDefinition::ModelDefinition(CompPkgNamespaces* compns)
  : Model(compns)
{
  // set the element namespace of this object
  setElementNamespace(compns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(compns);
}


ModelDefinition::ModelDefinition(const Model& source) 
  : Model (source)
{
  if (source.getTypeCode() == SBML_MODEL) {
    //The namespace has changed.
    CompPkgNamespaces* moddef = new CompPkgNamespaces(); //Take the default level/version/version
    moddef->addNamespaces(source.getSBMLNamespaces()->getNamespaces());
    setSBMLNamespacesAndOwn(moddef);
  }
  // connect child elements to this element.
  connectToChild();
  // load package extensions bound with this object (if any) 
  loadPlugins(getSBMLNamespaces());
}

ModelDefinition& ModelDefinition::operator=(const Model& source)
{
  if(&source!=this)
  {
    ModelDefinition::operator=(source);
    if (source.getTypeCode() == SBML_MODEL) {
      CompPkgNamespaces* moddef = new CompPkgNamespaces(); //Take the default level/version/version
      moddef->addNamespaces(source.getSBMLNamespaces()->getNamespaces());
      setSBMLNamespacesAndOwn(moddef);
    }
    // connect child elements to this element.
    connectToChild();
  }
  // load package extensions bound with this object (if any) 
  loadPlugins(getSBMLNamespaces());

  return *this;
}


ModelDefinition*
ModelDefinition::clone() const
{
  return new ModelDefinition(*this);
}


ModelDefinition::~ModelDefinition ()
{
}


const std::string&
ModelDefinition::getElementName () const
{
  static const std::string name = "modelDefinition";
  return name;
}

int
ModelDefinition::getTypeCode () const
{
  return SBML_COMP_MODELDEFINITION;
}

int 
ModelDefinition::removeFromParentAndDelete()
{
  //Since Model overrode the SBase function, we must call it explicitly.  We need not roll our own defintion entirely, though, since ModelDefinitions live in a ListOf just like almost everything else under the sun.
  return SBase::removeFromParentAndDelete();
}


/** @cond doxygenLibsbmlInternal */
bool
ModelDefinition::accept (SBMLVisitor& v) const
{
  v.visit(*this);
  return true;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void 
ModelDefinition::addExpectedAttributes(ExpectedAttributes& attributes)
{
  Model::addExpectedAttributes(attributes);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void 
ModelDefinition::readAttributes (const XMLAttributes& attributes, 
                               const ExpectedAttributes& expectedAttributes)
{
  const unsigned int sbmlLevel   = getLevel  ();
  const unsigned int sbmlVersion = getVersion();

  // look to see whether an unknown attribute error was logged
  // during the read of the ListOfModelDefinitions - which will have
  // happened immediately prior to this read
  if (getErrorLog() != NULL && 
    static_cast<ListOfModelDefinitions*>(getParentSBMLObject())->size() < 2)
  {
    unsigned int numErrs = getErrorLog()->getNumErrors();
    for (int n = (int)numErrs-1; n >= 0; --n)
    {
      if (getErrorLog()->getError((unsigned int)n)->getErrorId() == UnknownPackageAttribute)
      {
        const std::string details = 
          getErrorLog()->getError((unsigned int)n)->getMessage();
        getErrorLog()->remove(UnknownPackageAttribute);
        getErrorLog()->logPackageError("comp", CompLOModelDefsAllowedAttributes,
          getPackageVersion(), sbmlLevel, sbmlVersion, details, getLine(), getColumn());
      } 
      else if (getErrorLog()->getError((unsigned int)n)->getErrorId() == UnknownCoreAttribute)
      {
        const std::string details = 
          getErrorLog()->getError((unsigned int)n)->getMessage();
        getErrorLog()->remove(UnknownCoreAttribute);
        getErrorLog()->logPackageError("comp", CompLOModelDefsAllowedAttributes,
          getPackageVersion(), sbmlLevel, sbmlVersion, details, getLine(), getColumn());
      } 
    }
  }

  Model::readAttributes(attributes, expectedAttributes);
  SBMLErrorLog* log = getErrorLog();
  string compid = attributes.getValue("id", mURI);
  string coreid = attributes.getValue("id", "");
  string compname = attributes.getValue("name", mURI);
  string corename = attributes.getValue("name", "");
  if (!compid.empty())
  {
    string details = "The <comp:modelDefinition> element ";
    if (!coreid.empty()) {
      details += "with the 'id' with the value '" + coreid + "' and ";
    }
    details += "with the 'comp:id' with value '" + compid 
      + "' may not use a 'comp:id': the id attribute from core must be used instead.";
    log->logError(AllowedAttributesOnModel, sbmlLevel, sbmlVersion, details);
  }
  if (!compname.empty())
  {
    string details = "The <comp:modelDefinition> element ";
    if (!corename.empty()) {
      details += "with the 'name' with the value '" + corename + "' and ";
    }
    details += "with the 'comp:name' with value '" + compname 
      + "' may not use a 'comp:name': the name attribute from core must be used instead.";
    log->logError(AllowedAttributesOnModel, sbmlLevel, sbmlVersion, details);
  }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void
ModelDefinition::writeAttributes(XMLOutputStream& stream) const
{
  Model::writeAttributes(stream);

  // the id and name attribute on a ModelDefinition come from the parent model
  // BUT if the document is an L3V2 doc and we are using comp l3v1v1 we need to write
  // the id and name here but still in the sbml ns
  if (getSBMLDocument()->getVersion() > 1 && this->getPackageCoreVersion() == 1)
  {
    if (isSetId()) {
      stream.writeAttribute("id", getSBMLPrefix(), mId);
    }
    if (isSetName()) {
      stream.writeAttribute("name", getSBMLPrefix(), mName);
    }
  }
  ModelDefinition::writeExtensionAttributes(stream);
}
/** @endcond */



#endif /* __cplusplus */
/** @cond doxygenIgnored */
LIBSBML_EXTERN
ModelDefinition_t *
ModelDefinition_create(unsigned int level, unsigned int version,
                       unsigned int pkgVersion)
{
  return new ModelDefinition(level, version, pkgVersion);
}

LIBSBML_EXTERN
void
ModelDefinition_free(ModelDefinition_t * md)
{
  if (md != NULL)
    delete md;
}


LIBSBML_EXTERN
ModelDefinition_t *
ModelDefinition_clone(ModelDefinition_t * md)
{
  if (md != NULL)
  {
    return static_cast<ModelDefinition_t*>(md->clone());
  }
  else
  {
    return NULL;
  }
}
/** @endcond */
LIBSBML_CPP_NAMESPACE_END


/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    GroupsUniqueModelWideIds.cpp
 * @brief   Base class for Id constraints
 * @author  Ben Bornstein
 * 
 * <!--------------------------------------------------------------------------
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
 * ---------------------------------------------------------------------- -->*/

#include "GroupsUniqueModelWideIds.h"
#include <sbml/packages/groups/extension/GroupsModelPlugin.h>

/** @cond doxygenIgnored */
using namespace std;
/** @endcond */

LIBSBML_CPP_NAMESPACE_BEGIN


/*
 * Creates a new GroupsUniqueModelWideIds with the given constraint id.
 */
GroupsUniqueModelWideIds::GroupsUniqueModelWideIds (unsigned int id, Validator& v) : TConstraint<Model>(id, v)
{
}


/*
 * Destroys this Constraint.
 */
GroupsUniqueModelWideIds::~GroupsUniqueModelWideIds ()
{
}


/*
 * Checks that all ids for some given subset of the Model adhere to this
 * Constraint.  Override the doCheck() method to define your own subset.
 */
void
GroupsUniqueModelWideIds::check_ (const Model& m, const Model& )
{
  doCheck(m);
}


/*
 * @return the typename of the given SBase object.
 */
const char*
GroupsUniqueModelWideIds::getTypename (const SBase& object)
{
  return SBMLTypeCode_toString( object.getTypeCode(), object.getPackageName().c_str() );
}


/*
 * Logs a message that the given @p id (and its corresponding object) have
 * failed to satisfy this constraint.
 */
void
GroupsUniqueModelWideIds::logIdConflict (const std::string& id, const SBase& object)
{
  logFailure(object, getMessage(id, object));
}

/*
 * Resets the state of this GlobalConstraint by clearing its internal
 * list of error messages.
 */
void
GroupsUniqueModelWideIds::reset ()
{
  mIdMap.clear();
}


/*
 * Checks that the id associated with the given object is unique.  If it
 * is not, logIdConflict is called.
 */
void
GroupsUniqueModelWideIds::doCheckId (const SBase& object)
{ 
  if (object.isSetId())
  {
    const string& id = object.getId();

    if (mIdMap.insert( make_pair(id, &object) ).second == false)
    {
      logIdConflict(id, object);
    }
  }
}


void
GroupsUniqueModelWideIds::logId (const SBase& object)
{ 
  if (object.isSetId())
  {
    const string& id = object.getId();

    mIdMap.insert( make_pair(id, &object) );
  }
}


/*
 * @return the error message to use when logging constraint violations.
 * This method is called by logFailure.
 *
 * Returns a message that the given @p id and its corresponding object are
 * in  conflict with an object previously defined.
 */
const string
GroupsUniqueModelWideIds::getMessage (const string& id, const SBase& object)
{
  IdObjectMap::iterator iter = mIdMap.find(id);


  if (iter == mIdMap.end())
  {
    return
      "Internal (but non-fatal) Validator error in "
      "GroupsUniqueModelWideIds::getMessage().  The SBML object with duplicate id was "
      "not found when it came time to construct a descriptive error message.";
  }


  ostringstream oss_msg;
  const SBase&  previous = *(iter->second);


  //oss_msg << getPreamble();

  //
  // Example message: 
  //
  // The <compartment> id 'cell' conflicts with the previously defined
  // <parameter> id 'cell' at line 10.
  //

  oss_msg << "  The <" << object.getElementName()
      << "> id '" << id << "' conflicts with the previously defined <"
      << previous.getElementName()
      << "> id '" << id << "'";

  if (previous.getLine() != 0)
  {
    oss_msg << " at line " << previous.getLine();
  }

  oss_msg << '.';

  return oss_msg.str();
}
/*
 * Checks that all ids on the following Model objects are unique:
 * FunctionDefinitions, Species, Compartments, global Parameters,
 * Reactions, and Events.
 */
void
GroupsUniqueModelWideIds::doCheck (const Model& m)
{
  createExistingMap(m);

  unsigned int size = 0;

  const GroupsModelPlugin * modelPlug = 
    static_cast<const GroupsModelPlugin*>(m.getPlugin("groups"));


  size = modelPlug->getNumGroups();
  for (unsigned int n = 0; n < size; ++n) 
  {
    const Group *g = modelPlug->getGroup(n);
    doCheckId( *g );
    doCheckId( *(g->getListOfMembers()));
    for (unsigned int j = 0; j < g->getNumMembers(); j++)
    {
      doCheckId(*(g->getMember(j)));
    }
  }

  reset();
}

/*
 * Checks that all ids on the following Model objects are unique:
 * FunctionDefinitions, Species, Compartments, global Parameters,
 * Reactions, and Events.
 */
void
GroupsUniqueModelWideIds::createExistingMap (const Model& m)
{
  unsigned int n, size, sr, sr_size;

  logId( m );

  size = m.getNumFunctionDefinitions();
  for (n = 0; n < size; ++n) logId( *m.getFunctionDefinition(n) );

  size = m.getNumCompartments();
  for (n = 0; n < size; ++n) logId( *m.getCompartment(n) );

  size = m.getNumSpecies();
  for (n = 0; n < size; ++n) logId( *m.getSpecies(n) );

  size = m.getNumParameters();
  for (n = 0; n < size; ++n) logId( *m.getParameter(n) );

  size = m.getNumReactions();
  for (n = 0; n < size; ++n) 
  {
    logId( *m.getReaction(n) );

    sr_size = m.getReaction(n)->getNumReactants();
    for (sr = 0; sr < sr_size; sr++)
    {
      logId(*m.getReaction(n)->getReactant(sr));
    }

    sr_size = m.getReaction(n)->getNumProducts();
    for (sr = 0; sr < sr_size; sr++)
    {
      logId(*m.getReaction(n)->getProduct(sr));
    }

    sr_size = m.getReaction(n)->getNumModifiers();
    for (sr = 0; sr < sr_size; sr++)
    {
      logId((SBase&)(*m.getReaction(n)->getModifier(sr)));
    }

  }

  size = m.getNumEvents();
  for (n = 0; n < size; ++n) logId( *m.getEvent(n) );

  size = m.getNumCompartmentTypes();
  for (n = 0; n < size; ++n) logId( *m.getCompartmentType(n) );

  size = m.getNumSpeciesTypes();
  for (n = 0; n < size; ++n) logId( *m.getSpeciesType(n) );
}

LIBSBML_CPP_NAMESPACE_END
/** @endcond */

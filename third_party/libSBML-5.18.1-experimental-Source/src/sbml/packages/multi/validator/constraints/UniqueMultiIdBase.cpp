/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    UniqueMultiIdBase.cpp
 * @brief   Base class for unique id constraints
 * @author  Fengkai Zhang
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
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#include <sbml/SBase.h>

#include "UniqueMultiIdBase.h"

/** @cond doxygenIgnored */

using namespace std;

/** @endcond */

LIBSBML_CPP_NAMESPACE_BEGIN
#ifdef __cplusplus

/*
 * Creates a new UniqueMultiIdBase with the given constraint id.
 */
UniqueMultiIdBase::UniqueMultiIdBase (unsigned int id, MultiValidator& v) : MultiIdBase(id, v)
{
}


/*
 * Destroys this Constraint.
 */
UniqueMultiIdBase::~UniqueMultiIdBase ()
{
}


/*
 * Resets the state of this GlobalConstraint by clearing its internal
 * list of error messages.
 */
void
UniqueMultiIdBase::reset ()
{
  mIdObjectMap.clear();
}


/*
 * Checks that the id associated with the given object is unique.  If it
 * is not, logIdConflict is called.
 */
void
UniqueMultiIdBase::doCheckId (const string& id, const SBase& object)
{
  if (mIdObjectMap.insert( make_pair(id, &object) ).second == false)
  {
    logIdConflict(id, object);
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
UniqueMultiIdBase::getMessage (const string& id, const SBase& object)
{
  IdObjectMap::iterator iter = mIdObjectMap.find(id);


  if (iter == mIdObjectMap.end())
  {
    return
      "Internal (but non-fatal) Validator error in "
      "UniqueMultiIdBase::getMessage().  The SBML object with duplicate id was "
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

  oss_msg << "  The <" << object.getElementName() << "> " << getFieldname()
      << " '" << id << "' conflicts with the previously defined <"
      << previous.getElementName() << "> " << getFieldname()
      << " '" << id << "'";

  if (previous.getLine() != 0)
  {
    oss_msg << " at line " << previous.getLine();
  }

  oss_msg << '.';

  return oss_msg.str();
}

#endif /* __cplusplus */

LIBSBML_CPP_NAMESPACE_END

/** @endcond */

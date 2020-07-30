/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    QSAssignedOnce.cpp
 * @brief   Checks rule ordering for l2v1 and l1
 * @author  Sarah Keating
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

#include <cstring>

#include <sbml/Model.h>
#include <sbml/packages/qual/extension/QualModelPlugin.h>

#include "QSAssignedOnce.h"

/** @cond doxygenIgnored */
using namespace std;
/** @endcond */

LIBSBML_CPP_NAMESPACE_BEGIN

/**
 * Creates a new Constraint with the given constraint id.
 */
QSAssignedOnce::QSAssignedOnce (unsigned int id, Validator& v) :
  TConstraint<Model>(id, v)
{
}


/**
 * Destroys this Constraint.
 */
QSAssignedOnce::~QSAssignedOnce ()
{
}


/**
 * Checks that all ids on the following Model objects are unique:
 * event assignments and assignment rules.
 */
void
QSAssignedOnce::check_ (const Model& m, const Model&)
{
  const QualModelPlugin * plug = 
                 static_cast<const QualModelPlugin*>(m.getPlugin("qual"));
  
  if (plug == NULL) 
  {
    return;
  }

  mAssigned.clear();

  for (unsigned int i = 0; i < plug->getNumTransitions(); i++)
  {
    checkTransition(plug->getTransition(i));
  }

}
 
void
QSAssignedOnce::checkTransition(const Transition* tr)
{
  for (unsigned int i = 0; i < tr->getNumOutputs(); i++)
  {
    const Output* out = tr->getOutput(i);
    if (out->isSetTransitionEffect() && 
      out->getTransitionEffect() == OUTPUT_TRANSITION_EFFECT_ASSIGNMENT_LEVEL)
    {
      if (mAssigned.contains(out->getQualitativeSpecies()))
      {
        logMultipleAssignment(*tr, *out, out->getQualitativeSpecies());
      }
      else
      {
        mAssigned.append(out->getQualitativeSpecies());
      }
    }
  }
}


void
QSAssignedOnce::logMultipleAssignment (const Transition& tr,
                          const SBase& object,
                          std::string name)
{
  msg =
    "The <transition> with id '";
  msg += tr.getId();
  msg += "' includes an <output>";
  msg += " that uses an assignment to the <qualitativeSpecies> '";
  msg += name;
  msg += "' that has already been assigned.";
  
  logFailure(object);

}

LIBSBML_CPP_NAMESPACE_END
/** @endcond */

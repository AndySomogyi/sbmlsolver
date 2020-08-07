/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    UniqueCompIdBase.h
 * @brief   Base class for unique id constraints
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

#ifndef UniqueCompIdBase_h
#define UniqueCompIdBase_h


#ifdef __cplusplus


#include <string>
#include <sstream>
#include <map>

#include "CompIdBase.h"

LIBSBML_CPP_NAMESPACE_BEGIN

class SBase;
class Validator;


class UniqueCompIdBase: public CompIdBase
{
public:

  /**
   * Creates a new UniqueCompIdBase with the given constraint id.
   */
  UniqueCompIdBase (unsigned int id, CompValidator& v);

  /**
   * Destroys this Constraint.
   */
  virtual ~UniqueCompIdBase ();


protected:

  /**
   * Resets the state of this GlobalConstraint by clearing its internal
   * list of error messages.
   */
  void reset ();

  /**
   * Called by check().  Override this method to define your own subset.
   */
  virtual void doCheck (const Model& m) = 0;

  /**
   * Checks that the id associated with the given object is unique.  If it
   * is not, logFailure is called.
   */
  void doCheckId (const std::string& id, const SBase& object);


  /**
   * Returns the error message to use when logging constraint violations.
   * This method is called by logFailure.
   *
   * Returns a message that the given @p id and its corresponding object are
   * in  conflict with an object previously defined.
   *
   * @return the error message to use when logging constraint violations.
   */
  virtual const std::string
  getMessage (const std::string& id, const SBase& object);


  typedef std::map<std::string, const SBase*> IdObjectMap;
  IdObjectMap mIdObjectMap;
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* UniqueCompIdBase_h */
/** @endcond */


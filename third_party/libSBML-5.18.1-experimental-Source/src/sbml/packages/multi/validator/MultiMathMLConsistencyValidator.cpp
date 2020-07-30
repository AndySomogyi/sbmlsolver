  /** @cond doxygenLibsbmlInternal */

/**
 * @file:   MultiMathMLConsistencyValidator.cpp
 * @brief:  Implementation of the MultiMathMLConsistencyValidator class
 * @author: Fengkai Zhang
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


#ifndef doxygen_ignore

#include <sbml/packages/multi/validator/MultiMathMLConsistencyValidator.h>

#include "constraints/MultiMathMLConsistencyConstraints.cpp"

LIBSBML_CPP_NAMESPACE_BEGIN
#ifdef __cplusplus

void
MultiMathMLConsistencyValidator::init ()
{
#define  AddingConstraintsToValidator 1
#include "constraints/MultiMathMLConsistencyConstraintsDeclared.cxx"
}

#endif /* __cplusplus */

LIBSBML_CPP_NAMESPACE_END

#endif  /* __doxygen_ignore */

  /** @endcond */



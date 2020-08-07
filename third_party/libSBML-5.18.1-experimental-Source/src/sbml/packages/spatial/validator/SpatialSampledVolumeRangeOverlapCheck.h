/**
* @cond doxygenLibsbmlInternal
*
* @file    SpatialSampledVolumeRangeOverlapCheck.h
* @brief   Ensure that all samples volume values are unique.
* @author  Sarah Keating, Lucian Smith
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

#ifndef SpatialSampledVolumeRangeOverlapCheck_h
#define SpatialSampledVolumeRangeOverlapCheck_h


#ifdef __cplusplus

#include <sbml/validator/VConstraint.h>
#include <sbml/packages/spatial/validator/SpatialValidator.h>


LIBSBML_CPP_NAMESPACE_BEGIN

class GeometryDefinition;
class SampledFieldGeometry;

class SpatialSampledVolumeRangeOverlapCheck: public TConstraint<Model>
{
public:

  /**
  * Creates a new Constraint with the given constraint id.
  */
  SpatialSampledVolumeRangeOverlapCheck (unsigned int id, SpatialValidator& v);

  /**
  * Destroys this Constraint.
  */
  virtual ~SpatialSampledVolumeRangeOverlapCheck ();


protected:

  virtual void check_ (const Model& m, const Model& object);

private:
  virtual void checkGeometryDefinition(const GeometryDefinition* gd, const Model& m);
  virtual void checkSampledFieldGeometry(const SampledFieldGeometry* sfg, const Model& m);

};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* SpatialSampledVolumeRangeOverlapCheck_h */
/** @endcond */

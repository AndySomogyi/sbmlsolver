/**
 * @file    Transformation.cpp
 * @brief   class representing a 3D affine transformation
 * @author  Ralph Gauges
 *
 */
/* Copyright 2010 Ralph Gauges
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is
 * provided in the file named "LICENSE.txt" included with this software
 * distribution.  It is also available online at
 * http://sbml.org/software/libsbml/license.html
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * The original code contained here was initially developed by:
 *
 *     Ralph Gauges
 *     Group for the modeling of biological processes 
 *     University of Heidelberg
 *     Im Neuenheimer Feld 267
 *     69120 Heidelberg
 *     Germany
 *
 *     mailto:ralph.gauges@bioquant.uni-heidelberg.de
 *
 * Contributor(s):
 */

#include "Transformation.h"
#include <limits>
#ifndef OMIT_DEPRECATED
#ifdef DEPRECATION_WARNINGS
#include <iostream>
#endif // DEPRECATION_WARNINGS
#endif // OMIT_DEPRECATED

LIBSBML_CPP_NAMESPACE_BEGIN


const double Transformation::IDENTITY3D[12]={1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0};

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new Transformation object with the given SBML level
 * and SBML version.
 *
 * @param level SBML level of the new object
 * @param level SBML version of the new object
 */
Transformation::Transformation (unsigned int level, unsigned int version, unsigned int pkgVersion) : 
    SBase(level,version)
{
    setSBMLNamespacesAndOwn(new RenderPkgNamespaces(level,version,pkgVersion));  
    unsigned int i;
    for(i=0;i<12;++i)
    {
        mMatrix[i]=std::numeric_limits<double>::quiet_NaN();
    }
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new Transformation object with the given SBMLNamespaces.
 *
 * @param sbmlns The SBML namespace for the object.
 */
Transformation::Transformation (RenderPkgNamespaces* renderns):
    SBase(renderns)
{
    unsigned int i;
    for(i=0;i<12;++i)
    {
        mMatrix[i]=std::numeric_limits<double>::quiet_NaN();
    }
        // set the element namespace of this object
  setElementNamespace(renderns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(renderns);
}
/** @endcond */

/*
 * Copy constructor.
 */
Transformation::Transformation(const Transformation& other)
  : SBase(other)
{
  setMatrix(other.getMatrix());
}

/*
 * Destroy this object.
 */
Transformation::~Transformation ()
{
}


/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new Transformation object from the given XMLNode object.
 * The XMLNode object has to contain a valid XML representation of a 
 * Transformation object as defined in the render extension specification.
 * This method is normally called when render information is read from a file and 
 * should normally not have to be called explicitely.
 *
 * @param node the XMLNode object reference that describes the Transformation
 * object to be instantiated.
 */
Transformation::Transformation(const XMLNode& node, unsigned int l2version)
  : SBase(2, l2version)
{
  mURI = RenderExtension::getXmlnsL3V1V1();
  unsigned int i;
  for(i=0;i<12;++i)
  {
      mMatrix[i]=std::numeric_limits<double>::quiet_NaN();
  }

  
  setSBMLNamespacesAndOwn(new RenderPkgNamespaces(2,l2version));  

  connectToChild();
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Sets the matrix to the values given in the array.
 *
 * @param m array with new values to be set for this Transformation object.
 */
void Transformation::setMatrix(const double m[12])
{
    unsigned int i;
    for(i=0;i<12;++i)
    {
        mMatrix[i]=m[i];
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the matrix which is an array of double values of length 12.
 *
 * @return a pointer to the array of numbers for the transformation.
 */
const double* Transformation::getMatrix() const
{
    return mMatrix;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a 3D identity matrix.
 * The matrix contains 12 double values.
 */
const double* Transformation::getIdentityMatrix()
{
    return IDENTITY3D;
}
/** @endcond */



/** @cond doxygenLibsbmlInternal */
/*
 * Returns true if the matrix has been set or false otherwise.
 * The matrix is considered as set if none of the values in the matrix is NaN.
 *
 * @return true or false depending on whether a NaN was found.
 */
bool Transformation::isSetMatrix() const
{
  if (this == NULL) return false;
    bool result=true;
    unsigned int i;
    for(i=0;result && i<12;++i)
    {
        result=(mMatrix[i]==mMatrix[i]);
    }
    return result;
}
/** @endcond */


LIBSBML_CPP_NAMESPACE_END 

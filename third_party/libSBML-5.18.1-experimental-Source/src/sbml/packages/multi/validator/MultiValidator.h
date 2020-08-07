/**
 * @file:   MultiValidator.h
 * @brief:  Implementation of the MultiValidator class
 * @author: SBMLTeam
 * @author: Fengkai Zhang
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
 *
 * @class MultiValidator
 * @sbmlbrief{multi} Entry point for package validation.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * @copydetails doc_common_intro_package_validators
 *
 * The MultiValidator class extends the Validator class from core libSBML to
 * apply validation to the constructs introduced by the SBML Level&nbsp;3
 * @ref multi (&ldquo;multi&rdquo;) package.  This class then acts as a base
 * class for any validators that apply rules to the &ldquo;fbc&rdquo; package
 * specification constructs or to entire models that use the
 * &ldquo;multi&rdquo; package, and may therefore be subject to other global
 * restrictions introduced.
 *
 * @copydetails doc_section_package_validators_general_info
 */

#ifndef MultiValidator__H
#define MultiValidator__H


#ifdef __cplusplus


  /** @cond doxygenLibsbmlInternal */

#include <list>
#include <string>

  /** @endcond */


#include <sbml/SBMLError.h>
#include <sbml/validator/Validator.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class VConstraint;
struct MultiValidatorConstraints;
class SBMLDocument;


class MultiValidator : public Validator
{
public:

  /**
   * Constructor; creates a new MultiValidator object for the given
   * category of validation.
   *
   * @param category code indicating the kind of validations that this
   * validator will perform.  The category code value must be
   * @if clike taken from the enumeration #SBMLErrorCategory_t @endif@~
   * @if java one of of the values from the set of constants whose names
   * begin with the characters <code>LIBSBML_CAT_</code> in the interface
   * class {@link libsbmlConstants}.@endif@~
   * @if python one of of the values from the set of constants whose names
   * begin with the characters <code>LIBSBML_CAT_</code> in the interface
   * class @link libsbml libsbml@endlink.@endif@~
   */
  MultiValidator ( SBMLErrorCategory_t category = LIBSBML_CAT_SBML );


  /**
   * Destroys this MultiValidator object.
   */
  virtual ~MultiValidator ();


  /**
   * Initializes this MultiValidator object.
   *
   * When creating a subclass of MultiValidator, override this method to add
   * your own validation code.
   */
  virtual void init () = 0;


  /**
   * Adds the given VContraint object to this validator.
   *
   * @param c the VConstraint ("validator constraint") object to add.
   */
  virtual void addConstraint (VConstraint* c);


  /**
   * Validates the given SBML document.
   *
   * @param d the SBMLDocument object to be validated.
   *
   * @return the number of validation failures that occurred.  The objects
   * describing the actual failures can be retrieved using getFailures().
   */
  virtual unsigned int validate (const SBMLDocument& d);


  /**
   * Validates the SBML document located at the given file name.
   *
   * @param filename the path to the file to be read and validated.
   *
   * @return the number of validation failures that occurred.  The objects
   * describing the actual failures can be retrieved using getFailures().
   */
  virtual unsigned int validate (const std::string& filename);


protected:
  /** @cond doxygenLibsbmlInternal */

  MultiValidatorConstraints* mMultiConstraints;

  friend class MultiValidatingVisitor;

  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus*/
#endif  /* MultiValidator__H */

/**
 * @file    Reaction.h
 * @brief   Definitions of Reaction and ListOfReactions.
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
 * ------------------------------------------------------------------------ -->
 * 
 * @class Reaction
 * @sbmlbrief{core} An SBML reaction between species in an SBML model.
 *
 * A @em reaction represents any transformation, transport or binding
 * process, typically a chemical reaction, that can change the quantity of
 * one or more species.  In SBML, a reaction is defined primarily in terms
 * of the participating reactants and products (and their corresponding
 * stoichiometries), along with optional modifier species, an optional rate
 * at which the reaction takes place, and optional parameters.
 *
 * As with other major objects in SBML, Reaction has a mandatory attribute,
 * "id", used to give the reaction an identifier.  The identifier
 * must be a text string conforming to the identifer syntax permitted in
 * SBML.  In SBML Level&nbsp;2 and Level&nbsp;3, the reaction "id"
 * identifier can be used in mathematical formulas elsewhere in an SBML
 * model to represent the rate of that reaction; this usage is explained
 * below.  Reaction also has an optional "name" attribute, of type
 * @c string.  The "id" and "name" must be used according to the guidelines
 * described in the SBML specification.
 *
 * The species participating as reactants, products, and/or modifiers in a
 * reaction are declared using lists of SpeciesReference and/or
 * ModifierSpeciesReference instances stored in subelements
 * "listOfReactants", "listOfProducts" and "listOfModifiers".  Certain
 * restrictions are placed on the appearance of species in reaction
 * definitions:
 * <ul>
 * <li> The ability of a species to appear as a reactant or product of any
 * reaction in a model is governed by certain flags in that species'
 * definition; see the definition of Species for more information.
 *
 * <li> Any species appearing in the mathematical formula of the subelement
 * "kineticLaw" (described below) of a Reaction must be declared in at
 * least one of that Reaction's lists of reactants, products, and/or
 * modifiers.  Put another way, it is an error for a reaction's kinetic law
 * formula to refer to species that have not been declared for that
 * reaction.
 *
 * <li> For SBML Levels 1, 2, and SBML Level&nbsp;3 Version&nbsp;1, a 
 * reaction definition can contain an empty list of reactants
 * <em>or</em> an empty list of products, but it must have at least one
 * reactant or product; in other words, a reaction without any reactant or
 * product species is not permitted.  (This restriction does not apply to
 * modifier species, which remain optional in all cases.)  In SBML 
 * Level&nbsp;3 Version&nbsp;2, this requirement was dropped, allowing 
 * the creation of reactions with neither reactants nor products.
 * </ul>
 * 
 * A reaction can contain up to one KineticLaw object in a subelement named
 * "kineticLaw".  It defines the speed at which the process defined by the
 * reaction takes place.  The description of KineticLaw provides more
 * details about its use.  Note that although the inclusion of a KineticLaw
 * object in an instance of a Reaction component is optional, there is no
 * useful default that can be substituted in place of a missing rate
 * expression in a reaction.  Moreover, a reaction's rate cannot be defined
 * in any other way in SBML---InitialAssignment, AssignmentRule,
 * RateRule, AlgebraicRule, Event, and other constructs in SBML cannot be
 * used to set the reaction rate separately.  Nevertheless, for some
 * modeling applications, reactions without any defined rate can be
 * perfectly acceptable.
 *
 * Reaction also has a boolean attribute named "reversible" for indicating
 * whether the reaction is reversible.  This attribute is optional in SBML
 * Level&nbsp;2, with a default of @c true; it is mandatory in SBML
 * Level&nbsp;3 (with no default value).  To say that a reaction is @em
 * reversible is to say it can proceed in either the forward or the reverse
 * direction.  Although the reversibility of a reaction can sometimes be
 * deduced by inspecting its rate expression, this is not always the case,
 * especially for complicated expressions.  Moreover, the need in SBML to
 * allow rate expressions (i.e., KineticLaw) to be optional leads to the
 * need for a separate flag indicating reversibility.  Note that labeling a
 * reaction as irreversible is an assertion that the reaction always
 * proceeds in the given forward direction.  (Why else would it be flagged
 * as irreversible?)  This implies the rate expression in the KineticLaw
 * always has a non-negative value during simulations.  Software tools
 * could provide a means of optionally testing that this condition holds.
 * The presence of reversibility information in two places (i.e., the rate
 * expression and the "reversible" attribute on Reaction) leaves open the
 * possibility that a model could contain contradictory information, but
 * the creation of such a model would be an error on the part of the
 * software generating it.
 *
 * The Reaction object class has another boolean attribute called "fast".
 * This attribute is optional in SBML Level&nbsp;2, with a default of
 * @c false; it is mandatory in SBML Level&nbsp;3 (with no default value).  
 * In SBML Level&nbsp;3 Version&nbsp;2, a value of @c true for the "fast"
 * attribute is deprecated in favor of all reactions having a "fast" value 
 * of @c false.  It
 * is used to indicate that a reaction occurs on a vastly faster time scale
 * than others in a system.  Readers are directed to the SBML Level&nbsp;2
 * Version&nbsp;4 specification, which provides more detail about the
 * conditions under which a reaction can be considered to be fast in this
 * sense.  SBML Level&nbsp;1
 * and Level&nbsp;2 Version&nbsp;1 incorrectly claimed that software tools
 * could ignore this attribute if they did not implement support for the
 * corresponding concept; however, further research in SBML has revealed
 * that this is not true, and "fast" <em>cannot be ignored</em> if it is
 * set to @c true.  SBML Level&nbsp;2 Versions&nbsp;2&ndash;4 therefore
 * stipulate that if a model has any reactions with "fast" set to @c true,
 * a software tool must be able to respect the attribute or else indicate
 * to the user that it does not have the capacity to do so.  Analysis
 * software cannot ignore the value of the "fast" attribute because doing
 * so may lead to different results as compared to a software system that
 * <em>does</em> make use of "fast".
 *
 * @copydetails doc_fast_attribute_removed
 *
 * In SBML Level&nbsp;3, the Reaction object has an
 * additional optional attribute named "compartment", whose value must be
 * the identifier of a compartment defined in the enclosing Model object.
 * The "compartment" attribute can be used to indicate the compartment in
 * which the reaction is assumed to take place.  If the attribute is
 * present, its value must be the identifier of a Compartment object
 * defined in the enclosing Model object.  Similar to the "reversible"
 * attribute, the value of the "compartment" attribute has no direct impact
 * on the construction of mathematical equations for the SBML model.  When
 * a kinetic law is given for a reaction, the compartment location may
 * already be implicit in the kinetic law (although this cannot always be
 * guaranteed).  Nevertheless, software tools may find the "compartment"
 * attribute value useful for such purposes as analyzing the structure of
 * the model, guiding the modeler in constructing correct rate formulas,
 * and visualization purposes.
 *
 * Readers are urged to read the SBML specification for more details about
 * the proper use of Reaction.
 * 
 * 
 * <!-- ------------------------------------------------------------------- -->
 * @class ListOfReactions
 * @sbmlbrief{core} A list of Reaction objects.
 * 
 * @copydetails doc_what_is_listof
 */

/**
 * <!-- ~ ~ ~ ~ ~ Start of common documentation strings ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
 * The following text is used as common documentation blocks copied multiple
 * times elsewhere in this file.  The use of @class is a hack needed because
 * Doxygen's @copydetails command has limited functionality.  Symbols
 * beginning with "doc_" are marked as ignored in our Doxygen configuration.
 * ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  -->
 *
 * @class doc_warning_reaction_cant_ignore_fast
 * 
 * @warning <span class="warning">SBML definitions before SBML Level&nbsp;2
 * Version&nbsp;2 incorrectly indicated that software tools could ignore the
 * "fast" attribute if they did not implement support for the corresponding
 * concept; however, further research in SBML has revealed that this is not
 * true, and "fast" <em>cannot be ignored</em> if it is set to @c true.
 * Beginning with SBML Level&nbsp;2 Versions&nbsp;2, the SBML specifications
 * therefore stipulate that if a model has any reactions with "fast" set to
 * @c true, a software tool must be able to respect the attribute or else
 * indicate to the user that it does not have the capacity to do so.  Readers
 * are directed to the SBML specifications, which provides more detail about
 * the conditions under which a reaction can be considered to be fast in this
 * sense.</span>
 *
 */

#ifndef Reaction_h
#define Reaction_h


#include <sbml/common/sbmlfwd.h>


#ifdef __cplusplus


#include <string>

#include <sbml/SBase.h>
#include <sbml/ListOf.h>
#include <sbml/SpeciesReference.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class KineticLaw;
class SBMLVisitor;


class LIBSBML_EXTERN Reaction : public SBase
{
public:

  /**
   * Creates a new Reaction using the given SBML @p level and @p version
   * values.
   *
   * @param level an unsigned int, the SBML Level to assign to this Reaction.
   *
   * @param version an unsigned int, the SBML Version to assign to this
   * Reaction.
   *
   * @copydetails doc_throw_exception_lv
   *
   * @copydetails doc_note_setting_lv
   */
  Reaction (unsigned int level, unsigned int version);


  /**
   * Creates a new Reaction using the given SBMLNamespaces object
   * @p  sbmlns.
   *
   * @copydetails doc_what_are_sbmlnamespaces
   *
   * @param sbmlns an SBMLNamespaces object.
   *
   * @copydetails doc_throw_exception_namespace
   *
   * @copydetails doc_note_setting_lv
   */
  Reaction (SBMLNamespaces* sbmlns);


  /**
   * Destroys this Reaction.
   */
  virtual ~Reaction ();


  /**
   * Copy constructor; creates a copy of this Reaction.
   *
   * @param orig the object to copy.
   */
  Reaction (const Reaction& orig);


  /**
   * Assignment operator for Reaction.
   *
   * @param rhs the object whose values are used as the basis of the
   * assignment.
   */
  Reaction& operator=(const Reaction& rhs);


  /** @cond doxygenLibsbmlInternal */
  /**
   * Accepts the given SBMLVisitor for this instance of Reaction.
   *
   * @param v the SBMLVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept (SBMLVisitor& v) const;
  /** @endcond */


  /**
   * Creates and returns a deep copy of this Reaction object.
   *
   * @return the (deep) copy of this Reaction object.
   */
  virtual Reaction* clone () const;


   /**
   * Returns the first child element found that has the given @p id in the
   * model-wide SId namespace, or @c NULL if no such object is found.
   *
   * @param id string representing the id of the object to find.
   *
   * @return pointer to the first element found with the given @p id.
   */
  virtual SBase* getElementBySId(const std::string& id);
  
  
  /**
   * Returns the first child element it can find with the given @p metaid, or
   * @c NULL if no such object is found.
   *
   * @param metaid string representing the metaid of the object to find.
   *
   * @return pointer to the first element found with the given @p metaid.
   */
  virtual SBase* getElementByMetaId(const std::string& metaid);
  
  
  /**
   * Returns a List of all child SBase objects, including those nested to an
   * arbitrary depth
   *
   * @param filter a pointer to an ElementFilter, which causes the function 
   * to return only elements that match a particular set of constraints.  
   * If NULL (the default), the function will return all child objects.
   *
   * @return a List of pointers to all children objects.
   */
  virtual List* getAllElements(ElementFilter* filter=NULL);
  
  
  /**
   * @copydoc doc_renamesidref_common
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * Initializes the fields of this Reaction object to "typical" default
   * values.
   *
   * The SBML Reaction component has slightly different aspects and
   * default attribute values in different SBML Levels and Versions.
   * This method sets the values to certain common defaults, based
   * mostly on what they are in SBML Level&nbsp;2.  Specifically:
   * 
   * @li Sets the "reversible" attribute to @c true
   * @li Sets the "fast" attribute to @c false
   * @li Marks the "fast" attribute as @em not having been set for
   *     SBML Level&nbsp;2, but @em as having been set for
   *     SBML Level&nbsp;1 and SBML Level&nbsp;3.
   *
   * @copydetails doc_warning_reaction_cant_ignore_fast
   */
  void initDefaults ();


  /**
   * Returns the value of the "id" attribute of this Reaction.
   *
   * @note Because of the inconsistent behavior of this function with 
   * respect to assignments and rules, it is now recommended to
   * use the getIdAttribute() function instead.
   *
   * @copydetails doc_id_attribute
   *
   * @return the id of this Reaction.
   *
   * @see getIdAttribute()
   * @see setIdAttribute(const std::string& sid)
   * @see isSetIdAttribute()
   * @see unsetIdAttribute()
   */
  virtual const std::string& getId () const;


  /**
   * Returns the value of the "name" attribute of this Reaction object.
   *
   * @copydetails doc_get_name
   */
  virtual const std::string& getName () const;


  /**
   * Returns the KineticLaw object contained in this Reaction.
   * 
   * @return the KineticLaw instance.
   */
  const KineticLaw* getKineticLaw () const;


  /**
   * Returns the KineticLaw object contained in this Reaction.
   * 
   * @return the KineticLaw instance.
   */
  KineticLaw* getKineticLaw ();


  /**
   * Returns the value of the "reversible" attribute on the Reaction as a
   * boolean value.
   * 
   * @return the reversibility status of this Reaction.
   */
  bool getReversible () const;


  /**
   * Returns the value of the "fast" attribute of this Reaction.
   * 
   * @copydetails doc_fast_attribute_removed
   *
   * @return the "fast" status of this Reaction.
   *
   * @copydetails doc_warning_reaction_cant_ignore_fast
   */
  bool getFast () const;


  /**
   * (SBML Level&nbsp;3 only) Returns the value of the "compartment"
   * attribute on the Reaction.
   * 
   * @return the compartment of this Reaction.
   *
   * @note The "compartment" attribute is available in SBML Level&nbsp;3,
   * but is not present on Reaction in lower Levels of
   * SBML.
   */
  const std::string& getCompartment () const;


  /**
   * Predicate returning @c true if this
   * Reaction's "id" attribute is set.
   *
   * @copydetails doc_isset_id
   */
  virtual bool isSetId () const;


  /**
   * Predicate returning @c true if this
   * Reaction's "name" attribute is set.
   *
   * @copydetails doc_isset_name
   */
  virtual bool isSetName () const;


  /**
   * Predicate returning @c true if this
   * Reaction contains a kinetic law object.
   *
   * @return @c true if a KineticLaw is present in this Reaction,, @c false
   * otherwise.
   */
  bool isSetKineticLaw () const;


  /**
   * Predicate returning @c true if the value of
   * the "fast" attribute on this Reaction is set.
   *
   * @copydetails doc_fast_attribute_removed
   *
   * @return @c true if the "fast" attribute is set, @c false otherwise.
   *
   * @copydetails doc_warning_reaction_cant_ignore_fast
   */
  bool isSetFast () const;


  /**
   * Predicate returning @c true if this
   * Reaction's "compartment" attribute is set.
   *
   * @return @c true if the "compartment" attribute of this Reaction is
   * set, @c false otherwise.
   *
   * @note The "compartment" attribute is available in SBML
   * Level&nbsp;3, but is not present on Reaction in
   * lower Levels of SBML.
   */
  bool isSetCompartment () const;


  /**
   * Predicate returning @c true if this
   * Reaction's "reversible" attribute is set.
   *
   * @return @c true if the "reversible" attribute of this Reaction is
   * set, @c false otherwise.
   */
  bool isSetReversible () const;


  /**
   * Sets the value of the "id" attribute of this Reaction.
   *
   * @copydetails doc_set_id
   */
  virtual int setId(const std::string& sid);


  /**
   * Sets the value of the "name" attribute of this Reaction.
   *
   * @copydetails doc_set_name
   */
  virtual int setName (const std::string& name);


  /**
   * Sets the "kineticLaw" subelement of this Reaction to a copy of the
   * given KineticLaw object.
   *
   * @param kl the KineticLaw object to use.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
  */
  int setKineticLaw (const KineticLaw* kl);


  /**
   * Sets the value of the "reversible" attribute of this Reaction.
   *
   * @param value the value of the "reversible" attribute.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   */
  int setReversible (bool value);


  /**
   * Sets the value of the "fast" attribute of this Reaction.
   *
   * @copydetails doc_fast_attribute_removed
   *
   * Calling this function with an argument of @c true for an
   * SBML Level&nbsp;3 Version&nbsp;2 Reaction will not set
   * the value, as the attribute does not exist for that level/version.
   * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
   *
   * @param value the value of the "fast" attribute.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
   *
   * @copydetails doc_warning_reaction_cant_ignore_fast
   */
  int setFast (bool value);


  /**
   * Sets the value of the "compartment" attribute of this Reaction.
   *
   * The string @p sid is copied.  
   *
   * @param sid the string to use as the compartment of this Reaction.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   *
   * @note The "compartment" attribute is available in SBML
   * Level&nbsp;3, but is not present on Reaction in
   * lower Levels of SBML.
   */
  int setCompartment (const std::string& sid);


  /**
   * Unsets the value of the "name" attribute of this Reaction.
   *
   * @copydetails doc_unset_name
   */
  virtual int unsetName ();


  /**
   * Unsets the "kineticLaw" subelement of this Reaction.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetKineticLaw ();


  /**
   * Unsets the value of the "fast" attribute of this Reaction.
   *
   * @copydetails doc_fast_attribute_removed
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
   *
   * @copydetails doc_warning_reaction_cant_ignore_fast
   */
  int unsetFast ();


  /**
   * Unsets the value of the "compartment" attribute of this Reaction.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   *
   * @note The "compartment" attribute is available in SBML
   * Level&nbsp;3, but is not present on Reaction in
   * lower Levels of SBML.
   */
  int unsetCompartment ();


  /**
   * Unsets the value of the "reversible" attribute of this Reaction.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  int unsetReversible ();


  /**
   * Adds a given SpeciesReference object as a reactant in this Reaction.
   *
   * The SpeciesReference instance in @p sr is copied.
   *
   * @param sr a SpeciesReference object referring to a Species in the
   * enclosing Model.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied 
   *
   * @see createReactant()
   */
  int addReactant (const SpeciesReference* sr);

  /**
   * Adds the given species as a reactant with the given stoichiometry
   *
   * @param species the species to be added as reactant.
   *
   * @param stoichiometry an optional parameter specifying the
   *        stoichiometry of the added reactant (defaulting to 1).
   *
   * @param id an optional id to be given to the species reference that will
   *        be created. (defaulting to empty string, i.e. not set).
   *
   * @param constant an attribute specifying whether the species reference is
   *        constant or not (defaulting to true).
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied
   *
   * @note the Species object itself is NOT added to the model
   *
   * @see createProduct()
   */
  int addReactant(const Species* species,
                  double stoichiometry = 1.0,
                  const std::string id = "",
                  bool constant = true);

  /**
   * Adds a given SpeciesReference object as a product in this Reaction.
   *
   * The SpeciesReference instance in @p sr is copied.
   *
   * @param sr a SpeciesReference object referring to a Species in the
   * enclosing Model.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied 
   *
   * @see createProduct()
   */
  int addProduct (const SpeciesReference* sr);

  /**
   * Adds the given species as a product with the given stoichiometry
   *
   * @param species the species to be added as product.
   *
   * @param stoichiometry an optional parameter specifying the
   *        stoichiometry of the added product (defaulting to 1).
   *
   * @param id an optional id to be given to the species reference that will
   *        be created. (defaulting to empty string, i.e. not set).
   *
   * @param constant an attribute specifying whether the species reference is
   *        constant or not (defaulting to true).
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied
   *
   * @note the Species object itself is NOT added to the model
   *
   * @see createProduct()
   */
  int addProduct(const Species* species,
                 double stoichiometry = 1.0,
                 const std::string id = "",
                 bool constant = true);

  /**
   * Adds a given ModifierSpeciesReference object as a product in this
   * Reaction.
   *
   * The ModifierSpeciesReference instance in @p msr is copied.
   *
   * @param msr a ModifierSpeciesReference object referring to a Species in
   * the enclosing Model.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied 
   *
   * @see createModifier()
   */
  int addModifier (const ModifierSpeciesReference* msr);

  /**
   * Adds the given species as a modifier to this reaction
   *
   * @param species the species to be added as modifier.
   *
   * @param id an optional id to be given to the species reference that will
   *        be created. (defaulting to empty string, i.e. not set).
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_DUPLICATE_OBJECT_ID, OperationReturnValues_t}
   *
   * @copydetails doc_note_object_is_copied
   *
   * @note the Species object itself is NOT added to the model
   *
   * @see createModifier()
   */
  int addModifier(const Species *species,
                  const std::string id = "");

  /**
   * Creates a new SpeciesReference, adds it to this Reaction's list of
   * reactants, and returns it.
   *
   * @return a new SpeciesReference object.
   */
  SpeciesReference* createReactant ();


  /**
   * Creates a new SpeciesReference, adds it to this Reaction's list of
   * products, and returns it.
   *
   * @return a new SpeciesReference object.
   */
  SpeciesReference* createProduct ();


  /**
   * Creates a new ModifierSpeciesReference, adds it to this Reaction's
   * list of modifiers and returns it.
   *
   * @return a new ModifierSpeciesReference object.
   */
  ModifierSpeciesReference* createModifier ();


  /**
   * Creates a new KineticLaw object, installs it as this Reaction's
   * "kineticLaw" subelement, and returns it.
   *
   * If this Reaction had a previous KineticLaw, it will be destroyed.
   *
   * @return the new KineticLaw object.
   */
  KineticLaw* createKineticLaw ();


  /**
   * Returns the list of reactants in this Reaction object.
   * 
   * @return the ListOfSpeciesReferences containing the references to the
   * species acting as reactants in this reaction.
   */
  const ListOfSpeciesReferences* getListOfReactants () const;


  /**
   * Returns the list of reactants in this Reaction object.
   * 
   * @return the ListOfSpeciesReferences containing the references to the
   * species acting as reactants in this reaction.
   */
  ListOfSpeciesReferences* getListOfReactants ();


  /**
   * Returns the list of products in this Reaction object.
   * 
   * @return the ListOfSpeciesReferences containing the references to the
   * species acting as products in this reaction.
   */
  const ListOfSpeciesReferences* getListOfProducts () const;


  /**
   * Returns the list of products in this Reaction object.
   * 
   * @return the ListOfSpeciesReferences containing the references to the
   * species acting as products in this reaction.
   */
  ListOfSpeciesReferences* getListOfProducts ();


  /**
   * Returns the list of modifiers in this Reaction object.
   * 
   * @return the ListOfSpeciesReferences containing the references to the
   * species acting as modifiers in this reaction.
   */
  const ListOfSpeciesReferences* getListOfModifiers () const;


  /**
   * Returns the list of modifiers in this Reaction object.
   * 
   * @return the ListOfSpeciesReferences containing the references to the
   * species acting as modifiers in this reaction.
   */
  ListOfSpeciesReferences* getListOfModifiers ();


  /**
   * Returns the nth reactant species (as a SpeciesReference object) in 
   * the list of reactants in this Reaction.
   *
   * Callers should first call getNumReactants() to find out how many
   * reactants there are, to avoid using an invalid index number.
   *
   * @param n the index of the reactant sought.
   * 
   * @return the nth reactant (as a SpeciesReference object) of this
   * Reaction.
   * If the index @p n is invalid, @c NULL is returned.
   */
  const SpeciesReference* getReactant (unsigned int n) const;


  /**
   * Returns the nth reactant species (as a SpeciesReference object) 
   * in the list of reactants in this Reaction.
   *
   * Callers should first call getNumReactants() to find out how many
   * reactants there are, to avoid using an invalid index number.
   *
   * @param n the index of the reactant sought.
   * 
   * @return the nth reactant (as a SpeciesReference object) of this
   * Reaction.
   * If the index @p n is invalid, @c NULL is returned.
   */
  SpeciesReference* getReactant (unsigned int n);


  /**
   * Returns the reactant species (as a SpeciesReference object) having 
   * a specific identifier in this Reaction.
   *
   * @param species the identifier of the reactant Species ("species" 
   * attribute of the reactant SpeciesReference object).
   *
   * @return a SpeciesReference object, or @c NULL if no species with the
   * given identifier @p species appears as a reactant in this Reaction.
   */
  const SpeciesReference* getReactant (const std::string& species) const;


  /**
   * Returns the reactant species (as a SpeciesReference object) having 
   * a specific identifier in this Reaction.
   *
   * @param species the identifier of the reactant Species ("species" 
   * attribute of the reactant SpeciesReference object).
   *
   * @return a SpeciesReference object, or @c NULL if no species with the
   * given identifier @p species appears as a reactant in this Reaction.
   */
  SpeciesReference* getReactant (const std::string& species);


  /**
   * Returns the nth product species (as a SpeciesReference object) in 
   * the list of products in this Reaction.
   *
   * Callers should first call getNumProducts() to find out how many
   * products there are, to avoid using an invalid index number.
   *
   * @param n the index of the product sought.
   * 
   * @return the nth product (as a SpeciesReference object) of this
   * Reaction.
   * If the index @p n is invalid, @c NULL is returned.
   */
  const SpeciesReference* getProduct (unsigned int n) const;


  /**
   * Returns the nth product species (as a SpeciesReference object) 
   * in the list of products in this Reaction.
   *
   * Callers should first call getNumProducts() to find out how many
   * products there are, to avoid using an invalid index number.
   *
   * @param n the index of the product sought.
   * 
   * @return the nth product (as a SpeciesReference object) of this
   * Reaction.
   * If the index @p n is invalid, @c NULL is returned.
   */
  SpeciesReference* getProduct (unsigned int n);


  /**
   * Returns the product species (as a SpeciesReference object) having 
   * a specific identifier in this Reaction.
   *
   * @param species the identifier of the product Species ("species"
   * attribute of the product SpeciesReference object).
   *
   * @return a SpeciesReference object, or @c NULL if no species with the
   * given identifier @p species appears as a product in this Reaction.
   */
  const SpeciesReference* getProduct (const std::string& species) const;


  /**
   * Returns the product species (as a SpeciesReference object) having 
   * a specific identifier in this Reaction.
   *
   * @param species the identifier of the product Species ("species"
   * attribute of the product SpeciesReference object).
   *
   * @return a SpeciesReference object, or @c NULL if no species with the
   * given identifier @p species appears as a product in this Reaction.
   */
  SpeciesReference* getProduct (const std::string& species);


  /**
   * Returns the nth modifier species (as a ModifierSpeciesReference object) 
   * in the list of modifiers of this Reaction.
   *
   * Callers should first call getNumModifiers() to find out how many
   * modifiers there are, to avoid using an invalid index number.
   *
   * @param n the index of the modifier species sought.
   * 
   * @return the nth modifier (as a ModifierSpeciesReference object) of
   * this Reaction.
   * If the index @p n is invalid, @c NULL is returned.
   */
  const ModifierSpeciesReference* getModifier (unsigned int n) const;


  /**
   * Returns the nth modifier species (as a ModifierSpeciesReference object) 
   * in the list of modifiers of this Reaction.
   *
   * Callers should first call getNumModifiers() to find out how many
   * modifiers there are, to avoid using an invalid index number.
   *
   * @param n the index of the modifier species sought.
   * 
   * @return the nth modifier (as a ModifierSpeciesReference object) of
   * this Reaction.
   * If the index @p n is invalid, @c NULL is returned.
   */
  ModifierSpeciesReference* getModifier (unsigned int n);


  /**
   * Returns the modifier species (as a ModifierSpeciesReference object) 
   * having a specific identifier in this Reaction.
   *
   * @param species the identifier of the modifier Species ("species" 
   * attribute of the ModifierSpeciesReference object).
   *
   * @return a ModifierSpeciesReference object, or @c NULL if no species with
   * the given identifier @p species appears as a modifier in this
   * Reaction.
   */
  const ModifierSpeciesReference*
  getModifier (const std::string& species) const;


  /**
   * Returns the modifier species (as a ModifierSpeciesReference object) 
   * having a specific identifier in this Reaction.
   *
   * @param species the identifier of the modifier Species ("species" 
   * attribute of the ModifierSpeciesReference object).
   *
   * @return a ModifierSpeciesReference object, or @c NULL if no species with
   * the given identifier @p species appears as a modifier in this
   * Reaction.
   */
  ModifierSpeciesReference* getModifier (const std::string& species);


  /**
   * Returns the number of reactant species in this Reaction.
   * 
   * @return the number of reactants in this Reaction.
   */
  unsigned int getNumReactants () const;


  /**
   * Returns the number of product species in this Reaction.
   * 
   * @return the number of products in this Reaction.
   */
  unsigned int getNumProducts () const;


  /**
   * Returns the number of modifier species in this Reaction.
   * 
   * @return the number of modifiers in this Reaction.
   */
  unsigned int getNumModifiers () const;


  /**
   * Removes the nth reactant species (SpeciesReference object) in the list of 
   * reactants in this Reaction and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for deleting it.
   * The caller should first call getNumReactants() to find out how many
   * reactants there are, to avoid using an invalid index number.
   *
   * @param n the index of the reactant SpeciesReference object to remove.
   *
   * @return the removed reactant SpeciesReference object, or @c NULL if the 
   * given index is out of range.
   */
  SpeciesReference* removeReactant (unsigned int n);


  /**
   * Removes the reactant species (SpeciesReference object) having the given  
   * "species" attribute in this Reaction and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for deleting it.
   *
   * @param species the "species" attribute of the reactant SpeciesReference 
   * object.
   *
   * @return the removed reactant SpeciesReference object, or @c NULL if no 
   * reactant SpeciesReference object with the given "species" attribute 
   * @p species exists in this Reaction.
   */
  SpeciesReference* removeReactant (const std::string& species);


  /**
   * Removes the nth product species (SpeciesReference object) in the list of 
   * products in this Reaction and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for deleting it.
   * The caller should first call getNumProducts() to find out how many
   * products there are, to avoid using an invalid index number.
   *
   * @param n the index of the product SpeciesReference object to remove.
   *
   * @return the removed product SpeciesReference object, or @c NULL if the 
   * given index is out of range.
   */
  SpeciesReference* removeProduct (unsigned int n);


  /**
   * Removes the product species (SpeciesReference object) having the given  
   * "species" attribute in this Reaction and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for deleting it.
   *
   * @param species the "species" attribute of the product SpeciesReference 
   * object.
   *
   * @return the removed product SpeciesReference object, or @c NULL if no 
   * product SpeciesReference object with the given "species" attribute 
   * @p species exists in this Reaction.
   */
  SpeciesReference* removeProduct (const std::string& species);


  /**
   * Removes the nth modifier species (ModifierSpeciesReference object) in 
   * the list of  modifiers in this Reaction and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for deleting it.
   * The caller should first call getNumModifiers() to find out how many
   * modifiers there are, to avoid using an invalid index number.
   *
   * @param n the index of the ModifierSpeciesReference object to remove.
   *
   * @return the removed ModifierSpeciesReference object, or @c NULL if the 
   * given index is out of range.
   */
  ModifierSpeciesReference* removeModifier (unsigned int n);


  /**
   * Removes the modifier species (ModifierSpeciesReference object) having 
   * the given "species" attribute in this Reaction and returns a pointer to it.
   *
   * The caller owns the returned object and is responsible for deleting it.
   *
   * @param species the "species" attribute of the ModifierSpeciesReference 
   * object.
   *
   * @return the removed ModifierSpeciesReference object, or @c NULL if no 
   * ModifierSpeciesReference object with the given "species" attribute @p 
   * species exists in this Reaction.
   */
  ModifierSpeciesReference* removeModifier (const std::string& species);


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this SBML object.
   */
  virtual void setSBMLDocument (SBMLDocument* d);


  /**
   * Sets this SBML object to child SBML objects (if any).
   * (Creates a child-parent relationship by the parent)
   *
   * Subclasses must override this function if they define
   * one ore more child elements.
   * Basically, this function needs to be called in
   * constructor, copy constructor and assignment operator.
   *
   * @see setSBMLDocument
   * @see enablePackageInternal
   */
  virtual void connectToChild ();


  /**
   * Enables/Disables the given package with this element and child
   * elements (if any).
   * (This is an internal implementation for enablePackage function)
   *
   * @note Subclasses of the SBML Core package in which one or more child
   * elements are defined must override this function.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);

  /** @endcond */

  /** @cond doxygenLibsbmlInternal */

  virtual void updateSBMLNamespace(const std::string& package,
    unsigned int level, unsigned int version);

  /** @endcond */


  /**
   * Returns the libSBML type code for this SBML object.
   * 
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_REACTION, SBMLTypeCode_t} (default).
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /**
   * Returns the XML element name of this object, which for Reaction, is
   * always @c "reaction".
   * 
   * @return the name of this element, i.e., @c "reaction".
   */
  virtual const std::string& getElementName () const;


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parent's
   * implementation of this method as well.
   */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */


  /**
   * Predicate returning @c true if all the required attributes for this
   * Reaction object have been set.
   *
   * The required attributes for a Reaction object are:
   * @li "id" (or "name" in SBML Level&nbsp;1)
   * @li "fast" (in Level&nbsp;3 Version&nbsp;1 only, where it is defined as a required attribute)
   * @li "reversible" (in Level&nbsp;3 only, where it is defined as a required attribute)
   *
   * @return @c true if the required attributes have been set, @c false
   * otherwise.
   */
  virtual bool hasRequiredAttributes() const ;






  #ifndef SWIG



  /** @cond doxygenLibsbmlInternal */

  /**
   * Gets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName, bool& value)
    const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Gets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName, int& value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Gets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           double& value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Gets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           unsigned int& value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Gets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int getAttribute(const std::string& attributeName,
                           std::string& value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Gets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to retrieve.
   *
   * @param value, the address of the value to record.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  //virtual int getAttribute(const std::string& attributeName,
  //                         const char* value) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Predicate returning @c true if this Reaction's attribute "attributeName"
   * is set.
   *
   * @param attributeName, the name of the attribute to query.
   *
   * @return @c true if this Reaction's attribute "attributeName" has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetAttribute(const std::string& attributeName) const;

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, bool value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, int value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName, double value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName,
                           unsigned int value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int setAttribute(const std::string& attributeName,
                           const std::string& value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Sets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to set.
   *
   * @param value, the value of the attribute to set.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  //virtual int setAttribute(const std::string& attributeName, const char*
  //  value);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Unsets the value of the "attributeName" attribute of this Reaction.
   *
   * @param attributeName, the name of the attribute to query.
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   */
  virtual int unsetAttribute(const std::string& attributeName);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Creates and returns an new "elementName" object in this Reaction.
   *
   * @param elementName, the name of the element to create.
   *
   * @return pointer to the element created.
   */
  virtual SBase* createChildObject(const std::string& elementName);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
  * Add a new "elementName" object to this Reaction.
  *
  * @param elementName, the name of the element to create.
  * @param element, pointer to the element to be added.
  *
  * @copydetails doc_returns_success_code
  * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
  * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
  */
  virtual int addChildObject(const std::string& elementName, const SBase* element);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
  * Removes and returns the "elementName" object with the given id in this Reaction.
  *
  * @param elementName, the name of the element to remove.
  * @param id, the id of the element to remove
  *
  * @return pointer to the element removed.
  */
  virtual SBase* removeChildObject(const std::string& elementName, const std::string& id);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the number of "elementName" in this Reaction.
   *
   * @param elementName, the name of the element to get number of.
   *
   * @return unsigned int number of elements.
   */
  virtual unsigned int getNumObjects(const std::string& elementName);

  /** @endcond */



  /** @cond doxygenLibsbmlInternal */

  /**
   * Returns the nth object of "objectName" in this Reaction.
   *
   * @param elementName, the name of the element to get number of.
   *
   * @param index, unsigned int the index of the object to retrieve.
   *
   * @return pointer to the object.
   */
  virtual SBase* getObject(const std::string& elementName, unsigned int index);

  /** @endcond */




  #endif /* !SWIG */


protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or @c NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);


  /**
   * Subclasses should override this method to get the list of
   * expected attributes.
   * This function is invoked from corresponding readAttributes()
   * function.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parent's implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes,
                               const ExpectedAttributes& expectedAttributes);

  void readL1Attributes (const XMLAttributes& attributes);

  void readL2Attributes (const XMLAttributes& attributes);
  
  void readL3Attributes (const XMLAttributes& attributes);


  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parent's implementation
   * of this method as well.
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;

  bool isExplicitlySetReversible() const { return mExplicitlySetReversible; };
  bool isExplicitlySetFast() const { return mExplicitlySetFast; };

  //std::string mId;
  //std::string mName;
 
  ListOfSpeciesReferences  mReactants;
  ListOfSpeciesReferences  mProducts;
  ListOfSpeciesReferences  mModifiers;

  KineticLaw* mKineticLaw;
  bool        mReversible;
  bool        mFast;

  bool mIsSetFast;
  std::string mCompartment;
  bool mIsSetReversible;

  bool mExplicitlySetReversible;
  bool mExplicitlySetFast;

  /* the validator classes need to be friends to access the 
   * protected constructor that takes no arguments
   */
  friend class Validator;
  friend class ConsistencyValidator;
  friend class IdentifierConsistencyValidator;
  friend class InternalConsistencyValidator;
  friend class L1CompatibilityValidator;
  friend class L2v1CompatibilityValidator;
  friend class L2v2CompatibilityValidator;
  friend class L2v3CompatibilityValidator;
  friend class L2v4CompatibilityValidator;
  friend class MathMLConsistencyValidator;
  friend class ModelingPracticeValidator;
  friend class OverdeterminedValidator;
  friend class SBOConsistencyValidator;
  friend class UnitConsistencyValidator;

  /** @endcond */
};



class LIBSBML_EXTERN ListOfReactions : public ListOf
{
public:

  /**
   * Creates a new ListOfReactions object.
   *
   * The object is constructed such that it is valid for the given SBML
   * Level and Version combination.
   *
   * @param level the SBML Level.
   * 
   * @param version the Version within the SBML Level.
   *
   * @copydetails doc_throw_exception_lv
   *
   * @copydetails doc_note_setting_lv
   */
  ListOfReactions (unsigned int level, unsigned int version);
          

  /**
   * Creates a new ListOfReactions object.
   *
   * The object is constructed such that it is valid for the SBML Level and
   * Version combination determined by the SBMLNamespaces object in @p
   * sbmlns.
   *
   * @param sbmlns an SBMLNamespaces object that is used to determine the
   * characteristics of the ListOfReactions object to be created.
   *
   * @copydetails doc_throw_exception_namespace
   *
   * @copydetails doc_note_setting_lv
   */
  ListOfReactions (SBMLNamespaces* sbmlns);


  /**
   * Creates and returns a deep copy of this ListOfReactions object.
   *
   * @return the (deep) copy of this ListOfReactions object.
   */
  virtual ListOfReactions* clone () const;


  /**
   * Returns the libSBML type code for the objects contained in this ListOf
   * (i.e., Reaction objects, if the list is non-empty).
   * 
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for objects contained in this list:
   * @sbmlconstant{SBML_REACTION, SBMLTypeCode_t} (default).
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getItemTypeCode () const;


  /**
   * Returns the XML element name of this object
   *
   * For ListOfReactions, the XML element name is @c "listOfReactions".
   * 
   * @return the name of this element, i.e., @c "listOfReactions".
   */
  virtual const std::string& getElementName () const;


  /**
   * Get a Reaction from the ListOfReactions.
   *
   * @param n the index number of the Reaction to get.
   * 
   * @return the nth Reaction in this ListOfReactions.
   * If the index @p n is invalid, @c NULL is returned.
   *
   * @see size()
   */
  virtual Reaction * get(unsigned int n); 


  /**
   * Get a Reaction from the ListOfReactions.
   *
   * @param n the index number of the Reaction to get.
   * 
   * @return the nth Reaction in this ListOfReactions.
   * If the index @p n is invalid, @c NULL is returned.
   *
   * @see size()
   */
  virtual const Reaction * get(unsigned int n) const; 


  /**
   * Get a Reaction from the ListOfReactions based on its identifier.
   *
   * @param sid a string representing the identifier of the Reaction to get.
   * 
   * @return Reaction in this ListOfReactions with the given @p sid or
   * @c NULL if no such Reaction exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual Reaction* get (const std::string& sid);


  /**
   * Get a Reaction from the ListOfReactions based on its identifier.
   *
   * @param sid a string representing the identifier of the Reaction to get.
   * 
   * @return Reaction in this ListOfReactions with the given @p sid or
   * @c NULL if no such Reaction exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const Reaction* get (const std::string& sid) const;


  /**
   * Removes the nth item from this ListOfReactions items and returns a
   * pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove.
   *
   * @see size()
   */
  virtual Reaction* remove (unsigned int n);


  /**
   * Removes item in this ListOfReactions items with the given identifier.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * NULL is returned.
   *
   * @param sid the identifier of the item to remove.
   *
   * @return the item removed.  As mentioned above, the caller owns the
   * returned item.
   */
  virtual Reaction* remove (const std::string& sid);


  /** @cond doxygenLibsbmlInternal */
  /**
   * Return the position of this element.
   *
   * @return the ordinal position of the element with respect to its
   * siblings or -1 (default) to indicate the position is not significant.
   */
  virtual int getElementPosition () const;

  /** @endcond */


protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or @c NULL if the token was not recognized.
   */
  virtual SBase* createObject (XMLInputStream& stream);

  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Creates a new Reaction_t structure using the given SBML @p level
 * and @p version values.
 *
 * @param level an unsigned int, the SBML Level to assign to this
 * Reaction_t.
 *
 * @param version an unsigned int, the SBML Version to assign to this
 * Reaction_t.
 *
 * @return a pointer to the newly created Reaction_t structure.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
Reaction_t *
Reaction_create (unsigned int level, unsigned int version);


/**
 * Creates a new Reaction_t structure using the given
 * SBMLNamespaces_t structure.
 *
 * @param sbmlns SBMLNamespaces_t, a pointer to an SBMLNamespaces_t structure
 * to assign to this Reaction_t.
 *
 * @return a pointer to the newly created Reaction_t structure.
 *
 * @copydetails doc_note_setting_lv
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
Reaction_t *
Reaction_createWithNS (SBMLNamespaces_t *sbmlns);


/**
 * Frees the given Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
void
Reaction_free (Reaction_t *r);


/**
 * @return a (deep) copy of this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
Reaction_t *
Reaction_clone (const Reaction_t *r);


/**
 * Initializes the fields of this Reaction_t to their defaults:
 *
 * The SBML Reaction component has slightly different aspects and
 * default attribute values in different SBML Levels and Versions.
 * This method sets the values to certain common defaults, based
 * mostly on what they are in SBML Level&nbsp;2.  Specifically:
 *
 * @li Sets the "reversible" attribute to @c true
 * @li Sets the "fast" attribute to @c false
 * @li Marks the "fast" attribute as @em not having been set for
 *     SBML Level&nbsp;2, but @em as having been set for
 *     SBML Level&nbsp;1 and SBML Level&nbsp;3.
 *
 * @copydetails doc_warning_reaction_cant_ignore_fast
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
void
Reaction_initDefaults (Reaction_t *r);


/**
 * Returns a list of XMLNamespaces_t associated with this Reaction_t
 * structure.
 *
 * @param r the Reaction_t structure.
 * 
 * @return pointer to the XMLNamespaces_t structure associated with 
 * this structure
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
const XMLNamespaces_t *
Reaction_getNamespaces(Reaction_t *r);


/**
 * @return the id of this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
const char *
Reaction_getId (const Reaction_t *r);


/**
 * @return the name of this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
const char *
Reaction_getName (const Reaction_t *r);


/**
 * @return the KineticLaw_t of this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
KineticLaw_t *
Reaction_getKineticLaw (Reaction_t *r);


/**
 * @return the reversible status of this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_getReversible (const Reaction_t *r);


/**
 * @return the fast status of this Reaction_t.
 *
 * @copydetails doc_fast_attribute_removed
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_getFast (const Reaction_t *r);


/**
 * @return the compartment of this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
const char *
Reaction_getCompartment (const Reaction_t *r);


/**
 * @return @c 1 (true) if the id of this Reaction_t is set, 
 * @c 0 (false) otherwise.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_isSetId (const Reaction_t *r);


/**
 * @return @c 1 (true) if the name of this Reaction_t is set, 
 * @c 0 (false) otherwise.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_isSetName (const Reaction_t *r);


/**
 * @return @c 1 (true) if the KineticLaw_t of this Reaction_t is set,
 * @c 0 (false) otherwise.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_isSetKineticLaw (const Reaction_t *r);


/**
 * @return @c 1 (true) if the fast status of this Reaction_t is set,
 * @c 0 (false) otherwise.
 *
 * In L1, fast is optional with a default of false, which means it is
 * effectively always set.  In L2, however, fast is optional with no
 * default value, so it may or may not be set to a specific value.
 *
 * @copydetails doc_fast_attribute_removed
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_isSetFast (const Reaction_t *r);


/**
 * @return @c 1 (true) if the KineticLaw_t of this Reaction_t is set,
 * @c 0 (false) otherwise.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_isSetCompartment (const Reaction_t *r);


/**
 * @return @c 1 (true) if the reversible attribute of this Reaction_t is set,
 * @c 0 (false) otherwise.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_isSetReversible (const Reaction_t *r);


/**
 * Sets the id of this Reaction_t to a copy of @p sid.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with an @p sid of NULL is equivalent to
 * unsetting the "id" attribute.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_setId (Reaction_t *r, const char *sid);


/**
 * Sets the name of this Reaction_t to a copy of name.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the name set to NULL is equivalent to
 * unsetting the "name" attribute.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_setName (Reaction_t *r, const char *name);


/**
 * Sets the KineticLaw_t of this Reaction_t to a copy of the given KineticLaw_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_setKineticLaw (Reaction_t *r, const KineticLaw_t *kl);


/**
 * Sets the reversible status of this Reaction_t to value (boolean).
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_setReversible (Reaction_t *r, int value);


/**
 * Sets the fast status of this Reaction_t to value (boolean).
 *
 * @copydetails doc_fast_attribute_removed
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_setFast (Reaction_t *r, int value);


/**
 * Sets the compartment of this Reaction_t to a copy of compartment.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_INVALID_ATTRIBUTE_VALUE, OperationReturnValues_t}
 *
 * @note Using this function with the compartment set to NULL is equivalent to
 * unsetting the "compartment" attribute.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_setCompartment (Reaction_t *r, const char *compartment);


/**
 * Unsets the name of this Reaction_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_unsetName (Reaction_t *r);


/**
 * Unsets the compartment of this Reaction_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_unsetCompartment (Reaction_t *r);


/**
 * Unsets the KineticLaw_t of this Reaction_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_unsetKineticLaw (Reaction_t *r);


/**
 * Unsets the fast status of this Reation_t.
 *
 * @copydetails doc_fast_attribute_removed
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
 *
 * In L1, fast is optional with a default of false, which means it is
 * effectively always set.  In L2, however, fast is optional with no
 * default value, so it may or may not be set to a specific value.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_unsetFast (Reaction_t *r);


/**
 * Unsets the reversible status of this Reaction_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_unsetReversible (Reaction_t *r);


/**
 * Predicate returning @c 1 (true) or @c 0 (false) depending on whether
 * all the required attributes for this Reaction_t structure
 * have been set.
 *
 * The required attributes for a Reaction_t structure are:
 * @li "id" (or "name" in SBML Level&nbsp;1)
 * @li "fast" (in Level&nbsp;3 Version&nbsp;1 only, where it is defined as a required attribute)
 * @li "reversible" (in Level&nbsp;3 only, where it is defined as a required attribute)
 *
 * @param r the Reaction_t structure to check.
 *
 * @return @c 1 (true) if all the required
 * attributes for this structure have been defined, @c 0 (false) otherwise.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_hasRequiredAttributes (Reaction_t *r);


/**
 * Adds a copy of the given reactant (SpeciesReference_t) to this Reaction_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_addReactant (Reaction_t *r, const SpeciesReference_t *sr);


/**
 * Adds a copy of the given product (SpeciesReference_t) to this Reaction_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_addProduct (Reaction_t *r, const SpeciesReference_t *sr);


/**
 * Adds a copy of the given modifier (modifier SpeciesReference_t) to this
 * Reaction_t.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_UNEXPECTED_ATTRIBUTE, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_addModifier (Reaction_t *r, const SpeciesReference_t *msr);

/**
 * Adds a copy of the given Species_t object as a reactant to this Reaction_t.
 *
 * @param r the Reaction_t structure to which the reactant is added.
 * @param s the Species_t structure to be added as reactant.
 * @param stoichiometry the stoichiometry of the added reactant.
 * @param id the string to be used as the id of the SpeciesReference_t that will
 *        be created. 
 * @param constant an attribute specifying whether the created SpeciesReference_t is
 *        constant or not.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @note the Species_t structure itself is NOT added to the model
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_addReactantBySpecies (Reaction_t *r, const Species_t *s, 
                               double stoichiometry, const char *id,
                               int constant);


/**
 * Adds a copy of the given Species_t object as a product to this Reaction_t.
 *
 * @param r the Reaction_t structure to which the product is added.
 * @param s the Species_t structure to be added as product.
 * @param stoichiometry the stoichiometry of the added product.
 * @param id the string to be used as the id of the SpeciesReference_t that will
 *        be created.
 * @param constant an attribute specifying whether the SpeciesReference_t is
 *        constant or not.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @note the Species_t structure itself is NOT added to the model
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_addProductBySpecies (Reaction_t *r, const Species_t *s, 
                               double stoichiometry, const char *id,
                               int constant);


/**
 * Adds a copy of the given Species_t object as a modifier to this Reaction_t.
 *
 * @param r the Reaction_t structure to which the modifier is added.
 * @param s the Species_t structure to be added as modifier.
 * @param id the string to be used as the id of the SpeciesReference_t that will
 *        be created.
 *
 * @copydetails doc_returns_success_code
 * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_LEVEL_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_VERSION_MISMATCH, OperationReturnValues_t}
 * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
 *
 * @note the Species_t structure itself is NOT added to the model
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
int
Reaction_addModifierBySpecies (Reaction_t *r, const Species_t *s, 
                               const char *id);


/**
 * Creates a new SpeciesReference, adds it to this Reaction_t's list of
 * reactants and returns it.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_createReactant (Reaction_t *r);


/**
 * Creates a new SpeciesReference, adds it to this Reaction_t's list of
 * products and returns it.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_createProduct (Reaction_t *r);


/**
 * Creates a new SpeciesReference, adds it to this Reaction_t's list of
 * modifiers and returns it.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_createModifier (Reaction_t *r);


/**
 * Creates a new KineticLaw_t for this Reaction_t and returns it.  If this
 * Reaction_t had a previous KineticLaw_t, it will be destroyed.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
KineticLaw_t *
Reaction_createKineticLaw (Reaction_t *r);


/**
 * @return the list of Reactants for this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
ListOf_t *
Reaction_getListOfReactants (Reaction_t *r);


/**
 * @return the list of Products for this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
ListOf_t *
Reaction_getListOfProducts (Reaction_t *r);


/**
 * @return the list of Modifiers for this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
ListOf_t *
Reaction_getListOfModifiers (Reaction_t *r);


/**
 * @return the nth reactant (SpeciesReference_t) of this Reaction_t.
 * If the index @p n is invalid, @c NULL is returned.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_getReactant (Reaction_t *r, unsigned int n);


/**
 * @return the reactant (SpeciesReference_t) in this Reaction_t with the given
 * species or @c NULL if no such reactant exists.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_getReactantBySpecies (Reaction_t *r, const char *species);


/**
 * @return the nth product (SpeciesReference_t) of this Reaction_t.
 * If the index @p n is invalid, @c NULL is returned.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_getProduct (Reaction_t *r, unsigned int n);


/**
 * @return the product (SpeciesReference_t) in this Reaction_t with the given
 * species or @c NULL if no such product exists.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_getProductBySpecies (Reaction_t *r, const char *species);


/**
 * @return the nth modifier (modifier SpeciesReference_t) of this Reaction_t.
 * If the index @p n is invalid, @c NULL is returned.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_getModifier (Reaction_t *r, unsigned int n);


/**
 * @return the modifier (modifier SpeciesReference_t) in this Reaction_t with
 * the given species or @c NULL if no such modifier exists.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_getModifierBySpecies (Reaction_t *r, const char *species);


/**
 * @return the number of reactants (SpeciesReference_t's) in this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
unsigned int
Reaction_getNumReactants (const Reaction_t *r);


/**
 * @return the number of products (SpeciesReference_t's) in this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
unsigned int
Reaction_getNumProducts (const Reaction_t *r);


/**
 * @return the number of modifiers (modifier SpeciesReference_t's) in this
 * Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
unsigned int
Reaction_getNumModifiers (const Reaction_t *r);


/**
 * Removes the nth reactant SpeciesReference_t structure from this 
 * Reaction_t structure and returns a pointer to it.
 *
 * The caller owns the returned structure and is responsible for deleting it.
 *
 * @param r the Reaction_t structure.
 * @param n the integer index of the reactant SpeciesReference_t to remove.
 *
 * @return the reactant SpeciesReference_t structure removed.  As mentioned 
 * above, the caller owns the returned structure. @c NULL is returned if the 
 * given index is out of range.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_removeReactant (Reaction_t *r, unsigned int n);


/**
 * Removes the reactant SpeciesReference_t structure with the given 
 * "species" attribute from this Reaction_t structure and returns a pointer
 * to it.
 *
 * The caller owns the returned structure and is responsible for deleting it.
 *
 * @param r the Reaction_t structure.
 * @param species the "species" attribute of the reactant SpeciesReference_t 
 * to remove.
 *
 * @return the reactant SpeciesReference_t structure removed.  As mentioned 
 * above, the caller owns the returned structure. @c NULL is returned if no 
 * reactant SpeciesReference_t structure with the "species" attribute exists 
 * in this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_removeReactantBySpecies (Reaction_t *r, const char *species);


/**
 * Removes the nth product SpeciesReference_t structure from this 
 * Reaction_t structure and returns a pointer to it.
 *
 * The caller owns the returned structure and is responsible for deleting it.
 *
 * @param r the Reaction_t structure.
 * @param n the integer index of the product SpeciesReference_t to remove.
 *
 * @return the product SpeciesReference_t structure removed.  As mentioned 
 * above, the caller owns the returned structure. @c NULL is returned if the 
 * given index is out of range.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_removeProduct (Reaction_t *r, unsigned int n);


/**
 * Removes the product SpeciesReference_t structure with the given 
 * "species" attribute from this Reaction_t structure and returns a pointer
 * to it.
 *
 * The caller owns the returned structure and is responsible for deleting it.
 *
 * @param r the Reaction_t structure.
 * @param species the "species" attribute of the product SpeciesReference_t 
 * to remove.
 *
 * @return the product SpeciesReference_t structure removed.  As mentioned 
 * above, the caller owns the returned structure. @c NULL is returned if no 
 * product SpeciesReference_t structure with the "species" attribute exists 
 * in this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_removeProductBySpecies (Reaction_t *r, const char *species);


/**
 * Removes the nth modifier SpeciesReference_t structure from this 
 * Reaction_t structure and returns a pointer to it.
 *
 * The caller owns the returned structure and is responsible for deleting it.
 *
 * @param r the Reaction_t structure.
 * @param n the integer index of the modifier SpeciesReference_t to remove.
 *
 * @return the modifier SpeciesReference_t structure removed.  As mentioned 
 * above, the caller owns the returned structure. @c NULL is returned if the 
 * given index is out of range.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_removeModifier (Reaction_t *r, unsigned int n);


/**
 * Removes the modifier SpeciesReference_t structure with the given 
 * "species" attribute from this Reaction_t structure and returns a pointer
 * to it.
 *
 * The caller owns the returned structure and is responsible for deleting it.
 *
 * @param r the Reaction_t structure.
 * @param species the "species" attribute of the modifier SpeciesReference_t 
 * to remove.
 *
 * @return the modifier SpeciesReference_t structure removed.  As mentioned 
 * above, the caller owns the returned structure. @c NULL is returned if no 
 * modifier SpeciesReference_t structure with the "species" attribute exists 
 * in this Reaction_t.
 *
 * @memberof Reaction_t
 */
LIBSBML_EXTERN
SpeciesReference_t *
Reaction_removeModifierBySpecies (Reaction_t *r, const char *species);


/**
 * Returns the Reaction_t structure having a given identifier.
 *
 * @param lo the ListOfReactions_t structure to search.
 * @param sid the "id" attribute value being sought.
 *
 * @return item in the @p lo ListOfReactions with the given @p sid or a
 * null pointer if no such item exists.
 *
 * @see ListOf_t
 *
 * @memberof ListOfReactions_t
 */
LIBSBML_EXTERN
Reaction_t *
ListOfReactions_getById (ListOf_t *lo, const char *sid);


/**
 * Removes a Reaction_t structure based on its identifier.
 *
 * The caller owns the returned item and is responsible for deleting it.
 *
 * @param lo the list of Reaction_t structures to search.
 * @param sid the "id" attribute value of the structure to remove.
 *
 * @return The Reaction_t structure removed, or a null pointer if no such
 * item exists in @p lo.
 *
 * @see ListOf_t
 *
 * @memberof ListOfReactions_t
 */
LIBSBML_EXTERN
Reaction_t *
ListOfReactions_removeById (ListOf_t *lo, const char *sid);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* Reaction_h */


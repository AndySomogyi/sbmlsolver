/**
 * @file    SpeciesReferenceGlyph.h
 * @brief   Definition of SpeciesReferenceGlyph for SBML Layout.
 * @author  Ralph Gauges
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
 * Copyright (C) 2004-2008 by European Media Laboratories Research gGmbH,
 *     Heidelberg, Germany
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class SpeciesReferenceGlyph
 * @sbmlbrief{layout} A glyph for an SBML <em>species reference</em>.
 *
 * The %SpeciesReferenceGlyph element describes the graphical connection
 * between a SpeciesGlyph and a ReactionGlyph (which would be an arrow or
 * some curve in most cases). A SpeciesReferenceGlyph inherits from
 * GraphicalObject, and adds a mandatory attribute 'speciesGlyph' and two
 * optional attributes 'speciesReference' and 'role'. Optionally, the
 * SpeciesReferenceGlyph also has a child element 'curve'.
 *
 * If the curve is specified, it overrides the inherited bounding box.
 */

#ifndef SpeciesReferenceGlyph_H__
#define SpeciesReferenceGlyph_H__

#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/layout/common/layoutfwd.h>

#ifdef __cplusplus


#include <string>

#include <sbml/packages/layout/sbml/GraphicalObject.h>
#include <sbml/packages/layout/sbml/SpeciesReferenceRole.h>
#include <sbml/packages/layout/sbml/Curve.h>
#include <sbml/packages/layout/extension/LayoutExtension.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN SpeciesReferenceGlyph : public GraphicalObject
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::string mSpeciesReference;
  std::string mSpeciesGlyph;
  SpeciesReferenceRole_t mRole;
  Curve mCurve;
  bool mCurveExplicitlySet;
  /** @endcond */

public:

  /**
   * Creates a new SpeciesReferenceGlyph with the given SBML level, version and
   * package version.  The id if the associated species
   * reference and the id of the associated species glyph are set to the
   * empty string.  The role is set to @sbmlconstant{SPECIES_ROLE_INVALID, SpeciesReferenceRole_t}.
   *
   * @param level the SBML Level.
   * @param version the Version within the SBML Level.
   * @param pkgVersion the version of the package.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  SpeciesReferenceGlyph (unsigned int level      = LayoutExtension::getDefaultLevel(),
                         unsigned int version    = LayoutExtension::getDefaultVersion(),
                         unsigned int pkgVersion = LayoutExtension::getDefaultPackageVersion());

  
  /**
   * Constructor.
   *
   * @copydetails doc_what_are_sbml_package_namespaces
   *
   * @param layoutns the LayoutPkgNamespaces object.
   *
   * @copydetails doc_note_setting_lv_pkg
   */
  SpeciesReferenceGlyph(LayoutPkgNamespaces* layoutns);
        

  /**
   * Creates a new SpeciesReferenceGlyph.  The id is given as the first
   * argument, the id of the associated species glyph is given as the
   * second argument.  The third argument is the id of the associated
   * species reference and the fourth argument is the role.
   */
  SpeciesReferenceGlyph (LayoutPkgNamespaces* layoutns, const std::string& sid,
                          const std::string& speciesGlyphId,
                          const std::string& speciesReferenceId,
                          SpeciesReferenceRole_t role );
        

  /**
   * Creates a new SpeciesReferenceGlyph from the given XMLNode
   */
  SpeciesReferenceGlyph(const XMLNode& node, unsigned int l2version=4);

  /**
   * Copy constructor.
   *
   * @param source the instance to copy.
   */
   SpeciesReferenceGlyph(const SpeciesReferenceGlyph& source);

  /**
   * Assignment operator.
   *
   * @param source the object whose values are used as the basis of the
   * assignment.
   */
   virtual SpeciesReferenceGlyph& operator=(const SpeciesReferenceGlyph& source);

  /**
   * Destructor.
   */
  virtual ~SpeciesReferenceGlyph (); 

        
  /**
   * Returns the id of the associated SpeciesGlyph.
   */
  const std::string& getSpeciesGlyphId () const;
        
  /**
   * Sets the id of the associated species glyph.
   */
  void setSpeciesGlyphId (const std::string& speciesGlyphId);
        
  /**
   * Returns the id of the associated species reference.
   */
  const std::string& getSpeciesReferenceId() const;
        
  /**
   * Sets the id of the associated species reference.
   */
  void setSpeciesReferenceId (const std::string& id);

  /**
   * Returns a string representation of the role.
   */
  const std::string& getRoleString() const;

        
  /**
   * Returns the role.
   */
  SpeciesReferenceRole_t getRole() const;
        
  /**
   * Sets the role based on a string.
   * The String can be one of:
   * SUBSTRATE
   * PRODUCT
   * SIDESUBSTRATE
   * SIDEPRODUCT
   * MODIFIER
   * ACTIVATOR
   * INHIBITOR    
   */
  void setRole (const std::string& role);

  /**
   * Sets the role.
   */
  void setRole (SpeciesReferenceRole_t role);
     
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
   * Returns the curve object for the species reference glyph
   */
  Curve* getCurve () ;

  /**
   * Returns the curve object for the species reference glyph
   */
  const Curve* getCurve () const;

  /**
   * Sets the curve object for the species reference glyph.
   */
  void setCurve (const Curve* curve);
       
  /**
   * Returns @c true if the curve consists of one or more segments.
   */
  
    bool isSetCurve () const;


  bool getCurveExplicitlySet() const;
  /**
   * Returns @c true if the id of the associated species glyph is not the
   * empty string.
   */
  bool isSetSpeciesGlyphId () const;
        
  /**
   * Returns @c true if the id of the associated species reference is not the
   * empty string.
   */
  bool isSetSpeciesReferenceId() const;
        
  /**
   * Returns @c true of role is different from @sbmlconstant{SPECIES_ROLE_INVALID, SpeciesReferenceRole_t}.
   */
  bool isSetRole () const;
        
  /**
   * Calls initDefaults on GraphicalObject and sets role to
   * @sbmlconstant{SPECIES_ROLE_INVALID, SpeciesReferenceRole_t}.
   */
  void initDefaults ();

  /**
   * Creates a new LineSegment object, adds it to the end of the list of
   * curve segment objects of the curve and returns a reference to the
   * newly created object.
   */
  LineSegment* createLineSegment ();

  /**
   * Creates a new CubicBezier object, adds it to the end of the list of
   * curve segment objects of the curve and returns a reference to the
   * newly created object.
   */
  CubicBezier* createCubicBezier ();

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parent's
   * implementation of this method as well.  For example:
   *
   *   SBase::writeElements(stream);
   *   mReactants.write(stream);
   *   mProducts.write(stream);
   *   ...
   */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */


  /**
   * Returns the XML element name of
   * this SBML object.
   *
   * @return the string of the name of this element.
   */
  virtual const std::string& getElementName () const ;


  /**
   * Creates and returns a deep copy of this SpeciesReferenceGlyph.
   * 
   * @return a (deep) copy of this SpeciesReferenceGlyph.
   */
  virtual SpeciesReferenceGlyph* clone () const;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @sbmlconstant{SBML_LAYOUT_SPECIESREFERENCEGLYPH, SBMLLayoutTypeCode_t}.
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


  /** @cond doxygenLibsbmlInternal */
  /**
   * Accepts the given SBMLVisitor.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether or not the Visitor would like to visit the SBML object's next
   * sibling object (if available).
   */
  virtual bool accept (SBMLVisitor& v) const;
  /** @endcond */
   

   /**
    * Creates an XMLNode object from this.
    */
    virtual XMLNode toXML() const;


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets the parent SBMLDocument of this SBML object.
   *
   * @param d the SBMLDocument object to use.
   */
  virtual void setSBMLDocument (SBMLDocument* d);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Sets this SBML object to child SBML objects (if any).
   * (Creates a child-parent relationship by the parent)
   *
   * Subclasses must override this function if they define
   * one ore more child elements.
   * Basically, this function needs to be called in
   * constructor, copy constructor, assignment operator.
   *
   * @see setSBMLDocument
   * @see enablePackageInternal
   */
  virtual void connectToChild ();
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Enables/Disables the given package with this element and child
   * elements (if any).
   * (This is an internal implementation for enablePakcage function)
   *
   * @note Subclasses in which one or more child elements are defined
   * must override this function.
   */
  virtual void enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag);
  /** @endcond */
    
protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or @c NULL if the token was not recognized.
   */
  virtual SBase*
  createObject (XMLInputStream& stream);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to get the list of
   * expected attributes.
   * This function is invoked from corresponding readAttributes()
   * function.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parent's implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes, 
                               const ExpectedAttributes& expectedAttributes);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parent's implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "id"  , mId   );
   *   stream.writeAttribute( "name", mName );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */

};

LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * Creates a new SpeciesReferenceGlyph_t structure and returns a pointer to it.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
SpeciesReferenceGlyph_t *
SpeciesReferenceGlyph_create (void);

/**
 * Creates a new SpeciesReferenceGlyph_t from a template.
 *
 * @param temp the SpeciesReferenceGlyph_t structure to copy.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
SpeciesReferenceGlyph_t *
SpeciesReferenceGlyph_createFrom (const SpeciesReferenceGlyph_t *temp);

/**
 * Creates a new SpeciesReferenceGlyph_t structure.  The id is given as the first
 * argument, the id of the associated species glyph is given as the
 * second argument.  The third argument is the id of the associated species
 * reference and the fourth argument is the role.
 *
 * @param sid the string to use as the ID of the SpeciesReferenceGlyph_t.
 * @param speciesGlyphId the string to use as the species glyph.
 * @param speciesReferenceId the string to use as the species reference.
 * @param role the role of the created SpeciesReferenceGlyph_t.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
SpeciesReferenceGlyph_t *
SpeciesReferenceGlyph_createWith ( const char *sid,
                                   const char *speciesGlyphId,
                                   const char *speciesReferenceId,
                                   SpeciesReferenceRole_t role );


/**
 * Frees the memory for the SpeciesReferenceGlyph_t
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
void
SpeciesReferenceGlyph_free (SpeciesReferenceGlyph_t *srg);


/**
 * Sets the reference species for the species glyph.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 * @param id the string to use as the species reference.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
void
SpeciesReferenceGlyph_setSpeciesReferenceId (SpeciesReferenceGlyph_t *srg,
                                             const char *id);

/**
 * Returns the reference species id for the given species glyph.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
const char *
SpeciesReferenceGlyph_getSpeciesReferenceId(const SpeciesReferenceGlyph_t *srg);

/**
 * Returns @c 0 (false) if the reference species reference has not been set for this
 * glyph and @c 1 (true) otherwise.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
int
SpeciesReferenceGlyph_isSetSpeciesReferenceId(const SpeciesReferenceGlyph_t *srg);

/**
 * Sets the species glyph reference for the species glyph.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 * @param id the string to use as the species glyph.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
void
SpeciesReferenceGlyph_setSpeciesGlyphId (SpeciesReferenceGlyph_t *srg,
                                         const char *id);

/**
 * Returns the reference species id for the given species glyph.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
const char *
SpeciesReferenceGlyph_getSpeciesGlyphId (const SpeciesReferenceGlyph_t *srg);

/**
 * Returns @c 0 (false) if the reference species reference has not been set for this
 * glyph and @c 1 (true) otherwise.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
int
SpeciesReferenceGlyph_isSetSpeciesGlyphId (const SpeciesReferenceGlyph_t *srg);


/**
 * Sets the curve for the species reference glyph.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 * @param c the Curve_t to set for the SpeciesReferenceGlyph_t.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
void
SpeciesReferenceGlyph_setCurve (SpeciesReferenceGlyph_t *srg, Curve_t *c);

/**
 * Returns the Curve_t for the given species reference glyph.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
Curve_t *
SpeciesReferenceGlyph_getCurve (SpeciesReferenceGlyph_t *srg);

/**
 * Returns @c 1 (true) if the Curve_t has one or more LineSegment_t's.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
int
SpeciesReferenceGlyph_isSetCurve(SpeciesReferenceGlyph_t* srg);

/**
 * Sets the role of the species reference glyph based on the string.  The
 * string can be one of:
 * @li "substrate" (@sbmlconstant{SPECIES_ROLE_SUBSTRATE, SpeciesReferenceRole_t})
 * @li "product" (@sbmlconstant{SPECIES_ROLE_PRODUCT, SpeciesReferenceRole_t})
 * @li "sidesubstrate" (@sbmlconstant{SPECIES_ROLE_SIDESUBSTRATE, SpeciesReferenceRole_t})
 * @li "sideproduct" (@sbmlconstant{SPECIES_ROLE_SIDEPRODUCT, SpeciesReferenceRole_t})
 * @li "modifier" (@sbmlconstant{SPECIES_ROLE_MODIFIER, SpeciesReferenceRole_t})
 * @li "activator" (@sbmlconstant{SPECIES_ROLE_ACTIVATOR, SpeciesReferenceRole_t})
 * @li "inhibitor" (@sbmlconstant{SPECIES_ROLE_INHIBITOR, SpeciesReferenceRole_t})
 * @li "undefined" (@sbmlconstant{SPECIES_ROLE_UNDEFINED, SpeciesReferenceRole_t})
 *
 * If it is none of those, the role is set to 
 * @sbmlconstant{SPECIES_ROLE_INVALID, SpeciesReferenceRole_t}.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 * @param r the role to use for the SpeciesReferenceGlyph_t.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
void
SpeciesReferenceGlyph_setRole (SpeciesReferenceGlyph_t *srg, const char *r);

/**
 * Returns the role of the species reference.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
SpeciesReferenceRole_t
SpeciesReferenceGlyph_getRole (const SpeciesReferenceGlyph_t *srg);

/**
 * Returns a string representation of the role of the species reference.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
const char*
SpeciesReferenceGlyph_getRoleString(const SpeciesReferenceGlyph_t* srg);


/**
 * Returns @c 1 (true) if the role is not
 * @sbmlconstant{SPECIES_ROLE_INVALID, SpeciesReferenceRole_t}.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
int
SpeciesReferenceGlyph_isSetRole(const SpeciesReferenceGlyph_t *srg);

/**
 * Calls initDefaults on GraphicalObject and sets role to
 * @sbmlconstant{SPECIES_ROLE_INVALID, SpeciesReferenceRole_t}.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
void
SpeciesReferenceGlyph_initDefaults (SpeciesReferenceGlyph_t *srg);

/**
 * Creates a new LineSegment_t structure, adds it to the end of the list of
 * curve segment of the curve and returns a reference to the newly
 * created structure.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
LineSegment_t *
SpeciesReferenceGlyph_createLineSegment (SpeciesReferenceGlyph_t *srg);

/**
 * Creates a new CubicBezier_t structure, adds it to the end of the list of
 * curve segments of the curve and returns a reference to the newly
 * created structure.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
CubicBezier_t *
SpeciesReferenceGlyph_createCubicBezier (SpeciesReferenceGlyph_t *srg);

/**
 * @return a (deep) copy of this SpeciesReferenceGlyph_t.
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
SpeciesReferenceGlyph_t *
SpeciesReferenceGlyph_clone (const SpeciesReferenceGlyph_t *srg);


/**
 * Returns @c 1 (true) if the id is set
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
int
SpeciesReferenceGlyph_isSetId (const SpeciesReferenceGlyph_t *srg);


/**
 * Returns the id
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
const char *
SpeciesReferenceGlyph_getId (const SpeciesReferenceGlyph_t *srg);


/**
 * Sets the id
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 * @param sid the string to use as the ID of the SpeciesReferenceGlyph_t.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
int
SpeciesReferenceGlyph_setId (SpeciesReferenceGlyph_t *srg, const char *sid);


/**
 * Unsets the id
 *
 * @param srg the SpeciesReferenceGlyph_t structure.
 *
 * @memberof SpeciesReferenceGlyph_t
 */
LIBSBML_EXTERN
void
SpeciesReferenceGlyph_unsetId (SpeciesReferenceGlyph_t *srg);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END


#endif /* !SWIG */
#endif /* SpeciesReferenceGlyph_H__ */

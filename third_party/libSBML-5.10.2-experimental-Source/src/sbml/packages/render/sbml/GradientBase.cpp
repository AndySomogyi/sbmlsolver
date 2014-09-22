/**
 * @file    GradientBase.cpp
 * @brief   abstract base class for gradient objects
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

#include "GradientBase.h"
#include <sbml/packages/layout/util/LayoutAnnotation.h>
#include <sbml/packages/layout/util/LayoutUtilities.h>
#include <sbml/packages/render/extension/RenderExtension.h>
#include <sbml/SBMLErrorLog.h>
#include <sbml/SBMLVisitor.h>
#include <sbml/xml/XMLNode.h>
#include <sbml/xml/XMLToken.h>
#include <sbml/xml/XMLAttributes.h>
#include <sbml/xml/XMLInputStream.h>
#include <sbml/xml/XMLOutputStream.h>
#include <sbml/packages/render/sbml/LinearGradient.h>
#include <sbml/packages/render/sbml/RadialGradient.h>

#include <algorithm>
#include <assert.h>
#ifndef OMIT_DEPRECATED
#ifdef DEPRECATION_WARNINGS
#include <iostream>
#endif // DEPRECATION_WARNINGS
#endif // OMIT_DEPRECATED

#include <sbml/util/ElementFilter.h>

LIBSBML_CPP_NAMESPACE_BEGIN

const std::string ListOfGradientDefinitions::ELEMENT_NAME="listOfGradientDefinitions";

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new GradientBase object with the given SBML level
 * and SBML version.
 *
 * @param level SBML level of the new object
 * @param level SBML version of the new object
 */
GradientBase::GradientBase (unsigned int level, unsigned int version, unsigned int pkgVersion) : 
    SBase(level,version)
    ,mId("")
    ,mSpreadMethod(GradientBase::PAD)
    ,mGradientStops(level, version, pkgVersion)
{
  setSBMLNamespacesAndOwn(new RenderPkgNamespaces(level,version,pkgVersion));  
    connectToChild();
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new GradientBase object with the given SBMLNamespaces.
 *
 * @param sbmlns The SBML namespace for the object.
 */
GradientBase::GradientBase (RenderPkgNamespaces* renderns):
    SBase(renderns)
    ,mId("")
    ,mSpreadMethod(GradientBase::PAD)
    ,mGradientStops(renderns)
{
      // set the element namespace of this object
  setElementNamespace(renderns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(renderns);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new GradientBase object from the given XMLNode object.
 * The XMLNode object has to contain a valid XML representation of a 
 * GradientBase object as defined in the render extension specification.
 * This method is normally called when render information is read from a file and 
 * should normally not have to be called explicitely.
 *
 * @param node the XMLNode object reference that describes the GradientBase
 * object to be instantiated.
 */
GradientBase::GradientBase(const XMLNode& node, unsigned int l2version) 
  : SBase(2, l2version)
  , mGradientStops(node, l2version)
{
    const XMLAttributes& attributes=node.getAttributes();
    const XMLNode* child;
    mURI = RenderExtension::getXmlnsL3V1V1();        

    ExpectedAttributes ea;
    addExpectedAttributes(ea);

    this->readAttributes(attributes, ea);
    unsigned int n=0,nMax = node.getNumChildren();
    while(n<nMax)
    {
        child=&node.getChild(n);
        const std::string& childName=child->getName();
        if(childName=="stop")
        {
            this->mGradientStops.appendAndOwn(new GradientStop(*child));
        }
        else if(childName=="annotation")
        {
            this->mAnnotation=new XMLNode(*child);
        }
        else if(childName=="notes")
        {
            this->mNotes=new XMLNode(*child);
        }
        ++n;
    }

    
  setSBMLNamespacesAndOwn(new RenderPkgNamespaces(2,l2version));  

  connectToChild();
}
/** @endcond */


/*
 * Destroy this object.
 */
GradientBase::~GradientBase ()
{
}





#ifndef OMIT_DEPRECATED
/** @cond doxygenLibsbmlInternal */
/*
 * Constructor which creates a GradientBase with no gradient stops.
 * The spreadMethod attribute is set to GradientBase::PAD and the id is
 * set to the given value.
 * This object is not valid until it gets at least two gradient stops.
 *
 * @param id The id for the gradient definition object
 *
 * This constructor is deprecated. The new libsbml API only has
 * constructors which take the SBML level and version or one that takes
 * an SBMLNamespaces object.
 */
GradientBase::GradientBase(RenderPkgNamespaces* renderns, const std::string& id)
  : SBase(renderns)
  , mId(id)
  , mSpreadMethod(GradientBase::PAD)
  , mGradientStops(renderns)
{
#ifdef DEPRECATION_WARNINGS
    std::cerr << "Warning. GradientBase::GradientBase(const std::string& id) is deprecated." << std::endl;
#endif // DEPRECATION_WARNINGS
        // set the element namespace of this object
  setElementNamespace(renderns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(renderns);
}
/** @endcond */
#endif // OMIT_DEPRECATED

List*
GradientBase::getAllElements(ElementFilter* filter)
{
  List* ret = new List();
  List* sublist = NULL;

  ADD_FILTERED_LIST(ret, sublist, mGradientStops, filter);

  ADD_FILTERED_FROM_PLUGIN(ret, sublist, filter);

  return ret;
}


/** @cond doxygenLibsbmlInternal */
/*
 * Returns the spreadmethod of the gradient.
 * Valid values are GradientBase::PAD, GradientBase::REFLECT
 * and GradientBase::REPEAT.
 *
 * @return the spread method for the gradient object.
 */
GradientBase::SPREADMETHOD GradientBase::getSpreadMethod() const
{
    return this->mSpreadMethod;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the spread method.
 * Valid values are GradientBase::PAD, GradientBase::REFLECT
 * and GradientBase::REPEAT.
 *
 * @param method The new spread method for the gradient.
 */
void GradientBase::setSpreadMethod(GradientBase::SPREADMETHOD method)
{
    this->mSpreadMethod=method;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the number of gradient stops.
 * A valid gradient needs at least two gradient stops
 *
 * @return the number of gradient stops in the gradient.
 */
unsigned int GradientBase::getNumGradientStops() const
{
    return this->mGradientStops.size();
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the gradient stop vector.
 *
 * @return a pointer to the ListOfGradientStops object
 * for the gradient.
 */
ListOfGradientStops* GradientBase::getListOfGradientStops()
{
    return &(this->mGradientStops);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the gradient stop vector.
 *
 * @return a const pointer to the ListOfGradientStops object
 * for the gradient.
 */
const ListOfGradientStops* GradientBase::getListOfGradientStops() const
{
    return &(this->mGradientStops);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the gradient stop with the given index or NULL
 * if the index is invalid.
 *
 * @param i index of the gradient stop to be returned. The index has to be between 0 and
 * getNumGradientStops() - 1.
 *
 * @return a pointer to the gradient stop with the given index
 * or NULL if the index was out of bounds.
 */
GradientStop* GradientBase::getGradientStop(unsigned int i)
{
    return (i<=this->mGradientStops.size())?static_cast<GradientStop*>(this->mGradientStops.get(i)):NULL;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the gradient stop with the given index or NULL
 * if the index is invalid.
 *
 * @param i index of the gradient stop to be returned. The index has to be between 0 and
 * getNumGradientStops() - 1.
 *
 * @return a const pointer to the gradient stop with the given index
 * or NULL if the index was out of bounds.
 */
const GradientStop* GradientBase::getGradientStop(unsigned int i) const
{
    return (i<=this->mGradientStops.size())?static_cast<const GradientStop*>(this->mGradientStops.get(i)):NULL;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new GradientStop. The new GradientStop object is automatically
 * added to the gradient and the gradient own the object-
 *
 * @return a pointer to the newly created GradientStop.
 */
GradientStop* GradientBase::createGradientStop()
{
    GradientStop* pGradientStop=NULL;
    try
    {
      RENDER_CREATE_NS(renderns, this->getSBMLNamespaces());
      pGradientStop = new GradientStop(renderns);
	 delete renderns;
    }
    catch (...)
    {
        /* here we do not create a default object as the level/version must
         * match the parent object
         *
         * so do nothing
         */
    }


    if(pGradientStop != NULL)
    {
        this->mGradientStops.appendAndOwn(pGradientStop);
    }
    return pGradientStop;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Adds a copy of the given GradientStop object to the end
 * of the list of gradient stops.
 *
 * @param pStop a const pointer to the new gradient stop 
 *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif The possible values
 * returned by this function are:
 * @li LIBSBML_OPERATION_SUCCESS
 * @li LIBSBML_LEVEL_MISMATCH
 * @li LIBSBML_VERSION_MISMATCH
 * @li LIBSBML_OPERATION_FAILED
 *
 * @note This method should be used with some caution.  The fact that
 * this method @em copies the object passed to it means that the caller
 * will be left holding a physically different object instance than the
 * one contained in this GradientBase.  Changes made to the original object
 * instance (such as resetting attribute values) will <em>not affect the
 * instance in the GradientBase</em>.  In addition, the caller should make
 * sure to free the original object if it is no longer being used, or
 * else a memory leak will result.  Please see GradientBase::createGradientStop()
 * for a method that does not lead to these issues.
 *
 * @see createGradientStop()
 */
int GradientBase::addGradientStop(const GradientStop* pStop)
{
    if (pStop == NULL)
    {
        return LIBSBML_OPERATION_FAILED;
    }
    else if (!(pStop->hasRequiredAttributes()) || !(pStop->hasRequiredElements()))
    {
        return LIBSBML_INVALID_OBJECT;
    }
    else if (getLevel() != pStop->getLevel())
    {
        return LIBSBML_LEVEL_MISMATCH;
    }
    else if (getVersion() != pStop->getVersion())
    {
        return LIBSBML_VERSION_MISMATCH;
    }
    else
    {

        this->mGradientStops.append(pStop);

        return LIBSBML_OPERATION_SUCCESS;
    }
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Creates and returns a deep copy of the ListOfGradientDefinitions object.
 *
 * @return a (deep) copy of this ListOfGradientDefinitions
 */
ListOfGradientDefinitions* ListOfGradientDefinitions::clone () const
{
    return new ListOfGradientDefinitions(*this);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Copy constructor. Creates a copy of this ListOfGradientDefinitions object.
 */
ListOfGradientDefinitions::ListOfGradientDefinitions(const ListOfGradientDefinitions& source):ListOf(source)
{
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Assignment operator for ListOfGradientDefinitions objects..
 */
ListOfGradientDefinitions& ListOfGradientDefinitions::operator=(const ListOfGradientDefinitions& source)
{
    if(&source!=this)
    {
        this->ListOf::operator=(source);
    }
    return *this;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the libSBML type code for the objects contained in this ListOf
 * (i.e., GradientDefinition objects, if the list is non-empty).
 * 
 * @if clike LibSBML attaches an identifying code to every
 * kind of SBML object.  These are known as <em>SBML type codes</em>.
 * The set of possible type codes is defined in the enumeration
 * #SBMLTypeCode_t.  The names of the type codes all begin with the
 * characters @c SBML_. @endif@if java LibSBML attaches an
 * identifying code to every kind of SBML object.  These are known as
 * <em>SBML type codes</em>.  In other languages, the set of type codes
 * is stored in an enumeration; in the Java language interface for
 * libSBML, the type codes are defined as static integer constants in
 * interface class {@link libsbmlConstants}.  The names of the type codes
 * all begin with the characters @c SBML_. @endif
 * 
 * @return the SBML type code for the objects contained in this ListOf
 * instance, or @c SBML_UNKNOWN (default).
 *
 * @see getElementName()
 */
int ListOfGradientDefinitions::getItemTypeCode () const
{
    return SBML_RENDER_GRADIENTDEFINITION;
}
/** @endcond */


bool ListOfGradientDefinitions::isValidTypeForList(SBase * item)
{
  if (item == NULL) return false;
  int typeCode = item->getTypeCode();
  return (
    typeCode == SBML_RENDER_LINEARGRADIENT ||
    typeCode == SBML_RENDER_RADIALGRADIENT
    );
}

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the XML element name of this object, which for
 * ListOfGradientDefinitions, is always @c "listOfGradientDefinitions".
 * 
 * @return the name of this element, i.e., @c "listOfGradientDefinitions".
 */
const std::string& ListOfGradientDefinitions::getElementName () const
{
  static std::string name = ListOfGradientDefinitions::ELEMENT_NAME;
  return name;
}
/** @endcond */


/*
 * Ctor.
 */
ListOfGradientDefinitions::ListOfGradientDefinitions(RenderPkgNamespaces* renderns)
 : ListOf(renderns)
{
  //
  // set the element namespace of this object
  //
  setElementNamespace(renderns->getURI());
}


/*
 * Ctor.
 */
ListOfGradientDefinitions::ListOfGradientDefinitions(unsigned int level, unsigned int version, unsigned int pkgVersion)
 : ListOf(level,version)
{
  setSBMLNamespacesAndOwn(new RenderPkgNamespaces(level,version,pkgVersion));
};
/** @cond doxygenLibsbmlInternal */
/*
 * Creates an XMLNode object from this ListOfGradientDefinitions object.
 *
 * @return the XMLNode with the XML representation for the 
 * ListOfGradientDefinitions object.
 */
XMLNode ListOfGradientDefinitions::toXML() const
{
  return getXmlNodeForSBase(this);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase* GradientBase::createObject (XMLInputStream& stream)
{
    SBase*        object = NULL;
    
    object = this->mGradientStops.createObject(stream);
    
    return object;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase* ListOfGradientDefinitions::createObject (XMLInputStream& stream)
{
    const std::string& name   = stream.peek().getName();
    SBase*        object = NULL;

    RENDER_CREATE_NS(renderns, this->getSBMLNamespaces());

    if (name == "linearGradient")
    {
        object = new LinearGradient(renderns);
    }
    else if(name == "radialGradient")
    {
        object = new RadialGradient(renderns);
    }
    if(object != NULL) mItems.push_back(object);
    delete renderns;
    return object;
}
/** @endcond */



/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to write out their contained
 * SBML objects as XML elements.  Be sure to call your parents
 * implementation of this method as well.  For example:
 *
 *   SBase::writeElements(stream);
 *   mReactants.write(stream);
 *   mProducts.write(stream);
 *   ...
 */
void GradientBase::writeElements (XMLOutputStream& stream) const
{
    SBase::writeElements(stream);
    unsigned int i,iMax=this->mGradientStops.size();
    for(i=0;i<iMax;++i)
    {
      this->getGradientStop(i)->write(stream);
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void
GradientBase::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("spreadMethod");
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void GradientBase::readAttributes (const XMLAttributes& attributes, const ExpectedAttributes& expectedAttributes)
{

    SBase::readAttributes(attributes,expectedAttributes);
    attributes.readInto("id", mId, getErrorLog(), false, getLine(), getColumn());
    std::string s;
    if(attributes.readInto("spreadMethod",s, getErrorLog(), false, getLine(), getColumn()))
    {
      this->mSpreadMethod=GradientBase::getSpreadMethodForString(s);
    }
    else
    {
        this->mSpreadMethod=GradientBase::PAD;
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the XML element name of this object.
 *
 * This is overridden by subclasses to return a string appropriate to the
 * SBML component.  For example, Model defines it as returning "model",
 * CompartmentType defines it as returning "compartmentType", etc.
 */
const std::string& GradientBase::getElementName() const
{
  static std::string name = "gradientBase";
  return name;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Converts the given string into a spread method.
 * If the string does not represnt a valid spread method, PAD is
 * returned.
 */
GradientBase::SPREADMETHOD GradientBase::getSpreadMethodForString(const std::string& s)
{
    GradientBase::SPREADMETHOD m=PAD;
    if(s=="reflect")
    {
        m=REFLECT;
    }
    else if(s=="repeat")
    {
        m=REPEAT;
    }
    return m;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to write their XML attributes
 * to the XMLOutputStream.  Be sure to call your parents implementation
 * of this method as well.  For example:
 *
 *   SBase::writeAttributes(stream);
 *   stream.writeAttribute( "id"  , mId   );
 *   stream.writeAttribute( "name", mName );
 *   ...
 */
void GradientBase::writeAttributes (XMLOutputStream& stream) const
{
    SBase::writeAttributes(stream);
    stream.writeAttribute("id", getPrefix(), this->mId);
    switch(this->mSpreadMethod)
    {
        case PAD:
            break;
        case REFLECT:
            stream.writeAttribute("spreadMethod", getPrefix(), std::string("reflect"));
            break;
        case REPEAT:
            stream.writeAttribute("spreadMethod", getPrefix(), std::string("repeat"));
            break;
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the attribute string for the spread method set on the
 * gradient.
 */
std::string GradientBase::getSpreadMethodString() const
{
    std::string s;
    switch(this->mSpreadMethod)
    {
        case PAD:
            s="pad";
            break;
        case REFLECT:
            s="reflect";
            break;
        case REPEAT:
            s="repeat";
            break;
    }
    return s;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Accepts the given SBMLVisitor.
 *
 * @return the result of calling <code>v.visit()</code>, which indicates
 * whether or not the Visitor would like to visit the SBML object's next
 * sibling object (if available).
 */
bool GradientBase::accept (SBMLVisitor& v) const
{
    return false;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new ListOfGradientDefinitions object from the given XMLNode object.
 * The XMLNode object has to contain a valid XML representation of a 
 * ListOfGradientDefinitions object as defined in the render extension specification.
 * This method is normally called when render information is read from a file and 
 * should normally not have to be called explicitely.
 *
 * @param node the XMLNode object reference that describes the ListOfGradientDefinitions
 * object to be instantiated.
 */
ListOfGradientDefinitions::ListOfGradientDefinitions(const XMLNode& node, unsigned int l2version)
  : ListOf(2, l2version)
{
    const XMLAttributes& attributes=node.getAttributes();
    const XMLNode* child;
    ExpectedAttributes ea;
    mURI = RenderExtension::getXmlnsL3V1V1();    
    addExpectedAttributes(ea);

    this->readAttributes(attributes, ea);
    unsigned int n=0,nMax = node.getNumChildren();
    while(n<nMax)
    {
        child=&node.getChild(n);
        const std::string& childName=child->getName();
        if(childName=="linearGradient")
        {
            LinearGradient* gradient=new LinearGradient(*child, l2version);
            this->appendAndOwn(gradient);
        }
        else if(childName=="radialGradient")
        {
            RadialGradient* gradient=new RadialGradient(*child, l2version);
            this->appendAndOwn(gradient);
        }
        else if(childName=="annotation")
        {
            this->mAnnotation=new XMLNode(*child);
        }
        else if(childName=="notes")
        {
            this->mNotes=new XMLNode(*child);
        }
        ++n;
    }

  setSBMLNamespacesAndOwn(new RenderPkgNamespaces(2,l2version));  

  connectToChild();
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * This method is used when writing out gradietns to XML.
 * I writes out the attributes and children that re common to linear and radial gradient.
 */
void GradientBase::addGradientAttributesAndChildren(const GradientBase& gradient,XMLAttributes& att,XMLNode& node)
{
    addSBaseAttributes(gradient,att);
    att.add("id",gradient.mId);
    switch(gradient.mSpreadMethod)
    {
        case PAD:
            break;
        case REFLECT:
            att.add("spreadMethod","reflect");
            break;
        case REPEAT:
            att.add("spreadMethod","repeat");
            break;
    }
    // add the notes and annotations
    if(gradient.mNotes)
    {
        node.addChild(*gradient.mNotes);
    }
    if(gradient.mAnnotation)
    {
        node.addChild(*gradient.mAnnotation);
    }
    unsigned int i,iMax=gradient.mGradientStops.size();
    const GradientStop* stop=NULL;
    for(i=0;i<iMax;++i)
    {
        stop=dynamic_cast<const GradientStop*>(gradient.mGradientStops.get(i));  
        node.addChild(stop->toXML());
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the value of the "id" attribute of this GradientBase object.
 *
 * @return the id of the gradient
 */
const std::string& GradientBase::getId () const
{
    return mId;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Predicate returning @c true or @c false depending on whether this
 * GradientBase's "id" attribute has been set.
 *
 * @return returns true or false depending on whether the id on the 
 * GradientBase has been set.
 */
bool GradientBase::isSetId () const
{
    return (mId.empty() == false);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the value of the "id" attribute of this GradientBase.
 *
 * @param id the new id for the GradientBase 
 *
 * @return status if the operation succeeded
 */
int GradientBase::setId (const std::string& id)
{
    if (!(SyntaxChecker::isValidSBMLSId(id)))
    {
        return LIBSBML_INVALID_ATTRIBUTE_VALUE;
    }
    else
    {
        mId = id;
        return LIBSBML_OPERATION_SUCCESS;
    }
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Unsets the value of the "id" attribute of this GradientBase.
 */
int GradientBase::unsetId ()
{
    mId.erase();
    if (mId.empty())
  {
    return LIBSBML_OPERATION_SUCCESS;
  }
  else
  {
    return LIBSBML_OPERATION_FAILED;
  }
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the GradientBase with the given index or NULL if
 * the index is invalid.
 * 
 * @param i index of the GradientBase object to be returned
 * 
 * @return pointer to the GradientBase at the given index or NULL.
 */
GradientBase* ListOfGradientDefinitions::get(unsigned int i)
{
    return static_cast<GradientBase*>(this->ListOf::get(i));
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the GradientBase with the given index or NULL if
 * the index is invalid.
 * 
 * @param i index of the GradientBase object to be returned
 * 
 * @return const pointer to the GradientBase at the given index or NULL.
 */
const GradientBase* ListOfGradientDefinitions::get(unsigned int i) const
{
    return static_cast<const GradientBase*>(this->ListOf::get(i));
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Used by ListOf::get() to lookup an SBase based by its id.
 */
struct IdEqGradientBase : public std::unary_function<SBase*, bool>
{
    const std::string& id;

    IdEqGradientBase (const std::string& id) : id(id) { }
    bool operator() (SBase* sb) 
    { return static_cast <GradientBase *> (sb)->getId() == id; }
};
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the GradientBase with the given @p id or @c NULL if
 * the id is invalid.
 * 
 * @param id id of the GradientBase object to be returned
 * 
 * @return pointer to the GradientBase at the given @p id or @c NULL.
 */
GradientBase* ListOfGradientDefinitions::get(const std::string& id)
{
    return const_cast<GradientBase*>( 
            static_cast<const ListOfGradientDefinitions*>(this)->get(id) );
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the GradientBase with the given @p id or @c NULL if
 * the id is invalid.
 * 
 * @param id id of the GradientBase object to be returned
 * 
 * @return const pointer to the GradientBase at the given @p id or @c NULL.
 */
const GradientBase* ListOfGradientDefinitions::get(const std::string& id) const
{
    std::vector<SBase*>::const_iterator result;

    result = std::find_if( mItems.begin(), mItems.end(), IdEqGradientBase(id) );
    return (result == mItems.end()) ? 0 : static_cast <GradientBase*> (*result);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/* Removes the nth item from this list */
    GradientBase*
ListOfGradientDefinitions::remove (unsigned int n)
{
    return static_cast<GradientBase*>(ListOf::remove(n));
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/* Removes item in this list by id */
    GradientBase*
ListOfGradientDefinitions::remove (const std::string& sid)
{
    SBase* item = NULL;
    std::vector<SBase*>::iterator result;

    result = std::find_if( mItems.begin(), mItems.end(), IdEqGradientBase(sid) );

    if (result != mItems.end())
    {
        item = *result;
        mItems.erase(result);
    }

    return static_cast <GradientBase*> (item);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the parent SBMLDocument of this SBML object.
 *
 * @param d The SBMLDocument to set on the objects and it's children if there are any.
 */
    void
GradientBase::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);
  this->mGradientStops.setSBMLDocument(d);
}


/*
 * Sets this SBML object to child SBML objects (if any).
 * (Creates a child-parent relationship by the parent)
 */
void
GradientBase::connectToChild()
{
  SBase::connectToChild();
  mGradientStops.connectToParent(this);
}

/*
 * Enables/Disables the given package with this element and child
 * elements (if any).
 * (This is an internal implementation for enablePakcage function)
 */
void
GradientBase::enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag)
{
  SBase::enablePackageInternal(pkgURI,pkgPrefix,flag);

  mGradientStops.enablePackageInternal(pkgURI,pkgPrefix,flag);
}




/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the parent SBML object of this SBML object.
 *
 * @param sb the SBML object to use
 */
    void 
GradientBase::setParentSBMLObject (SBase* sb)
{
    this->mParentSBMLObject = sb;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/* function returns true if component has all the required
 * attributes
 */
bool GradientBase::hasRequiredAttributes() const
{
    bool result = this->SBase::hasRequiredAttributes();
    result = result && this->isSetId();
    return result;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/* function returns true if component has all the required
 * elements
 */
bool GradientBase::hasRequiredElements() const 
{
    bool result = this->SBase::hasRequiredElements();
    return result;
}
/** @endcond */



LIBSBML_CPP_NAMESPACE_END 

/**
 * @file    ColorDefinition.cpp
 * @brief   class for rgba color definitions
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

#include "ColorDefinition.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <string.h>
#ifndef OMIT_DEPRECATED
#ifdef DEPRECATION_WARNINGS
#include <iostream>
#endif // DEPRECATION_WARNINGS
#endif // OMIT_DEPRECATED
#include "sbml/packages/layout/util/LayoutUtilities.h"
#include "sbml/xml/XMLInputStream.h"
#include <sbml/packages/render/extension/RenderExtension.h>

#include <assert.h>

LIBSBML_CPP_NAMESPACE_BEGIN
const std::string ColorDefinition::ELEMENT_NAME="colorDefinition";
  
const std::string ListOfColorDefinitions::ELEMENT_NAME="listOfColorDefinitions";

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new ColorDefinition object with the given SBML level
 * and SBML version.
 *
 * @param level SBML level of the new object
 * @param level SBML version of the new object
 */
ColorDefinition::ColorDefinition (unsigned int level, unsigned int version, unsigned int pkgVersion) : 
    SBase(level,version)
    ,mId("")
    ,mRed(0)
    ,mGreen(0)
    ,mBlue(0)
    ,mAlpha(255)
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw SBMLConstructorException();

    // set an SBMLNamespaces derived object (GroupsPkgNamespaces) of this package.
  setSBMLNamespacesAndOwn(new RenderPkgNamespaces(level,version,pkgVersion));  

  // connect child elements to this element.
  connectToChild();

}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new ColorDefinition object with the given SBMLNamespaces.
 *
 * @param sbmlns The SBML namespace for the object.
 */
ColorDefinition::ColorDefinition (RenderPkgNamespaces* renderns) :
    SBase(renderns)
    ,mId("")
    ,mRed(0)
    ,mGreen(0)
    ,mBlue(0)
    ,mAlpha(255)
{
  if (!hasValidLevelVersionNamespaceCombination())
    throw SBMLConstructorException();

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
 * Creates a new ColorDefinition object from the given XMLNode object.
 * The XMLNode object has to contain a valid XML representation of a 
 * ColorDefinition object as defined in the render extension specification.
 * This method is normally called when render information is read from a file and 
 * should normally not have to be called explicitely.
 *
 * @param node the XMLNode object reference that describes the ColorDefinition
 * object to be instantiated.
 */
ColorDefinition::ColorDefinition(const XMLNode& node, unsigned int l2version)
  : SBase(2, l2version)
{
    const XMLAttributes& attributes=node.getAttributes();
    const XMLNode* child;
    ExpectedAttributes ea;
    addExpectedAttributes(ea);
    this->readAttributes(attributes, ea);
    unsigned int n=0,nMax = node.getNumChildren();
    while(n<nMax)
    {
        child=&node.getChild(n);
        const std::string& childName=child->getName();
        if(childName=="annotation")
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

#ifndef OMIT_DEPRECATED
/** @cond doxygenLibsbmlInternal */
/*
 * Constructor which sets the ColorDefinition to completely opaque
 * black and sets the id to the given string.
 *
 * @param id the id of the color definition. The user has to make sure 
 * that the id is unique within the given set of color definitions. 
 *
 * This constructor is deprecated. The new libsbml API only has
 * constructors which take the SBML level and version or one that takes
 * an SBMLNamespaces object.
 */
ColorDefinition::ColorDefinition(RenderPkgNamespaces* renderns, const std::string& id)
   :SBase(renderns)
    ,mId(id)
    ,mRed(0)
    ,mGreen(0)
    ,mBlue(0)
    ,mAlpha(255)
{

  // set the element namespace of this object
  setElementNamespace(renderns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(renderns);

#ifdef DEPRECATION_WARNINGS
    std::cerr << "Warning. ColorDefinition::ColorDefinition(const std::string& id) is deprecated." << std::endl;
#endif // DEPRECATION_WARNINGS
}
/** @endcond */
#endif // OMIT_DEPRECATED

#ifndef OMIT_DEPRECATED
/** @cond doxygenLibsbmlInternal */
/*
 * Constructor which sets the ColorDefinition to the given RGBA values
 * and sets the id.
 *
 * @param id the id of the color definition. The user has to make sure 
 * that the id is unique within the given set of color definitions. 
 * @param r Red component value. Has to be in the range of 0 to 255.
 * @param g Green component value. Has to be in the range of 0 to 255.
 * @param b Blue component value. Has to be in the range of 0 to 255.
 * @param a Alpha component value. Has to be in the range of 0 to 255. 
 * The alpha component can be omitted. In that case it has a default value of 255.
 *
 * This constructor is deprecated. The new libsbml API only has
 * constructors which take the SBML level and version or one that takes
 * an SBMLNamespaces object.
 */
ColorDefinition::ColorDefinition(RenderPkgNamespaces* renderns, const std::string& id,unsigned char r,unsigned char g,unsigned char b,unsigned char a)
   :SBase(renderns)
    ,mId(id)
    ,mRed(r)
    ,mGreen(g)
    ,mBlue(b)
    ,mAlpha(a)
{

  // set the element namespace of this object
  setElementNamespace(renderns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(renderns);

#ifdef DEPRECATION_WARNINGS
    std::cerr << "Warning. ColorDefinition::ColorDefinition(const std::string& id,unsigned char r,unsigned char g,unsigned char b,unsigned char a) is deprecated." << std::endl;
#endif // DEPRECATION_WARNINGS
}
/** @endcond */
#endif // OMIT_DEPRECATED


#ifndef OMIT_DEPRECATED
/** @cond doxygenLibsbmlInternal */
/*
 * Constructor which sets the ColorDefinition to the given RGBA values.
 *
 * @param r Red component value. Has to be in the range of 0 to 255.
 * @param g Green component value. Has to be in the range of 0 to 255.
 * @param b Blue component value. Has to be in the range of 0 to 255.
 * @param a Alpha component value. Has to be in the range of 0 to 255. 
 * The alpha component can be omitted. In that case it has a default value of 255.
 *
 * This constructor is deprecated. The new libsbml API only has
 * constructors which take the SBML level and version or one that takes
 * an SBMLNamespaces object.
 */
ColorDefinition::ColorDefinition(RenderPkgNamespaces* renderns, unsigned char r,unsigned char g,unsigned char b,unsigned char a)
   :SBase(renderns)
    ,mId()
    ,mRed(r)
    ,mGreen(g)
    ,mBlue(b)
    ,mAlpha(a)
{

  // set the element namespace of this object
  setElementNamespace(renderns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(renderns);

#ifdef DEPRECATION_WARNINGS
    std::cerr << "Warning. ColorDefinition::ColorDefinition(unsigned char r,unsigned char g,unsigned char b,unsigned char a) is deprecated." << std::endl;
#endif // DEPRECATION_WARNINGS
}
/** @endcond */
#endif // OMIT_DEPRECATED

/** @cond doxygenLibsbmlInternal */
/*
 * Destroys this ColorDefinition object.
 */
ColorDefinition::~ColorDefinition(){}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Returns the red color component.
 *
 * @return the red color component for the ColorDefinition.
 */
unsigned char ColorDefinition::getRed() const
{
    return mRed;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the green color component.
 *
 * @return the green color component for the ColorDefinition.
 */
unsigned char ColorDefinition::getGreen() const
{
    return mGreen;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the blue color component.
 *
 * @return the blue color component for the ColorDefinition.
 */
unsigned char ColorDefinition::getBlue() const
{
    return mBlue;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the alpha color component.
 *
 * @return the alpha color component for the ColorDefinition.
 */
unsigned char ColorDefinition::getAlpha() const
{
    return mAlpha;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the red color component.
 *
 * @param c the new red component value for the color definition.
 */
void ColorDefinition::setRed(unsigned char c)
{
    this->mRed=c;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the green color component.
 *
 * @param c the new green component value for the color definition.
 */
void ColorDefinition::setGreen(unsigned char c)
{
    this->mGreen=c;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the blue color component.
 *
 * @param c the new blue component value for the color definition.
 */
void ColorDefinition::setBlue(unsigned char c)
{
    this->mBlue=c;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets alpha red color component.
 *
 * @param c the new alpha component value for the color definition.
 */
void ColorDefinition::setAlpha(unsigned char c)
{
    this->mAlpha=c;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the red green, blue and alpha color component.
 * The alpha value is optional and defaults to 255 if not given.
 * @param r Red component value. Has to be in the range of 0 to 255.
 * @param g Green component value. Has to be in the range of 0 to 255.
 * @param b Blue component value. Has to be in the range of 0 to 255.
 * @param a Alpha component value. Has to be in the range of 0 to 255. 
 * The alpha component can be omitted. In that case it has a default value of 255.
 */
void ColorDefinition::setRGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    this->mRed=r;
    this->mGreen=g;
    this->mBlue=b;
    this->mAlpha=a;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates and returns a deep copy of this ColorDefinition object.
 * 
 * @return a (deep) copy of this ColorDefinition object
 */
ColorDefinition* 
ColorDefinition::clone () const
{
    return new ColorDefinition(*this);
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Sets the color value from a given value string.
 * If the string is not a valid value string, the color value is set to
 * black and false is returned.
 *
 * @param valueString A const reference to a string that represents a valid color value,
 * e.g. "#FFFFFFFF" for fully opaque white.
 *
 * @return true or false depending on whether setting the color value from the string
 * was successfull.
 */
bool ColorDefinition::setColorValue(const std::string& valueString)
{
    bool result=true;	
    size_t first_letter=valueString.find_first_not_of(std::string(" \t\r\n"));
    if(first_letter == std::string::npos)
    {
        result=false;
    }
    else
    {
      std::string trimmed=valueString.substr(first_letter,valueString.find_last_not_of(" \t\r\n")-first_letter+1);
      if(trimmed[0]=='#' && (trimmed.size() == 7 || trimmed.size() == 9) && trimmed.find_first_not_of("0123456789ABCDEFabcdef",1) == std::string::npos)
      {
        this->mRed=(unsigned char)strtol(trimmed.substr(1,2).c_str(),NULL,16);
        this->mGreen=(unsigned char)strtol(trimmed.substr(3,2).c_str(),NULL,16);
        this->mBlue=(unsigned char)strtol(trimmed.substr(5,2).c_str(),NULL,16);
        if(trimmed.size() == 9)
        {
            // set the alpha value
            this->mAlpha=(unsigned char)strtol(trimmed.substr(7,2).c_str(),NULL,16);
        }
        else
        {
            // if no alpha is given, the color is completely opaque
            this->mAlpha=255;
        }
      }
      else
      {
	 result=false;
      }
    }
    if(result==false)
     {
         this->mRed=0;
         this->mGreen=0;
         this->mBlue=0;
         this->mAlpha=255;
     }
     return result;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a string the represents the current color value.
 *
 * @return The string representation of the color value.
 */
std::string ColorDefinition::createValueString() const
{
   std::ostringstream os;
   os << "#" << std::hex << std::setw(2) << std::setfill('0') <<  (unsigned int)mRed << std::hex << std::setw(2) << std::setfill('0') <<  (unsigned int)mGreen << std::hex << std::setw(2) << std::setfill('0') <<  (unsigned int)mBlue;
   if(mAlpha!=255)
   {
     os  << std::hex << std::setw(2) << std::setfill('0') <<  (unsigned int)mAlpha;
   }
   return os.str();
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void
ColorDefinition::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("value");
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void ColorDefinition::readAttributes (const XMLAttributes& attributes, const ExpectedAttributes& expectedAttributes)
{
  SBase::readAttributes(attributes,expectedAttributes);
  std::string valueString;
  attributes.readInto("id", mId, getErrorLog(), false, getLine(), getColumn());
  attributes.readInto("value", valueString, getErrorLog(), false, getLine(), getColumn());
  this->setColorValue(valueString);
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
void ColorDefinition::writeElements (XMLOutputStream& stream) const
{
  SBase::writeElements(stream);
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
void ColorDefinition::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);
  stream.writeAttribute("id", getPrefix(), this->getId());
  stream.writeAttribute("value", getPrefix(), this->createValueString());
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the libSBML type code for this %SBML object.
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
 * @return the SBML type code for this object, or @c SBML_UNKNOWN (default).
 *
 * @see getElementName()
 */
int
ColorDefinition::getTypeCode () const
{
  return SBML_RENDER_COLORDEFINITION;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the XML element name of this object, which for
 * ListOfColorDefinitions, is always @c "colorDefinition".
 * 
 * @return the name of this element, i.e., @c "colorDefinition".
 */
const std::string& ColorDefinition::getElementName () const
{
  static std::string name = ColorDefinition::ELEMENT_NAME;
  return name;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates an XMLNode object from this ListOfColorDefinitions object.
 *
 * @return the XMLNode with the XML representation for the 
 * ColorDefinition object.
 */
XMLNode ColorDefinition::toXML() const
{
  return getXmlNodeForSBase(this);  
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Subclasses should override this method to create, store, and then
 * return an SBML object corresponding to the next XMLToken in the
 * XMLInputStream.
 *
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase*
ColorDefinition::createObject (XMLInputStream& stream)
{
  return NULL;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Accepts the given SBMLVisitor for this instance of ColorDefinition.
 *
 * @param v the SBMLVisitor instance to be used.
 *
 * @return the result of calling <code>v.visit()</code>.
 */
bool ColorDefinition::accept (SBMLVisitor& v) const
{
	return false;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new ListOfColorDefinitions object from the given XMLNode object.
 * The XMLNode object has to contain a valid XML representation of a 
 * ListOfColorDefinitions object as defined in the render extension specification.
 * This method is normally called when render information is read from a file and 
 * should normally not have to be called explicitely.
 *
 * @param node the XMLNode object reference that describes the ListOfColorDefinitions
 * object to be instantiated.
 */
ListOfColorDefinitions::ListOfColorDefinitions(const XMLNode& node)
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
        if(childName=="colorDefinition")
        {
            ColorDefinition* cd=new ColorDefinition(*child);
	    this->appendAndOwn(cd);
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
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Creates and returns a deep copy of the ListOfColorDefinitions object.
 *
 * @return a (deep) copy of this ListOfColorDefinitions
 */
ListOfColorDefinitions* ListOfColorDefinitions::clone () const
{
	return new ListOfColorDefinitions(*this);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Copy constructor. Creates a copy of this ListOfColorDefinitions object.
 */
ListOfColorDefinitions::ListOfColorDefinitions(const ListOfColorDefinitions& source):ListOf(source)
{
}
/** @endcond */

/*
 * Constructor which instantiates an empty ListOfColorDefinitions object.
 */
ListOfColorDefinitions::ListOfColorDefinitions(unsigned int level,
                         unsigned int version,
                         unsigned int pkgVersion)
                         : ListOf(level, version)
{
  setSBMLNamespacesAndOwn(new RenderPkgNamespaces(level,version,pkgVersion));
}

/*
 * Ctor.
 */
ListOfColorDefinitions::ListOfColorDefinitions(RenderPkgNamespaces* renderns) 
  : ListOf(renderns)
{
   setElementNamespace(renderns->getURI());
}


/** @cond doxygenLibsbmlInternal */
/*
 * Assignment operator for ListOfColorDefinitions objects.
 */
ListOfColorDefinitions& ListOfColorDefinitions::operator=(const ListOfColorDefinitions& source)
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
 * (i.e., ColorDefinition objects, if the list is non-empty).
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
int ListOfColorDefinitions::getItemTypeCode () const
{
	return SBML_RENDER_COLORDEFINITION;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the XML element name of this object, which for
 * ListOfColorDefinitions, is always @c "listOfColorDefinitions".
 * 
 * @return the name of this element, i.e., @c "listOfColorDefinitions".
 */
const std::string& ListOfColorDefinitions::getElementName () const
{
  static std::string name = ListOfColorDefinitions::ELEMENT_NAME;
  return name;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Creates an XMLNode object from this ListOfColorDefinitions object.
 *
 * @return the XMLNode with the XML representation for the 
 * ListOfColorDefinitions object.
 */
XMLNode ListOfColorDefinitions::toXML() const
{
  return getXmlNodeForSBase(this);
}
/** @endcond */
 

/** @cond doxygenLibsbmlInternal */
/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase* ListOfColorDefinitions::createObject (XMLInputStream& stream)
{
  const std::string& name   = stream.peek().getName();
  SBase*        object = NULL;


  if (name == "colorDefinition")
  {
    RENDER_CREATE_NS(renderns, this->getSBMLNamespaces());
    object = new ColorDefinition(renderns);
    if(object != NULL) this->mItems.push_back(object);
    delete renderns;
  }
  return object;
}
/** @endcond */

/*
 * Returns the value of the "id" attribute of this ColorDefinition.
 *
 * @return the id of the ColorDefinition
 */
const std::string& ColorDefinition::getId () const
{
  return mId;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Predicate returning @c true or @c false depending on whether this
 * GraphicalPrimitive's "id" attribute has been set.
 *
 * @return returns true or false depending on whether the id on the 
 * GraphicalPrimitive has been set.
 */
bool ColorDefinition::isSetId () const
{
  return (mId.empty() == false);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the value of the "id" attribute of this GraphicalPrimitive.
 *
 * @param id the new id for the GraphicalPrimitive 
 *
 * @return status if the operation succeeded
 */
int ColorDefinition::setId (const std::string& id)
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
 * Unsets the value of the "id" attribute of this ColorDefinition.
 */
int ColorDefinition::unsetId ()
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
 * Returns a pointer to the ColorDefinition with the given index or NULL if
 * the index is invalid.
 * 
 * @param i index of the ColorDefinition object to be returned
 * 
 * @return pointer to the ColorDefinition at the given index or NULL.
 */
ColorDefinition* ListOfColorDefinitions::get(unsigned int i)
{
    return static_cast<ColorDefinition*>(this->ListOf::get(i));
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the ColorDefinition with the given index or NULL if
 * the index is invalid.
 * 
 * @param i index of the ColorDefinition object to be returned
 * 
 * @return const pointer to the ColorDefinition at the given index or NULL.
 */
const ColorDefinition* ListOfColorDefinitions::get(unsigned int i) const
{
    return static_cast<const ColorDefinition*>(this->ListOf::get(i));
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Used by ListOf::get() to lookup an SBase based by its id.
 */
struct IdEqColorDefinition : public std::unary_function<SBase*, bool>
{
    const std::string& id;

    IdEqColorDefinition (const std::string& id) : id(id) { }
    bool operator() (SBase* sb) 
    { return static_cast <ColorDefinition *> (sb)->getId() == id; }
};
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the ColorDefinition with the given @p id or @c NULL if
 * the id is invalid.
 * 
 * @param id id of the ColorDefinition object to be returned
 * 
 * @return pointer to the ColorDefinition at the given @p id or @c NULL.
 */
ColorDefinition* ListOfColorDefinitions::get(const std::string& id)
{
    return const_cast<ColorDefinition*>( 
            static_cast<const ListOfColorDefinitions*>(this)->get(id) );
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the ColorDefinition with the given @p id or @c NULL if
 * the id is invalid.
 * 
 * @param id id of the ColorDefinition object to be returned
 * 
 * @return const pointer to the ColorDefinition at the given @p id or @c NULL.
 */
const ColorDefinition* ListOfColorDefinitions::get(const std::string& id) const
{
    std::vector<SBase*>::const_iterator result;

    result = std::find_if( mItems.begin(), mItems.end(), IdEqColorDefinition(id) );
    return (result == mItems.end()) ? 0 : static_cast <ColorDefinition*> (*result);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/* Removes the nth item from this list */
ColorDefinition*
ListOfColorDefinitions::remove (unsigned int n)
{
   return static_cast<ColorDefinition*>(ListOf::remove(n));
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Removes the nth item from this ListOfColorDefinitions items and returns a pointer to
 * it.
 *
 * The caller owns the returned item and is responsible for deleting it.
 *
 * @param n the index of the item to remove
 *
 * @see size()
 */
ColorDefinition*
ListOfColorDefinitions::remove (const std::string& sid)
{
  SBase* item = NULL;
  std::vector<SBase*>::iterator result;

  result = find_if( mItems.begin(), mItems.end(), IdEqColorDefinition(sid) );

  if (result != mItems.end())
  {
    item = *result;
    mItems.erase(result);
  }

  return static_cast <ColorDefinition*> (item);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/* function returns true if component has all the required
 * attributes
 */
bool ColorDefinition::hasRequiredAttributes() const
{
    bool result = this->SBase::hasRequiredAttributes();
    result = result && this->isSetId();
    // the color values can't be NaN
    result = result && 
             (this->mRed   == this->mRed)   &&
             (this->mGreen == this->mGreen) &&
             (this->mBlue  == this->mBlue)  &&
             (this->mAlpha == this->mAlpha);
    return result;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/* function returns true if component has all the required
 * elements
 */
bool ColorDefinition::hasRequiredElements() const 
{
    bool result = this->SBase::hasRequiredElements();
    return result;
}
/** @endcond */


LIBSBML_CPP_NAMESPACE_END

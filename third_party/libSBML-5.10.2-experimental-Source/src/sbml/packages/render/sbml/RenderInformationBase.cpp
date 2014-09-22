/**
 * @file    RenderInformationBase.cpp
 * @brief   abstract base class for local and global render information
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

#include "RenderInformationBase.h"

#include <sbml/util/ElementFilter.h>

#ifndef OMIT_DEPRECATED
#ifdef DEPRECATION_WARNINGS
#include <iostream>
#endif // DEPRECATION_WARNINGS
#endif // OMIT_DEPRECATED

#include <sbml/xml/XMLInputStream.h>

LIBSBML_CPP_NAMESPACE_BEGIN

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new RenderInformationBase object with the given SBML level
 * and SBML version.
 *
 * @param level SBML level of the new object
 * @param level SBML version of the new object
 */
RenderInformationBase::RenderInformationBase (unsigned int level, unsigned int version, unsigned int pkgVersion)
  : SBase(level,version)
  , mId("")
  , mName ("")
  , mProgramName ("")
  , mProgramVersion ("")
  , mReferenceRenderInformation ("")
  , mBackgroundColor("")
  , mListOfColorDefinitions(level, version, pkgVersion) 
  , mListOfGradientDefinitions(level, version, pkgVersion) 
  , mListOfLineEndings(level, version, pkgVersion) 
{
  setSBMLNamespacesAndOwn(new RenderPkgNamespaces(level,version,pkgVersion));  
    connectToChild();
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new RenderInformationBase object with the given SBMLNamespaces.
 *
 * @param sbmlns The SBML namespace for the object.
 */
RenderInformationBase::RenderInformationBase (RenderPkgNamespaces* renderns)
  : SBase(renderns)
  , mId("")
  , mName ("")
  , mProgramName ("")
  , mProgramVersion ("")
  , mReferenceRenderInformation ("")
  , mBackgroundColor("")
  , mListOfColorDefinitions(renderns) 
  , mListOfGradientDefinitions(renderns) 
  , mListOfLineEndings(renderns) 
{
      // set the element namespace of this object
  setElementNamespace(renderns->getURI());

  // connect child elements to this element.
  connectToChild();

  // load package extensions bound with this object (if any) 
  loadPlugins(renderns);
}
/** @endcond */


/*
 * Destroy this object.
 */
RenderInformationBase::~RenderInformationBase ()
{
}


List*
RenderInformationBase::getAllElements(ElementFilter* filter)
{
  List* ret = new List();
  List* sublist = NULL;

  ADD_FILTERED_LIST(ret, sublist, mListOfColorDefinitions, filter);
  ADD_FILTERED_LIST(ret, sublist, mListOfGradientDefinitions, filter);
  ADD_FILTERED_LIST(ret, sublist, mListOfLineEndings, filter);

  ADD_FILTERED_FROM_PLUGIN(ret, sublist, filter);

  return ret;
}



/** @cond doxygenLibsbmlInternal */
void
RenderInformationBase::addExpectedAttributes(ExpectedAttributes& attributes)
{
  SBase::addExpectedAttributes(attributes);

  attributes.add("id");
  attributes.add("name");
  attributes.add("programName");
  attributes.add("programVersion");
  attributes.add("referenceRenderInformation");
  attributes.add("backgroundColor");

}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
void RenderInformationBase::readAttributes (const XMLAttributes& attributes, const ExpectedAttributes& expectedAttributes)
{
  SBase::readAttributes(attributes, expectedAttributes);
    attributes.readInto("id", mId, getErrorLog(), false, getLine(), getColumn());
    attributes.readInto("name", mName, getErrorLog(), false, getLine(), getColumn());
    attributes.readInto("programName", mProgramName, getErrorLog(), false, getLine(), getColumn());
    attributes.readInto("programVersion", mProgramVersion, getErrorLog(), false, getLine(), getColumn());
    attributes.readInto("referenceRenderInformation", mReferenceRenderInformation, getErrorLog(), false, getLine(), getColumn());
    if(!attributes.readInto("backgroundColor",mBackgroundColor, getErrorLog(), false, getLine(), getColumn()))
    {
        // make white the default background  
        mBackgroundColor="#FFFFFFFF";
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Parses the xml information in the given node and sets the attributes.
 * This method should never be called by the user. It is only used to read render 
 * information from annotations.
 *
 * @param node the XMLNode object reference that describes the RenderInfromationBase
 * object to be instantiated.
 */
void RenderInformationBase::parseXML(const XMLNode& node)
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
        if(childName=="listOfColorDefinitions")
        {
            this->mListOfColorDefinitions=ListOfColorDefinitions(*child);
            this->mListOfColorDefinitions.setSBMLDocument(this->mSBML);
        }
        else if(childName=="listOfGradientDefinitions")
        {
            this->mListOfGradientDefinitions=ListOfGradientDefinitions(*child);
            this->mListOfGradientDefinitions.setSBMLDocument(this->mSBML);
        }
        else if(childName=="listOfLineEndings")
        {
            this->mListOfLineEndings=ListOfLineEndings(*child);
            this->mListOfLineEndings.setSBMLDocument(this->mSBML);
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



#ifndef OMIT_DEPRECATED
/** @cond doxygenLibsbmlInternal */
/*
 * Constructor which creates a RenderInformationBase object
 * empty color definition, gradient definition
 * and line endings set.
 * For the object to be valid a valid background color value.
 *
 * This constructor is deprecated. The new libsbml API only has
 * constructors which take the SBML level and version or one that takes
 * an SBMLNamespaces object.
 */
RenderInformationBase::RenderInformationBase(RenderPkgNamespaces* renderns, const std::string& id)
  : SBase(renderns)
  , mId(id)
  , mName ("")
  , mProgramName ("")
  , mProgramVersion ("")
  , mReferenceRenderInformation ("")
  , mBackgroundColor("#FFFFFFFF")
  , mListOfColorDefinitions(renderns) 
  , mListOfGradientDefinitions(renderns) 
  , mListOfLineEndings(renderns) 
{
#ifdef DEPRECATION_WARNINGS
    std::cerr << "Warning. RenderInformationBase::RenderInformationBase(const std::string& id) is deprecated." << std::endl;
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

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the program name that created the render information.
 *
 * @return the name string of the program
 */
const std::string& RenderInformationBase::getProgramName() const
{
    return this->mProgramName;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the name of the program that created the render information.
 *
 * @param name the name of the programm
 */
void RenderInformationBase::setProgramName(const std::string& name)
{
    this->mProgramName=name;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the version of the program that created the render information.
 *
 * @return the version of the program as a string.
 */
const std::string& RenderInformationBase::getProgramVersion() const
{
    return this->mProgramVersion;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the version string of the program that created the render information.
 *
 * @param version version string of the program
 */
void RenderInformationBase::setProgramVersion(const std::string& version)
{
    this->mProgramVersion=version;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the id of the referenced render information object.
 * RenderInfromation objects can reference other render information objects
 * and information that is not found in the current render information is then
 * expected to be in the referenced render information object.
 *
 * Global render information objects can only reference other global 
 * render information objects, local render information objects can reference other local
 * render information objects from the same list of local render information or other
 * global render infromation.
 *
 * @return the id of the referenced render infromation object.
 */
const std::string& RenderInformationBase::getReferenceRenderInformationId() const
{
    return this->mReferenceRenderInformation;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the id of the referenced render information object.
 * The user has to make sure that render infromation referencing 
 * does not create loops.
 *
 * @param id the id of the referenced render infromation
 */
void RenderInformationBase::setReferenceRenderInformationId(const std::string& id)
{
    this->mReferenceRenderInformation=id;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the number of color definitions.
 *
 * @return the number of color definitions in the render information.
 */
unsigned int RenderInformationBase::getNumColorDefinitions() const
{
    return this->mListOfColorDefinitions.size();
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the list of color definitions.
 *
 * @return pointer to the list of color definitions.
 */
ListOfColorDefinitions* RenderInformationBase::getListOfColorDefinitions()
{
    return &(this->mListOfColorDefinitions);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the list of color definitions.
 *
 * @return const pointer to the list of color definitions.
 */
const ListOfColorDefinitions* RenderInformationBase::getListOfColorDefinitions() const
{
    return &(this->mListOfColorDefinitions);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the color definition with the given index, or @c NULL
 * if the index is invalid.
 * 
 * @param index of the ColorDefinition object to be returned
 *
 * @return pointer to the ColorDefinition object at the given index or NULL
 */
ColorDefinition* RenderInformationBase::getColorDefinition(unsigned int index)
{
    return (index < this->mListOfColorDefinitions.size())?static_cast<ColorDefinition*>(this->mListOfColorDefinitions.get(index)):NULL;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the color definition with the given index, or @c NULL
 *if the index is invalid.
 * 
 * @param index of the ColorDefinition object to be returned
 *
 * @return const pointer to the ColorDefinition object at the given index or NULL
 */
const ColorDefinition* RenderInformationBase::getColorDefinition(unsigned int index) const
{
    return (index < this->mListOfColorDefinitions.size())?static_cast<const ColorDefinition*>(this->mListOfColorDefinitions.get(index)):NULL;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the color definition with the given @p id, or @c NULL
 * if there is no color definition with that id.
 *
 * @param id of the color definition object to be returned.
 *
 * @return a pointer to the color definition object with the given @p id 
 * or NULL if there is no color definition with given @p id
 */
ColorDefinition* RenderInformationBase::getColorDefinition(const std::string& id)
{
    return this->mListOfColorDefinitions.get(id);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the color definition with the given @p id, or @c NULL
 * if there is no color definition with that id.
 *
 * @param id of the color definition object to be returned.
 *
 * @return a const pointer to the color definition object with the given @p id 
 * or NULL if there is no color definition with given @p id
 */
const ColorDefinition* RenderInformationBase::getColorDefinition(const std::string& id) const
{
    return this->mListOfColorDefinitions.get(id);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new color definition object without an id.
 * For the object to be valid an id has to be set that is unique
 * within the list of color definitions and the list of gradients within 
 * the render information.
 *
 * The created ColorDefinition object is added to and owned by the render information.
 *
 * @return pointer to new ColorDefinition object
 */
ColorDefinition* RenderInformationBase::createColorDefinition()
{
    ColorDefinition* cd=NULL;
    try
    {
        RENDER_CREATE_NS(renderns, this->getSBMLNamespaces());
        cd = new ColorDefinition(renderns);
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


    if(cd != NULL)
    {
        this->mListOfColorDefinitions.appendAndOwn(cd);
    }
    return cd;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Removes the color definition with the given index from the list of color definitions.
 * If the index is valid, the object is removed and a pointer to the removed object 
 * is returned.
 * The caller is responsible for deleting thew object.
 * If the index is invalid, @c NULL is returned.
 *
 * @param index index of the color definition to be removed.
 *
 * @ return pointer to the removed object
 */
ColorDefinition* RenderInformationBase::removeColorDefinition(unsigned int index)
{
    ColorDefinition* pReturn=NULL;
    if(index < this->mListOfColorDefinitions.size())
    {
        pReturn=dynamic_cast<ColorDefinition*>(this->mListOfColorDefinitions.remove(index));
    }
    return pReturn;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Adds a copy of the given color definition to the end of the list of
 * color definitions.
 *
 * The color definition has to be valid, i.e. have a unique id and a valid color value.
 * Otherwise it is not added.
 *
 * @param cd const pointer to ColorDefinition object to be added
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
 * one contained in this Reaction.  Changes made to the original object
 * instance (such as resetting attribute values) will <em>not affect the
 * instance in the Reaction</em>.  In addition, the caller should make
 * sure to free the original object if it is no longer being used, or
 * else a memory leak will result.  Please see RenderInformationBase::createColorDefinition()
 * for a method that does not lead to these issues.
 *
 * @see createColorDefinition()
 */
int RenderInformationBase::addColorDefinition(const ColorDefinition* cd)
{
    if (cd == NULL)
    {
        return LIBSBML_OPERATION_FAILED;
    }
    else if (!(cd->hasRequiredAttributes()) || !(cd->hasRequiredElements()))
    {
        return LIBSBML_INVALID_OBJECT;
    }
    else if (getLevel() != cd->getLevel())
    {
        return LIBSBML_LEVEL_MISMATCH;
    }
    else if (getVersion() != cd->getVersion())
    {
        return LIBSBML_VERSION_MISMATCH;
    }
    else if (cd->isSetId() 
            && (getListOfColorDefinitions()->get(cd->getId())) != NULL)
    {
        // an object with this id already exists
        return LIBSBML_DUPLICATE_OBJECT_ID;
    }
    else
    {
 
        this->mListOfColorDefinitions.append(cd);

        return LIBSBML_OPERATION_SUCCESS;
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the number of gradient definitions in the render information.
 *
 * @return number of gradient definitions
 */
unsigned int RenderInformationBase::getNumGradientDefinitions() const
{
    return this->mListOfGradientDefinitions.size();
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the list of gradient definitions.
 *
 * @return pointer to ListOfGradientDefinitions
 */
ListOfGradientDefinitions* RenderInformationBase::getListOfGradientDefinitions()
{
    return &(this->mListOfGradientDefinitions);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the list of gradient definitions.
 *
 * @return const pointer to ListOfGradientDefinitions
 */
const ListOfGradientDefinitions* RenderInformationBase::getListOfGradientDefinitions() const
{
    return &(this->mListOfGradientDefinitions);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the gradient definition with the given index, or @c NULL
 * if the index is invalid.
 *
 * @param index index of the GradientDefinition object to be returned
 *
 * @return pointer to the GradientDefinition object with the given index or NULL 
 * if the index was invalid.
 */
GradientBase* RenderInformationBase::getGradientDefinition(unsigned int index)
{
    return (index < this->mListOfGradientDefinitions.size())?static_cast<GradientBase*>(this->mListOfGradientDefinitions.get(index)):NULL;
}

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the gradient definition with the given index, or @c NULL
 *if the index is invalid.
 *
 * @param index index of the GradientDefinition object to be returned
 *
 * @return const pointer to the GradientDefinition object with the given index or NULL 
 * if the index was invalid.
 */
const GradientBase* RenderInformationBase::getGradientDefinition(unsigned int index) const
{
    return (index < this->mListOfGradientDefinitions.size())?static_cast<const GradientBase*>(this->mListOfGradientDefinitions.get(index)):NULL;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the gradient definition with the given @p id, or @c NULL
 * if there is no gradient definition with that id.
 *
 * @param id of the gradient definition object to be returned.
 *
 * @return a pointer to the gradient definition object with the given @p id 
 * or NULL if there is no gradient definition with given @p id
 */
GradientBase* RenderInformationBase::getGradientDefinition(const std::string& id)
{
    return this->mListOfGradientDefinitions.get(id);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the gradient definition with the given @p id, or @c NULL
 * if there is no gradient definition with that id.
 *
 * @param id of the gradient definition object to be returned.
 *
 * @return a const pointer to the gradient definition object with the given @p id 
 * or NULL if there is no gradient definition with given @p id
 */
const GradientBase* RenderInformationBase::getGradientDefinition(const std::string& id) const
{
    return this->mListOfGradientDefinitions.get(id);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new radial gradient definition.
 * The newly created object is added to the render 
 * information and also owned by it.
 *
 * Since the newly created object has no id and no gradient stops, 
 * it is invalid until those things have been added.
 *
 * @return pointer to newly created RadialGradient object.
 */
RadialGradient* RenderInformationBase::createRadialGradientDefinition()
{
    RadialGradient* rg=NULL;
    try
    {
      RENDER_CREATE_NS(renderns, this->getSBMLNamespaces());
      rg = new RadialGradient(renderns);
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


    if(rg != NULL)
    {
        this->mListOfGradientDefinitions.appendAndOwn(rg);
    }
    return rg;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new linear gradient definition.
 * The newly created object is added to the render 
 * information and also owned by it.
 *
 * Since the newly created object has no id and no gradient stops, 
 * it is invalid until those things have been added.
 *
 * @return pointer to newly created LinearGradient object.
 */
LinearGradient* RenderInformationBase::createLinearGradientDefinition()
{
    LinearGradient* lg=NULL;
    try
    {
      RENDER_CREATE_NS(renderns, this->getSBMLNamespaces());
      lg = new LinearGradient(renderns);
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


    if(lg != NULL)
    {
        this->mListOfGradientDefinitions.appendAndOwn(lg);
    }
    return lg;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Removes the gradient definition with the given index.
 * If the index is valid, the object is removed and a pointer to the removed object 
 * is returned.
 * The caller is responsible for deleting thew object.
 * If the index is invalid, @c NULL is returned.
 *
 * @param index index of the gradient definition object to be removed.
 *
 * @ return pointer to the removed object
 */
GradientBase* RenderInformationBase::removeGradientDefinition(unsigned int index)
{
    GradientBase* pReturn=NULL;
    if(index < this->mListOfGradientDefinitions.size())
    {
        pReturn=dynamic_cast<GradientBase*>(this->mListOfGradientDefinitions.remove(index));
    }
    return pReturn;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Adds a copy of the given gradient definition to the end of the list of
 * gradient definitions.
 * The Gradient definition has to be valid, so is has to have at least two
 * gradient stops and an id.
 *
 * @param gradient GradientDefinition object to be added
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
 * one contained in this RenderInformationBase.  Changes made to the original object
 * instance (such as resetting attribute values) will <em>not affect the
 * instance in the RenderInformationBase</em>.  In addition, the caller should make
 * sure to free the original object if it is no longer being used, or
 * else a memory leak will result.  Please see 
 * RenderInformationBase::createLinearGradientDefinition() or
 * RenderInformationBase::createRadialGradientDefinition()
 * for methods that does not lead to these issues.
 *
 * @see createRadialGradientDefinition()
 * @see createLinearGradientDefinition()
 */
int RenderInformationBase::addGradientDefinition(const GradientBase* gradient)
{
    if (gradient == NULL)
    {
        return LIBSBML_OPERATION_FAILED;
    }
    else if (!(gradient->hasRequiredAttributes()) || !(gradient->hasRequiredElements()))
    {
        return LIBSBML_INVALID_OBJECT;
    }
    else if (getLevel() != gradient->getLevel())
    {
        return LIBSBML_LEVEL_MISMATCH;
    }
    else if (getVersion() != gradient->getVersion())
    {
        return LIBSBML_VERSION_MISMATCH;
    }
    else if (gradient->isSetId() 
            && (getListOfGradientDefinitions()->get(gradient->getId())) != NULL)
    {
        // an object with this id already exists
        return LIBSBML_DUPLICATE_OBJECT_ID;
    }
    else
    {

        this->mListOfGradientDefinitions.append(gradient);

        return LIBSBML_OPERATION_SUCCESS;
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the number of line endings for the render information.
 *
 * @return number of line endings in the render information.
 */
unsigned int RenderInformationBase::getNumLineEndings() const
{
    return this->mListOfLineEndings.size();
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the list of line endings.
 *
 * @return pointer to the list of line endings.
 */
ListOfLineEndings* RenderInformationBase::getListOfLineEndings()
{
    return &(this->mListOfLineEndings);
}

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the list of line endings.
 *
 * @return const pointer to the list of line endings.
 */
const ListOfLineEndings* RenderInformationBase::getListOfLineEndings() const
{
    return &(this->mListOfLineEndings);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the line ending with the given index, or @c NULL
 * if the index is invalid.
 *
 * @param index of the line ending object to be returned.
 *
 * @return a pointer to the line ending object with the given index 
 * or NULL if the index was out of bounds.
 */
LineEnding* RenderInformationBase::getLineEnding(unsigned int index)
{
    return (index < this->mListOfLineEndings.size())?static_cast<LineEnding*>(this->mListOfLineEndings.get(index)):NULL;
}

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the line ending with the given index, or @c NULL
 * if the index is invalid.
 *
 * @param index of the line ending object to be returned.
 *
 * @return a const pointer to the line ending object with the given index 
 * or NULL if the index was out of bounds.
 */
const LineEnding* RenderInformationBase::getLineEnding(unsigned int index) const
{
    return (index < this->mListOfLineEndings.size())?static_cast<const LineEnding*>(this->mListOfLineEndings.get(index)):NULL;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a pointer to the line ending with the given @p id, or @c NULL
 * if there is no line ending with that id.
 *
 * @param id of the line ending object to be returned.
 *
 * @return a pointer to the line ending object with the given @p id 
 * or NULL if there is no line ending with given @p id
 */
LineEnding* RenderInformationBase::getLineEnding(const std::string& id)
{
    return this->mListOfLineEndings.get(id);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns a const pointer to the line ending with the given @p id, or @c NULL
 * if there is no line ending with that id.
 *
 * @param id of the line ending object to be returned.
 *
 * @return a const pointer to the line ending object with the given @p id 
 * or NULL if there is no line ending with given @p id
 */
const LineEnding* RenderInformationBase::getLineEnding(const std::string& id) const
{
    return this->mListOfLineEndings.get(id);
}
/** @endcond */



/** @cond doxygenLibsbmlInternal */
/*
 * Creates a new line ending.
 * The new line ending object is added to and owned by the 
 * render information. Since it does not have an id or a valid group
 * or a valid viewport, it should be considered invalid until those
 * things have been set.
 *
 * @return pointer to the newlyy created LineEnding object
 */
LineEnding* RenderInformationBase::createLineEnding()
{
    LineEnding* le=NULL;
    try
    {
      RENDER_CREATE_NS(renderns, this->getSBMLNamespaces());
      le = new LineEnding(renderns);
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


    if(le != NULL)
    {
        this->mListOfLineEndings.appendAndOwn(le);
    }
    return le;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Removes the line ending with the given index.
 * If the index is valid, the object is removed and a pointer to the removed object 
 * is returned.
 * The caller is responsible for deleting thew object.
 * If the index is invalid, @c NULL is returned.
 *
 * @param index index of the object to be removed.
 *
 * @ return pointer to the removed object
 */
LineEnding* RenderInformationBase::removeLineEnding(unsigned int index)
{
    LineEnding* pReturn = NULL;
    if(index < this->mListOfLineEndings.size())
    {
        pReturn = dynamic_cast<LineEnding*>(this->mListOfLineEndings.remove(index));
    }
    return pReturn;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Adds a copy of the given line ending to the end of the list of line
 * endings.
 * The new LineEnding is only added if it is valid.
 *
 * @param le const pointer to LineEnding to be added
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
 * one contained in this RenderInformationBase.  Changes made to the original object
 * instance (such as resetting attribute values) will <em>not affect the
 * instance in the RenderInformationBase</em>.  In addition, the caller should make
 * sure to free the original object if it is no longer being used, or
 * else a memory leak will result.  Please see RenderInformationBase::createLineEnding()
 * for a method that does not lead to these issues.
 *
 * @see createLineEnding()
 */
int RenderInformationBase::addLineEnding(const LineEnding* le)
{
    if (le == NULL)
    {
        return LIBSBML_OPERATION_FAILED;
    }
    else if (!(le->hasRequiredAttributes()) || !(le->hasRequiredElements()))
    {
        return LIBSBML_INVALID_OBJECT;
    }
    else if (getLevel() != le->getLevel())
    {
        return LIBSBML_LEVEL_MISMATCH;
    }
    else if (getVersion() != le->getVersion())
    {
        return LIBSBML_VERSION_MISMATCH;
    }
    else if (le->isSetId() 
            && (getListOfLineEndings()->get(le->getId())) != NULL)
    {
        // an object with this id already exists
        return LIBSBML_DUPLICATE_OBJECT_ID;
    }
    else
    {

        this->mListOfLineEndings.append(le);

        return LIBSBML_OPERATION_SUCCESS;
    }
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
void RenderInformationBase::writeAttributes (XMLOutputStream& stream) const
{
  SBase::writeAttributes(stream);
  stream.writeAttribute("id", getPrefix(), this->mId);
  if(this->isSetName())
  {
      stream.writeAttribute("name", getPrefix(), this->mName);
  }
  if(!this->mReferenceRenderInformation.empty())
  {
      stream.writeAttribute("referenceRenderInformation", getPrefix(), this->mReferenceRenderInformation);
  }
  if(!this->mProgramName.empty())
  {
      stream.writeAttribute("programName", getPrefix(), this->mProgramName);
  }
  if(!this->mProgramVersion.empty())
  {
      stream.writeAttribute("programVersion", getPrefix(), this->mProgramVersion);
  }
  if(!this->mBackgroundColor.empty())
  {
      stream.writeAttribute("backgroundColor", getPrefix(), this->mBackgroundColor);
  }
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Adds the RenderInformationBase specific attributes to the given XMLAttributes object.
 */
void RenderInformationBase::addRenderInformationBaseAttributes(const RenderInformationBase& object,XMLAttributes& att)
{
    att.add("id",object.mId);
    if(object.isSetName())
    {
        att.add("name",object.mName);
    }
    if(!object.mReferenceRenderInformation.empty())
    {
        att.add("referenceRenderInformation",object.mReferenceRenderInformation);
    }
    if(!object.mProgramName.empty())
    {
        att.add("programName",object.mProgramName);
    }
    if(!object.mProgramVersion.empty())
    {
        att.add("programVersion",object.mProgramVersion);
    }
    if(!object.mBackgroundColor.empty())
    {
        att.add("backgroundColor",object.mBackgroundColor);
    }
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the background color which is either the id of a color in the
 * list of color definitions, or a color value.
 *
 * @return background color id or value string
 */
const std::string& RenderInformationBase::getBackgroundColor() const
{
    return mBackgroundColor;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the background color to either the id of a color in the list of
 * color definitions, or a color value.
 *
 * @param bg id of a color definition or a valid color value to be used as background color.
 */
void RenderInformationBase::setBackgroundColor(const std::string& bg)
{
    this->mBackgroundColor=bg;
}

/** @cond doxygenLibsbmlInternal */
/*
 * Returns the value of the "id" attribute of this RenderInformationBase.
 *
 * @return the id of the RenderInformationBase
 */
const std::string& RenderInformationBase::getId () const
{
    return mId;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Predicate returning @c true or @c false depending on whether this
 * RenderInformationBase's "id" attribute has been set.
 *
 * @return returns true or false depending on whether the id on the 
 * RenderInformationBase has been set.
 */
bool RenderInformationBase::isSetId () const
{
    return (mId.empty() == false);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the value of the "id" attribute of this RenderInformationBase.
 *
 * @param id the new id for the RenderInformationBase 
 *
 * @return status if the operation succeeded
 */
int RenderInformationBase::setId (const std::string& id)
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
 * Unsets the value of the "id" attribute of this RenderInformationBase.
 */
int RenderInformationBase::unsetId ()
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
 * Returns the value of the "name" attribute of this RenderInformationBase.
 *
 * @return the name of the RenderInformationBase
 */
const std::string& RenderInformationBase::getName () const
{
    return mName;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Predicate returning @c true or @c false depending on whether this
 * RenderInformationBase's "name" attribute has been set.
 *
 * @return returns true or false depending on whether the name on the 
 * RenderInformationBase has been set.
 */
bool RenderInformationBase::isSetName () const
{
    return (mName.empty() == false);
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * Sets the value of the "name" attribute of this RenderInformationBase.
 *
 * @param name the new name for the RenderInformationBase 
 *
 * @return status if the operation succeeded
 */
int RenderInformationBase::setName (const std::string& name)
{
    mName = name;
    return LIBSBML_OPERATION_SUCCESS;
}
/** @endcond */


/** @cond doxygenLibsbmlInternal */
/*
 * Unsets the value of the "name" attribute of this RenderInformationBase.
 */
int RenderInformationBase::unsetName ()
{
    mName.erase();
    if (mName.empty())
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
 * Sets the parent SBMLDocument of this SBML object.
 *
 * @param d The SBMLDocument to set on the objects and it's children if there are any.
 */
    void
RenderInformationBase::setSBMLDocument (SBMLDocument* d)
{
  SBase::setSBMLDocument(d);
    this->mListOfColorDefinitions.setSBMLDocument(d);
    this->mListOfGradientDefinitions.setSBMLDocument(d);
    this->mListOfLineEndings.setSBMLDocument(d);
}
/** @endcond */


/*
 * Sets this SBML object to child SBML objects (if any).
 * (Creates a child-parent relationship by the parent)
 */
void
RenderInformationBase::connectToChild()
{
  SBase::connectToChild();
  mListOfColorDefinitions.connectToParent(this);
  mListOfGradientDefinitions.connectToParent(this);
  mListOfLineEndings.connectToParent(this);
}

/*
 * Enables/Disables the given package with this element and child
 * elements (if any).
 * (This is an internal implementation for enablePakcage function)
 */
void
RenderInformationBase::enablePackageInternal(const std::string& pkgURI,
                                     const std::string& pkgPrefix, bool flag)
{
  SBase::enablePackageInternal(pkgURI,pkgPrefix,flag);

  mListOfColorDefinitions.enablePackageInternal(pkgURI,pkgPrefix,flag);
  mListOfGradientDefinitions.enablePackageInternal(pkgURI,pkgPrefix,flag);
  mListOfLineEndings.enablePackageInternal(pkgURI,pkgPrefix,flag);
}



/** @cond doxygenLibsbmlInternal */
/*
 * Sets the parent SBML object of this SBML object.
 *
 * @param sb the SBML object to use
 */
    void 
RenderInformationBase::setParentSBMLObject (SBase* sb)
{
    this->mParentSBMLObject = sb;
}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/* function returns true if component has all the required
 * attributes
 */
bool RenderInformationBase::hasRequiredAttributes() const
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
bool RenderInformationBase::hasRequiredElements() const 
{
    bool result = this->SBase::hasRequiredElements();
    return result;
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
void RenderInformationBase::writeElements (XMLOutputStream& stream) const
{
    SBase::writeElements(stream);
    if(this->mListOfColorDefinitions.size() > 0)
    {
      this->mListOfColorDefinitions.write(stream);
    }
    if(this->mListOfGradientDefinitions.size() > 0)
    {
      this->mListOfGradientDefinitions.write(stream);
    }
    if(this->mListOfLineEndings.size() > 0)
    {
      this->mListOfLineEndings.write(stream);
    }

}
/** @endcond */

/** @cond doxygenLibsbmlInternal */
/*
 * @return the SBML object corresponding to next XMLToken in the
 * XMLInputStream or NULL if the token was not recognized.
 */
SBase* RenderInformationBase::createObject (XMLInputStream& stream)
{
    const std::string& name   = stream.peek().getName();
    SBase*        object = NULL;

    if (name == "listOfColorDefinitions")
    {
        object = &this->mListOfColorDefinitions;
    }
    else if (name == "listOfGradientDefinitions")
    {
        object = &this->mListOfGradientDefinitions;
    }
    else if (name == "listOfLineEndings")
    {
        object = &this->mListOfLineEndings;
    }
    return object;
}
/** @endcond */


LIBSBML_CPP_NAMESPACE_END 

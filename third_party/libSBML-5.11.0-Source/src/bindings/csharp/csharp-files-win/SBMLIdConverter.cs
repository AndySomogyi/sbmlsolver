/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace libsbml {

 using System;
 using System.Runtime.InteropServices;

/** 
 * @sbmlpackage{core}
 *
@htmlinclude pkg-marker-core.html Converter for replacing object identifiers.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * This converter translates all instances of a given identifier (i.e., SBML object 'id'
 * attribute value) of type 'SId' in a Model to another identifier.  It does this based on a list of source
 * identifiers, translating each one to its corresponding replacement value
 * in a list of replacement identifiers.  It also updates all references to
 * the identifiers so replaced.  (More technically, it replaces all values
 * known as type @c SIdRef in the SBML Level&nbsp;3 specifications.)
 *
 * This converter only searches the global SId namespace for the Model child of the 
 * SBMLDocument.  It does not replace any IDs or SIdRefs for LocalParameters, nor
 * does it replace any UnitSIds or UnitSIdRefs.  It likewise does not replace any IDs
 * in a new namespace introduced by a package, such as the PortSId namespace
 * from the Hierarchical %Model Composition package, nor any Model objects that are
 * not the direct child of the SBMLDocument, such as the ModelDefinitions from 
 * the Hierarchical %Model Composition package.
 *
 * If, however, a package introduces a new element with an 'id' attribute
 * of type SId, any attribute of type SIdRef, or child of type SIdRef (such as 
 * a new Math child of a package element), those IDs will be replaced if they
 * match a source identifier.
 *
 * @section SBMLIdConverter-usage Configuration and use of SBMLIdConverter
 *
 * SBMLIdConverter is enabled by creating a ConversionProperties object with
 * the option @c 'renameSIds', and passing this properties object to
 * SBMLDocument::convert(@if java ConversionProperties@endif).
 * The converter accepts two options, and both must
 * be set or else no conversion is performed:
 *
 * @li @c 'currentIds': A comma-separated list of identifiers to replace.
 * @li @c 'newIds': A comma-separated list of identifiers to use as the
 * replacements.  The values should correspond one-to-one with the identifiers
 * in @c 'currentIds' that should be replaced.
 *
 * *
 * @section using-converters General information about the use of SBML converters
 *
 * The use of all the converters follows a similar approach.  First, one
 * creates a ConversionProperties object and calls
 * ConversionProperties::addOption(@if java ConversionOption@endif)
 * on this object with one arguments: a text string that identifies the desired
 * converter.  (The text string is specific to each converter; consult the
 * documentation for a given converter to find out how it should be enabled.)
 *
 * Next, for some converters, the caller can optionally set some
 * converter-specific properties using additional calls to
 * ConversionProperties::addOption(@if java ConversionOption@endif).
 * Many converters provide the ability to
 * configure their behavior to some extent; this is realized through the use
 * of properties that offer different options.  The default property values
 * for each converter can be interrogated using the method
 * SBMLConverter::getDefaultProperties() on the converter class in question .
 *
 * Finally, the caller should invoke the method
 * SBMLDocument::convert(@if java ConversionProperties@endif)
 * with the ConversionProperties object as an argument.
 *
 * @subsection converter-example Example of invoking an SBML converter
 *
 * The following code fragment illustrates an example using
 * SBMLReactionConverter, which is invoked using the option string @c
 * 'replaceReactions':
 *
 * @if cpp
 * @code{.cpp}
ConversionProperties props;
props.addOption('replaceReactions');
@endcode
@endif
@if python
@code{.py}
config = ConversionProperties()
if config != None:
  config.addOption('replaceReactions')
@endcode
@endif
@if java
@code{.java}
ConversionProperties props = new ConversionProperties();
if (props != null) {
  props.addOption('replaceReactions');
} else {
  // Deal with error.
}
@endcode
@endif
 *
 * In the case of SBMLReactionConverter, there are no options to affect
 * its behavior, so the next step is simply to invoke the converter on
 * an SBMLDocument object.  Continuing the example code:
 *
 * @if cpp
 * @code{.cpp}
// Assume that the variable 'document' has been set to an SBMLDocument object.
int status = document->convert(props);
if (status != LIBSBML_OPERATION_SUCCESS)
{
  cerr << 'Unable to perform conversion due to the following:' << endl;
  document->printErrors(cerr);
}
@endcode
@endif
@if python
@code{.py}
  # Assume that the variable 'document' has been set to an SBMLDocument object.
  status = document.convert(config)
  if status != LIBSBML_OPERATION_SUCCESS:
    # Handle error somehow.
    print('Error: conversion failed due to the following:')
    document.printErrors()
@endcode
@endif
@if java
@code{.java}
  // Assume that the variable 'document' has been set to an SBMLDocument object.
  status = document.convert(config);
  if (status != libsbml.LIBSBML_OPERATION_SUCCESS)
  {
    // Handle error somehow.
    System.out.println('Error: conversion failed due to the following:');
    document.printErrors();
  }
@endcode
@endif
 *
 * Here is an example of using a converter that offers an option. The
 * following code invokes SBMLStripPackageConverter to remove the
 * SBML Level&nbsp;3 @em %Layout package from a model.  It sets the name
 * of the package to be removed by adding a value for the option named
 * @c 'package' defined by that converter:
 *
 * @if cpp
 * @code{.cpp}
ConversionProperties props;
props.addOption('stripPackage');
props.addOption('package', 'layout');

int status = document->convert(props);
if (status != LIBSBML_OPERATION_SUCCESS)
{
    cerr << 'Unable to strip the Layout package from the model';
    cerr << 'Error returned: ' << status;
}
@endcode
@endif
@if python
@code{.py}
def strip_layout_example(document):
  config = ConversionProperties()
  if config != None:
    config.addOption('stripPackage')
    config.addOption('package', 'layout')
    status = document.convert(config)
    if status != LIBSBML_OPERATION_SUCCESS:
      # Handle error somehow.
      print('Error: unable to strip the Layout package.')
      print('LibSBML returned error: ' + OperationReturnValue_toString(status).strip())
  else:
    # Handle error somehow.
    print('Error: unable to create ConversionProperties object')
@endcode
@endif
@if java
@code{.java}
ConversionProperties config = new ConversionProperties();
if (config != None) {
  config.addOption('stripPackage');
  config.addOption('package', 'layout');
  status = document.convert(config);
  if (status != LIBSBML_OPERATION_SUCCESS) {
    // Handle error somehow.
    System.out.println('Error: unable to strip the Layout package');
    document.printErrors();
  }
} else {
  // Handle error somehow.
  System.out.println('Error: unable to create ConversionProperties object');
}
@endcode
@endif
 *
 * @subsection available-converters Available SBML converters in libSBML
 *
 * LibSBML provides a number of built-in converters; by convention, their
 * names end in @em Converter. The following are the built-in converters
 * provided by libSBML @htmlinclude libsbml-version.html:
 *
 * @copydetails doc_list_of_libsbml_converters
 *
 *
 */

public class SBMLIdConverter : SBMLConverter {
	private HandleRef swigCPtr;
	
	internal SBMLIdConverter(IntPtr cPtr, bool cMemoryOwn) : base(libsbmlPINVOKE.SBMLIdConverter_SWIGUpcast(cPtr), cMemoryOwn)
	{
		//super(libsbmlPINVOKE.SBMLIdConverterUpcast(cPtr), cMemoryOwn);
		swigCPtr = new HandleRef(this, cPtr);
	}
	
	internal static HandleRef getCPtr(SBMLIdConverter obj)
	{
		return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
	}
	
	internal static HandleRef getCPtrAndDisown (SBMLIdConverter obj)
	{
		HandleRef ptr = new HandleRef(null, IntPtr.Zero);
		
		if (obj != null)
		{
			ptr             = obj.swigCPtr;
			obj.swigCMemOwn = false;
		}
		
		return ptr;
	}

  ~SBMLIdConverter() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          libsbmlPINVOKE.delete_SBMLIdConverter(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  
/** */ /* libsbml-internal */ public
 static void init() {
    libsbmlPINVOKE.SBMLIdConverter_init();
  }

  
/**
   * Creates a new SBMLIdConverter object.
   */ public
 SBMLIdConverter() : this(libsbmlPINVOKE.new_SBMLIdConverter__SWIG_0(), true) {
  }

  
/**
   * Copy constructor; creates a copy of an SBMLIdConverter
   * object.
   *
   * @param obj the SBMLIdConverter object to copy.
   */ public
 SBMLIdConverter(SBMLIdConverter obj) : this(libsbmlPINVOKE.new_SBMLIdConverter__SWIG_1(SBMLIdConverter.getCPtr(obj)), true) {
    if (libsbmlPINVOKE.SWIGPendingException.Pending) throw libsbmlPINVOKE.SWIGPendingException.Retrieve();
  }

  
/**
   * Creates and returns a deep copy of this SBMLIdConverter
   * object.
   *
   * @return a (deep) copy of this converter.
   */ public new
 SBMLConverter clone() {
    IntPtr cPtr = libsbmlPINVOKE.SBMLIdConverter_clone(swigCPtr);
    SBMLIdConverter ret = (cPtr == IntPtr.Zero) ? null : new SBMLIdConverter(cPtr, true);
    return ret;
  }

  
/**
   * Returns @c true if this converter object's properties match the given
   * properties.
   *
   * A typical use of this method involves creating a ConversionProperties
   * object, setting the options desired, and then calling this method on
   * an SBMLIdConverter object to find out if the object's
   * property values match the given ones.  This method is also used by
   * SBMLConverterRegistry::getConverterFor(@if java ConversionProperties@endif)
   * to search across all registered converters for one matching particular
   * properties.
   *
   * @param props the properties to match.
   *
   * @return @c true if this converter's properties match, @c false
   * otherwise.
   */ public new
 bool matchesProperties(ConversionProperties props) {
    bool ret = libsbmlPINVOKE.SBMLIdConverter_matchesProperties(swigCPtr, ConversionProperties.getCPtr(props));
    if (libsbmlPINVOKE.SWIGPendingException.Pending) throw libsbmlPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  
/**
   * Perform the conversion.
   *
   * This method causes the converter to do the actual conversion work,
   * that is, to convert the SBMLDocument object set by
   * SBMLConverter::setDocument(@if java SBMLDocument@endif) and
   * with the configuration options set by
   * SBMLConverter::setProperties(@if java ConversionProperties@endif).
   *
   * *
 * @return integer value indicating success/failure of the
 * function.  @if clike The value is drawn from the
 * enumeration #OperationReturnValues_t. @endif The possible values
 * returned by this function are:
 *
 *
   * @li @link libsbml#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS@endlink
   * @li @link libsbml#LIBSBML_OPERATION_FAILED LIBSBML_OPERATION_FAILED@endlink
   * @li @link libsbml#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT@endlink
   * @li @link libsbml#LIBSBML_UNEXPECTED_ATTRIBUTE LIBSBML_UNEXPECTED_ATTRIBUTE@endlink
   * @li @link libsbml#LIBSBML_INVALID_ATTRIBUTE_VALUE LIBSBML_INVALID_ATTRIBUTE_VALUE@endlink
   */ public new
 int convert() {
    int ret = libsbmlPINVOKE.SBMLIdConverter_convert(swigCPtr);
    return ret;
  }

  
/**
   * Returns the default properties of this converter.
   *
   * A given converter exposes one or more properties that can be adjusted
   * in order to influence the behavior of the converter.  This method
   * returns the @em default property settings for this converter.  It is
   * meant to be called in order to discover all the settings for the
   * converter object.
   *
   * @return the ConversionProperties object describing the default properties
   * for this converter.
   */ public new
 ConversionProperties getDefaultProperties() {
    ConversionProperties ret = new ConversionProperties(libsbmlPINVOKE.SBMLIdConverter_getDefaultProperties(swigCPtr), true);
    return ret;
  }

}

}

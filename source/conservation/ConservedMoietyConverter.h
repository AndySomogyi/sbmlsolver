/*
 * ConservedMoietyConverter.h
 *
 *  Created on: Oct 19, 2013
 *      Author: andy
 */

#ifndef ConservedMoietyConverter_h
#define ConservedMoietyConverter_h

#include "rrExporter.h"

#include <sbml/SBMLNamespaces.h>
#include <sbml/conversion/SBMLConverter.h>
#include <sbml/conversion/SBMLConverterRegister.h>

namespace ls {
    class LibStructural;
}

namespace rr
{
namespace conservation
{

class RR_DECLSPEC ConservedMoietyConverter: public libsbml::SBMLConverter
{
public:

    /**
     * register with the ConversionRegistry
     */
    static void init();

    /**
     * Creates a new SBMLLevelVersionConverter object.
     */
    ConservedMoietyConverter();

    /**
     * Copy constructor; creates a copy of an SBMLLevelVersionConverter
     * object.
     *
     * @param obj the SBMLLevelVersionConverter object to copy.
     */
    ConservedMoietyConverter(const ConservedMoietyConverter& obj);

    /**
     * Creates and returns a deep copy of this SBMLLevelVersionConverter
     * object.
     *
     * @return a (deep) copy of this converter.
     */
    virtual libsbml::SBMLConverter* clone() const;

    /**
     * Returns @c true if this converter object's properties match the given
     * properties.
     *
     * A typical use of this method involves creating a ConversionProperties
     * object, setting the options desired, and then calling this method on
     * an SBMLLevelVersionConverter object to find out if the object's
     * property values match the given ones.  This method is also used by
     * SBMLConverterRegistry::getConverterFor(@if java const ConversionProperties& props@endif)
     * to search across all registered converters for one matching particular
     * properties.
     *
     * @param props the properties to match.
     *
     * @return @c true if this converter's properties match, @c false
     * otherwise.
     */
    virtual bool matchesProperties(
            const libsbml::ConversionProperties &props) const;

    /**
     * Perform the conversion.
     *
     * This method causes the converter to do the actual conversion work,
     * that is, to convert the SBMLDocument object set by
     * SBMLConverter::setDocument(@if java const SBMLDocument* doc@endif) and
     * with the configuration options set by
     * SBMLConverter::setProperties(@if java const ConversionProperties *props@endif).
     *
     * @return  integer value indicating the success/failure of the operation.
     * @if clike The value is drawn from the enumeration
     * #OperationReturnValues_t. @endif@~ The possible values are:
     * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS @endlink
     * @li @link OperationReturnValues_t#LIBSBML_INVALID_OBJECT LIBSBML_INVALID_OBJECT @endlink
     * @li @link OperationReturnValues_t#LIBSBML_CONV_INVALID_SRC_DOCUMENT LIBSBML_CONV_INVALID_SRC_DOCUMENT @endlink
     */
    virtual int convert();

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
     */
    virtual libsbml::ConversionProperties getDefaultProperties() const;

    /**
     * Returns the SBML document that is the subject of the conversions.
     *
     * @return the current SBMLDocument object.
     */
    virtual libsbml::SBMLDocument* getDocument();

    /**
     * Returns the SBML document that is the subject of the conversions.
     *
     * @return the current SBMLDocument object, as a const reference.
     */
    virtual const libsbml::SBMLDocument* getDocument() const;


    /**
     * this converter first need to convert the doc to a l3v1 document,
     * this returns the leve / version updated doc.
     */
    const libsbml::SBMLDocument *getLevelConvertedDocument() const;

    /**
     * Sets the current SBML document to the given SBMLDocument object.
     *
     * @param doc the document to use for this conversion.
     *
     * @return integer value indicating the success/failure of the operation.
     * @if clike The value is drawn from the enumeration
     * #OperationReturnValues_t. @endif@~ The set of possible values that may
     * be returned ultimately depends on the specific subclass of
     * SBMLConverter being used, but the default method can return the
     * following values:
     * @li @link OperationReturnValues_t#LIBSBML_OPERATION_SUCCESS LIBSBML_OPERATION_SUCCESS @endlink
     */
    virtual int setDocument(const libsbml::SBMLDocument* doc);

private:

    /**
     * class used to calculate the L0 matrix.
     *
     * Will be replaced with a sparse version in next RoadRunner release.
     */
    ls::LibStructural *structural;

    /**
     * base class has an mDocument field, use this for the src doc
     */

    /**
     * source model
     */
    libsbml::Model *mModel;

    libsbml::SBMLDocument *resultDoc;

    libsbml::Model *resultModel;
};

} } // namespace rr } namespace conservation }
#endif  /* ConservedMoietyConverter_h */


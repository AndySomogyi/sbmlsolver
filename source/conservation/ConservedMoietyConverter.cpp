/*
 * ConservedMoietyConverter.cpp
 *
 *  Created on: Oct 19, 2013
 *      Author: andy
 */

#include "ConservedMoietyConverter.h"
#include "rr-libstruct/lsLibStructural.h"

#include <sbml/conversion/SBMLConverterRegistry.h>
#include <sbml/conversion/SBMLLevelVersionConverter.h>
#include "ConservationExtension.h"
#include "rrLogger.h"


#include <sbml/math/ASTNode.h>
#include <sbml/AlgebraicRule.h>
#include <sbml/AssignmentRule.h>
#include <sbml/RateRule.h>
#include <sbml/InitialAssignment.h>
#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>

#include <algorithm>
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace libsbml;

namespace rr { namespace conservation {


void ConservedMoietyConverter::init()
{
    ConservedMoietyConverter converter;
    SBMLConverterRegistry::getInstance().addConverter(&converter);
}

ConservedMoietyConverter::ConservedMoietyConverter() :
        SBMLConverter(),
        mModel(0),
        structural(0),
        resultDoc(0),
        resultModel(0)
{
}

ConservedMoietyConverter::ConservedMoietyConverter(
        const ConservedMoietyConverter& orig) :
        SBMLConverter(orig),
        mModel(0),
        structural(0),
        resultDoc(0),
        resultModel(0)
{
}

SBMLConverter* ConservedMoietyConverter::clone() const
{
    return new ConservedMoietyConverter(*this);
}

ConversionProperties ConservedMoietyConverter::getDefaultProperties() const
{
    static ConversionProperties prop;
    prop.addOption("sortRules", true,
            "Sort AssignmentRules and InitialAssignments in the model");
    return prop;
}

bool ConservedMoietyConverter::matchesProperties(
        const ConversionProperties &props) const
{
    if (&props == NULL || !props.hasOption("sortRules"))
        return false;
    return true;
}

int ConservedMoietyConverter::convert()
{
    if (mDocument == NULL)
        return LIBSBML_INVALID_OBJECT;

    Model* mModel = mDocument->getModel();
    if (mModel == NULL)
        return LIBSBML_INVALID_OBJECT;


    /* check consistency of model */
    /* since this function will write to the error log we should
     * clear anything in the log first
     */
    mDocument->getErrorLog()->clearLog();
    unsigned char origValidators = mDocument->getApplicableValidators();

    mDocument->setApplicableValidators(AllChecksON);

    mDocument->checkConsistency();

    /* replace original consistency checks */
    mDocument->setApplicableValidators(origValidators);

    if (mDocument->getErrorLog()->getNumFailsWithSeverity(LIBSBML_SEV_ERROR)
            != 0)
    {
        return LIBSBML_CONV_INVALID_SRC_DOCUMENT;
    }

    resultDoc = new SBMLDocument(new ConservationPkgNamespaces(3,1,1));

    // makes a clone of the model
    resultDoc->setModel(mModel);

    resultModel = resultDoc->getModel();

    // when a model is set to a doc, the doc does not replace the namespace,
    // so we do that here so when we allocate new parameters and
    // species, they will have the ConservedMoiety plugin.
    //resultModel->setSBMLNamespacesAndOwn(new ConservationPkgNamespaces(3,1,1));







    return LIBSBML_OPERATION_SUCCESS;

}

libsbml::SBMLDocument* ConservedMoietyConverter::getDocument()
{
    return resultDoc;
}

const libsbml::SBMLDocument* ConservedMoietyConverter::getDocument() const
{
    return resultDoc;
}

int ConservedMoietyConverter::setDocument(const libsbml::SBMLDocument* doc)
{
    // TODO clear and delete all memeber vars

    int result = 0;

    if (doc == 0)
    {
        return LIBSBML_INVALID_OBJECT;
    }

    if (doc->getLevel() != ConservationExtension::getDefaultLevel()
        || doc->getVersion() != ConservationExtension::getDefaultVersion())
    {
        Log(rr::Logger::PRIO_NOTICE) << "source document is level " << doc->getLevel()
                << ", version " << doc->getVersion() << ", converting to"
                << "level " << ConservationExtension::getDefaultLevel()
                << ", version " << ConservationExtension::getDefaultVersion()
                << " for Moiety Conservation Conversion";

        // this does an in-place conversion, at least for the time being
        SBMLLevelVersionConverter versionConverter;

        ConversionProperties versionProps = versionConverter.getDefaultProperties();

        versionConverter.setProperties(&versionProps);

        versionConverter.setDocument(doc);

        if ((result = versionConverter.convert()) != LIBSBML_OPERATION_SUCCESS)
        {
            Log(rr::Logger::PRIO_ERROR) < "could not upgrade source sbml level or version";

            const SBMLErrorLog *log = doc->getErrorLog();
            string errors = log ? log->toString() : " NULL SBML Error Log";
            Log(rr::Logger::PRIO_ERROR) << "Conversion Errors: " + errors;


            return result;
        }

        // TODO, in case behavior of version converter changes,
        // need to update ownership logic.
        doc = versionConverter.getDocument();
    }

    if ((result = SBMLConverter::setDocument(doc)) != LIBSBML_OPERATION_SUCCESS)
    {
        return result;
    }

    // owned by the doc
    mModel = mDocument->getModel();
    if (mModel == NULL)
    {
        return LIBSBML_INVALID_OBJECT;
    }

    structural = new ls::LibStructural(mModel);

    return LIBSBML_OPERATION_SUCCESS;
}



}
} // namespace rr } namespace conservation }


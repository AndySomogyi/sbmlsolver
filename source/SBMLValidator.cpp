/*
 * SBMLValidator.cpp
 *
 *  Created on: Aug 24, 2014
 *      Author: andy
 */

#include "SBMLValidator.h"
#include "rrSBMLReader.h"

#include <sbml/SBMLDocument.h>

#include "rrLogger.h"

#include <sstream>

using namespace libsbml;

using std::string;

namespace rr
{

std::string validateSBML(const std::string src, unsigned opt)
{
    std::stringstream errors;
    SBMLDocument *doc = NULL;

    try {
        string sbml = SBMLReader::read(src);
        doc =  readSBMLFromString (sbml.c_str());

        doc->setConsistencyChecks(LIBSBML_CAT_GENERAL_CONSISTENCY,
                opt & VALIDATE_GENERAL);

        doc->setConsistencyChecks(LIBSBML_CAT_UNITS_CONSISTENCY,
                opt & VALIDATE_UNITS);

        doc->setConsistencyChecks(LIBSBML_CAT_IDENTIFIER_CONSISTENCY,
                opt & VALIDATE_IDENTIFIER);

        doc->setConsistencyChecks(LIBSBML_CAT_MATHML_CONSISTENCY,
                opt & VALIDATE_MATHML);

        doc->setConsistencyChecks(LIBSBML_CAT_OVERDETERMINED_MODEL,
                opt & VALIDATE_OVERDETERMINED);

        doc->setConsistencyChecks(LIBSBML_CAT_MODELING_PRACTICE,
                opt & VALIDATE_MODELING_PRACTICE);

        doc->checkConsistency();

        SBMLErrorLog* errorLog = doc->getErrorLog();

        if(errorLog) {
            errorLog->printErrors(errors);
        }

    } catch(...) {
        delete doc;
        throw;
    }

    delete doc;
    return errors.str();
}

// Return true if stoichiometry is defined for this species reference
static bool isStoichDefined(const SpeciesReference* s) {
    if (!s)
        return false;
    return s->isSetStoichiometry();
}

// Return true if stoichiometry is defined for every reaction in the model
bool isStoichDefined(const std::string sbml) {
    SBMLDocument *doc = NULL;

    try {
        doc =  readSBMLFromString (sbml.c_str());

        const Model *m = doc->getModel();

        for (int j = 0; j<m->getNumReactions(); ++j) {
            const Reaction* r = m->getReaction(j);
            if (!r)
                throw std::runtime_error("No reaction");

            // check stoich defined on reactants / products
            for (int k = 0; k<r->getNumReactants(); ++k) {
                if (!isStoichDefined(r->getReactant(k)))
                    return false;
            }

            for (int k = 0; k<r->getNumProducts(); ++k) {
                if (!isStoichDefined(r->getProduct(k)))
                    return false;
            }

            // modifiers have no stoichiometry
        }

    } catch(...) {
        delete doc;
        throw;
    }

    delete doc;
    return true;
}

std::string fixMissingStoich(const std::string sbml) {
    SBMLDocument *doc = NULL;

    try {
        doc =  readSBMLFromString (sbml.c_str());

        Model *m = doc->getModel();

        for (int j = 0; j<m->getNumReactions(); ++j) {
            Reaction* r = m->getReaction(j);
            if (!r)
                throw std::runtime_error("No reaction");

            // check stoich defined on reactants / products
            for (int k = 0; k<r->getNumReactants(); ++k) {
                SpeciesReference* s = r->getReactant(k);
                if (!isStoichDefined(s))
                    if (s->setStoichiometry(1.) != LIBSBML_OPERATION_SUCCESS)
                        throw std::runtime_error("Unable to set stoichiometry");
            }

            for (int k = 0; k<r->getNumProducts(); ++k) {
                SpeciesReference* s = r->getProduct(k);
                if (!isStoichDefined(s))
                    if (s->setStoichiometry(1.) != LIBSBML_OPERATION_SUCCESS)
                        throw std::runtime_error("Unable to set stoichiometry");
            }

            // modifiers have no stoichiometry
        }

    } catch(...) {
        delete doc;
        throw;
    }

    SBMLWriter writer;

    char* sbml_cstr = writer.writeSBMLToString(doc);
    delete doc;
    std::string result(sbml_cstr);
    free(sbml_cstr);
    return result;
}

} /* namespace rr */

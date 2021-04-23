/*
 * SBMLValidator.cpp
 *
 *  Created on: Aug 24, 2014
 *      Author: andy
 */

#include "SBMLValidator.h"
#include "rrSBMLReader.h"

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
        std::string sbml = SBMLReader::read(src);
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

// Return true if this species reference has an initial assignment
static bool hasInitialAssignment(const SpeciesReference* s) {
    const Model* m = s->getModel();
    const ListOfInitialAssignments *assn = m->getListOfInitialAssignments();

    for (unsigned i = 0; i < assn->size(); ++i) {
        const InitialAssignment *a = assn->get(i);
        if (a->getSymbol() == s->getId())
            return true;
    }

    return false;
}

// Return true if this species reference has an assignment rule
static bool hasAssignmentRule(const SpeciesReference* s) {
    const Model* m = s->getModel();
    const ListOfRules* rules = m->getListOfRules();

    for (unsigned i = 0; i < rules->size(); ++i) {
        const Rule *rule = rules->get(i);
        if (const AssignmentRule* a = dynamic_cast<const AssignmentRule*>(rule))
            if (a->getVariable() ==  s->getId())
                return true;
    }

    return false;
}

// Return true if stoichiometry is defined for this species reference
static bool isStoichDefined(const SpeciesReference* s) {
    if (!s)
        // null ref
        return false;
    if (hasInitialAssignment(s) || hasAssignmentRule(s))
        // stoich can be set by assignments
        return true;
    return (s->isSetStoichiometry() || s->isSetStoichiometryMath());
}

void setSpeciesRefId(SpeciesReference* sr, const std::string& idbase)
{
    Model* model = const_cast<Model*>(sr->getModel());
    const SBase* sbase = model->getElementBySId(idbase);
    int i = 0;
    std::stringstream newid(idbase);
    while (sbase != NULL) {
        i++;
        newid.clear();
        newid << idbase << i;
        sbase = model->getElementBySId(newid.str());
    }
    int setret = sr->setId(newid.str());
    if (setret != LIBSBML_OPERATION_SUCCESS && sr->getLevel() == 2 && sr->getVersion() == 1) {
        // Might have to move to l2v2 to get ids on speciesReferences.
        //  They don't officially exist in l2v1, but setId usually works
        //  anyway because there's special layout code to handle it.
        //
        // If we do update the doc l/v, the objects remain the same, so it's OK
        //  to continue in the same loops.
        SBMLDocument* doc = model->getSBMLDocument();
        doc->setLevelAndVersion(2, 2, false);
        setret = sr->setId(newid.str());
    }
    if (setret != LIBSBML_OPERATION_SUCCESS) {
        throw std::runtime_error("Unable to set variable stoichiometry ID.");
    }
}

std::string fixMissingStoich(const std::string sbml) {
    SBMLDocument *doc = NULL;

    bool sbml_decl_okay = false;

    // check for <?xml
    size_t pos = sbml.find("<");
    if (pos != std::string::npos) {
      pos = sbml.find("?", pos+1);
      if (pos != std::string::npos) {
        pos = sbml.find("xml", pos+1);
        if (pos != std::string::npos)
          sbml_decl_okay = true;
      }
    }

    // check for <sbml
    pos = sbml.find("<");
    if (pos != std::string::npos) {
      pos = sbml.find("sbml", pos+1);
      if (pos != std::string::npos)
        sbml_decl_okay = true;
    }

    if (!sbml_decl_okay)
      throw std::runtime_error("SBML document must begin with an XML declaration or an SBML declaration");


    try {
        doc =  readSBMLFromString (sbml.c_str());

        Model *m = doc->getModel();

        for (unsigned int j = 0; j<m->getNumReactions(); ++j) {
            Reaction* r = m->getReaction(j);
            if (!r)
                throw std::runtime_error("No reaction");

            // check stoich defined on reactants / products
            for (unsigned int k = 0; k<r->getNumReactants(); ++k) {
                SpeciesReference* s = r->getReactant(k);
                if (!isStoichDefined(s))
                    if (s->setStoichiometry(1.) != LIBSBML_OPERATION_SUCCESS) {
                        throw std::runtime_error("Unable to set stoichiometry");
                    }
                if (s->isSetStoichiometryMath()) {
                    std::string id = s->getId();
                    if (!s->isSetId()) {
                        setSpeciesRefId(s, r->getId() + "_reactant_" + s->getSpecies() + "_stoichiometry");
                    }
                }
            }

            for (unsigned int k = 0; k<r->getNumProducts(); ++k) {
                SpeciesReference* s = r->getProduct(k);
                if (!isStoichDefined(s))
                    if (s->setStoichiometry(1.) != LIBSBML_OPERATION_SUCCESS) {
                        throw std::runtime_error("Unable to set stoichiometry");
                    }
                if (s->isSetStoichiometryMath()) {
                    std::string id = s->getId();
                    if (!s->isSetId()) {
                        setSpeciesRefId(s, r->getId() + "_product_" + s->getSpecies() + "_stoichiometry");
                    }
                }
            }

            // modifiers have no stoichiometry
        }

    } catch(...) {
        delete doc;
        throw;
    }

    std::string result = writeSBMLToStdString(doc);
    delete doc;
    return result;
}

} /* namespace rr */

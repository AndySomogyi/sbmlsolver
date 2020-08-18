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

        for (int j = 0; j<m->getNumReactions(); ++j) {
            Reaction* r = m->getReaction(j);
            if (!r)
                throw std::runtime_error("No reaction");

            // check stoich defined on reactants / products
            for (int k = 0; k<r->getNumReactants(); ++k) {
                SpeciesReference* s = r->getReactant(k);
                if (!isStoichDefined(s))
                    if (s->setStoichiometry(1.) != LIBSBML_OPERATION_SUCCESS) {
                        throw std::runtime_error("Unable to set stoichiometry");
                    }
                if (s->isSetStoichiometryMath()) {
                    string id = s->getId();
                    if (!s->isSetId()) {
                        int idset = s->setId(r->getId() + "_reactant_" + s->getSpecies() + "_stoichiometry");
                        if (idset != LIBSBML_OPERATION_SUCCESS) {
                            if (s->getLevel() == 2 && s->getVersion() == 1) {
                                //Have to move to l2v2 to get ids on speciesReferences.
                                //However, this code doesn't get tested because the
                                // speciesReference is given an ID anyway in l2v1 because
                                // of the layout extension, somehow.
                                doc->setLevelAndVersion(2, 2, false);
                                string newversion = writeSBMLToStdString(doc);
                                delete doc;
                                return fixMissingStoich(newversion);
                            }
                        }
                    }
                }
            }

            for (int k = 0; k<r->getNumProducts(); ++k) {
                SpeciesReference* s = r->getProduct(k);
                if (!isStoichDefined(s))
                    if (s->setStoichiometry(1.) != LIBSBML_OPERATION_SUCCESS) {
                        throw std::runtime_error("Unable to set stoichiometry");
                    }
                if (s->isSetStoichiometryMath()) {
                    string id = s->getId();
                    if (!s->isSetId()) {
                        if (s->setId(r->getId() + "_product_" + s->getSpecies() + "_stoichiometry") != LIBSBML_OPERATION_SUCCESS) {
                            if (s->getLevel() == 2 && s->getVersion() == 1) {
                                //Have to move to l2v2 to get ids on speciesReferences.
                                doc->setLevelAndVersion(2, 2, false);
                                string newversion = writeSBMLToStdString(doc);
                                delete doc;
                                return fixMissingStoich(newversion);
                            }
                            throw std::runtime_error("Unable to set variable stoichiometry ID.");
                        }
                    }
                }
            }

            // modifiers have no stoichiometry
        }

    } catch(...) {
        delete doc;
        throw;
    }

    string result = writeSBMLToStdString(doc);
    delete doc;
    return result;
}

} /* namespace rr */

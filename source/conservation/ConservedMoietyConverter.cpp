/*
 * ConservedMoietyConverter.cpp
 *
 *  Created on: Oct 19, 2013
 *      Author: andy
 */

#include "ConservedMoietyConverter.h"
#include "ConservedMoietyPlugin.h"
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
#include <set>

#include "rrStringUtils.h"


#include <Poco/UUIDGenerator.h>


using namespace std;
using namespace libsbml;

static const int loggingLevel = rr::Logger::LOG_DEBUG;

namespace rr { namespace conservation {

/**
 * A little hack so we can copy species and get the ConservedMoiety plugin.
 *
 * Otherwise, we would have had to essentially re-write the Species
 * copy ctor, which is problematic should the contents of Species
 * ever change.
 */
class ConservedMoietySpecies : public libsbml::Species
{
public:
    ConservedMoietySpecies(libsbml::Species& orig, bool conservedMoiety) :
        libsbml::Species(orig)
    {
        ConservationPkgNamespaces ns(3,1,1);
        this->loadPlugins(&ns);

        ConservedMoietyPlugin *plugin = (ConservedMoietyPlugin*)getPlugin("conservation");

        assert(plugin && "could not get conservation plugin from new species");

        plugin->setConservedMoiety(conservedMoiety);
    }
};


typedef std::vector<std::string> StringVector;

/**
 * remove any species from new model and replace with species
 * in
 */
static void createReorderedSpecies(Model* newModel, Model* oldModel,
        const std::vector<std::string>& indSpecies,
        const std::vector<std::string>& depSpecies);

/**
 * creates a set of global parameters for the conserved moieties.
 *
 * A set of initialAssigments are also created to set thier values.
 */
static std::vector<std::string> createConservedMoietyParameters(
        Model* newModel,
        const ls::DoubleMatrix& L0, const std::vector<std::string>& indSpecies,
        const std::vector<std::string>& depSpecies);


static void createDependentSpeciesRules(Model* newModel,
        const ls::DoubleMatrix& L0,
        const std::vector<std::string>& conservedMoieties,
        const std::vector<std::string>& indSpecies,
        const std::vector<std::string>& depSpecies);

static void updateReactions(Model* newModel,
        const std::vector<std::string>& indSpecies,
        const std::vector<std::string>& depSpecies);

static void insertAsModifier(Reaction* reaction, SimpleSpeciesReference *s);

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

    vector<string> indSpecies = structural->getIndependentSpecies();

    vector<string> depSpecies = structural->getDependentSpecies();

    ls::DoubleMatrix *L0 = structural->getL0Matrix();

    if (rr::Logger::getLevel() >= loggingLevel)
    {
        Log(loggingLevel) << "performing conversion on " << mModel->getName();
        Log(loggingLevel) << "independent species: " << toString(indSpecies);
        Log(loggingLevel) << "dependent species: " << toString(depSpecies);
        Log(loggingLevel) << "L0 matrix: " << endl << *L0;
        Log(loggingLevel) << "Stoichiometry Matrix: " << endl
                << *(structural->getStoichiometryMatrix());
        Log(loggingLevel) << "Reordered Stoichiometry Matrix: "
                << endl << *(structural->getReorderedStoichiometryMatrix());
    }

    createReorderedSpecies(resultModel, mModel, indSpecies, depSpecies);

    vector<string> conservedMoieties = createConservedMoietyParameters(
            resultModel, *L0, indSpecies, depSpecies);

    createDependentSpeciesRules(resultModel, *L0, conservedMoieties,
            indSpecies, depSpecies);

    updateReactions(resultModel, indSpecies, depSpecies);

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
        Log(rr::Logger::LOG_NOTICE) << "source document is level " << doc->getLevel()
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
            Log(rr::Logger::LOG_ERROR) < "could not upgrade source sbml level or version";

            const SBMLErrorLog *log = doc->getErrorLog();
            string errors = log ? log->toString() : string(" NULL SBML Error Log");
            Log(rr::Logger::LOG_ERROR) << "Conversion Errors: " + errors;


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

const libsbml::SBMLDocument *ConservedMoietyConverter::getLevelConvertedDocument() const
{
    return mDocument;
}

static void createReorderedSpecies(Model* newModel, Model* oldModel,
        const std::vector<std::string>& indSpecies,
        const std::vector<std::string>& depSpecies)
{
    // remove all the existing independent species
    ListOfSpecies *species = newModel->getListOfSpecies();

    unsigned index = 0;

    while(index < species->size())
    {
        Species *s = species->get(index);
        if (!s->getBoundaryCondition())
        {
            species->remove(index);
            delete s;
        }
        else
        {
            index++;
        }
    }

#if DEBUG
    // TODO make sure that remaining species are in the given list.
#endif


    species = oldModel->getListOfSpecies();
    ListOfSpecies *newSpecies = newModel->getListOfSpecies();

    // insert independent first, then dependent
    index = 0;
    for (int i = 0; i < indSpecies.size(); ++i)
    {
        Species *s = species->get(indSpecies[i]);

        assert(s && "could not get independent species from original model");

        newSpecies->insertAndOwn(index++, new ConservedMoietySpecies(*s, false));
    }

    for (int i = 0; i < depSpecies.size(); ++i)
    {
        Species *s = species->get(depSpecies[i]);

        assert(s && "could not get dependent species from original model");

        newSpecies->insertAndOwn(index++, new ConservedMoietySpecies(*s, true));
    }
}

static std::vector<std::string> createConservedMoietyParameters(
        Model* newModel,
        const ls::DoubleMatrix& L0, const std::vector<std::string>& indSpecies,
        const std::vector<std::string>& depSpecies)
{
    StringVector conservedMoieties(indSpecies.size());

    Poco::UUIDGenerator uuidGen;

    for (int i = 0; i < depSpecies.size(); ++i)
    {
        Poco::UUID uuid = uuidGen.create();
        string id = "cm_" + rr::toString(i) + "_" + uuid.toString();
        std::replace( id.begin(), id.end(), '-', '_');

        Parameter *cm = newModel->createParameter();
        cm->setId(id);
        cm->setConstant(true);

        ConservedMoietyPlugin *plugin = dynamic_cast<ConservedMoietyPlugin*>(
                cm->getPlugin("conservation"));
        plugin->setConservedMoiety(true);

        conservedMoieties[i] = id;

        InitialAssignment *ia = newModel->createInitialAssignment();
        ia->setSymbol(id);

        ASTNode sum(AST_PLUS);

        for (int j = 0; j < indSpecies.size(); ++j)
        {
            double stoich = L0(i, j);

            if (stoich != 0)
            {
                ASTNode *times = new ASTNode(AST_TIMES);
                ASTNode *value = new ASTNode(AST_REAL);
                ASTNode *name = new ASTNode(AST_NAME);

                value->setValue(stoich);
                name->setName(indSpecies[j].c_str());

                times->addChild(value);
                times->addChild(name);

                sum.addChild(times);
            }
        }

        ia->setMath(&sum);
    }

    return conservedMoieties;
}

static void createDependentSpeciesRules(Model* newModel,
        const ls::DoubleMatrix& L0,
        const std::vector<std::string>& conservedMoieties,
        const std::vector<std::string>& indSpecies,
        const std::vector<std::string>& depSpecies)
{
    for (unsigned i = 0; i < depSpecies.size(); ++i)
    {
        const string& id = depSpecies[i];


        AssignmentRule *rule = newModel->createAssignmentRule();
        rule->setVariable(id);

        ASTNode eqn(AST_PLUS);

        ASTNode *t = new ASTNode(AST_NAME);
        t->setName(conservedMoieties[i].c_str());

        eqn.addChild(t);

        for (int j = 0; j < indSpecies.size(); ++j)
        {
            double stoich = L0(i, j);

            if (stoich != 0)
            {
                ASTNode *times = new ASTNode(AST_TIMES);
                ASTNode *value = new ASTNode(AST_REAL);
                ASTNode *name = new ASTNode(AST_NAME);

                value->setValue(stoich);
                name->setName(indSpecies[j].c_str());

                times->addChild(value);
                times->addChild(name);

                eqn.addChild(times);
            }
        }

        rule->setMath(&eqn);
    }
}

static void updateReactions(Model* newModel,
        const std::vector<std::string>& indSpecies,
        const std::vector<std::string>& depSpecies)
{
    set<string> depSet(depSpecies.begin(), depSpecies.end());

    ListOfReactions *reactions = newModel->getListOfReactions();

    for (unsigned i = 0; i < reactions->size(); ++i)
    {
        Reaction *r = reactions->get(i);
        ListOfSpeciesReferences *products = r->getListOfProducts();
        ListOfSpeciesReferences *reactants = r->getListOfReactants();

        unsigned j = 0;
        while(j < products->size())
        {
            SimpleSpeciesReference *product = products->get(j);

            if (depSet.find(product->getSpecies()) != depSet.end())
            {
                products->remove(j);
                insertAsModifier(r, product);
                delete product;
            }
            else
            {
                j++;
            }
        }

        j = 0;
        while(j < reactants->size())
        {
            SimpleSpeciesReference *reactant = reactants->get(j);

            if (depSet.find(reactant->getSpecies()) != depSet.end())
            {
                reactants->remove(j);
                insertAsModifier(r, reactant);
                delete reactant;
            }
            else
            {
                j++;
            }
        }
    }
}

static void insertAsModifier(Reaction* reaction, SimpleSpeciesReference *s)
{
    ModifierSpeciesReference *m = reaction->createModifier();
    m->setId(s->getId());
    m->setSpecies(s->getSpecies());
    m->setName(s->getName());
}



}
} // namespace rr } namespace conservation }


#ifndef rrNOMSupportH
#define rrNOMSupportH
//---------------------------------------------------------------------------
#include <list>
#include <vector>
#include <deque>
#include <string>
#include <stack>
#include <tr1proxy/rr_memory.h>
#include "sbml/math/ASTNode.h"
#include "rrExporter.h"
#include "rrStringListContainer.h"
#include "rrHashTable.h"
//---------------------------------------------------------------------------

namespace rr
{
using std::vector;
using std::string;
using std::pair;
using std::list;
using std::stack;
using std::deque;

// You HAVE to define LIBSBML_USE_CPP_NAMESPACE for your project to compile this!
using libsbml::Model;
using libsbml::SBMLDocument;
using libsbml::ASTNode;
using libsbml::SBase;
using libsbml::Rule;
using libsbml::UnitDefinition;
using libsbml::KineticLaw;

/**
 * Methods to query various information from an SBML document.
 *
 * This class provides a set of static method to query various items
 * from sbml objects.
 */
class RR_DECLSPEC NOMSupport
{
public:
    NOMSupport();
    ~NOMSupport();
    Model*                  getModel();
    SBMLDocument*           getSBMLDocument();
    static std::string           getlibSBMLVersion();

    StringListContainer     getDerivedUnitDefinition(const std::string& sId);
    StringListContainer     getListOfBoundarySpeciesIds();
    StringListContainer     getListOfErrors();
    StringListContainer     getListOfFloatingSpecies();
    StringListContainer     getListOfFloatingSpeciesIds();
    StringListContainer     getListOfParameters();
    StringListContainer     getNthError(const int& nIndex);
    StringListContainer     getNthEvent(const int& arg);
    StringListContainer     getNthFunctionDefinition(const int& arg);
    StringListContainer     getNthListOfModifiers(const int& nIndex);
    StringListContainer     getNthListOfProducts(const int& nIndex);
    StringListContainer     getNthListOfReactants(const int& nIndex);
    ASTNode                 replaceSymbol(ASTNode* node, const std::string& oldId, const std::string& newId);
    bool                    exists(const std::string& sId);
    bool                    getNthParameterHasValue(const int& nReactionIndex, const int& nParameterIndex);
    bool                    getNthUseValuesFromTriggerTime(const int& arg);
    bool                    hasInitialAmount(const std::string& sId);
    bool                    hasInitialConcentration(const std::string& sId);
    bool                    hasSBOTerm(const std::string& sId);
    bool                    hasValue(const std::string& sId);

    /**
     * Checks whether the element for the given SBML id is a boundary species
     *
     * @ param name the id to check
     */
    bool                    isBoundary(const std::string& sId);

    /**
     * Checks whether the element for the given SBML id is a compartment
     *
     * @param sId the id to check
     * @returns true if element is a compartment, false otherwise
     */
    bool                    isCompartment(const std::string& sId);
    bool                    isConstantImpl(const std::string& sId);
    bool                    isFloating(const std::string& sId);
    bool                    isReactionReversible(const int& nIndex);
    bool                    isSpecies (const std::string& sId);
    bool                    multiplyCompartment(const std::string& sbmlId, std::string& compartmentId);
    bool                    needEmptySetNode(Model model);
    bool                    needSinkNode(Model model);
    bool                    needSourceNode(Model model);
    double                  getNthParameterValue(const int& nReactionIndex, const int& nParameterIndex);
    double                  getNthProductStoichiometryDouble(const int& nIndex, const int& nProduct);
    double                  getNthReactantStoichiometryDouble(const int& nIndex, const int& nReactant);
    double                  getValue(const std::string& sId);
    int                     checkConsistency();
    int                     getNthProductStoichiometry(const int& nIndex, const int& nProduct);
    int                     getNthReactantStoichiometry(const int& nIndex, const int& nReactant);
    int                     getNumBoundarySpecies();
    int                     getNumCompartments();
    int                     getNumConstraints();
    int                     getNumErrors();
    int                     getNumEvents();
    int                     getNumFloatingSpecies();
    int                     getNumFunctionDefinitions();
    int                     getNumGlobalParameters();
    int                     getNumInitialAssignments();

    /**
     * @return the number of parameters for the i'th reaction, if reaction is
     * not a kinetic law, 0 otherwise.
     */
    int                     getNumParameters(int reaction);

    int                     getNumProducts(const int& var0);
    int                     getNumReactants(const int& var0);

    /**
     * @return the number of Reactions in this Model.
     */
    int                     getNumReactions();

    int                     getNumRules();
    int                     getSBOTerm(const std::string& sId);
    std::pair<std::string, std::string>    getNthInitialAssignmentPair(const int& nIndex);
    SBase*                  getElement(const std::string& sId);

    std::string                  addEmptySetNode(const std::string& sbml);
    std::string                  addEmptySetNodes(const std::string& sbml);
    std::string                  addMissingModifiers(const std::string& sModel);
    std::string                  addSourceSinkNodes(const std::string& sbml);
    std::string                  convertLevel1ToLevel2Impl(const std::string& sSBML);
    std::string                  convertLevel2ToLevel1Impl(const std::string& sSBML);

    std::string                  convertPowImpl(const std::string& sSBML);
    std::string                  convertSBML(const std::string& sModel, const int& nLevel, const int& nVersion);
    std::string                  convertSBML(const std::string& sModel, const int& nLevel, const int& nVersion, bool throwError);
    std::string                  convertTimeToCSymbol(const std::string& sArg, const std::string& sTimeSymbol);
    std::string                  fixCommonIssues(const std::string& sbml);
    std::string                  fixCommonIssues(const std::string& sbml, const std::string& programName, const std::string& programVersion);
    std::string                  fetAnnotatedModel(const std::string& targetSBML, const std::string& sourceSBML, bool checkModelId);

    std::string                  getAnnotation(const std::string& sId);
    std::string                  getCompartmentIdBySpeciesId(const std::string& sId);
    /**
     * gets the kinetic law math formula for the i'th reaction.
     * This will be changed to return a KineticLaw object soon.
     */
    std::string                  getKineticLaw(const int& index);
    std::string                  getMetaId(const std::string& sId);
    std::string                  getModelId();
    std::string                  getModelName() const;
    std::string                  getNotes(const std::string& sId);
    std::string                  getNthBoundarySpeciesId(const int& nIndex);
    std::string                  getNthBoundarySpeciesName(const int& nIndex);
    std::string                  getNthCompartmentId(const int& nIndex);
    std::string                  getNthCompartmentName(const int& nIndex);
    std::string                  getNthConstraint(const int& nIndex, std::string& sMessage);
    std::string                  getNthFloatingSpeciesId(const int& nIndex);
    std::string                  getNthFloatingSpeciesName(const int& nIndex);
    std::string                  getNthGlobalParameterId(const int& nIndex);
    std::string                  getNthGlobalParameterName(const int& nIndex);
    std::string                  getNthInitialAssignment(const int& nIndex);
    std::string                  getNthParameterId(const int& nReactionIndex, const int& nParameterIndex);
    std::string                  getNthParameterName(const int& nReactionIndex, const int& nParameterIndex);
    std::string                  getNthProductName(const int& nIndex, const int& nProduct);
    std::string                  getNthReactantName(const int& nIndex, const int& nReactant);
    std::string                  getNthReactionId(const int& nIndex);
    std::string                  getNthReactionName(const int& nIndex);
    std::string                  getNthRule(const int& nIndex);
    std::string                  getNthRuleType(const int& arg);
    std::string                  getOutsideCompartment(const std::string& var0);

    std::string                  getSBML();
    std::string                  getSBOCapableSBML(const std::string& sModel);
    std::string                  removeJD1Layout(const std::string& sSBML);
    std::string                  removeJD2Layout(const std::string& sSBML);
    std::string                  removeLayoutInformation(const std::string& sSBML);
    std::string                  validateSBML(const std::string& sModel);
    std::string                  validateWithConsistency(const std::string& sModel);
    std::vector<std::string>          getBuiltinFunctionInfo(const std::string& var0);
    std::vector<std::string>          getBuiltinFunctions();
    void                    changeConstantForRules(Model* model);

    void                    loadFromFile(const std::string& fileName);
    void                    loadParameterPromotedSBML(const std::string& var0, const std::string& sTimeSymbol);
    void                    loadSBML(const std::string& var0);
    void                    loadSBML(const std::string& var0, const std::string& sTimeSymbol);

    void                    setAnnotation(const std::string& sId, const std::string& sAnnotation);
    void                    setModelId(const std::string& sId);
    void                    setNotes(const std::string& sId, const std::string& sNotes);
    void                    setSBOTerm(const std::string& sId, const int& nSBOTerm);
    void                    setValue(const std::string& sId, const double& dValue);
    void                    setValue(Model* model, const std::string& id, const double& value, const bool& throwIfNotFound);
    void                    testASTTime();

    std::string                  getNthBoundarySpeciesCompartmentName(const int& nIndex);

    /**
     * gets the compartment name of the i'th species with boundary condition's
     * compartment name. So, say we have a model like:
     * model {
     *     species{boundary = false}
     *     species{boundary = true}
     *     species{boundary = false}
     *     species{boundary = true}
     *     species{boundary = true}
     *     ...
     * }
     * Using 0 based indexing, if index = 0, we'd get the compartment name for species 1,
     * index = 1 yields compartment name for species 3,
     * index = 2 yields compartment name for species 4, and so forth.
     */
    std::string                  getNthFloatingSpeciesCompartmentName(int index);
    StringListContainer     getListOfBoundarySpecies();

    /**
     * stateless utility functions, these obviously do not
     * change the state of this NOM object.
     */
    static std::string           convertTime(const std::string& sArg, const std::string& sTimeSymbol);

    /**
     * Returns the list of all names contained in the ASTNode
     *
     * @param math ASTNode
     * @returns List of all symbols
     */
    static StringList       getSymbols(const ASTNode* math);

    /**
     * This should return an initialization for the given sbmlId that is sideeffect free
     */
    stack<std::string>           getMatchForSymbol(const std::string& sbmlId);

    /**
     * Reorders assignment rules. In SBML assignment rules does not have to appear in the correct order.
     * That is you could have an assignment rule A = B, and a rule B = C. Now the result would differ,
     * if the first rule is evaluated before the second. Thus the rules will be reordered such that
     * this will be taken care of.
     *
     * @param assignmentRules assignment rules in original ordering
     * @return assignment rules in independent order
     */
    static deque<Rule*>     reorderAssignmentRules(deque<Rule*>& assignmentRules);

    static void             fillStack(stack<std::string>& stack, SBMLSymbol& symbol);

    /**
     * Reorders the Rules of the model in such a way, that AssignmentRules are calculated first,
     * followed by Rate Rules and Algebraic Rules.
     *
     * @param doc the document to use
     * @param name the model to use
     */
    static void             reorderRules(SBMLDocument& oc, Model& model);

    static std::string           getParamPromotedSBML(const std::string& sArg);

    static std::string           getId(SBase& element);
    static std::string           convertMathMLToString(const std::string& sMathML);

    static std::string           convertStringToMathML(const std::string& var0);

private:

    /**
     * the loaded sbml doc, we own this.
     */
    SBMLDocument            *mSBMLDoc;

    /**
     * reference to the model object owned by the sbml doc,
     * hence we do NOT delete this.
     */
    Model                   *mModel;

    StringSymbolHashTable   mSymbolTable;

    StringListContainer     returnUnitDefinition(UnitDefinition oDefinition);

    ASTNode                 changeTimeToCSymbol(ASTNode* node, const std::string& name, const int& type);
    bool                    addMissingModifiers(Model *oModel);
    StringList              getSymbols(const std::string& formula);

    std::string                  getInitialAssignmentFor(const std::string& sbmlId);

    std::string                  getRuleFor(const std::string& sbmlId);
    void                    addDependenciesToList(const ASTNode* node, StringList& sResult);
    void                    addMissingParameter(const std::string& parameterId, SBMLDocument *doc);
    void                    buildSymbolTable();
    void                    changeNameToCSymbol(Model* model, const std::string& name, const int& type);
    void                    changePow(ASTNode* node);
    void                    checkForMissingNames(ASTNode *node, StringListContainer results, StringListContainer symbols);
       void                    lookForDependencies();
    void                    removeSpatialSizeUnitsFromSpecies(SBMLDocument* doc);
    void                    removeSubstanceUnitsFromKineticLaws(SBMLDocument* doc);
    void                    removeTimeUnitsFromKineticLaws(SBMLDocument* doc);
    void                    updateDependencies(const std::string& sbmlId);
    void                    upgradeToL2V4IfNecessary(SBMLDocument* doc);

    /**
     * clear the state of this object, free any objects we own.
     */
    void                    reset();
    /**
     * returns empty std::string if element is null, otherwise gets the element's
     * name if its set, otherwise gets the element's id.
     */
    static std::string           getName(const SBase* element);
    static void             modifyKineticLaws(SBMLDocument& oSBMLDoc, Model &oModel);
    static void             modifyKineticLawsForLocalParameters(KineticLaw& oLaw, const std::string& reactionId, Model &oModel);
    static void             modifyKineticLawsForReaction(KineticLaw& oLaw, const std::string& reactionId, Model &oModel);
    static void             changeSymbol(Model& oModel, const std::string& sTimeSymbol, const int& targetType);
    static void             changeTimeSymbol(Model& model, const std::string& timeSymbol);

    static void             changeParameterName(ASTNode& node, const std::string& sParameterName, const std::string& sPrefix);
    static void             getSymbols(const ASTNode* node, StringList& list);

    /**
     * type of formating we should use for printing doubles.
     */
    static const std::string     STR_DoubleFormat;
};

/**
 * SBML_formulaToString is used all over the place here,
 * SBML_formulaToString returns a char* that MUST BE FREED!!!
 *
 * This function frees the std::string and returns a std::string with its contents.
 */
std::string SBML_formulaToStdString(const ASTNode *tree);

}//namespace rr
#endif

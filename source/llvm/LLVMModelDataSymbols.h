/*
 * LLVMModelDataSymbols.h
 *
 * Created on: Jun 28, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef LLVMModelDataSymbolsH
#define LLVMModelDataSymbolsH

#include "LLVMModelData.h"
#include "rrExecutableModel.h"

#include <map>
#include <set>
#include <list>

namespace libsbml
{
    class Model;
    class SimpleSpeciesReference;
    class ASTNode;
}

namespace rrllvm
{

enum ModelDataFields {
    Size = 0,                                 // 0
    Flags,                                    // 1
    Time,                                     // 2
    NumIndCompartments,                       // 3
    NumIndFloatingSpecies,                    // 4
    NumConservedSpecies,                      // 5
    NumIndBoundarySpecies,                    // 6
    NumIndGlobalParameters,                   // 7
    NumRateRules,                             // 8
    NumReactions,                             // 9

    NumInitCompartments,                      // 10
    NumInitFloatingSpecies,                   // 11
    NumInitBoundarySpecies,                   // 12
    NumInitGlobalParameters,                  // 13

    Stoichiometry,                            // 14

    NumEvents,                                // 15
    StateVectorSize,                          // 16
    StateVector,                              // 17
    StateVectorRate,                          // 18
    RateRuleRates,                            // 19
    FloatingSpeciesAmountRates,               // 20

    CompartmentVolumesAlias,                  // 21
    InitCompartmentVolumesAlias,              // 22
    InitFloatingSpeciesAmountsAlias,          // 23
    InitConservedSpeciesAmountsAlias,         // 24
    BoundarySpeciesAmountsAlias,              // 25
    InitBoundarySpeciesAmountsAlias,          // 26
    GlobalParametersAlias,                    // 27
    InitGlobalParametersAlias,                // 28
    ReactionRatesAlias,                       // 29

    RateRuleValuesAlias,                      // 30
    FloatingSpeciesAmountsAlias,              // 31

    CompartmentVolumes,                       // 32
    InitCompartmentVolumes,                   // 33
    InitFloatingSpeciesAmounts,               // 34
    InitConservedSpeciesAmounts,              // 35
    BoundarySpeciesAmounts,                   // 36
    InitBoundarySpeciesAmounts,               // 37
    GlobalParameters,                         // 38
    InitGlobalParameters,                     // 39
    ReactionRates,                            // 40
    NotSafe_RateRuleValues,                   // 41
    NotSafe_FloatingSpeciesAmounts,           // 42
};

enum EventAtributes
{
    EventUseValuesFromTriggerTime = (0x1 << 0), // => 0x00000001
    EventInitialValue             = (0x1 << 1), // => 0x00000010
    EventPersistent               = (0x1 << 2)  // => 0x00000100
};

/**
 * stores the names of all the symbols in the sbml model and thier
 * indexes in the ModelData arrays. This class essentially maps symbol
 * names into ModelData indexes.
 *
 * The logic behind splitting up LLVMModelDataSymbols and LLVMModelSymbols
 * is that LLVMModelSymbols contains all the ASTs from the model, and this
 * contains only ModelData indices -- once the all the model functions
 * are generated, there is no need to keep AST information around, but we
 * still need to know what symbol is at what index for the various
 * Model accessor functions.
 *
 * * data storage:
 * There can exist rules (assigment, rate, and eventually algebraic) rules
 * that determine the value of a symbol.
 *
 * All items for which a rate rule exists are stored in the
 * modelData::rateRules array.
 *
 * No space is allocated for items determined by rate rules.
 *
 * * All elements get an index value, even the dependent ones, this allows
 * us uniquely indentify them in the generated accessor functions.
 *
 * * Most of the indexes used in this class are indexes into ModelData
 * arrays, therefore we use unsigned integers -- these are less error
 * prone and its easier to check if they are valid i.e. only check that
 * they are less than the the array size and we do not have to check that
 * it is positive.
 *
 * * All symbols from the sbml are reordered such that the independent
 * ones are first, followed by the dependent values.
 */
class LLVMModelDataSymbols
{
public:

    typedef std::map<std::string, uint> StringUIntMap;
    typedef std::pair<std::string, uint> StringUIntPair;

    enum SpeciesReferenceType
    {
        Reactant, Product, Modifier
    };

    struct SpeciesReferenceInfo
    {
        uint row;
        uint column;
        SpeciesReferenceType type;
        std::string id;
    };

    LLVMModelDataSymbols();

    LLVMModelDataSymbols(libsbml::Model const* model, unsigned options);

    virtual ~LLVMModelDataSymbols();

    const std::string& getModelName() const;

    uint getCompartmentIndex(std::string const&) const;

    /**
     * get the index of a floating species.
     *
     * @param requireIndependent if true, the float species must be independent --
     * not defined by a rule.
     */
    uint getFloatingSpeciesIndex(std::string const&, bool requireIndependent = true) const;

    uint getBoundarySpeciesIndex(std::string const&) const;

    /**
     * number of boundary species not defined by rules.
     */
    uint getIndependentBoundarySpeciesSize() const;

    /**
     * number of fully indenpendent species, these are species that
     * have thier dynamics fully determined by the reaction rates /
     * stoichiometric matrix -- these DO NOT have any rules determining
     * their dynamics.
     *
     * This is size of the allocated array in the ModelData struct.
     */
    uint getIndependentFloatingSpeciesSize() const;


    uint getConservedSpeciesSize() const;

    uint getGlobalParameterIndex(std::string const&) const;

    uint getRateRuleIndex(std::string const&) const;
    uint getRateRuleSize() const;

    /**
     * number of global parameters which are not determined by rules.
     */
    uint getIndependentGlobalParameterSize() const;

    uint getReactionIndex(std::string const&) const;
    std::vector<std::string> getReactionIds() const;
    uint getReactionSize() const;

    std::vector<std::string> getGlobalParameterIds() const;

    /**
     * the list that is returned by ExecutableModel, so order must
     * remain constant.
     *
     * All floating species ids, independent first.
     */
    std::vector<std::string> getFloatingSpeciesIds() const;


    /**
     * total size of all floating species.
     */
    uint getFloatingSpeciesSize() const;

    uint getBoundarySpeciesSize() const;

    uint getCompartmentsSize() const;

    uint getGlobalParametersSize() const;

    std::vector<std::string> getCompartmentIds() const;

    /**
     * number of compartments which are not determined by rules.
     */
    uint getIndependentCompartmentSize() const;



    /**
     * all the boundary species ids, independent first.
     */
    std::vector<std::string> getBoundarySpeciesIds() const;

    /**
     * get a list of all the non-zero entries in the
     * stoichiometry matrix.
     *
     * in the list of pairs, first is the row (species) index,
     * and second is the column (reaction) index.
     */
    std::list<SpeciesReferenceInfo> getStoichiometryIndx() const;

    /**
     * initialize and allocate the buffers (including the stoich matrix)
     * for an existing ModelData struct.
     */
    //void initAllocModelDataBuffers(LLVMModelData& m) const;

    void print() const;

    /**
     * if there are no rules for an element, then they are considered
     * independent.
     *
     * These are only for elements, not init values, will return false
     * for all init symbols.
     */
    bool isIndependentElement(const std::string& id) const;


    bool hasRateRule(const std::string& id) const;

    bool hasAssignmentRule(const std::string& id) const;

    bool hasInitialAssignmentRule(const std::string& id) const;

    bool isIndependentFloatingSpecies(const std::string& id) const;

    bool isIndependentBoundarySpecies(const std::string& id) const;

    bool isIndependentGlobalParameter(const std::string& id) const;

    bool isIndependentCompartment(const std::string& id) const;

    bool isNamedSpeciesReference(const std::string& id) const;

    const SpeciesReferenceInfo& getNamedSpeciesReferenceInfo(
            const std::string& id) const;



    /**
     * checks if the given symbol is a init value for a conserved species.
     *
     * Global parameters or floating species can be conservied moieties,
     * a global parameter is a CM if it is defined by a rule as a linear
     * combination of floating species.
     *
     * A floating species may be a CM if it is defined by a rule
     * as a linear combination of independent species.
     *
     * If a floating species is a CM, then it must by definition
     * be a dependent floating species.
     *
     * It can however have independent initial conditions defined
     * either by intial values or initial assignment rules.
     */
    bool isConservedMoiety(const std::string& symbol) const;


    const std::vector<unsigned char>& getEventAttributes() const;

    /**
     * get the textual form of the field names.
     */
    static const char* getFieldName(ModelDataFields field);

    /**
     * get the size (in number of doubles) of the buffer
     * that events need to save the event data to.
     */
    uint getEventBufferSize(uint eventId) const;

    /**
     * the the row indices of non-zero stoichiometry values
     */
    const std::vector<uint>& getStoichRowIndx() const;

    /**
     * the the column indices of non-zero stoichiometry values
     */
    const std::vector<uint>& getStoichColIndx() const;


    /************************ Initial Conditions Section **************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * checks if the given symbol is an init value for an independent
     * floating species.
     */
    bool isIndependentInitFloatingSpecies(const std::string& symbol) const;

    /**
     * Is this sbml element an independent initial value.
     *
     * Independent initial values do not have assignment or
     * initial assigment rules, but may have rate rules.
     */
    bool isIndependentInitElement(const std::string& symbol) const;

    bool isIndependentInitCompartment(const std::string& symbol) const;

    /**
     * get the index of a floating species initial value.
     *
     * has the same index as the run time floating species.
     */
    uint getFloatingSpeciesInitIndex(const std::string& symbol) const;

    /**
     * get the index of a compartment initial value
     *
     * has the same index as the run time compartment.
     */
    uint getCompartmentInitIndex(const std::string& symbol) const;


    uint getInitCompartmentSize() const;
    uint getInitFloatingSpeciesSize() const;
    uint getInitBoundarySpeciesSize() const;
    uint getInitGlobalParameterSize() const;

private:

    std::set<std::string> initAssignmentRules;

    /**
     * map of floating species init value symbols to thier
     * index in the array.
     *
     * All floating species are stored in this map, however
     * they are reordered and only the ones with index <
     * independentInitFloatingSpeciesSize are allocated storage.
     */
    StringUIntMap initFloatingSpeciesMap;
    StringUIntMap initBoundarySpeciesMap;
    StringUIntMap initCompartmentsMap;
    StringUIntMap initGlobalParametersMap;

    /**
     * Elements that do NOT have assignment rules are considered
     * independent initial conditions.
     *
     * Ind init conditions CAN however defined by rules.
     *
     * ind init conditions are all elements that do not have
     * assignment or initial assignment rules.
     */

    /**
     * float species init array has floating species NOT
     * defined by init assignment rules, assignment rules,
     * but rate rules are OK. This does not contain the init values
     * for conserved species.
     */
    uint independentInitFloatingSpeciesSize;

    /**
     * boundary species not defined by any rule
     * including init assignment rules.
     */
    uint independentInitBoundarySpeciesSize;

    /**
     * global params not defined by any rule
     */
    uint independentInitGlobalParameterSize;

    /**
     * compartments not defined by any rules.
     */
    uint independentInitCompartmentSize;


    /************************ End Initial Conditions Section **********************/
    #endif /***********************************************************************/
    /******************************************************************************/


private:

    std::string modelName;
    StringUIntMap floatingSpeciesMap;
    StringUIntMap boundarySpeciesMap;
    StringUIntMap compartmentsMap;
    StringUIntMap globalParametersMap;



    /**
     * map of all identified species reference (species references with ids)
     * to their indices in the stoichiometric matrix.
     */
    typedef std::map<std::string, SpeciesReferenceInfo> StringRefInfoMap;
    StringRefInfoMap namedSpeciesReferenceInfo;

    /**
     * all reactions are named.
     */
    StringUIntMap reactionsMap;

    /**
     * the stochiometry matrix is # reactions rows by # species columns.
     *
     * the species indices go here.
     */
    std::vector<uint> stoichColIndx;

    /**
     * and the reaction indices go here.
     */
    std::vector<uint> stoichRowIndx;

    /**
     * most species references are un-named, this is used so that if named ones
     * have a rate rule, we can set the proper initial conditions.
     */
    std::vector<std::string> stoichIds;

    std::vector<SpeciesReferenceType> stoichTypes;


    /**
     * the set of rule, these contain the variable name of the rule so that
     * we can quickly see if a symbol has an associated rule.
     */
    std::set<std::string> assigmentRules;




    /**
     * rate rules, index by variable name.
     */
    StringUIntMap rateRules;

    uint independentFloatingSpeciesSize;
    uint independentBoundarySpeciesSize;
    uint independentGlobalParameterSize;
    uint independentCompartmentSize;



    /**
     * the number of assignments each event has
     */
    std::vector<uint> eventAssignmentsSize;

    std::vector<unsigned char> eventAttributes;

    void initBoundarySpecies(const libsbml::Model *);

    void initFloatingSpecies(const libsbml::Model *,
            bool computeAndAssignConsevationLaws);


    /**
     *
     */
    void initCompartments(const libsbml::Model *);

    /**
     * get the global parameters, need to reorder them to set the independent
     * ones first
     */
    void initGlobalParameters(const libsbml::Model *model);

    void initReactions(const libsbml::Model *model);

    void displayCompartmentInfo();

    void initEvents(const libsbml::Model *model);

    /**
     * determine is this species can be used as a species reference,
     * if not, logs the reason why its not valid.
     */
    bool isValidSpeciesReference(const libsbml::SimpleSpeciesReference*,
            const std::string& reacOrProd);
};

} /* namespace rr */
#endif /* RRLLVMMODELDATASYMBOLS_H_ */

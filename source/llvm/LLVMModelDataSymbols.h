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

namespace libsbml { class Model; }

namespace rr
{

enum ModelDataFields {
    Size = 0,                                 // 0
    Flags,                                    // 1
    Time,                                     // 2
    NumIndependentSpecies,                    // 3
    NumDependentSpecies,                      // 4
    DependentSpeciesConservedSums,            // 5
    NumGlobalParameters,                      // 6
    GlobalParameters,                         // 7
    NumReactions,                             // 8
    ReactionRates,                            // 9
    NumRateRules,                             // 10
    RateRuleValues,                           // 11
    RateRuleRates,                            // 12
    LocalParametersOffsets,                   // 13
    LocalParametersNum,                       // 14
    LocalParameters,                          // 15
    NumFloatingSpecies,                       // 16
    FloatingSpeciesConcentrations,            // 17
    FloatingSpeciesInitConcentrations,        // 18
    FloatingSpeciesAmountRates,               // 19
    FloatingSpeciesAmounts,                   // 20
    FloatingSpeciesCompartments,              // 21
    NumBoundarySpecies,                       // 22
    BoundarySpeciesConcentrations,            // 23
    BoundarySpeciesAmounts,                   // 24
    BoundarySpeciesCompartments,              // 25
    NumCompartments,                          // 26
    CompartmentVolumes,                       // 27
    Stoichiometry,                            // 28
    NumEvents,                                // 29
    EventTypeSize,                            // 30
    EventType,                                // 31
    EventPersistentTypeSize,                  // 32
    EventPersistentType,                      // 33
    EventTestsSize,                           // 34
    EventTests,                               // 35
    EventPrioritiesSize,                      // 36
    EventPriorities,                          // 37
    EventStatusArraySize,                     // 38
    EventStatusArray,                         // 39
    PreviousEventStatusArraySize,             // 40
    PreviousEventStatusArray,                 // 41
    StateVectorSize,                          // 42
    StateVector,                              // 43
    StateVectorRate,                          // 44
    WorkSize,                                 // 45
    Work,                                     // 46
    EventDelays,                              // 47
    EventAssignments,                         // 47
    ComputeEventAssignments,                  // 48
    PerformEventAssignments,                  // 50
    ModelName,                                // 51
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
 */
class LLVMModelDataSymbols
{
public:

    typedef std::map<std::string, uint> StringUIntMap;
    typedef std::pair<std::string, uint> StringUIntPair;

    LLVMModelDataSymbols();

    LLVMModelDataSymbols(libsbml::Model const* model,
            bool computeAndAssignConsevationLaws);

    virtual ~LLVMModelDataSymbols();


    uint getCompartmentIndex(std::string const&) const;
    uint getFloatingSpeciesIndex(std::string const&) const;
    uint getBoundarySpeciesIndex(std::string const&) const;

    uint getIndependentBoundarySpeciesSize() const;

    uint getFloatingSpeciesCompartmentIndex(std::string const&) const;
    uint getBoundarySpeciesCompartmentIndex(std::string const&) const;

    /**
     * number of linearly indenent rows in the stochiometry matrix.
     */
    uint getLinearlyIndependentFloatingSpeciesSize() const;

    /**
     * number of fully indenpendent species, these are species that
     * have thier dynamics fully determined by the reaction rates /
     * stoichiometric matrix -- these DO NOT have any rules determining
     * their dynamics.
     *
     * This is size of the allocated array in the ModelData struct.
     */
    uint getIndependentFloatingSpeciesSize() const;

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
    std::vector<std::string> getFloatingSpeciesIds() const;
    uint getFloatingSpeciesSize() const;

    uint getBoundarySpeciesSize() const;

    uint getCompartmentsSize() const;

    uint getGlobalParametersSize() const;

    std::vector<std::string> getCompartmentIds() const;

    /**
     * number of compartments which are not determined by rules.
     */
    uint getIndependentCompartmentSize() const;

    std::vector<std::string> getBoundarySpeciesIds() const;

    /**
     * get a list of all the non-zero entries in the
     * stoichiometry matrix.
     *
     * in the list of pairs, first is the row (species) index,
     * and second is the column (reaction) index.
     */
    std::list<std::pair<uint,uint> > getStoichiometryIndx() const;

    /**
     * initialize and allocate the buffers (including the stoich matrix)
     * for an existing ModelData struct.
     */
    void initAllocModelDataBuffers(LLVMModelData& m) const;

    void print() const;

    /**
     * if there are no rules for an element, then they are considered
     * independent.
     */
    bool isIndependentElement(const std::string& id) const;

    bool hasRateRule(const std::string& id) const;

    bool hasAssignmentRule(const std::string& id) const;

    bool isIndependentFloatingSpecies(const std::string& id) const;

    bool isIndependentBoundarySpecies(const std::string& id) const;

    bool isIndependentGlobalParameter(const std::string& id) const;

    bool isIndependentCompartment(const std::string& id) const;

    /**
     * get the textual form of the field names.
     */
    static const char* getFieldName(ModelDataFields field);


private:

    std::string modelName;
    StringUIntMap floatingSpeciesMap;
    StringUIntMap boundarySpeciesMap;
    StringUIntMap compartmentsMap;
    StringUIntMap globalParametersMap;

    /**
     * all reactions are named.
     */
    StringUIntMap reactionsMap;

    /**
     * compartment that a floating species belongs to,
     * indexed by floating species index.
     */
    std::vector<uint> floatingSpeciesCompartments;

    /**
     * compartment that a boundary species belongs to,
     * indexed by boundary species index.
     */
    std::vector<uint> boundarySpeciesCompartments;

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
     * the set of rule, these contain the variable name of the rule so that
     * we can quickly see if a symbol has an associated rule.
     */
    std::set<std::string> assigmentRules;

    /**
     * rate rules, index by variable name.
     */
    StringUIntMap rateRules;

    uint linearlyIndependentFloatingSpeciesSize;
    uint independentFloatingSpeciesSize;
    uint independentBoundarySpeciesSize;
    uint independentGlobalParameterSize;
    uint independentCompartmentSize;

};

} /* namespace rr */
#endif /* RRLLVMMODELDATASYMBOLS_H_ */

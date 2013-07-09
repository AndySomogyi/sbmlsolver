/*
 * rrLLVMModelDataSymbols.h
 *
 * Created on: Jun 28, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef LLVMModelDataSymbolsH
#define LLVMModelDataSymbolsH

#include "rrModelData.h"
#include "rrExecutableModel.h"

#include <map>

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
    RateRules,                                // 11
    NumRates,                                 // 12
    Rates,                                    // 13
    LocalParametersOffsets,                   // 14
    LocalParametersNum,                       // 15
    LocalParameters,                          // 16
    NumFloatingSpecies,                       // 17
    FloatingSpeciesConcentrations,            // 18
    FloatingSpeciesInitConcentrations,        // 19
    FloatingSpeciesConcentrationRates,        // 20
    FloatingSpeciesAmounts,                   // 21
    FloatingSpeciesCompartments,              // 22
    NumBoundarySpecies,                       // 23
    BoundarySpeciesConcentrations,            // 24
    BoundarySpeciesAmounts,                   // 25
    BoundarySpeciesCompartments,              // 26
    NumCompartments,                          // 27
    CompartmentVolumes,                       // 28
    Stoichiometry,                            // 29
    NumEvents,                                // 30
    EventTypeSize,                            // 31
    EventType,                                // 32
    EventPersistentTypeSize,                  // 33
    EventPersistentType,                      // 34
    EventTestsSize,                           // 35
    EventTests,                               // 36
    EventPrioritiesSize,                      // 37
    EventPriorities,                          // 38
    EventStatusArraySize,                     // 39
    EventStatusArray,                         // 40
    PreviousEventStatusArraySize,             // 41
    PreviousEventStatusArray,                 // 42
    WorkSize,                                 // 43
    Work,                                     // 44
    EventDelays,                              // 45
    EventAssignments,                         // 46
    ComputeEventAssignments,                  // 47
    PerformEventAssignments,                  // 48
    ModelName,                                // 49
    VariableTable,                            // 50
    BoundaryTable,                            // 51
    GlobalParameterTable,                     // 52
    SrSize,                                   // 53
    Sr                                        // 54
};

class LLVMModelDataSymbols
{
public:

    typedef std::map<std::string, int> StringIntMap;
    typedef std::pair<std::string, int> StringIntPair;

    LLVMModelDataSymbols();

    LLVMModelDataSymbols(libsbml::Model const* model,
            bool computeAndAssignConsevationLaws);

    virtual ~LLVMModelDataSymbols();



    int getCompartmentIndex(std::string const&) const;
    int getFloatingSpeciesIndex(std::string const&) const;
    int getBoundarySpeciesIndex(std::string const&) const;
    int getFloatingSpeciesCompartmentIndex(std::string const&) const;
    int getBoundarySpeciesCompartmentIndex(std::string const&) const;
    int getGlobalParameterIndex(std::string const&) const;
    std::vector<std::string> getGlobalParameterIds() const;
    std::vector<std::string> getFloatingSpeciesIds() const;
    std::vector<std::string> getCompartmentIds() const;
    std::vector<std::string> getBoundarySpeciesIds() const;





    void initAllocModelDataBuffers(ModelData& m) const;

    void print() const;



private:

    std::string modelName;
    StringIntMap floatingSpeciesMap;
    StringIntMap boundarySpeciesMap;
    StringIntMap compartmentsMap;
    StringIntMap globalParametersMap;
    std::vector<int> floatingSpeciesCompartments;
    std::vector<int> boundarySpeciesCompartments;


};

} /* namespace rr */
#endif /* RRLLVMMODELDATASYMBOLS_H_ */

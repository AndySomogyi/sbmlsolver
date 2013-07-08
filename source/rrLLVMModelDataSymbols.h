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
    Size = 0,
    ModelName,
    Time,
    NumIndependentSpecies,
    NumDependentSpecies,
    DependentSpeciesConservedSums,
    NumGlobalParameters,
    GlobalParameters,
    NumReactions,
    ReactionRates,
    LocalParametersOffsets,
    LocalParametersNum,
    LocalParameters,
    NumFloatingSpecies,
    FloatingSpeciesConcentrations,
    FloatingSpeciesInitConcentrations,
    FloatingSpeciesConcentrationRates,
    FloatingSpeciesAmounts,
    FloatingSpeciesCompartments,
    NumBoundarySpecies,
    BoundarySpeciesConcentrations,
    BoundarySpeciesAmounts,
    BoundarySpeciesCompartments,
    NumCompartments,
    CompartmentVolumes,
    NumRateRules,
    RateRules,
    NumEvents,
    EventTypeSize,
    EventType,
    EventPersistentTypeSize,
    EventPersistentType,
    EventTestsSize,
    EventTests,
    EventPrioritiesSize,
    EventPriorities,
    EventStatusArraySize,
    EventStatusArray,
    PreviousEventStatusArraySize,
    PreviousEventStatusArray,
    WorkSize,
    Work,
    EventDelays,
    EventAssignments,
    ComputeEventAssignments,
    PerformEventAssignments,
    VariableTable,
    BoundaryTable,
    GlobalParameterTable,
    SrSize,
    Sr
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

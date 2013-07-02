/*
 * rrLLVMModelData.h
 *
 * Created on: Jun 17, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef rrLLVMModelDataH
#define rrLLVMModelDataH

namespace llvm {
class StructType;
class Module;
class ExecutionEngine;
class Value;
}

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

/**
 * A class with a long name...
 *
 * In RoadRunner, all model data is stored appropriately in the ModelData class,
 * as arrays of concentrations, parameters, etc...
 *
 * This class provides symbolic access to all of this data for the LLVM code
 * generator, i.e. this class is the symbol table for all the named SBML values.
 */
class LLVMModelDataValue
{
public:


    LLVMModelDataValue();
    LLVMModelDataValue(llvm::Value *rhs);
    ~LLVMModelDataValue();



    static llvm::StructType *getStructType(llvm::Module *module,
            llvm::ExecutionEngine *engine = 0);



private:
    /**
     * TODO: figure out if its a problem re-creating this???
     */
    llvm::Value *value;

};

} /* namespace rr */
#endif /* rrLLVMModelDataTypeH */

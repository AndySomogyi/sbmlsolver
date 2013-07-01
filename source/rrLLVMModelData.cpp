/*
 * rrLLVMModelData.cpp
 *
 *  Created on: Jun 17, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#include "rrLLVMModelData.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrModelData.h"

#include "rrLLVMIncludes.h"
#include "rrLLVMException.h"

#include <vector>
#include <string>

using namespace std;
using namespace llvm;



namespace rr
{

LLVMModelDataValue::LLVMModelDataValue() :
    value(0)
{
}

LLVMModelDataValue::~LLVMModelDataValue()
{
    // we don't own anything
}




llvm::StructType *LLVMModelDataValue::getStructType(llvm::Module *module, llvm::ExecutionEngine *engine)
{
    StructType *structType = module->getTypeByName("rr::ModelData");

    if (!structType)
    {
        LLVMContext &context = module->getContext();

        // different compilers define bool differently, it used to be more common to store
        // bool as a 32 bit int, but now its more common to use an 8 bit int.
        Type *boolType = IntegerType::get(context, sizeof(bool) * 8);
        Type *boolPtrType = boolType->getPointerTo();
        Type *charStarStarType = Type::getInt8PtrTy(context)->getPointerTo();
        Type *voidPtrType = Type::getInt8PtrTy(context);

        vector<Type*> elements;

        elements.push_back(Type::getInt32Ty(context));                        // unsigned                            size;
        elements.push_back(Type::getInt8PtrTy(context));                      // char*                               modelName;
        elements.push_back(Type::getDoubleTy(context));                       // double                              time;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numIndependentSpecies;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numDependentSpecies;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             dependentSpeciesConservedSums;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numGlobalParameters;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             globalParameters;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numReactions;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             reactionRates;
        elements.push_back(Type::getInt32PtrTy(context));                     // int*                                localParametersOffsets;
        elements.push_back(Type::getInt32PtrTy(context));                     // int*                                localParametersNum;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             localParameters;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numFloatingSpecies;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             floatingSpeciesConcentrations;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             floatingSpeciesInitConcentrations;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             floatingSpeciesConcentrationRates;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             floatingSpeciesAmounts;
        elements.push_back(Type::getInt32PtrTy(context));                     // int*                                floatingSpeciesCompartments;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numBoundarySpecies;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             boundarySpeciesConcentrations;
        elements.push_back(Type::getInt32PtrTy(context));                     // int*                                boundarySpeciesCompartments;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numCompartments;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             compartmentVolumes;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numRateRules;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             rateRules;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 numEvents;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 eventTypeSize;
        elements.push_back(boolPtrType);                                      // bool*                               eventType;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 eventPersistentTypeSize;
        elements.push_back(boolPtrType);                                      // bool*                               eventPersistentType;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 eventTestsSize;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             eventTests;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 eventPrioritiesSize;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             eventPriorities;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 eventStatusArraySize;
        elements.push_back(boolPtrType);                                      // bool*                               eventStatusArray;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 previousEventStatusArraySize;
        elements.push_back(boolPtrType);                                      // bool*                               previousEventStatusArray;
        elements.push_back(voidPtrType);                                      // TEventDelayDelegate*                eventDelays;
        elements.push_back(voidPtrType);                                      // TEventAssignmentDelegate*           eventAssignments;
        elements.push_back(voidPtrType);                                      // TComputeEventAssignmentDelegate*    computeEventAssignments;
        elements.push_back(voidPtrType);                                      // TPerformEventAssignmentDelegate*    performEventAssignments;
        elements.push_back(charStarStarType);                                 // char**                              variableTable;
        elements.push_back(charStarStarType);                                 // char**                              boundaryTable;
        elements.push_back(charStarStarType);                                 // char**                              globalParameterTable;
        elements.push_back(Type::getInt32Ty(context));                        // int                                 srSize;
        elements.push_back(Type::getDoublePtrTy(context));                    // double*                             sr;

        StructType *structType = StructType::create(context, elements, "rr::ModelData");

        const DataLayout *dl = engine->getDataLayout();

        size_t llvm_size = dl->getTypeStoreSize(structType);

        printf("TestStruct size: %i, , LLVM Size: %i\n", sizeof(ModelData), llvm_size);

        printf("C++ bool size: %i, LLVM bool size: %i\n", sizeof(bool), dl->getTypeStoreSize(boolType));
        printf("C++ bool* size: %i, LLVM bool* size: %i\n", sizeof(bool*), dl->getTypeStoreSize(boolPtrType));
        printf("C++ char** size: %i, LLVM char** size: %i\n", sizeof(char**), dl->getTypeStoreSize(charStarStarType));
        printf("C++ void* size: %i, LLVM void* size: %i\n", sizeof(void*), dl->getTypeStoreSize(voidPtrType));

        printf("C++ TEventDelayDelegate* size: %i\n", sizeof(TEventDelayDelegate*));
        printf("C++ TEventAssignmentDelegate* size: %i\n", sizeof(TEventAssignmentDelegate* ));
        printf("C++ TComputeEventAssignmentDelegate* size: %i\n", sizeof(TComputeEventAssignmentDelegate*));
        printf("C++ TPerformEventAssignmentDelegate* size: %i\n", sizeof(TPerformEventAssignmentDelegate*));
    }
    return structType;
}



} /* namespace rr */

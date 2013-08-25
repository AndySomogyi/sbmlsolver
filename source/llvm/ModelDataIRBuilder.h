/*
 * ModelDataIRBuilder.h
 *
 * Created on: Jun 28, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef ModelDataIRBuilderH
#define ModelDataIRBuilderH

#include "LLVMIncludes.h"
#include "LLVMModelDataSymbols.h"

#include <map>

namespace libsbml
{
class Model;
}

namespace rr
{

class ModelDataIRBuilder
{
public:

    /**
     * these only make sense attached to a modelData pointer.
     */
    ModelDataIRBuilder(llvm::Value *modelData, LLVMModelDataSymbols const&,
            llvm::IRBuilder<> &);

    llvm::Value *createFloatSpeciesAmtGEP(const std::string &id,
            const llvm::Twine &name = "");

    llvm::Value *createBoundSpeciesAmtGEP(const std::string &id,
            const llvm::Twine &name = "");

    llvm::Value *createGlobalParamGEP(const std::string &id);

    /**
     * create a GEP for a ModelData structure field.
     *
     * @param md: value for a ModelData pointer type.
     */
    llvm::Value *createGEP(ModelDataFields field, const llvm::Twine& name = "");

    /**
     * create a GEP for an array which belongs to the ModelData struct,
     * only valid for arrays, will crash on non-array fields.
     */
    llvm::Value *createGEP(ModelDataFields field, unsigned index,
            const llvm::Twine& name = "");

    llvm::Value *createLoad(ModelDataFields field, unsigned index,
            const llvm::Twine& name = "");

    llvm::Value *createStore(ModelDataFields field, unsigned index,
            llvm::Value *value, const llvm::Twine& name = "");

    /**
     * the GEP for a compartment
     */
    llvm::Value* createCompGEP(const std::string& id,
            const llvm::Twine &name = "");

    /**
     * load the compartment value
     */
    llvm::Value *createCompLoad(const std::string& id,
            const llvm::Twine& name ="");

    /**
     * store the compartment value
     */
    llvm::Value *createCompStore(const std::string &id,
            llvm::Value *value);

    /**
     * load the compartment value
     */
    llvm::Value *createGlobalParamLoad(const std::string& id,
            const llvm::Twine& name = "");

    /**
     * store the compartment value
     */
    llvm::Value *createGlobalParamStore(const std::string &id,
            llvm::Value *value);

    /**
     * load the floating species amount value
     */
    llvm::Value *createFloatSpeciesAmtLoad(const std::string& id,
            const llvm::Twine& name = "");

    /**
     * store the floating species amount
     */
    llvm::Value *createFloatSpeciesAmtStore(const std::string &id,
            llvm::Value *value);

    /**
     * load the floating species amount value
     */
    llvm::Value *createBoundSpeciesAmtLoad(const std::string& id,
            const llvm::Twine& name = "");

    /**
     * store the floating species amount
     */
    llvm::Value *createBoundSpeciesAmtStore(const std::string &id,
            llvm::Value *value);

    /**
     * load the floating species amount value
     */
    llvm::Value *createReactionRateLoad(const std::string& id,
            const llvm::Twine& name = "");

    /**
     * store the floating species amount
     */
    llvm::Value *createReactionRateStore(const std::string &id,
            llvm::Value *value);

    /**
     * rate rule GEP
     */
    llvm::Value *createRateRuleValueGEP(const std::string &id,
            const llvm::Twine &name = "");

    /**
     * load the rate rule value
     */
    llvm::Value *createRateRuleValueLoad(const std::string& id,
            const llvm::Twine& name = "");

    /**
     * store the rate rule value
     */
    llvm::Value *createRateRuleValueStore(const std::string &id,
            llvm::Value *value);

    /**
     * rate rule GEP
     */
    llvm::Value *createRateRuleRateGEP(const std::string &id,
            const llvm::Twine &name = "");

    /**
     * load the rate rule rate
     */
    llvm::Value *createRateRuleRateLoad(const std::string& id,
            const llvm::Twine& name = "");

    /**
     * store the rate rule rate
     */
    llvm::Value *createRateRuleRateStore(const std::string &id,
            llvm::Value *value);

    llvm::Value *createStoichiometryStore(uint row, uint col,
            llvm::Value *value, const llvm::Twine& name = "");

    llvm::Value *createStoichiometryLoad(uint row, uint col,
            const llvm::Twine& name = "");

    /**
     * create a call to the csr_matrix_set_nz function.
     *
     * only valid whilst a basic block is being filled.
     */
    static llvm::CallInst *createCSRMatrixSetNZ(llvm::IRBuilder<> &,
            llvm::Value *csrPtr, llvm::Value *row, llvm::Value *col,
            llvm::Value *value, const llvm::Twine& name = "");

    /**
     * create a call to the csr_matrix_get_nz function.
     *
     * only valid whilst a basic block is being filled.
     */
    static llvm::CallInst *createCSRMatrixGetNZ(llvm::IRBuilder<> &,
            llvm::Value *csrPtr, llvm::Value *row, llvm::Value *col,
            const llvm::Twine& name = "");

    /**
     * get the function declaration (prototype) for the csr_matrix_set_nz function.
     *
     * create if not found.
     */
    static llvm::Function *getCSRMatrixSetNZDecl(llvm::Module *module);

    /**
     * get the function declaration (prototype) for the csr_matrix_set_nz function.
     *
     * create if not found
     */
    static llvm::Function *getCSRMatrixGetNZDecl(llvm::Module *module);

    /**
     * get the struct type for the CSR sparse storage struct.
     *
     * create if not found
     */
    static llvm::StructType *getCSRSparseStructType(llvm::Module *module,
            llvm::ExecutionEngine *engine = 0);

    /**
     * get the ModelData struct type.
     *
     * create if not found.
     */
    static llvm::StructType *getStructType(llvm::Module *module,
            llvm::ExecutionEngine *engine = 0);

    static const char* LLVMModelDataName;
    static const char* csr_matrixName;
    static const char* csr_matrix_set_nzName;
    static const char* csr_matrix_get_nzName;

private:
    /**
     * the model data struct we generate the loads / stores relative to.
     */
    llvm::Value *modelData;

    llvm::IRBuilder<> &builder;

    LLVMModelDataSymbols const& symbols;

    /**
     * get the module, only valid whilst a BasicBlock is begin filled.
     *
     * @param fname: name of the calling function, used for generating exception
     * on failure.
     *
     * @returns a module on success, throws exeption on failure.
     */
    static llvm::Module *getModule(llvm::IRBuilder<> &, const char* func);

    /**
     * check to see that s is a ModelData struct pointer,
     * throw an exception if not.
     */
    void validateStruct(llvm::Value *s, const char* funcName);

    friend class LLVMModelDataIRBuilderTesting;
};

/**
 * testing class
 */
class LLVMModelDataIRBuilderTesting
{
public:

    LLVMModelDataIRBuilderTesting(LLVMModelDataSymbols const&,
            llvm::IRBuilder<> &);

    /*********************** TESTING STUFF WILL GO AWAY EVENTUALLY ***********************/

    void createAccessors(llvm::Module *module);

    std::pair<llvm::Function*, llvm::Function*> createFloatingSpeciesAccessors(
            llvm::Module *module, const std::string id);

    void test(llvm::Module *module, llvm::IRBuilder<> *build,
            llvm::ExecutionEngine * engine);

    llvm::Value *createFloatSpeciesConcGEP(const std::string &id);

    llvm::Value *createFloatSpeciesConcStore(const std::string &id,
            llvm::Value *value);

    static llvm::Function *getDispIntDecl(llvm::Module *module);
    llvm::CallInst *createDispInt(llvm::Value *intVal);

    static llvm::Function *getDispDoubleDecl(llvm::Module *module);
    llvm::CallInst *createDispDouble(llvm::Value *doubleVal);

    static llvm::Function *getDispCharDecl(llvm::Module *module);
    llvm::CallInst *createDispChar(llvm::Value *doubleVal);

    /*************************************************************************************/


    llvm::IRBuilder<> &builder;

    LLVMModelDataSymbols const& symbols;
};

} /* namespace rr */
#endif /* ModelDataIRBuilderH */

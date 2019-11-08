/*
 * ASTNodeCodeGen.h
 *
 * Created on: Jun 16, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */
#ifndef ASTNodeCodeGenH
#define ASTNodeCodeGenH

#include "LLVMIncludes.h"
#include "CodeGen.h"
#include "ModelGeneratorContext.h"

namespace libsbml
{
class ASTNode;
}

namespace rrllvm
{
/**
 * All of the LLVM IR generation is handled here.
 */
class ASTNodeCodeGen
{
public:
    ASTNodeCodeGen(llvm::IRBuilder<> &builder,
            LoadSymbolResolver &resolver,
            const ModelGeneratorContext& ctx,
            llvm::Value *modelData);
    ~ASTNodeCodeGen();

    llvm::Value *codeGen(const libsbml::ASTNode *ast);
private:

    /**
     * sbml ASTNode does not contain as assigment '=' type, assigment
     * is handled by other sbml elements such as initialAssigment.
     *
     */
    llvm::Value *binaryExprCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *notImplemented(const libsbml::ASTNode *ast);

    llvm::Value *delayExprCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *nameExprCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *realExprCodeGen(const libsbml::ASTNode *ast);

    /**
     * Support new AST nodes for distributions
     */
    llvm::Value *distribCodeGen(const libsbml::ASTNode *ast);


    /**
     * for now, just convert to double,
     *
     * TODO: is this right???
     */
    llvm::Value *integerCodeGen(const libsbml::ASTNode *ast);

    /**
     * would have made the most sense using templated pointer to a
     * IRBuilder member function, but many compilers seem to have trouble
     * with such templates.
     */
    llvm::Value *applyArithmeticCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *minmaxCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *applyRelationalCodeGen(const libsbml::ASTNode *ast);

    // JKM: NOTE: Not SBML-compliant, needed for idiosyncrasies in some legacy JDesigner models
    llvm::Value *applyScalarRelationalCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *applyLogicalCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *functionCallCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *intrinsicCallCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *piecewiseCodeGen(const libsbml::ASTNode *ast);

    /**
     * coerces a value to a boolean single bit.
     *
     * If value is already a boolean, it is unchanged.
     */
    llvm::Value *toBoolean(llvm::Value *value);

    /**
     * coerces a value to a double
     *
     * If value is already a boolean, it is unchanged.
     */
    llvm::Value *toDouble(llvm::Value* value);


    llvm::IRBuilder<> &builder;
    LoadSymbolResolver &resolver;
    const ModelGeneratorContext& ctx;
    llvm::Value *modelData;

    /**
     * get the module, only valid whilst a BasicBlock is begin filled.
     *
     * @param fname: name of the calling function, used for generating exception
     * on failure.
     *
     * @returns a module on success, throws exeption on failure.
     */
    llvm::Module *getModule();

    bool scalar_mode_;

    friend class ASTNodeCodeGenScalarTicket;
};

std::string to_string(const libsbml::ASTNode *ast);

class ASTNodeCodeGenScalarTicket {
    public:
        ASTNodeCodeGenScalarTicket(ASTNodeCodeGen& gen, bool val, std::string n = "");

        ~ASTNodeCodeGenScalarTicket();

    private:
        ASTNodeCodeGen& z_;
        bool v_;
        std::string n_;
};

} /* namespace rr */

#endif /* ASTNodeCodeGenH */

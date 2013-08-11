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

namespace libsbml
{
class ASTNode;
}

namespace rr
{
/**
 * All of the LLVM IR generation is handled here.
 */
class ASTNodeCodeGen
{
public:
    ASTNodeCodeGen(llvm::IRBuilder<> &builder,
            LoadSymbolResolver &resolver);
    ~ASTNodeCodeGen();

    llvm::Value *codeGen(const libsbml::ASTNode *ast);

    /**
     * sbml ASTNode does not contain as assigment '=' type, assigment
     * is handled by other sbml elements such as initialAssigment.
     *
     */
    llvm::Value *binaryExprCodeGen(const libsbml::ASTNode *ast);

    /**
     * most of the AST types correspond to llvm intrinsic, i.e.
     * cos, sqrt, pow, etc...
     */
    llvm::Value *intrinsicCallCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *notImplemented(const libsbml::ASTNode *ast);

    llvm::Value *nameExprCodeGen(const libsbml::ASTNode *ast);

    llvm::Value *realExprCodeGen(const libsbml::ASTNode *ast);

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

    llvm::Value *applyRelationalCodeGen(const libsbml::ASTNode *ast);

private:
    llvm::IRBuilder<> &builder;
    LoadSymbolResolver &resolver;
};



} /* namespace rr */



#endif /* ASTNodeCodeGenH */

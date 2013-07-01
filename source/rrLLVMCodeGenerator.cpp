/*
 * rrLLVMCodeGenerator.cpp
 *
 * Created on: Jun 16, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#include "rrLLVMCodeGenerator.h"

#include <sbml/math/ASTNode.h>

#include "rrLLVMIncludes.h"

using namespace libsbml;
using namespace llvm;

namespace rr
{





LLVMCodeGenerator::LLVMCodeGenerator() :
        builder(getGlobalContext())
{
    // TODO Auto-generated constructor stub

}


LLVMCodeGenerator::~LLVMCodeGenerator()
{
    // TODO Auto-generated destructor stub
}

Value *LLVMCodeGenerator::BinaryExprCodegen(const ASTNode *ast)
{
    Value *result = 0;

    Value *lhs = Codegen(ast->getLeftChild());
    Value *rhs = Codegen(ast->getRightChild());
    if (lhs == 0 || rhs == 0)
    {
        // TODO: report this error???
        return 0;
    }

    switch (ast->getType())
    {
    case AST_PLUS:
        result = builder.CreateFAdd(lhs, rhs, "addtmp");
        break;
    case AST_MINUS:
        result = builder.CreateFSub(lhs, rhs, "subtmp");
        break;
    case AST_TIMES:
        result = builder.CreateFMul(lhs, rhs, "multmp");
        break;
    case AST_DIVIDE:
        result = builder.CreateFDiv(lhs, rhs, "divtmp");
        break;
        /*
         case '<':
         L = Builder.CreateFCmpULT(L, R, "cmptmp");
         // Convert bool 0/1 to double 0.0 or 1.0
         return Builder.CreateUIToFP(L, Type::getDoubleTy(getGlobalContext()),
         "booltmp");
         */
    default:
        break;
    }

    return result;
}

llvm::Value* LLVMCodeGenerator::Codegen(const libsbml::ASTNode* ast)
{
    Value *result = 0;
    switch (ast->getType())
    {
    case AST_PLUS:      /* '+' */
    case AST_MINUS:     /* '-' */
    case AST_TIMES:     /* '*' */
    case AST_DIVIDE:    /* '/' */
        result = BinaryExprCodegen(ast);
        break;
    case AST_POWER:         // '^' sbml considers this an operator,
                            // left and right child nodes are the first
                            // 2 child nodes for args.
    case AST_FUNCTION_ABS:
    case AST_FUNCTION_ARCCOS:
    case AST_FUNCTION_ARCCOSH:
    case AST_FUNCTION_ARCCOT:
    case AST_FUNCTION_ARCCOTH:
    case AST_FUNCTION_ARCCSC:
    case AST_FUNCTION_ARCCSCH:
    case AST_FUNCTION_ARCSEC:
    case AST_FUNCTION_ARCSECH:
    case AST_FUNCTION_ARCSIN:
    case AST_FUNCTION_ARCSINH:
    case AST_FUNCTION_ARCTAN:
    case AST_FUNCTION_ARCTANH:
    case AST_FUNCTION_CEILING:
    case AST_FUNCTION_COS:
    case AST_FUNCTION_COSH:
    case AST_FUNCTION_COT:
    case AST_FUNCTION_COTH:
    case AST_FUNCTION_CSC:
    case AST_FUNCTION_CSCH:
    case AST_FUNCTION_DELAY:
    case AST_FUNCTION_EXP:
    case AST_FUNCTION_FACTORIAL:
    case AST_FUNCTION_FLOOR:
    case AST_FUNCTION_LN:
    case AST_FUNCTION_LOG:
    case AST_FUNCTION_POWER:
    case AST_FUNCTION_ROOT:
    case AST_FUNCTION_SEC:
    case AST_FUNCTION_SECH:
    case AST_FUNCTION_SIN:
    case AST_FUNCTION_SINH:
    case AST_FUNCTION_TAN:
    case AST_FUNCTION_TANH:
        result = IntrinsicCallCodegen(ast);
        break;

    }
    return result;
}

llvm::Value* LLVMCodeGenerator::IntrinsicCallCodegen(
        const libsbml::ASTNode* ast)
{
    return 0;
}

} /* namespace rr */

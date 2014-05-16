/*
 * FunctionResolver.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "FunctionResolver.h"
#include "ASTNodeCodeGen.h"
#include "LLVMException.h"
#include "rrStringUtils.h"

namespace rrllvm
{

using namespace libsbml;
using namespace std;
using namespace llvm;

using rr::Logger;
using rr::getLogger;

FunctionResolver::FunctionResolver(LoadSymbolResolver& parentResolver,
        const libsbml::Model* model, llvm::IRBuilder<>& builder) :
                parentResolver(parentResolver),
                model(model),
                builder(builder),
                symbols(0)
{
}


llvm::Value* FunctionResolver::loadSymbolValue(const std::string& symbol,
        const llvm::ArrayRef<llvm::Value*>& args)
{
    const FunctionDefinition *funcDef = 0;
    if (symbols)
    {
        map<string,Value*>::const_iterator i = symbols->find(symbol);
        if (i != symbols->end())
        {
            return i->second;
        }
        else
        {
            return parentResolver.loadSymbolValue(symbol, args);
        }
    }
    else if ((funcDef = model->getListOfFunctionDefinitions()->get(symbol)))
    {
        recursiveSymbolPush(symbol);

        const ASTNode *math = funcDef->getMath();
        const unsigned nchild = math->getNumChildren();

        if (!math->isLambda())
        {
            throw_llvm_exception(symbol + ", math element of function definition must be a lambda");
        }

        // must have at least one trailing apply node
        if (nchild < 1)
        {
            throw_llvm_exception("function node " + symbol + " must have at least one child node, "
                    "this node has " + rr::toString(nchild) +  " child nodes");
        }

        if (nchild - 1 != args.size())
        {
            string expected = rr::toString(nchild - 1);
            string got = rr::toString(args.size());

            throw_llvm_exception(symbol + ", argument count does not match, expected " +
                    expected + ", recieved: " + got);
        }

        symbols = new map<string, Value*>();

        // first set of child nodes are bvars, these go into new local scope
        for (uint i = 0; i < nchild - 1; ++i)
        {
            const ASTNode *c = math->getChild(i);
            assert(c->isBvar());
            (*symbols)[c->getName()] = args[i];
        }

        ASTNodeCodeGen astCodeGen(builder, *this);

        // the last child should be an apply node
        const ASTNode *apply = math->getChild(nchild - 1);
        Value *result = astCodeGen.codeGen(apply);

        delete symbols;
        symbols = 0;

        recursiveSymbolPop();

        return result;
    }

    return 0;
}

void FunctionResolver::recursiveSymbolPush(const std::string& symbol)
{
    parentResolver.recursiveSymbolPush(symbol);
}

void FunctionResolver::recursiveSymbolPop()
{
    parentResolver.recursiveSymbolPop();
}

} /* namespace rr */

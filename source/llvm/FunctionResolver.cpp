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

FunctionResolver::~FunctionResolver()
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
            return parentResolver.loadSymbolValue(symbol);
        }
    }
    else if ((funcDef = model->getListOfFunctionDefinitions()->get(symbol)))
    {
        const ASTNode *math = funcDef->getMath();

        if (!math->isLambda())
        {
            throw_llvm_exception(symbol + ", math elemetn of function definition must be a lambda");
        }

        if (math->getNumChildren() != args.size() + 1)
        {
            string expected = rr::toString(args.size() + 1);
            string got = rr::toString(math->getNumChildren());

            throw_llvm_exception(symbol + ", argument count does not match, expected " +
                    expected + ", recieved: " + got);
        }

        symbols = new map<string, Value*>();

        for (uint i = 0; i < math->getNumChildren() - 1; ++i)
        {
            const ASTNode *c = math->getChild(i);
            assert(c->isBvar());
            (*symbols)[c->getName()] = args[i];
        }

        ASTNodeCodeGen astCodeGen(builder, *this);
        Value *result = astCodeGen.codeGen(math->getChild(math->getNumChildren() - 1));
        delete symbols;
        symbols = 0;
        return result;
    }

    return 0;
}

} /* namespace rr */

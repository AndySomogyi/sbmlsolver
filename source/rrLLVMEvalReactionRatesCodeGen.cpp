/*
 * rrLLVMEvalReactionRatesCodeGen.cpp
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#include "rrLLVMEvalReactionRatesCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataSymbolResolver.h"
#include "rrLogger.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>
#include <Poco/Logger.h>


using namespace libsbml;
using namespace llvm;
using namespace std;


namespace rr
{

const char* LLVMEvalReactionRatesCodeGen::FunctionName = "evalReactionRates";

LLVMEvalReactionRatesCodeGen::LLVMEvalReactionRatesCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase<LLVMEvalReactionRates_FunctionPtr>(mgc)
{
}

LLVMEvalReactionRatesCodeGen::~LLVMEvalReactionRatesCodeGen()
{
}

Value* LLVMEvalReactionRatesCodeGen::codeGen()
{
    Value *modelData = 0;
    codeGenVoidModelDataHeader(FunctionName, modelData);

    LLVMModelDataLoadSymbolResolver resolver(modelData,model,modelSymbols,
            dataSymbols,builder);
    LLVMModelDataIRBuilder mdbuilder(modelData, dataSymbols, builder);
    LLVMASTNodeCodeGen astCodeGen(builder, resolver);
    LLVMASTNodeFactory nodes;

    // iterate through all of the reaction, and generate code based on thier
    // kinetic rules.

    // currently, our AST does not support assigments, so we do it
    // here.

    const ListOfReactions *reactions = model->getListOfReactions();

    for (int i = 0; i < reactions->size(); ++i)
    {
        const Reaction *r = reactions->get(i);
        const KineticLaw *kinetic = r->getKineticLaw();
        const ASTNode *math = 0;
        Value *value = 0;
        if (kinetic)
        {
            math = kinetic->getMath();
        }
        else
        {
            poco_warning(getLogger(), "Reaction " + r->getId() + " has no KineticLaw, it will be set to zero");
            ASTNode *m = nodes.create(AST_REAL);
            m->setValue(0);
        }
        value = astCodeGen.codeGen(math);
        mdbuilder.createReactionRateStore(r->getId(), value);
    }

    builder.CreateRetVoid();

    return verifyFunction();
}


} /* namespace rr */

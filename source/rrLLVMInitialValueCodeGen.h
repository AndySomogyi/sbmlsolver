/*
 * rrLLVMInitialValueCodeGen.h
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#ifndef RRLLVMINITIALVALUECODEGEN_H_
#define RRLLVMINITIALVALUECODEGEN_H_

#include "rrLLVMModelGeneratorContext.h"
#include <sbml/SBMLVisitor.h>




namespace rr
{

using libsbml::SBMLVisitor;
using llvm::Function;

class LLVMInitialValueCodeGen : public libsbml::SBMLVisitor
{
public:
    LLVMInitialValueCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMInitialValueCodeGen();

    Function *CodeGen();

};

} /* namespace rr */
#endif /* RRLLVMINITIALVALUECODEGEN_H_ */

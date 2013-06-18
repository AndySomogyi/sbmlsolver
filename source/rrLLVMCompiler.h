/*
 * rrLLVMCompiler.h
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef rrLLVMCompilerH
#define rrLLVMCompilerH

#include <rrCompiler.h>

namespace rr
{

class LLVMCompiler: public rr::Compiler
{
public:
    LLVMCompiler();
    virtual ~LLVMCompiler();
};

} /* namespace rr */
#endif /* rrLLVMCompilerH */

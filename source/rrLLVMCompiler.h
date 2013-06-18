/*
 * rrLLVMCompiler.h
 *
 *  Created on: Jun 3, 2013
 *      Author: Andy Somogyi, andy.somogyi@gmail.com
 */

#ifndef RRLLVMCOMPILER_H_
#define RRLLVMCOMPILER_H_

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
#endif /* RRLLVMCOMPILER_H_ */

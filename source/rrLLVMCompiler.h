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

#include "rrCompiler.h"

namespace rr
{

class RR_DECLSPEC LLVMCompiler: public rr::Compiler
{
public:
    LLVMCompiler();
    virtual ~LLVMCompiler();

    virtual std::string getCompiler() const;
    virtual bool setCompiler(const std::string& compiler);

    virtual std::string getCompilerLocation() const;
    virtual bool setCompilerLocation(const std::string& path);

    virtual std::string getSupportCodeFolder() const;
    virtual bool setSupportCodeFolder(const std::string& path);
};

} /* namespace rr */
#endif /* rrLLVMCompilerH */

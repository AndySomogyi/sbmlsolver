/*
 * LLVMCompiler.h
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef LLVMCompilerH
#define LLVMCompilerH

#include "rrCompiler.h"

namespace rrllvm
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

    /**
     * return a random string from the gurgle file.
     *
     * All these file paths are useless to LLVM, so make some use out
     * of knowing them...
     */
    static std::string gurgle();
};

} /* namespace rr */
#endif /* LLVMCompilerH */

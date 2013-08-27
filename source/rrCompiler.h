/*
 * rrCompiler.h
 *
 *  Author: Andy Somogyi
 *  Created on: Jul 15, 2013
 */

#ifndef rrCompiler_H
#define rrCompiler_H

#include "rrOSSpecifics.h"
#include <string>

namespace rr
{

/**
 * interface to manipulate 'compiler' settings.
 *
 * In the C generating model sytem, an external C compiler is used to compile
 * generated C code. However, in the LLVM system, there is no code generation
 * nor compilation, so this interface is not really usefull, but is implemented
 * for compatiblity with the C system.
 */
class RR_DECLSPEC Compiler
{
public:
    virtual std::string getCompiler() const = 0;
    virtual bool setCompiler(const std::string& compiler) = 0;

    virtual std::string getCompilerLocation() const = 0;
    virtual bool setCompilerLocation(const std::string& path) = 0;

    virtual std::string getSupportCodeFolder() const = 0;
    virtual bool setSupportCodeFolder(const std::string& path) = 0;

protected:
    virtual ~Compiler() {};
};

} /* namespace rr */
#endif /* rrCompiler_H */

/*
 * rrLLVMModelGenerator.h
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */
#ifndef rrLLVMModelGeneratorH
#define rrLLVMModelGeneratorH

#include "rrModelGenerator.h"
#include "rrLLVMCompiler.h"

namespace rr
{

/**
 * General concepts:
 *
 * SBML defines chemical species.
 *
 * Floating Species: species whose value varies over time, i.e. they can 'float'
 * Boundary Species: boundary condition species, values are fixed to initial conditions.
 */
class RR_DECLSPEC LLVMModelGenerator: public rr::ModelGenerator
{
public:
    LLVMModelGenerator();
    virtual ~LLVMModelGenerator();

    /**
     * certain model generators, such as the compiler based ones
     * generate files such as shared libraries. This specifies the
     * location where they are stored.
     */
    virtual bool setTemporaryDirectory(const string& path);

    /**
     * certain model generators, such as the compiler based ones
     * generate files such as shared libraries. This specifies the
     * location where they are stored.
     */
    virtual string getTemporaryDirectory();

    /**
     * Create an executable model from an sbml string
     *
     * The LibStructural and NOMSupport are legacy C generator classes that are
     * completly ignored.
     */
    virtual ExecutableModel *createModel(const string& sbml, ls::LibStructural *,
            bool forceReCompile, bool computeAndAssignConsevationLaws);

    /**
     * Get the compiler object that the model generator is using to
     * 'compile' sbml. Certain model generators may be interpreters, in this
     * case, the Compiler interface should still be sufficiently general to
     * manipulate interpreters as well.
     */
    virtual Compiler *getCompiler();

    /**
     * No effect on LLVM generator.
     */
    virtual bool setCompiler(const string& compiler);


private:
    LLVMCompiler compiler;

};

} /* namespace rr */
#endif /* rrLLVMModelGeneratorH */

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

#include <rrModelGenerator.h>

namespace rr
{

/**
 * General concepts:
 * test3
 * SBML defines chemical species.
 *
 * Floating Species: species whose value varies over time, i.e. they can 'float'
 * Boundary Species: boundary condition species, values are fixed to initial conditions.
 */
class LLVMModelGenerator: public rr::ModelGenerator
{
public:
    LLVMModelGenerator();
    virtual ~LLVMModelGenerator();

    /**
     * certain model generators, such as the compiler based ones
     * generate files such as shared libraries. This specifies the
     * location where they are stored.
     */
    virtual bool                        setTemporaryDirectory(const string& path);

    /**
     * certain model generators, such as the compiler based ones
     * generate files such as shared libraries. This specifies the
     * location where they are stored.
     */
    virtual string                      getTemporaryDirectory();

    /**
     * Create an executable model from an sbml string, a LibStructural and a NOMSupport.
     * The libstruct and nom objects must already have the sbml loaded into them.
     *
     * For the time being, this sets up a bunch of ivars, such as mLibStruct and mNOM,
     * and in order to preserve compatibility, thise will remain pointing to whatever
     * was passed in.
     * Eventually these ivars will either go away or will be cleared. The ModelGenerator
     * is intended ONLY to make models, not query NOM info.
     */
    virtual ExecutableModel             *createModel(const string& sbml, LibStructural *ls, NOMSupport *nom,
            bool forceReCompile, bool computeAndAssignConsevationLaws);

    /**
     * Get the compiler object that the model generator is using to
     * 'compile' sbml. Certain model generators may be interpreters, in this
     * case, the Compiler interface should still be sufficiently general to
     * manipulate interpreters as well.
     *
     * TODO: Make Compiler an interface.
     */
    virtual                             Compiler *getCompiler();

    /**
     * Set the name of the compiler to use. In the case of source code generating
     * model generators, this is the exectuable name of the external compiler, i.e.
     * 'gcc', 'icc', etc... For JITing generators, this may have no effect.
     */
    virtual                             bool setCompiler(const string& compiler);


};

} /* namespace rr */
#endif /* rrLLVMModelGeneratorH */

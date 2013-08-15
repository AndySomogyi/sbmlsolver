#ifndef rrModelGeneratorH
#define rrModelGeneratorH
#include <string>
#include <list>
#include <vector>
#include "rrExecutableModel.h"


namespace rr
{
class Compiler;

/**
 * The interface which generates executable models from sbml source.
 * This can have different concrete implementations such as compiler
 * based generators, JIT'ed or interpreter based ones.
 */
class RR_DECLSPEC ModelGenerator
{
public:
    enum ModelGeneratorOptions
    {
        /** currently only implemented with the C version */
        ComputeAndAssignConsevationLaws = (0x1 << 0), // => 0x00000001

        /** C version specific, forces the model to be re-compiled */
        ForceReCompile                  = (0x1 << 1), // => 0x00000010
    };

    /**
     * certain model generators, such as the compiler based ones
     * generate files such as shared libraries. This specifies the
     * location where they are stored.
     */
    virtual bool setTemporaryDirectory(const std::string& path) = 0;

    /**
     * certain model generators, such as the compiler based ones
     * generate files such as shared libraries. This specifies the
     * location where they are stored.
     */
    virtual std::string getTemporaryDirectory() = 0;

    /**
     * Create an executable model from an sbml string.
     *
     * @param sbml a string containging the contents of an sbml doc.
     * @param options a bitfield containing items from the ModelGeneratorOptions
     * enumeration.
     */
    virtual ExecutableModel *createModel(const std::string& sbml,
            unsigned int options) = 0;

    /**
     * Get the compiler object that the model generator is using to
     * 'compile' sbml. Certain model generators may be interpreters, in this
     * case, the Compiler interface should still be sufficiently general to
     * manipulate interpreters as well.
     */
    virtual Compiler *getCompiler() = 0;

    /**
     * Set the name of the compiler to use. In the case of source code generating
     * model generators, this is the exectuable name of the external compiler, i.e.
     * 'gcc', 'icc', etc... For JITing generators, this may have no effect.
     */
    virtual bool setCompiler(const std::string& compiler) = 0;

    /**
     * public dtor, one can and most certainly delete an object of this class.
     */
    virtual ~ModelGenerator() {};

protected:
    ModelGenerator() {};

};
}

#endif

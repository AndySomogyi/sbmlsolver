#ifndef rrModelGeneratorH
#define rrModelGeneratorH
#include <string>
#include <vector>
#include <list>
#include "rrObject.h"
#include "rrStringList.h"
#include "rrSymbolList.h"
#include "rrCodeBuilder.h"
#include "rrNOMSupport.h"
#include "rrScanner.h"
#include "rrExecutableModel.h"
#include "rrModelSymbols.h"
#include "rr-libstruct/lsMatrix.h"
#include "rr-libstruct/lsLibStructural.h"

using std::string;
using std::vector;
using std::list;

namespace ls {
class LibStructural;
}

namespace rr
{
class Compiler;
class NOMSupport;


/**
 * The interface which generates executable models from sbml source.
 * This can have different concrete implementations such as compiler
 * based generators, JIT'ed or interpreter based ones.
 */
class RR_DECLSPEC ModelGenerator
{
public:
    ModelGenerator();

    /**
     * certain model generators, such as the compiler based ones
     * generate files such as shared libraries. This specifies the
     * location where they are stored.
     */
    virtual bool                        setTemporaryDirectory(const string& path) = 0;

    /**
     * certain model generators, such as the compiler based ones
     * generate files such as shared libraries. This specifies the
     * location where they are stored.
     */
    virtual string                      getTemporaryDirectory() = 0;

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
    virtual ExecutableModel *createModel(const string& sbml, ls::LibStructural *ls,
            NOMSupport *nom, bool forceReCompile,
            bool computeAndAssignConsevationLaws) = 0;

    /**
     * Get the compiler object that the model generator is using to
     * 'compile' sbml. Certain model generators may be interpreters, in this
     * case, the Compiler interface should still be sufficiently general to
     * manipulate interpreters as well.
     */
    virtual                             Compiler *getCompiler() = 0;

    /**
     * Set the name of the compiler to use. In the case of source code generating
     * model generators, this is the exectuable name of the external compiler, i.e.
     * 'gcc', 'icc', etc... For JITing generators, this may have no effect.
     */
    virtual                             bool setCompiler(const string& compiler) = 0;

    /**
     * public dtor, one can and most certainly delete an object of this class.
     */
    virtual                             ~ModelGenerator();

};
}

#endif

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
#include "rr-libstruct/lsMatrix.h"
#include "rr-libstruct/lsLibStructural.h"

using std::string;
using std::vector;
using std::list;
using namespace ls;
namespace rr
{
class Compiler;

/**
 * The interface which generates executable models from sbml source.
 * This can have different concrete implementations such as compiler
 * based generators, JIT'ed or interpreter based ones.
 */
class RR_DECLSPEC ModelGenerator : public rrObject
{
public:

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
    virtual ExecutableModel             *createModel(const string& sbml, LibStructural *ls, NOMSupport *nom,
            bool forceReCompile, bool computeAndAssignConsevationLaws) = 0;

    /**
     * Get the compiler object that the model generator is using to
     * 'compile' sbml. Certain model generators may be interpreters, in this
     * case, the Compiler interface should still be sufficiently general to
     * manipulate interpreters as well.
     *
     * TODO: Make Compiler an interface.
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

protected:

    void                                reset();

    int                                 numAdditionalRates();        //this variable is the size of moMapRateRule

    /**
     * Refernce to libstruct library
     * this are set by createModel, and for the time being remain after createModel
     * completes.
     */
    LibStructural*                      mLibStruct;

    /**
     * Object that provide some wrappers and new "NOM" functions.
     * this are set by createModel, and for the time being remain after createModel
     * completes.
     */
    NOMSupport*                         mNOM;

    int                                 mNumModifiableSpeciesReferences;

    SymbolList                          mBoundarySpeciesList;
    SymbolList                          mCompartmentList;
    SymbolList                          mConservationList;
    SymbolList                          mFloatingSpeciesAmountsList;
    SymbolList                          mFloatingSpeciesConcentrationList;
    SymbolList                          mGlobalParameterList;
    SymbolList                          mReactionList;
    IntStringHashTable                  mMapRateRule;
    int                                 mNumBoundarySpecies;
    int                                 mNumCompartments;
    int                                 mNumDependentSpecies;
    int                                 mNumEvents;
    int                                 mNumFloatingSpecies;
    int                                 mNumGlobalParameters;
    int                                 mNumIndependentSpecies;
    int                                 mNumReactions;
    int                                 mTotalLocalParmeters;

    /**
     * protected ctor, this is an partially abstract class.
     */
    ModelGenerator();

    bool                                mComputeAndAssignConsevationLaws;
    const string                        mDoubleFormat;
    const string                        mFixAmountCompartments;
    vector<int>                         mLocalParameterDimensions;
    string                              mModelName;

    StringList                          mFunctionNames;
    StringList                          mFunctionParameters;
    StringList                          mDependentSpeciesList;
    StringList                          mIndependentSpeciesList;

    vector<SymbolList>                  mLocalParameterList;

    StringList                          mWarnings;

    /**
     * get various information about the model in a user displayable format.
     */
    virtual string                      getInfo();



    //Non virtuals..


    /**
     * creates a new AST node
     */
    static ASTNode*                     cleanEquation(ASTNode* ast);
    static string                       cleanEquation(const string& equation);

    ls::DoubleMatrix*                   initializeL0(int& nrRows, int& nrCols);
    bool                                expressionContainsSymbol(ASTNode* ast, const string& symbol);
    bool                                expressionContainsSymbol(const string& expression, const string& symbol);
    Symbol*                             getSpecies(const string& id);
    int                                 readGlobalParameters();
    void                                readLocalParameters(const int& numReactions,  vector<int>& localParameterDimensions, int& totalLocalParmeters);
    int                                 readCompartments();
    int                                 readModifiableSpeciesReferences();
    SymbolList                          mModifiableSpeciesReferenceList;

    string                              writeDouble(const double& value, const string& format = "%G");

    // for the time being, we'll store the state vars here until
    // we move them to the model, but we have cleaned up the interface,
    // so ONLY this executableModel access the state vars.
    friend class CompiledExecutableModel;
};
}

#endif

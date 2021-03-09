#ifndef CModelGenerator_H_
#define CModelGenerator_H_
//---------------------------------------------------------------------------

#include "rrCompiledModelGenerator.h"
#include "rrModelSharedLibrary.h"
#include "rrCodeBuilder.h"
#include "rrCCompiler.h"

namespace rr
{

using Poco::Mutex;

/**
 * Generate executable SBML models by generating and compiling C
 * source code into shared libraries with an external C compiler.
 */
class RR_DECLSPEC CModelGenerator : public CompiledModelGenerator
{

public:
    CModelGenerator(const std::string& tempFolder, const std::string& supportCodeFolder, const std::string& compiler);
    virtual                            ~CModelGenerator();

    virtual  ExecutableModel            *createModel(const std::string& sbml, uint options);

    virtual bool                        setTemporaryDirectory(const std::string& path);

    /**
     * Get the location where this model generator creates source file and shared libraries.
     */
    virtual std::string                      getTemporaryDirectory();

    /**
     * Get the compiler object that the model generator is using to
     * 'compile' sbml.
     */
    virtual                             Compiler* getCompiler();

    /**
     * Set the name of the compiler to use. As this is a C source code compiler, this
     * is the name of the external C compiler, which would typically be 'gcc', 'cc', 'icc', etc...
     */
    virtual                             bool setCompiler(const std::string& compiler);

    /**
     * These are access in the c_api, should they be??? TODO: fix this.
     */
    std::string                              getHeaderCode();
    std::string                              getSourceCode();
    std::string                              getSourceCodeFileName();

private:
    /**
     * create a C compiled executable model.
     *
     * The caller own this.
     */
    ExecutableModel                     *createModel(const std::string& sbml, ls::LibStructural *ls,
                                                     bool forceReCompile, bool computeAndAssignConsevationLaws);

    CodeBuilder                         mHeader;
    CodeBuilder                         mSource;

    std::string                              mHeaderCodeFileName;
    std::string                              mSourceCodeFileName;

    std::string                              mCurrentSBML;

    // the shared lib thats created.
    // this needs to belong to the model
    // TODO: fix this!
    ModelSharedLibrary                  *mModelLib;

    CCompiler                           mCompiler;

    std::string                              mTempFileFolder;

    bool generateModelCode(const std::string& sbml, const std::string& modelName, bool computeAndAssignConsevationLaws);

    static Mutex                        mCompileMutex;

    bool compileModel();

    bool compileCurrentModel();



    //C Specifics..

    std::string                              getHeaderCodeFileName();
    bool                                saveSourceCodeToFolder(const std::string& folder, const std::string& baseName);
    int                                 getNumberOfFloatingSpecies();


    std::string                              convertUserFunctionExpression(const std::string& equation);
    std::string                              convertCompartmentToC(const std::string& compartmentName);
    std::string                              convertSpeciesToBc(const std::string& speciesName);
    std::string                              convertSpeciesToY(const std::string& speciesName);

    /**
     * creates a std::string of the format "md->c[comp_index]", where
     * comp_index is the index of the named compartment
     */
    std::string                              convertSymbolToC(const std::string& compartmentName);
    std::string                              convertSymbolToGP(const std::string& parameterName);

    void                                substituteEquation(const std::string& reactionName, Scanner& s, CodeBuilder& sb);
    void                                substituteWords(const std::string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb);
    void                                substituteToken(const std::string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb);
    std::string                              findSymbol(const std::string& varName);
    void                                write_getModelNameFunction(CodeBuilder& ignore, CodeBuilder& source);
    void                                writeComputeAllRatesOfChange(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0);
    void                                writeComputeConservedTotals(CodeBuilder& sb, const int& numFloatingSpecies, const int& numDependentSpecies);
    void                                writeUpdateDependentSpecies(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0);
    void                                writeUserDefinedFunctions(CodeBuilder& sb);
    void                                writeResetEvents(CodeBuilder& sb, const int& numEvents);
    void                                writeSetConcentration(CodeBuilder& sb);
    void                                writeGetConcentration(CodeBuilder& sb);
    void                                writeConvertToAmounts(CodeBuilder& sb);
    void                                writeConvertToConcentrations(CodeBuilder& sb);
    void                                writeProperties(CodeBuilder& sb);
    void                                writeAccessors(CodeBuilder& sb);
    void                                writeOutVariables(CodeBuilder& sb);
    void                                writeClassHeader(CodeBuilder& sb);
    void                                writeTestConstraints(CodeBuilder& sb);
    void                                writeEvalInitialAssignments(CodeBuilder& sb, const int& numReactions);
    int                                 writeComputeRules(CodeBuilder& sb, const int& numReactions);
    void                                writeComputeReactionRates(CodeBuilder& sb, const int& numReactions);
    void                                writeEvalEvents(CodeBuilder& sb, const int& numEvents, const int& numFloatingSpecies);
    void                                writeEvalModel(CodeBuilder& sb, const int& numReactions, const int& numIndependentSpecies, const int& numFloatingSpecies, const int& numOfRules);
    void                                writeEventAssignments(CodeBuilder& sb, const int& numReactions, const int& numEvents);
    void                                writeSetParameterValues(CodeBuilder& sb, const int& numReactions);
    void                                writeSetCompartmentVolumes(CodeBuilder& sb);
    void                                writeSetBoundaryConditions(CodeBuilder& sb);
    void                                writeSetInitialConditions(CodeBuilder& sb, const int& numFloatingSpecies);
    void                                writeInitFunction(CodeBuilder& sbh, CodeBuilder& sbc);
    void                                writeInitModelDataFunction(CodeBuilder& sbh, CodeBuilder& sbc);

    int                                 readFloatingSpecies();
    // int                                 readBoundarySpecies();

    // Generates the Model Code from the SBML std::string
    // TODO major clean up
    std::string                              generateModelCode(const std::string& sbmlStr, const bool& _computeAndAssignConsevationLaws);

    // Generates the Model Code from th e SBML std::string
    // TODO major clean up also
    std::string                              generateModelCode(const std::string& sbmlStr, LibStructural *ls, NOMSupport *nom,
                                                          const bool& _computeAndAssignConsevationLaws = false);

    static bool                         loadSBMLIntoNOM(NOMSupport &nom, const std::string& sbml);

};


}

#endif

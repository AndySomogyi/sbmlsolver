#ifndef rrCSharpGeneratorH
#define rrCSharpGeneratorH
//---------------------------------------------------------------------------
#include "rrCompiledModelGenerator.h"

namespace rr
{

class RR_DECLSPEC CSharpModelGenerator : public CompiledModelGenerator
{
    protected:
        string                              mSourceCodeFileName;
        CodeBuilder                            mSource;
        string                              convertUserFunctionExpression(const string& equation);
        string                              convertCompartmentToC(const string& compartmentName);
        string                              convertSpeciesToBc(const string& speciesName);
        string                              convertSpeciesToY(const string& speciesName);
        string                              convertSymbolToC(const string& compartmentName);
        string                              convertSymbolToGP(const string& parameterName);

        void                                substituteEquation(const string& reactionName, Scanner& s, CodeBuilder& sb);
        void                                substituteWords(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb);
        void                                substituteToken(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb);
        string                              findSymbol(const string& varName);
        void                                writeOutSymbolTables(CodeBuilder& sb);
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


    public:
                                            CSharpModelGenerator(LibStructural *ls, NOMSupport *nom);
        virtual                             ~CSharpModelGenerator();

        // Generates the Model Code from the SBML string
        string                              generateModelCode(const string& sbmlStr, const bool& computeAndAssignConsevationLaws = false);
        bool                                saveSourceCodeToFolder(const string& folder, const string& baseName);
        string                              getSourceCode();

        virtual ExecutableModel *createModel(const string& sbml, ls::LibStructural *ls,
                bool forceReCompile, bool computeAndAssignConsevationLaws);

        virtual ExecutableModel *createModel(const string& sbml,
                bool computeAndAssignConsevationLaws);


        virtual bool                        setTemporaryDirectory(const string& path);

        /**
         * Get the location where this model generator creates source file and shared libraries.
         */
        virtual string                      getTemporaryDirectory();

        /**
         * Get the compiler object that the model generator is using to
         * 'compile' sbml.
         *
         * TODO: Make Compiler an interface.
         */
        virtual                             Compiler* getCompiler();

        /**
         * Set the name of the compiler to use. For C# model generators,
         * could this be 'gmcs'? Presumably, on Windows, this would be 'mcs'.
         */
        virtual                             bool setCompiler(const string& compiler);

};

}
#endif

#ifndef rrCGeneratorH
#define rrCGeneratorH
//---------------------------------------------------------------------------
#include "rrModelGenerator.h"
#include "rrCodeBuilder.h"
namespace rr
{


class RR_DECLSPEC CGenerator : public ModelGenerator
{
    protected:
        CodeBuilder                         mHeader;
        CodeBuilder                         mSource;

        string                              mHeaderCodeFileName;
        string                              mSourceCodeFileName;

        string                              convertUserFunctionExpression(const string& equation);
        string                              convertCompartmentToC(const string& compartmentName);
        string                              convertSpeciesToBc(const string& speciesName);
        string                              convertSpeciesToY(const string& speciesName);
        string                              convertSymbolToC(const string& compartmentName);
        string                              convertSymbolToGP(const string& parameterName);

        void                                SubstituteEquation(const string& reactionName, Scanner& s, CodeBuilder& sb);
        void                                SubstituteWords(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb);
        void                                SubstituteToken(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb);
        string                              FindSymbol(const string& varName);
        void                                WriteOutSymbolTables(CodeBuilder& sb);
        void                                WriteComputeAllRatesOfChange(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0);
        void                                WriteComputeConservedTotals(CodeBuilder& sb, const int& numFloatingSpecies, const int& numDependentSpecies);
        void                                WriteUpdateDependentSpecies(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0);
        void                                WriteUserDefinedFunctions(CodeBuilder& sb);
        void                                WriteResetEvents(CodeBuilder& sb, const int& numEvents);
        void                                WriteSetConcentration(CodeBuilder& sb);
        void                                WriteGetConcentration(CodeBuilder& sb);
        void                                WriteConvertToAmounts(CodeBuilder& sb);
        void                                WriteConvertToConcentrations(CodeBuilder& sb);
        void                                WriteProperties(CodeBuilder& sb);
        void                                WriteAccessors(CodeBuilder& sb);
        void                                WriteOutVariables(CodeBuilder& sb);
        void                                WriteClassHeader(CodeBuilder& sb);
        void                                WriteTestConstraints(CodeBuilder& sb);
        void                                WriteEvalInitialAssignments(CodeBuilder& sb, const int& numReactions);
        int                                 WriteComputeRules(CodeBuilder& sb, const int& numReactions);
        void                                WriteComputeReactionRates(CodeBuilder& sb, const int& numReactions);
        void                                WriteEvalEvents(CodeBuilder& sb, const int& numEvents, const int& numFloatingSpecies);
        void                                WriteEvalModel(CodeBuilder& sb, const int& numReactions, const int& numIndependentSpecies, const int& numFloatingSpecies, const int& numOfRules);
        void                                WriteEventAssignments(CodeBuilder& sb, const int& numReactions, const int& numEvents);
        void                                WriteSetParameterValues(CodeBuilder& sb, const int& numReactions);
        void                                WriteSetCompartmentVolumes(CodeBuilder& sb);
        void                                WriteSetBoundaryConditions(CodeBuilder& sb);
        void                                WriteSetInitialConditions(CodeBuilder& sb, const int& numFloatingSpecies);
        int                                 ReadFloatingSpecies();
        int                                 ReadBoundarySpecies();

    public:
//                                            CGenerator(RoadRunner* rr);
                                            CGenerator(NOMSupport& nom);
        virtual                             ~CGenerator();

        // Generates the Model Code from th e SBML string
        string                              generateModelCode(const string& sbmlStr, const bool& _computeAndAssignConsevationLaws = false);

        //C Specifics..
        void                                WriteInitFunction(CodeBuilder& sbh, CodeBuilder& sbc);
        string                              GetHeaderCode();
        string                              GetSourceCode();
        string                              GetSourceCodeFileName();
        string                              GetHeaderCodeFileName();
        bool                                SaveSourceCodeToFolder(const string& folder);
        int                                 GetNumberOfFloatingSpecies();

};
}

#endif

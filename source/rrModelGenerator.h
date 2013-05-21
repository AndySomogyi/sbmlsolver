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
#include "rr-libstruct/lsMatrix.h"
#include "rr-libstruct/lsLibStructural.h"

using std::string;
using std::vector;
using std::list;
using namespace ls;
namespace rr
{
class RoadRunner;

class RR_DECLSPEC ModelGenerator : public rrObject
{
    public:
        bool								mComputeAndAssignConsevationLaws;
        const string                        mDoubleFormat;
        const string                        mFixAmountCompartments;
        vector<int>                         mLocalParameterDimensions;
        string                              mModelName;
        int                                 mNumBoundarySpecies;
        int                                 mNumCompartments;
        int                                 mNumDependentSpecies;
        int                                 mNumEvents;
        int                                 mNumFloatingSpecies;
        int                                 mNumGlobalParameters;
        int                                 mNumIndependentSpecies;
        int                                 mNumReactions;

        int                                 mTotalLocalParmeters;
        StringList                          mFunctionNames;
        StringList                          mFunctionParameters;
        StringList                          mDependentSpeciesList;
        StringList                          mIndependentSpeciesList;
        int                                 mNumModifiableSpeciesReferences;
		LibStructural&                      mLibStruct;                          //Refernce to libstruct library
        NOMSupport&                         mNOM;                                //Object that provide some wrappers and new "NOM" functions.
        IntStringHashTable                  mMapRateRule;
        SymbolList                          mBoundarySpeciesList;
        SymbolList                          mCompartmentList;
        SymbolList                          mConservationList;
        SymbolList                          mFloatingSpeciesAmountsList;
        SymbolList                          mFloatingSpeciesConcentrationList;
        SymbolList                          mGlobalParameterList;
        vector<SymbolList>                  mLocalParameterList;
        SymbolList                          mReactionList;
        StringList                          mWarnings;

        //Pure Virtual functions... =====================================
        virtual string                      convertUserFunctionExpression(const string& equation) = 0;
        virtual void                        substituteEquation(const string& reactionName, Scanner& s, CodeBuilder& sb) = 0;
        virtual void                        substituteWords(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb) = 0;
        virtual void                        substituteToken(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb) = 0;
        virtual string                      findSymbol(const string& varName) = 0;
        virtual int                         readFloatingSpecies() = 0;
        virtual int                         readBoundarySpecies() = 0;
        virtual void                        writeOutSymbolTables(CodeBuilder& sb) = 0;
        virtual void                        writeComputeAllRatesOfChange(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0) = 0;
        virtual void                        writeComputeConservedTotals(CodeBuilder& sb, const int& numFloatingSpecies, const int& numDependentSpecies) = 0;
        virtual void                        writeUpdateDependentSpecies(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0) = 0;
        virtual void                        writeUserDefinedFunctions(CodeBuilder& sb) = 0;
        virtual void                        writeResetEvents(CodeBuilder& sb, const int& numEvents) = 0;
        virtual void                        writeSetConcentration(CodeBuilder& sb) = 0;
        virtual void                        writeGetConcentration(CodeBuilder& sb) = 0;
        virtual void                        writeConvertToAmounts(CodeBuilder& sb) = 0;
        virtual void                        writeConvertToConcentrations(CodeBuilder& sb) = 0;
        virtual void                        writeProperties(CodeBuilder& sb) = 0;
        virtual void                        writeAccessors(CodeBuilder& sb) = 0;
        virtual void                        writeOutVariables(CodeBuilder& sb) = 0;
        virtual void                        writeClassHeader(CodeBuilder& sb) = 0;
        virtual void                        writeTestConstraints(CodeBuilder& sb) = 0;
        virtual void                        writeEvalInitialAssignments(CodeBuilder& sb, const int& numReactions) = 0;
        virtual int                         writeComputeRules(CodeBuilder& sb, const int& numReactions) = 0;
        virtual void                        writeComputeReactionRates(CodeBuilder& sb, const int& numReactions) = 0;
        virtual void                        writeEvalEvents(CodeBuilder& sb, const int& numEvents, const int& numFloatingSpecies) = 0;
        virtual void                        writeEvalModel(CodeBuilder& sb, const int& numReactions, const int& numIndependentSpecies, const int& numFloatingSpecies, const int& numOfRules) = 0;
        virtual void                        writeEventAssignments(CodeBuilder& sb, const int& numReactions, const int& numEvents) = 0;
        virtual void                        writeSetParameterValues(CodeBuilder& sb, const int& numReactions) = 0;
        virtual void                        writeSetCompartmentVolumes(CodeBuilder& sb) = 0;
        virtual void                        writeSetBoundaryConditions(CodeBuilder& sb) = 0;
        virtual void                        writeSetInitialConditions(CodeBuilder& sb, const int& numFloatingSpecies) = 0;
        virtual string                      convertCompartmentToC(const string& compartmentName) = 0;
        virtual string                      convertSpeciesToBc(const string& speciesName) = 0;
        virtual string                      convertSpeciesToY(const string& speciesName) = 0;
        virtual string                      convertSymbolToC(const string& compartmentName) = 0;
        virtual string                      convertSymbolToGP(const string& parameterName) = 0;

		//Non virtuals..
        string                              substituteTerms(const int& numReactions, const string& reactionName, const string& equation);
        ASTNode*                            cleanEquation(ASTNode* ast);
        string                              cleanEquation(const string& equation);
        string                              substituteTerms(const string& reactionName, const string& inputEquation, bool bFixAmounts);
        ls::DoubleMatrix*               	initializeL0(int& nrRows, int& nrCols);
        bool                                expressionContainsSymbol(ASTNode* ast, const string& symbol);
        bool                                expressionContainsSymbol(const string& expression, const string& symbol);
        Symbol*                             getSpecies(const string& id);
        int                                 readGlobalParameters();
        void                                readLocalParameters(const int& numReactions,  vector<int>& localParameterDimensions, int& totalLocalParmeters);
        int                                 readCompartments();
        int                                 readModifiableSpeciesReferences();
        SymbolList                          mModifiableSpeciesReferenceList;

    public:
                                            ModelGenerator(LibStructural& ls, NOMSupport& nom);
        virtual                             ~ModelGenerator();
        void                                reset();
        int                                 getNumberOfReactions();
        int                                 numAdditionalRates();        //this variable is the size of moMapRateRule

        StringList                          getCompartmentList();
        StringList                          getConservationList();

        StringList                          getGlobalParameterList();
        StringList                          getLocalParameterList(int reactionId);

        StringList                          getReactionIds();
        SymbolList&                         getReactionListReference();

        StringList                          getFloatingSpeciesConcentrationList();    //Just returns the Ids...!
        SymbolList&                         getFloatingSpeciesConcentrationListReference();

        StringList                          getBoundarySpeciesList();
        SymbolList&                         getBoundarySpeciesListReference();
        SymbolList&                         getGlobalParameterListReference();
        SymbolList&                         getConservationListReference();
        string                              writeDouble(const double& value, const string& format = "%G");

        // Generates the Model Code from theSBML string
        virtual string                      generateModelCode(const string& sbmlStr, const bool& _computeAndAssignConsevationLaws) = 0;    //Any decendant need to implement at least this one
        virtual bool                     	saveSourceCodeToFolder(const string& folder, const string& codeBaseName);
//        void                                SetXMLModelFileName(const string& name);
};
}

#endif

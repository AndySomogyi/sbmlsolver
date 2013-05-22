#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iostream>
#include <cmath>
#include <stack>
#include "sbml/SBMLDocument.h"
#include "rrModelGenerator.h"
#include "rrLogger.h"
#include "rrStringUtils.h"
#include "rrException.h"
//---------------------------------------------------------------------------

// check to see that these prickly variables are valid before using them.
// this will go away as soon as these vars are no longer kept around.
#define CHECK_LIB_NOM()  \
{ \
    if (mNOM == 0 || mLibStruct == 0) \
    { \
        std::string __mstr = "Error, atempted use of ModelGenerator query function when with mNOM or mLibStruct are null in "; \
        __mstr = __mstr + __FUNC__; \
        throw CoreException(__mstr); \
    } \
}

using namespace std;

namespace rr
{
ModelGenerator::ModelGenerator()
:
mComputeAndAssignConsevationLaws(false),
mDoubleFormat("%.19G"),
mFixAmountCompartments("*"),
mLibStruct(0),
mNOM(0),
mNumBoundarySpecies(0),
mNumCompartments(0),
mTotalLocalParmeters(0),
mNumEvents(0),
mNumGlobalParameters(0),
mNumIndependentSpecies(0),
mNumReactions(0),
mNumModifiableSpeciesReferences(0),
mNumFloatingSpecies(0),
mNumDependentSpecies(0)

{
}

ModelGenerator::~ModelGenerator() {}

SymbolList& ModelGenerator::getReactionListReference()
{
    return mReactionList;
}

SymbolList& ModelGenerator::getFloatingSpeciesConcentrationListReference()
{
    return mFloatingSpeciesConcentrationList;
}

SymbolList& ModelGenerator::getBoundarySpeciesListReference()
{
    return mBoundarySpeciesList;
}

SymbolList& ModelGenerator::getGlobalParameterListReference()
{
    return mGlobalParameterList;
}

SymbolList& ModelGenerator::getConservationListReference()
{
    return mConservationList;
}

void ModelGenerator::reset()
{
    CHECK_LIB_NOM();
    mNOM->reset();
    mLibStruct->Reset();
}

int ModelGenerator::numAdditionalRates()
{
    return mMapRateRule.size();
}

StringList ModelGenerator::getCompartmentList()
{
    StringList tmp;
    for (u_int i = 0; i < mCompartmentList.size(); i++)
    {
        tmp.Add(mCompartmentList[i].name);
    }
    return tmp;
}

string ModelGenerator::getInfo()
{
    // TODO placeholder for
    //info<<"Compiler location: "     <<  getCompiler()->getCompilerLocation()<<endl;
    //info<<"Support Code Folder: "   <<  getCompiler()->getSupportCodeFolder()<<endl;
    return "";
}

string ModelGenerator::substituteTerms(const int& numReactions, const string& reactionName, const string& equation)
{
    return substituteTerms(reactionName, equation, false);
}

ASTNode* ModelGenerator::cleanEquation(ASTNode* astP)
{
    ASTNode& ast = *astP; //For convenience...

    if (ast.getType() == AST_PLUS && ast.getNumChildren() == 0)
    {
        ASTNode* result = new ASTNode(AST_INTEGER);
        result->setValue(0);
        return result;
    }
    else if (ast.getType() == AST_TIMES && ast.getNumChildren() == 0)
    {
        ASTNode* result = new ASTNode(AST_INTEGER);
        result->setValue(1);
        return result;
    }
    else if (ast.getType() == AST_PLUS && ast.getNumChildren() == 1)
    {
        return ast.getChild(0);
    }
    else if (ast.getType() == AST_TIMES && ast.getNumChildren() == 1)
    {
        return ast.getChild(0);
    }

    for (int i = (int) ast.getNumChildren() - 1; i >= 0; i--)
    {
        ast.replaceChild(i, cleanEquation(ast.getChild(i)));
    }

    return astP;
}

string ModelGenerator::cleanEquation(const string& eqn)
{
    if (eqn.size() < 1)
    {
        return "0";
    }
    string equation(eqn);
    if (equation == " + ")
    {
        return "0";
    }

    if (equation == " * ")
    {
        return "1";
    }

    ASTNode* ast = SBML_parseFormula(equation.c_str());
    if (ast == NULL)
    {
        // we are in trouble!
        if (EndsWith(equation, "* "))
        {
              equation = equation.substr(0, equation.size() - 2);
        }

           string sought("*  +");
        if(equation.find(sought) != string::npos)
        {
            equation.replace(equation.find(sought), sought.size(), string("+"));
        }
        sought = ("*  -");
        if(equation.find(sought) != string::npos)
        {
            equation = equation.replace(equation.find(sought), sought.size(), "-");
        }

        ast = SBML_parseFormula(equation.c_str());
        if (ast == NULL)
        {
            return equation;
        }
    }

    ast = cleanEquation(ast);
    return SBML_formulaToString(ast);
}

string ModelGenerator::substituteTerms(const string& reactionName, const string& inputEquation, bool bFixAmounts)
{
    string equation = cleanEquation(inputEquation);
    if (equation.size() < 1)
    {
        return string("0");
    }

     Scanner s;
     stringstream ss;
     ss<<equation;

     s.AssignStream(ss);
     s.startScanner();
     s.nextToken();
     CodeBuilder sb;

    try
    {
        while (s.token() != CodeTypes::tEndOfStreamToken)
        {
            substituteToken(reactionName, bFixAmounts, s, sb);
            s.nextToken();
        }
    }
    catch (const Exception& e)
    {
       throw Exception(e.Message());
    }
    return sb.ToString();
}

ls::DoubleMatrix* ModelGenerator::initializeL0(int& nrRows, int& nrCols)
{
    ls::DoubleMatrix* L0;
    try
    {
        if (mNumDependentSpecies > 0)
        {
            vector<string> RowLabels;
            vector<string> ColumnLabels; //Todo: Filling these out here is meaningless?
            L0 = mLibStruct->getL0Matrix();//(RowLabels, ColumnLabels);
            nrRows = L0->RSize();//.size();
            nrCols = L0->CSize();//.size();
        }
        else
        {
            L0 = new ls::DoubleMatrix(1,1);//.Allocate(1,1);// = new double[0][];             //Todo: memoryleak
            nrRows = nrCols = 1;
        }
    }
    catch (const Exception& e)
    {
        nrRows = nrCols = 0;
        L0 = NULL;
        throw Exception(e.Message());
    }
    return L0;
}

int ModelGenerator::readGlobalParameters()
{
    CHECK_LIB_NOM();

    int numGlobalParameters;
    ArrayList oParameters = mNOM->getListOfParameters();
    numGlobalParameters = oParameters.Count();
    for (u_int i = 0; i < numGlobalParameters; i++)
    {
        StringList parameter = oParameters[i];

        string name     = parameter[0];
        double value     = ToDouble(parameter[1]);
        Symbol aSymbol(name, value);
        Log(lDebug5)<<"Adding symbol"<<aSymbol<<" to global parameters";

        mGlobalParameterList.Add(aSymbol);
    }
    return numGlobalParameters;
}

//Todo: totalLocalParmeters is not used
void ModelGenerator::readLocalParameters(const int& numReactions,  vector<int>& localParameterDimensions, int& totalLocalParmeters)
{
    CHECK_LIB_NOM();

    string name;
    double value;
    int numLocalParameters;
    totalLocalParmeters = 0;
    string reactionName;
    localParameterDimensions.resize(numReactions);
    for (int i = 0; i < numReactions; i++)
    {
        numLocalParameters = mNOM->getNumParameters(i);
        reactionName = mNOM->getNthReactionId(i);
        mReactionList.Add(Symbol(reactionName, 0.0));
        SymbolList newList;
        for (u_int j = 0; j < numLocalParameters; j++)
        {
            localParameterDimensions[i] = numLocalParameters;
            name = mNOM->getNthParameterId(i, j);
            value = mNOM->getNthParameterValue(i, j);
            newList.Add(Symbol(reactionName, name, value));
        }
        mLocalParameterList.push_back(newList);
    }
}

bool ModelGenerator::expressionContainsSymbol(ASTNode *ast, const string& symbol)
{
    if (ast == NULL || IsNullOrEmpty(symbol))
    {
        return false;
    }

    if (ast->getType() == libsbml::AST_NAME && Trim(ast->getName()) == Trim(symbol))
    {
        return true;
    }

    for (u_int i = 0; i < ast->getNumChildren(); i++)
    {
        if (expressionContainsSymbol(ast->getChild(i), symbol))
        {
            return true;
        }
    }

    return false;
}

bool ModelGenerator::expressionContainsSymbol(const string& expression,const string& symbol)
{
      if (IsNullOrEmpty(expression) || IsNullOrEmpty(symbol))
      {
          return false;
      }
      ASTNode *ast = SBML_parseFormula(expression.c_str());
      return expressionContainsSymbol(ast, symbol);
}

Symbol* ModelGenerator::getSpecies(const string& id)
{
    int index;
    if (mFloatingSpeciesConcentrationList.find(id, index))
    {
        return &(mFloatingSpeciesConcentrationList[index]);
    }

    if (mBoundarySpeciesList.find(id, index))
    {
        return &(mBoundarySpeciesList[index]);
    }
    return NULL;
}

string ModelGenerator::writeDouble(const double& value, const string& format)
{
    return ToString(value, format);
}

int ModelGenerator::readCompartments()
{
    CHECK_LIB_NOM();

    int numCompartments = mNOM->getNumCompartments();
    for (u_int i = 0; i < numCompartments; i++)
    {
        string sCompartmentId = mNOM->getNthCompartmentId(i);
        double value = mNOM->getValue(sCompartmentId);

        if(IsNaN(value))
        {
            value = 1;
        }
        mCompartmentList.Add(Symbol(sCompartmentId, value));
    }
    return numCompartments;
}

int ModelGenerator::readModifiableSpeciesReferences()
{
    CHECK_LIB_NOM();

    if(!mNOM->GetSBMLDocument())
    {
        return -1;
    }
    SBMLDocument &SBMLDoc = *mNOM->GetSBMLDocument();
    Model &SbmlModel  = *mNOM->GetModel();

    if(mNOM->GetSBMLDocument()->getLevel() < 3)
    {
        return 0;
    }

    string id;
    double value;
    int numReactions = SbmlModel.getNumReactions();
    for (u_int i = 0; i < numReactions; i++)
    {
        Reaction &reaction = *(SbmlModel.getReaction(i));
        for (u_int j = 0; j < reaction.getNumReactants(); j++)
        {
            SpeciesReference &reference = *(reaction.getReactant(j));
            id = reference.getId();
            if (!(id.size()))
            {
                continue;
            }
            value = reference.getStoichiometry();
            if (IsNaN(value))
                value = 1;

            if (reference.isSetId())
            {
                mModifiableSpeciesReferenceList.Add(Symbol(id, value));
            }
        }
        for (u_int j = 0; j < reaction.getNumProducts(); j++)
        {
            SpeciesReference &reference = *(reaction.getProduct(j));
            id = reference.getId();
            if (IsNullOrEmpty(id))
            {
                continue;
            }
            value = reference.getStoichiometry();
            if (IsNaN(value))
            {
                value = 1;
            }

            if (reference.isSetId())
            {
                mModifiableSpeciesReferenceList.Add(Symbol(id, value));
            }
        }
    }
    return mModifiableSpeciesReferenceList.size();
}

StringList ModelGenerator::getReactionIds()
{
    StringList tmp;
    for (int i = 0; i < mReactionList.size(); i++)
    {
        tmp.Add(mReactionList[i].name);
    }
    return tmp;
}

StringList ModelGenerator::getFloatingSpeciesConcentrationList()
{
    StringList tmp;
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        tmp.Add(mFloatingSpeciesConcentrationList[i].name);
    }
    return tmp;
}

StringList ModelGenerator::getBoundarySpeciesList()
{
    StringList tmp;
    for (int i = 0; i < mBoundarySpeciesList.size(); i++)
    {
        tmp.Add(mBoundarySpeciesList[i].name);
    }
    return tmp;
}

StringList ModelGenerator::getGlobalParameterList()
{
    StringList tmp;
    for (int i = 0; i < mGlobalParameterList.size(); i++)
    {
        tmp.Add(mGlobalParameterList[i].name);
    }

    for (int i = 0; i < mConservationList.Count(); i++)
    {
        tmp.Add(mConservationList[i].name);
    }

    return tmp;
}

int ModelGenerator::getNumberOfReactions()
{
    return mReactionList.size();
}

StringList ModelGenerator::getConservationList()
{
    StringList tmp;// = new ArrayList();
    for (int i = 0; i < mConservationList.Count(); i++)
    {
        tmp.Add(mConservationList[i].name);
    }
    return tmp;
}



}//rr namespace

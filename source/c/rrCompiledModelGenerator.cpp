/*
 * rrCompiledModelGenerator.cpp
 *
 *  Created on: May 26, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "rrCompiledModelGenerator.h"
#include "rrModelGenerator.h"
#include "rrLogger.h"
#include "rrStringUtils.h"
#include "rrException.h"
#include <iostream>
#include <cmath>
#include <stack>
#include "sbml/SBMLDocument.h"


namespace rr
{
using namespace std;
using namespace libsbml;


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

CompiledModelGenerator::CompiledModelGenerator()
:
mComputeAndAssignConsevationLaws(false),
mFixAmountCompartments("*"),
mLibStruct(0),
mNOM(0)
{
}

CompiledModelGenerator::~CompiledModelGenerator() {}

string CompiledModelGenerator::substituteTerms(const int& numReactions, const string& reactionName, const string& equation)
{
    return substituteTerms(reactionName, equation, false);
}

string CompiledModelGenerator::substituteTerms(const string& reactionName, const string& inputEquation, bool bFixAmounts)
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

void CompiledModelGenerator::reset()
{
    mNOM = NULL;
    mLibStruct = NULL;
}

int CompiledModelGenerator::numAdditionalRates()
{
    return ms.mRateRules.size();
}

string CompiledModelGenerator::getInfo()
{
    // TODO placeholder for
    //info<<"Compiler location: "     <<  getCompiler()->getCompilerLocation()<<endl;
    //info<<"Support Code Folder: "   <<  getCompiler()->getSupportCodeFolder()<<endl;
    return "";
}

ASTNode* CompiledModelGenerator::cleanEquation(ASTNode* astP)
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
    else if ((ast.getType() == AST_PLUS && ast.getNumChildren() == 1) ||
            (ast.getType() == AST_TIMES && ast.getNumChildren() == 1))
    {
        ASTNode *p = ast.getChild(0);
        return p ? new ASTNode(*p) : p;
    }

    for (int i = (int) ast.getNumChildren() - 1; i >= 0; i--)
    {
        ASTNode *p = ast.getChild(i);
        ast.replaceChild(i, cleanEquation(p));
        delete p;
    }

    return new ASTNode(ast);
}

string CompiledModelGenerator::cleanEquation(const string& eqn)
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
        if (endsWith(equation, "* "))
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

    ASTNode *ast2 = cleanEquation(ast);
    string result = SBML_formulaToStdString(ast2);
    delete ast;
    delete ast2;
    return result;
}

ls::DoubleMatrix* CompiledModelGenerator::initializeL0(int& nrRows, int& nrCols)
{
    ls::DoubleMatrix* L0;
    try
    {
        if (ms.mNumDependentSpecies > 0)
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

bool CompiledModelGenerator::expressionContainsSymbol(ASTNode *ast, const string& symbol)
{
    if (ast == NULL || isNullOrEmpty(symbol))
    {
        return false;
    }

    if (ast->getType() == libsbml::AST_NAME && trim(ast->getName()) == trim(symbol))
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

bool CompiledModelGenerator::expressionContainsSymbol(const string& expression,const string& symbol)
{
      if (isNullOrEmpty(expression) || isNullOrEmpty(symbol))
      {
          return false;
      }
      ASTNode *ast = SBML_parseFormula(expression.c_str());
      return expressionContainsSymbol(ast, symbol);
}

const Symbol* CompiledModelGenerator::getSpecies(const string& id)
{
    int index;
    if (ms.mFloatingSpeciesConcentrationList.find(id, index))
    {
        return &(ms.mFloatingSpeciesConcentrationList[index]);
    }

    if (ms.mBoundarySpeciesList.find(id, index))
    {
        return &(ms.mBoundarySpeciesList[index]);
    }
    return NULL;
}

string CompiledModelGenerator::writeDouble(const double& value, const string& format)
{
    return toString(value, format);
}


} /* namespace rr */

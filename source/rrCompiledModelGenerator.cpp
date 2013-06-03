/*
 * rrCompiledModelGenerator.cpp
 *
 *  Created on: May 26, 2013
 *      Author: andy
 */

#include "rrCompiledModelGenerator.h"
#include "rrException.h"

namespace rr {

CompiledModelGenerator::CompiledModelGenerator() {
    // TODO Auto-generated constructor stub

}

CompiledModelGenerator::~CompiledModelGenerator() {
    // TODO Auto-generated destructor stub
}

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

} /* namespace rr */

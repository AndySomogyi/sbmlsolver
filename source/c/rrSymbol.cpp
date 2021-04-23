#pragma hdrstop
#include "rrSymbol.h"
//---------------------------------------------------------------------------
namespace rr
{

Symbol::Symbol(const std::string& _name, const double& _value)
:
name(_name),
value(_value),
rateRule(false),
hasOnlySubstance(false)
{}

Symbol::Symbol(const std::string& _keyName, const std::string& _name, const double& _value)
:
keyName(_keyName),
name(_name),
value(_value),
hasOnlySubstance(false),
rateRule(false)
{}

Symbol::Symbol(const std::string& _name, const double& _value, const std::string& _compartmentName)
:
name(_name),
value(_value),
compartmentName(_compartmentName),
hasOnlySubstance(false),
rateRule(false)
{}

Symbol::Symbol(const std::string& _name, const double& _value, const std::string& _compartmentName, const std::string& _formula)
:
name(_name),
value(_value),
compartmentName(_compartmentName),
formula(_formula),
hasOnlySubstance(false),
rateRule(false)
{}

std::ostream& operator<<(std::ostream& stream, const Symbol& symbol)
{
    stream<<"Name: "<<symbol.name;
    return stream;
}
}

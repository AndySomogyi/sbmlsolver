#pragma hdrstop
#include "rrHashTable.h"
//---------------------------------------------------------------------------

namespace rr
{

StringSymbolHashTable::StringSymbolHashTable()
{}


bool StringSymbolHashTable::ContainsKey(const std::string& aKey)
{
    return (this->find( aKey ) != this->end()) ? true : false;
}


/////////////////////
IntStringHashTable::IntStringHashTable()
{}

std::ostream& operator<<(std::ostream& stream, StringSymbolHashTable& hash)
{
    std::map<std::string, SBMLSymbol>::iterator iter;

    for(iter = hash.begin(); iter != hash.end(); iter++)
    {
        stream<<"Key: "<<(*iter).first<<"\tValue:"<<(*iter).second<<"\n";
    }
    return stream;
}



}

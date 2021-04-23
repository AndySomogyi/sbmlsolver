#ifndef rrHashTableH
#define rrHashTableH
#include <map>
#include <string>
#include "rrExporter.h"
#include "rrSBMLSymbol.h"
//---------------------------------------------------------------------------

//using std::hash_map;
using std::map;
using std::string;

namespace rr
{

class RR_DECLSPEC StringSymbolHashTable: public std::map<std::string, SBMLSymbol>
{
    protected:

    public:
        StringSymbolHashTable();
        std::map<std::string, SBMLSymbol>::iterator     mIter;
        bool                                  ContainsKey(const std::string& aKey);
};

std::ostream& operator<<(std::ostream& stream, StringSymbolHashTable& hash);
class RR_DECLSPEC IntStringHashTable:  public std::map<int, std::string>
{
    protected:

    public:
        IntStringHashTable();

};


} //namespace rr
#endif

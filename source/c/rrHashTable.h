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

class RR_DECLSPEC StringSymbolHashTable: public map<string, SBMLSymbol>
{
    protected:

    public:
        StringSymbolHashTable();
        map<string, SBMLSymbol>::iterator     mIter;
        bool                                  ContainsKey(const string& aKey);
};

ostream& operator<<(ostream& stream, StringSymbolHashTable& hash);
class RR_DECLSPEC IntStringHashTable:  public map<int, string>
{
    protected:

    public:
        IntStringHashTable();

};


} //namespace rr
#endif

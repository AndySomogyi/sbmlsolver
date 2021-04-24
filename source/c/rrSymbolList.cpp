#pragma hdrstop
#include "rrSymbolList.h"
//---------------------------------------------------------------------------



namespace rr
{
void SymbolList::Clear()
{
    clear();
}

int SymbolList::Add(const Symbol& item)
{
    push_back(item);
    return size();
}

double SymbolList::getValue(const int& index)
{
    return at(index).value;
}

std::string SymbolList::getName(const int& index)
{
//            return ((Symbol) base[index]).name;
    return at(index).name;
}

std::string SymbolList::getKeyName(const int& index)
{
//            return ((Symbol) base[index]).keyName;
    return at(index).keyName;
}


bool SymbolList::find(const std::string& name, int& index) const
{
    index = -1;
    for (unsigned int i = 0; i < size(); i++)
    {
        Symbol sym = at(i);
        if (name == sym.name)
        {
            index = i;
            return true;
        }
    }
    return false;
}

bool SymbolList::find(const std::string& keyName, const std::string& name, int& index) const
{
    index = -1;
    for(unsigned int i = 0; i < size(); i++)
    {
        Symbol sym = at(i);
        if ((sym.name == name) && (sym.keyName == keyName))
        {
            index = i;
            return true;
        }
    }
    return false;
}

}//namespace rr




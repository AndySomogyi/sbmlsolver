#pragma hdrstop
#include "rrArrayListItem.h"
#include "rrArrayList.h"
//---------------------------------------------------------------------------
namespace rrc
{

std::ostream& operator<<(std::ostream& stream, const ArrayListItemBase& item)
{
    //Have to figure out subtype of item
    ArrayListItemBase* ptr = const_cast<ArrayListItemBase*>(&item);
    if(dynamic_cast<ArrayListItem<int>*>(ptr))
    {
        stream << (int) *(dynamic_cast<ArrayListItem<int>*>(ptr));
    }
    else if(dynamic_cast<ArrayListItem<double>*>(ptr))
    {
        stream << (double) *(dynamic_cast<ArrayListItem<double>*>(ptr));
    }
    else if(dynamic_cast<ArrayListItem<string>*>(ptr))
    {
        stream << "\""<<(string) *(dynamic_cast<ArrayListItem<string>*>(ptr))<<"\"";
    }
    else if(dynamic_cast<ArrayListItem<StringList>*>(ptr))
    {
        stream << (StringList) *(dynamic_cast<ArrayListItem<StringList>*>(ptr));
    }
    else if(dynamic_cast<ArrayListItem<ArrayList>*>(ptr))
    {
        stream << (ArrayList) *(dynamic_cast<ArrayListItem<ArrayList>*>(ptr));
    }
    else
    {
        stream<<"Stream operator not implemented for this type";
    }
    return stream;
}

}

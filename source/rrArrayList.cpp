#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <sstream>
#include "rrStringList.h"
#include "rrArrayList.h"
#include "rrLogger.h"
//---------------------------------------------------------------------------

namespace rr
{

ArrayList::ArrayList()
{}

ArrayList::ArrayList(const string& lbl, const StringList& stringList)
{
	Add(lbl, stringList);
}

ArrayList::ArrayList(const string& lbl, const ArrayList& arrayList)
{
	Add(lbl, arrayList);
}

ArrayList::~ArrayList()
{
    if(mList.size())
    {
        for(u_int i = 0; i < Count(); i++)
        {
            delete mList[i];
        }
        mList.clear();
    }
}

string ArrayList::GetString(const int& index)
{
    if(index < mList.size())
    {
		ArrayListItemObject* listPtr = mList[index];

   		if(listPtr)
        {
        	if(dynamic_cast< ArrayListItem<string>* >(listPtr))
            {
				return *(dynamic_cast< ArrayListItem<string>* >(listPtr));
            }
        }
    }

	throw("No string at index");
}

StringList ArrayList::GetStringList(const int& index)
{
    if(index < mList.size())
    {
		ArrayListItemObject* listPtr = mList[index];

   		if(listPtr)
        {
        	if(dynamic_cast< ArrayListItem<StringList>* >(listPtr))
            {
				return *(dynamic_cast< ArrayListItem<StringList>* >(listPtr));
            }
        }
    }

	throw("No Stringlist at index");
}

StringList ArrayList::GetStringList(const string& lName)
{
    //Look for ann array list whose first item is a string with lName and second item is a stringlist, i.e. {{string, {string string string}}
    StringList aList;
    for(u_int i = 0; i < Count(); i++)
    {
        ArrayListItemObject* listPtr = const_cast<ArrayListItemObject*>(mList[i]);

        //Check for a list which first element is a string, i.e. a {{string}, {string, string}} list
        if(dynamic_cast< ArrayListItem<ArrayList> *>(listPtr))
        {
			ArrayList list = (ArrayList) *(dynamic_cast< ArrayListItem<ArrayList> *>(listPtr));
            if(list.Count())
            {
                ArrayListItemObject* anItem = &list[0];
                if(dynamic_cast<ArrayListItem<string>*>(anItem))
                {
                    string str = (string) *dynamic_cast<ArrayListItem<string>*>(anItem);

                    if(str == lName && list.Count() > 1)
                    {
                        ArrayListItemObject* anItem = &list[1];
                        if(dynamic_cast<ArrayListItem<StringList> *>(anItem))
                        {
                            //This is a stringList
                            StringList  list = (StringList) *(dynamic_cast<ArrayListItem<StringList>*>(anItem));
                            for(int i = 0; i < list.Count(); i++)
                            {
                            	string str = list[i];
                                aList.Add(str);
                            }
                        }
                    }
                }
            }
        }
    }
    return aList;
}

void ArrayList::Clear()
{
    if(Count())
    {
        for(u_int i = 0; i < Count(); i++)
        {
            delete mList[i];
        }
        mList.clear();
    }
}

unsigned int ArrayList::Count() const
{
    return mList.size();
}

ArrayList::ArrayList(const ArrayList& copyMe)
{
    //Deep copy
    Clear();
    mList.resize(copyMe.Count());
    for(u_int i = 0; i < copyMe.Count(); i++)
    {
        //const ArrayListItem<T>& item = copyMe[i];
        ArrayListItemObject* ptr = const_cast<ArrayListItemObject*>(&copyMe[i]);
        if(dynamic_cast<ArrayListItem<int>*>(ptr))
        {
            mList[i] = new ArrayListItem<int>(*(dynamic_cast<ArrayListItem<int>*>(ptr)));
        }
        else if(dynamic_cast<ArrayListItem<double>*>(ptr))
        {
            mList[i] = new ArrayListItem<double>(*(dynamic_cast<ArrayListItem<double>*>(ptr)));
        }
        else if(dynamic_cast<ArrayListItem<string>*>(ptr))
        {
            mList[i] = new ArrayListItem<string>(*(dynamic_cast<ArrayListItem<string>*>(ptr)));
        }
        else if(dynamic_cast<ArrayListItem<StringList>*>(ptr))
        {
            mList[i] = new ArrayListItem<StringList>(*(dynamic_cast<ArrayListItem<StringList>*>(ptr)));
        }
        else if(dynamic_cast<ArrayListItem<ArrayList>*>(ptr))
        {
            mList[i] = new ArrayListItem<ArrayList>(*(dynamic_cast<ArrayListItem<ArrayList>*>(ptr)));
        }

//        else if(dynamic_cast<ArrayListItem<ArrayListItem>*>(ptr))
//        {
//            mList[i] = new ArrayListItem<ArrayListItem>(*(dynamic_cast<ArrayListItem<ArrayListItem>*>(ptr)));
//        }
        else
        {
            mList[i] = NULL;
        }
    }
}

void ArrayList::operator=(const ArrayList& rhs)
{
    //Deep copy
    Clear();
    mList.resize(rhs.Count());
    for(u_int i = 0; i < rhs.Count(); i++)
    {
        ArrayListItemObject* ptr = const_cast<ArrayListItemObject*>(&rhs[i]);
        if(dynamic_cast<ArrayListItem<int>*>(ptr))
        {
            mList[i] = new ArrayListItem<int>(*(dynamic_cast<ArrayListItem<int>*>(ptr)));
        }
        else if(dynamic_cast<ArrayListItem<double>*>(ptr))
        {
            mList[i] = new ArrayListItem<double>(*(dynamic_cast<ArrayListItem<double>*>(ptr)));
        }
        else if(dynamic_cast<ArrayListItem<string>*>(ptr))
        {
            mList[i] = new ArrayListItem<string>(*(dynamic_cast<ArrayListItem<string>*>(ptr)));
        }
        else if(dynamic_cast<ArrayListItem<ArrayList>*>(ptr))
        {
            mList[i] = new ArrayListItem<ArrayList>(*(dynamic_cast<ArrayListItem<ArrayList>*>(ptr)));
        }

//        else if(dynamic_cast<ArrayListItem<ArrayListItem>*>(ptr))
//        {
//            mList[i] = new ArrayListItem<ArrayListItem>(*(dynamic_cast<ArrayListItem<ArrayListItem>*>(ptr)));
//        }
        else
        {
            mList[i] = NULL;
        }
    }
}

void ArrayList::Add(const int& item)
{
    ArrayListItem<int>* ptr =  new ArrayListItem<int>(item);
    mList.push_back(ptr);
}

void ArrayList::Add(const double& item)
{
    ArrayListItem<double>* ptr = new ArrayListItem<double>(item);
    mList.push_back(ptr);
}

void ArrayList::Add(const string& item)
{
    ArrayListItem<string> *ptr = new ArrayListItem<string>(item);
    mList.push_back(ptr);
}

void ArrayList::Add(const StringList& item)
{
    ArrayListItem< StringList > *ptr = new ArrayListItem<StringList>(item);
    mList.push_back(ptr);
}

void ArrayList::Add(const ArrayList& item)
{
    ArrayListItem<ArrayList> *aList = new ArrayListItem<ArrayList>(item);
    mList.push_back(aList);
}

void ArrayList::Add(const string& lbl, const StringList& list)
{
    ArrayList temp;
    temp.Add(lbl);
    temp.Add(list);
    Add(temp);
}

void ArrayList::Add(const string& lbl, const ArrayList& lists)
{
    ArrayList temp;
    temp.Add(lbl);
    temp.Add(lists);
    Add(temp);
}

const ArrayListItemObject& ArrayList::operator[](int pos) const
{
    return *mList[pos];
}

ArrayListItemObject& ArrayList::operator[](int pos)
{
    return *mList[pos];
}

//================== ostreams =============
ostream& operator<<(ostream& stream, const ArrayList& list)
{
   	stream<<"{";

    for(u_int i = 0; i < list.Count(); i++)
    {
        stream<<list[i];
        if(i < list.Count() -1)
        {
        	stream<<",";
        }
    }
    stream<<"}";
    return stream;
}

}





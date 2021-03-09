#pragma hdrstop
#include <algorithm>
#include <iostream>
#include <sstream>
#include "rrStringUtils.h"
#include "rrStringList.h"
#include "rrUtils.h"
#include "rrException.h"



namespace rr
{

using namespace rrc;

StringList::StringList()
{}

StringList::StringList(const std::vector<std::string>& strings)
:
mStrings(strings)
{}

StringList::StringList(RRStringArray* cp)
{
    if(!cp)
    {
        return;
    }

    for(int i = 0; i < cp->Count; i++)
    {
        mStrings.push_back(cp->String[i]);
    }
}

StringList::StringList(const std::string& str, const std::string& delimiter)
{
    mStrings = splitString(str, delimiter);
}

StringList::StringList(char** aList, const int& count)
{
    //Copy C stringlist into the container..
    for(int i = 0; i < count; i++)
    {
        mStrings.push_back(aList[i]);
    }
}

StringList::StringList(const StringList& cp)
{
    mStrings = cp.mStrings;
}

StringList::~StringList()
{}

std::vector<std::string>::iterator StringList::begin()
{
    return mStrings.begin();
}

std::vector<std::string>::iterator StringList::end()
{
    return mStrings.end();
}

std::string& StringList::operator[](const int& index)
{
    if(index > Count() -1 )
    {
        std::stringstream msg;
        msg<<"index ("<<index<<") out of bounds in StringList with count "<<Count();

        throw(CoreException(msg.str()));
    }
    return mStrings[index];
}

const std::string& StringList::operator[](const int& index) const
{
    if(index > Count() -1 )
    {
        std::stringstream msg;
        msg<<"index ("<<index<<") out of bounds in StringList with count "<<Count();

        throw(CoreException(msg.str()));
    }

    return mStrings[index];
}

unsigned int StringList::size() const
{
    return mStrings.size();
}

unsigned int StringList::Count() const
{
    return mStrings.size();
}

std::string StringList::AsString(const std::string& delimiter) const
{
    std::stringstream names;
    for(int i = 0; i < mStrings.size(); i++)
    {
        names<<mStrings[i];
        if( i < mStrings.size() - 1)
        {
            names<<delimiter;
        }
    }
    return names.str();
}

void StringList::PreFix(const std::string& fix)
{
     for(mLI = mStrings.begin(); mLI != mStrings.end(); mLI++)
    {
        (*mLI) = fix + (*mLI) ;
    }
}

void StringList::PostFix(const std::string& fix)
{
    for(mLI = mStrings.begin(); mLI != mStrings.end(); mLI++)
    {
        (*mLI) = (*mLI) + fix;
    }
}

StringList StringList::operator-(const StringList& rhs)
{
    StringList newList;

    for(int i = 0; i < Count(); i++)
    {
        std::string item = mStrings[i] + "-" + rhs[i];
        newList.add(item);
    }

    return newList;
}

void StringList::InsertAt(const int& index, const std::string& item)
{
    mLI = mStrings.begin() + index;
    if(mLI != mStrings.end())
    {
        mStrings.insert(mLI, item);
    }
}

void StringList::Append(const StringList& list)
{
    for(int i = 0; i < list.Count(); i++)
    {
        mStrings.push_back(list[i]);
    }
}

void StringList::add(const std::string& item)
{
    mStrings.push_back(item);
}

int StringList::find(const std::string& item)
{
    return rr::indexOf(mStrings, item);
}

int StringList::indexOf(const std::string& item)
{
    return rr::indexOf(mStrings, item);
}

void StringList::removeAt(const int& index)
{
    mLI = mStrings.begin() + index;
    mStrings.erase(mLI);
}

bool StringList::Contains(const std::string& item) const
{
    return std::find(mStrings.begin(), mStrings.end(), item) != mStrings.end() ? true : false;
}

bool StringList::DontContain(const std::string& item) const
{
    return !Contains(item);
}

void StringList::clear()
{
    mStrings.clear();
}

void StringList::empty()
{
    mStrings.clear();
}

StringList& StringList::operator=(const StringList& rhs)
{
    mStrings = rhs.mStrings;
    return *this;
}

StringList& StringList::operator=(const std::vector<std::string>& rhs)
{
    mStrings = rhs;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const StringList& list)
{
    stream<<"{";
    for(int i = 0; i < list.Count(); i++)
    {
        stream<<"\""<<list[i]<<"\"";
        if(i < list.Count() -1)
        {
            stream<<",";
        }

    }
    stream<<"}";
    return stream;
}


} //namespace rr


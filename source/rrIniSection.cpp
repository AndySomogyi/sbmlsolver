#pragma hdrstop
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrLogger.h"
#include "rrIniSection.h"

using namespace rr;

namespace rr
{
IniSection::IniSection()
:
mIsDirty(true),
mIsUsed(false),
mName(""),
mComment("")
{

}

IniSection::IniSection(const std::string& nameValueString, const char& sep)
:
mIsDirty(true),
mIsUsed(false),
mName(""),
mComment("")
{
    std::vector<std::string> keys = splitString(nameValueString, sep);

    //Insert each key in the section
    for(unsigned int i = 0; i < keys.size(); i++)
    {
        IniKey *aKey = new IniKey(keys[i]);
        mKeys.push_back(aKey);
    }
}

IniSection::~IniSection()
{
    //detete all keys
    for(unsigned int i = 0; i < mKeys.size(); i++)
    {
        IniKey *key = mKeys[i];
           delete key;
    }
    mKeys.clear();
}

//IniKey function
IniKey*    IniSection::GetKey(const std::string& keyName, bool create)
{
    //Go trough the key list and return key with key name
       KeyItor k_pos;
    for (k_pos = mKeys.begin(); k_pos != mKeys.end(); k_pos++)
    {
        if ( compareNoCase( (*k_pos)->mKey, keyName ) == 0 )
            return *k_pos;
    }

    if(create)
    {
        CreateKey(keyName);
        return GetKey(keyName, false);
    }

    return NULL;
}

//IniKey function
IniKey*    IniSection::GetKey(const int& keyNr)
{
    //Go trough the key list and return key with key name

    if(keyNr < mKeys.size())
    {
        return mKeys[keyNr];
    }

    return NULL;
}

std::string IniSection::GetNonKeysAsString()
{
    std::string tmp = "";
       NonKeyItor listPos;

    if(!mNonKeys.size())
    {
        return tmp;
    }

    for(listPos = mNonKeys.begin(); listPos != mNonKeys.end(); listPos++)
    {
        if ((*listPos).size())
        {
            tmp += (*listPos);
            tmp += "\n";
        }
    }

    return tmp;
}

std::string IniSection::AsString()
{
    std::string tmp = "";
       KeyItor listPos;
    for(listPos = mKeys.begin(); listPos != mKeys.end(); listPos++)
    {
        if ((*listPos)->AsString().size())
        {
            tmp += (*listPos)->AsString();
            tmp += "\n";
        }
    }

    return tmp;
}

IniKey* IniSection::CreateKey(const std::string& mKey, const std::string& mValue, const std::string& mComment)
{
    IniKey*         pKey = GetKey(mKey);

    //Check if the key exists
    if(pKey)
    {
        pKey->mValue = mValue;
        pKey->mComment = mComment;
        return pKey;
    }

    //Create the key
    pKey = new IniKey;

    pKey->mKey = mKey;
    pKey->mValue = mValue;
    pKey->mComment = mComment;
    mIsDirty = true;
    mKeys.push_back(pKey);
    return pKey;
}

}//namespace

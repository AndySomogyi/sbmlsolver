#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrIniKey.h"
#ifdef __CODEGEARC__
#pragma package(smart_init)
#endif

using namespace rr;
namespace rr
{
rrIniKey::rrIniKey(const string& key)
{
    SetupKey(key);
}

void rrIniKey::ReKey(const string& key)
{
    SetupKey(key);
}

ostream&  operator<<(ostream& stream, const rrIniKey& aKey)
{
    stream<<aKey.AsString();
    return stream;
}

string rrIniKey::AsString() const
{
    string tmp = mKey;
    tmp += " = ";
    tmp += mValue;
    return tmp;
}

int rrIniKey::AsBool() const
{
    return ToBool(mValue);
}

int rrIniKey::AsInt() const
{
    return ToInt(mValue);
}

double rrIniKey::AsFloat() const
{
    return ToDouble(mValue);
}

void rrIniKey::SetupKey(const string& key)
{
    if(key.size())
    {
        vector<string> recs = SplitString(key, "=");
        if(recs.size() > 0)
        {
            mKey = recs[0];
            mValue = recs[1];
          }
	    mComment 	= EmptyString;
    }
    else
    {
        mKey 		= EmptyString;
        mValue 		= EmptyString;
        mComment 	= EmptyString;
    }
}

}

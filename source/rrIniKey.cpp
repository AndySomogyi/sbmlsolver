#pragma hdrstop
#include "rrIniKey.h"


namespace rr
{
IniKey::IniKey(const std::string& key)
{
    SetupKey(key);
}

void IniKey::ReKey(const std::string& key)
{
    SetupKey(key);
}

std::ostream&  operator<<(std::ostream& stream, const IniKey& aKey)
{
    stream<<aKey.AsString();
    return stream;
}

std::string IniKey::AsString() const
{
    std::string tmp = mKey;
    tmp += " = ";
    tmp += mValue;
    return tmp;
}

int IniKey::AsBool() const
{
    return toBool(mValue);
}

int IniKey::AsInt() const
{
    return toInt(mValue);
}

double IniKey::AsFloat() const
{
    return toDouble(mValue);
}

complex<double> IniKey::AsComplex() const
{
    std::vector<std::string> vals = splitString(mValue,",");
    if(vals.size() == 2)
    {
        return complex<double>(toDouble(vals[0]), toDouble(vals[1]));
    }
    return complex<double>(toDouble(mValue), 0 );
}

void IniKey::SetupKey(const std::string& key)
{
    if(key.size())
    {
        std::vector<std::string> recs = splitString(key, "=");
        if(recs.size() > 0)
        {
            mKey = recs[0];
            mValue = recs[1];
          }
        mComment = "";
    }
    else
    {
        mKey = "";
        mValue = "";
        mComment = "";
    }
}

}

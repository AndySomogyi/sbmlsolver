#pragma hdrstop
#include "rrStringUtils.h"
#include "rrFileName.h"

using std::ostream;

namespace rr
{
//---------------------------------------------------------------------------
FileName::FileName(const std::string& name, const std::string& path)
{
    SetNameAndPath(path, name);
}

FileName::~FileName()
{

}

FileName::FileName(const FileName& fN)
{
    mPath = fN.GetPath();
    mName = fN.GetFileName();
    MakeFileString();
}

FileName& FileName::operator=(const FileName& fN)
{
    mPath = fN.GetPath();
    mName = fN.GetFileName();
    MakeFileString();
    return *this;
}

FileName& FileName::operator=(const std::string& fN)
{
    mPathAndName = fN;
    mPath = getFilePath(fN);
    mName = rr::getFileName(fN);
    return *this;
}

bool FileName::operator!=(const char* fN)
{
    return (mName != fN) ? false : true;
}

std::string FileName::GetFileName() const {return mName;}
std::string FileName::GetPath() const {return mPath;}
std::string FileName::GetPathAndFileName() const {return mPathAndName;}
size_t FileName::size(){return mName.size();}

FileName::operator std::string() {return mPathAndName;}
std::string FileName::Get()
{
    MakeFileString();
    return mPathAndName;
}

std::string FileName::GetFileNameNoExtension()
{
    //remove extension
    std::string fname = getFileNameNoExtension(GetFileName());
    return fname;
}

void FileName::SetFileName(const std::string& name)
{
    mName = name;
    MakeFileString();
}

bool FileName::SetPath(const std::string& path)
{
    mPath = path;
    MakeFileString();
    return true;
}

void FileName::SetNameAndPath(const std::string& path, const std::string& name)
{
    mPath = path;
    mName = name;
    MakeFileString();
}

void FileName::SetFileNameAndPath(const std::string& file)
{
    if(!file.size())
    {
        mPathAndName = "";
        mName = "";
        mPath = "";
    }
    else
    {
        mName = getFileName(file);
        mPath = getFilePath(file);
        MakeFileString();
    }
}

void FileName::MakeFileString()
{
    mPathAndName = "";
    if(mPath.size())
    {
        if(mPath[mPath.size()-1] == '\\' || mPath[mPath.size()-1] == '/')
        {
            mPathAndName = mPath + mName;
        }
        else //Add slashes to path
        {
#if defined(_WIN32) || defined(__WIN32__)
            mPath = mPath + "\\";
#else
            mPath = mPath + "/";
#endif
            mPathAndName = mPath + mName;
        }
    }
    else // No path
    {
        if(mName.size())
        {
            mPathAndName += mName;
        }
    }
}

std::ostream& operator <<(std::ostream& os, FileName& obj)
{
    os<<obj.Get();
    return os;
}

}

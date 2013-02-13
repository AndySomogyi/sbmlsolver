#pragma hdrstop
#include "Poco/UUIDGenerator.h"
#include "Poco/UUIDGenerator.h"
#include "rrModelSharedLibrary.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------
using Poco::UUID;
using Poco::UUIDGenerator;

namespace rr
{

ModelSharedLibrary::ModelSharedLibrary(const string& pathTo)
{
	if(FileExists(pathTo))
    {
    	load(pathTo);
    }
}

ModelSharedLibrary::~ModelSharedLibrary()
{}

bool ModelSharedLibrary::isLoaded()
{
	return mTheLib.isLoaded();
}

void* ModelSharedLibrary::getSymbol(const string& name)
{
	return mTheLib.getSymbol(name);
}
bool  ModelSharedLibrary::hasSymbol(const string& name)
{
	return mTheLib.hasSymbol(name);
}

bool ModelSharedLibrary::setPath(const string& pathTo)
{
	mPathToLib = pathTo;
    return true;
}

bool ModelSharedLibrary::load()
{
	mTheLib.load(getFullFileName());
    return true;
}

bool ModelSharedLibrary::load(const string& libName)
{
	mPathToLib = ExtractFilePath(libName);
    mLibName = ExtractFileName(libName);
	mTheLib.load(libName);
    return true;
}

bool ModelSharedLibrary::unload()
{
	mTheLib.unload();
    return true;
}

string ModelSharedLibrary::getName()
{
	return mLibName;
}

string ModelSharedLibrary::getFullFileName()
{
	return JoinPath(mPathToLib, mLibName);
}

string ModelSharedLibrary::createName(const string& baseName)
{

	if(!baseName.size())
    {
		//Create  a new UUID
		UUIDGenerator& generator = UUIDGenerator::defaultGenerator();
		UUID uuid2(generator.createRandom());
    	mLibName = uuid2.toString();
    }
    else
    {
    	mLibName = baseName;
    }

    mLibName.append(mTheLib.suffix());
	return mLibName;
}

}

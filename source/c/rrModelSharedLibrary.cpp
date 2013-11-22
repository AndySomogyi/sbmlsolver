#pragma hdrstop
#include "Poco/UUIDGenerator.h"
#include "Poco/UUIDGenerator.h"
#include "rrModelSharedLibrary.h"
#include "rrLogger.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------


namespace rr
{
using Poco::UUID;
using Poco::UUIDGenerator;

ModelSharedLibrary::ModelSharedLibrary(const string& pathTo)
{
	if(fileExists(pathTo))
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
	return load(getFullFileName());
}

bool ModelSharedLibrary::load(const string& libName)
{
	mPathToLib = getFilePath(libName);
    mLibName = getFileName(libName);
#if defined(_WIN32) || defined(__WIN32__)    
	mTheLib.load(libName);
#elif defined(__linux)
    //This flags causes symbols being 'local'
	mTheLib.load(libName, Poco::SharedLibrary::SHLIB_LOCAL);
#else
    mTheLib.load(libName);
#endif

    return mTheLib.isLoaded();
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
	return joinPath(mPathToLib, mLibName);
}

string ModelSharedLibrary::createName(const string& baseName)
{
	string newName;
	if(!baseName.size())
    {
		//Create  a new UUID
		UUIDGenerator& generator = UUIDGenerator::defaultGenerator();
		UUID uuid2(generator.createRandom());
    	newName = uuid2.toString();
    }
    else
    {
    	newName = baseName;
    }
#if defined(WIN32)
    newName.append(".dll");		//Poco suffix adds "d.dll" in debug mode :(
#else
	newName.append(mTheLib.suffix());
#endif

    if(newName != mLibName && isLoaded())
    {
    	//Unload the "old" model dll
        unload();
    }
    mLibName = newName;
	return mLibName;
}

}

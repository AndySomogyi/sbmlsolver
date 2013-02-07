#pragma hdrstop
#include "Poco/UUIDGenerator.h"
#include "Poco/UUIDGenerator.h"
#include "rrModelSharedLibrary.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------

using Poco::UUID;
using Poco::UUIDGenerator;
using namespace rr;
namespace rr
{

ModelSharedLibrary::ModelSharedLibrary(const string& pathTo)
:
mSharedLibFileName(getFileName()),
mPathToLib(pathTo)
{}

ModelSharedLibrary::~ModelSharedLibrary()
{}

bool ModelSharedLibrary::setPath(const string& pathTo)
{
	mPathToLib = pathTo;
    return true;
}

bool ModelSharedLibrary::load()
{
	Poco::SharedLibrary::load(JoinPath(mPathToLib, mSharedLibFileName));
    return true;
}

string ModelSharedLibrary::getFileName()
{
	if(mSharedLibFileName.size())
    {
    	return mSharedLibFileName;
    }

    return createName();
}

string ModelSharedLibrary::getFullFileName()
{
	if(mSharedLibFileName.size())
    {
    	return JoinPath(mPathToLib, mSharedLibFileName);
    }

    return JoinPath(mPathToLib, createName());
}

string ModelSharedLibrary::createName()
{
	//Create  a new UUID
	UUIDGenerator& generator = UUIDGenerator::defaultGenerator();
	UUID uuid2(generator.createRandom());
    mSharedLibFileName = uuid2.toString();
    mSharedLibFileName.append(suffix());

	return mSharedLibFileName;
}

}
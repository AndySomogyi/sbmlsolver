#pragma hdrstop
#include <vector>
#include "rrMemoLogger.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;
using namespace rr;
//Minimalistic logger to a memo...
TMemoLogger::TMemoLogger(TMemo* aMemo)
:
mMemo(aMemo)
{}

TMemoLogger::~TMemoLogger()
{
	vector<string> lines = splitString(mStream.str(),"\n");

    for(int i = 0; i < lines.size(); i++)
    {
	    mMemo->Lines->Add(lines[i].c_str());
    }
}

std::ostringstream& TMemoLogger::Get()
{
	return mStream;
}


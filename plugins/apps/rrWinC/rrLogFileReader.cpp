#ifdef MTK_PCH
#include "mtk_pch.h"
#endif
#pragma hdrstop
#include <Classes.hpp>
#include "rrUtils.h"
#include "rrLogFileReader.h"
#include "rrLogger.h"
#include "MainForm.h"
//---------------------------------------------------------------------------

LogFileReader::LogFileReader(const string& file, TMainF* mainForm)
:
mFileName(file),
mMainForm(mainForm)
{
    if(fileExists(mFileName.c_str()))
    {
        mFS.open(file.c_str());
    }
}

void LogFileReader::SetFileName(const string& fName)
{
    mFileName = fName;
}

bool LogFileReader::isActive()
{
	return mThread.isRunning();
}

void LogFileReader::start()
{
	mIsTimeToDie = false;
	mThread.start(*this);
}

void LogFileReader::run()
{
	worker();
}

void LogFileReader::shutDown()
{
	mIsTimeToDie = true;
}

void LogFileReader::worker()
{
    //First advance to end
    if(!mFS.is_open())
    {
        mFS.open(mFileName.c_str());
        if(!mFS)
        {
            mIsTimeToDie = true;
        }
    }
    mFS.seekg (0, ios::end);
    streampos pos = mFS.tellg();
    streampos lastPosition = pos;

	while(!mIsTimeToDie)
	{
        //Read until end of file
        while(!mFS.eof())
        {
            char* data = new char[2048];
            mFS.getline(data, 2048);

            if(strlen(data) > 1)
            {
                if(mMainForm)
                {
                    while(mMainForm->mLogString)	//The main form consumes and sets to NULL
                    {
                        ;
                    }
                    mMainForm->mLogString = new string(data);
                    TThread::Synchronize(NULL, mMainForm->LogMessage);
                }
            }
            delete [] data;

        }
        if(mFS.fail())
        {
            mFS.clear();
        }
        pos = mFS.tellg();
        Sleep(100);    		//Instead of a sleep, a OS call, onFileChange could be used...
	}
    mMainForm->mLogString = new string("Last log message..");
    TThread::Synchronize(NULL, mMainForm->LogMessage);
}


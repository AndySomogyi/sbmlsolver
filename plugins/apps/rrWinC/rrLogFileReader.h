#ifndef rrLogFileReaderH
#define rrLogFileReaderH
//---------------------------------------------------------------------------
#include <string>
#include <fstream>
#include "Poco/Thread.h"
#include "Poco/Runnable.h"

class TMainF;

using namespace std;
class LogFileReader : public Poco::Runnable
{
    protected:
        ifstream                mFS;
        string                  mFileName;
        TMainF*                 mMainForm;
        bool 					mIsTimeToDie;
	    Poco::Thread 			mThread;

    public:
                                LogFileReader(const string& fName = "", TMainF* mainForm = NULL);
        void                    worker();
        void                    SetFileName(const string& fName);

        void					start();
        void					run();
        bool					isActive();
        void					shutDown();
};

#endif

#ifndef rrLogContainerH
#define rrLogContainerH
//---------------------------------------------------------------------------
#include <sstream>
#include <string>
#include <stdio.h>
#include "rrObject.h"
#include "rrLogLevel.h"

using std::string;
using std::ostringstream;

namespace rr
{

template<class T>
class RR_DECLSPEC LogContainer
{
public:
    LogContainer();
    virtual ~LogContainer();
    std::ostream& Get(const LogLevel& level);
    string GetCurrentLogLevel();

private:
    LogLevel mCurrentLogLevel;
    LogContainer(const LogContainer&);    //Don't copy this one..

    std::ostringstream mOutputStream;
};

}
#endif

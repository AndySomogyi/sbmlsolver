#pragma hdrstop
#include <iomanip>
#include "rrStringBuilder.h"
#include "rrStringUtils.h"
#include "rrLogger.h"
//---------------------------------------------------------------------------


namespace rr
{

StringBuilder::StringBuilder(const std::string& aStr)
{
    mStringing<<aStr;
}

std::string StringBuilder::ToString()
{
    return mStringing.str();
}

void StringBuilder::Clear()
{
    mStringing.str("");
}

std::stringstream& StringBuilder::operator<<(const std::string& str)
{
    mStringing<<str;
    rrLog(lDebug5)<<"Appended :"<<removeNewLines(str, 1);
    return mStringing;
}

std::stringstream& StringBuilder::operator<<(const char& ch)
{
    mStringing<<ch;
    rrLog(lDebug5)<<"Appended :"<<ch;
    return mStringing;
}

void StringBuilder::NewLine(const std::string& line)
{
    mStringing<<"\n"<<line<<std::endl;
}

void StringBuilder::Line(const std::string& line)
{
    mStringing<<line<<std::endl;
}

void StringBuilder::TLine(const std::string& line, const int& nrTabs)
{
    std::string tabs;
    for(int i = 0; i < nrTabs; i++)
    {
        tabs +="\t";
    }

    mStringing<<tabs<<line<<std::endl;
}

}

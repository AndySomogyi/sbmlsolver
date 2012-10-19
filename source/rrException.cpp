#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrException.h"
//---------------------------------------------------------------------------


namespace rr
{

Exception::Exception(const string& desc)
:
mMessage(desc)//, Message(mMessage)
{
}

Exception::~Exception() throw() {}

const char* Exception::what() const throw()
{
    return mMessage.c_str();
}

string Exception::Message() const
{
    return mMessage;
}

RRException::RRException(const string& msg)
:
Exception(msg)
{}

SBWApplicationException::SBWApplicationException(const string& msg)
:
RRException(msg)
{}

SBWApplicationException::SBWApplicationException(const string& msg1, const string& msg2)
:
RRException(msg1 + msg2)
{}

SBWException::SBWException(const string& msg)
:
RRException(msg)
{}

ScannerException::ScannerException(const string& msg)
:
RRException(msg)
{}

NLEQException::NLEQException(const string& msg)
:
RRException(msg)
{}

CvodeException::CvodeException(const string& msg)
:
RRException(msg)
{}

}

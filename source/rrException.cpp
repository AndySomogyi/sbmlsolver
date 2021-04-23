#pragma hdrstop

#include "rrException.h"
//---------------------------------------------------------------------------


namespace rr {

    Exception::Exception(const std::string &desc) : mMessage(desc) {}

    Exception::~Exception() throw() {}

    const char *Exception::what() const throw() {
        return mMessage.c_str();
    }

    std::string Exception::Message() const {
        return mMessage;
    }

    std::string Exception::getMessage() const {
        return mMessage;
    }

    CoreException::CoreException(const std::string &msg) : Exception(msg) {}

    CoreException::CoreException(const std::string &msg1, const std::string &msg2) : Exception(msg1 + msg2) {}

    ScannerException::ScannerException(const std::string &msg) : Exception(msg) {}

    NLEQException::NLEQException(const std::string &msg) : Exception(msg) {}

    NOMException::NOMException(const std::string &msg) : Exception(msg) {}

    CVODEException::CVODEException(const std::string &msg) : Exception(msg) {}

    KinsolException::KinsolException(const std::string &msg) : Exception(msg) {}

    DidNotConvergeException::DidNotConvergeException(const std::string &msg) : Exception(msg) {}

    NullPointerException::NullPointerException(const std::string &msg) : Exception(msg) {}

    NotImplementedException::NotImplementedException(const std::string &msg) : Exception(msg) {}

    InvalidKeyException::InvalidKeyException(const std::string &msg) : Exception(msg) {}

    UninitializedValueException::UninitializedValueException(const std::string &msg) : Exception(msg) {}

    void UninitializedValue(const std::string &msg) {
        throw UninitializedValueException(msg);
    }

}

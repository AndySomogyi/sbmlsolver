// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Integrator.h
* @author ETS, JKM
* @date 2014
* @copyright Apache License, Version 2.0
* @brief RoadRunner exception classes
**/

#ifndef rrExceptionH
#define rrExceptionH
#include <exception>
#include <string>
#include <variant>
#include "rrExporter.h"

using std::string;
using std::exception;

namespace rr
{

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

    /** @class Exception
    * The RoadRunner exception class.
    */

    class RR_DECLSPEC Exception : public std::exception
    {
    protected:
        std::string mMessage;   //Exception message

    public:
        //std::string& Message;
        Exception(const std::string& desc);
        virtual                ~Exception() throw();
        virtual const char* what() const throw();
        std::string                  Message() const;
        std::string                  getMessage() const;
    };


    /** @class CoreException
    * Generic exception class.
    */
    class RR_DECLSPEC CoreException : public Exception
    {
    public:
        CoreException(const std::string& msg);
        CoreException(const std::string& msg1, const std::string& msg2);
    };

    /** @class ScannerException
    * Exception class for scanners.
    */
    class RR_DECLSPEC ScannerException : public Exception
    {
    public:
        ScannerException(const std::string& msg);
    };

    /** @class NLEQException
    * Exception class for scanners.
    */
    class RR_DECLSPEC NLEQException : public Exception
    {
    public:
        NLEQException(const std::string& msg);
    };

    /** @class NOMException
    * Exception class for scanners.
    */
    class RR_DECLSPEC NOMException : public Exception
    {
    public:
        NOMException(const std::string& msg);
    };

    /** @class CVODEException
    * Exception class for scanners.
    */
    class RR_DECLSPEC CVODEException : public Exception
    {
    public:
        CVODEException(const std::string& msg);
    };

    /** @class KinsolException
    * Exception class for scanners.
    */
    class RR_DECLSPEC KinsolException : public Exception
    {
    public:
        explicit KinsolException(const std::string& msg);
    };

    /** @class DidNotConvergeException
    * Exception class for scanners.
    */
    class RR_DECLSPEC DidNotConvergeException : public Exception
    {
    public:
        explicit DidNotConvergeException(const std::string& msg);
    };

    /** @class NullPointerException
    * Exception class for scanners.
    */
    class RR_DECLSPEC NullPointerException : public Exception
    {
    public:
        explicit NullPointerException(const std::string& msg);
    };

    /** @class NotImplementedException
    * Exception class for scanners.
    */
    class RR_DECLSPEC NotImplementedException : public Exception
    {
    public:
        NotImplementedException(const std::string& msg);
    };

    /** @class InvalidKeyException
    * Exception class for scanners.
    */
    class RR_DECLSPEC InvalidKeyException : public Exception
    {
    public:
        explicit InvalidKeyException(const std::string& msg);
    };


    /**
     * @class UninitializedValueException
     * @brief Thrown whenever an uninitialized value is encountered,
     * see https://github.com/sys-bio/roadrunner/issues/180
     */
    class RR_DECLSPEC UninitializedValueException : public Exception
    {
    public:
        UninitializedValueException(const std::string& msg);
    };

    /**
     * @author JKM
     * @brief Called whenever an uninitialized value is encountered,
     * throws @ref UninitializedValueException
     */
    void UninitializedValue(const std::string& msg);

}//namepsace rr
#endif

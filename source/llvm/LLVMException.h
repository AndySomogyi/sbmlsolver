/*
 * LLVMException.h
 *
 *  Created on: Jun 30, 2013
 *      Author: andy
 */

#ifndef RRLLVMEXCEPTION_H_
#define RRLLVMEXCEPTION_H_

#include <stdexcept>
#include "rrLogger.h"
#include "rrOSSpecifics.h"

namespace rrllvm
{

class LLVMException: public std::runtime_error
{
public:
    explicit LLVMException(const std::string& what) :
            std::runtime_error(what)
    {
        Log(rr::Logger::LOG_DEBUG) << __FUNC__ << "what: " << what;
    }

    explicit LLVMException(const std::string& what, const std::string &where) :
            std::runtime_error("Error in " + where + ": " + what)
    {
        Log(rr::Logger::LOG_DEBUG) << __FUNC__ << "what: " << what << ", where: " << where;
    }
};

#define throw_llvm_exception(what) \
        { std::string _err_msg = std::string("Error in ") + \
          std::string(__FUNC__) + ": " + std::string(what); \
          throw LLVMException(_err_msg); }


}

#endif /* RRLLVMEXCEPTION_H_ */

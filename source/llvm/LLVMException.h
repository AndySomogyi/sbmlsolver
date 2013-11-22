/*
 * LLVMException.h
 *
 *  Created on: Jun 30, 2013
 *      Author: andy
 */

#ifndef RRLLVMEXCEPTION_H_
#define RRLLVMEXCEPTION_H_

#include <stdexcept>
#include <Poco/Logger.h>
#include "rrLogger.h"

namespace rrllvm
{

class LLVMException: public std::runtime_error
{
public:
    explicit LLVMException(const std::string& what) :
            std::runtime_error(what)
    {
    }

    explicit LLVMException(const std::string& what, const std::string &where) :
            std::runtime_error("Error in " + where + ": " + what)
    {
    }
};

#define throw_llvm_exception(what) \
        { std::string _err_msg = std::string("Error in ") + \
          std::string(__FUNC__) + ": " + std::string(what); \
          poco_error(rr::getLogger(), _err_msg); \
          throw LLVMException(_err_msg); }


}

#endif /* RRLLVMEXCEPTION_H_ */

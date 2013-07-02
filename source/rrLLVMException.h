/*
 * rrLLVMException.h
 *
 *  Created on: Jun 30, 2013
 *      Author: andy
 */

#ifndef RRLLVMEXCEPTION_H_
#define RRLLVMEXCEPTION_H_

#include <stdexcept>

namespace rr
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

}

#endif /* RRLLVMEXCEPTION_H_ */

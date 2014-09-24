/*
 * LLVMRandom.h
 *
 *  Created on: Sep 23, 2014
 *      Author: andy
 */

#ifndef _RRLLVM_RANDOM_H_
#define _RRLLVM_RANDOM_H_

// bugs in gcc 44 c++ random generator
#if (__cplusplus >= 201103L) || defined(_MSC_VER)
    #include <random>
    #define cxx11_ns std
    #define RR_CXX_RANDOM 1
#elif  __clang_major__ >= 4 || defined(__APPLE__)
    #include <tr1/random>
    #define cxx11_ns std::tr1
    #define RR_CXX_RANDOM 1
#else
    #include <stdlib.h>
    #include <sys/time.h>
#endif

namespace rrllvm
{

class Random
{
public:
    /**
     * creates a new Random object and adds the distrib as global mappings
     * to the execution engine.
     */
    Random(class ModelGeneratorContext& ctx);

    /**
     * copy constructor, copy the distributions but reset the RNG to what
     * is specified by the config.
     */
    Random(const Random& other);

    /**
     * assignment operator, copies the fields from the
     * other object, but does not re-intialize them.
     */
    Random& operator=( const Random& rhs);

    ~Random();

    /**
     * RNG engine.
     */
    cxx11_ns::mt19937 engine;
};



} /* namespace rrllvm */

#endif /* _RRLLVM_RANDOM_H_ */

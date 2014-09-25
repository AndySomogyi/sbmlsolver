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

#include <stdint.h>

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

    /**
     * default ctor, this obviously does not add any mappings to a
     * ModelGeneratorContext. This would be used when a Random is created
     * by itself, to use the attached RNG engine.
     */
    Random();

    ~Random();

    /**
     * return a normalized random number between 0 and 1 using the
     * RNG engine.
     */
    double operator()();

    /**
     * min random number. MSVC looks at this, but gcc stdlib assumes normalized dist.
     */
    double min() { return 0.0; };

    /**
     * max random number. MSVC looks at this, but gcc stdlib assumes normalized dist.
     */
    double max() { return 1.0; };

    /**
     * Try to hide the RNG, so we can use different RNGs in the future.
     * set the seed used by the random number generator. This will by definition
     * reset the RNG.
     */
    void setRandomSeed(int64_t);

    /**
     * Try to hide the RNG so we can used different RNGs in the future.
     * get the seed used by the RNG.
     */
    int64_t getRandomSeed();

    /**
     * RNG engine.
     */
    cxx11_ns::mt19937 engine;

private:
    // seed that was used to seed the engine.
    int64_t randomSeed;
};



} /* namespace rrllvm */

#endif /* _RRLLVM_RANDOM_H_ */

/*
 * LLVMModelGenerator.h
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */
#ifndef rrLLVMModelGeneratorH
#define rrLLVMModelGeneratorH

#include "rrRoadRunnerOptions.h"
#include "LLVMCompiler.h"

#if (__cplusplus >= 201103L) || defined(_MSC_VER)
#include <memory>
#include <unordered_map>
#define cxx11_ns std
#else
#include <tr1/memory>
#include <tr1/unordered_map>
#define cxx11_ns std::tr1
#endif

namespace rrllvm
{

/**
 * General concepts:
 *
 * SBML defines chemical species.
 *
 * Floating Species: species whose value varies over time, i.e. they can 'float'
 * Boundary Species: boundary condition species, values are fixed to initial conditions.
 */
class RR_DECLSPEC LLVMModelGenerator
{
public:

    /**
     * Create an executable model from an sbml string
     */
    static rr::ExecutableModel *createModel(const std::string& sbml, uint options);

};

} /* namespace rr */
#endif /* rrLLVMModelGeneratorH */

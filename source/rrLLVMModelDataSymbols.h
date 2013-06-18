/*
 * rrLLVMModelDataSymbols.h
 *
 * Created on: Jun 17, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef rrLLVMModelDataSymbolsH
#define rrLLVMModelDataSymbolsH

namespace rr
{

/**
 * A class with a long name...
 *
 * In RoadRunner, all model data is stored appropriately in the ModelData class,
 * as arrays of concentrations, parameters, etc...
 *
 * This class provides symbolic access to all of this data for the LLVM code
 * generator, i.e. this class is the symbol table for all the named SBML values.
 */
class LLVMModelDataSymbols
{
public:
    LLVMModelDataSymbols();
    ~LLVMModelDataSymbols();
};

} /* namespace rr */
#endif /* rrLLVMModelDataSymbolsH */

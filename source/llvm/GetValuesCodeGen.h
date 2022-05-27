/*
 * GetValuesCodeGen.h
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */

#ifndef RRLLVM_GETVALUESCODEGEN_H_
#define RRLLVM_GETVALUESCODEGEN_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include "GetValueCodeGenBase.h"
#include <sbml/Model.h>

namespace rrllvm
{

    /** @class GetBoundarySpeciesAmountCodeGen
    * Get the current amount of a boundary species.
    */
    class GetBoundarySpeciesAmountCodeGen : public
        GetValueCodeGenBase<GetBoundarySpeciesAmountCodeGen, true>
    {
    public:
        GetBoundarySpeciesAmountCodeGen(const ModelGeneratorContext& mgc);
        ~GetBoundarySpeciesAmountCodeGen() {};

        std::vector<std::string> getIds()
        {
            return dataSymbols.getBoundarySpeciesIds();
        }

        static const char* FunctionName;
        static const char* IndexArgName;
    };

    /** @class GetFloatingSpeciesAmountCodeGen
    * Get the current amount of a floating species.
    */
    class GetFloatingSpeciesAmountCodeGen : public
        GetValueCodeGenBase<GetFloatingSpeciesAmountCodeGen, true>
    {
    public:
        GetFloatingSpeciesAmountCodeGen(const ModelGeneratorContext& mgc);
        ~GetFloatingSpeciesAmountCodeGen() {};

        std::vector<std::string> getIds()
        {
            return dataSymbols.getFloatingSpeciesIds();
        }

        static const char* FunctionName;
        static const char* IndexArgName;
    };

    /** @class GetBoundarySpeciesConcentrationCodeGen
    * Get the current concentration of a boundary species.
    */
    class GetBoundarySpeciesConcentrationCodeGen : public
        GetValueCodeGenBase<GetBoundarySpeciesConcentrationCodeGen, false>
    {
    public:
        GetBoundarySpeciesConcentrationCodeGen(const ModelGeneratorContext& mgc);
        ~GetBoundarySpeciesConcentrationCodeGen() {};

        std::vector<std::string> getIds()
        {
            return dataSymbols.getBoundarySpeciesIds();
        }

        static const char* FunctionName;
        static const char* IndexArgName;
    };

    /** @class GetFloatingSpeciesConcentrationCodeGen
    * Get the current concentration of a floating species.
    */
    class GetFloatingSpeciesConcentrationCodeGen : public
        GetValueCodeGenBase<GetFloatingSpeciesConcentrationCodeGen, false>
    {
    public:
        GetFloatingSpeciesConcentrationCodeGen(const ModelGeneratorContext& mgc);
        ~GetFloatingSpeciesConcentrationCodeGen() {};

        std::vector<std::string> getIds()
        {
            return dataSymbols.getFloatingSpeciesIds();
        }

        static const char* FunctionName;
        static const char* IndexArgName;
    };

    /** @class GetCompartmentVolumeCodeGen
    * Get the current volume of a compartment.
    */
    class GetCompartmentVolumeCodeGen : public
        GetValueCodeGenBase<GetCompartmentVolumeCodeGen, false>
    {
    public:
        GetCompartmentVolumeCodeGen(const ModelGeneratorContext& mgc);
        ~GetCompartmentVolumeCodeGen() {};

        std::vector<std::string> getIds()
        {
            return dataSymbols.getCompartmentIds();
        }

        static const char* FunctionName;
        static const char* IndexArgName;
    };

    /** @class GetGlobalParameterCodeGen
    * Get the current value of a parameter.
    */
    class GetGlobalParameterCodeGen : public
        GetValueCodeGenBase<GetGlobalParameterCodeGen, false>
    {
    public:
        GetGlobalParameterCodeGen(const ModelGeneratorContext& mgc);
        ~GetGlobalParameterCodeGen() {};

        std::vector<std::string> getIds()
        {
            return dataSymbols.getGlobalParameterIds();
        }

        static const char* FunctionName;
        static const char* IndexArgName;
    };



} /* namespace rr */
#endif /* RRLLVM_GETVALUESCODEGEN_H_ */

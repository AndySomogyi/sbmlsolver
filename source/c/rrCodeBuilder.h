#ifndef rrCodeBuilderH
#define rrCodeBuilderH
#include "rrStringBuilder.h"

namespace rr
{

class RR_DECLSPEC CodeBuilder : public StringBuilder
{
    protected:
        int                     mSizeOfVarField1;
        int                     mSizeOfVarField2;
        int                     mSizeOfVarField3;
        std::string                  mDeclSpec;
        std::string                  mCallingConvention;

    public:
                                CodeBuilder(const std::string& aStr = "", const std::string& decl_spec = "D_S", const std::string& call_conv = "__cdecl");
        void                    FormatVariable(const std::string& type, const std::string& varName, const std::string& comment = "");
        void                    AddFunctionExport(const std::string& retValue, const std::string& funcProto);
        void                    AddFunctionProto(const std::string& retValue, const std::string& funcProto);
        void                    FormatArray(const std::string& type, const std::string& varName, const int& arraySize, const std::string& comment = "");
};

}

#endif

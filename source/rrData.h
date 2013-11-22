#ifndef rrDataH
#define rrDataH
#include <string>
#include "rrExporter.h"
#include "rrIniFile.h"
//---------------------------------------------------------------------------
/* Abstract class for data.. */

namespace rr
{
class IniSection;

/**
 * @internal
 */
class RR_DECLSPEC Data
{
    private:
        IniFile             mIniFile;    //Read/Write inifile format
        IniSection          *mDataSection;
        IniSection          *mInfoSection;


    public:
                            Data(const std::string& fName = "");
        virtual             ~Data();
        virtual             void Load(const std::string& fName = "") = 0;
        virtual             void Save(const std::string& fName = "") = 0;

};
}
#endif

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

class RR_DECLSPEC Data : public rrObject
{
	private:
		IniFile			    mIniFile;	//Read/Write inifile format
        IniSection		   *mDataSection;
        IniSection		   *mInfoSection;


    public:
	    					Data(const std::string& fName = EmptyString);
        				   ~Data();
		virtual 			Load(const std::string& fName = EmptyString);
		virtual 			Save(const std::string& fName = EmptyString);

};
}
#endif

#ifndef rrIniSectionH
#define rrIniSectionH
#include <vector>
#include <string>
#include "rrObject.h"
#include "rrIniKey.h"

namespace rr
{

class IniKey;
typedef std::vector<IniKey*>     		KeyList;
typedef KeyList::iterator 		    	KeyItor;
typedef std::vector<string> 	    	NonKeyList;
typedef NonKeyList::iterator 		    NonKeyItor;


// This class stores the definition of a section. A section contains any number
// of keys (see IniKeys), and may or may not have a comment.
class RR_DECLSPEC IniSection : public rrObject
{
    private:
        bool            mIsDirty;

	public:
		string		    mName;
		string		    mComment;
		KeyList		    mKeys;			//vector of pointers to keys
		NonKeyList 	    mNonKeys; 		//vector of pointers to non_keys

                        IniSection();
                        IniSection(const std::string& nameValueString, const char& sep);
                       ~IniSection();
        IniKey*  		CreateKey(const string& _keyName, const string& Value = gEmptyString, const string& Comment = gEmptyString);
        IniKey*			GetKey(const int& i);
        IniKey*			GetKey(const string& keyName, bool create = false);
        size_t 			KeyCount(){return mKeys.size();}
        size_t 			NonKeyCount(){return mNonKeys.size();}
        void			Clear(){mKeys.clear(); mNonKeys.clear();}
	   	string			GetNonKeysAsString();
        string			AsString();
};
}
#endif

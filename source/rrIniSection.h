#ifndef rrIniSectionH
#define rrIniSectionH
#include <vector>
#include <string>
#include "rrExporter.h"
#include "rrIniKey.h"

namespace rr
{

class IniKey;
typedef std::vector<IniKey*>             KeyList;
typedef KeyList::iterator                 KeyItor;
typedef std::vector<std::string>             NonKeyList;
typedef NonKeyList::iterator             NonKeyItor;


/**
 * @internal
 * This class stores the definition of a section. A section contains any number
 * of keys (see IniKeys), and may or may not have a comment.
 */
class RR_DECLSPEC IniSection
{
    private:
        bool            mIsDirty;

    public:
        bool              mIsUsed;
        std::string            mName;
        std::string            mComment;
        KeyList           mKeys;            //std::vector of pointers to keys
        NonKeyList        mNonKeys;         //std::vector of pointers to non_keys

                        IniSection();
                        IniSection(const std::string& nameValueString, const char& sep);
                       ~IniSection();
        IniKey*          CreateKey(const std::string& _keyName, const std::string& Value = "", const std::string& Comment = "");
        IniKey*            GetKey(const int& i);
        IniKey*            GetKey(const std::string& keyName, bool create = false);
        size_t             KeyCount(){return mKeys.size();}
        size_t             NonKeyCount(){return mNonKeys.size();}
        void            Clear(){mKeys.clear(); mNonKeys.clear();}
           std::string            GetNonKeysAsString();
        std::string            AsString();
};
}
#endif

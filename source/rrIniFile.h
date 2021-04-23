#ifndef IniFileH
#define IniFileH
#include <vector>
#include <fstream>
#include <string>
#include "rrExporter.h"
#include "rrStringUtils.h"
#include "rrIniSection.h"
#include "rrIniKey.h"
#include "rrFileName.h"

namespace rr
{

using std::string;
using std::fstream;
using std::ios;
using std::ios_base;

const int MAX_LINE_BUFFER_SIZE = 2048;
class IniKey;
class IniSection;

// IniFile typedefs
typedef std::vector<IniKey*>             KeyList;
typedef std::vector<std::string>             NonKeyList;
typedef std::vector<IniSection*>          SectionList;
typedef SectionList::iterator           SectionItor;

/**
 * @internal
 */
class RR_DECLSPEC IniFile
{
    private:
        // When set, this define will cause SetValue() to create a new section, if
        // the requested section does not allready exist.
        const int           mAutoCreateSections;

        // When set, this define causes SetValue() to create a new key, if the
        // requested key does not allready exist.
        const int           mAutoCreateKeys;

        std::string              mCommentIndicators;
        const std::string        mEqualIndicator;
        const std::string        mWhiteSpace;

        // General Purpose Utility Functions
        std::string              GetNextWord(std::string& CommandLine);
        std::string              Trim(std::string& Str);
        int                 WriteLine(std::fstream& stream, const char* fmt, ...);

    protected:
        SectionList         mSections;        // List of sections
        FileName            mIniFileName;    // The filename to write to
        bool                mIsDirty;        // Tracks whether or not data has changed.
        bool                mWasFound;
        bool                mAutoSave;        //Save ini file automatically on destruction


    public:
        int                 mFlags;        // Our settings flags.
                            IniFile(const std::string& fName = "", bool autoLoad = false, bool autoSave = false);
        virtual                ~IniFile();

        size_t             GetNumberOfSections(){return mSections.size();}
        IniSection*        GetSection(int i){return mSections[i];}
        IniSection*        GetSection(size_t i) { return mSections[i]; }

        // File handling methods
        std::string             GetFilePath(){return mIniFileName.GetPath();}
        std::string             GetFileName(){return mIniFileName;}
        std::string             GetFullFileName(){return mIniFileName.GetPathAndFileName();}
        bool               SetFilePath(const std::string& path);
        bool               Load(const std::string& fName = "");
        IniSection*        LoadSection(const std::string& theSection);
        bool               Save(ios_base::openmode openMode = std::ios::out|std::ios::trunc);
        bool               UnLoad(){return Save();}
        bool               WasItFound(){return mWasFound;} //!Boolean indicating if the last key was found in the ini file
        bool               SectionExists(const std::string& section);

        //Reading
        // ReadValue: Default access method. Returns the raw std::string value
        std::string                ReadValue(const std::string& Key, const std::string& Section = "");
         std::string                ReadString(const std::string& Key, const std::string& Section = "", const std::string& def_val="");
        double                ReadDouble(const std::string& Key, const std::string& Section = "", double def_value = 0);
        float                ReadFloat(const std::string& Key, const std::string& Section = "", double def_value = 0){return (float) ReadDouble(Key, Section, def_value);}
        int                    ReadInteger(const std::string& Key, const std::string& Section = "", int def_value = 0);
        bool                ReadBool(const std::string& Key, const std::string& Section = "", bool def_value = false);

           //Writing
         bool                WriteValue(const std::string& Key, const std::string& Value, const std::string& Comment = "", const std::string& Section = "");
        bool                WriteFloat(const std::string& Key, double value, const std::string& Comment = "", const std::string& Section = "");
        bool                WriteInteger(const std::string& Key, int nValue, const std::string& Comment = "", const std::string& Section = "");
        bool                WriteBool(const std::string& Key, bool bValue, const std::string& Comment = "", const std::string& Section = "");
        bool                WriteString(const std::string& Key, const std::string& Value, const std::string& Comment = "", const std::string& Section = ""){return WriteValue(Key, Value, Comment, Section);}
         bool                WriteNonKey(const std::string& nonKey, const std::string& Section = "");

        //KeyHandling
        bool                SetKeyComment(const std::string& Key, const std::string& Comment, const std::string& Section = "");
        bool                SetSectionComment(const std::string& Section, const std::string& Comment);
        bool                DeleteKey(const std::string& Key, const std::string& FromSection = "");
        bool                DeleteSection(const std::string& Section);
        bool                DeleteSectionsWithKeyValue(const std::string& key, const std::string& value);
        bool                CreateKey(const std::string& Key, const std::string& Value, const std::string& Comment = "", const std::string& Section = "");

        //Section stuff
        bool                CreateSection(const std::string& Section, const std::string& Comment = "");
        bool                CreateSection(const std::string& Section, const std::string& Comment, KeyList Keys);

        // Utility Methods
        size_t              SectionCount();
        size_t              KeyCount();
        size_t              KeyCount(const std::string& section);
        void                Clear();
        void                SetFileName(const std::string& fName);
        std::string              CommentStr(std::string& Comment);
        IniKey*             GetKey(const std::string& Key, const std::string& Section);
        IniSection*         GetSection(const std::string& Section, bool create = false);
        IniSection*         GetSection(const unsigned int secNr);
        bool                ClearSection(const std::string& section);
        bool                IsModified(){return mIsDirty;}
};

}
#endif

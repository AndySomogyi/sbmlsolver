#ifndef rrStringListH
#define rrStringListH
#include <vector>
#include <string>
#include "rrConstants.h"
#include "../wrappers/C/rrc_types.h"

namespace rr
{
using std::vector;
using std::string;
using std::ostream;

/**
 * @internal
 * @deprecated
 */
class RR_DECLSPEC StringList
{
    protected:
        std::vector<std::string>              mStrings;
        std::vector<std::string>::iterator    mLI;

    public:
                                    StringList();
                                    StringList(char** stringList, const int& count);
                                    StringList(const std::string& str, const std::string& delimiters = ", ");
                                    StringList(const std::vector<std::string>& strings);
                                    StringList(const StringList& cp);
                                    StringList(rrc::RRStringArrayPtr cp);
                                   ~StringList();

        void                        InsertAt(const int& index, const std::string& item);
        void                        Append(const StringList& list);
        std::string                      AsString(const std::string& delimiter = gComma) const;

        /**
         * get the size to be compatible with std::vector<std::string>
         */
        unsigned int size() const;


        unsigned int                Count() const;


        StringList&                 operator=(const StringList& rhs);
        StringList&                 operator=(const std::vector<std::string>& rhs);

        std::string&                     operator[](const int& index);
        const std::string&               operator[](const int& index) const;
        StringList                  operator-(const StringList& rhs);
        void                        removeAt(const int& index);
        int                         find(const std::string& item);
        int                         indexOf(const std::string& item);
        void                        clear();
        void                        empty();
        bool                        Contains(const std::string& item) const;
        bool                        DontContain(const std::string& item) const;
        void                        add(const std::string& item);
        std::vector<std::string>::iterator    begin();
        std::vector<std::string>::iterator    end();
        void                        PreFix(const std::string& fix);
        void                        PostFix(const std::string& fix);
        RR_DECLSPEC
        friend std::ostream&             operator<<(std::ostream& stream, const StringList& list);

        /**
         * so we can start getting rid of this and using
         * standard std::vector<std::string>
         */
        //operator std::vector<std::string>() const { return mStrings; }
        operator const std::vector<std::string>&() const { return mStrings;}
};



}
#endif

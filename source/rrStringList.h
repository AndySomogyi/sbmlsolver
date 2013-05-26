#ifndef rrStringListH
#define rrStringListH
#include <vector>
#include <string>
#include "rrObject.h"
#include "../Wrappers/C/rrc_types.h"

namespace rr
{
using std::vector;
using std::string;
using std::ostream;


class RR_DECLSPEC StringList : public rrObject
{
    protected:

        vector<string>              mStrings;
        vector<string>::iterator    mLI;

    public:
                                    StringList();
                                    StringList(char** stringList, const int& count);
                                    StringList(const string& str, const string& delimiters = ", ");
                                    StringList(const vector<string>& strings);
                                    StringList(const StringList& cp);
                                    StringList(rrc::RRStringArrayHandle cp);
                                   ~StringList();

        void                        InsertAt(const int& index, const string& item);
		void 						Append(const StringList& list);
        string                      AsString(const string& delimiter = gComma) const;
        unsigned int                Count() const;
        StringList&                 operator=(const StringList& rhs);
        StringList&                 operator=(const vector<string>& rhs);

        string&                     operator[](const int& index);
        const string&               operator[](const int& index) const;
        StringList                  operator-(const StringList& rhs);
		void						removeAt(const int& index);
        int                         find(const string& item);
        int                         indexOf(const string& item);
        void                        clear();
        void                        empty();
        bool                        Contains(const string& item) const;
        bool                        DontContain(const string& item) const;
        void                        add(const string& item);
        vector<string>::iterator    begin();
        vector<string>::iterator    end();
        void                        PreFix(const string& fix);
        void                        PostFix(const string& fix);
		RR_DECLSPEC
        friend ostream& 			operator<<(ostream& stream, const StringList& list);
};



}
#endif

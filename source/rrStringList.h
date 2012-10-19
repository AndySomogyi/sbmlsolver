#ifndef rrStringListH
#define rrStringListH
#include <vector>
#include <string>
#include "rrObject.h"

using std::vector;
using std::string;
using std::ostream;

namespace rr
{

class RR_DECLSPEC StringList : public rrObject
{
    protected:

        vector<string>              mStrings;
        vector<string>::iterator    mLI;    //ListITerator

    public:
           string                   mLabel;
                                    StringList();
                                    StringList(const string& str, const string& delimiter = ",");
                                    StringList(const vector<string>& strings);
                                    StringList(const StringList& cp);
                                   ~StringList();

        void                        Label(const string& lbl);
        void                        InsertAt(const int& index, const string& item);
        void                        Add(const string& str);
        string                      AsString(const string& delimiter = Comma) const;
        unsigned int                Count() const;
        void                        operator=(const StringList& rhs);
        string&                     operator[](const int& index);
        string                      operator[](const int& index) const;
        StringList                  operator-(const StringList& rhs);

        int                         find(const string& item);
        int                         IndexOf(const string& item);
        void                        clear();
        void                        empty();
        bool                        Contains(const string& item) const;
        bool                        DontContain(const string& item) const;
        void                        push_back(const string& item);
        vector<string>::iterator    begin();
        vector<string>::iterator    end();
        void                        PreFix(const string& fix);
        void                        PostFix(const string& fix);
RR_DECLSPEC friend ostream&         operator<<(ostream& stream, const StringList& list);
};



}
#endif

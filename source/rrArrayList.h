#ifndef rrArrayListH
#define rrArrayListH
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include "rrArrayListItem.h"

using namespace std;
namespace rr
{
class StringList;

class RR_DECLSPEC ArrayList : public rrObject
{
    protected:
    public:
        vector< ArrayListItemObject* >		mList; //List of ArrayListItemObject items

    public:
                                            ArrayList();
                                            ArrayList(const ArrayList& cpyMe);
                                            ArrayList(const string& lbl, const StringList& stringList);
                                            ArrayList(const string& lbl, const ArrayList& stringList);
                                           ~ArrayList();
        unsigned int                        Count() const;
        void                                Clear();
        void                                Add(const int& item);
        void                                Add(const double& item);
        void                                Add(const string& item);
        void                                Add(const ArrayList& item);
        void                                Add(const StringList& list);
        void                                Add(const string& lbl, const StringList& list);
        void                                Add(const string& lbl, const ArrayList& list);


        const ArrayListItemObject&          operator[](int pos) const;
        ArrayListItemObject&                operator[](int pos);
        void                                operator = (const ArrayList& rhs);
        StringList                          GetStringList(const string& lName);
        StringList                          GetStringList(const int& index);
        string                          	GetString(const int& index);
};


RR_DECLSPEC ostream& operator<<(ostream& stream, const ArrayList& list);

//typedef ArrayList StringArrayList;
}
#endif

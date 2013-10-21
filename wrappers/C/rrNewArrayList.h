#ifndef rrNewArrayListH
#define rrNewArrayListH
#include <vector>
#include <string>
#include <list>
#include <ostream>
#include "rrNewArrayListItem.h"
#include "rrc_exporter.h"

namespace rr
{
class StringList;
}

namespace rrc
{
using rr::StringList;
using std::vector;
using std::string;

/**
 * @internal
 * @deprecated
 * a proprietaty collection class that is massivly deprecated.
 */
class C_DECL_SPEC NewArrayList
{
    protected:
    public:
        vector< NewArrayListItemObject* >   mList; //List of NewArrayListItemObject items

    public:
                                            NewArrayList();
                                            NewArrayList(const NewArrayList& cpyMe);
                                            NewArrayList(const string& lbl, const StringList& stringList);
                                            NewArrayList(const string& lbl, const NewArrayList& stringList);
                                           ~NewArrayList();
        unsigned int                        Count() const;
        void                                Clear();
        void                                Add(const int& item);
        void                                Add(const double& item);
        void                                Add(const string& item);
        void                                Add(const NewArrayList& item);
        void                                Add(const StringList& list);
        void                                Add(const string& lbl, const StringList& list);
        void                                Add(const string& lbl, const NewArrayList& list);


        const NewArrayListItemObject&       operator[](int pos) const;
        NewArrayListItemObject&             operator[](int pos);
        void                                operator = (const NewArrayList& rhs);
        StringList                          GetStringList(const string& lName);
        StringList                          GetStringList(const int& index);
        string                              GetString(const int& index);
        string                              AsString();
};


C_DECL_SPEC std::ostream& operator<<(std::ostream& stream, const NewArrayList& list);

}
#endif

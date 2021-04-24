#ifndef rrStringListContainerH
#define rrStringListContainerH
#include <vector>
#include <string>
#include "rrExporter.h"
#include "rrStringList.h"
using std::vector;
using std::string;

namespace rr
{

/**
 * @internal
 * @deprecated
 */
class RR_DECLSPEC StringListContainer
{
    protected:
        std::vector<StringList>              mContainer;

    public:
                                        StringListContainer();
                                        StringListContainer(const std::string& lbl, const StringListContainer& cp);
                                        StringListContainer(const StringListContainer& cp);
                                        StringListContainer(const StringList& cp);
        void                            operator=(const StringListContainer& rhs);
        void                            Add(const StringListContainer& lists);
        void                            Add(const std::string& listName, const StringList& coll);
        void                            Add(const StringList& coll);
        void                            Add(const std::string& coll);
        void                            Add(const int& coll);
        int                             Count() const;
        int                             ListCount() const;
        int                             TotalCount() const;
        StringList&                     operator[](const int& index);
        const StringList&               operator[](const int& index) const;
        std::vector<StringList>::iterator    begin();
        std::vector<StringList>::iterator    end();
};

std::ostream& operator<<(std::ostream& stream, const StringListContainer& list);
}
#endif

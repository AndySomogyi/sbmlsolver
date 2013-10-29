#ifndef rrParametersH
#define rrParametersH
#include <string>
#include <vector>
#include "rrPluginsAPIExporter.h"
#include "rrBaseParameter.h"
#include "rrStringList.h"
//---------------------------------------------------------------------------

namespace rrp
{

using rr::StringList;
using std::vector;
using std::string;

/**
 * @internal
 * @deprecated
 */
class PLUGINS_API_DECLSPEC Parameters
{
    protected:
        vector<BaseParameter*>             mParas;

    public:
                                           Parameters();
        void                               add(BaseParameter* me);
        rr::StringList                     asStringList() const;
        u_int                              count() const;

        const BaseParameter*               operator[](const int& i) const;
        BaseParameter*                     operator[](const int& i);
        string                             getName() const;
        string                             getDescription() const;
        BaseParameter*                     getParameter(const string& paraName);
        void                               clear();

        PLUGINS_API_DECLSPEC friend ostream&       operator <<(ostream& os, const Parameters& pars);
};

}

#endif

#ifndef rrParametersH
#define rrParametersH
#include <string>
#include <vector>
#include <utility>
#include "rrPluginsAPIExporter.h"
#include "rrPluginParameter.h"
#include "rrStringList.h"
//---------------------------------------------------------------------------

namespace rrp
{
using std::vector;
using std::pair;
using rr::StringList;

using std::string;

class PLUGINS_API_DECLSPEC Parameters
{
    protected:
        vector< pair<PluginParameter*, bool> >    mParas;  //The container may owe the parameter. default is false

    public:
                                                Parameters();
        virtual                                ~Parameters();
        void                                    add(PluginParameter* me, bool own = false);
        rr::StringList                          asStringList() const;
        u_int                                   count() const;

        const PluginParameter*                    operator[](const int& i) const;
        PluginParameter*                          operator[](const int& i);
        string                                  getName() const;
        string                                  getDescription() const;
        PluginParameter*                          getParameter(const string& paraName);
        void                                    clear();

        PLUGINS_API_DECLSPEC
        friend ostream&                         operator <<(ostream& os, const Parameters& pars);
};

}

#endif

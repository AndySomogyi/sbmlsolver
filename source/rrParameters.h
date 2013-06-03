#ifndef rrParametersH
#define rrParametersH
#include <string>
#include <vector>
#include "rrObject.h"
#include "rrBaseParameter.h"
#include "rrStringList.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC Parameters : public rrObject
{
    protected:
        vector<BaseParameter*>          	mParas;

    public:
                                            Parameters();
        void                                add(BaseParameter* me);
        StringList                          asStringList() const;
        u_int                               count() const;

        const BaseParameter*   		        operator[](const int& i) const;
        BaseParameter*   			        operator[](const int& i);
        string                              getName() const;
        string                              getDescription() const;
        BaseParameter*	   					getParameter(const string& paraName);
        void								clear();

		RR_DECLSPEC friend ostream&   		operator <<(ostream& os, const Parameters& pars);
};

}

#endif

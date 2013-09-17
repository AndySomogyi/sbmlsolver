#ifndef rrRoadRunnerListH
#define rrRoadRunnerListH
#include <vector>
#include "rrExporter.h"
//---------------------------------------------------------------------------

namespace rr
{
using std::vector;
class RoadRunner;
class RR_DECLSPEC RoadRunnerList
{
    private:

    protected:
        vector<RoadRunner*>        mRRs;

    public:
                                RoadRunnerList(const int& nrOfRRs, const string& tempFolder = "");
        virtual                   ~RoadRunnerList();
        RoadRunner*                operator[](const int& index);
        unsigned int            count();

};

}
#endif

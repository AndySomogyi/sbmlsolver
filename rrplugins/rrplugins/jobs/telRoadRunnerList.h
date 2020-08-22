#ifndef telRoadRunnerListH
#define telRoadRunnerListH
#include <vector>
#include <string>
#include "telJobsExporter.h"
#include "telConstants.h"
//#include "rrObject.h"
//---------------------------------------------------------------------------

namespace tlp
{
//using namespace rr;
using std::vector;
class RoadRunner;

class JOBS_DECLSPEC RoadRunnerList //: public rrObject
{
    private:

    protected:
        vector<RoadRunner*>     mRRs;

    public:
                                RoadRunnerList(const int& nrOfRRs, const string& tempFolder = tlp::gEmptyString);
        virtual                ~RoadRunnerList();
        RoadRunner*             operator[](const int& index);
        unsigned int            count();

};

}
#endif

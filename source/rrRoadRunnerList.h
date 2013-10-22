#ifndef rrRoadRunnerListH
#define rrRoadRunnerListH
#include <vector>
#include <string>
#include "rrExporter.h"
//---------------------------------------------------------------------------

namespace rr
{
using std::vector;
class RoadRunner;

/**
 * @internal
 */
class RR_DECLSPEC RoadRunnerList
{
    private:

    protected:
        std::vector<RoadRunner*>        mRRs;

    public:
                                RoadRunnerList(const int& nrOfRRs, const std::string& tempFolder = "");
        virtual                   ~RoadRunnerList();
        RoadRunner*                operator[](const int& index);
        unsigned int            count();

};

}
#endif

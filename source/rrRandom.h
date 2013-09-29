#ifndef rrRandomH
#define rrRandomH
//---------------------------------------------------------------------------
#include "rrExporter.h"
#include "mtrand/mtrand.h"

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC Random
{

    private:
        MTRand          mRand;        //Get a double in [0, 1)
    public:
                        Random();
        double          NextDouble() const;
};

}
#endif

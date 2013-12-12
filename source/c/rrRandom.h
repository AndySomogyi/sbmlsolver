#ifndef rrRandomH
#define rrRandomH
//---------------------------------------------------------------------------

#include "mtrand.h"

namespace rr
{

/**
 * @internal
 */
class  Random
{

    private:
        MTRand          mRand;        //Get a double in [0, 1)
    public:
                        Random();
        double          NextDouble() const;
};

}
#endif

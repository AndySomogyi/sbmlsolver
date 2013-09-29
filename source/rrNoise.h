#ifndef rrNoiseH
#define rrNoiseH
#include "rrExporter.h"
//---------------------------------------------------------------------------

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC Noise
{
    private:
        double         mean;
        double         sigma;

    public:
                    Noise(double m = 0, double s = 1);
        double         getNoise();
        void        randomize();
};

}
#endif

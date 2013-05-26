#ifndef rrNoiseH
#define rrNoiseH
#include "rrObject.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC Noise : public rrObject
{
    private:
        double 		mean;
        double 		sigma;

    public:
        			Noise(double m = 0, double s = 1);
        double 		getNoise();
        void		randomize();
};

}
#endif

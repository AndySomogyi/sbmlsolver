#ifndef noiseH
#define noiseH
//---------------------------------------------------------------------------

namespace addNoise
{

class Noise
{
    private:
        double         mean;
        double         sigma;

    public:
                    Noise(double m = 0, double s = 1);
        double      getNoise();
        void        randomize();
};

}
#endif

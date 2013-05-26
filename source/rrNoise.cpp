#pragma hdrstop
#include "rrNoise.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------
namespace rr
{
Noise::Noise(double m, double s)
:
mean(m),
sigma(s)
{

}

double Noise::getNoise()
{
	return gaussNoise(mean, sigma);
}

void Noise::randomize()
{
   	srand( time( NULL ) );
}

}

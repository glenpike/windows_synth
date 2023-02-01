
#include <math.h>

#include "misc.h"

/* end of includes */

int g_Channels=2;
double g_SampleRate=44100.0;
double g_rSampleRate=1.0/g_SampleRate;

void SetSampleRate(double SampleRate)
{
	g_SampleRate=SampleRate;
	g_rSampleRate=1.0/g_SampleRate;
}

double dBToGain(double val)
{
  return pow(10.0,(val*.05));
}

double GainTodB(double val,double mindB)
{
	double l=mindB;

  if (val>0.0)
    l=20.0*log10(val);

	l=MAX(l,mindB);

	return l;
}



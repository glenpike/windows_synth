
#ifndef __AUDIO_MISC__
#define __AUDIO_MISC__

#ifndef __TYPES__
#define __TYPES__
typedef unsigned long ULONG;
typedef unsigned short UWORD;
typedef unsigned char UBYTE;
#endif

#include <math.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define pi 3.1415926535897932384626433832795
#define root2 1.4142135623730950488016887242097
#define invroot2 (1.0/root2)
#define ln2 0.693147180559945309417232121458177

double dBToGain(double val);
double GainTodB(double val,double mindB=-140.0);

void SetSampleRate(double SampleRate);

extern int g_Channels;
extern double g_SampleRate,g_rSampleRate;

#endif

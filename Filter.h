// Filter.h: interface for the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTER_H__8144E291_C6FA_11D4_AA28_000000000000__INCLUDED_)
#define AFX_FILTER_H__8144E291_C6FA_11D4_AA28_000000000000__INCLUDED_

#include "EnvelopeGen.h"	// Added by ClassView
//#include "WavPack.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_COEFFS 3
#define MAX_ZEDS 4

enum enFiltType {LOPASS = 0, HIPASS, BANDPASS, BANDSTOP};
enum {FILTER_TYPE = 0, CUTOFF, REZ, GAIN, SLOPE};

class CFilter  
{
public:
	UINT ProcessAudio(double *Audio);
	void ChangeVal(int type, int val);
	CFilter();
	virtual ~CFilter();

protected:
	void ResetFilter();
	void CalculateCoeffs();
	
	enFiltType m_nFilterType;
	double m_fGain;
	double m_fRez;
	double m_fCutoff;
	double m_arrACoeffs[MAX_COEFFS];
	double m_arrBCoeffs[MAX_COEFFS];
	double m_arrZeds[MAX_ZEDS];
	byte m_bySlope;
private:
	double omega, sn, cs, alpha;
	byte m_byParamChanged;
};

#endif // !defined(AFX_FILTER_H__8144E291_C6FA_11D4_AA28_000000000000__INCLUDED_)

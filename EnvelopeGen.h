// EnvelopeGen.h: interface for the CEnvelopeGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVELOPEGEN_H__590324B2_C630_11D4_AA27_000000000000__INCLUDED_)
#define AFX_ENVELOPEGEN_H__590324B2_C630_11D4_AA27_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

enum enEGState {NONE = 0, ATTACK, DECAY, SUSTAIN, RELEASE, ATTACKLEV, RELEASELEV};
enum enEGType {AMPEG = 0, PITCHEG, FILTEREG};
/*
	Crude.  Needs to take a trigger and produce an
	output that is fed to something else.
	Don't process any input
*/
class CEnvelopeGen  
{
public:
	void SetType(enEGType type);
	
	CEnvelopeGen();
	virtual ~CEnvelopeGen();
	UINT ProcessAudio(double * Audio);
	void ChangeVal(int type, int newval);
	void NoteOn();
	void NoteOff();

protected:
	void HardReset();

private:
	byte m_byNoteOn;
	enEGState m_enState;
	enEGType m_enType;
	double m_fAtTime, m_fDecTime, m_fSusLev, m_fRelTime;
	double m_fAtLev, m_fRelLev;
	double m_fCount;
};

#endif // !defined(AFX_ENVELOPEGEN_H__590324B2_C630_11D4_AA27_000000000000__INCLUDED_)

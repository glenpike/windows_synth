// Osc.h: interface for the COsc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OSC_H__62B073AE_BCC3_11D4_AA07_000000000000__INCLUDED_)
#define AFX_OSC_H__62B073AE_BCC3_11D4_AA07_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	
#define SIZE_WAVE_TABLE 1800

enum enShape {SINE = 0, SQUARE, SAWTOOTH, TRIANGLE, NOISE} ;
enum enRes {OK, SYNC};  //return values for ProcessAudio
enum {SHAPE = 0, PITCH, FINETUNE, DETUNE, FMSWITCH, FMDEPTH, PULSEWIDTH, OFFSET, RESYNC, SYNCMASTER, SYNCSLAVE,RINGMOD};

/*
	Quite a fiddly implementation of this class.
	Needs to be simplified to produce wave forms.
	Input will be from other osc / controller.
	THINK ANALOGUE SYNTH.  THINK C++ - DIVIDE UP THE
	CLASS SO THAT PROCESSING IS MINIMISED AND CONTROL
	IS SIMPLE!
*/
class COsc  
{
public:
	COsc();
	virtual ~COsc();
	virtual	UINT ProcessAudio(double * Audio);
	virtual void ChangeVal(int type, int val);

protected:
	void SetOffset(int);
	void SetFMDepth(int depth);
	void UnMakeSlave();
	void UnMakeMaster();
	void FineTune(int tune);
	void MakeSlave();
	void MakeMaster();
	void ReSync();
	void Detune(int detune);
	void SetPitch(int note);
	void SetShape(enShape shape);
	void SetPulseWidth(int nWidth);
	void ResetFilterMem();

private:
	byte m_byFM;
	byte m_byRingMod;
	
	double m_fOffset;
	double m_fAmountFM;
	double m_fFineTune;
	double m_fDetune;
	void CalculateFilter();
	void CalculateWaveTable();
	
	double m_fWaveTable[SIZE_WAVE_TABLE+1];
	double m_fcount;
	double m_fPitch;
	double b0,b1,b2,a0,a1,a2;
	double Pn, Qn;

	byte m_byMaster;
	byte m_bySlave;

	enShape m_nShape;
	int m_nPulseWidth;

	int		m_RandomNumber;
	int		m_RandomNumberMultiplier;
	int		m_RandomNumberModulo;
};

#endif // !defined(AFX_OSC_H__62B073AE_BCC3_11D4_AA07_000000000000__INCLUDED_)

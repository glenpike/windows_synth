// EnvelopeGen.cpp: implementation of the CEnvelopeGen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "synth1.h"
#include "EnvelopeGen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnvelopeGen::CEnvelopeGen()
{
	m_enState = NONE;
	m_enType = AMPEG;
	m_fAtTime = m_fDecTime = m_fSusLev = m_fRelTime = 1.0;
	m_fAtLev = m_fRelLev = 0.0;
	m_fCount = 0.0;
	m_byNoteOn = FALSE;
}	


CEnvelopeGen::~CEnvelopeGen()
{

}

UINT CEnvelopeGen::ProcessAudio(double * Audio)
{
	int i = 0;
	switch(m_enState)
	{
		case ATTACK:
			if(m_fCount <= m_fAtTime)
			{
				m_fCount += m_fAtTime/g_SampleRate;
			}
			else
			{
				if(m_fSusLev != 1.0)
				{	
					m_enState = DECAY;
					m_fCount = m_fDecTime;
				}
				else
				{
					m_enState = SUSTAIN;
					m_fCount =  m_fSusLev;
				}
				
			}
			break;
		case DECAY:
			if(m_fCount >= m_fSusLev)
			{
				m_fCount -= m_fDecTime/g_SampleRate;
			}
			else
			{
				m_enState = SUSTAIN;
				m_fCount = m_fSusLev;
			}
			break;
		case SUSTAIN:
			break;
		case RELEASE:
			if(m_fCount >= m_fRelLev)
			{
				m_fCount -= m_fRelTime/g_SampleRate;//fix it!
			}
			else
			{
				m_enState = NONE;
				m_fCount = 0.0;
			}
			break;
		default:
			break;
	}
	if(m_enType != PITCHEG)
		*Audio = m_fCount;
	else
		*Audio = pow(2.0, 1.0-m_fCount);
	return 0;
}

void CEnvelopeGen::ChangeVal(int type, int newval)
{
	double val = (double)(1000 - newval)/1000; //ms
	switch(type)
	{
		case ATTACK:
			m_fAtTime = val;
			break;
		case DECAY:
			m_fDecTime = val;
			break;
		case SUSTAIN:
			m_fSusLev = val;
			break;
		case RELEASE:
			m_fRelTime = val;
			break;
		case ATTACKLEV:
			if(m_enType == PITCHEG)
				m_fAtLev = val;
			else
				m_fAtLev = 0.0;
			break;
		case RELEASELEV:
			if(m_enType == PITCHEG)
				m_fRelLev = val;
			else
				m_fRelLev = 0.0;
			break;	
		default:
			break;
	}
	TRACE("EG val %.2lf %d\n", val, type);
}

void CEnvelopeGen::NoteOn()
{
	
	HardReset();
	m_fCount = 1.0 - m_fAtTime;
	m_byNoteOn = TRUE;
}

void CEnvelopeGen::NoteOff()
{
	if(m_byNoteOn)
	{
		m_enState = RELEASE;
		m_byNoteOn = FALSE;
		m_fCount = m_fSusLev;
	}
}

void CEnvelopeGen::HardReset()
{
	m_fCount =	0.0;
	m_enState = ATTACK;
}

void CEnvelopeGen::SetType(enEGType type)
{
	m_enType = type;
}

// Osc.cpp: implementation of the COsc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "synth1.h"
#include "Osc.h"
#include "math.h"
//#include "wavetab2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COsc::COsc()
{
	//Do not forget to initialise all your floating point variables bozo.	
	m_nShape = SINE;
	m_nPulseWidth = SIZE_WAVE_TABLE; //Pulses the waveform - Aliasing!
	m_byMaster = m_bySlave = m_byRingMod = m_byFM = FALSE;
	m_fDetune = m_fFineTune = m_fAmountFM = 1.0;
	m_fcount = m_fOffset = 0.0;
	m_fPitch = 1.0;
	m_RandomNumber = 1;
	m_RandomNumberMultiplier = 16807;
	m_RandomNumberModulo = 0x7fffffff;

	b0 = b1 = b2 = a0 = a1 = a2 = 0.0;

	CalculateWaveTable();
	CalculateFilter();
}

COsc::~COsc()
{
}

UINT COsc::ProcessAudio(double *Audio)
{
	double weight1, audio, Yn, FM = 1.0;
	int count = (int)m_fcount;
	UINT res = (UINT)OK;
	
	weight1 = m_fcount - count;

	if(m_fcount < m_nPulseWidth)
	{
		if(m_nShape != NOISE)
		{
			audio = m_fWaveTable[count++]*(1 - weight1);
			if(count >= SIZE_WAVE_TABLE)
				count = 0;
			audio += m_fWaveTable[count]*weight1;
		}
		else
		{
			//Stole it Mr Day.
			m_RandomNumber *= m_RandomNumberMultiplier;
			m_RandomNumber &= m_RandomNumberModulo;
			short val = (short) ( m_RandomNumber >> 15);
			//this may need improving - short may differ in size for various systems
			audio = (double)val/65536;
		}
		audio += m_fOffset;
	}
	else 
	{
		audio = 0.0;
	}
	if(m_byFM)
	{
		 FM = pow(2.0, (*Audio)*m_fAmountFM*1.0);
	}
	
	m_fcount += m_fPitch * m_fFineTune * m_fDetune * FM;

	while(m_fcount > SIZE_WAVE_TABLE)
	{
		m_fcount -= SIZE_WAVE_TABLE;
		if(m_byMaster == TRUE)
		{
			res = (UINT)SYNC;
		}
	}
	while(m_fcount < 0)
	{
		m_fcount += SIZE_WAVE_TABLE;
	}
	
	Yn = audio * a0 + Pn;
	Pn = a1 * audio - b1 * Yn + Qn;
	Qn= a2 * audio - b2 * Yn;
	audio = Yn;
	
	*Audio = audio;

	return res;
}

void COsc::ChangeVal(int type, int val)
{
	switch(type)
	{
		case SHAPE:
			SetShape((enShape)val);
			break;
		case PITCH:
			SetPitch(val);
			break;
		case FINETUNE:
			FineTune(val);
			break;
		case DETUNE:
			Detune(val);
			break;
		case FMDEPTH:
			SetFMDepth(val);
			break;
		case PULSEWIDTH:
			SetPulseWidth(val);
			break;
		case OFFSET:
			SetOffset(val);
			break;
		case SYNCMASTER:
			if(val)
				MakeMaster();
			else
				UnMakeMaster();
			break;
		case SYNCSLAVE:
			if(val)
				MakeSlave();
			else
				UnMakeSlave();
			break;
		case RINGMOD:
			if(val)
				m_byRingMod = TRUE;
			else
				m_byRingMod = FALSE;
			break;
		case FMSWITCH:
			if(val)
				m_byFM = TRUE;
			else
				m_byFM = FALSE;
			break;
		case RESYNC:
			ReSync();
			break;
		default:
			break;
	}
}
void COsc::CalculateWaveTable()
{
	int i;
	for(i=0;i<SIZE_WAVE_TABLE;i++)
	{
		m_fWaveTable[i] = 0.0;
	}
	switch(m_nShape)
	{
	case SINE:
		for(i=0;i<SIZE_WAVE_TABLE;i++)
		{
			m_fWaveTable[i] = sin(i*((2*pi)/SIZE_WAVE_TABLE));
		}
		break;
	case SQUARE:
		for(i=0;i<SIZE_WAVE_TABLE/2;i++)
		{
				m_fWaveTable[i] = 0.9;
		}
		for(i=0;i<SIZE_WAVE_TABLE/2;i++)
		{
			m_fWaveTable[i + SIZE_WAVE_TABLE/2] = -m_fWaveTable[i];
		}
		break;
	case SAWTOOTH:
		for(i=0;i<SIZE_WAVE_TABLE;i++)
		{
			m_fWaveTable[i] = (2*((double)i/SIZE_WAVE_TABLE))-1;
		}
		break;
	case TRIANGLE:
		for(i = 0;i<SIZE_WAVE_TABLE;i++)
		{
				if(i<SIZE_WAVE_TABLE/2)
				{
					m_fWaveTable[i] = 2*((double)i/(SIZE_WAVE_TABLE/2))-1;
				}
				else
					m_fWaveTable[i] = (2-(2*((double)(i-SIZE_WAVE_TABLE/2)/(SIZE_WAVE_TABLE/2))))-1;
		}
		break;
	default:
		break;
	}
}

void COsc::CalculateFilter()
{
	//Anti-aliasing - at the moment, the notes do not go higher than 3951Hz fund
	//so this is fairly adequate - future - may need to be more than 12dB/Octave
	//And may need to adjust if doing FM.  Bollox with square wave......

	double omega=2.0*pi*8000/44100; 
	double sn=sin(omega);
	double cs=cos(omega);
	double alpha=1.0;

	a0 =  (1.0 - cs)*.5;
	a1 =   1.0 - cs;
	a2 =  (1.0 - cs)*.5;
	b0 =   1.0 + alpha;
	b1 =  -2.0*cs;
	b2 =   1.0 - alpha;
	
	double rb = 1.0/b0;

	b1 *= rb;
	b2 *= rb;
	a0 *= rb;
	a1 *= rb;
	a2 *= rb;
	ResetFilterMem();

}

void COsc::ResetFilterMem()
{
		Pn = Qn = 0.0;
}

void COsc::SetShape(enum enShape Shape)
{
	if(Shape != m_nShape)
	{
		m_nShape = Shape;
		if(m_nShape != NOISE)
		{
			CalculateWaveTable();
		}
	}
}

void COsc::SetPulseWidth(int nWidth)
{
	if(nWidth != m_nPulseWidth)
	{
		m_nPulseWidth = nWidth;
	}
}

void COsc::SetPitch(int note)
{
	double pitch;
	int offset = -7;
	//G0 = note number 0  = 24.5Hz
	note += offset;
	if(note > 88) //hurts too much if any more!
		note = 88;
	pitch = 44100.0/SIZE_WAVE_TABLE;
	pitch *= (pow(2.0,((double)note/12)));
	m_fPitch = SIZE_WAVE_TABLE / (44100.0/pitch);
	ASSERT(m_fPitch>0);
	//TRACE("Note %d %.2lf\n", note, m_fPitch);
}	

void COsc::Detune(int detune)
{
	//1200 cents is one octave
	//coarse tune will 120 steps to the octave
	m_fDetune = pow(2.0,((double)detune/120));
	//TRACE("Detune %0.2f Pitch %0.2f\n", m_fDetune, m_fPitch);
}

void COsc::FineTune(int tune)
{
	//one cent at a time
	m_fFineTune = pow(2.0,((double)tune/1200));
	//TRACE("FineTune %0.2f Pitch %0.2f\n", m_fFineTune, m_fPitch);
}

void COsc::SetFMDepth(int depth)
{
	m_fAmountFM = (double)depth/100;
	//TRACE("FM Depth %0.2f\n", m_fAmountFM);
}

void COsc::SetOffset(int offset)
{
	//wraps wave form round
	//nice & dirty
	m_fOffset = ((double)offset)/100;
}

void COsc::MakeMaster()
{
	m_bySlave = FALSE;
	m_byMaster = TRUE;
}

void COsc::MakeSlave()
{
	m_bySlave = TRUE;
	m_byMaster = FALSE;
}

void COsc::UnMakeMaster()
{
	m_byMaster = FALSE;
	m_bySlave = FALSE;
}

void COsc::UnMakeSlave()
{
	m_bySlave = FALSE;
	m_byMaster = FALSE;
}

void COsc::ReSync()
{
	if(m_bySlave)
	{
		m_fcount = 0.0;
	}
}


// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "synth1.h"
#include "Filter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilter::CFilter()
{
	m_fGain = m_fCutoff = m_fRez = 1.0;
	m_nFilterType = LOPASS;
	m_bySlope = FALSE;
	ResetFilter();
	m_byParamChanged = TRUE;
}

CFilter::~CFilter()
{

}

void CFilter::ChangeVal(int type, int val)
{
	double exp = (double)val/10000.0 - 3.0;
	
	switch(type)
	{
		case FILTER_TYPE:
			m_nFilterType = (enFiltType)val;
			break;
		case CUTOFF:
			omega=pi*pow(10.0, exp);
			//TRACE("Cutoff %.2lf %.2lf\n", omega, exp);
			break;
		case REZ:
			m_fRez = ((double)val)/10.0;
			//TRACE("Rez %0.1lf\n", m_fRez);
			break;
		case GAIN:
			m_fGain = (double)val/100.0;
			break;
		case SLOPE:
			m_bySlope = val?TRUE:FALSE;
			break;
		default:
			break;
	}
	if(type != GAIN)
		m_byParamChanged = TRUE;
//	TRACE("Filt Parm %d val %d\n", type, val);
}

UINT CFilter::ProcessAudio(double *Audio)
{
	double Yn, Wn, Xn=(*Audio);
	if(m_byParamChanged)
		CalculateCoeffs();

	Wn = m_arrACoeffs[0] * Xn + m_arrZeds[0];
	m_arrZeds[0] = m_arrACoeffs[1] * Xn - m_arrBCoeffs[1] * Wn + m_arrZeds[1];
	m_arrZeds[1] = m_arrACoeffs[2] * Xn - m_arrBCoeffs[2] * Wn;
	if(!m_bySlope)
	{		
		*Audio = Wn * m_fGain;
		return 0;
	}
	else
	{
		Yn = m_arrACoeffs[0] * Wn + m_arrZeds[2];
		m_arrZeds[2] = m_arrACoeffs[1] * Wn - m_arrBCoeffs[1] * Yn + m_arrZeds[3];
		m_arrZeds[3] = m_arrACoeffs[2] * Wn - m_arrBCoeffs[2] * Yn;
		*Audio = Yn * m_fGain;
	}
	return 0;
}

void CFilter::ResetFilter()
{	
	for(int i = 0;i<MAX_COEFFS;i++)
	{
		m_arrACoeffs[i] = 0.0;
		m_arrBCoeffs[i] = 0.0;
	}
}

void CFilter::CalculateCoeffs()
{
/*EQ Theory

    A     = sqrt[ 10^(dBgain/20) ]   = 10^(dBgain/40)

    omega = 2*PI*frequency/sample_rate
    
    sn    = sin(omega)
    cs    = cos(omega)

    alpha = sn/(2*Q)
          = sn*sinh[ ln(2)/2 * bandwidth * omega/sn ]

    beta  = sqrt[ (A^2 + 1)/2 - (A-1)^2 ]


                1.0        1.0 - z^-1
  s  <-  -------------- * ----------
          tan(omega/2)     1.0 + z^-1
*/
	double rb;

	sn=sin(omega);
	cs=cos(omega);
	alpha =sn/(2.0 * m_fRez);

	switch(m_nFilterType)
	{
	case LOPASS:
/*LPF:            H(s) = 1.0 / (s^2 + s/Q + 1)

                a0 =  (1.0 - cs)/2.0
                a1 =   1.0 - cs
                a2 =  (1.0 - cs)/2.0
                b0 =   1.0 + alpha
                b1 =  -2.0*cs
                b2 =   1.0 - alpha*/
		m_arrBCoeffs[0] = 1.0 + alpha;
		m_arrBCoeffs[1] = (-2.0*cs);
		m_arrBCoeffs[2] = (1.0 - alpha);
		m_arrACoeffs[0] = ((1.0 - cs)/2.0);
		m_arrACoeffs[1] = (1.0 - cs);
		m_arrACoeffs[2] = ((1.0 - cs)/2.0);	
	
		break;
	case HIPASS:
/*HPF:          H(s) = s^2 / (s^2 + s/Q + 1)

                b0 =   1.0 + alpha
                b1 =  -2.0*cs
                b2 =   1.0 - alpha
								a0 =  (1.0 + cs)/2.0
                a1 = -(1.0 + cs)
                a2 =  (1.0 + cs)/2.0*/
		m_arrBCoeffs[0] = 1.0 + alpha;
    m_arrBCoeffs[1] =	-2.0 * cs;
		m_arrBCoeffs[2] = 1.0 - alpha;
		m_arrACoeffs[0] = m_arrACoeffs[2] = (1.0 + cs)/2.0; 
    m_arrACoeffs[1] = -(1.0 + cs);
		break;
	case BANDPASS:
/*BPF:          H(s) = (s/Q) / (s^2 + s/Q + 1)

                b0 =   1.0 + alpha
                b1 =  -2.0*cs
                b2 =   1.0 - alpha
								a0 =   alpha
                a1 =   0.0
                a2 =  -alpha*/
		m_arrBCoeffs[0] = 1.0 + alpha;
		m_arrBCoeffs[1] = -2.0 * cs;
		m_arrBCoeffs[2] = 1.0 - alpha;
		m_arrACoeffs[0] = alpha;
		m_arrACoeffs[1] = 0.0;
		m_arrACoeffs[2] = -alpha;
		break;
	case BANDSTOP:
/*notch:        H(s) = (s^2 + 1) / (s^2 + s/Q + 1)

                b0 =   1.0 + alpha
                b1 =  -2.0*cs
                b2 =   1.0 - alpha
								a0 =   1.0
                a1 =  -2/0*cs
                a2 =   1.0*/
		m_arrBCoeffs[0] = 1.0 + alpha;
		m_arrBCoeffs[1] = -2.0 * cs;
		m_arrBCoeffs[2] = 1.0 - alpha;
		m_arrACoeffs[0] = 1.0;
		m_arrACoeffs[1] = -2.0 * cs;
		m_arrACoeffs[2] = 1.0;
		break;
	default:
//		TRACE("Filter.cpp should not be here\n");	
		break;
	}
	
	rb = 1.0/m_arrBCoeffs[0];
	
	m_arrBCoeffs[1] *= rb;
	m_arrBCoeffs[2] *= rb;
	m_arrACoeffs[0] *= rb;
	m_arrACoeffs[1] *= rb;
	m_arrACoeffs[2] *= rb;
	
	m_byParamChanged = FALSE;
}

#ifndef __WAVE_WRITER__
#define __WAVE_WRITER__

#ifndef __AUDIO_MISC__
#include "misc.h"
#endif

class WaveWriter {
public:
	void Write(BYTE* data, DWORD dwLen);
	WaveWriter();
	virtual ~WaveWriter();

	virtual int Open(char *pFilename);
	virtual void Close();
	
	virtual void SetFadeLen(double t=.01);
	virtual void StartFadeDown() {
		SetFadeLen(m_FadeLen);
		m_Inc=-m_Inc;
	}

	int IsOpen() {return (m_File!=NULL);}

	virtual void Write(double *pData);
	
	double Length() {return (double)m_Length/44100/*g_SampleRate*/;}

	int Fading() {return (m_Level!=0.0);}
	int FadingUp() {return (m_Inc>0.0);}
	int FadingDown() {return (m_Inc<0.0);}

protected:
	typedef struct {
		ULONG m_RIFFTag,m_RIFFLength;
		ULONG m_WAVETag,m_fmtTag;
		ULONG m_Format1;
		UWORD m_Format2;
		UWORD m_Channels;
		ULONG m_SampleRate;
		ULONG m_BytesPerSecond;
		UWORD m_BlockAlign;
		UWORD m_BitsPerSample;
		ULONG m_dataTag;
		ULONG m_dataLength;
	} WAVEFormat;
	WAVEFormat m_Format;
	FILE *m_File;
	int m_Length;
	double m_Level,m_Inc,m_FadeLen;
};

#endif

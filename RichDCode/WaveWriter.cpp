#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "WaveWriter.h"

/* end of includes */

#define RIFF_CHUNK_ID 0x52494646ul // 'RIFF'
#define WAVE_CHUNK_ID 0x57415645ul // 'WAVE'
#define fmt_CHUNK_ID 0x666d7420ul // 'fmt '
#define data_CHUNK_ID 0x64617461ul // 'data'

#define PAD_CHUNK_ID 0x50414420ul // 'PAD '
#define FACT_CHUNK_ID 0x66616374ul // 'fact'
#define JUNK_CHUNK_ID 0x4a554e4bul // JUNK'
#define CUE_CHUNK_ID 0x63756520ul // 'cue '
#define DATA_CHUNK_ID 0x4c495354ul // 'DATA'
#define INFO_CHUNK_ID 0x494e464ful //' INFO'

UWORD SwapBytes(UWORD data)
{
	UWORD data1=data;
	char *pData=(char *)&data;
	char *pData1=(char *)&data1;
	
	pData1[0]=pData[1];
	pData1[1]=pData[0];

	return data1;
}

ULONG SwapBytes(ULONG data)
{
	ULONG data1=data;
	char *pData=(char *)&data;
	char *pData1=(char *)&data1;
	
	pData1[0]=pData[3];
	pData1[1]=pData[2];
	pData1[2]=pData[1];
	pData1[3]=pData[0];

	return data1;
}

WaveWriter::WaveWriter()
{
	m_File=NULL;

	memset(&m_Format,0,sizeof(m_Format));

	m_Format.m_RIFFTag=SwapBytes(RIFF_CHUNK_ID);
	m_Format.m_WAVETag=SwapBytes(WAVE_CHUNK_ID);
	m_Format.m_fmtTag=SwapBytes(fmt_CHUNK_ID);
	m_Format.m_dataTag=SwapBytes(data_CHUNK_ID);

	m_Format.m_Format1=0x10;
	m_Format.m_Format2=1;
	m_Format.m_Channels=2;//g_Channels;
	m_Format.m_SampleRate=(ULONG)44100;//g_SampleRate;
	m_Format.m_BlockAlign=2*m_Format.m_Channels;
	m_Format.m_BytesPerSecond=m_Format.m_SampleRate*m_Format.m_BlockAlign;
	m_Format.m_BitsPerSample=16;

	m_Length=0;

	m_Level=m_Inc=m_FadeLen=0.0;

	SetFadeLen(.01);
}

WaveWriter::~WaveWriter()
{
	Close();
}

void WaveWriter::SetFadeLen(double t)
{
	int FadeSamples=(int)(t*44100/*g_SampleRate*/+.5);

	m_FadeLen=t;
	if (FadeSamples>0)
		m_Inc=1.0/(double)FadeSamples;
	else
		m_Inc=1.0;
}

int WaveWriter::Open(char *pFilename)
{
	int res=0;

	if (m_File==NULL) {
		if ((m_File=fopen(pFilename,"wb+"))!=NULL) {
			m_Length=0;

			m_Format.m_dataLength=m_Length*m_Format.m_BlockAlign;
			m_Format.m_RIFFLength=m_Format.m_dataLength+sizeof(m_Format)-8;
			fwrite(&m_Format,sizeof(m_Format),1,m_File);

			m_Level=0.0;
			SetFadeLen(m_FadeLen);
			res=1;
		}
	}

	return res;
}

void WaveWriter::Close()
{
	if (m_File!=NULL) {
		fflush(m_File);
		rewind(m_File);

		m_Format.m_dataLength=m_Length*m_Format.m_BlockAlign;
		m_Format.m_RIFFLength=m_Format.m_dataLength+sizeof(m_Format)-8;
		fwrite(&m_Format,sizeof(m_Format),1,m_File);
		fclose(m_File);
		m_File=NULL;
	}

	m_Length=0;
	m_Level=m_Inc=0.0;
}


void WaveWriter::Write(double *pData)
{
	if (m_File!=NULL) {
		short data[2];
		int i;

		for (i=0;i<2/*g_Channels*/;i++) {
			double val=pData[i]*32768.0*m_Level;

			if (val<-32768.0)
				data[i]=-32768;
			else if (val>32767.0)
				data[i]=32767;
			else
				data[i]=(short)val;
		}

		fwrite(data,sizeof(short),2/*g_Channels*/,m_File);
		m_Length++;

		m_Level+=m_Inc;
		if (m_Level>1.0) {
			m_Level=1.0;
			m_Inc=0.0;
		} else if (m_Level<0.0) {
			m_Level=0.0;
			m_Inc=0.0;
		}
	}
}

void WaveWriter::Write(BYTE *data, DWORD dwLen)
{
	fwrite(data,dwLen*4,2,m_File);
	m_Length += dwLen ;
}


#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "AudioHandler.h"

/* end of includes */

AudioHandler::AudioHandler(int Device)
{
  memset(&m_Format,0,sizeof(m_Format));
  memset(&m_WaveHeader,0,sizeof(m_WaveHeader));
  memset(m_pBuffer,0,sizeof(m_pBuffer));
  m_pCurrentBuffer=NULL;

	m_Time.wType=TIME_SAMPLES;

  m_Device=Device;

  m_Level=1.0;

  m_Buffer=m_Pos=0;

  m_Format.wFormatTag=WAVE_FORMAT_PCM;
  m_Format.nChannels=g_Channels;
  m_Format.nSamplesPerSec=(int)g_SampleRate;
  m_Format.nBlockAlign=2*m_Format.nChannels;
  m_Format.nAvgBytesPerSec=m_Format.nBlockAlign*m_Format.nSamplesPerSec;
  m_Format.wBitsPerSample=16;
  m_Format.cbSize=0;

  int i;
  for (i=0;i<2;i++) {
    m_WaveHeader[i].dwFlags=0;
    if ((m_pBuffer[i]=new short[g_Channels*BufferSize])!=NULL) {
      m_WaveHeader[i].lpData=(char *)m_pBuffer[i];
      memset(m_pBuffer[i],0,g_Channels*BufferSize*sizeof(short));
			m_WaveHeader[i].dwBufferLength=g_Channels*BufferSize*sizeof(short);
		} else {
			TRACE("Failed to allocate Audio buffer %d\n",i);
		}
  }
  if ((m_pCurrentBuffer=new short[g_Channels*BufferSize])!=NULL)
    memset(m_pCurrentBuffer,0,g_Channels*BufferSize*sizeof(short));
	else {
		TRACE("Failed to allocate current buffer\n");
	}
}

AudioHandler::~AudioHandler()
{
  Stop();

  if (m_pCurrentBuffer!=NULL) {
    delete[] m_pCurrentBuffer;
    m_pCurrentBuffer=NULL;
  }

  int i;
  for (i=0;i<2;i++) {
    if (m_pBuffer[i]!=NULL) {
      delete[] m_pBuffer[i];
      m_pBuffer[i]=NULL;
    }
  }
}

void AudioHandler::Reset()
{
	SoundHandler::Reset();

  m_Buffer=0;
  m_Pos=0;
}

int AudioHandler::Stop()
{
  m_Buffer=0;
  m_Pos=0;

	return SoundHandler::Stop();
}

/* -------------------------------------------- */

AudioInputHandler::AudioInputHandler(int Device) : AudioHandler(Device)
{
  m_hHandle=(HWAVEIN)INVALID_HANDLE_VALUE;
  m_Level=1.0/32767.0;
}

void AudioInputHandler::Reset()
{
	AudioHandler::Reset();
}

int AudioInputHandler::Start()
{
  DWORD Result;
	int res=AudioHandler::Start();

  if ((Result=waveInOpen(&m_hHandle,m_Device,&m_Format,0,0,CALLBACK_NULL))==MMSYSERR_NOERROR) {
    m_Buffer=0;
    res=1;
    
    if ((Result=waveInPrepareHeader(m_hHandle,m_WaveHeader,sizeof(WAVEHDR)))==MMSYSERR_NOERROR) {
      if ((Result=waveInAddBuffer(m_hHandle,m_WaveHeader,sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
				TRACE("Failed to Add input buffer: %lu\n",Result);
				res=0;
      }
    } else {
      TRACE("Failed to Prepare input buffer: %lu\n",Result);
      res=0;
    }
    
    if ((Result=waveInPrepareHeader(m_hHandle,&m_WaveHeader[1],sizeof(WAVEHDR)))==MMSYSERR_NOERROR) {
      if ((Result=waveInAddBuffer(m_hHandle,&m_WaveHeader[1],sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
				TRACE("Failed to Add input buffer: %lu\n",Result);
				res=0;
      }
    } else {
      TRACE("Failed to Prepare input buffer: %lu\n",Result);
      res=0;
    }

    if (res) {
      if ((Result=waveInStart(m_hHandle))==MMSYSERR_NOERROR) {
				TRACE("Opened and Started input device %d okay\n",m_Device);
      } else {
				TRACE("Failed to Start input device %d: %lu\n",m_Device,Result);
				res=0;
      }
    }
  } else
    TRACE("Failed to Open input device %d: %lu\n",m_Device,Result);

  return res;
}

int AudioInputHandler::Stop()
{
  DWORD Result;
  int res=0;

  if (m_hHandle!=INVALID_HANDLE_VALUE) {
    res=1;

		waveInReset(m_hHandle);

    int i;
    for (i=0;i<2;i++) {
      if ((m_WaveHeader[m_Buffer].dwFlags&WHDR_PREPARED)!=0) {
        while ((m_WaveHeader[m_Buffer].dwFlags&WHDR_DONE)==0)
          Sleep(10);
        if ((Result=waveInUnprepareHeader(m_hHandle,m_WaveHeader+m_Buffer,sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
          TRACE("Failed to Unprepare input header %d at stop: %lu\n",m_Buffer,Result);
          res=0;
        }
      }
      m_WaveHeader[m_Buffer].dwFlags=0;
      m_Buffer^=1;
    }
    Sleep(20);
    if ((Result=waveInStop(m_hHandle))==MMSYSERR_NOERROR) {
      Sleep(20);
      if ((Result=waveInClose(m_hHandle))==MMSYSERR_NOERROR) {
        TRACE("Closed input device %d okay\n",m_Device);
        m_hHandle=(HWAVEIN)INVALID_HANDLE_VALUE;
      } else {
        TRACE("Failed to Close input device %d: %lu\n",m_Device,Result);
				res=0;
      }
    } else {
      TRACE("Failed to Stop input device %d: %lu\n",m_Device,Result);
      res=0;
    }
  }

	if (!AudioHandler::Stop())
	  res=0;

  return res;
}

double AudioInputHandler::Read()
{
  double audio=0.0;

  if (m_pCurrentBuffer!=NULL) {
    if (m_Pos==0)
      ReadBuffer();

    audio=((double)m_pCurrentBuffer[m_Pos++])*m_Level;
    if (m_Pos==(g_Channels*BufferSize))
      m_Pos=0;
  }

  return audio;
}

void AudioInputHandler::Read(double *pAudio,double Scale)
{
	WORD i;

	for (i=0;i<m_Format.nChannels;i++)
		pAudio[i]=Read()*Scale;
}

int AudioInputHandler::SampleCount()
{
	DWORD Result;
	int nSamples=0;

	if (m_hHandle!=INVALID_HANDLE_VALUE) {
		if ((Result=waveInGetPosition(m_hHandle,&m_Time,sizeof(m_Time)))==MMSYSERR_NOERROR) {
			nSamples=m_Time.u.sample;
		} else
			TRACE("Failed to GetPosition, error %lu\n",Result);
	}

	return nSamples;
}

void AudioInputHandler::ReadBuffer()
{
  DWORD Result;

  if (m_hHandle!=INVALID_HANDLE_VALUE) {
		if ((m_WaveHeader[m_Buffer].dwFlags&WHDR_PREPARED)!=0) {
      while ((m_WaveHeader[m_Buffer].dwFlags&WHDR_DONE)==0)
				Sleep(1);

      if ((Result=waveInUnprepareHeader(m_hHandle,m_WaveHeader+m_Buffer,sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
				TRACE("Failed to Unprepare input header %d: %lu\n",m_Buffer,Result);
      }
      memcpy(m_pCurrentBuffer,m_pBuffer[m_Buffer],g_Channels*BufferSize*sizeof(short));
    }

    if ((Result=waveInPrepareHeader(m_hHandle,m_WaveHeader+m_Buffer,sizeof(WAVEHDR)))==MMSYSERR_NOERROR) {
      if ((Result=waveInAddBuffer(m_hHandle,m_WaveHeader+m_Buffer,sizeof(WAVEHDR)))!=MMSYSERR_NOERROR)
				TRACE("Failed to Add input buffer %d: %lu\n",m_Buffer,Result);
    } else
			TRACE("Failed to Prepare input header %d: %lu\n",m_Buffer,Result);
    
    m_Buffer^=1;
  }
}

/* -------------------------------------------- */

AudioOutputHandler::AudioOutputHandler(int Device) : AudioHandler(Device)
{
  m_hHandle=(HWAVEOUT)INVALID_HANDLE_VALUE;
  m_Level=32767.0;
}

void AudioOutputHandler::Reset()
{
	AudioHandler::Reset();

	m_Buffers=0;
}

int AudioOutputHandler::Start()
{
  DWORD Result;
  int res=AudioHandler::Start();

  if ((Result=waveOutOpen(&m_hHandle,m_Device,&m_Format,0,0,CALLBACK_NULL))==MMSYSERR_NOERROR) {
		if ((Result=waveOutPause(m_hHandle))==MMSYSERR_NOERROR) {
			res=1;

			m_Buffer=0;
			TRACE("Opened output device %d okay\n",m_Device);
		} else
	    TRACE("Failed to Pause output device %d: %lu\n",m_Device,Result);
  } else
    TRACE("Failed to Open output device %d: %lu\n",m_Device,Result);

  return res;
}

int AudioOutputHandler::Stop()
{
  DWORD Result;
  int res=0;

  if (m_hHandle!=INVALID_HANDLE_VALUE) {
    int i;
    
		waveOutReset(m_hHandle);

    res=1;
    for (i=0;i<2;i++) {
      if ((m_WaveHeader[m_Buffer].dwFlags&WHDR_PREPARED)!=0) {
				while ((m_WaveHeader[m_Buffer].dwFlags&WHDR_DONE)==0)
          Sleep(10);
				if ((Result=waveOutUnprepareHeader(m_hHandle,m_WaveHeader+m_Buffer,sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
          TRACE("Failed to Unprepare output header %d at stop: %lu\n",m_Buffer,Result);
					res=0;
				}
      }
      m_WaveHeader[m_Buffer].dwFlags=0;
      m_Buffer^=1;
    }

    Sleep(20);
    if ((Result=waveOutClose(m_hHandle))==MMSYSERR_NOERROR) {
      TRACE("Closed output device %d okay\n",m_Device);
      m_hHandle=(HWAVEOUT)INVALID_HANDLE_VALUE;
    } else {
      TRACE("Failed to Close output device %d: %lu\n",m_Device,Result);
      res=0;
    }
  }

  if (!AudioHandler::Stop())
		res=0;

  return res;
}

void AudioOutputHandler::Write(double audio)
{
  if (m_pCurrentBuffer!=NULL) {
		audio*=m_Level;
    if (audio<-32768.0)
      audio=-32768.0;
    else if (audio>32767.0)
      audio=32767.0;

    m_pCurrentBuffer[m_Pos++]=(short)audio;
    if (m_Pos==(g_Channels*BufferSize))
      WriteBuffer();
  }
}

void AudioOutputHandler::Write(double *pAudio,double Scale)
{
	WORD i;

	for (i=0;i<m_Format.nChannels;i++)
		Write(pAudio[i]*Scale);
}

int AudioOutputHandler::SampleCount()
{
	DWORD Result;
	int nSamples=0;

	if (m_hHandle!=INVALID_HANDLE_VALUE) {
		if ((Result=waveOutGetPosition(m_hHandle,&m_Time,sizeof(m_Time)))==MMSYSERR_NOERROR) {
			nSamples=m_Time.u.sample;
		} else
			TRACE("Failed to GetPosition, error %lu\n",Result);
	}

	return nSamples;
}

void AudioOutputHandler::WriteBuffer()
{
  DWORD Result;

  if (m_hHandle!=INVALID_HANDLE_VALUE) {
    if ((m_WaveHeader[m_Buffer].dwFlags&WHDR_PREPARED)!=0) {
      while ((m_WaveHeader[m_Buffer].dwFlags&WHDR_DONE)==0)
				Sleep(1);
      
      if ((Result=waveOutUnprepareHeader(m_hHandle,m_WaveHeader+m_Buffer,sizeof(WAVEHDR)))!=MMSYSERR_NOERROR)
				TRACE("Failed to Unprepare output header %d: %lu\n",m_Buffer,Result);
    }
    memcpy(m_pBuffer[m_Buffer],m_pCurrentBuffer,g_Channels*BufferSize*sizeof(short));

    if ((Result=waveOutPrepareHeader(m_hHandle,m_WaveHeader+m_Buffer,sizeof(WAVEHDR)))==MMSYSERR_NOERROR) {
      if ((Result=waveOutWrite(m_hHandle,m_WaveHeader+m_Buffer,sizeof(WAVEHDR)))!=MMSYSERR_NOERROR)
				TRACE("Failed to Write output header %d: %lu\n",m_Buffer,Result);
    } else
      TRACE("Failed to Prepare output header %d: %lu\n",m_Buffer,Result);

    m_Buffer^=1;
		m_Buffers++;
		if (m_Buffers==2) {
			if ((Result=waveOutRestart(m_hHandle))!=MMSYSERR_NOERROR) {
	      TRACE("Failed to Restart output device %d: %lu\n",m_Device,Result);
			}
		}
  }

  m_Pos=0;
}

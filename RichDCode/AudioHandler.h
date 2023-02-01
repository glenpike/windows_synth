
#ifndef __AUDIO_HANDLER__
#define __AUDIO_HANDLER__

#ifndef __SOUND_HANDLER__
#include "SoundHandler.h"
#endif

class AudioHandler : public SoundHandler {
public:
  AudioHandler(int Device=0);
  virtual ~AudioHandler();

	virtual void Reset();

  virtual int Stop();

	virtual int StartOfBuffer() {return (m_Pos==0);}

	enum {BufferSize=8192*2};
	//enum {BufferSize=8192};
	short *CurrentBuffer() {return m_pCurrentBuffer;}

 	virtual int SampleCount() {return 0;}

protected:
  WAVEFORMATEX m_Format;
  WAVEHDR m_WaveHeader[2];
	MMTIME m_Time;

  short *m_pBuffer[2],*m_pCurrentBuffer;
  double m_Level;
  int m_Device,m_Buffer,m_Pos;
};

class AudioInputHandler : public AudioHandler {
public:
	AudioInputHandler(int Device=0);

	virtual void Reset();

	virtual int Start();
  virtual int Stop();

  virtual double Read();
  virtual void Read(double *pAudio,double Scale=1.0);

 	virtual int SampleCount();

protected:
  virtual void ReadBuffer();

protected:
  HWAVEIN m_hHandle;
};

class AudioOutputHandler : public AudioHandler  {
public:
  AudioOutputHandler(int Device=0);

	virtual void Reset();

	virtual int Start();
  virtual int Stop();

  virtual void Write(double audio);
	virtual void Write(double *pAudio,double Scale=1.0);

 	virtual int SampleCount();

protected:
  virtual void WriteBuffer();

protected:
  HWAVEOUT m_hHandle;
	int m_Buffers;
};

#endif

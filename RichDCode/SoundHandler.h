
#ifndef __SOUND_HANDLER__
#define __SOUND_HANDLER__

#include "misc.h"
#include "afxwin.h"
#include "mmsystem.h"

class SoundHandler {
public:
	SoundHandler() {m_PositionError=0;}
	virtual ~SoundHandler() {}

	virtual void Reset() {m_PositionError=0;}

	virtual int Start() {return 1;}
  virtual int Stop() {return 1;}

  virtual double Read() {return 0.0;}
  virtual void Read(double *pAudio,double Scale=1.0) {}
  virtual void Write(double audio) {}
	virtual void Write(double *pAudio,double Scale=1.0) {}

	virtual int SampleCount() {return 0;}
	int CorrectedSampleCount() {return SampleCount()+m_PositionError;}

protected:
	int m_PositionError;
};

#endif

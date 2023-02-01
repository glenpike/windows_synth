// synth1.h : main header file for the SYNTH1 application
//

#if !defined(AFX_SYNTH1_H__62B073A4_BCC3_11D4_AA07_000000000000__INCLUDED_)
#define AFX_SYNTH1_H__62B073A4_BCC3_11D4_AA07_000000000000__INCLUDED_

#include "KeyboardInput.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "misc.h"
#include "AudioHandler.h"
#include "WaveWriter.h"
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSynth1App:
// See synth1.cpp for the implementation of this class
//

class CSynth1App : public CWinApp
{
public:
	CSynth1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSynth1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSynth1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNTH1_H__62B073A4_BCC3_11D4_AA07_000000000000__INCLUDED_)

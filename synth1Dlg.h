// synth1Dlg.h : header file
//

#if !defined(AFX_SYNTH1DLG_H__62B073A6_BCC3_11D4_AA07_000000000000__INCLUDED_)
#define AFX_SYNTH1DLG_H__62B073A6_BCC3_11D4_AA07_000000000000__INCLUDED_

#include "Osc.h"	// Added by ClassView
#include "EnvelopeGen.h"	// Added by ClassView
#include "Filter.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSynth1Dlg dialog

class CSynth1Dlg : public CDialog
{
// Construction
protected:
	CKeyboardInput m_Keys;
	COsc m_Osc, m_OscSlave;	
	CFilter m_Filter1, m_Filter2;
	CEnvelopeGen m_EnvGen;
	int m_lastnote, m_nOctave;
	byte m_byStop, m_byDone, m_byRunning, m_byPause;

	UINT ProcessAudio();

public:
	friend UINT ProcessThread(LPVOID p);
	
	CSynth1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSynth1Dlg)
	enum { IDD = IDD_SYNTH1_DIALOG };
	CSliderCtrl	m_ctlFiltType;
	CSliderCtrl	m_ctlRez;
	CSliderCtrl	m_ctlCutoff;
	CSliderCtrl	m_ctlRelease;
	CSliderCtrl	m_ctlSustain;
	CSliderCtrl	m_ctlDecay;
	CSliderCtrl	m_ctlAttack;
	CSliderCtrl	m_ctlOffset2;
	CSliderCtrl	m_ctlOffset1;
	CSliderCtrl	m_ctlFM;
	CSliderCtrl	m_ctlFine;
	CSliderCtrl	m_ctlCoarse;
	CSliderCtrl	m_ctlPulseWdth;
	CSliderCtrl	m_ctlShape2;
	CSliderCtrl	m_ctlPulseWdth2;
	CSliderCtrl	m_ctlShape;
	BOOL	m_bSync;
	BOOL	m_bRing;
	BOOL	m_bFM;
	BOOL	m_bSlope;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSynth1Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSynth1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	virtual void OnOK();
	afx_msg void OnShape(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnPulsewidth2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShape2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPulsewidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFine(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDetune(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSync();
	afx_msg void OnRing();
	afx_msg void OnFmdepth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFm();
	afx_msg void OnOffset2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRelease(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSustain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAttack(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDecay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRez(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCutoff(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSlope();
	afx_msg void OnFilttype(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNTH1DLG_H__62B073A6_BCC3_11D4_AA07_000000000000__INCLUDED_)

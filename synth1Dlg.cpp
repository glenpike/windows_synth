// synth1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "synth1.h"
#include "synth1Dlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSynth1Dlg dialog

CSynth1Dlg::CSynth1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSynth1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSynth1Dlg)
	m_bSync = FALSE;
	m_bRing = FALSE;
	m_bFM = FALSE;
	m_bSlope = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSynth1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSynth1Dlg)
	DDX_Control(pDX, IDC_FILTTYPE, m_ctlFiltType);
	DDX_Control(pDX, IDC_REZ, m_ctlRez);
	DDX_Control(pDX, IDC_CUTOFF, m_ctlCutoff);
	DDX_Control(pDX, IDC_RELEASE, m_ctlRelease);
	DDX_Control(pDX, IDC_SUSTAIN, m_ctlSustain);
	DDX_Control(pDX, IDC_DECAY, m_ctlDecay);
	DDX_Control(pDX, IDC_ATTACK, m_ctlAttack);
	DDX_Control(pDX, IDC_OFFSET2, m_ctlOffset2);
	DDX_Control(pDX, IDC_OFFSET, m_ctlOffset1);
	DDX_Control(pDX, IDC_FMDEPTH, m_ctlFM);
	DDX_Control(pDX, IDC_DETUNE2, m_ctlFine);
	DDX_Control(pDX, IDC_DETUNE, m_ctlCoarse);
	DDX_Control(pDX, IDC_PULSEWIDTH, m_ctlPulseWdth);
	DDX_Control(pDX, IDC_SHAPE2, m_ctlShape2);
	DDX_Control(pDX, IDC_PULSEWIDTH2, m_ctlPulseWdth2);
	DDX_Control(pDX, IDC_SHAPE, m_ctlShape);
	DDX_Check(pDX, IDC_SYNC, m_bSync);
	DDX_Check(pDX, IDC_RING, m_bRing);
	DDX_Check(pDX, IDC_FM, m_bFM);
	DDX_Check(pDX, IDC_SLOPE, m_bSlope);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSynth1Dlg, CDialog)
	//{{AFX_MSG_MAP(CSynth1Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SHAPE, OnShape)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_PULSEWIDTH2, OnPulsewidth2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SHAPE2, OnShape2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_PULSEWIDTH, OnPulsewidth)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_DETUNE2, OnFine)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_DETUNE, OnDetune)
	ON_BN_CLICKED(IDC_SYNC, OnSync)
	ON_BN_CLICKED(IDC_RING, OnRing)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_FMDEPTH, OnFmdepth)
	ON_BN_CLICKED(IDC_FM, OnFm)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_OFFSET2, OnOffset2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_OFFSET, OnOffset)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_RELEASE, OnRelease)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SUSTAIN, OnSustain)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ATTACK, OnAttack)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_DECAY, OnDecay)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_REZ, OnRez)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_CUTOFF, OnCutoff)
	ON_BN_CLICKED(IDC_SLOPE, OnSlope)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_FILTTYPE, OnFilttype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSynth1Dlg message handlers

BOOL CSynth1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_ctlPulseWdth.SetRange(1,SIZE_WAVE_TABLE,FALSE);
	m_ctlPulseWdth.SetPos(SIZE_WAVE_TABLE);
	m_ctlPulseWdth.SetTicFreq(200);
	m_ctlShape.SetRange(0,4,FALSE);
	m_ctlShape.SetPos(0);
	m_ctlShape.SetTicFreq(1);
	
	m_ctlPulseWdth2.SetRange(1,SIZE_WAVE_TABLE,FALSE);
	m_ctlPulseWdth2.SetPos(SIZE_WAVE_TABLE);
	m_ctlPulseWdth2.SetTicFreq(200);
	m_ctlShape2.SetRange(0,4,FALSE);
	m_ctlShape2.SetPos(0);
	m_ctlShape2.SetTicFreq(1);

	m_ctlFine.SetRange(-120,120,FALSE);
	m_ctlFine.SetPos(0);
	m_ctlFine.SetTicFreq(20);

	m_ctlCoarse.SetRange(-240,240,FALSE);
	m_ctlCoarse.SetPos(0);
	m_ctlCoarse.SetTicFreq(40);
	
	m_ctlFM.SetRange(0,100, FALSE);
	m_ctlFM.SetPos(100);
	m_ctlFM.SetTicFreq(10);

	m_ctlOffset1.SetRange(-100,100,FALSE);
	m_ctlOffset1.SetPos(0);
	m_ctlOffset1.SetTicFreq(20);

	m_ctlOffset2.SetRange(-100,100,FALSE);
	m_ctlOffset2.SetPos(0);
	m_ctlOffset2.SetTicFreq(20);

	m_ctlAttack.SetRange(1, 1000, FALSE);
	m_ctlAttack.SetPos(0);
	m_ctlAttack.SetTicFreq(100);

	m_ctlDecay.SetRange(1, 1000, FALSE);
	m_ctlDecay.SetPos(0);
	m_ctlDecay.SetTicFreq(100);

	m_ctlSustain.SetRange(1, 1000, FALSE);
	m_ctlSustain.SetPos(1000);
	m_ctlSustain.SetTicFreq(100);

	m_ctlRelease.SetRange(1, 1000, FALSE);
	m_ctlRelease.SetPos(0);
	m_ctlRelease.SetTicFreq(100);

	m_ctlCutoff.SetRange(0,29000,FALSE);
	m_ctlCutoff.SetPos(20000);
	m_ctlCutoff.SetTicFreq(2000);

	m_ctlRez.SetRange(10,100,FALSE);
	m_ctlRez.SetPos(0);
	m_ctlRez.SetTicFreq(10);
	
	m_ctlFiltType.SetRange(0,3, FALSE);
	m_ctlFiltType.SetPos(0);
	m_ctlFiltType.SetTicFreq(1);

	m_byDone = m_byRunning = m_byStop = m_byPause = FALSE;
	g_Channels=2;
  g_SampleRate=44100;
	m_Osc.ChangeVal(SHAPE, SINE);
	m_OscSlave.ChangeVal(SHAPE, SINE);
	m_EnvGen.SetType((enEGType)AMPEG);
	m_lastnote = -1;
	m_nOctave = 3;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSynth1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSynth1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSynth1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSynth1Dlg::OnPlay() 
{
	if (!m_byRunning) 
	{
		m_byRunning=m_byStop=m_byDone=FALSE;
		AfxBeginThread(ProcessThread,(LPVOID)this);
		while (!m_byRunning)
		  Sleep(10);
	}
	TRACE("Started Thread\n");
}

void CSynth1Dlg::OnStop() 
{
	if(m_byRunning) 
	{
		m_byStop = TRUE;
		while(!m_byDone)
			Sleep(10);
		m_byRunning = m_byStop = m_byDone = FALSE;
	}
	TRACE("Stopped Thread\n");
}

void CSynth1Dlg::OnOK() 
{
	OnStop();
	CDialog::OnOK();
}

UINT ProcessThread(LPVOID p)
{
  CSynth1Dlg *proc=(CSynth1Dlg *)p;
	
  return proc->ProcessAudio();
}

UINT CSynth1Dlg::ProcessAudio()
{
	double AudioOut, AudioOut2, AmpEnv = 1.0;
	g_Channels = 1;//testing
	AudioOutputHandler m_AudioHandler;
#ifdef _DEBUG	
	WaveWriter file;
	file.Open("c:\\temp\\synth1.wav");
#endif
	m_byRunning = TRUE;
	m_AudioHandler.Reset();
	m_AudioHandler.Start();
	
	while(!m_byStop)
	{
		if(m_Osc.ProcessAudio(&AudioOut)==SYNC)
			m_OscSlave.ChangeVal(RESYNC, TRUE);
		if(m_bFM)
		{
			AudioOut2 = AudioOut;
		}
		m_OscSlave.ProcessAudio(&AudioOut2); 
		if(m_bRing)
		{
			AudioOut *= AudioOut2;
			AudioOut2 = AudioOut;
		}
		
		m_Filter1.ProcessAudio(&AudioOut);
		m_Filter2.ProcessAudio(&AudioOut2);
		m_EnvGen.ProcessAudio(&AmpEnv);
		
		AudioOut *= AmpEnv;
		AudioOut2 *= AmpEnv;
		AudioOut += AudioOut2;	
		AudioOut /= 2.0;

		for (int j=0;j<g_Channels;j++) {
			m_AudioHandler.Write(&AudioOut);
#ifdef _DEBUG
			file.Write(&AudioOut);
#endif
    }
	}
 	m_AudioHandler.Stop();
	m_byDone = TRUE;
	return 0;
}

void CSynth1Dlg::OnCancel() 
{
	OnStop();
	CDialog::OnCancel();
}

void CSynth1Dlg::OnShape(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Osc.ChangeVal(SHAPE, m_ctlShape.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnShape2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_OscSlave.ChangeVal(SHAPE, m_ctlShape2.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnPulsewidth2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_OscSlave.ChangeVal(PULSEWIDTH, m_ctlPulseWdth2.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnPulsewidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Osc.ChangeVal(PULSEWIDTH,m_ctlPulseWdth.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnFine(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_OscSlave.ChangeVal(FINETUNE,m_ctlFine.GetPos());	
	*pResult = 0;
}

void CSynth1Dlg::OnDetune(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_OscSlave.ChangeVal(DETUNE, m_ctlCoarse.GetPos());	
	*pResult = 0;
}

void CSynth1Dlg::OnSync() 
{
	int check = IsDlgButtonChecked(IDC_SYNC);
	m_bSync = (check != BST_INDETERMINATE)?check:m_bSync;
	m_Osc.ChangeVal(SYNCMASTER, (int)m_bSync);
	m_OscSlave.ChangeVal(SYNCSLAVE,(int)m_bSync);
	TRACE("%d Sync\n", m_bSync);
}

void CSynth1Dlg::OnRing() 
{
	UINT check = IsDlgButtonChecked(IDC_RING);
	m_bRing = (check != BST_INDETERMINATE)?check:m_bRing;
	TRACE("%d Ring\n", m_bRing);
}

void CSynth1Dlg::OnFmdepth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_OscSlave.ChangeVal(FMDEPTH,m_ctlFM.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnFm() 
{
	UINT check = IsDlgButtonChecked(IDC_FM);
	m_bFM = (check != BST_INDETERMINATE)?check:m_bFM;
	m_OscSlave.ChangeVal(FMSWITCH, (int)m_bFM);
	TRACE("%d FM\n", m_bFM);
}

void CSynth1Dlg::OnOffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Osc.ChangeVal(OFFSET,m_ctlOffset1.GetPos());		
	*pResult = 0;
}

void CSynth1Dlg::OnOffset2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_OscSlave.ChangeVal(OFFSET,m_ctlOffset2.GetPos());	
	*pResult = 0;
}

void CSynth1Dlg::OnAttack(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_EnvGen.ChangeVal(ATTACK, m_ctlAttack.GetPos());	
	*pResult = 0;
}

void CSynth1Dlg::OnDecay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_EnvGen.ChangeVal(DECAY, m_ctlDecay.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnSustain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_EnvGen.ChangeVal(SUSTAIN, m_ctlSustain.GetPos());	
	*pResult = 0;
}

void CSynth1Dlg::OnRelease(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_EnvGen.ChangeVal(RELEASE, m_ctlRelease.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnRez(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Filter1.ChangeVal(REZ, m_ctlRez.GetPos());
	m_Filter2.ChangeVal(REZ, m_ctlRez.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnCutoff(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Filter1.ChangeVal(CUTOFF, m_ctlCutoff.GetPos());
	m_Filter2.ChangeVal(CUTOFF, m_ctlCutoff.GetPos());
	*pResult = 0;
}

void CSynth1Dlg::OnSlope() 
{
	UINT check = IsDlgButtonChecked(IDC_SLOPE);
	m_bSlope = (check != BST_INDETERMINATE)?check:m_bSlope;
	m_Filter1.ChangeVal(SLOPE, m_bSlope);
	m_Filter2.ChangeVal(SLOPE, m_bSlope);
	TRACE("%d Slope\n", m_bSlope);	
}

/*
	The bit where the keypresses are deciphered and
	maybe used to change the pitch and other parameters
	of the synth
*/
BOOL CSynth1Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_PRIOR)
		{
			m_nOctave++;
			if(m_nOctave > 7)
				m_nOctave = 7;
		}
		if(pMsg->wParam == VK_NEXT)	
		{
			m_nOctave--;
			if(m_nOctave < 0)
				m_nOctave = 0;
		}
		int note = m_Keys.KeyToNoteNum((int)pMsg->wParam);
		
		if( note != -1 && note != m_lastnote)
		{
			m_lastnote = note;
			m_EnvGen.NoteOn();
			m_Osc.ChangeVal(PITCH,(note + (m_nOctave * 12)));
			m_OscSlave.ChangeVal(PITCH,(note + (m_nOctave * 12)));
			//TRACE("KeyDown %c note %d Oct %d \n", pMsg->wParam, m_lastnote, m_nOctave);
		}
		return true;
	}
	if(pMsg->message == WM_KEYUP)
	{
		int note = m_Keys.KeyToNoteNum((int)pMsg->wParam);
		if(note != -1 && note == m_lastnote)
		{
			m_EnvGen.NoteOff();
			m_lastnote = -1;
		}
		return true;
	}
	else
		return CDialog::PreTranslateMessage(pMsg);
}


void CSynth1Dlg::OnFilttype(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Filter1.ChangeVal(FILTER_TYPE, m_ctlFiltType.GetPos());
	m_Filter2.ChangeVal(FILTER_TYPE, m_ctlFiltType.GetPos());
	*pResult = 0;
}

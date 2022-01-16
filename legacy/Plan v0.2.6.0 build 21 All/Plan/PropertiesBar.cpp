// PropertiesBar.cpp : implementation file
//

#include "stdafx.h"
#include "Plan.h"
#include "PropertiesBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar dialog


CPropertiesBar::CPropertiesBar()
{
	m_Schedule.Set();
	m_iCurSel = CB_ERR;
	//{{AFX_DATA_INIT(CPropertiesBar)
	//}}AFX_DATA_INIT
}


void CPropertiesBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesBar)
	DDX_Control(pDX, IDC_EVENTS, m_cmbEvents);
	DDX_Control(pDX, IDC_DATE, m_dtcDate);
	DDX_Control(pDX, IDC_DAYS, m_lbDays);
	//}}AFX_DATA_MAP
	DDX_DateTime(pDX,IDC_DATE,IDC_TIME,m_Time);
}


BEGIN_MESSAGE_MAP(CPropertiesBar, CDialogBar)
	//{{AFX_MSG_MAP(CPropertiesBar)
	ON_BN_CLICKED(IDC_ONCE, OnOnce)
	ON_BN_CLICKED(IDC_DAILY, OnDaily)
	ON_CBN_SELCHANGE(IDC_EVENTS, OnSelChangeEvents)
	ON_LBN_SELCHANGE(IDC_DAYS, OnSelChangeDays)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar message handlers

BOOL CPropertiesBar::Create(CWnd *pParentWnd,UINT nStyle)
{
	if(!CDialogBar::Create(pParentWnd,IDD,nStyle,IDD))
		return FALSE;
	
	// Fill dialog with data
	FillData();
	
	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();
	rEventLib.LoadEvents(m_hWnd);
	RECT rectEventPlace;
	GetDlgItem(IDC_EVENTPLACE)->GetWindowRect(&rectEventPlace);
	ScreenToClient(&rectEventPlace);

	unsigned i;
	unsigned nEvents=rEventLib.GetEventCount();
	for(i=0;i<nEvents;i++)
	{
		rEventLib.SetWorkEvent(i);
		m_cmbEvents.AddString(rEventLib.GetEventName());
		rEventLib.Move(rectEventPlace,FALSE);
	}
	
	return TRUE;
}

void CPropertiesBar::FillData()
{
	// CTime MinTime(1970,1,2,0,0,0);
	CTime MinTime=CTime::GetCurrentTime();
	CTime MaxTime(2038,1,18,23,59,59);
	
	UpdateData(FALSE);

	m_dtcDate.SetRange(&MinTime,&MaxTime);

	// Fill days listbox
	int iDay;
	CString str;
	for(iDay=dayFirst;iDay<=dayLast;iDay++)
	{
		str.LoadString(iDay);
		m_lbDays.AddString(str);
	}
}

void CPropertiesBar::OnCancel()
{
	GetParentFrame()->SetFocus();
}

void CPropertiesBar::OnOnce()
{
	OnceDaily(TRUE);
}

void CPropertiesBar::OnDaily()
{
	OnceDaily(FALSE);
}

void CPropertiesBar::OnceDaily(BOOL bOnce)
{
	GetDlgItem(IDC_DAYS)->EnableWindow(bOnce?FALSE:TRUE);
	GetDlgItem(IDC_DATE)->EnableWindow(bOnce?TRUE:FALSE);
	CheckRadioButton(IDC_ONCE,IDC_DAILY,bOnce?IDC_ONCE:IDC_DAILY);
	m_Schedule.Once=bOnce?1:0;
}

void CPropertiesBar::OnSelChangeEvents() 
{
	int iCurSel=m_cmbEvents.GetCurSel();
	if(iCurSel!=CB_ERR && iCurSel != m_iCurSel)
	{
		EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();
		rEventLib.Show(FALSE);
		rEventLib.SetWorkEvent(iCurSel);
		rEventLib.Show(TRUE);
		m_iCurSel=iCurSel;
	}
}

void CPropertiesBar::Empty()
{
	if(m_iCurSel!=CB_ERR)
		((CPlanApp *)AfxGetApp())->GetEventLib().Empty();
}

CTime CPropertiesBar::GetTime()
{
	return m_Time;
}

BOOL CPropertiesBar::IsOnce()
{
	return IsDlgButtonChecked(IDC_ONCE);
}

void CPropertiesBar::GetSchedule(Schedule *pSchedule)
{
	ASSERT(pSchedule!=NULL);
	*pSchedule=m_Schedule;
}

void CPropertiesBar::OnSelChangeDays() 
{
	int nCount=m_lbDays.GetSelCount();
	int *p=new int[nCount];
	int *ip=(int *)&m_Schedule;
	int i;

	m_lbDays.GetSelItems(nCount,p);
	m_Schedule.Set();
	for(i=0;i<nCount;i++)
		*ip|=2<<*(p+i);

	delete [] p;

}

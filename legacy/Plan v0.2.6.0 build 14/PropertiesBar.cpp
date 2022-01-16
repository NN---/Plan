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
	//nEventLib = 0;
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
	
	m_EventLib.LoadEvents(m_hWnd);
	RECT rectEventPlace;
	GetDlgItem(IDC_EVENTPLACE)->GetWindowRect(&rectEventPlace);
	ScreenToClient(&rectEventPlace);

	unsigned i;
	unsigned nEvents=m_EventLib.GetEventCount();
	for(i=0;i<nEvents;i++)
	{
		m_EventLib.SetEventLib(i);
		m_cmbEvents.AddString(m_EventLib.GetEventName());
		m_EventLib.Move(rectEventPlace);
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
	int i;
	CString str;
	for(i=0;i<7;i++)
	{
		str.LoadString(IDS_SUNDAY+i);
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
}

void CPropertiesBar::OnSelChangeEvents() 
{
	int iCurSel=m_cmbEvents.GetCurSel();
	if(iCurSel!=CB_ERR && iCurSel != m_iCurSel)
	{
		m_EventLib.Show(FALSE);
		m_EventLib.SetEventLib(iCurSel);
		m_EventLib.Show(TRUE);
		m_iCurSel=iCurSel;
	}
}

EventLib &CPropertiesBar::GetEventLib()
{
	return m_EventLib;
}

CTime CPropertiesBar::GetTime()
{
	return m_Time;
}

void CPropertiesBar::GetTimeString(LPTSTR szTimeString)
{
	SYSTEMTIME st;
	UpdateData();
	m_Time.GetAsSystemTime(st);

	ASSERT(szTimeString != NULL);
	if(szTimeString == NULL)
		return;

	szTimeString[0]=0;

	if(IsDlgButtonChecked(IDC_ONCE))
	{
		::GetDateFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE|DATE_SHORTDATE
			,&st,NULL,szTimeString,
			::GetDateFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE|DATE_SHORTDATE
			,&st,NULL,NULL,0));
		lstrcat(szTimeString,_T(" - "));
	}

	TCHAR str[20];
	
	::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
		&st,NULL,str,
		::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
		&st,NULL,NULL,0));

	lstrcat(szTimeString,str);
}

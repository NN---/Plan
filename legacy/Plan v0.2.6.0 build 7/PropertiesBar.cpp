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
	//{{AFX_DATA_INIT(CPropertiesBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertiesBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesBar)
	DDX_Control(pDX, IDC_DATE, m_dtcDate);
	DDX_Control(pDX, IDC_DAYS, m_lbDays);
	//}}AFX_DATA_MAP
	DDX_DateTime(pDX,IDC_DATE,IDC_TIME,m_Time);
}


BEGIN_MESSAGE_MAP(CPropertiesBar, CDialogBar)
	//{{AFX_MSG_MAP(CPropertiesBar)
	ON_BN_CLICKED(IDC_ONCE, OnOnce)
	ON_BN_CLICKED(IDC_DAILY, OnDaily)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar message handlers

BOOL CPropertiesBar::Create(CWnd *pParentWnd,UINT nStyle)
{
	return CDialogBar::Create(pParentWnd,IDD,nStyle,IDD);
}

void CPropertiesBar::OnCancel()
{
	GetParentFrame()->SetFocus();
}

void CPropertiesBar::Initialize()
{
	//CTime MinTime(1970,1,2,0,0,0);
	CTime MinTime=CTime::GetCurrentTime();
	CTime MaxTime(2038,1,18,23,59,59);

	UpdateData(FALSE);

	OnOnce();
	CheckDlgButton(IDC_ONCE,1);
	m_dtcDate.SetRange(&MinTime,&MaxTime);

	// Fill days listbox
	m_lbDays.AddString("Sunday");
	m_lbDays.AddString("Monday");
	m_lbDays.AddString("Tuesday");
	m_lbDays.AddString("Wednesday");
	m_lbDays.AddString("Thursday");
	m_lbDays.AddString("Friday");
	m_lbDays.AddString("Saturday");
}

void CPropertiesBar::OnOnce() 
{
	GetDlgItem(IDC_DAYS)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATE)->EnableWindow(TRUE);
}

void CPropertiesBar::OnDaily() 
{
	GetDlgItem(IDC_DAYS)->EnableWindow(TRUE);
	GetDlgItem(IDC_DATE)->EnableWindow(FALSE);
}

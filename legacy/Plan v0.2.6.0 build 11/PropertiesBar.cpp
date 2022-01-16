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
	if(!CDialogBar::Create(pParentWnd,IDD,nStyle,IDD))
		return FALSE;
	
	// Fill dialog with data
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
	
	return TRUE;
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

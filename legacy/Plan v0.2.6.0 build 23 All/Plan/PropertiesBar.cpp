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
	m_Data.m_Schedule.Set();
	m_Data.m_Time=CTime::GetCurrentTime();
	m_iCurSel = CB_ERR;
	//{{AFX_DATA_INIT(CPropertiesBar)
	//}}AFX_DATA_INIT
}


void CPropertiesBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesBar)
	DDX_Control(pDX, IDC_EVENTS, m_cmbEvents);
	DDX_Control(pDX, IDC_DAYS, m_lbDays);
	//}}AFX_DATA_MAP
	DDX_PropertiesBar(pDX);
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
	rEventLib.SetParent(m_hWnd);
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

	((CDateTimeCtrl *)GetDlgItem(IDC_DATE))->SetRange(&MinTime,&MaxTime);

	// Fill days listbox
	unsigned uDay;
	CString str;
	for(uDay=IDS_FIRSTDAY;uDay<=IDS_LASTDAY;uDay++)
	{
		str.LoadString(uDay);
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
	m_Data.m_Schedule.SetOnce(bOnce);
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

void CPropertiesBar::OnSelChangeDays() 
{
	UpdateData();
}

void CPropertiesBar::GetData(Data &rData)
{
	UpdateData();
	rData=m_Data;
}

void CPropertiesBar::SetData(const Data &rData)
{
	m_Data=rData;
	UpdateData(FALSE);
}

// Data Exchange
// PropertiesBar
void CPropertiesBar::DDX_PropertiesBar(CDataExchange *pDX)
{
	if(!(HWND)m_lbDays)
		return;

	bool bOnce=m_Data.m_Schedule.Once;

	if(pDX->m_bSaveAndValidate)
	{
		if(!bOnce)
		{
			int nCount=m_lbDays.GetSelCount();
			int *p=new int[nCount];
			int *ip=(int *)&m_Data.m_Schedule;
			int i;

			m_lbDays.GetSelItems(nCount,p);
			m_Data.m_Schedule.Set();
			for(i=0;i<nCount;i++)
				*ip|=2<<*(p+i);

			delete [] p;
		}
	}
	else
	{
		int *ip=(int *)&m_Data.m_Schedule;
		int i;

		for(i=0;i<sizeof(Data::Schedule)/sizeof(char);i++)
			m_lbDays.SetSel(i,2<<*(ip+i));
	}
	
	CDateTimeCtrl* pWndTime = (CDateTimeCtrl*) GetDlgItem(IDC_TIME);
	CDateTimeCtrl* pWndDate = (CDateTimeCtrl*) GetDlgItem(IDC_DATE);

	if(!pWndTime || !pWndDate)
		return;

	if(pDX->m_bSaveAndValidate)
	{
		CTime tmptime;
		pWndTime->GetTime(tmptime);
		if(bOnce)
		{
			CTime tmpdate;
			pWndDate->GetTime(tmpdate);
			m_Data.m_Time=CTime(tmpdate.GetYear(),tmpdate.GetMonth(),tmpdate.GetDay(),
			tmptime.GetHour(),tmptime.GetMinute(),tmptime.GetSecond());
		}
		else
			m_Data.m_Time=tmptime;
	}
	else
	{
		pWndTime->SetTime(&m_Data.m_Time);
		if(!bOnce)
			pWndDate->SetTime(&m_Data.m_Time);
	}
}

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
	phEventLib = NULL;
	nEventLib = 0;
	pID = NULL;
	m_iCurSel = CB_ERR;

	//{{AFX_DATA_INIT(CPropertiesBar)
		// NOTE: the ClassWizard will add member initialization here
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
	ON_WM_DESTROY()
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
	
	// Find and Load Libraries
	LoadEventLib();
	
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

void CPropertiesBar::LoadEventLib()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile;

	hFindFile = ::FindFirstFile(_T("*.dll"),&FindFileData);
	
	if(hFindFile)
	{
		HMODULE hModule;
		do{
			// LoadLibrary
			hModule=::LoadLibrary(FindFileData.cFileName);
			// Check return value
			if(!hModule)
				continue;
			// CheckEventLibrary
			if(!CheckEventLibrary(hModule))
			{
				::FreeLibrary(hModule);
				continue;
			}
			// Load
			DGPA(hModule,Load);
			if(!NPF(Load)(m_hWnd))
			{
				::FreeLibrary(hModule);
				continue;
			}
			// Succeed
			// Allocate memory for hModule and ID
			nEventLib++;
			
			phEventLib = (HMODULE *)realloc(phEventLib,nEventLib*sizeof(HMODULE));
			*(phEventLib+nEventLib-1) = hModule;

			DGPA(hModule,GetID);
			pID = (const EventID **)realloc((void *)pID, nEventLib*sizeof(EventID *));
			*(pID+nEventLib-1) = NPF(GetID)();

			// GetName
			DGPA(hModule,GetName);
			m_cmbEvents.AddString(NPF(GetName)());
			// EventPlace
			RECT rectEventPlace;
			GetDlgItem(IDC_EVENTPLACE)->GetWindowRect(&rectEventPlace);
			ScreenToClient(&rectEventPlace);
			// Move
			DGPA(hModule,Move);
			NPF(Move)(rectEventPlace,0);
		}while(::FindNextFile(hFindFile,&FindFileData));
	}
}

void CPropertiesBar::OnDestroy() 
{
	CDialogBar::OnDestroy();
	
	// Free Libraries
	DPF(Unload);
	int i;
	for(i=0;i<nEventLib;i++)
	{
		FNGPA(*(phEventLib+i),Unload)();
		::FreeLibrary(*(phEventLib+i));
	}
	free(phEventLib);
	free(pID);
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
		ShowEvent(m_iCurSel,FALSE);
		ShowEvent(iCurSel,TRUE);
		m_iCurSel=iCurSel;
	}
}

void CPropertiesBar::ShowEvent(int iEvent,BOOL bShow)
{
	if(iEvent == CB_ERR)
		return;
	ASSERT(iEvent >= 0 && iEvent <nEventLib);
	FDGPA(phEventLib[iEvent],Show)(bShow);
}

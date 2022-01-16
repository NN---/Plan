// MainFrame.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Plan.h"

#include "MainFrame.h"

#include "PlanDoc.h"
#include "PlanView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SHOWME,OnLoadMe)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bFirstTime=TRUE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CString strText;
	EnableDocking(CBRS_ALIGN_ANY);
	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	strText.LoadString(IDS_TOOLBAR);
	m_wndToolBar.SetWindowText(strText);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if( !m_PropertiesBar.Create(this,WS_CHILD | WS_VISIBLE | CBRS_LEFT
		/*| CBRS_GRIPPER */| CBRS_TOOLTIPS | CBRS_FLYBY))
	{
		TRACE0("Failed to create propertiesbar\n");
		return -1;
	}
	m_PropertiesBar.Initialize();
	strText.LoadString(IDS_PROPERTIESBAR);
	m_PropertiesBar.SetWindowText(strText);
	m_PropertiesBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockControlBar(&m_PropertiesBar);
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;
	
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnDestroy() 
{
	BOOL bMin,bMax;
	WINDOWPLACEMENT wndpl;
	CWinApp *pApp=AfxGetApp();
	wndpl.length=sizeof(WINDOWPLACEMENT);
	
	GetWindowPlacement(&wndpl);
	if(wndpl.showCmd==SW_SHOWMAXIMIZED)
	{
		bMin=FALSE;
		bMax=TRUE;
	}
	else
	if(wndpl.showCmd==SW_SHOWMINIMIZED)
	{
		bMin=TRUE;
		bMax=wndpl.flags?TRUE:FALSE;
	}
	else // if(wndpl.showCmd==SW_SHOWNORMAL)
	{
		bMin=FALSE;
		bMax=FALSE;
	}

	pApp->WriteProfileInt("WindowPos","Max",bMax);
	pApp->WriteProfileInt("WindowPos","Min",bMin);
	pApp->WriteProfileInt("WindowPos","Top",wndpl.rcNormalPosition.top);
	pApp->WriteProfileInt("WindowPos","Bottom",wndpl.rcNormalPosition.bottom);
	pApp->WriteProfileInt("WindowPos","Left",wndpl.rcNormalPosition.left);
	pApp->WriteProfileInt("WindowPos","Right",wndpl.rcNormalPosition.right);

	SaveBarState(AfxGetApp()->m_pszProfileName);
	
	CFrameWnd::OnDestroy();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	if(m_bFirstTime)
	{
		const RECT rectDefault={100,100,600,450};
		BOOL bMin,bMax;
		WINDOWPLACEMENT wndpl;
		CWinApp *pApp=AfxGetApp();

		bMin=pApp->GetProfileInt("WindowPos","Min",FALSE);
		bMax=pApp->GetProfileInt("WindowPos","Max",FALSE);
		wndpl.rcNormalPosition.top=pApp->GetProfileInt("WindowPos","Top",0);
		wndpl.rcNormalPosition.bottom=pApp->GetProfileInt("WindowPos","Bottom",0);
		wndpl.rcNormalPosition.left=pApp->GetProfileInt("WindowPos","Left",0);
		wndpl.rcNormalPosition.right=pApp->GetProfileInt("WindowPos","Right",0);
		if(CRect(0,0,0,0)==wndpl.rcNormalPosition)
			wndpl.rcNormalPosition=rectDefault;

		if(bMin)
		{
			nCmdShow=SW_SHOWMINNOACTIVE;
			if(bMax)
				wndpl.flags=WPF_RESTORETOMAXIMIZED;
			else
				wndpl.flags=WPF_SETMINPOSITION;
		}
		else
		{
			if(bMax)
			{
				nCmdShow=SW_SHOWMAXIMIZED;
				wndpl.flags=WPF_RESTORETOMAXIMIZED;
			}
			else
			{
				nCmdShow=SW_NORMAL;
				wndpl.flags=WPF_SETMINPOSITION;
			}
		}

		wndpl.length=sizeof(WINDOWPLACEMENT);
		wndpl.showCmd=nCmdShow;
		wndpl.ptMinPosition=CPoint(0,0);
		wndpl.ptMaxPosition=CPoint(-::GetSystemMetrics(SM_CXBORDER),
			-::GetSystemMetrics(SM_CYBORDER));

		LoadBarState(AfxGetApp()->m_pszProfileName);
		SetWindowPlacement(&wndpl);

		m_bFirstTime=FALSE;
	}

	CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnClose() 
{	
	CFrameWnd::OnClose();
}

void CMainFrame::OnLoadMe(UINT /*uMsg*/,WPARAM /*wParam*/,LPARAM /*lParam*/)
{
	ShowWindow(SW_RESTORE);
}

void CMainFrame::OnViewProperties() 
{
	if(m_PropertiesBar.IsWindowVisible())
		m_PropertiesBar.SetFocus();
	else
		ShowControlBar(&m_PropertiesBar,TRUE,0);
}

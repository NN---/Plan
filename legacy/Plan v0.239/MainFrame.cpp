// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Plan.h"

#include "MainFrame.h"

#include "PropertiesBar.h"

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
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES, OnUpdateViewProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CString strText;
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(FWS_PREFIXTITLE,0);
	EnableDocking(CBRS_ALIGN_ANY);

	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_DROPDOWN, 
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_SIZE_DYNAMIC |
		CBRS_FLOATING | CBRS_TOOLTIPS | CBRS_GRIPPER) ||
		!m_ToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0(_T("Failed to create toolbar\n"));
		return -1;      // fail to create
	}
	m_ToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_ToolBar);
	strText.LoadString(IDS_TOOLBAR);
	m_ToolBar.SetWindowText(strText);

	if (!m_StatusBar.Create(this) ||
		!m_StatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0(_T("Failed to create status bar\n"));
		return -1;      // fail to create
	}

	if (!m_PropertiesBar.Create(this,CBRS_LEFT))
	{
		TRACE0(_T("Failed to create properties bar\n"));
		return -1;		// fail to create
	}
	m_PropertiesBar.Initialize();
	m_PropertiesBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockControlBar(&m_PropertiesBar);
	strText.LoadString(IDS_EVENTPROPERTIES);
	m_PropertiesBar.SetWindowText(strText);

	LoadBarState(PROFILE_NAME);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
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

void CMainFrame::OnClose() 
{
	WINDOWPLACEMENT wndpl;
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();

	GetWindowPlacement(&wndpl);

	if(wndpl.showCmd!=SW_SHOW&&wndpl.showCmd!=SW_SHOWMAXIMIZED&&
		wndpl.showCmd!=SW_SHOWMINIMIZED)
		wndpl.showCmd=SW_SHOW;
	
	pApp->WriteProfileInt(SETTINGS,WINDOWTOP,wndpl.rcNormalPosition.top);
	pApp->WriteProfileInt(SETTINGS,WINDOWBOTTOM,wndpl.rcNormalPosition.bottom);
	pApp->WriteProfileInt(SETTINGS,WINDOWLEFT,wndpl.rcNormalPosition.left);
	pApp->WriteProfileInt(SETTINGS,WINDOWRIGHT,wndpl.rcNormalPosition.right);
	pApp->WriteProfileInt(SETTINGS,WINDOWSTATE,wndpl.showCmd);
	
	SaveBarState(PROFILE_NAME);

	// Confirm Exit
	if(pApp->m_Options.Exit)
	{
		CString strText;
		strText.LoadString(IDS_AREYOUSURETOEXIT);
		if(MessageBox(strText,NULL,MB_YESNO)!=IDYES)
			return;
	}
	
	CFrameWnd::OnClose();
}

void CMainFrame::WindowShow()
{
	CWinApp *pApp=AfxGetApp();
	WINDOWPLACEMENT wndpl;

	GetWindowPlacement(&wndpl);

	wndpl.length=sizeof(wndpl);
	wndpl.flags=0;
	wndpl.showCmd=pApp->GetProfileInt(SETTINGS,WINDOWSTATE,SW_SHOW);
	wndpl.rcNormalPosition.top=pApp->GetProfileInt(SETTINGS,WINDOWTOP,wndpl.rcNormalPosition.top);
	wndpl.rcNormalPosition.bottom=pApp->GetProfileInt(SETTINGS,WINDOWBOTTOM,wndpl.rcNormalPosition.bottom);
	wndpl.rcNormalPosition.left=pApp->GetProfileInt(SETTINGS,WINDOWLEFT,wndpl.rcNormalPosition.left);
	wndpl.rcNormalPosition.right=pApp->GetProfileInt(SETTINGS,WINDOWRIGHT,wndpl.rcNormalPosition.right);

	SetWindowPlacement(&wndpl);
}

CStatusBar &CMainFrame::GetStatusBar()
{
	return m_StatusBar;
}

CToolBar &CMainFrame::GetToolBar()
{
	return m_ToolBar;
}

CPropertiesBar &CMainFrame::GetPropertiesBar()
{
	return m_PropertiesBar;
}

void CMainFrame::OnViewProperties() 
{
	if(m_PropertiesBar.IsWindowVisible())
		m_PropertiesBar.SetFocus();
	else
		ShowControlBar(&m_PropertiesBar,TRUE,0);
}

void CMainFrame::OnUpdateViewProperties(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_PropertiesBar.IsWindowVisible());
	
}

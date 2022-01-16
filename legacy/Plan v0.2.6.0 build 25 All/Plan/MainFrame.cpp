// MainFrame.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Plan.h"

#include "MainFrame.h"

//
// Change
//
#include "PlanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

// static members
const TCHAR CMainFrame::s_strWindowPos[]=_T("WindowPos");
const TCHAR CMainFrame::s_strMin[]=_T("Min");
const TCHAR CMainFrame::s_strMax[]=_T("Max");
const TCHAR CMainFrame::s_strRect[]=_T("Rect");
const RECT CMainFrame::rectDefault={55,100,640,450};

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_WM_DESTROY()
	ON_COMMAND(ID_EVENT_EMPTY, OnEventEmpty)
	ON_COMMAND(ID_TOOLS_CUSTOMIZE, OnToolsCustomize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = 
{
	ID_SEPARATOR,
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

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~FWS_ADDTOTITLE;
	return CFrameWnd::PreCreateWindow(cs);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CString strText;
	EnableDocking(CBRS_ALIGN_ANY);
	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_MenuBar.Create(this,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_RAISEDBORDER |
		CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_MenuBar.LoadMenuBar(IDR_MAINFRAME))
	{
		ShowError(IDE_MENUBAR,this);
		return -1;      // fail to create
	}
	strText.LoadString(IDS_MENUBAR);
	m_MenuBar.SetWindowText(strText);
	m_MenuBar.EnableDockingEx(CBRS_ALIGN_ANY);
	DockControlBar(&m_MenuBar);

	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		ShowError(IDE_TOOLBAR,this);
		return -1;      // fail to create
	}
	m_ToolBar.SetToolBarInfoForCustomization();
	m_ToolBar.RestoreState();
	strText.LoadString(IDS_TOOLBAR);
	m_ToolBar.SetWindowText(strText);
	m_ToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_ToolBar);

	if(!m_PropertiesBar.Create(this,WS_CHILD | WS_VISIBLE | CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY/* | CBRS_SIZE_DYNAMIC*/))
	{
		ShowError(IDE_PROPERTIESBAR,this);
		return -1;
	}
	strText.LoadString(IDS_PROPERTIESBAR);
	m_PropertiesBar.SetWindowText(strText);
	m_PropertiesBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockControlBar(&m_PropertiesBar);
	
	if (!m_StatusBar.Create
		(this,WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | CBRS_GRIPPER) ||
		!m_StatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		ShowError(IDE_STATUSBAR,this);
		return -1;      // fail to create
	}

	return 0;
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

	pApp->WriteProfileInt(s_strWindowPos,s_strMax,bMax);
	pApp->WriteProfileInt(s_strWindowPos,s_strMin,bMin);
	pApp->WriteProfileBinary(s_strWindowPos,s_strRect,(LPBYTE)&wndpl.rcNormalPosition,sizeof(RECT));
		
	SaveBarState(AfxGetApp()->m_pszProfileName);
	
	CFrameWnd::OnDestroy();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	bool bFirstTime=true;

	if(bFirstTime)
	{
		BOOL bMin,bMax;
		WINDOWPLACEMENT wndpl;
		CPlanApp *pApp=(CPlanApp *)AfxGetApp();

		bMin=pApp->GetProfileInt(s_strWindowPos,s_strMin,FALSE);
		bMax=pApp->GetProfileInt(s_strWindowPos,s_strMax,FALSE);
		
		UINT uBytes=sizeof(RECT);
		RECT *pRect;
		pApp->GetProfileBinary(s_strWindowPos,s_strRect,(LPBYTE *)&pRect,&uBytes);
		wndpl.rcNormalPosition=pRect && uBytes==sizeof(RECT)?*pRect:rectDefault;
		delete pRect;

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

		bFirstTime=false;
	}

	CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::GetData(Data &rData)
{
	m_PropertiesBar.GetData(rData);
}

void CMainFrame::SetData(const Data &rData, 
				PCEventID pcEventID, const LPBYTE pData)
{
	m_PropertiesBar.SetData(rData, pcEventID, pData);
}

void CMainFrame::OnViewProperties() 
{
	if(m_PropertiesBar.IsWindowVisible())
		m_PropertiesBar.SetFocus();
	else
		ShowControlBar(&m_PropertiesBar,TRUE,0);
}

void CMainFrame::OnEventEmpty()
{
	m_PropertiesBar.Empty();
}

void CMainFrame::OnToolsCustomize() 
{
	m_ToolBar.GetToolBarCtrl().Customize();	
}

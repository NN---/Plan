#include "StdAfx.h"
#include "PlanFrame.h"
#include "AboutDlg.h"

// Virtual Functions
BOOL CPlanFrame::PreTranslateMessage(struct tagMSG *pMsg)
{
	if(baseClass::PreTranslateMessage(pMsg))
		return TRUE;

   	if(m_EventView.PreTranslateMessage(pMsg))
		return TRUE;
	return FALSE;
}

BOOL CPlanFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

void CPlanFrame::GetData(Data &rData)
{
	rData;
}

void CPlanFrame::SetData(Data &rData, PCEventID pcEventID, const LPBYTE pData)
{
	rData;
	pcEventID;
	pData;
}

// Message
// WM_CREATE
LRESULT CPlanFrame::OnCreate(UINT /*uMsg*/,WPARAM /*wParam*/,LPARAM /*lParam*/,BOOL &/*bHandled*/)
{
	m_PropertiesBar.Create(m_hWnd);

	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_PLANFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_PLANFRAME, FALSE,
		ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	AddSimpleReBarBand(m_PropertiesBar, NULL, TRUE);

	CreateSimpleStatusBar(ATL_IDS_IDLEMESSAGE,WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS | CCS_BOTTOM, ATL_IDW_STATUS_BAR);

	// Views
	// EventView
	m_hWndClient = m_EventView.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	AddView(&m_EventView);
	
	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, DEF_TOOLBAR);
	UISetCheck(ID_VIEW_STATUS_BAR, DEF_STATUSBAR);
	UISetCheck(ID_VIEW_PROPERTIESBAR, DEF_PROPERTIESBAR);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	InitialUpdateAllViews();

	return 0;
}

// Command
// Menu
// File -> Exit
LRESULT CPlanFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

// View -> ToolBar
LRESULT CPlanFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = DEF_TOOLBAR;
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}
	
// View -> StatusBar
LRESULT CPlanFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

// View -> PropertiesBar
LRESULT CPlanFrame::OnViewPropertiesBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = DEF_PROPERTIESBAR;
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 2);	// PropertiesBar is 3rd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_PROPERTIESBAR, bVisible);
	UpdateLayout();
	return 0;
}

// Help -> About
LRESULT CPlanFrame::OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	return 0;
}
#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "PlanFile.h"
#include "PlanView.h"
#include "PropertiesBar.h"

#define DEF_TOOLBAR 1
#define DEF_STATUSBAR 1
#define DEF_PROPERTIESBAR 1

class CMainFrame : public CFrameWindowImpl<CMainFrame>,
					public CUpdateUI<CMainFrame>,
					public CMessageFilter,
					public CIdleHandler,
					public CPlanFile
{					
	typedef CMainFrame thisClass;
	typedef CFrameWindowImpl<thisClass> baseClass;
public:
	struct Data : public CPropertiesBar::Data
	{
		const bool operator==(const Data &rData) const
		{
			return !memcmp(this,&rData,sizeof(*this));
		}
	
	};
	// Declare FrameWindow Class
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	// Members
	CPlanView m_View;
	CCommandBarCtrl m_CmdBar;
	CPropertiesBar m_PropertiesBar;

	// Virtual functions
	virtual int PreTranslateMessage(struct tagMSG *pMsg)
	{
		if(baseClass::PreTranslateMessage(pMsg))
			return TRUE;

		// Only Active View
		return ((CViewAddWnd *)GetActiveView())->PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Messages
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		// Commands
		// Menu
			// Event -> Empty Properties
			COMMAND_ID_HANDLER(ID_EVENT_EMPTY, OnEventEmpty)
			// View -> ToolBar
			COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
			// View -> StatusBar
			COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
			// View -> Properties
			COMMAND_ID_HANDLER(ID_VIEW_PROPERTIES, OnViewProperties)
		// Chain Message
		CHAIN_MSG_MAP(CPlanFile)
		CHAIN_MSG_MAP(CUpdateUI<thisClass>)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	// UpdateUI
	BEGIN_UPDATE_UI_MAP(thisClass)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_PROPERTIES, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	// Messages
	// WM_CREATE
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(!m_PropertiesBar.Create(m_hWnd))
		{
			DestroyDocument();
			return TRUE;
		}

		if(!CreateDocument())
		{
			DestroyDocument();
			return TRUE;
		}	
		
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE,
			ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
		AddSimpleReBarBand(m_PropertiesBar, NULL, TRUE);

		CreateSimpleStatusBar(ATL_IDS_IDLEMESSAGE,WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
			WS_CLIPSIBLINGS | CCS_BOTTOM, ATL_IDW_STATUS_BAR);

		// Views
		// PlanView
		CPlanView *pPlanView=new CPlanView();
		m_hWndClient = pPlanView->Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
		AddView(pPlanView);
		SetActiveView(pPlanView);
		SetActiveFrame((CFrameWindow *)this);
		
		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, DEF_TOOLBAR);
		UISetCheck(ID_VIEW_STATUS_BAR, DEF_STATUSBAR);
		UISetCheck(ID_VIEW_PROPERTIES, DEF_PROPERTIESBAR);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		LoadRegistry();
		UpdateTitle();

		UpdateAllViews(CViewAdd::Message::Types::Init,(int)(CPlanFile *)this);

		if(m_Options.bEnabled)
			CPlanFile::SetTimer();

		return 0;
	}

	// WM_CLOSE
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		DestroyDocument();
		return 0;
	}
	
	// Commands
	// Menu
	// Event -> Empty Properties
	LRESULT OnEventEmpty(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ 
		return m_PropertiesBar.OnEmpty(wNotifyCode, wID, hWndCtl, bHandled);
	}

	// View -> ToolBar
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}
	
	// View -> Properties
	LRESULT OnViewProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = DEF_PROPERTIESBAR;
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 2);	// PropertiesBar is 3rd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_PROPERTIES, bVisible);
		UpdateLayout();
		return 0;
	}

	// Helper functions
	virtual void UpdateTitle()
	{
		CString strTitle;
		CString strEnabledDisabled;
	
		strTitle.LoadString(IDR_MAINFRAME);
	
		strEnabledDisabled.LoadString(m_Options.bEnabled?IDS_ENABLED:IDS_DISABLED);

		strTitle+=_T(" - ");
		strTitle+=strEnabledDisabled;

		SetWindowText(strTitle);
	}

	virtual void ShowWindow(int nCmdShow=-1)
	{
		if(nCmdShow!=-1)
			m_wndpos.wndpl.showCmd=nCmdShow;
		SetWindowPlacement(&m_wndpos.wndpl);
	}

	virtual void DestroyDocument()
	{
		CPlanFile::DestroyDocument();

		m_PropertiesBar.DestroyWindow();

		baseClass::DestroyWindow();
	}

	void GetData(CPlanFile::Data &rData)
	{
		m_PropertiesBar.GetData(rData);
	}

	void SetData(CPlanFile::Data &rData, PCEventID pcEventID, const LPBYTE pData)
	{
		m_PropertiesBar.SetData(rData,pcEventID,pData);
	}
};

#endif // __MAINFRAME_H__
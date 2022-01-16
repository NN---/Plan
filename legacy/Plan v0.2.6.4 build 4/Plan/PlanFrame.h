#ifndef __PLANFRAME_H__
#define __PLANFRAME_H__

#pragma once

#include "PlanDocument.h"
#include "EventView.h"
#include "EventBar.h"

#define DEF_TOOLBAR 1
#define DEF_STATUSBAR 1
#define DEF_EVENTBAR 1

class CPlanFrame : public CFrameWindowImpl<CPlanFrame>,
					public CUpdateUI<CPlanFrame>,
					public CMessageFilter,
					public CIdleHandler,
					public CPlanDocument
{
	typedef CPlanFrame thisClass;
	typedef CFrameWindowImpl<CPlanFrame> baseClass;
	// Members
	CEventView m_EventView;
	CCommandBarCtrl m_CmdBar;
	CEventBar m_PropertiesBar;
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_PLANFRAME)
	// Virtual functions
	virtual BOOL PreTranslateMessage(struct tagMSG *pMsg);
	virtual BOOL OnIdle();
	virtual void GetData(Data &rData);
	virtual void SetData(Data &rData, PCEventID pcEventID, const LPBYTE pData);
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		// Command
		// Menu
			// File -> Exit
			COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
			// Help -> About
			COMMAND_ID_HANDLER(ID_APP_ABOUT, OnHelpAbout)
			// View -> ToolBar -> ToolBar
			COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
			// View -> StatusBar -> StatusBar
			COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
			// View -> PropertiesBar -> PropertiesBar
			COMMAND_ID_HANDLER(ID_VIEW_PROPERTIESBAR, OnViewPropertiesBar)
		// Chain Message
		CHAIN_MSG_MAP(CPlanDocument)
		CHAIN_MSG_MAP(CUpdateUI<thisClass>)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	// Update UI Map
	BEGIN_UPDATE_UI_MAP(thisClass)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_PROPERTIESBAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled);
	LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewToolBar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewStatusBar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewPropertiesBar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif // __PLANFRAME_H__
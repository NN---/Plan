#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#pragma once

// Includes
#include "DocView.h"
#include "AboutDlg.h"
#include "PlanView.h"
#include "PlanDoc.h"
#include "PropertiesBar.h"
#include "OptionsSheet.h"
#include "RegKey.h"
#include "SystemTime.h"
#include "TrayIconImpl.h"

// Defines
#define HKEY_PROG HKEY_CURRENT_USER
#define STR_REG_PROG _T("Software\\NN\\Plan")
#define STR_REG_RUN _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run")
#define STR_PLAN _T("Plan")
#define STR_SETTINGS _T("Settings")
#define STR_WINDOWPOS _T("WindowPos")
#define STR_OPTIONS _T("Options")
#define STR_EVENTS _T("Events")
#define STR_ID _T("ID")
#define STR_DATA _T("Data")
#define STR_DATASIZE _T("DataSize")
#define STR_FDATA _T("FData")

#define EVENT_MAX_CHAR 3

#define PLAN_FILTER _T("Plan Files (*.pln)")
#define PLAN_EXT _T("pln")

#define TIMER_ID 1000
#define TIMER_DELAY 1000

#define HOTKEY_ID	0x3000

#define DEF_TOOLBAR 1
#define DEF_STATUSBAR 1
#define DEF_PROPERTIESBAR 1

class CMainFrame : public CFrameWindowImpl<CMainFrame>,
					public CUpdateUI<CMainFrame>,
					public CMessageFilter,
					public CIdleHandler,
					public DocTemplateMsg<CPlanDoc, CPlanView, CMainFrame, IDR_MAINFRAME>,
					public CTrayIconImpl<CMainFrame>
{
	typedef CMainFrame thisClass;
	typedef CFrameWindowImpl<CMainFrame> baseClass;
	typedef DocTemplateMsg<CPlanDoc, CPlanView, CMainFrame, IDR_MAINFRAME> baseDocClass;
public:
	// Members
	CCommandBarCtrl m_CmdBar;
	CPropertiesBar m_PropertiesBar;

	// Declare Frame Window Class
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Messages
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
		MESSAGE_HANDLER(GetTrayIconMessage(), OnTrayIcon)
		// Commands
		// Menu
			// File -> Export
			COMMAND_ID_HANDLER(ID_FILE_EXPORT, OnFileExport)
			// File -> Import
			COMMAND_ID_HANDLER(ID_FILE_IMPORT, OnFileImport)
			// File -> Exit
			COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
			// View -> ToolBar
			COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
			// View -> StatusBar
			COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
			// View -> Properties
			COMMAND_ID_HANDLER(ID_VIEW_PROPERTIES, OnViewProperties)
			// Tools -> Enable/Disable
			COMMAND_ID_HANDLER(ID_TOOLS_ENABLEDISABLE, OnToolsEnableDisable)
			// Tools -> Options
			COMMAND_ID_HANDLER(ID_TOOLS_OPTIONS, OnToolsOptions)
			// Help -> About
			COMMAND_ID_HANDLER(ID_APP_ABOUT, OnHelpAbout)
		// PropertiesBar
			// Add Event
			NOTIFY_ID_HANDLER(IDC_ADD, OnAdd)
			// Update Event
			NOTIFY_ID_HANDLER(IDC_UPDATE, OnUpdate)
			// Event -> Delete Event
			NOTIFY_ID_HANDLER(IDC_DELETE, OnDelete)
		// Chain Message Map
		CHAIN_MSG_MAP(baseDocClass)
		CHAIN_MSG_MAP(CUpdateUI<thisClass>)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	// UpdateUI Map
	BEGIN_UPDATE_UI_MAP(thisClass)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_PROPERTIES, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	// Virtual Functions
	virtual int PreTranslateMessage(struct tagMSG *pMsg)
	{
		if(baseClass::PreTranslateMessage(pMsg))
			return TRUE;

		// Only Active View
		//if(GetActiveView())
		//	return GetActiveView()->PreTranslateMessage(pMsg);
		//else
		return FALSE;
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	virtual bool CreateDocument()
	{
//		bool bRes=baseClass::CreateDocument();
//		if(bRes)
//			m_bCreateSuccess=true;
//		return bRes;
	}

	virtual void InitDocument()
	{
		/*
		InitDocument();
		aID.RemoveAll();
		aFData.RemoveAll();
		int i,n;
		for(i=0,n=aData.GetSize();i<n;i++)
			delete aData[i];
		aData.RemoveAll();
		aDataSize.RemoveAll();
		m_numEvents=0;
		*/
	}

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

	virtual void DestroyDocument()
	{
		if(m_bCreateSuccess)
			m_PropertiesBar.DestroyWindow();

		if(m_Options.bEnabled)
			KillTimer(TIMER_ID);

		SaveRegistry();

		baseClass::DestroyDocument();

		baseClass::DestroyWindow();
	}

	virtual void GetData(CPlanDoc::Data &rData)
	{
		m_PropertiesBar.GetData(rData);
	}

	virtual void SetData(CPlanDoc::Data &rData, PCEventID pcEventID, const LPBYTE pData)
	{
		m_PropertiesBar.SetData(rData,pcEventID,pData);
	}

	void UpdateAllViews(CPlanDoc::ViewMessage::Types Type,int iEvent=0)
	{
		ViewMessage message={Type,iEvent};
		baseClass::UpdateAllViews((LPVOID)&message);
	}

	virtual void UpdateAllViews(void *pData)
	{
		int i;
		int n=GetViewArray()->GetSize();
		for(i=0;i<n;i++)
			baseClass::UpdateAllViews(pData);
	}

	virtual void Show(int nCmdShow)
	{
		ShowWindow(nCmdShow);
	}

	// Messages
	// WM_CREATE
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(!m_PropertiesBar.Create(m_hWnd))
		{
			DestroyDocument();
			return -1;
		}

		if(!CreateDocument())
		{
			DestroyDocument();
			return -1;
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
		/*CPlanView *pPlanView=new CPlanView();
		m_hWndClient = pPlanView->Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
		AddView(pPlanView);
		SetActiveView(pPlanView);*/
//		AddDocTemplate(&m_MyDocTemplate);
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		CreateContext<CPlanDoc, CPlanView> * pContext =
         (CCreateContext<CPlaDoc, CPlanView> *)lpms->lParam;
		ATLASSERT(pContext->m_pCurrentView == NULL);
		pContext->m_pCurrentView = new CPlanView();
		m_pView = pContext->m_pCurrentView;
		m_hWndClient = m_pView->Create(m_hWnd, pContext);
		bHandled = FALSE;
		
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

		UpdateAllViews(ViewMessage::Types::Init,(int)(CMainFrameBase *)this);

		if(m_Options.bEnabled)
			SetTimer(TIMER_ID,TIMER_DELAY);

		return 0;
	}

	// WM_CLOSE
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		DestroyDocument();
		return 0;
	}

	// WM_TIMER
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(!m_numEvents)
			return 0;
		SYSTEMTIME stcurtime;
		SYSTEMTIME st;
		GetSystemTime(&stcurtime);
		Data *pFData=NULL;
		DWORD dwEvent;
		for(dwEvent=0;dwEvent<m_numEvents;dwEvent++)
		{
			pFData=&aFData[dwEvent];
			if(pFData->m_Schedule.Once)
			{
				st=pFData->m_Time-stcurtime;
				if(!st.wYear&&
					!st.wMonth&&
					!st.wDay&&
					!st.wHour&&
					!st.wMinute&&
					st.wSecond<=TIMER_DELAY/1000)
				{
					_EventLib.Do(aData[dwEvent]);
					break;
				}
			}
			else
			{
				if(!(1<<stcurtime.wDayOfWeek & pFData->m_Schedule.GetDayOfWeek()))
					continue;
				st.wYear=0;
				st.wMonth=0;
				st.wDay=0;
				st.wHour=(WORD)(stcurtime.wHour-pFData->m_Time.wHour);
				st.wMinute=(WORD)(stcurtime.wMinute-pFData->m_Time.wMinute);
				st.wSecond=(WORD)(stcurtime.wSecond-pFData->m_Time.wSecond);
				st.wMilliseconds=0;
				if(!st.wHour&&
					!st.wMinute&&
					st.wSecond<=TIMER_DELAY/1000)
				{
					_EventLib.Do(aData[dwEvent]);
					break;
				}
			}
		}
		return 0;
	}

	// WM_SYSCOMMAND
	LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled=FALSE;
		return 0;
	}

	// WM_HOTKEY
	LRESULT OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		_EventLib.SetWorkEvent(wParam-HOTKEY_ID);
		_EventLib.Do(aData[wParam-HOTKEY_ID]);
		return 0;
	}

	// GetTrayIconMessage()
	LRESULT OnTrayIcon(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(wParam==WM_LBUTTONDBLCLK)
		{
		}
		bHandled=FALSE;
		return 0;
	}

	// Commands
	// Menu
	// File -> Export
	/*	Structure of file .pln 
		----------------------
		Version
		NumEvents
		[Event]
			ID
			FrameData
			DataSize
			Data
	*/
	LRESULT OnFileExport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if(!m_numEvents)
		{
			ShowError();
			return 0;
		}
		
		OPENFILENAME ofn;
		TCHAR szFile[MAX_PATH];
		szFile[0]=0;
						
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = PLAN_FILTER;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_HIDEREADONLY |
			OFN_OVERWRITEPROMPT	| OFN_CREATEPROMPT;
		if(!GetSaveFileName(&ofn))
			return 0;

		CFile file;
		if(!file.Create(ofn.lpstrFile,GENERIC_WRITE,0,OPEN_ALWAYS))
		{
			ShowError();
			return 0;
		}
	
		file.SetSize(0);

		file.Write(&curVersion,sizeof(Version));
		file.Write(&m_numEvents,sizeof(DWORD));
		DWORD i;
		DWORD DataSize;
		for(i=0;i<m_numEvents;i++)
		{
			file.Write(aID[i],sizeof(EventID));
			file.Write(&aFData[i],sizeof(Data));
			DataSize=aDataSize[i];
			file.Write(&DataSize,sizeof(DWORD));
			file.Write(aData[i],DataSize);
		}

		return 0;
	}

	// File -> Import
	/*	Structure of file .pln 
		----------------------
		Version
		NumEvents
		[Event]
			ID
			FrameData
			DataSize
			Data
	*/
	LRESULT OnFileImport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		OPENFILENAME ofn;
		TCHAR szFile[MAX_PATH];
		szFile[0]=0;
						
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = PLAN_FILTER;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_HIDEREADONLY |
			OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
		if(!GetOpenFileName(&ofn))
			return 0;

		CFile file;
		if(!file.Create(ofn.lpstrFile,GENERIC_READ,0,OPEN_EXISTING))
		{
			ShowError();
			return 0;
		}
	
		// Version
		Version version;
		if(!file.Read(&version,sizeof(Version)))
		{
			ShowError();
			return 0;
		}

		DWORD i;
		DWORD numVersions=sizeof(validVersion)/sizeof(Version);
		for(i=0;i<numVersions;i++)
		{
			if(!memcmp(&version,&validVersion[i],sizeof(Version)))
				break;
		}
		if(i==numVersions)
		{
			ShowError();
			return 0;
		}

		DWORD numEvents;
		file.Read(&numEvents,sizeof(DWORD));
		if(!numEvents)
			return 0;

		DWORD nEvents=m_numEvents;
		EventID eventID;
		Data fdata;
		DWORD uSize;
		LPBYTE pData;
		for(i=0;i<numEvents;i++)
		{
			file.Read(&eventID,sizeof(EventID));
			if(_EventLib.Find(&eventID,TRUE)==-1)
				continue;
	
			file.Read(&fdata,sizeof(Data));
	
			if(m_numEvents)
			{
				DWORD j;
				for(j=0;j<m_numEvents;j++)
				{
					if(aFData[j]==fdata)
					break;
				}
				if(j!=m_numEvents)
					continue;
			}

			file.Read(&uSize,sizeof(UINT));
			pData=new BYTE [uSize];
			file.Read(pData,uSize);
	
			aID.Add(_EventLib.GetEventID());
			aFData.Add(fdata);
			aDataSize.Add(uSize);
			aData.Add(pData);
	
			UpdateAllViews(ViewMessage::Types::AddEvent,nEvents);

			nEvents++;
		}
		m_numEvents=nEvents;

		return 0;
	}

	// File -> Exit
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
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

	// Tools -> Enable/Disable
	LRESULT OnToolsEnableDisable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if(m_Options.bEnabled)
			KillTimer(TIMER_ID);
		else
			SetTimer(TIMER_ID,TIMER_DELAY);
		m_Options.bEnabled=!m_Options.bEnabled;
		UpdateTitle();
		return 0;
	}
		
	// Tools -> Options
	LRESULT OnToolsOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		COptionsSheet OptionsSheet;
		OptionsSheet.DoModal(&m_Options,m_hWnd);	
		return 0;
	}

	// PropertiesBar
	// IDC_ADD
	LRESULT OnAdd(int, LPNMHDR /*lpNMHDR*/, BOOL& /*bHandled*/)
	{
		Data fdata;
		LPBYTE pData;
		DWORD dwSize;

		if(_EventLib.IsEmpty())
		{
			ShowError();
			return 0;
		}

		GetData(fdata);

		bool bHotKey = fdata.m_HotKey.vk && !fdata.m_HotKey.fsModifiers;
		bool bDay = fdata.m_Schedule.Once ? true : !fdata.m_Schedule.IsNoDays();
		
		if(!bHotKey&&!bDay)
		{
			ShowError();
			return 0;
		}

		dwSize=_EventLib.GetDataSize();
		pData=new BYTE[dwSize];
		if(!pData)
		{
			ShowError();
			return 0;
		}
		_EventLib.GetData(pData);
		aData.Add(pData);
		aDataSize.Add(dwSize);
		aID.Add(_EventLib.GetEventID());
		aFData.Add(fdata);
		
		UpdateAllViews(ViewMessage::Types::AddEvent,m_numEvents);

		m_numEvents++;

		return 0;
	}

	// IDC_UPDATE
	LRESULT OnUpdate(int, LPNMHDR /*lpNMHDR*/, BOOL& /*bHandled*/)
	{
		Data fdata;
		LPBYTE pData;
		DWORD dwSize;
	
		int iSelEvent;
		ViewMessage message={ViewMessage::Types::GetSelectedEvent,0};

		GetActiveView()->UpdateView(&message);
		iSelEvent=message.iEvent;
		if(iSelEvent==-1)
		{
			ShowError();
			return 0;
		}

		if(_EventLib.IsEmpty())
		{
			ShowError();
			return 0;
		}

		GetData(fdata);
		
		dwSize=_EventLib.GetDataSize();
		delete aData[iSelEvent];
		pData=new BYTE[dwSize];
		if(!pData)
		{
			ShowError();
			return 0;
		}
		_EventLib.GetData(pData);
		aData[iSelEvent]=pData;
		aDataSize[iSelEvent]=dwSize;
		aID[iSelEvent]=_EventLib.GetEventID();
		aFData[iSelEvent]=fdata;
		
		UpdateAllViews(ViewMessage::Types::UpdateEvent,iSelEvent);
		
		return 0;
	}

	// IDC_DELETE
	LRESULT OnDelete(int, LPNMHDR /*lpNMHDR*/, BOOL& /*bHandled*/)
	{
		int iSelEvent;
		
		ViewMessage message={ViewMessage::Types::GetSelectedEvent,0};

		GetActiveView()->UpdateView(&message);
		iSelEvent=message.iEvent;

		if(iSelEvent==-1)
		{
			ShowError();
			return 0;
		}

		aData.RemoveAt(iSelEvent);
		aDataSize.RemoveAt(iSelEvent);
		aID.RemoveAt(iSelEvent);
		aFData.RemoveAt(iSelEvent);

		if(m_numEvents)
			m_numEvents--;

		UpdateAllViews(ViewMessage::Types::DeleteEvent,iSelEvent);

		return 0;
	}

	// Help -> About
	LRESULT OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		return 0;
	}

	// Helper Functions
	void LoadRegistry()
	{
		// RegKey
		RegKey regProg;
		LONG lRes;
		DWORD dwType;
		DWORD dwSize;

		lRes=regProg.Open(HKEY_PROG,STR_REG_PROG);
		if(lRes!=ERROR_SUCCESS)
		{
			ShowError();
			return;
		}

		// Settings
		RegKey regSettings;
		lRes=regSettings.Create(regProg,STR_SETTINGS);
		if(lRes==ERROR_SUCCESS)
		{
			// Window Position
			dwType=REG_BINARY;
			dwSize=sizeof(WindowPos);
			lRes=regSettings.QueryValue((LPBYTE)&m_wndpos,STR_WINDOWPOS,&dwSize,&dwType);
			if(lRes!=ERROR_SUCCESS || dwType!=REG_BINARY || dwSize != sizeof(WindowPos))
				// Default
				m_wndpos=defWindowPos;
					
			if(m_wndpos.bMin)
			{
				m_wndpos.wndpl.showCmd=SW_SHOWMINNOACTIVE;
				if(m_wndpos.bMax)
					m_wndpos.wndpl.flags=WPF_RESTORETOMAXIMIZED;
				else
					m_wndpos.wndpl.flags=WPF_SETMINPOSITION;
			}
			else
			{
				if(m_wndpos.bMax)
				{
					m_wndpos.wndpl.showCmd=SW_SHOWMAXIMIZED;
					m_wndpos.wndpl.flags=WPF_RESTORETOMAXIMIZED;
				}
				else
				{
					m_wndpos.wndpl.showCmd=SW_NORMAL;
					m_wndpos.wndpl.flags=WPF_SETMINPOSITION;
				}
			}

			m_wndpos.wndpl.length=sizeof(WINDOWPLACEMENT);
			m_wndpos.wndpl.ptMinPosition=CPoint(0,0);
			m_wndpos.wndpl.ptMaxPosition=CPoint(-::GetSystemMetrics(SM_CXBORDER),
				-::GetSystemMetrics(SM_CYBORDER));

			// Options
			dwType=REG_BINARY;
			dwSize=sizeof(Options);
			lRes=regSettings.QueryValue((LPBYTE)&m_Options,STR_OPTIONS,&dwSize,&dwType);
			if(lRes!=ERROR_SUCCESS || dwType!=REG_BINARY || dwSize != sizeof(Options))
				// Default
				m_Options=defOptions;
		}
		else
			ShowError();

		regSettings.Close();

		// Events
		RegKey regEvents;
		lRes=regEvents.Create(regProg,STR_EVENTS);
		if(lRes==ERROR_SUCCESS)
		{
			TCHAR szEvent[EVENT_MAX_CHAR+1]="";
					
			dwSize=EVENT_MAX_CHAR;
			lRes=regEvents.EnumKey(0,szEvent,&dwSize);
			if(lRes==ERROR_SUCCESS)
			{	
				DWORD nEvents;
				lRes=regEvents.QueryInfoKey(NULL,NULL,&nEvents);
				if(lRes==ERROR_SUCCESS)
				{
					RegKey regEvent;
					DWORD dwEvent,dwDataSize;
					EventID ID;
					Data fdata;
					LPBYTE pData;
					
					for(dwEvent=0;dwEvent<nEvents;dwEvent++)
					{
						// Open
						dwSize=3;
						regEvents.EnumKey(dwEvent,szEvent,&dwSize);
						lRes=regEvent.Open(regEvents,szEvent);
						if(lRes!=ERROR_SUCCESS)
							continue;
		
						// ID
						dwSize=sizeof(EventID);
						dwType=REG_BINARY;
						lRes=regEvent.QueryValue((LPBYTE)&ID,STR_ID,&dwSize,&dwType);
						if(lRes!=ERROR_SUCCESS)
							continue;
						if(dwSize!=sizeof(EventID)||dwType!=REG_BINARY)
							continue;
		
						// Find ID
						dwEvent=_EventLib.Find(&ID);
						if(dwEvent==(DWORD)-1)
							continue;
		
						// FrameData
						dwSize=sizeof(Data);
						dwType=REG_BINARY;
						regEvent.QueryValue((LPBYTE)&fdata,STR_FDATA,&dwSize,&dwType);
						if(lRes!=ERROR_SUCCESS)
							continue;
						if(dwSize!=sizeof(Data)||dwType!=REG_BINARY)
							continue;
		
						// Size
						dwDataSize=0;
						lRes=regEvent.QueryValue(dwDataSize,STR_DATASIZE);
						if(lRes!=ERROR_SUCCESS)
							continue;
						if(!dwDataSize)
							continue;
		
						// Data
						pData=new BYTE[dwDataSize];
						if(!pData)
							continue;
						dwSize=dwDataSize;
						dwType=REG_BINARY;
						lRes=regEvent.QueryValue(pData,STR_DATA,&dwSize,&dwType);
						if(lRes!=ERROR_SUCCESS)
							continue;
						if(dwSize!=dwDataSize||dwType!=REG_BINARY)
						{
							delete [] pData;
							continue;
						}
			
						// Add this Event
						_EventLib.SetWorkEvent(dwEvent);
						aID.Add(_EventLib.GetEventID());
						aFData.Add(fdata);
						aDataSize.Add(dwSize);
						aData.Add(pData);
					}

					m_numEvents=aID.GetSize();
					regEvent.Close();
				}
			}
			regEvents.Close();
		}
		else
			ShowError();

		regProg.Close();
	}

	void SaveRegistry()
	{
		// RegKey
		RegKey regProg;
		LONG lRes;

		lRes=regProg.Create(HKEY_PROG,STR_REG_PROG);
		if(lRes!=ERROR_SUCCESS)
		{
			ShowError();
			return;
		}
		
		// Settings
		RegKey regSettings;
		lRes=regSettings.Create(regProg,STR_SETTINGS);
		if(lRes==ERROR_SUCCESS)
		{
			if(m_bCreateSuccess)
			{
				// Window Position
				WindowPos wndpos;
				wndpos.wndpl.length=sizeof(WINDOWPLACEMENT);
		
				GetWindowPlacement(&wndpos.wndpl);

				if(wndpos.wndpl.showCmd==SW_SHOWMAXIMIZED)
				{
					wndpos.bMin=false;
					wndpos.bMax=true;
				}
				else
				if(wndpos.wndpl.showCmd==SW_SHOWMINIMIZED)
				{
					wndpos.bMin=true;
					wndpos.bMax=wndpos.wndpl.flags?true:false;
				}
				else // if(wndpl.showCmd==SW_SHOWNORMAL)
				{
					wndpos.bMin=false;
					wndpos.bMax=false;
				}

				regSettings.SetValue((const LPBYTE)&wndpos,sizeof(WindowPos),STR_WINDOWPOS);
			}

			// Options
			regSettings.SetValue((const LPBYTE)&m_Options,sizeof(Options),STR_OPTIONS);
			
			RegKey regLoadOnStartup;
			regLoadOnStartup.Open(HKEY_PROG,STR_REG_RUN);
			if(m_Options.bLoadOnStartup)
			{
				TCHAR szFileName[_MAX_PATH];
				LPTSTR psz=szFileName;
				psz=lstrcpy(psz,_T("\""));
				psz=lstrcat(psz,_Module.GetArgV()[0]);
				lstrcat(psz,_T(" -tray\""));
				regLoadOnStartup.SetValue(szFileName,STR_PLAN,_MAX_PATH);
			}
			else
				regLoadOnStartup.DeleteValue(STR_PLAN);

			regLoadOnStartup.Close();
		}
		else
			ShowError();

		regSettings.Close();

		// Events
		RegKey regEvents;
		lRes=regEvents.Create(regProg,STR_EVENTS);
		if(lRes==ERROR_SUCCESS&&m_bCreateSuccess)
		{
			DWORD dwEvent;
			DWORD dwDataSize;
			RegKey regEvent;
			TCHAR szEvent[EVENT_MAX_CHAR+1];
		
			regEvents.DeleteAllSubKeys();	
	
			for(dwEvent=0;dwEvent<m_numEvents;dwEvent++)
			{
				wsprintf(szEvent,_T("%u"),dwEvent);
				lRes=regEvent.Create(regEvents,szEvent);
				if(lRes!=ERROR_SUCCESS)
					continue;
				regEvent.SetValue((const LPBYTE)aID[dwEvent],sizeof(EventID),STR_ID);
				regEvent.SetValue((const LPBYTE)&aFData[dwEvent],sizeof(Data),STR_FDATA);
				dwDataSize=aDataSize[dwEvent];
				regEvent.SetValue(dwDataSize,STR_DATASIZE);
				regEvent.SetValue((const LPBYTE)aData[dwEvent],dwDataSize,STR_DATA);
			}
		}
	}
};

#endif // __MAINFRAME
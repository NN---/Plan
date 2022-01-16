#ifndef __PLANFILE_H__
#define __PLANFILE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DocView.h"
#include "ViewAdd.h"
#include "RegKey.h"
#include "PropertiesBar.h"
#include "AboutDlg.h"

#define EVENT_IMPORT
#include "..\Event\Event.h"
extern EventLib _EventLib;

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

// Version
struct Version
{
	WORD V1;
	WORD V2;
	WORD V3;
	WORD V4;
};

static const Version validVersion[]=
{
	{
		0, // V1
		2, // V2
		6, // V3
		1, // V4
	},
};

class ATL_NO_VTABLE CPlanFile : public CDocument
{
	typedef CPlanFile thisClass;
	typedef CDocument baseClass;
public:
	struct Data : public CPropertiesBar::Data
	{
		const bool operator==(const Data &rData) const
		{
			return !memcmp(this,&rData,sizeof(*this));
		}
	
	};
	class CViewAddWnd : public CWindowImpl<CViewAddWnd>,
							public CViewAdd
	{
	public:
		// Virtual functions
		virtual BOOL PreTranslateMessage(MSG* pMsg){pMsg;return FALSE;}
		virtual int Init(Message *pMessage){pMessage;return 0;}
		virtual int AddEvent(Message *pMessage){pMessage;return 0;}
		virtual int UpdateEvent(Message *pMessage){pMessage;return 0;}
		virtual int DeleteEvent(Message *pMessage){pMessage;return 0;}
		virtual int GetSelectedEvent(Message *pMessage=NULL){pMessage;return 0;}
		DECLARE_EMPTY_MSG_MAP()
	};
	// Members
	struct Options
	{
		unsigned bEnabled : 1;
		unsigned bLoadOnStartup : 1;
	};
	Options m_Options;
	static const Options defOptions;
	struct WindowPos
	{
		bool bMax;
		bool bMin;
		WINDOWPLACEMENT wndpl;
	};
	WindowPos m_wndpos;
	static const WindowPos defWindowPos;
	static const Version curVersion;
	DWORD m_numEvents;
	class PCEventIDArray : public CSimpleValArray<PCEventID>
	{
	public:
		PCEventID& operator[] (int nIndex) const
		{
			return CSimpleArray<PCEventID>::operator[](nIndex);
		}
	};
	PCEventIDArray aID;
	CSimpleArray<LPBYTE> aData;
	CSimpleArray<DWORD> aDataSize;
	CSimpleArray<Data> aFData;

	BEGIN_MSG_MAP(thisClass)
		// Messages
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		// Commands
		// Menu			
			// File -> Export
			COMMAND_ID_HANDLER(ID_FILE_EXPORT, OnFileExport)
			// File -> Import
			COMMAND_ID_HANDLER(ID_FILE_IMPORT, OnFileImport)
			// File -> Exit
			COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
			// Event -> Add Event
			COMMAND_ID_HANDLER(ID_EVENT_ADD, OnEventAdd)
			NOTIFY_ID_HANDLER(ID_EVENT_ADD, OnEventAddNotify)
			// Event -> Update Event
			COMMAND_ID_HANDLER(ID_EVENT_UPDATE, OnEventUpdate)
			NOTIFY_ID_HANDLER(ID_EVENT_UPDATE, OnEventUpdateNotify)
			// Event -> Delete Event
			COMMAND_ID_HANDLER(ID_EVENT_DELETE, OnEventDelete)
			NOTIFY_ID_HANDLER(ID_EVENT_DELETE, OnEventDeleteNotify)
			// Tools -> Enable/Disable
			COMMAND_ID_HANDLER(ID_TOOLS_ENABLEDISABLE, OnToolsEnableDisable)
			// Tools -> Options
			COMMAND_ID_HANDLER(ID_TOOLS_OPTIONS, OnToolsOptions)
			// Help -> About
			COMMAND_ID_HANDLER(ID_APP_ABOUT, OnHelpAbout)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	// Messages
	// WM_TIMER
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(!m_numEvents)
			return 0;
		SYSTEMTIME stcurtime;
		SYSTEMTIME st;
		GetSystemTime(&stcurtime);
		CPlanFile::Data *pFData=NULL;
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
			ShowError2();
			return 0;
		}
		
		OPENFILENAME ofn;
		TCHAR szFile[MAX_PATH];
		szFile[0]=0;
						
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GetActiveFrame()->m_hWnd;
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
			ShowError2();
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
		ofn.hwndOwner = GetActiveFrame()->m_hWnd;
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
			ShowError2();
			return 0;
		}
	
		// Version
		Version version;
		if(!file.Read(&version,sizeof(Version)))
		{
			ShowError2();
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
			ShowError2();
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
	
			UpdateAllViews(CViewAdd::Message::Types::AddEvent,nEvents);

			nEvents++;
		}
		m_numEvents=nEvents;

		return 0;
	}

	// File -> Exit
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		GetActiveFrame()->PostMessage(WM_CLOSE);
		return 0;
	}

	// Event -> Add Event
	LRESULT OnEventAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		Data fdata;
		LPBYTE pData;
		DWORD dwSize;

		if(_EventLib.IsEmpty())
		{
			ShowError2();
			return 0;
		}

		GetData(fdata);
	
		if(!fdata.m_Schedule.Once && fdata.m_Schedule.IsNoDays())
		{
			ShowError2();
			return 0;
		}

		dwSize=_EventLib.GetDataSize();
		pData=new BYTE[dwSize];
		if(!pData)
		{
			ShowError2();
			return 0;
		}
		_EventLib.GetData(pData);
		aData.Add(pData);
		aDataSize.Add(dwSize);
		aID.Add(_EventLib.GetEventID());
		aFData.Add(fdata);
		
		UpdateAllViews(CViewAdd::Message::Types::AddEvent,m_numEvents);

		m_numEvents++;

		return 0;
	}
	LRESULT OnEventAddNotify(int, LPNMHDR lpNMHDR, BOOL& bHandled)
	{
		return OnEventAdd((WORD)lpNMHDR->code,(WORD)lpNMHDR->idFrom,lpNMHDR->hwndFrom,bHandled);
	}

	// Event -> Update Event
	LRESULT OnEventUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		Data fdata;
		LPBYTE pData;
		DWORD dwSize;
	
		int iSelEvent=((CViewAddWnd *)GetActiveView())->GetSelectedEvent();
		if(iSelEvent==-1)
		{
			ShowError2();
			return 0;
		}

		if(_EventLib.IsEmpty())
		{
			ShowError2();
			return 0;
		}

		GetData(fdata);
		
		dwSize=_EventLib.GetDataSize();
		delete aData[iSelEvent];
		pData=new BYTE[dwSize];
		if(!pData)
		{
			ShowError2();
			return 0;
		}
		_EventLib.GetData(pData);
		aData[iSelEvent]=pData;
		aDataSize[iSelEvent]=dwSize;
		aID[iSelEvent]=_EventLib.GetEventID();
		aFData[iSelEvent]=fdata;
		
		UpdateAllViews(CViewAdd::Message::Types::UpdateEvent,iSelEvent);
		
		return 0;
	}
	LRESULT OnEventUpdateNotify(int, LPNMHDR lpNMHDR, BOOL& bHandled)
	{
		return OnEventUpdate((WORD)lpNMHDR->code,(WORD)lpNMHDR->idFrom,lpNMHDR->hwndFrom,bHandled);
	}

	// Event -> Delete Event
	LRESULT OnEventDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int iSelEvent=((CViewAddWnd *)GetActiveView())->GetSelectedEvent();
		if(iSelEvent==-1)
		{
			ShowError2();
			return 0;
		}

		aData.RemoveAt(iSelEvent);
		aDataSize.RemoveAt(iSelEvent);
		aID.RemoveAt(iSelEvent);
		aFData.RemoveAt(iSelEvent);

		if(m_numEvents)
			m_numEvents--;

		UpdateAllViews(CViewAdd::Message::Types::DeleteEvent,iSelEvent);

		return 0;
	}
	LRESULT OnEventDeleteNotify(int, LPNMHDR lpNMHDR, BOOL& bHandled)
	{
		return OnEventDelete((WORD)lpNMHDR->code,(WORD)lpNMHDR->idFrom,lpNMHDR->hwndFrom,bHandled);
	}

	// Tools -> Enable/Disable
	LRESULT OnToolsEnableDisable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if(m_Options.bEnabled)
			KillTimer();
		else
			SetTimer();
		m_Options.bEnabled=!m_Options.bEnabled;
		UpdateTitle();
		return 0;
	}
		
	// Tools -> Options
	LRESULT OnToolsOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		class CPageAdd
		{
		public:
			CPageAdd() { m_pOptions = NULL; }
			void SetOptions(CPlanFile::Options *pOptions)
			{m_pOptions=pOptions;}
			CPlanFile::Options *m_pOptions;
		};
		class CGeneralPage : public CPropertyPage<IDD_GENERAL>,
								public CPageAdd
		{
			typedef CGeneralPage thisClass;
		public:
			
			BEGIN_MSG_MAP(thisClass)
				// Message
				MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
				// Commands
				COMMAND_ID_HANDLER(IDC_LOADONSTARTUP, OnLoadOnStartup)
			END_MSG_MAP()
			
			// Messages
			// WM_INITDIALOG
			LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
			{
				CheckDlgButton(IDC_LOADONSTARTUP,m_pOptions->bLoadOnStartup);
				return 0;
			}
			
			// Commands
			// WM_COMMAND - IDC_LOADONSTARTUP
			LRESULT OnLoadOnStartup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
			{
				m_pOptions->bLoadOnStartup=!m_pOptions->bLoadOnStartup;
				return 0;
			}

		};

		CPropertySheet OptionsSheet(IDS_OPTIONS);
		CGeneralPage pageGeneral;
		pageGeneral.SetOptions(&m_Options);

		OptionsSheet.m_psh.dwFlags &= ~(PSH_HASHELP);
		OptionsSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
		OptionsSheet.AddPage(pageGeneral);
	
		OptionsSheet.DoModal();
	
		return 0;
	}

	// Help -> About
	LRESULT OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		return 0;
	}

	// Helper functions
	virtual bool CreateDocument()
	{
		return CDocument::CreateDocument();
		return TRUE;
	}

	virtual void InitDocument()
	{
		CDocument::InitDocument();
		aID.RemoveAll();
		aFData.RemoveAll();
		int i,n;
		for(i=0,n=aData.GetSize();i<n;i++)
			delete aData[i];
		aData.RemoveAll();
		aDataSize.RemoveAll();
		m_numEvents=0;
	}

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
			ShowError2();
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
			ShowError2();

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
			ShowError2();

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
			ShowError2();
			return;
		}
		
		// Settings
		RegKey regSettings;
		lRes=regSettings.Create(regProg,STR_SETTINGS);
		if(lRes==ERROR_SUCCESS)
		{
			// Window Position
			WindowPos wndpos;
			wndpos.wndpl.length=sizeof(WINDOWPLACEMENT);
		
			GetActiveFrame()->GetWindowPlacement(&wndpos.wndpl);

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

			// Options
			regSettings.SetValue((const LPBYTE)&m_Options,sizeof(Options),STR_OPTIONS);
			
			RegKey regLoadOnStartup;
			regLoadOnStartup.Open(HKEY_PROG,STR_REG_RUN);
			if(m_Options.bLoadOnStartup)
			{
				TCHAR szFileName[_MAX_PATH+10]=_T("");
				GetModuleFileName(_Module.GetModuleInstance(),szFileName,_MAX_PATH);
				lstrcat(szFileName,_T(" -min")); // Minimize
				regLoadOnStartup.SetValue(szFileName,STR_PLAN,_MAX_PATH);
			}
			else
				regLoadOnStartup.DeleteValue(STR_PLAN);

			regLoadOnStartup.Close();
		}
		else
			ShowError2();

		regSettings.Close();

		// Events
		RegKey regEvents;
		lRes=regEvents.Create(regProg,STR_EVENTS);
		if(lRes==ERROR_SUCCESS)
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

	void UpdateAllViews(CViewAdd::Message::Types Type,int iEvent=0)
	{
		CViewAdd::Message message={Type,iEvent};
		CDocument::UpdateAllViews((LPVOID)&message);
	}

	virtual void UpdateAllViews(LPVOID lpData)
	{
		CViewAdd::Message *pMessage=(CViewAdd::Message *)lpData;
		ATLASSERT(pMessage != NULL);

		int i;
		int n=GetViewArray()->GetSize();
		for(i=0;i<n;i++)
			(((CViewAddWnd *)GetView(i))->*VTable[pMessage->Type])(pMessage);
		//CDocument::UpdateAllViews(lpData);
	}

	virtual void GetData(Data &/*rData*/)
	{
	}

	virtual void SetData(Data &/*rData*/, PCEventID /*pcEventID*/, const LPBYTE /*pData*/)
	{
	}

	virtual void UpdateTitle()
	{
	}

	virtual void ShowWindow()
	{
	}

	virtual void DestroyDocument()
	{
		if(m_Options.bEnabled)
			KillTimer();

		SaveRegistry();

		CDocument::DestroyDocument();
	}

	DWORD GetNumEvents()
	{
		return m_numEvents;
	}

	Data& GetFData(DWORD nIndex)
	{
		return aFData[nIndex];
	}

	PCEventID GetEventID(DWORD nIndex)
	{
		return aID[nIndex];
	}

	void SelectEvent(int iEvent)
	{
		ATLASSERT(iEvent>=0 && (DWORD)iEvent<m_numEvents);
		SetData(aFData[iEvent],aID[iEvent],aData[iEvent]);
	}

	UINT SetTimer()
	{
		return GetActiveFrame()->SetTimer(TIMER_ID,TIMER_DELAY);
	}

	UINT KillTimer()
	{
		return GetActiveFrame()->KillTimer(TIMER_ID);
	}
};

// static members
const CPlanFile::Options CPlanFile::defOptions=
{
	1, // unsigned bEnabled : 1
	1, // unsigned bLoadOnStartup : 1
};
const CPlanFile::WindowPos CPlanFile::defWindowPos=
{
	false, // bool bMin
	false, // bool bMax
	{ // WINDOWPLACEMENT wndpl
		0, // UINT length
		0, // UINT flags
		SW_NORMAL, // UINT showCmd
		{ // POINT ptMinPosition
			0, // LONG x
			0, // LONG y
		},
		{ // POINT ptMaxPosition
			0, // LONG x
			0, // LONG y
		},
		{ // RECT rcNormalPosition;
			145, // LONG left
			100, // LONG top
			610, // LONG right
			480, // LONG bottom
		}
	}
};
const Version CPlanFile::curVersion=
{
	0, // V1
	2, // V2
	6, // V3
	1, //V4
};

#endif // __PLANFILE_H__
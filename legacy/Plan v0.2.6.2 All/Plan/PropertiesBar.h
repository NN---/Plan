#ifndef __PROPERTIESDLG_H__
#define __PROPERTIESDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define EVENT_IMPORT
#include "..\Event\Event.h"
extern EventLib _EventLib;

class CPropertiesBar : public CDialogImpl<CPropertiesBar>,
						public CWinDataExchange<CPropertiesBar>
{
	typedef CPropertiesBar thisClass;
	typedef CDialogImpl<thisClass> baseClass;
public:
	enum { IDD = IDD_PROPERTIES };

	struct Data
	{
		const bool operator==(const Data &rData) const
		{
			return !memcmp(this,&rData,sizeof(Data));
		}

		struct Schedule
		{
			unsigned Sunday:1;
			unsigned Monday:1;
			unsigned Tuesday:1;
			unsigned Wednesday:1;
			unsigned Thursday:1;
			unsigned Friday:1;
			unsigned Saturday:1;
			unsigned Once:1;

			void Set(const unsigned uOnce=0,
				const unsigned uSunday=0,
				const unsigned uMonday=0,
				const unsigned uTuesday=0,
				const unsigned uWednesday=0,
				const unsigned uThursday=0,
				const unsigned uFriday=0,
				const unsigned uSaturday=0)
			{
					Once=uOnce;
					Sunday=uSunday;
					Monday=uMonday;
					Tuesday=uTuesday;
					Wednesday=uWednesday;
					Thursday=uThursday;
					Friday=uFriday;
					Saturday=uSaturday;
			}
			
			void SetOnce(unsigned uOnce)
			{
				Once=uOnce;
			}

			bool IsNoDays()
			{
				return !(Sunday | Monday |
					Tuesday | Wednesday |
					Thursday | Friday |
					Saturday);
			}

			WORD GetDayOfWeek()
			{
				return (WORD)(*((char *)this)>>1);
			}

		};

		struct HotKey
		{
			int id;
			WORD fsModifiers;
			WORD vk;
			BOOL Register(HWND hWnd)
			{
				return RegisterHotKey(hWnd,id,fsModifiers,vk);
			}
			BOOL Unregister(HWND hWnd)
			{
				return UnregisterHotKey(hWnd,id);
			}
		};

		SYSTEMTIME m_Time;
		Schedule m_Schedule;
		HotKey m_HotKey;
	};

	Data m_Data;
	int m_iCurSel;

	CPropertiesBar()
	{
		m_Data.m_Schedule.Set();
		::GetLocalTime(&m_Data.m_Time);
		m_iCurSel = CB_ERR;
	}

	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Messages
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		// Commands
		COMMAND_ID_HANDLER(IDC_ONCE, OnEventTime)
		COMMAND_ID_HANDLER(IDC_DAILY, OnEventTime)
		COMMAND_ID_HANDLER(IDC_HOTKEYB, OnEventTime)
		COMMAND_ID_HANDLER(ID_EVENT_EMPTY, OnEmpty)
		COMMAND_ID_HANDLER(ID_EVENT_ADD, OnEventCmd)
		COMMAND_ID_HANDLER(ID_EVENT_UPDATE, OnEventCmd)
		COMMAND_ID_HANDLER(ID_EVENT_DELETE, OnEventCmd)
		COMMAND_HANDLER(IDC_EVENTS, CBN_SELCHANGE, OnSelChangeEvents)
		COMMAND_HANDLER(IDC_DAYS, LBN_SELCHANGE, OnSelChangeDays)
	END_MSG_MAP()

	BEGIN_DDX_MAP(thisClass)
		bool bHotKey = IsDlgButtonChecked(IDC_HOTKEYB)==BST_CHECKED;
	
		if(!bHotKey)
		{
			bool bOnce=m_Data.m_Schedule.Once;
			CListBox lbDays=GetDlgItem(IDC_DAYS);

			if(bSaveAndValidate)
			{
				if(!bOnce)
				{
					int nCount=lbDays.GetSelCount();
					int *p=new int[nCount];
					int *ip=(int *)&m_Data.m_Schedule;
					int i;
	
					lbDays.GetSelItems(nCount,p);
					m_Data.m_Schedule.Set();
					for(i=0;i<nCount;i++)
						*ip|=1<<*(p+i);
	
					delete [] p;
				}
			}
			else
			{
				//if(bOnce)
				//	SendMessage(WM_COMMAND,MAKELONG(BN_CLICKED,IDC_ONCE));
				//else
				if(!bOnce)
				{
					int *ip=(int *)&m_Data.m_Schedule;
					int i;
		
					for(i=0;i<sizeof(Data::Schedule)/sizeof(char);i++)
						lbDays.SetSel(i,*ip&1<<i);
	
					//SendMessage(WM_COMMAND,MAKELONG(BN_CLICKED,IDC_DAILY));
				}
			}
			
			CDateTimePickerCtrl dtcTime = GetDlgItem(IDC_TIME);
			CDateTimePickerCtrl dtcDate = GetDlgItem(IDC_DATE);
	
			if(bSaveAndValidate)
			{
				SYSTEMTIME stTime;
				dtcTime.GetSystemTime(&stTime);
				if(bOnce)
				{
					SYSTEMTIME stDate;
					dtcDate.GetSystemTime(&stDate);
				m_Data.m_Time.wYear=stDate.wYear;
				m_Data.m_Time.wMonth=stDate.wMonth;
				m_Data.m_Time.wDayOfWeek=stDate.wDayOfWeek;
				m_Data.m_Time.wDay=stDate.wDay;
				m_Data.m_Time.wHour=stTime.wHour;
				m_Data.m_Time.wMinute=stTime.wMinute;
				m_Data.m_Time.wSecond=stTime.wSecond;
				//m_Data.m_Time.wMilliseconds=stTime.wMilliseconds;
				}
			else
				dtcTime.GetSystemTime(&m_Data.m_Time);
			}
			else
			{
				dtcTime.SetSystemTime(GDT_VALID,&m_Data.m_Time);
				if(bOnce)
					dtcDate.SetSystemTime(GDT_VALID,&m_Data.m_Time);
			}
		}

		CHotKeyCtrl hotkeyctrl = GetDlgItem(IDC_HOTKEY);
		if(bSaveAndValidate)
			hotkeyctrl.GetHotKey(m_Data.m_HotKey.vk,m_Data.m_HotKey.fsModifiers);
		else
			hotkeyctrl.SetHotKey(m_Data.m_HotKey.vk,m_Data.m_HotKey.fsModifiers);
	END_DDX_MAP()

	// Messages
	// WM_INITDIALOG
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		unsigned nEvents=LoadEvents();
		ATLTRACE("Events - %d\n",nEvents);
		if(!nEvents)
		{
			DestroyWindow();
			ShowError(IDE_NOEVENTLIB,m_hWnd);
			return NULL;
		}
		
		// Fill dialog with data
		FillData();
	
		RECT rectEventPlace;
		::GetWindowRect(GetDlgItem(IDC_EVENTPLACE),&rectEventPlace);
		ScreenToClient(&rectEventPlace);

		CComboBox cmbEvents=GetDlgItem(IDC_EVENTS);

		unsigned i;
		for(i=0;i<nEvents;i++)
		{
			_EventLib.SetWorkEvent(i);
			cmbEvents.AddString(_EventLib.GetEventName());
			_EventLib.Move(rectEventPlace,FALSE);
		}

		BOOL bHandled;
		OnEventTime(BN_CLICKED,IDC_ONCE,GetDlgItem(IDC_ONCE),bHandled);
		return 0;
	}

	// WM_DESTROY
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		_EventLib.UnloadEvents();
		return 0;
	}

	// Commands
	// WM_COMMAND - ID_EVENT_ADD
	// WM_COMMAND - ID_EVENT_UPDATE
	// WM_COMMAND - ID_EVENT_DELETE
	LRESULT OnEventCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		NMHDR nmhdr={hWndCtl,wID,wNotifyCode};
		SendMessage(GetParent(),WM_NOTIFY,0,(LPARAM)&nmhdr);
		return 0;
	}

	// WM_COMMAND - IDC_DAILY
	// WM_COMMAND - IDC_ONCE
	// WM_COMMAND - IDC_HOTKEYB
	LRESULT OnEventTime(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		bool bOnce = wID==IDC_ONCE;
		bool bDaily = wID==IDC_DAILY;
		bool bHotKey = wID==IDC_HOTKEYB;
		::EnableWindow(GetDlgItem(IDC_DAYS),bDaily);
		::EnableWindow(GetDlgItem(IDC_DATE),bOnce);
		::EnableWindow(GetDlgItem(IDC_TIME),!bHotKey);
		CheckRadioButton(IDC_ONCE,IDC_HOTKEYB,
			bOnce ? IDC_ONCE : bDaily ? IDC_DAILY : IDC_HOTKEYB);
		if(bHotKey)
			m_Data.m_Schedule.Set();
		else
			m_Data.m_Schedule.SetOnce(bOnce);
		return 0;
	}
	
	// WM_COMMAND - ID_EVENT_EMPTY
	LRESULT OnEmpty(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if(m_iCurSel!=CB_ERR)
			_EventLib.Empty();
		return 0;
	}

	// WM_COMMAND - CBN_SELCHANGE - IDC_EVENTS
	LRESULT OnSelChangeEvents(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int iCurSel=CComboBox(GetDlgItem(IDC_EVENTS)).GetCurSel();
		if(iCurSel!=CB_ERR && iCurSel != m_iCurSel)
		{
			_EventLib.Show(FALSE);
			_EventLib.SetWorkEvent(iCurSel);
			_EventLib.Show(TRUE);
			m_iCurSel=iCurSel;
		}
		return 0;
	}

	// WM_COMMAND - LBN_SELCHANGE - IDC_DAYS
	LRESULT OnSelChangeDays(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		DoDataExchange(DDX_SAVE);
		return 0;
	}

	// Helper functions
	int LoadEvents()
	{
		// Set Current Directory for Exe
		TCHAR szFileName[_MAX_PATH]=_T("");
		GetModuleFileName(_Module.GetModuleInstance(),szFileName,_MAX_PATH);

		int i=_MAX_PATH-1;
		while(*(szFileName+i)!='\\') i--;
		szFileName[i]=0;
	  
		return _EventLib.LoadEvents(m_hWnd,szFileName);
	}

	void FillData()
	{
		CListBox lbDays=GetDlgItem(IDC_DAYS);

		// Fill days listbox
		unsigned uDay;
		CString str;
		for(uDay=IDS_SUNDAY;uDay<=IDS_SATURDAY;uDay++)
		{
			str.LoadString(uDay);
			lbDays.AddString(str);
		}
	}
	
	void CPropertiesBar::GetData(Data &rData)
	{
		DoDataExchange(DDX_SAVE);
		rData=m_Data;
	}

	void CPropertiesBar::SetData(const Data &rData,
				PCEventID pcEventID, const LPBYTE pData)
	{
		m_Data=rData;

		bool bOnce = rData.m_Schedule.Once;
		bool bHotKey = rData.m_HotKey.vk && rData.m_HotKey.fsModifiers && !bOnce;
		WORD wID = (WORD)(bHotKey ? IDC_HOTKEYB : bOnce ? IDC_ONCE : IDC_DAILY);
		BOOL bHandled;
		//SendMessage(WM_COMMAND,MAKELONG(BN_CLICKED,wID));
		OnEventTime(BN_CLICKED, wID, 0/*GetDlgItem(wID)*/ ,bHandled);

		DoDataExchange(DDX_LOAD);

		CComboBox cmbEvents=GetDlgItem(IDC_EVENTS);
		DWORD dwEvent=_EventLib.Find(pcEventID,FALSE);
		cmbEvents.SetCurSel(dwEvent);

		//SendMessage(WM_COMMAND,MAKELONG(CBN_SELCHANGE,IDC_EVENTS));
		OnSelChangeEvents(CBN_SELCHANGE, IDC_EVENTS, 0/*GetDlgItem(IDC_EVENTS)*/, bHandled);

		_EventLib.SetData(pData);
	}
};

#endif // __PROPERTIESDLG_H__

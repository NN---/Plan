#ifndef __PLANVIEW_H__
#define __PLANVIEW_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ViewAdd.h"
#include "PlanFile.h"

#define EVENT_IMPORT
#include "..\Event\Event.h"
extern EventLib _EventLib;

#define STRID_FIRST_COLUMN IDS_EVENT
#define STRID_LAST_COLUMN IDS_SCHEDULE

#define COLUMN_EVENT 0
#define COLUMN_DATE 1
#define COLUMN_TIME 2
#define COLUMN_SCHEDULE 3

class CPlanView : public CWindowImpl<CPlanView, CListViewCtrl>,
					public CViewAdd
{
	typedef CPlanView thisClass;
	typedef CWindowImpl<thisClass,CListViewCtrl> baseClass;
public:
	CPlanFile *m_pParent;
	// Declare SuperClass
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	// Virtual functions
	virtual BOOL PreTranslateMessage(MSG* /*pMsg*/)
	{
		return FALSE;
	}

	virtual int Init(Message *pMessage)
	{
		pMessage;

		ModifyStyle(LVS_TYPEMASK | LVS_ALIGNMASK,
			LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
		/*ModifyStyleEx(0,LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP |
			LVS_EX_TRACKSELECT | LVS_EX_FLATSB  | LVS_EX_GRIDLINES);*/

		m_pParent=(CPlanFile *)pMessage->iEvent;

		// Columns
		SetColumns();
	
		// Items
		SetItems();
	
		return 0;
	}

	virtual int AddEvent(Message *pMessage)
	{
		ATLASSERT(pMessage->Type==Message::Types::AddEvent);
		SetItem(Message::Types::AddEvent,m_pParent->GetNumEvents(),
			&m_pParent->GetFData(pMessage->iEvent));
		return 0;
	}

	virtual int UpdateEvent(Message *pMessage)
	{
		ATLASSERT(pMessage->Type==Message::Types::UpdateEvent);
		SetItem(Message::Types::UpdateEvent,pMessage->iEvent,
			&m_pParent->GetFData(pMessage->iEvent));
		return 0;
	}
	
	virtual int DeleteEvent(Message *pMessage)
	{
		ATLASSERT(pMessage->Type==Message::Types::DeleteEvent);
		DeleteItem(pMessage->iEvent);
		return 0;
	}

	virtual int GetSelectedEvent(Message *pMessage=NULL)
	{
		pMessage;
		return GetSelectionMark();
	}
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Messages
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	// Messages
	LRESULT OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		int iEvent=GetSelectionMark();
		if(iEvent!=-1)
			m_pParent->SelectEvent(iEvent);	
	
		return 0;
	}

	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(wParam==VK_RETURN)
		{	
			int iEvent=GetSelectionMark();
			if(iEvent!=-1)
				m_pParent->SelectEvent(iEvent);	
		}
		
		return 0;
	}
	
	void SetColumns()
	{
		static bool bCreated;
		int iColumn;
		CString str;
		
		if(bCreated)
			return;
		
		for(iColumn=STRID_FIRST_COLUMN;iColumn<=STRID_LAST_COLUMN;iColumn++)
		{
			str.LoadString(iColumn);
			InsertColumn(iColumn,str,LVCFMT_LEFT,100,iColumn);
		}
		bCreated=true;
	}

	void SetItems()
	{
		DWORD nItems=m_pParent->GetNumEvents();
		DWORD iItem;
	
		DeleteAllItems();

		for(iItem=0;iItem<nItems;iItem++)
			SetItem(Message::Types::AddEvent,iItem,&m_pParent->GetFData(iItem));
	}

	void SetItem(Message::Types Type,int iItem,const CPlanFile::Data *pFData)
	{
		ATLASSERT(Type==Message::Types::AddEvent||
			Type==Message::Types::UpdateEvent);
		
		DWORD DateTimeSize;
		CString str,s;
		
		bool bAdd=Type==Message::Types::AddEvent;

		if(bAdd)
			if(iItem==-1)
				iItem=GetItemCount();
		if(!pFData)
			return;
		
		if(bAdd)
			InsertItem(iItem,"",0);
		
		_EventLib.Find(m_pParent->GetEventID(iItem),TRUE);
		// Event Name
		baseClass::SetItem(iItem,COLUMN_EVENT,LVIF_TEXT,_EventLib.GetEventName(),0,0,0,0);
		// Date & Time
		
		if(pFData->m_Schedule.Once)
		{
			str.LoadString(IDS_ONCE);
			baseClass::SetItem(iItem,COLUMN_SCHEDULE,LVIF_TEXT,str,0,0,0,0);

			// Date
			DateTimeSize=::GetDateFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
					&pFData->m_Time,NULL,NULL,NULL);

			::GetDateFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
				&pFData->m_Time,NULL,str.GetBuffer(DateTimeSize),DateTimeSize);
			str.ReleaseBuffer();

			baseClass::SetItem(iItem,COLUMN_DATE,LVIF_TEXT,str,0,0,0,0);
		}
		else
		{
			int j;
			int *ip=(int *)&pFData->m_Schedule;
			str.Empty();
			for(j=0;j<=IDS_SATURDAY-IDS_SUNDAY;j++)
				if(*ip&2<<j)
				{
					s.LoadString(IDS_SUNDAY+j);
					str+=s;
					str+=_T(" ");
				}

			baseClass::SetItem(iItem,COLUMN_SCHEDULE,LVIF_TEXT,str,0,0,0,0);
			
			// No Date
			baseClass::SetItem(iItem,COLUMN_DATE,LVIF_TEXT,"",0,0,0,0);
		}

		// Time
		DateTimeSize=::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
			&pFData->m_Time,NULL,NULL,NULL);

		::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
			&pFData->m_Time,NULL,str.GetBuffer(DateTimeSize),DateTimeSize);
		str.ReleaseBuffer();

		baseClass::SetItem(iItem,COLUMN_TIME,LVIF_TEXT,str,0,0,0,0);
	}

	virtual void UpdateView(void *pData)
	{
		pData;
		/*
		Message *pMessage=(Message *)pData;
		(((CViewAddWnd *)this)->*VTable[pMessage->Type])(pMessage);
		*/
	}

	virtual void DestroyView()
	{
		DestroyWindow();
	}
};


#endif // __PLANVIEW_H__
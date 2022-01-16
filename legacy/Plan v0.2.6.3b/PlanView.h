#ifndef __PLANVIEW_H__
#define __PLANVIEW_H__

#pragma once

#include "DocView.h"
//#include "MainFrameBase.h"
//#include "ViewMessage.h"
#include "PlanDoc.h"

#define EVENT_IMPORT
#include "..\Event\Event.h"
extern EventLib _EventLib;

#define STRID_FIRST_COLUMN IDS_EVENT
#define STRID_LAST_COLUMN IDS_HOTKEY

#define COLUMN_EVENT 0
#define COLUMN_DATE 1
#define COLUMN_TIME 2
#define COLUMN_SCHEDULE 3
#define COLUMN_HOTKEY 4

class CPlanView : public CWindowImpl<CPlanView,CListViewCtrl>,
					public ViewImpl<CPlanView,CPlanDocView>
{
	typedef CPlanView thisClass;
	typedef CWindowImpl<CPlanView,CListViewCtrl> baseClass;
public:

	// Declare SuperClass
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	// Virtual functions
	//virtual BOOL PreTranslateMessage(MSG* /*pMsg*/)
	/*{
		return FALSE;
	}
*/
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Messages
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	// Messages
	// WM_LBUTTONDBLCLK
	LRESULT OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		int iEvent=GetSelectionMark();
		if(iEvent!=-1)
			GetDocument()->SelectEvent(iEvent);	
	
		return 0;
	}

	// WM_KEYDOWN
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(wParam==VK_RETURN)
		{	
			int iEvent=GetSelectionMark();
			if(iEvent!=-1)
				GetDocument()->SelectEvent(iEvent);	
		}
		
		return 0;
	}

	virtual void OnInitialUpdate()
	{
		ModifyStyle(LVS_TYPEMASK | LVS_ALIGNMASK,
			LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
		ModifyStyleEx(0,LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP |
			LVS_EX_TRACKSELECT | LVS_EX_FLATSB  | LVS_EX_GRIDLINES);

		// Columns
		SetColumns();
	
		// Items
		SetItems();
	}

	void AddEvent(int iEvent)
	{
		SetItem(CPlanDoc::ViewMessage::Types::AddEvent,GetDocument()->GetNumEvents(),
			&GetDocument()->GetFData(iEvent));
	}

	void UpdateEvent(int iEvent)
	{
		SetItem(CPlanDoc::ViewMessage::Types::UpdateEvent,iEvent,
			&GetDocument()->GetFData(iEvent));
	}
	
	void DeleteEvent(int iEvent)
	{
		DeleteItem(iEvent);
	}

	void GetSelectedEvent(int *piEvent)
	{
		ATLASSERT(piEvent);
		*piEvent=GetSelectionMark();
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
			InsertColumn(iColumn,str,LVCFMT_LEFT,90,iColumn);
		}
		bCreated=true;
	}

	void SetItems()
	{
		DWORD nItems=GetDocument()->GetNumEvents();
		DWORD iItem;
	
		DeleteAllItems();

		for(iItem=0;iItem<nItems;iItem++)
			SetItem(CPlanDoc::ViewMessage::Types::AddEvent,iItem,&GetDocument()->GetFData(iItem));
	}

	void SetItem(CPlanDoc::ViewMessage::Types Type,int iItem,const CPlanDoc::Data *pFData)
	{
		ATLASSERT(Type==CPlanDoc::ViewMessage::Types::AddEvent||
			Type==CPlanDoc::ViewMessage::Types::UpdateEvent);
		
		DWORD DateTimeSize;
		CString str,s;
		
		bool bAdd=Type==CPlanDoc::ViewMessage::Types::AddEvent;

		if(bAdd)
			if(iItem==-1)
				iItem=GetItemCount();
		if(!pFData)
			return;
		
		if(bAdd)
			InsertItem(iItem,"",0);
		
		_EventLib.Find(GetDocument()->GetEventID(iItem),TRUE);
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
				if(*ip&1<<j)
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
		if(!pFData->m_HotKey.fsModifiers || !pFData->m_HotKey.vk)
		{
			DateTimeSize=::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
				&pFData->m_Time,NULL,NULL,NULL);
		
			::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
				&pFData->m_Time,NULL,str.GetBuffer(DateTimeSize),DateTimeSize);
			str.ReleaseBuffer();
			baseClass::SetItem(iItem,COLUMN_TIME,LVIF_TEXT,str,0,0,0,0);
		}
		else
		{
			baseClass::SetItem(iItem,COLUMN_HOTKEY,LVIF_TEXT,_T("HotKey"),0,0,0,0);
			baseClass::SetItem(iItem,COLUMN_TIME,LVIF_TEXT,_T(""),0,0,0,0);
		}

		
	}

	virtual void OnFinalMessage(HWND /*hWnd*/)
	{
		GetDocument()->RemoveView(this);
		delete this;
	}

	void OnUpdate(CPlanView* /*pSender*/, LPARAM /*Hint*/, LPVOID pHint)
//	virtual void UpdateView(void *pData)
	{
//		ATLASSERT(pData);
		
		CPlanDoc::ViewMessage *pMessage=(CPlanDoc::ViewMessage *)pHint;
		if(!pHint)
			return;

		switch(pMessage->Type)
		{
			case CPlanDoc::ViewMessage::Types::AddEvent:
				AddEvent(pMessage->iEvent);
				break;
			case CPlanDoc::ViewMessage::Types::UpdateEvent:
				UpdateEvent(pMessage->iEvent);
				break;
			case CPlanDoc::ViewMessage::Types::DeleteEvent:
				DeleteEvent(pMessage->iEvent);
				break;
			case CPlanDoc::ViewMessage::Types::GetSelectedEvent:
				GetSelectedEvent(&pMessage->iEvent);
				break;
		}
	}

/*
	virtual void DestroyView()
	{
		DestroyWindow();
	}*/
};


#endif // __PLANVIEW_H__
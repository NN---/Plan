// PlanView.cpp : implementation of the CPlanView class
//

#include "stdafx.h"
#include "Plan.h"
#include "PlanDoc.h"
#include "PlanView.h"
#include "MainFrame.h"

#include "PropertiesBar.h"

#include "MessageEvent.h"
#include "ShutDownEvent.h"
#include "RunProgramEvent.h"

#include "GeneralPage.h"
#include "ConfirmationsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlanView

IMPLEMENT_DYNCREATE(CPlanView, CListView)

BEGIN_MESSAGE_MAP(CPlanView, CListView)
	//{{AFX_MSG_MAP(CPlanView)
	ON_WM_DESTROY()
	ON_COMMAND(ID_EVENT_ADDEVENT, OnEventAddEvent)
	ON_COMMAND(ID_EVENT_UPDATEEVENT, OnEventUpdateEvent)
	ON_COMMAND(ID_EVENT_REMOVEEVENT, OnEventRemoveEvent)
	ON_COMMAND(ID_EVENT_CLEARPROPERTIES, OnEventClearProperties)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblClk)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeyDown)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND(ID_VIEW_ENABLEDISABLE, OnViewEnableDisable)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanView construction/destruction

CPlanView::CPlanView()
{
	m_pStateImageList=NULL;
	m_bDragging=FALSE;
}

CPlanView::~CPlanView()
{
	delete m_pStateImageList;
}

void CPlanView::OnInitialUpdate()
{
	CListCtrl &ListCtrl=GetListCtrl();
	
	ListCtrl.ModifyStyle(LVS_TYPEMASK | LVS_ALIGNMASK,
		LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |
		LVS_EX_HEADERDRAGDROP | LVS_EX_TRACKSELECT | LVS_EX_FLATSB);
	
	CListView::OnInitialUpdate();
	
	ListCtrl.DeleteAllItems();
	
	if(!m_pStateImageList)
	{
		m_pStateImageList=new CImageList;
		m_pStateImageList->Create(IDB_STATEIMAGE,STATEIMAGE_WIDTH,1,STATEIMAGE_COLORMASK);
		ListCtrl.SetImageList(m_pStateImageList,LVSIL_STATE);
	}

	SetColumns();

	InsertAllEvents();

	if(((CPlanApp *)AfxGetApp())->m_Options.Timer)
		SetTimer(ID_TIMER,TIMER_TIME,Timer);
}

/////////////////////////////////////////////////////////////////////////////
// CPlanView drawing

void CPlanView::OnDraw(CDC* /*pDC*/)
{
	CPlanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CPlanView diagnostics

#ifdef _DEBUG
void CPlanView::AssertValid() const
{
	CListView::AssertValid();
}

void CPlanView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPlanDoc* CPlanView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlanDoc)));
	return (CPlanDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlanView message handlers

void CPlanView::OnDestroy()
{
	KillTimer(ID_TIMER);

	CListView::OnDestroy();
}

void CPlanView::AddUpdateEvent(BOOL bAdd)
{
	Event eEvent;
	Event eItem;
	CPlanDoc *lpDoc=GetDocument();
	CListCtrl &ListCtrl=GetListCtrl();
	CString strText;
	CPropertiesBar &PropertiesBar=((CMainFrame *)GetParentFrame())->GetPropertiesBar();
	int iItem=-1;

	// Confirm Add/Update
	if(bAdd&&((CPlanApp *)AfxGetApp())->m_Options.Add||
		!bAdd&&((CPlanApp *)AfxGetApp())->m_Options.Update)
	{
		strText.LoadString(bAdd?IDS_AREYOUSURETOADD:IDS_AREYOUSURETOUPDATE);
		if(MessageBox(strText,NULL,MB_YESNO)!=IDYES)
			return;
	}
			
	PropertiesBar.UpdateAllData(TRUE);

	if(PropertiesBar.IsEventClear())
	{
		strText.LoadString(IDS_EVENTCLEAR);
		MessageBox(strText,NULL,MB_ERRORI);
		return;
	}
	
	PropertiesBar.GetEvent(&eEvent);
	if(FindEventItem(&eEvent,bAdd?FALSE:TRUE)!=-1)
	{
		strText.LoadString(IDS_TWOEVENTS);
		MessageBox(strText,NULL,MB_ERRORI);
		return;
	}

	if(!bAdd)
	{
		iItem=ListCtrl.GetSelectionMark();
		
		if(iItem==-1)
		{
			strText.LoadString(IDS_SELECTITEM);
			MessageBox(strText,NULL,MB_ERRORI);
			return;
		}
		
		eItem=lpDoc->aItems[iItem];

		if(eItem.Type()!=eEvent.Type())
		{
			strText.LoadString(IDS_UPDATETYPE);
			MessageBox(strText,NULL,MB_ERRORI);
			return;
		}
	}

	// Add/Update event
	switch(eEvent.Type())
	{
		case EVENT_MESSAGE:
		{
			Message eMessage;
			PropertiesBar.GetMessage(&eMessage);
			eMessage.State()=STATEIMAGE_OFF;
			if(bAdd)
				lpDoc->aMessage.Add(eMessage);
			else
				lpDoc->aMessage[FindEventItem(&eItem)]=eMessage;
			SetEvent(eMessage,iItem);
		}
		break;

		case EVENT_SHUTDOWN:
		{
			ShutDown eShutDown;
			PropertiesBar.GetShutDown(&eShutDown);
			eShutDown.State()=STATEIMAGE_OFF;
			if(bAdd)
				lpDoc->aShutDown.Add(eShutDown);
			else
				lpDoc->aShutDown[FindEventItem(&eItem)]=eShutDown;
			SetEvent(eShutDown,iItem);
		}
		break;

		case EVENT_RUNPROGRAM:
		{
			RunProgram eRunProgram;
			PropertiesBar.GetRunProgram(&eRunProgram);
			eRunProgram.State()=STATEIMAGE_OFF;
			if(bAdd)
				lpDoc->aRunProgram.Add(eRunProgram);
			else
				lpDoc->aRunProgram[FindEventItem(&eItem)]=eRunProgram;
			SetEvent(eRunProgram,iItem);
		}
		break;
	}
	SortItems(lpDoc->SortColumn,lpDoc->Sort);
}

void CPlanView::OnEventAddEvent() 
{
	AddUpdateEvent(TRUE);
}

void CPlanView::OnEventUpdateEvent() 
{
	AddUpdateEvent(FALSE);
}

void CPlanView::OnEventRemoveEvent()
{
	CListCtrl &ListCtrl=GetListCtrl();
	CPlanDoc *lpDoc=GetDocument();
	int iItem;
	CString strText;
	
	// Confirm Remove
	if(((CPlanApp *)AfxGetApp())->m_Options.Remove)
	{
		CString strText;
		strText.LoadString(IDS_AREYOUSURETOREMOVE);
		if(MessageBox(strText,NULL,MB_YESNO)!=IDYES)
			return;
	}

	if(!ListCtrl.GetSelectedCount())
	{
		strText.LoadString(IDS_SELECTITEM);
		MessageBox(strText,NULL,MB_ERRORI);
		return;
	}

	iItem=ListCtrl.GetSelectionMark();
	
	switch(lpDoc->aItems[iItem].Type())
	{
		case EVENT_MESSAGE:
			lpDoc->aMessage.RemoveAt(FindEventItem(&lpDoc->aItems[iItem]));
			break;
		case EVENT_SHUTDOWN:
			lpDoc->aShutDown.RemoveAt(FindEventItem(&lpDoc->aItems[iItem]));
			break;
		case EVENT_RUNPROGRAM:
			lpDoc->aRunProgram.RemoveAt(FindEventItem(&lpDoc->aItems[iItem]));
			break;
	}
	
	ListCtrl.DeleteItem(iItem);
	lpDoc->aItems.RemoveAt(iItem);

	lpDoc->DoFileSave();
}

void CPlanView::OnEventClearProperties()
{
	((CMainFrame *)GetParentFrame())->GetPropertiesBar().Clear();
}

void CPlanView::InsertAllEvents()
{
	CPlanDoc *lpDoc=GetDocument();
	CListCtrl &ListCtrl=GetListCtrl();
	int nItems=lpDoc->aItems.GetSize();
	int iMessage;
	int iShutDown;
	int iRunProgram;
	int i;
	
	for(i=0,iMessage=0,iShutDown=0,iRunProgram=0;i<nItems;i++)
	{
		ListCtrl.InsertItem(i,_T(""));
		switch(lpDoc->aItems[i].Type())
		{
			case EVENT_MESSAGE:
				SetEvent(lpDoc->aMessage[iMessage++],i,FALSE);
				break;
			case EVENT_SHUTDOWN:
				SetEvent(lpDoc->aShutDown[iShutDown++],i,FALSE);
				break;
			case EVENT_RUNPROGRAM:
				SetEvent(lpDoc->aRunProgram[iRunProgram++],i,FALSE);
				break;
		}
	}
}

int CPlanView::FindEventItem(const Event *pEvent,BOOL bCast/*=FALSE*/)
{
	int i;
	CPlanDoc *lpDoc=GetDocument();

	switch(pEvent->Type())
	{
		case EVENT_MESSAGE:
			int nMessage;
			for(i=0,nMessage=lpDoc->aMessage.GetSize();i<nMessage;i++)
				if(bCast)
				{
					if(*((Message *)pEvent)==lpDoc->aMessage[i])
						return i;
				}
				else
					if(*pEvent==lpDoc->aMessage[i])
						return i;
			break;
		case EVENT_SHUTDOWN:
			int nShutDown;
			for(i=0,nShutDown=lpDoc->aShutDown.GetSize();i<nShutDown;i++)
				if(bCast)
				{
					if(*((ShutDown *)pEvent)==lpDoc->aShutDown[i])
						return i;
				}
				else
					if(*pEvent==lpDoc->aShutDown[i])
						return i;
			break;
		case EVENT_RUNPROGRAM:
			int nRunProgram;
			for(i=0,nRunProgram=lpDoc->aRunProgram.GetSize();i<nRunProgram;i++)
				if(bCast)
				{
					if(*((RunProgram *)pEvent)==lpDoc->aRunProgram[i])
						return i;
				}
				else
					if(*pEvent==lpDoc->aRunProgram[i])
						return i;
			break;
		default:
			return -1;
	}
	return -1;
}

void CPlanView::SetEvent(Event &rEvent,int iItem/*=-1*/,BOOL bSave/*=TRUE*/)
{
	CListCtrl &ListCtrl=GetListCtrl();
	CPlanDoc *lpDoc=GetDocument();
	SYSTEMTIME st;
	TCHAR str[20];

	if(iItem==-1)
	{
		iItem=ListCtrl.GetItemCount();
		ListCtrl.InsertItem(iItem,_T(""));
		lpDoc->aItems.Add(rEvent);
	}
	else
	if(iItem<-1)
	{
		iItem=ListCtrl.GetSelectionMark();
		lpDoc->aItems[iItem]=rEvent;
	}
	else
		lpDoc->aItems[iItem]=rEvent;

	LoadString(AfxGetInstanceHandle(),STR_EVENT(rEvent.Type()),str,sizeof(str)/sizeof(TCHAR));
	ListCtrl.SetItem(iItem,SUB_ITEM_EVENT,LVIF_TEXT|LVIF_STATE,str,0,
		INDEXTOSTATEIMAGEMASK(rEvent.State()),LVIS_STATEIMAGEMASK,0);
	
	rEvent.Time().GetAsSystemTime(st);
	
	GetDateFormat(LOCALE_SYSTEM_DEFAULT,LOCALE_NOUSEROVERRIDE|DATE_SHORTDATE
		,&st,NULL,str,sizeof(str)/sizeof(TCHAR));
	ListCtrl.SetItem(iItem,SUB_ITEM_DATE,LVIF_TEXT,str,0,0,0,0);
	
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT,LOCALE_NOUSEROVERRIDE,
		&st,NULL,str,sizeof(str)/sizeof(TCHAR));
	ListCtrl.SetItem(iItem,SUB_ITEM_TIME,LVIF_TEXT,str,0,0,0,0);

	if(bSave)
		lpDoc->DoFileSave();
}

void CPlanView::SetColumns()
{
	CListCtrl &ListCtrl=GetListCtrl();
	CHeaderCtrl *pHeaderCtrl=ListCtrl.GetHeaderCtrl();
	CPlanDoc *lpDoc=GetDocument();
	int i,nItems=pHeaderCtrl->GetItemCount();
	int nWidth=0;
	TCHAR str[20];
	CSize sizenew;
	HDITEM hditem;
	CDC *pDC=GetDC();

	// Const
	hditem.mask=HEADER_MASK;
	hditem.fmt=HEADER_FMT;

	// Event column
	if(!nItems)
		ListCtrl.InsertColumn(SUB_ITEM_EVENT,0,0,0,SUB_ITEM_EVENT);
		
	if(lpDoc->ColumnWidth[SUB_ITEM_EVENT])
		nWidth=lpDoc->ColumnWidth[SUB_ITEM_EVENT];
	else
	{
		for(i=0;i<NUM_EVENTS;i++)
		{
			LoadString(AfxGetInstanceHandle(),STR_EVENT(i),str,sizeof(str)*sizeof(TCHAR));
			sizenew=pDC->GetTextExtent(str,lstrlen(str));
			if(sizenew.cx>nWidth)
				nWidth=sizenew.cx;
		}
		nWidth+=5;
		lpDoc->ColumnWidth[SUB_ITEM_EVENT]=(WORD)nWidth;
	}
	
	LoadString(AfxGetInstanceHandle(),STR_SUB_ITEM_EVENT,str,sizeof(str)*sizeof(TCHAR));
	hditem.pszText=str;
	hditem.cchTextMax=lstrlen(str);
	hditem.cxy=lpDoc->ColumnWidth[SUB_ITEM_EVENT];
	pHeaderCtrl->SetItem(SUB_ITEM_EVENT,&hditem);

	if(!lpDoc->ColumnOrder[SUB_ITEM_EVENT])
		lpDoc->ColumnOrder[SUB_ITEM_EVENT]=SUB_ITEM_EVENT;

	// Date and Time columns
	for(i=1;i<NUM_SUB_ITEMS;i++)
	{
		if(!nItems)
			ListCtrl.InsertColumn(SUB_ITEM(i),0,0,0,SUB_ITEM(i));
		
		if(lpDoc->ColumnWidth[i])
			hditem.cxy=lpDoc->ColumnWidth[i];
		else
		{
			SYSTEMTIME st;
			GetSystemTime(&st);
			if(i==SUB_ITEM_DATE)
				GetDateFormat(LOCALE_SYSTEM_DEFAULT,LOCALE_NOUSEROVERRIDE|
					DATE_SHORTDATE,&st,NULL,str,sizeof(str)/sizeof(TCHAR));
			else
				GetTimeFormat(LOCALE_SYSTEM_DEFAULT,LOCALE_NOUSEROVERRIDE,
					&st,NULL,str,sizeof(str)/sizeof(TCHAR));
			
			hditem.cxy=pDC->GetTextExtent(str,lstrlen(str)).cx+5;
			lpDoc->ColumnWidth[i]=(WORD)hditem.cxy;
		}
		
		LoadString(AfxGetInstanceHandle(),STR_SUB_ITEM(i),str,sizeof(str)/sizeof(TCHAR));
		hditem.pszText=str;
		hditem.cchTextMax=lstrlen(str);
				
		pHeaderCtrl->SetItem(SUB_ITEM(i),&hditem);

		if(!lpDoc->ColumnOrder[i])
			lpDoc->ColumnOrder[i]=(BYTE)i;
	}
	
	ReleaseDC(pDC);

	int array[NUM_SUB_ITEMS];
	for(i=0;i<NUM_SUB_ITEMS;i++)
		*(array+i)=*(lpDoc->ColumnOrder+i);

	pHeaderCtrl->SetOrderArray(NUM_SUB_ITEMS,array);

	if(lpDoc->Sort)
	{
		HBITMAP hBitmap;

		if(lpDoc->Sort==SORT_ASCENDING)
			hBitmap=LoadBitmap(AfxGetInstanceHandle(),(LPCTSTR)IDB_SORTASCENDING);
		else //lpDoc->Sort==SORT_ASCENDING
			hBitmap=LoadBitmap(AfxGetInstanceHandle(),(LPCTSTR)IDB_SORTDESCENDING);

		hditem.mask=HDI_BITMAP|HDI_FORMAT;
		hditem.fmt=HEADER_FMT|HDF_BITMAP|HDF_BITMAP_ON_RIGHT;
		hditem.hbm=hBitmap;

		pHeaderCtrl->SetItem(lpDoc->SortColumn,&hditem);
	}
}

void CPlanView::OnViewEnableDisable() 
{
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();
	
	if(pApp->m_Options.Timer)
		KillTimer(ID_TIMER);
	else
		SetTimer(ID_TIMER,TIMER_TIME,Timer);
	
	pApp->m_Options.Timer=!pApp->m_Options.Timer;
	
	GetDocument()->SetTitle(NULL);
}

void EXPORT CALLBACK Timer(HWND hWnd, UINT /*nMsg*/, UINT /*nIDEvent*/, DWORD /*dwCount*/)
{
	CPlanView *pView=(CPlanView *)CWnd::FromHandle(hWnd);

	int i;
	int iItems;
	CTime curtime=CTime::GetCurrentTime();
	CTimeSpan timespan;
	CPlanDoc *lpDoc=pView->GetDocument();
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();
	CPropertiesBar &PropertiesBar=((CMainFrame *)pView->GetParentFrame())->GetPropertiesBar();

	for(i=0,iItems=lpDoc->aMessage.GetSize();i<iItems;i++)
	{
		timespan=lpDoc->aMessage[i].Time()-curtime;
		if(timespan.GetTotalSeconds()>=0&&
			timespan.GetTotalSeconds()<TIMER_SEC&&
			lpDoc->aMessage[i].State()==STATEIMAGE_OFF)
		{
			// Check / Remove Event
			lpDoc->aMessage[i].State()=STATEIMAGE_ON;
			if(pApp->m_Options.Check)
				pView->SetEvent(lpDoc->aMessage[i],pView->FindEventItem(&lpDoc->aMessage[i]));
			else
				pView->GetListCtrl().DeleteItem(pView->FindEventItem(&lpDoc->aMessage[i]));
			// DoEvent
			PropertiesBar.GetMessageClass()->DoEvent(lpDoc->aMessage[i]);
			break;
		}
	}
	
	for(i=0,iItems=lpDoc->aShutDown.GetSize();i<iItems;i++)
	{
		timespan=lpDoc->aShutDown[i].Time()-curtime;
		if(timespan.GetTotalSeconds()>=0&&
			timespan.GetTotalSeconds()<TIMER_SEC&&
			lpDoc->aShutDown[i].State()==STATEIMAGE_OFF)
		{
			// Check / Remove Event
			lpDoc->aShutDown[i].State()=STATEIMAGE_ON;
			if(pApp->m_Options.Check)
				pView->SetEvent(lpDoc->aShutDown[i],pView->FindEventItem(&lpDoc->aShutDown[i]));
			else
				pView->GetListCtrl().DeleteItem(pView->FindEventItem(&lpDoc->aShutDown[i]));
			// DoEvent
			PropertiesBar.GetShutDownClass()->DoEvent(lpDoc->aShutDown[i]);
			break;
		}
	}
	
	for(i=0,iItems=lpDoc->aRunProgram.GetSize();i<iItems;i++)
	{
		timespan=lpDoc->aRunProgram[i].Time()-curtime;
		if(timespan.GetTotalSeconds()>=0&&
			timespan.GetTotalSeconds()<TIMER_SEC&&
			lpDoc->aRunProgram[i].State()==STATEIMAGE_OFF)
		{
			// Check / Remove Event
			lpDoc->aRunProgram[i].State()=STATEIMAGE_ON;
			if(pApp->m_Options.Check)
				pView->SetEvent(lpDoc->aRunProgram[i],pView->FindEventItem(&lpDoc->aRunProgram[i]));
			else
				pView->GetListCtrl().DeleteItem(pView->FindEventItem(&lpDoc->aRunProgram[i]));
			// DoEvent
			PropertiesBar.GetRunProgramClass()->DoEvent(lpDoc->aRunProgram[i]);
			break;
		}
	}
}

void CPlanView::OnDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{	
	SelectEventItem();
	
	*pResult = 0;
}

void CPlanView::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;

	*pResult = 0;
	
	if(pLVKeyDown->wVKey==VK_RETURN)
		SelectEventItem();
	else
	if(pLVKeyDown->wVKey==VK_SPACE)
		SetCheckItem();
}

void CPlanView::SelectEventItem()
{
	CListCtrl &ListCtrl=GetListCtrl();
	CPlanDoc *lpDoc=GetDocument();
	int iItem;
	int eItem;

	if(!ListCtrl.GetItemCount())
		return;

	iItem=ListCtrl.GetSelectionMark();
	
	eItem=FindEventItem(&lpDoc->aItems[iItem]);

	if(eItem!=-1)
		switch(lpDoc->aItems[iItem].Type())
		{
			case EVENT_MESSAGE:
				((CMainFrame *)GetParentFrame())->GetPropertiesBar().SetEvent(&lpDoc->aMessage[eItem]);
				break;
			case EVENT_SHUTDOWN:
				((CMainFrame *)GetParentFrame())->GetPropertiesBar().SetEvent(&lpDoc->aShutDown[eItem]);
				break;
			case EVENT_RUNPROGRAM:
				((CMainFrame *)GetParentFrame())->GetPropertiesBar().SetEvent(&lpDoc->aRunProgram[eItem]);
				break;
		}
}

void CPlanView::OnToolsOptions() 
{
	CPropertySheet OptionsSheet(IDS_OPTIONS,this);
	CGeneralPage pageGeneral;
	CConfirmationsPage pageConfirmations;
	
	OptionsSheet.m_psh.dwFlags &= ~(PSH_HASHELP);
	OptionsSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	OptionsSheet.AddPage(&pageGeneral);
	OptionsSheet.AddPage(&pageConfirmations);
	
	OptionsSheet.DoModal();
}

void CPlanView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags = 0;
	CListCtrl &ListCtrl=GetListCtrl();
	int nHitItem = ListCtrl.HitTest(point, &uFlags);

	//BOOL bHit = FALSE;
	if (uFlags & LVHT_ONITEMSTATEICON || (uFlags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED)))
	{
		RECT rect;
		
		ListCtrl.GetHeaderCtrl()->GetItemRect(SUB_ITEM_EVENT,&rect);
		
		if(point.x > rect.left && point.x < rect.right-STATEIMAGE_WIDTH)
			SetCheckItem(nHitItem);
	}
	
	CListView::OnLButtonDown(nFlags, point);
}

void CPlanView::SetCheckItem(int iItem,int nCheck)
{
	CPlanDoc *lpDoc=GetDocument();
	CListCtrl &ListCtrl=GetListCtrl();
	Event eEvent;
	int eItem;
		
	if(iItem==-1)
		if((iItem=ListCtrl.GetSelectionMark())==-1)
			return;
	
	eEvent=lpDoc->aItems[iItem];
	eItem=FindEventItem(&eEvent);
		
	eEvent.State()=nCheck!=-1?(BYTE)nCheck:
		eEvent.State()==STATEIMAGE_ON?
		STATEIMAGE_OFF:STATEIMAGE_ON;
	
	switch(eEvent.Type())
	{
		case EVENT_MESSAGE:
			lpDoc->aMessage[eItem].State()=
				lpDoc->aMessage[eItem].State()==STATEIMAGE_ON?
				STATEIMAGE_OFF:STATEIMAGE_ON;
			break;
		case EVENT_SHUTDOWN:
			lpDoc->aShutDown[eItem].State()=
				lpDoc->aShutDown[eItem].State()==STATEIMAGE_ON?
				STATEIMAGE_OFF:STATEIMAGE_ON;
			break;
		case EVENT_RUNPROGRAM:
			lpDoc->aRunProgram[eItem].State()=
				lpDoc->aRunProgram[eItem].State()==STATEIMAGE_ON?
				STATEIMAGE_OFF:STATEIMAGE_ON;
			break;
	}
	SetEvent(eEvent,iItem);
}

BOOL CPlanView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR *pNMHDR = (NMHDR *)lParam;
	
	if(pNMHDR->code==HDN_ITEMCLICKA||
		pNMHDR->code==HDN_ITEMCLICKW)
	{
		HD_NOTIFY *phdn=(HD_NOTIFY *)pNMHDR;
		CPlanDoc *lpDoc=GetDocument();

		if(lpDoc->SortColumn==phdn->iItem)
		{
			if(++lpDoc->Sort>SORT_DESCENDING)
				lpDoc->Sort=NOSORT;
		}
		else
			lpDoc->Sort=SORT_ASCENDING;
			
		lpDoc->SortColumn=(BYTE)phdn->iItem;
		
		SetColumns();
		SortItems(phdn->iItem,lpDoc->Sort);

		return FALSE;
	}
	else
		return CListView::OnNotify(wParam, lParam, pResult);
	
	return FALSE;
}

void CPlanView::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	ASSERT(!m_bDragging);

	m_bDragging=TRUE;
	m_iItemDrag=pNMListView->iItem;

	if(GetDocument()->Sort)
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSORNODRAG));
	else
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSORDRAG));

	SetCapture();

	*pResult = 0;
}

void CPlanView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		int iItemDrop=GetListCtrl().HitTest(point);
		if(iItemDrop!=-1)
			m_iItemDrop=iItemDrop;
	}
	
	CListView::OnMouseMove(nFlags, point);
}

void CPlanView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		m_bDragging=FALSE;

		SetCursor(LoadCursor(NULL,(LPCTSTR)IDC_ARROW));
		ReleaseCapture();

		if(!GetDocument()->Sort)
		{
			MoveItem(m_iItemDrag,m_iItemDrop);

			GetListCtrl().DeleteAllItems();
			InsertAllEvents();
		}
	}
		
	CListView::OnLButtonUp(nFlags, point);
}

void CPlanView::MoveItem(int iItemFrom,int iItemTo,BOOL bSave/*=TRUE*/)
{
	int i;
	CPlanDoc *lpDoc;
	Event eEventFrom;
		
	if(iItemFrom==iItemTo)
		return;

	if(iItemFrom==-1||iItemTo==-1)
		return;

	lpDoc=GetDocument();
	eEventFrom=lpDoc->aItems[iItemFrom];

	switch(eEventFrom.Type())
	{
		case EVENT_MESSAGE:
		{
			int iMessageFrom;
			int iMessageTo;
			Message eMessageFrom;
			
			iMessageFrom=FindEventItem(&eEventFrom);
			ASSERT(iMessageFrom!=-1);

			i=iItemTo;
			if(iItemTo<iItemFrom)
				while(lpDoc->aItems[i].Type()!=EVENT_MESSAGE
					&&i<iItemFrom) i++;
			else // iItemTo>iItemFrom
				while(lpDoc->aItems[i].Type()!=EVENT_MESSAGE
					&&i>iItemFrom) i--;
				
			iMessageTo=FindEventItem(&lpDoc->aItems[i]);

			if(iMessageTo==iMessageFrom)
				break;

			eMessageFrom=lpDoc->aMessage[iMessageFrom];
			lpDoc->aMessage.RemoveAt(iMessageFrom);
			lpDoc->aMessage.InsertAt(iMessageTo,eMessageFrom);
		}
		break;
		
		case EVENT_SHUTDOWN:
		{
			int iShutDownFrom;
			int iShutDownTo;
			ShutDown eShutDownFrom;
			
			iShutDownFrom=FindEventItem(&eEventFrom);
			ASSERT(iShutDownFrom!=-1);

			i=iItemTo;
			if(iItemTo<iItemFrom)
				while(lpDoc->aItems[i].Type()!=EVENT_SHUTDOWN
					&&i<iItemFrom) i++;
			else // iItemTo>iItemFrom
				while(lpDoc->aItems[i].Type()!=EVENT_SHUTDOWN
					&&i>iItemFrom) i--;
				
			iShutDownTo=FindEventItem(&lpDoc->aItems[i]);

			if(iShutDownTo==iShutDownFrom)
				break;

			eShutDownFrom=lpDoc->aShutDown[iShutDownFrom];
			lpDoc->aShutDown.RemoveAt(iShutDownFrom);
			lpDoc->aShutDown.InsertAt(iShutDownTo,eShutDownFrom);
		}
		break;

		case EVENT_RUNPROGRAM:
		{
			int iRunProgramFrom;
			int iRunProgramTo;
			RunProgram eRunProgramFrom;
			
			iRunProgramFrom=FindEventItem(&eEventFrom);
			ASSERT(iRunProgramFrom!=-1);

			i=iItemTo;
			if(iItemTo<iItemFrom)
				while(lpDoc->aItems[i].Type()!=EVENT_RUNPROGRAM
					&&i<iItemFrom) i++;
			else // iItemTo>iItemFrom
				while(lpDoc->aItems[i].Type()!=EVENT_RUNPROGRAM
					&&i>iItemFrom) i--;
				
			iRunProgramTo=FindEventItem(&lpDoc->aItems[i]);

			if(iRunProgramTo==iRunProgramFrom)
				break;

			eRunProgramFrom=lpDoc->aRunProgram[iRunProgramFrom];
			lpDoc->aRunProgram.RemoveAt(iRunProgramFrom);
			lpDoc->aRunProgram.InsertAt(iRunProgramTo,eRunProgramFrom);
		}
		break;
	}

	lpDoc->aItems.RemoveAt(iItemFrom);
	lpDoc->aItems.InsertAt(iItemTo,eEventFrom);
	
	if(bSave)
		lpDoc->DoFileSave();
}

void CPlanView::SortItems(int iSubItem,BYTE Sort)
{
	int i,j;
	CListCtrl &ListCtrl=GetListCtrl();
	int nItems=ListCtrl.GetItemCount();
	CPlanDoc *lpDoc=GetDocument();
	Event eEvent;
	Message eMessage;
	ShutDown eShutDown;
	RunProgram eRunProgram;
	int iMessage,iShutDown,iRunProgram;

	if(Sort==NOSORT)
		return;

	for(i=0;i<nItems;i++)
		for(j=0;j<nItems-1;j++)
			if(Sort==SORT_ASCENDING)
			{
				if(ListCtrl.GetItemText(j,iSubItem)>
					ListCtrl.GetItemText(j+1,iSubItem))
				{
					eEvent=lpDoc->aItems[j];
					SetEvent(lpDoc->aItems[j+1],j,FALSE);
					SetEvent(eEvent,j+1,FALSE);
				}
			}
			else //Sort==SORT_DESCENDING;
			{
				if(ListCtrl.GetItemText(j,iSubItem)<
					ListCtrl.GetItemText(j+1,iSubItem))
				{
					eEvent=lpDoc->aItems[j];
					SetEvent(lpDoc->aItems[j+1],j,FALSE);
					SetEvent(eEvent,j+1,FALSE);
				}
			}

	for(i=0,iMessage=0,iRunProgram=0,iShutDown=0;i<nItems;i++)
		switch(lpDoc->aItems[i])
		{
			case EVENT_MESSAGE:
				if(lpDoc->aItems[i]!=lpDoc->aMessage[iMessage])
				{
					j=FindEventItem(&lpDoc->aItems[i]);
					
					eMessage=lpDoc->aMessage[j];
					lpDoc->aMessage.RemoveAt(j);
					lpDoc->aMessage.InsertAt(iMessage,eMessage);
				}
				iMessage++;
				break;
			case EVENT_SHUTDOWN:
				if(lpDoc->aItems[i]!=lpDoc->aShutDown[iShutDown])
				{
					j=FindEventItem(&lpDoc->aItems[i]);
					
					eShutDown=lpDoc->aShutDown[j];
					lpDoc->aShutDown.RemoveAt(j);
					lpDoc->aShutDown.InsertAt(iShutDown,eShutDown);
				}
				iShutDown++;
				break;
			case EVENT_RUNPROGRAM:
				if(lpDoc->aItems[i]!=lpDoc->aRunProgram[iRunProgram])
				{
					j=FindEventItem(&lpDoc->aItems[i]);
					
					eRunProgram=lpDoc->aRunProgram[j];
					lpDoc->aRunProgram.RemoveAt(j);
					lpDoc->aRunProgram.InsertAt(iRunProgram,eRunProgram);
				}
				iRunProgram++;
				break;
		}

	lpDoc->DoFileSave();
}

void CPlanView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) 
{
	CHeaderCtrl *pHeaderCtrl=GetListCtrl().GetHeaderCtrl();
	HDITEM hditem;
	CPlanDoc *lpDoc=GetDocument();
	int i;
	
	if(!pHeaderCtrl)
		return;

	if(!pHeaderCtrl->GetItemCount())
		return;

	hditem.mask=HDI_ORDER|HDI_WIDTH;
	
	for(i=0;i<NUM_SUB_ITEMS;i++)
	{
		pHeaderCtrl->GetItem(SUB_ITEM(i),&hditem);
		*(lpDoc->ColumnWidth+i)=(WORD)hditem.cxy;
		*(lpDoc->ColumnOrder+i)=(BYTE)hditem.iOrder;
	}
}

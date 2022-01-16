// PlanView.cpp : implementation of the CPlanView class
//

#include "stdafx.h"
#include "Plan.h"

#include "MainFrame.h"

#include "PlanDoc.h"
#include "PlanView.h"

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
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanView construction/destruction
CPlanView::CPlanView()
{
}

CPlanView::~CPlanView()
{
}

BOOL CPlanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPlanView drawing

void CPlanView::OnDraw(CDC* /*pDC*/)
{
	CPlanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CPlanView::OnInitialUpdate()
{
	CListCtrl &ListCtrl=GetListCtrl();
	
	ListCtrl.ModifyStyle(LVS_TYPEMASK | LVS_ALIGNMASK,
		LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |
		LVS_EX_HEADERDRAGDROP | LVS_EX_TRACKSELECT | LVS_EX_FLATSB);
	
	// Columns
	SetColumns();
	
	// Items
	SetItems();
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

void CPlanView::SetColumns()
{
	static bool bCreated;
	int stringColumn;
	CString str;
	CListCtrl &ListCtrl=GetListCtrl();
	if(bCreated)
		return;
	for(stringColumn=stringFirstColumn;stringColumn<=stringLastColumn;stringColumn++)
	{
		str.LoadString(stringColumn);
		ListCtrl.InsertColumn(stringColumn,str,LVCFMT_CENTER,100,stringColumn);
	}
	bCreated=true;
}

void CPlanView::SetItems()
{
	CPlanDoc *pDoc=GetDocument();
	int nItems=pDoc->GetNumEvents();
	int iItem;
	
	GetListCtrl().DeleteAllItems();

	for(iItem=0;iItem<nItems;iItem++)
		SetItem(CPlanDoc::Message::Types::AddEvent,iItem,&pDoc->GetaFrameData()[iItem]);
}

void CPlanView::SetItem(CPlanDoc::Message::Types Action,int iItem,const CMainFrame::Data *pFrameData)
{
	if(Action==CPlanDoc::Message::Types::AddEvent||
		Action==CPlanDoc::Message::Types::UpdateEvent)
	{
		EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();
		CPlanDoc *pDoc=GetDocument();
		CListCtrl &ListCtrl = GetListCtrl();
		int DateTimeSize;
		CString str,s;
		SYSTEMTIME st;
		
		bool bAdd=Action==CPlanDoc::Message::Types::AddEvent;

		if(bAdd)
			if(iItem==-1)
				iItem=ListCtrl.GetItemCount();
		if(!pFrameData)
			return;
		
		if(bAdd)
			ListCtrl.InsertItem(iItem,"",0);
		
		rEventLib.Find(pDoc->GetaEventID()[iItem],TRUE);
		// Event Name
		ListCtrl.SetItem(iItem,columnEvent,LVIF_TEXT,rEventLib.GetEventName(),0,0,0,0);
		// Date & Time
		
		pFrameData=&pDoc->GetaFrameData()[iItem];
		pFrameData->m_Time.GetAsSystemTime(st);

		if(pFrameData->m_Schedule.Once)
		{
			str.LoadString(IDS_ONCE);
			ListCtrl.SetItem(iItem,columnSchedule,LVIF_TEXT,str,0,0,0,0);

			// Date
			DateTimeSize=::GetDateFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
				&st,NULL,NULL,NULL);

			::GetDateFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
				&st,NULL,str.GetBuffer(DateTimeSize),DateTimeSize);
			str.ReleaseBuffer();

			ListCtrl.SetItem(iItem,columnDate,LVIF_TEXT,str,0,0,0,0);
		}
		else
		{
			unsigned j;
			int *ip=(int *)&pFrameData->m_Schedule;
			str.Empty();
			for(j=0;j<=IDS_LASTDAY-IDS_FIRSTDAY;j++)
				if(*ip&2<<j)
				{
					s.LoadString(IDS_FIRSTDAY+j);
					str+=s;
					str+=_T(" ");
				}

			ListCtrl.SetItem(iItem,columnSchedule,LVIF_TEXT,str,0,0,0,0);
			
			// No Date
			ListCtrl.SetItem(iItem,columnDate,LVIF_TEXT,"",0,0,0,0);
		}

		// Time
		DateTimeSize=::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
			&st,NULL,NULL,NULL);

		::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
			&st,NULL,str.GetBuffer(DateTimeSize),DateTimeSize);
		str.ReleaseBuffer();

		ListCtrl.SetItem(iItem,columnTime,LVIF_TEXT,str,0,0,0,0);
		
	}
}

int CPlanView::GetSelectedEvent()
{
	return GetListCtrl().GetSelectionMark();
}

void CPlanView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/) 
{
	static void (CPlanView::*ActionTable[])(CPlanDoc::Message *)=
		{&CPlanView::AddEvent,
		&CPlanView::UpdateEvent,
		&CPlanView::DeleteEvent};
	CPlanDoc::Message *pMessage=(CPlanDoc::Message *)lHint;
	if(pMessage!=NULL)
		(this->*ActionTable[pMessage->Type])(pMessage);
}

void CPlanView::AddEvent(CPlanDoc::Message *pMessage)
{
	SetItem(CPlanDoc::Message::Types::AddEvent,GetDocument()->GetNumEvents(),
		&GetDocument()->GetaFrameData()[pMessage->iEvent]);
}

void CPlanView::UpdateEvent(CPlanDoc::Message *pMessage)
{
	SetItem(CPlanDoc::Message::Types::UpdateEvent,pMessage->iEvent,
		&GetDocument()->GetaFrameData()[pMessage->iEvent]);
}

void CPlanView::DeleteEvent(CPlanDoc::Message *pMessage)
{
	GetListCtrl().DeleteItem(pMessage->iEvent);
}

void CPlanView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int iEvent=GetListCtrl().GetSelectionMark();
	if(iEvent!=-1)
		GetDocument()->SelectEvent(iEvent);	

	CListView::OnLButtonDblClk(nFlags, point);
}

void CPlanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_RETURN)
	{	
		int iEvent=GetListCtrl().GetSelectionMark();
		if(iEvent!=-1)
			GetDocument()->SelectEvent(iEvent);	
	}
	
	CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}

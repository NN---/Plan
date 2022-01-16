// PlanView.cpp : implementation of the CPlanView class
//

#include "stdafx.h"
#include "Plan.h"

#include "PlanDoc.h"
#include "PlanView.h"

#include "MainFrame.h"

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
	ON_COMMAND(ID_EVENT_ADD, OnEventAdd)
	ON_COMMAND(ID_EVENT_DELETE, OnEventDelete)
	ON_COMMAND(ID_EVENT_UPDATE, OnEventUpdate)
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
	int i,j;
	CString str;
	CListCtrl &ListCtrl=GetListCtrl();
	if(bCreated)
		return;
	for(i=IDS_FIRSTCOLUMN,j=IDS_LASTCOLUMN;i<=j;i++)
	{
		str.LoadString(i);
		ListCtrl.InsertColumn(i,str,LVCFMT_CENTER,100,i);
	}
	bCreated=true;
}

void CPlanView::SetItems()
{
	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();
	CPlanDoc *pDoc=GetDocument();
	CListCtrl &ListCtrl = GetListCtrl();
	int nItems=pDoc->GetaID().GetSize();
	int i;
	Schedule *pSchedule;
	CString str,s;
	
	ListCtrl.DeleteAllItems();

	for(i=0;i<nItems;i++)
	{
		ListCtrl.InsertItem(i,"",0);
		rEventLib.Find(pDoc->GetaID()[i],TRUE);
		// Event Name
		ListCtrl.SetItem(i,EVENT_COLUMN,LVIF_TEXT,rEventLib.GetEventName(),0,0,0,0);
		// Date
		pSchedule=&pDoc->GetaSchedule()[i];
		if(pSchedule->Once)
		{
			str.LoadString(IDS_ONCE);
			ListCtrl.SetItem(i,SCHEDULE_COLUMN,LVIF_TEXT,str,0,0,0,0);
		}
		else
		{
			int j;
			int *ip=(int *)pSchedule;
			str.Empty();
			for(j=0;j<=IDS_LASTDAY-IDS_FIRSTDAY;j++)
				if(*ip&2<<j)
				{
					s.LoadString(IDS_FIRSTDAY+j);
					str+=s;
					str+=_T(" ");
				}

			ListCtrl.SetItem(i,SCHEDULE_COLUMN,LVIF_TEXT,str,0,0,0,0);
			
			// No Date
			ListCtrl.SetItem(i,DATE_COLUMN,LVIF_TEXT,"",0,0,0,0);
		}
		// Time
		ListCtrl.SetItem(i,TIME_COLUMN,LVIF_TEXT,"",0,0,0,0);
	}
}

void CPlanView::OnEventAdd()
{
	// Show added Event
	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();
	if(rEventLib.IsEmpty())
		return;
	CListCtrl &ListCtrl = GetListCtrl();
	int iItem=ListCtrl.GetItemCount();
	TCHAR szTime[50];
	//((CMainFrame *)GetParentFrame())->GetPropertiesBar().GetTimeString(szTime);
	ListCtrl.InsertItem(iItem,"",0);
	ListCtrl.SetItem(iItem,0,LVIF_TEXT,rEventLib.GetEventName(),0,0,0,0);
	ListCtrl.SetItem(iItem,1,LVIF_TEXT,"",0,0,0,0);
	// Save
	
	
/*	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->m_EventLib;
	CPropertiesBar &PropertiesBar=((CMainFrame *)AfxGetMainWnd())->GetPropertiesBar();
	aID.Add(rEventLib.GetEventID());
	aTime.Add(PropertiesBar.GetTime());
	//aOnce.Add(PropertiesBar.IsOnce());
	int size=rEventLib.DataSize();
	aDataSize.Add(size);
	LPBYTE p;
	p=new BYTE[size];
	rEventLib.GetData(p);
	aData.Add(p);
*/}

void CPlanView::OnEventDelete()
{

}

void CPlanView::OnEventUpdate()
{

}


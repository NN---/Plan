// PlanView.cpp : implementation of the CPlanView class
//

#include "stdafx.h"
#include "Plan.h"

#include "PlanDoc.h"
#include "PlanView.h"

#include "MainFrame.h"
#include "PropertiesBar.h"

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
	
	CListView::OnInitialUpdate();

	ListCtrl.DeleteAllItems();

	CString str;
	str.LoadString(IDS_EVENT);
	ListCtrl.InsertColumn(0,str,LVCFMT_CENTER,100,0);
	str.LoadString(IDS_DATETIME);
	ListCtrl.InsertColumn(1,str,LVCFMT_CENTER,130,1);
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

void CPlanView::OnEventAdd()
{
	// Show added Event
	CPropertiesBar &PropertiesBar = ((CMainFrame *)GetParentFrame())->GetPropertiesBar();
	CPlanApp *pApp = (CPlanApp *)AfxGetApp();
	if(pApp->m_EventLib.IsEmpty())
		return;
	CListCtrl &ListCtrl = GetListCtrl();
	int iItem=ListCtrl.GetItemCount();
	TCHAR szTime[50];
	PropertiesBar.GetTimeString(szTime);
	ListCtrl.InsertItem(iItem,"",0);
	ListCtrl.SetItem(iItem,0,LVIF_TEXT,pApp->m_EventLib.GetEventName(),0,0,0,0);
	ListCtrl.SetItem(iItem,1,LVIF_TEXT,szTime,0,0,0,0);
	// Save
	GetDocument()->Add();
}

void CPlanView::OnEventDelete()
{

}

void CPlanView::OnEventUpdate()
{

}


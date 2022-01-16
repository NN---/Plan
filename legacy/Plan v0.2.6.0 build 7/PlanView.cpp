// PlanView.cpp : implementation of the CPlanView class
//

#include "stdafx.h"
#include "Plan.h"

#include "PlanDoc.h"
#include "PlanView.h"

#include "RegKey.h"

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
	ON_COMMAND(ID_EVENT_CLEAR, OnEventClear)
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

	CString c="Column";
CString istr="Item ";
CString s;
int i,j;
char ch;
char ich,jch;
for(i=0,ch='0';i<4;i++,ch=(char)(i+'0'))
{
	s=c+ch;
	ListCtrl.InsertColumn(i,s,LVCFMT_LEFT,70,i);
}

for(i=0,ich='0';i<4;i++,ich=(char)(i+'0'))
{
	ListCtrl.InsertItem(i,"");
 
	for(j=0,jch='0';j<4;j++,jch=(char)(j+'0'))
 {
	s=istr+ich;
	s+='-';
	s+=jch;
	ListCtrl.SetItem(i,j,LVIF_TEXT,s,0,0,0,0);

	}
}
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

}

void CPlanView::OnEventDelete() 
{

	
}

void CPlanView::OnEventUpdate() 
{

	
}

void CPlanView::OnEventClear() 
{

	
}

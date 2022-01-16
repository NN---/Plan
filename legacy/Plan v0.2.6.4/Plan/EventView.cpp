#include "StdAfx.h"
#include "EventView.h"
#include "PlanDocument.h"

// Virtual Functions
void CEventView::OnUpdate(baseView* /*pSender*/, void * /*pData*/)
{
}
/*
void CEventView::OnDestroyView()
{
}
*/

void CEventView::OnInitialUpdate()
{
	ModifyStyle(LVS_TYPEMASK | LVS_ALIGNMASK,
		LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	ListView_SetExtendedListViewStyle(m_hWnd, LVS_EX_FULLROWSELECT | 
		LVS_EX_HEADERDRAGDROP | LVS_EX_TRACKSELECT | LVS_EX_GRIDLINES);
	//LVS_EX_FLATSB

	//
	InsertColumn(0,"A",LVCFMT_LEFT,80,0);
	InsertColumn(1,"B",LVCFMT_LEFT,80,1);
	InsertColumn(2,"C",LVCFMT_LEFT,80,2);
	InsertColumn(3,"D",LVCFMT_LEFT,80,3);

	InsertItem(0,"Item1 - A");
	SetItem(0,1,LVIF_TEXT,"Item1 - B",0,0,0,0);
	SetItem(0,2,LVIF_TEXT,"Item1 - C",0,0,0,0);
	SetItem(0,3,LVIF_TEXT,"Item1 - D",0,0,0,0);

	InsertItem(1,"Item2 - A");
	SetItem(1,1,LVIF_TEXT,"Item2 - B",0,0,0,0);
	SetItem(1,2,LVIF_TEXT,"Item2 - C",0,0,0,0);
	SetItem(1,3,LVIF_TEXT,"Item2 - D",0,0,0,0);

	InsertItem(2,"Item3 - A");
	SetItem(2,1,LVIF_TEXT,"Item3 - B",0,0,0,0);
	SetItem(2,2,LVIF_TEXT,"Item3 - C",0,0,0,0);
	SetItem(2,3,LVIF_TEXT,"Item3 - D",0,0,0,0);
	//
}

BOOL CEventView::PreTranslateMessage(MSG* /*pMsg*/)
{
	return FALSE;
}

// Message
// WM_LBUTTONDBLCLK
LRESULT CEventView::OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
/*	int iEvent=GetSelectionMark();
	if(iEvent!=-1)
		GetDocument()->SelectEvent(iEvent);	
*/	
	return 0;
}

// WM_KEYDOWN
LRESULT CEventView::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	wParam;
/*	if(wParam==VK_RETURN)
	{	
		int iEvent=GetSelectionMark();
		if(iEvent!=-1)
			m_pParent->SelectEvent(iEvent);	
	}
*/	
	return 0;
}
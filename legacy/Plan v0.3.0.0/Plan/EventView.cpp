#include "StdAfx.h"
#include "EventView.h"
#include "Column.h"

const CEventView::EventColumn CEventView::s_aColumn[]=
{
	{110,LVCFMT_LEFT,LVITEMINFO(CColumnEvent)},
	{90,LVCFMT_LEFT,LVITEMINFO(CColumnDate)},
	{90,LVCFMT_LEFT,LVITEMINFO(CColumnTime)},
	{100,LVCFMT_LEFT,LVITEMINFO(CColumnSchedule)},
	{130,LVCFMT_LEFT,LVITEMINFO(CColumnHotKey)},
	{50,LVCFMT_RIGHT,LVITEMINFO(CColumnDelay)}
};

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
	// Color
	NN::LVItemDraw drawItem;
	drawItem.m_clrText=RGB(0,0,110);
	drawItem.m_clrBk=RGB(166,202,240);
	SetSelItemDraw(drawItem);
	drawItem.m_clrText=RGB(0,0,0);
	drawItem.m_clrBk=RGB(0,200,0);
	SetSelRowDraw(drawItem);
	drawItem.m_clrText=RGB(255,255,255);
	drawItem.m_clrBk=RGB(64,144,196);
	SetItemDraw(drawItem);
	
	SetColumns();
}

BOOL CEventView::PreTranslateMessage(MSG* /*pMsg*/)
{
	return FALSE;
}

void CEventView::SetColumns()
{
	// Check that we mustn't add columns twice
	if(m_aColumn.GetCount()==COLUMN_LAST)
		return;

	// Columns
	int iCol;
	CString str;
	for(iCol=COLUMN_FIRST;iCol<=COLUMN_LAST;iCol++)
	{
		str.LoadString(IDS_EVENT+iCol);
		InsertLVColumn(iCol,str,s_aColumn[iCol].nFormat,s_aColumn[iCol].nWidth,iCol,s_aColumn[iCol].pInfo);
	}

	// Just to test
	for(int i=0;i<10;i++)
		InsertLVRow(i,false);
	
	UpdateAllItems();
}
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
	InsertLVColumn(0,"Event",LVCFMT_LEFT,100);
	InsertLVColumn(1,"Date",LVCFMT_LEFT,100);
	InsertLVColumn(2,"Time",LVCFMT_LEFT,100);
	InsertLVColumn(3,"Schedule",LVCFMT_LEFT,100);
	InsertLVColumn(4,"HotKey",LVCFMT_LEFT,130);

	InsertLVRow(0);
	InsertLVRow(1);
	InsertLVRow(2);

	SetLVColumnInfo(0,LVITEMINFO(LVItemEdit),false);
	SetLVColumnInfo(1,LVITEMINFO(LVItemDateTime),false);
	SetLVColumnInfo(2,LVITEMINFO(LVItemDateTime),false);
	SetLVColumnInfo(3,LVITEMINFO(LVItemButton),false);
	SetLVColumnInfo(4,LVITEMINFO(LVItemHotKey),false);

	int i;
	for(i=0;i<3;i++)
	{
		SetLVItemText(i,0,"Message",false);
		SetLVItemText(i,1,"01.01.2000",false);
		SetLVItemText(i,2,"12:12:12",false);
		SetLVItemText(i,3,"Once",false);
		SetLVItemText(i,4,"Ctrl+Alt+Shift+A",false);
	}

	NN::LVItem::Data *pData=NN::LVItem::CreateData();
	GetLVItemData(0,2,pData);
	pData->SetStyle((pData->GetStyle() & ~(DTS_LONGDATEFORMAT | DTS_SHORTDATEFORMAT)) | DTS_TIMEFORMAT | DTS_UPDOWN);
	SetLVColumnData(2,pData);
	delete pData;

	UpdateAllItems();
}

BOOL CEventView::PreTranslateMessage(MSG* /*pMsg*/)
{
	return FALSE;
}
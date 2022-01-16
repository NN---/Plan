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

	SetLVColumnInfo(0,LVITEMINFO(LVItemComboBox),false);
	SetLVColumnInfo(1,LVITEMINFO(LVItemDateTime),false);
	SetLVColumnInfo(2,LVITEMINFO(LVItemDateTime),false);
	SetLVColumnInfo(3,LVITEMINFO(LVItemEdit),false);
	SetLVColumnInfo(4,LVITEMINFO(LVItemHotKey),false);

	int i;
	for(i=0;i<3;i++)
		SetLVItemText(i,3,"Once",false);

	NN::LVItemDateTime::Data *pData=LVITEMINFO_DATA(LVItemDateTime);
	GetLVItemData(0,2,pData);
	pData->SetStyle((pData->GetStyle() & ~(DTS_LONGDATEFORMAT | DTS_SHORTDATEFORMAT)) | DTS_TIMEFORMAT | DTS_UPDOWN);
	SetLVColumnData(2,pData);
	delete pData;

	NN::LVItemComboBox::Data *pCmbData=LVITEMINFO_DATA(LVItemComboBox);
	GetLVItemData(0,0,pCmbData);
	pCmbData->AddItemString(_T("Message"));
	pCmbData->AddItemString(_T("ShutDown"));
	pCmbData->AddItemString(_T("RunFile"));
	pCmbData->SetItemHeight(150);
	SetLVItemData(0,0,pCmbData);
	//SetLVColumnData(0,pCmbData);
	delete pCmbData;

	UpdateAllItems();
}

BOOL CEventView::PreTranslateMessage(MSG* /*pMsg*/)
{
	return FALSE;
}
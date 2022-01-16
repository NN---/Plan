#ifndef __EVENTVIEW_H__
#define __EVENTVIEW_H__

#pragma once

#include "PlanDocument.h"
#include "LV.h"

#define COLUMN_EVENT 0
#define COLUMN_DATE 1
#define COLUMN_TIME 2
#define COLUMN_SCHEDULE 3
#define COLUMN_HOTKEY 4
#define COLUMN_DELAY 5

#define COLUMN_FIRST COLUMN_EVENT
#define COLUMN_LAST COLUMN_DELAY
 
class CEventView : public NN::LV,
					public NN::View<CPlanDocument>
{
	typedef NN::LV baseClass;
	typedef CEventView thisClass;
	typedef NN::View<CPlanDocument> baseView;
public:
	//DECLARE_WND_SUPERCLASS(NULL, baseClass::GetWndClassName())

	virtual BOOL PreTranslateMessage(MSG* /*pMsg*/);
	virtual void OnUpdate(baseView* pSender, void *pData);
	//virtual void OnDestroyView();
	virtual void OnInitialUpdate();

	BEGIN_MSG_MAP(thisClass)
		// Messages
		// Chain Message Map
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	void SetColumns();

	struct EventColumn
	{
		int nWidth;
		int nFormat;
		NN::LVItemInfo *pInfo;
	};
	
	const static EventColumn s_aColumn[];
};

#endif // __EVENTVIEW_H__
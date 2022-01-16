#pragma once

#include "DocumentView.h"
#include "PlanDocument.h"
#include "LV.h"

class CEventView : public NN::LV,
					public NN::View<CPlanDocument>
{
	typedef NN::LV baseClass;
	typedef CEventView thisClass;
	typedef NN::View<CPlanDocument> baseView;
public:
	DECLARE_WND_SUPERCLASS(NULL, baseClass::GetWndClassName())

	virtual BOOL PreTranslateMessage(MSG* /*pMsg*/);
	virtual void OnUpdate(baseView* pSender, void *pData);
	//virtual void OnDestroyView();
	virtual void OnInitialUpdate();

	BEGIN_MSG_MAP(thisClass)
		// Messages
		// Chain Message Map
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};

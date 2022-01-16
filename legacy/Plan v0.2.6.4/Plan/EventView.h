#pragma once

#include "DocumentView.h"
#include "PlanDocument.h"

class CEventView : public CWindowImpl<CEventView, CListViewCtrl>,
					public NN::View<CPlanDocument>
{
	typedef CWindowImpl<CEventView,CListViewCtrl> baseClass;
	typedef CEventView thisClass;
	typedef NN::View<CPlanDocument> baseView;
public:
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	virtual BOOL PreTranslateMessage(MSG* /*pMsg*/);
	virtual void OnUpdate(baseView* pSender, void *pData);
	//virtual void OnDestroyView();
	virtual void OnInitialUpdate();

	BEGIN_MSG_MAP(thisClass)
			// Messages
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	LRESULT OnLButtonDblClk(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled);
	LRESULT OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled);
};

#ifndef __PROPERTYPAGEADD_H__
#define __PROPERTYPAGEADD_H__

#pragma once

//#include "MainFrameBase.h"
#include "PlanDoc.h"

template <class T, int ID>
class CPropertyPageAdd : public CPropertyPage<ID>,
							public CWinDataExchange<T>
{
	typedef CPropertyPageAdd<T, ID> thisClass;
	typedef CPropertyPage<ID> baseClass;
public:
	void SetOptions(CPlanDoc::Options *pOptions)
	{
		ATLASSERT(pOptions);
		m_pOptions=pOptions;
	}
	CPlanDoc::Options *m_pOptions;
	
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Messages
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		// Chain Message Map
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	virtual BEGIN_DDX_MAP(thisClass)
	END_DDX_MAP()

	// Messages
	// WM_INITDIALOG
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		DoDataExchange(DDX_LOAD);
		return FALSE;
	}
};

#endif // __PROPERTYPAGEADD_H__
#ifndef __OPTIONSSHEET_H__
#define __OPTIONSSHEET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrameBase.h"
#include "GeneralPage.h"

class ATL_NO_VTABLE COptionsSheet : public CPropertySheet
{
	typedef COptionsSheet thisClass;
	typedef CPropertySheet baseClass;
public:
	BEGIN_MSG_MAP(thisClass)
		COMMAND_ID_HANDLER(IDOK, OnOK)
	END_MSG_MAP()

	CGeneralPage pageGeneral;

	COptionsSheet() : baseClass(IDS_OPTIONS) {}
		
	int DoModal(CMainFrameBase::Options *pOptions)
	{
		ATLASSERT(pOptions);
		pageGeneral.SetOptions(pOptions);

		m_psh.dwFlags &= ~(PSH_HASHELP);
		m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
		AddPage(pageGeneral);

		return baseClass::DoModal();
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		pageGeneral.DoDataExchange(DDX_SAVE);
		DestroyWindow();
		return 0;
	}
};

#endif // __OPTIONSSHEET_H__
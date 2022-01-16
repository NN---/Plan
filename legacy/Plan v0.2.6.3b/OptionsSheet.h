#ifndef __OPTIONSSHEET_H__
#define __OPTIONSSHEET_H__

#pragma once

//#include "MainFrameBase.h"
#include "PlanDoc.h"
#include "GeneralPage.h"
#include "ConfirmationsPage.h"

class COptionsSheet : public CPropertySheet
{
	typedef COptionsSheet thisClass;
	typedef CPropertySheet baseClass;

	CGeneralPage pageGeneral;
	CConfirmationsPage pageConfirmations;
public:
	BEGIN_MSG_MAP(thisClass)
		// Commands
		// IDOK
		COMMAND_ID_HANDLER(IDOK, OnOK)
	END_MSG_MAP()

	COptionsSheet() : baseClass(IDS_OPTIONS) {}

	int DoModal(CPlanDoc::Options *pOptions, HWND hWndParent = ::GetActiveWindow())
	{
		ATLASSERT(pOptions);

		pageGeneral.SetOptions(pOptions);
		pageConfirmations.SetOptions(pOptions);

		AddPage(pageGeneral);
		AddPage(pageConfirmations);

		m_psh.dwFlags &= ~(PSH_HASHELP);
		m_psh.dwFlags |= PSH_NOAPPLYNOW;

		return baseClass::DoModal(hWndParent);
	}

	// IDOK
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		SetActivePage(0);
		SetActivePage(1);
		pageGeneral.DoDataExchange(DDX_SAVE);
		pageConfirmations.DoDataExchange(DDX_SAVE);
		bHandled=FALSE;
		return 0;
	}
};

#endif // __OPTIONSSHEET_H__
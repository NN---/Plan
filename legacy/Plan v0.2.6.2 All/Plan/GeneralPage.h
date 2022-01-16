#ifndef __GENERALPAGE_H__
#define __GENERALPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertyPageAdd.h"

class ATL_NO_VTABLE CGeneralPage : public CPropertyPageAdd<CGeneralPage, IDD_GENERAL>
{
	typedef CGeneralPage thisClass;
public:
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	BEGIN_DDX_MAP(thisClass)
		int m;
		m=m_pOptions->bLoadOnStartup;
		DDX_CHECK(IDC_LOADONSTARTUP,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bLoadOnStartup=m;
		m=m_pOptions->bShowTrayIcon;
		DDX_CHECK(IDC_SHOWTRAYICON,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bShowTrayIcon=m;
		m=m_pOptions->bMinimizeToTray;
		DDX_CHECK(IDC_MINIMIZETOTRAY,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bMinimizeToTray=m;
	END_DDX_MAP()
	
	// Messages
	// WM_INITDIALOG
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		DoDataExchange(DDX_LOAD);
		return 0;
	}
};


#endif __GENERALPAGE_H__
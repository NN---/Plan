#ifndef __GENERALPAGE_H__
#define __GENERALPAGE_H__

#pragma once

#include "PropertyPageAdd.h"

class CGeneralPage : public CPropertyPageAdd<CGeneralPage, IDD_GENERAL>
{
	typedef CGeneralPage thisClass;
	typedef CPropertyPageAdd<CGeneralPage, IDD_GENERAL> baseClass;
public:
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	// DoDataExchange Map
	BEGIN_DDX_MAP(thisClass)
		int m;
		// IDC_LOADONSTARTUP - bLoadOnStartup
		m=m_pOptions->bLoadOnStartup;
		DDX_CHECK(IDC_LOADONSTARTUP,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bLoadOnStartup=m;
		// IDC_SHOWTRAYICON - bShowTrayIcon
		m=m_pOptions->bShowTrayIcon;
		DDX_CHECK(IDC_SHOWTRAYICON,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bShowTrayIcon=m;
		// IDC_MINIMIZETOTRAY - bMinimizeToTray
		m=m_pOptions->bMinimizeToTray;
		DDX_CHECK(IDC_MINIMIZETOTRAY,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bMinimizeToTray=m;
		// IDC_CLOSETOTRAY - bCloseToTray
		DDX_CHECK(IDC_CLOSETOTRAY,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bCloseToTray=m;
	END_DDX_MAP()
};


#endif __GENERALPAGE_H__
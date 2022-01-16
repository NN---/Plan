#ifndef __CONFIRMATIONSPAGE_H__
#define __CONFIRMATIONSPAGE_H__

#pragma once

#include "PropertyPageAdd.h"

class CConfirmationsPage : public CPropertyPageAdd<CConfirmationsPage, IDD_CONFIRMATIONS>
{
	typedef CConfirmationsPage thisClass;
	typedef CPropertyPageAdd<CConfirmationsPage, IDD_CONFIRMATIONS> baseClass;
public:
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Messages
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	// DoDataExchange Map
	BEGIN_DDX_MAP(thisClass)
		int m;
		// IDC_CONFIRMADD - bConfirmAdd
		m=m_pOptions->bConfirmAdd;
		DDX_CHECK(IDC_CONFIRMADD,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bConfirmAdd=m;
		// IDC_CONFIRMUPDATE - bConfirmUpdate
		m=m_pOptions->bConfirmUpdate;
		DDX_CHECK(IDC_CONFIRMUPDATE,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bConfirmUpdate=m;
		// IDC_CONFIRMDELETE - bConfirmDelete
		m=m_pOptions->bConfirmDelete;
		DDX_CHECK(IDC_CONFIRMDELETE,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bConfirmDelete=m;
		// IDC_CONFIRMEMPTYPROPERTIESBAR - bConfirmEmptyPropertiesBar
		m=m_pOptions->bConfirmEmptyPropertiesBar;
		DDX_CHECK(IDC_CONFIRMEMPTYPROPERTIESBAR,m)
		if(bSaveAndValidate==DDX_SAVE)
			m_pOptions->bConfirmEmptyPropertiesBar=m;
	END_DDX_MAP()
};

#endif // __CONFIRMATIONSPAGE_H__
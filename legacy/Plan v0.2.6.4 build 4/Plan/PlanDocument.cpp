#include "StdAfx.h"
#include "PlanDocument.h"

// Static members
const CPlanDocument::Options CPlanDocument::s_defaultOptions=
{
	1, // unsigned bEnabled : 1
	1, // unsigned bLoadOnStartup : 1
	1, // unsigned bShowTrayIcon : 1
	CPlanDocument::Options::mcNormal, // MinimizeClose m_Minimize
	CPlanDocument::Options::mcToTray, // MinimizeClose m_Close
	{ // Confirmations m_Confirmations
			1, // unsigned bExport : 1
			1, // unsigned bImport : 1
			1, // unsigned bAdd : 1
			1, // unsigned bUpdate : 1
			1, // unsigned bDelete : 1
			1, // unsigned bEmptyPropertiesBar : 1
	}
};

const CPlanDocument::Version CPlanDocument::s_currentVersion=
{
	0, // WORD V1
	2, // WORD V2
	6, // WORD V3
	4, // WORD V4
};
const CPlanDocument::Version CPlanDocument::s_validVersion[]=
{
	{
		0, // WORD V1
		2, // WORD V2
		6, // WORD V3
		4, // WORD V4
	},
};

const CPlanDocument::WindowPosition CPlanDocument::s_defaultWindowPosition=
{
	false, // bool bMin
	false, // bool bMax
	false, // bool bHide
	{ // WINDOWPLACEMENT wndpl
		0, // UINT length
		0, // UINT flags
		SW_SHOW, // UINT showCmd
		{ // POINT ptMinPosition
			0, // LONG x
			0, // LONG y
		},
		{ // POINT ptMaxPosition
			0, // LONG x
			0, // LONG y
		},
		{ // RECT rcNormalPosition
			140, // LONG left
			100, // LONG top
			610, // LONG right
			480, // LONG bottom
		}
	}
};

void CPlanDocument::SelectEvent(int iEvent)
{
	ATLASSERT(iEvent>=0 && iEvent<aEventData.GetSize());
	SetData(aEventData[iEvent].FData,aEventData[iEvent].pcEventID,aEventData[iEvent].pData);
}
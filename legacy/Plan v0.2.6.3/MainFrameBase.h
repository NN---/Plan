#ifndef __MAINFRAMEBASE_H__
#define __MAINFRAMEBASE_H__

#pragma once

#include "PropertiesBar.h"

class ATL_NO_VTABLE CMainFrameBase
{	
public:
	struct Data : public CPropertiesBar::Data
	{
		const bool operator==(const Data &rData) const
		{
			return *(CPropertiesBar::Data *)this==rData;
		}
	
	};

	// Members
	struct Options
	{
		unsigned bEnabled : 1;
		unsigned bLoadOnStartup : 1;
		unsigned bShowTrayIcon : 1;
		unsigned bMinimizeToTray : 1;
		unsigned bCloseToTray : 1;
		unsigned bConfirmAdd : 1;
		unsigned bConfirmUpdate : 1;
		unsigned bConfirmDelete : 1;
		unsigned bConfirmEmptyPropertiesBar : 1;
		unsigned bConfirmExit : 1;
	};
	Options m_Options;
	static const Options defOptions;
	struct WindowPos
	{
		bool bMax;
		bool bMin;
		bool bHide;
		WINDOWPLACEMENT wndpl;
	};
	WindowPos m_wndpos;
	static const WindowPos defWindowPos;
	// Version
	struct Version
	{
		WORD V1;
		WORD V2;
		WORD V3;
		WORD V4;
	};
	static const Version curVersion;
	static const Version validVersion[];
	DWORD m_numEvents;
	CSimpleValArray<PCEventID> aID;
	CSimpleValArray<LPBYTE> aData;
	CSimpleValArray<DWORD> aDataSize;
	CSimpleArray<Data> aFData;
	bool m_bCreateSuccess;

	virtual void GetData(Data &/*rData*/) = 0;
	virtual void SetData(Data &/*rData*/, PCEventID /*pcEventID*/, const LPBYTE /*pData*/) = 0;
	virtual void UpdateTitle() = 0;
	virtual void Show(int /*nShowCmd*/) = 0;

	const DWORD GetNumEvents() const
	{
		return m_numEvents;
	}

	const Data& GetFData(DWORD nIndex) const
	{
		return aFData[nIndex];
	}

	PCEventID GetEventID(DWORD nIndex)
	{
		return aID[nIndex];
	}

	void SelectEvent(int iEvent)
	{
		ATLASSERT(iEvent>=0 && (DWORD)iEvent<m_numEvents);
		SetData(aFData[iEvent],aID[iEvent],aData[iEvent]);
	}
};

// static members
const CMainFrameBase::Options CMainFrameBase::defOptions=
{
	1, // unsigned bEnabled : 1
	1, // unsigned bLoadOnStartup : 1
	1, // unsigned bShowTrayIcon : 1
	1, // unsigned bMinimizeToTray : 1
	1, // unsigned bConfirmAdd : 1;
	1, // unsigned bConfirmUpdate : 1;
	1, // unsigned bConfirmDelete : 1;
	1, // unsigned bConfirmEmptyPropertiesBar : 1;
	1, // unsigned bConfirmExit : 1;
};
const CMainFrameBase::WindowPos CMainFrameBase::defWindowPos=
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
const CMainFrameBase::Version CMainFrameBase::curVersion=
{
	0, // V1
	2, // V2
	6, // V3
	3, // V4
};
const CMainFrameBase::Version CMainFrameBase::validVersion[]=
{
	{
		0, // V1
		2, // V2
		6, // V3
		3, // V4
	},
};

#endif // __MAINFRAMEBASE_H__

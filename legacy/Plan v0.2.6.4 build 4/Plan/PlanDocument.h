#ifndef __PLANDOCUMENT_H__
#define __PLANDOCUMENT_H__

#pragma once

class CPlanDocument : public NN::Document<CPlanDocument>,
						public CMessageMap
{
	typedef CPlanDocument thisClass;
public:
	public:
	struct Data
	{
	};

	struct Options
	{
		unsigned bEnabled : 1;
		unsigned bLoadOnStartup : 1;
		unsigned bShowTrayIcon : 1;
		enum MinimizeClose { mcNormal, mcToTray, mcClose, mcAskMe };
		MinimizeClose m_Minimize;
		MinimizeClose m_Close;
		struct Confirmations
		{
			unsigned bExport : 1;
			unsigned bImport : 1;
			unsigned bAdd : 1;
			unsigned bUpdate : 1;
			unsigned bDelete : 1;
			unsigned bEmptyPropertiesBar : 1;
		};
		Confirmations m_Confirmations;
	};
	Options m_Options;
	static const Options s_defaultOptions;
	// Version
	struct Version
	{
		WORD V1;
		WORD V2;
		WORD V3;
		WORD V4;
	};
	static const Version s_currentVersion;
	static const Version s_validVersion[];
	struct WindowPosition
	{
		bool bMax;
		bool bMin;
		bool bHide;
		WINDOWPLACEMENT wndpl;
	};
	WindowPosition m_WindowPosition;
	static const WindowPosition s_defaultWindowPosition;
	struct EventData
	{
		PCEventID pcEventID;
		LPBYTE pData;
		DWORD dwDataSize;
		Data  FData;
	};
	CSimpleArray<EventData> aEventData;
	virtual void GetData(Data &rData) = 0;
	virtual void SetData(Data &rData, PCEventID pcEventID, const LPBYTE pData) = 0;
//	virtual void UpdateTitle() = 0;
//	virtual void Show(int /*nShowCmd*/) = 0;
	BEGIN_MSG_MAP(thisClass)
	END_MSG_MAP()
	void SelectEvent(int iEvent);
};

#endif // __PLANDOCUMENT_H__
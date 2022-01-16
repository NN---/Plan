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
		Data() : m_pcEventID(NULL) , uDelay(0)
		{ }

		const bool operator==(const Data &rData)
		{
			return !memcmp(this,&rData,sizeof(Data));
		}

		struct Schedule
		{
			unsigned Once : 1;
			Schedule() {Set();}
			void Set(unsigned uOnce=1)
			{ Once=uOnce; }
		};

		struct HotKey
		{
			int id;
			WORD fsModifiers;
			WORD vk;
			BOOL Register(HWND hWnd)
			{
				return RegisterHotKey(hWnd,id,fsModifiers,vk);
			}
			BOOL Unregister(HWND hWnd)
			{
				return UnregisterHotKey(hWnd,id);
			}
			HotKey() : id(0), fsModifiers(0), vk(0)
			{}
		};

		PCEventID m_pcEventID;
		NN::SystemTime m_Time;
		Schedule m_Schedule;
		HotKey m_HotKey;
		UINT uDelay;
		LPBYTE m_pData;
		DWORD m_dwDataSize;
	};

	struct Options
	{
		
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
	CAtlArray<Data> m_aData;
	BEGIN_MSG_MAP(thisClass)
	END_MSG_MAP()
	void SelectEvent(size_t nEvent);
};

#endif // __PLANDOCUMENT_H__
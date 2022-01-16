#pragma once

class CPropertiesBar : public CDialogImpl<CPropertiesBar>,
						public CWinDataExchange<CPropertiesBar>
{
	typedef CPropertiesBar thisClass;
	typedef CDialogImpl<thisClass> baseClass;
public:
	enum { IDD = IDD_PROPERTIESBAR };

	struct Data
	{
		const bool operator==(const Data &rData) const
		{
			return !memcmp(this,&rData,sizeof(Data));
		}

		struct Schedule
		{
			unsigned Sunday:1;
			unsigned Monday:1;
			unsigned Tuesday:1;
			unsigned Wednesday:1;
			unsigned Thursday:1;
			unsigned Friday:1;
			unsigned Saturday:1;
			unsigned Once:1;

			Schedule()
			{
				Set();
			}

			void Set(const unsigned uOnce=0,
				const unsigned uSunday=0,
				const unsigned uMonday=0,
				const unsigned uTuesday=0,
				const unsigned uWednesday=0,
				const unsigned uThursday=0,
				const unsigned uFriday=0,
				const unsigned uSaturday=0)
			{
					Once=uOnce;
					Sunday=uSunday;
					Monday=uMonday;
					Tuesday=uTuesday;
					Wednesday=uWednesday;
					Thursday=uThursday;
					Friday=uFriday;
					Saturday=uSaturday;
			}
			
			void SetOnce(unsigned uOnce)
			{
				Once=uOnce;
			}

			bool IsNoDays()
			{
				return !(Sunday | Monday |
					Tuesday | Wednesday |
					Thursday | Friday |
					Saturday);
			}

			WORD GetDayOfWeek()
			{
				return (WORD)(*((char *)this)>>1);
			}

		};

		struct HotKey
		{
			int id;
			WORD fsModifiers;
			WORD vk;

			HotKey() : id(0), fsModifiers(0), vk(0) { }

			BOOL Register(HWND hWnd)
			{
				return RegisterHotKey(hWnd,id,fsModifiers,vk);
			}

			BOOL Unregister(HWND hWnd)
			{
				return UnregisterHotKey(hWnd,id);
			}
		};

		Data() { Delay=0; }

		SYSTEMTIME m_Time;
		Schedule m_Schedule;
		HotKey m_HotKey;
		int Delay;
	};

	Data m_Data;
	int m_iCurSel;

	CPropertiesBar();

	// Virtual Functions
	virtual void GetData(Data &rData);
	virtual void SetData(const Data &rData,PCEventID pcEventID, const LPBYTE pData);
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		// Command
	/*	COMMAND_ID_HANDLER(IDC_ONCE, OnEventTime)
		COMMAND_ID_HANDLER(IDC_DAILY, OnEventTime)
		COMMAND_ID_HANDLER(IDC_HOTKEYB, OnEventTime)
		COMMAND_HANDLER(IDC_EVENTS, CBN_SELCHANGE, OnSelChangeEvents)
		COMMAND_HANDLER(IDC_DAYS, LBN_SELCHANGE, OnSelChangeDays)
	*/
	END_MSG_MAP()

	// DDX Map
	BEGIN_DDX_MAP(thisClass)
	END_DDX_MAP()

	// Messages
	BOOL OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled);
	BOOL OnDestroy(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled);
};

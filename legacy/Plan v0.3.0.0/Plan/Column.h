#ifndef __COLUMN_H__
#define __COLUMN_H__

class CColumnEvent : public NN::LVItemComboBoxImpl<CColumnEvent,NN::LVItemTraitsOR<CBS_DROPDOWNLIST|CBS_HASSTRINGS> >
{
	typedef NN::LVItemComboBoxImpl<CColumnEvent,NN::LVItemTraitsOR<CBS_DROPDOWNLIST|CBS_HASSTRINGS> > baseClass;
public:
	// Data
	struct Data : public baseClass::Data
	{
	public:
		Data()
		{
			size_t iDll,nDll;
			for(iDll=0,nDll=_EventDlls.GetDllCount();iDll<nDll;iDll++)
			{
				_EventDlls.SetWorkDll(iDll);
				AddItemString(_EventDlls.GetEventName());
			}
			SetItemHeight(150);
		}
		virtual ~Data(){}
	};
	// Destruction
	virtual ~CColumnEvent(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("CColumnEvent"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(CColumnEvent)
protected:
	CColumnEvent(){}
};

class CColumnDate : public NN::LVItemDateTimeImpl<CColumnDate>
{
public:
	// Destruction
	virtual ~CColumnDate(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("CColumnDate"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(CColumnDate)
protected:
	CColumnDate(){}
};

class CColumnTime : public NN::LVItemDateTimeImpl<CColumnTime,NN::LVItemTraitsOR<DTS_UPDOWN|DTS_TIMEFORMAT> >
{
	public:
	// Destruction
	virtual ~CColumnTime(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("CColumnTime"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(CColumnTime)
protected:
	CColumnTime(){}
};

class CColumnSchedule : public NN::LVItemComboBoxImpl<CColumnSchedule,NN::LVItemTraitsOR<CBS_DROPDOWNLIST|CBS_HASSTRINGS> >
{
	typedef NN::LVItemComboBoxImpl<CColumnSchedule,NN::LVItemTraitsOR<CBS_DROPDOWNLIST|CBS_HASSTRINGS> > baseClass;
public:
	// Data
	struct Data : public baseClass::Data
	{
	public:
		Data()
		{
			AddItemString(_T("Once"));
			SetItemHeight(150);
		}
		virtual ~Data(){}
	};
	// Destruction
	virtual ~CColumnSchedule(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("CColumnSchedule"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(CColumnSchedule)
protected:
	CColumnSchedule(){}
};

class CColumnHotKey : public NN::LVItemHotKeyImpl<CColumnHotKey>
{
public:
	// Destruction
	virtual ~CColumnHotKey(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("CColumnHotKey"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(CColumnHotKey)
protected:
	CColumnHotKey(){}
};

class CColumnDelay : public NN::LVItemEditImpl<CColumnDelay,NN::LVItemTraitsOR<ES_NUMBER> >
{
	typedef NN::LVItemEditImpl<CColumnDelay,NN::LVItemTraitsOR<ES_NUMBER> > baseClass;
public:
	// Data
	struct Data : public baseClass::Data
	{
	public:
		Data()
		{
			SetItemString(_T("0"));
			ModifyStyle(ES_LEFT|ES_CENTER,ES_RIGHT);
		}
		virtual ~Data(){}
	};
	// Destruction
	virtual ~CColumnDelay(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("CColumnDelay"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(CColumnDelay)
protected:
	CColumnDelay(){}
};

#endif // __COLUMN_H__
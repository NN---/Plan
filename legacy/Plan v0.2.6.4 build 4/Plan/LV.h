#ifndef __LV_H__
#define __LV_H__

#pragma once

#ifndef __ATLAPP_H__
#pragma message("atlapp.h was included in LV.h")
#include <atlapp.h>
#endif // __ATLAPP_H__

#ifndef  __ATLCOLL_H__
#pragma message("atlcoll.h was included in LV.h")
#include <atlcoll.h>
#endif // __ATLCOLL_H__

#ifndef __ATLMISC_H__
#define WTL_USE_CSTRING
#include <atlmisc.h>
#pragma message("WTL_USE_CSTRING was defined in LV.h")
#pragma message("atlmisc.h was included in LV.h")
#endif // __ATLMISC_H__

#ifndef __ATLCTRLS_H__
#pragma message("atlctrls.h was included in LV.h")
#include <atlctrls.h>
#endif // __ATLCTRLS_H__

#ifndef __SYSFILETIME_H__
#pragma message("SysFileTime.h was included in LV.h")
#include "SysFileTime.h"
#endif // __SYSFILETIME_H__

namespace NN
{
#define LVM_ITEM_FIRST(x) (WM_USER+200+x)
#define LVM_ITEM_DESELECT LVM_ITEM_FIRST(1) // wParam - GetData

// Forward declarations
template <DWORD t_dwStyle, DWORD t_dwExStyle, DWORD t_dwExListViewStyle>
class LVTraitsImpl;
	// typedef LVTraitsImpl<...> LVTraits;

// LVItemTraitsImpl;
template <DWORD t_dwStyle, DWORD t_dwExStyle, class TWinTraits>
class LVItemTraitsOR;
// LVItemTraits;

struct LVItemInfo;

class LVItem;
	class LVItemNull;
	class LVItemEdit;
	class LVItemHotKey;
	class LVItemDateTime;
	class LVItemComboBox;
	class LVItemComboBoxEx;

template<class T,class TBase,class TWinTraits>
class LVImpl;
	class LV;

// LVTraitsImpl
template <DWORD t_dwStyle, DWORD t_dwExStyle, DWORD t_dwExListViewStyle>
class LVTraitsImpl
{
public:
	static DWORD GetWndStyle(DWORD dwStyle)
	{ return (dwStyle == 0) ? t_dwStyle : dwStyle; }
	static DWORD GetWndExStyle(DWORD dwExStyle)
	{ return (dwExStyle == 0) ? t_dwExStyle : dwExStyle; }
	static DWORD GetExtendedLVStyle()
	{ return t_dwExListViewStyle; }
};

// LVTraits
typedef LVTraitsImpl<WS_CHILD | WS_VISIBLE | LVS_REPORT | 
	LVS_SHOWSELALWAYS | LVS_SINGLESEL,0,
	/*LVS_EX_CHECKBOXES |*/ LVS_EX_FULLROWSELECT |
	//LVS_EX_HEADERDRAGDROP |// LVS_EX_TRACKSELECT |
	LVS_EX_GRIDLINES/* | LVS_EX_INFOTIP*/> LVTraits;

typedef CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,0> LVItemTraitsImpl;
template <DWORD t_dwStyle=0, DWORD t_dwExStyle=0, class TWinTraits=LVItemTraitsImpl>
class LVItemTraitsOR
{
public:
	static DWORD GetWndStyle(DWORD dwStyle)
	{ return dwStyle | t_dwStyle | TWinTraits::GetWndStyle(dwStyle); }
	static DWORD GetWndExStyle(DWORD dwExStyle)
	{ return dwExStyle | t_dwExStyle | TWinTraits::GetWndExStyle(dwExStyle); }
};
typedef LVItemTraitsOR<> LVItemTraits;

// LVItem
class ATL_NO_VTABLE LVItem
{
public:
	// Data
	struct Data
	{
	protected:
		WTL::CString m_strItem;
		DWORD m_dwStyle;
		DWORD m_dwExStyle;
	public:
		Data() : m_dwStyle(LVItemTraits::GetWndStyle(0)), m_dwExStyle(LVItemTraits::GetWndExStyle(0))
		{ }
		Data(const Data &rData)
		{
			m_strItem=rData.m_strItem;
			m_dwStyle=rData.m_dwStyle;
			m_dwExStyle=rData.m_dwExStyle;
		}
		const Data operator=(const Data &rData)
		{
			if(this==&rData)
				return *this;
			m_strItem=rData.m_strItem;
			m_dwStyle=rData.m_dwStyle;
			m_dwExStyle=rData.m_dwExStyle;
			return *this;
		}
		void SetItemString(LPCTSTR lpszItem)
		{
			ATLASSERT(lpszItem);
			m_strItem=lpszItem;
		}
		const WTL::CString &GetItemString() const
		{ return m_strItem; }
		void SetStyle(DWORD dwNewStyle)
		{ m_dwStyle=dwNewStyle; }
		void SetExStyle(DWORD dwNewStyle)
		{ m_dwExStyle=dwNewStyle; }
		DWORD GetStyle() const
		{ return m_dwStyle; }
		DWORD GetExStyle() const
		{ return m_dwExStyle; }
	};
	static const Data s_defaultData;
	virtual void SetData(LVItem::Data *) = 0;
	virtual void GetData(LVItem::Data *&) = 0;
	static Data *CreateData() { return new Data; }
	// Select/Deselect
	virtual void SelectItem(HWND hWndParent,RECT &rectItem) = 0;
	virtual void DeselectItem() = 0;
	// Destruction
	virtual ~LVItem() {}
protected:
	LVItem() { }
private:
	LVItem(LVItem &);
	void operator=(LVItem &);
};

#ifdef LV_IMPLEMENT
const LVItem::Data LVItem::s_defaultData;
#endif // LV_IMPLEMENT

// Helper macro for LVItemInfo
#define LVITEMINFO_NS(name_space,class_name) ((NN::LVItemInfo *)(&name_space::class_name::class##class_name))
#define LVITEMINFO(class_name) LVITEMINFO_NS(NN,class_name)
#define LVITEMINFO_NS_DATA(name_space,class_name) (name_space::class_name::Data *)((LVITEMINFO_NS(name_space,class_name)->CreateData()))
#define LVITEMINFO_DATA(class_name) LVITEMINFO_NS_DATA(NN,class_name)

#define LVITEMINFO_DECLARE(class_name) \
	public : \
	static LVItemInfo class##class_name; \
	static LVItemInfo *GetItemInfo() \
	{ return LVITEMINFO(class_name); } \
	static LVItem *CreateItem() \
	{ return new class_name; } \
	static LVItem::Data *CreateData() \
	{ return new Data; } \

#ifdef LV_IMPLEMENT
#define LVITEMINFO_IMPLEMENT(class_name) \
	LVItemInfo class_name::class##class_name= \
	{ class_name::CreateItem, class_name::CreateData };
#else // !LV_IMPLEMENT
#define LVITEMINFO_IMPLEMENT(class_name)
#endif // LV_IMPLEMENT

// LVItemInfo
struct LVItemInfo
{
	LVItem* (*m_pfnCreateItem)();
	LVItem::Data* (*m_pfnCreateData)();
	LVItem *CreateItem()
	{ return (*m_pfnCreateItem)(); }
	LVItem::Data *CreateData()
	{ return (*m_pfnCreateData)(); }
};

// LVItemNull
class LVItemNull : public LVItem
{
public:
	// Data
	struct Data : public LVItem::Data { };
	virtual void SetData(LVItem::Data *pData)
	{
		ATLASSERT(pData);
		pData;
	} 
	virtual void GetData(LVItem::Data *&pData)
	{
		ATLASSERT(pData);
		pData;
	}
	// Select/Deselect
	virtual void SelectItem(HWND /*hWndParent*/,RECT &/*rectItem*/) { }
	virtual void DeselectItem(){}
	// Destruction
	virtual ~LVItemNull(){}
	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemNull)
protected:
	LVItemNull(){}
};

LVITEMINFO_IMPLEMENT(LVItemNull)

#define LVITEMDATAWINDOWSET(pData) \
	ATLASSERT(pData); \
	if(!m_hWnd) \
		return; \
	ModifyStyle((DWORD)-1,pData->GetStyle()); \
	ModifyStyleEx((DWORD)-1,pData->GetExStyle())

#define LVITEMDATAWINDOWGET(pData) \
	ATLASSERT(pData); \
	if(!m_hWnd) \
		return; \
	pData->SetStyle(GetWindowLong(GWL_STYLE)); \
	pData->SetExStyle(GetWindowLong(GWL_EXSTYLE))

#define LVITEMDATAWINDOWSTRSET(pData) \
	LVITEMDATAWINDOWSET(pData); \
	SetWindowText(pData->GetItemString())

#define LVITEMDATAWINDOWSTRGET(pData) \
	LVITEMDATAWINDOWGET(pData); \
	int nLength = GetWindowTextLength(); \
	WTL::CString _strText; \
	nLength = GetWindowText(_strText.GetBuffer(nLength+1), nLength+1); \
	_strText.ReleaseBuffer(nLength); \
	_strText.FreeExtra(); \
	pData->SetItemString(_strText)

// LVItemEdit
class LVItemEdit : public LVItem,
				public ATL::CWindowImpl<LVItemEdit,WTL::CEdit,LVItemTraitsOR<ES_AUTOHSCROLL> >
{
	typedef LVItemEdit thisClass;
public:
	// Data
	struct Data : public LVItem::Data {	};
	virtual void SetData(LVItem::Data *pData)
	{
		LVITEMDATAWINDOWSTRSET(pData);
	}
	virtual void GetData(LVItem::Data *&pData)
	{
		LVITEMDATAWINDOWSTRGET(pData);
	}
	// Select/Deselect
	virtual void SelectItem(HWND hWndParent,RECT &rectItem)
	{
		Create(hWndParent,rectItem);
		SetFocus();
	}
	virtual void DeselectItem()
	{
		if(m_hWnd)
			DestroyWindow();
	}
	// Destruction
	virtual ~LVItemEdit(){}

	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
	END_MSG_MAP()

	// Message
	// WM_KEYDOWN
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		BOOL bGetData=FALSE;
		switch(wParam)
		{
			case VK_RETURN:
				if(GetStyle()&ES_MULTILINE)
				{
					bHandled=FALSE;
					break;
				}
				bGetData=TRUE;
			case VK_ESCAPE:
				::PostMessage(GetParent(),LVM_ITEM_DESELECT,(WPARAM)bGetData,0);
				bHandled=TRUE;
				return FALSE;
			default:
				bHandled=FALSE;
				break;
		}
		return FALSE;
	}

	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemEdit)
protected:
	LVItemEdit(){}
};

LVITEMINFO_IMPLEMENT(LVItemEdit)

class LVItemHotKey : public LVItem,
				public ATL::CWindowImpl<LVItemHotKey,WTL::CHotKeyCtrl,LVItemTraits>
{
	typedef LVItemHotKey thisClass;
public:
	// Data
	struct Data : public LVItem::Data
	{
	protected:
		WORD m_wVirtualKeyCode;
		WORD m_wModifiers;
		void UpdateItemString()
		{
			if(!m_wVirtualKeyCode&&!m_wModifiers)
			{
				m_strItem=_T("None");
				return;
			}
			m_strItem=_T("");
			if(m_wModifiers&HOTKEYF_CONTROL)
				m_strItem+=_T("Ctrl + ");
			if(m_wModifiers&HOTKEYF_SHIFT)
                m_strItem+=_T("Shift + ");
			if(m_wModifiers&HOTKEYF_ALT)
				m_strItem+=_T("Alt + ");
			switch(m_wVirtualKeyCode)
			{
				case VK_ESCAPE:
					m_strItem+=_T("Escape");
					break;
				case VK_F1:
					m_strItem+=_T("F1");
					break;
				case VK_F2:
					m_strItem+=_T("F2");
					break;
				case VK_F3:
					m_strItem+=_T("F3");
					break;
				case VK_F4:
					m_strItem+=_T("F4");
					break;
				case VK_F5:
					m_strItem+=_T("F5");
					break;
				case VK_F6:
					m_strItem+=_T("F6");
					break;
				case VK_F7:
					m_strItem+=_T("F7");
					break;
				case VK_F8:
					m_strItem+=_T("F8");
					break;
				case VK_F9:
					m_strItem+=_T("F9");
					break;
				case VK_F10:
					m_strItem+=_T("F10");
					break;
				case VK_F11:
					m_strItem+=_T("F11");
					break;
				case VK_F12:
					m_strItem+=_T("F12");
					break;
				case VK_F13:
					m_strItem+=_T("F13");
					break;
				case VK_F14:
					m_strItem+=_T("F14");
					break;
				case VK_F15:
					m_strItem+=_T("F15");
					break;
				case VK_F16:
					m_strItem+=_T("F16");
					break;
				case VK_F17:
					m_strItem+=_T("F17");
					break;
				case VK_F18:
					m_strItem+=_T("F18");
					break;
				case VK_F19:
					m_strItem+=_T("F19");
					break;
				case VK_F20:
					m_strItem+=_T("F20");
					break;
				case VK_F21:
					m_strItem+=_T("F21");
					break;
				case VK_F22:
					m_strItem+=_T("F22");
					break;
				case VK_F23:
					m_strItem+=_T("F23");
					break;
				case VK_F24:
					m_strItem+=_T("F24");
					break;
				case VK_SCROLL:
					m_strItem+=_T("Scroll Lock");
					break;
				case VK_PAUSE:
					m_strItem+=_T("Pause");
					break;
				case VK_INSERT:
					m_strItem+=m_wModifiers&HOTKEYF_EXT?_T("Insert"):_T("Num 0");
					break;
				case VK_DELETE:
					m_strItem+=m_wModifiers&HOTKEYF_EXT?_T("Delete"):_T("Num Del");
					break;
				case VK_HOME:
					m_strItem+=m_wModifiers&HOTKEYF_EXT?_T("Home"):_T("Num 7");
					break;
				case VK_END:
					m_strItem+=m_wModifiers&HOTKEYF_EXT?_T("End"):_T("Num 1");
					break;
				case VK_PRIOR:
					m_strItem+=m_wModifiers&HOTKEYF_EXT?_T("Page Up"):_T("Num 9");
					break;
				case VK_NEXT:
					m_strItem+=m_wModifiers&HOTKEYF_EXT?_T("Page Down"):_T("Num 3");
					break;
				case VK_TAB:
					m_strItem+=_T("Tab");
					break;
				case VK_RETURN:
					m_strItem+=_T("Enter");
					break;
				case VK_CAPITAL:
					m_strItem+=_T("Caps Lock");
					break;
				case VK_LEFT:
					m_strItem+=_T("Left");
					break;
				case VK_RIGHT:
					m_strItem+=_T("Right");
					break;
				case VK_UP:
					m_strItem+=_T("Up");
					break;
				case VK_DOWN:
					m_strItem+=_T("Down");
					break;
				case VK_NUMLOCK:
					m_strItem+=_T("Num Lock");
					break;
				case VK_NUMPAD0:
					m_strItem+=_T("Num 0");
					break;
				case VK_NUMPAD1:
					m_strItem+=_T("Num 1");
					break;
				case VK_NUMPAD2:
					m_strItem+=_T("Num 2");
					break;
				case VK_NUMPAD3:
					m_strItem+=_T("Num 3");
					break;
				case VK_NUMPAD4:
					m_strItem+=_T("Num 4");
					break;
				case VK_NUMPAD5:
					m_strItem+=_T("Num 5");
					break;
				case VK_NUMPAD6:
					m_strItem+=_T("Num 6");
					break;
				case VK_NUMPAD7:
					m_strItem+=_T("Num 7");
					break;
				case VK_NUMPAD8:
					m_strItem+=_T("Num 8");
					break;
				case VK_NUMPAD9:
					m_strItem+=_T("Num 9");
					break;
				case VK_MULTIPLY:
					m_strItem+=_T("Num *");
					break;
				case VK_ADD:
					m_strItem+=_T("Num +");
					break;
				case VK_SUBTRACT:
					m_strItem+=_T("Num -");
					break;
				case VK_DECIMAL:
					m_strItem+=_T("Num Del");
					break;
				case VK_DIVIDE:
					m_strItem+=_T("Num /");
					break;
				case VK_CLEAR:
					m_strItem+=_T("Num 5");
					break;
				default:
					m_strItem+=(char)m_wVirtualKeyCode;
					break;
			}
		}
	public:
		Data() : m_wVirtualKeyCode(0) , m_wModifiers(0)
		{
			UpdateItemString();
		}
		Data(const Data &rData)
		{
			*((LVItem::Data *)this)=(const LVItem::Data &)rData;
			m_wVirtualKeyCode=rData.m_wVirtualKeyCode;
			m_wModifiers=rData.m_wModifiers;
		}
		const Data operator=(const Data &rData)
		{
			if(this==&rData)
				return *this;
			*((LVItem::Data *)this)=(const LVItem::Data &)rData;
			m_wVirtualKeyCode=rData.m_wVirtualKeyCode;
			m_wModifiers=rData.m_wModifiers;
			return *this;
		}
		void SetVirtualKeyCode(const WORD wVirtualKeyCode)
		{
			m_wVirtualKeyCode=wVirtualKeyCode;
			UpdateItemString();
		}
		const WORD GetVirtualKeyCode() const
		{ return m_wVirtualKeyCode; }
		void SetModifiers(const WORD wModifiers)
		{
			m_wModifiers=wModifiers;
			UpdateItemString();
		}
		const WORD GetModifiers() const
		{ return m_wModifiers; }
		void SetHotKey(const WORD wVirtualKeyCode,const WORD wModifiers)
		{
			m_wVirtualKeyCode=wVirtualKeyCode;
			m_wModifiers=wModifiers;
			UpdateItemString();
		}
		DWORD GetHotKey()
		{
			return MAKEWORD(m_wVirtualKeyCode,m_wModifiers);
		}
	};
	virtual void SetData(LVItem::Data *pData)
	{
		LVITEMDATAWINDOWSET(pData);
		Data *pItemData=(Data*)pData;
		SetHotKey(pItemData->GetVirtualKeyCode(),pItemData->GetModifiers());
	} 
	virtual void GetData(LVItem::Data *&pData)
	{
		LVITEMDATAWINDOWGET(pData);
		WORD wVirtualKeyCode,wModifiers;
		GetHotKey(wVirtualKeyCode,wModifiers);
		Data *pItemData=(Data*)pData;
		pItemData->SetHotKey(wVirtualKeyCode,wModifiers);
	}
	// Select/Deselect
	virtual void SelectItem(HWND hWndParent,RECT &rectItem)
	{
		Create(hWndParent,rectItem);
		SetFocus();
	}
	virtual void DeselectItem()
	{
		if(m_hWnd)
			DestroyWindow();
	}

	// Destruction
	virtual ~LVItemHotKey() {}
	
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
	END_MSG_MAP()

	// Message
	// WM_KEYDOWN
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		BOOL bGetData=FALSE;
		switch(wParam)
		{
			case VK_RETURN:
				bGetData=TRUE;
			case VK_ESCAPE:
				::PostMessage(GetParent(),LVM_ITEM_DESELECT,(WPARAM)bGetData,0);
				bHandled=TRUE;
				return FALSE;
			default:
				bHandled=FALSE;
				break;
		}
		return FALSE;
	}


	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemHotKey)
protected:
	LVItemHotKey() {}
};

LVITEMINFO_IMPLEMENT(LVItemHotKey)

class LVItemDateTime : public LVItem,
				public ATL::CWindowImpl<LVItemDateTime,WTL::CDateTimePickerCtrl,LVItemTraits>
{
	typedef LVItemDateTime thisClass;
public:
	// Data
	struct Data : public LVItem::Data
	{
	protected:
		SystemTime m_Time;
		WTL::CString m_strTimeFormat;
	public:
		Data()
		{
			GetLocalTime(&m_Time);
			FormatTime();
		}
		void SetStyle(DWORD dwNewStyle)
		{
			if(GetStyle()==dwNewStyle)
				return;
			LVItem::Data::SetStyle(dwNewStyle);
			FormatTime();
		}
		const SystemTime &GetTime() const
		{ return m_Time; }
		void SetTime(const SystemTime &rTime)
		{
			m_Time = rTime;
			FormatTime();
		}
		void SetTime(const SYSTEMTIME &st)
		{
			m_Time=st;
			FormatTime();
		}
		const WTL::CString &GetTimeFormat()
		{ return m_strTimeFormat; }
		void SetTimeFormat(LPCTSTR lpszFormat)
		{
			m_strTimeFormat=lpszFormat;
			FormatTime();	
		}
		const Data operator=(const Data &rData)
		{
			if(this==&rData)
				return *this;
			*((LVItem::Data *)this)=(const LVItem::Data &)rData;
			SetTime(rData.GetTime());
			return *this;
		}
		void FormatTime()
		{
			LONG lStyle=GetStyle();
			int cch;
			LPCTSTR lpszFormat=m_strTimeFormat==_T("")?NULL:(LPCTSTR)m_strTimeFormat;
			if(lStyle&DTS_TIMEFORMAT)
			{
				cch=::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
					&m_Time,lpszFormat,NULL,NULL);
				::GetTimeFormat(LOCALE_USER_DEFAULT,LOCALE_NOUSEROVERRIDE,
					&m_Time,lpszFormat,m_strItem.GetBuffer(cch+1),cch+1);
			}
			else
			if(lStyle&DTS_LONGDATEFORMAT)
			{
				cch=::GetDateFormat(LOCALE_USER_DEFAULT,DATE_LONGDATE,
					&m_Time,lpszFormat,NULL,NULL);
				::GetDateFormat(LOCALE_USER_DEFAULT,DATE_LONGDATE,
					&m_Time,lpszFormat,m_strItem.GetBuffer(cch+1),cch+1);
			}
			//
			// May Not Work Well
#if (_WIN32_IE >= 0x500)
			else
			if(lStyle&DTS_SHORTDATECENTURYFORMAT)
			{
				ATLTRACE("DTS_SHORTDATECENTURYFORMAT - Test,\n file - %s\nline - %d",__FILE__,__LINE__);
				cch=::GetDateFormat(LOCALE_USER_DEFAULT|LOCALE_SSHORTDATE,DATE_SHORTDATE,
					&m_Time,lpszFormat,NULL,NULL);
				::GetDateFormat(LOCALE_USER_DEFAULT,DATE_SHORTDATE,
					&m_Time,lpszFormat,m_strItem.GetBuffer(cch+1),cch+1);
			}
#endif // (_WIN32_IE >= 0x500)
			// May Not Work Well
			//
			else //	DTS_SHORTDATEFORMAT:
			{
				cch=::GetDateFormat(LOCALE_USER_DEFAULT,DATE_SHORTDATE,
					&m_Time,lpszFormat,NULL,NULL);
				::GetDateFormat(LOCALE_USER_DEFAULT,DATE_SHORTDATE,
					&m_Time,lpszFormat,m_strItem.GetBuffer(cch+1),cch+1);
			}
			m_strItem.ReleaseBuffer();
			m_strItem.FreeExtra();
		}
	};
	virtual void SetData(LVItem::Data *pData)
	{
		LVITEMDATAWINDOWSET(pData);
		Data *pItemData=(Data*)pData;
		SetFormat(pItemData->GetTimeFormat()==_T("")?NULL:(LPTSTR)(LPCTSTR)pItemData->GetTimeFormat());
		SetSystemTime(GDT_VALID,(LPSYSTEMTIME)(const LPSYSTEMTIME)&pItemData->GetTime());
	} 
	virtual void GetData(LVItem::Data *&pData)
	{
		LVITEMDATAWINDOWSTRGET(pData);
		Data *pItemData=(Data*)pData;
		SYSTEMTIME st;
		GetSystemTime(&st);
		pItemData->SetTime(st);
	}
	// Select/Deselect
	virtual void SelectItem(HWND hWndParent,RECT &rectItem)
	{
		Create(hWndParent,rectItem);
		SetFocus();
	}
	virtual void DeselectItem()
	{
		if(m_hWnd)
			DestroyWindow();
	}

	// Destruction
	virtual ~LVItemDateTime() {}

    BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
	END_MSG_MAP()

	// Message
	// WM_KEYDOWN
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		BOOL bGetData=FALSE;
		switch(wParam)
		{
			case VK_RETURN:
				bGetData=TRUE;
			case VK_ESCAPE:
				::PostMessage(GetParent(),LVM_ITEM_DESELECT,(WPARAM)bGetData,0);
				bHandled=TRUE;
				return FALSE;
			default:
				bHandled=FALSE;
				break;
		}
		return FALSE;
	}
	
	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemDateTime)
protected:
	LVItemDateTime() {}
};

LVITEMINFO_IMPLEMENT(LVItemDateTime)

// LVItemComboBox
class LVItemComboBox : public LVItem,
				public ATL::CWindowImpl<LVItemComboBox,WTL::CComboBox,LVItemTraitsOR<CBS_DROPDOWNLIST> >
{
	typedef LVItemComboBox thisClass;
public:
	// Data
	struct Data : public LVItem::Data
	{
	protected:
		ATL::CAtlArray<WTL::CString> m_astrItem;
		int m_nHeight;
	public:
		Data() : m_nHeight(0)
		{ }
		Data(const Data &rData)
		{
			*((LVItem::Data *)this)=(const LVItem::Data &)rData;
			m_nHeight=rData.m_nHeight;
			m_astrItem.RemoveAll();
			int i,nCount=rData.m_astrItem.GetCount();
			m_astrItem.SetCount(nCount);
			for(i=0;i<nCount;i++)
				m_astrItem[i]=rData.m_astrItem[i];
		}
		void AddItemString(LPCTSTR lpszItem)
		{
			m_astrItem.Add(lpszItem);
		}
		void RemoveItemString(size_t iElement)
		{
			m_astrItem.RemoveAt(iElement);
		}
		void RemoveAllItemString()
		{
			m_astrItem.RemoveAll();
		}
		void InsertItemString(size_t iElement,LPCTSTR lpszItem)
		{
			m_astrItem.InsertAt(iElement,lpszItem);
		}
		LPCTSTR GetItemString(size_t iElement)
		{
			return m_astrItem[iElement];
		}
		size_t GetItemStringCount()
		{
			return m_astrItem.GetCount();
		}
		void SetItemHeight(const int nHeight)
		{
			m_nHeight=nHeight;
		}
		const int &GetItemHeight() const
		{
			return m_nHeight;
		}
		const Data operator=(const Data &rData)
		{
			if(this==&rData)
				return *this;
			*((LVItem::Data *)this)=(const LVItem::Data &)rData;
			m_nHeight=rData.m_nHeight;
			m_astrItem.RemoveAll();
			int i,nCount=rData.m_astrItem.GetCount();
			m_astrItem.SetCount(nCount);
			for(i=0;i<nCount;i++)
				m_astrItem[i]=rData.m_astrItem[i];
			return *this;
		}
	};
	virtual void SetData(LVItem::Data *pData)
	{
		LVITEMDATAWINDOWSET(pData);
		Data *pItemData=(Data *)pData;
		ResetContent();
		int i,n;
		for(i=0,n=pItemData->GetItemStringCount();i<n;i++)
			AddString(pItemData->GetItemString(i));
		if(pItemData->GetItemHeight())
		{
			RECT rect;
			GetWindowRect(&rect);
			CWindow(GetParent()).ScreenToClient(&rect);
			rect.bottom=rect.top+pItemData->GetItemHeight();
			MoveWindow(&rect,TRUE);
		}
	}
	virtual void GetData(LVItem::Data *&pData)
	{
		LVITEMDATAWINDOWGET(pData);
		Data *pItemData=(Data *)pData;
		pItemData->RemoveAllItemString();
		WTL::CString strItem;
		int i,n;
		for(i=0,n=GetCount();i<n;i++)
		{
			GetLBText(i,strItem);
			pItemData->AddItemString(strItem);
		}
	}
	// Select/Deselect
	virtual void SelectItem(HWND hWndParent,RECT &rectItem)
	{
		Create(hWndParent,rectItem);
		SetFocus();
	}
	virtual void DeselectItem()
	{
		if(m_hWnd)
			DestroyWindow();
	}
	// Destruction
	virtual ~LVItemComboBox(){}

	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
	END_MSG_MAP()

	// Message
	// WM_KEYDOWN
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		BOOL bGetData=FALSE;
		switch(wParam)
		{
			case VK_RETURN:
				bGetData=TRUE;
			case VK_ESCAPE:
				::PostMessage(GetParent(),LVM_ITEM_DESELECT,(WPARAM)bGetData,0);
				bHandled=TRUE;
				return FALSE;
			default:
				bHandled=FALSE;
				break;
		}
		return FALSE;
	}

	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemComboBox)
protected:
	LVItemComboBox(){}
};

LVITEMINFO_IMPLEMENT(LVItemComboBox)

// LVImpl
template<class T,class TBase=WTL::CListViewCtrl,class TWinTraits=LVTraits>
class ATL_NO_VTABLE LVImpl : public ATL::CWindowImpl<T,WTL::CListViewCtrlT<TBase>,TWinTraits>,
						public WTL::CCustomDraw<T>
{
	typedef LVImpl<T,TBase,TWinTraits> thisClass;
	typedef ATL::CWindowImpl<T,WTL::CListViewCtrlT<TBase>,TWinTraits> baseClass;
protected:
	struct LVColumn
	{
		int m_nWidth;
		LVColumn()
		{
			m_nWidth=-1;
		}
		LVColumn(int nWidth) : m_nWidth(nWidth) {}
	};
	ATL::CAtlArray<LVColumn> m_aColumn;
	struct LVRow
	{
		struct LVRowItem
		{
			LVItemInfo *m_pInfo;
			LVItem::Data *m_pData;
			LVRowItem()
			{
				m_pInfo=LVITEMINFO(LVItemNull);
				m_pData=m_pInfo->CreateData();
				*m_pData=LVItem::s_defaultData;
			}
			~LVRowItem()
			{
				delete m_pData;
			}
			void SetInfo(LVItemInfo *pInfo)
			{
				ATLASSERT(pInfo);
				if(m_pInfo!=pInfo)
				{
                    delete m_pData;
                    m_pData=pInfo->CreateData();
					m_pInfo=pInfo;
				}
			}
			template<class TData>
			SetData(TData *pData)
			{
				ATLASSERT(pData);
				ATLASSERT(m_pData);
				*((TData *)m_pData)=*pData;
			}
			template<class TData>
			GetData(TData *pData)
			{
				ATLASSERT(pData);
				ATLASSERT(m_pData);
				*pData=*((TData *)m_pData);
			}
			const LVRowItem &operator=(const LVRowItem &rLVRowItem)
			{
				SetInfo(rLVRowItem.m_pInfo);
				SetData(rLVRowItem.m_pData);
				return *this;
			}
		};
		ATL::CAtlArray<LVRowItem> m_aItems;
		const LVRow &operator=(const LVRow &rLVRow)
		{
			m_aItems.RemoveAll();
			int n=rLVRow.m_aItems.GetCount();
			m_aItems.SetCount(n);
			int i;
			for(i=0;i<n;i++)
				m_aItems[i]=rLVRow.m_aItems[i];
			return *this;
		}
	};
	ATL::CAtlArray<LVRow> m_aRow;
	struct SelItem
	{
		LVItem *m_pItem;
		int m_iRow;
		int m_iCol;
		SelItem() : m_iRow(-1), m_iCol(-1)
		{
			m_pItem=LVITEMINFO(LVItemNull)->CreateItem();
		}
		~SelItem() { delete m_pItem; }
		void Set(int iRow,int iCol)
		{
			m_iRow=iRow;
			m_iCol=iCol;
		}
	};
	SelItem m_SelItem;
public:
	// Attributes
	static DWORD GetExtendedLVStyle()
	{ return TWinTraits::GetExtendedLVStyle(); }

	BOOL SubclassWindow(HWND hWnd)
	{
		BOOL bRet = baseClass::SubclassWindow(hWnd);
		if(bRet)
		{
			T *pT = static_cast<T*>(this);
			pT;
			SetExtendedListViewStyle(pT->GetExtendedLVStyle());
		}
		return bRet;
	}

	BOOL HitTestEx(const POINT &point,int *pRow,int *pCol)
	{
		if(GetStyle() | LVS_REPORT)
		{
			int iRow;
			iRow=HitTest(point,NULL);
			if(iRow==-1)
			{
				if(pRow)
					*pRow=-1;
				if(pCol)
					*pCol=-1;
				return FALSE;
			}
			if(pRow)
				*pRow=iRow;
			RECT rect;
			GetItemRect(iRow,&rect,LVIR_BOUNDS);
			
			int iCol,nCol,iColWidth;
			for(iCol=0,nCol=GetHeader().GetItemCount();iCol<nCol;iCol++)
			{
				iColWidth = GetColumnWidth(iCol);
				if(point.x>=rect.left && point.x<=(rect.left + iColWidth))
				{
					if(pCol)
						*pCol=iCol;
					break;
				}
				rect.left+=iColWidth;
			}
			return iRow!=-1&&iCol!=-1;
		}
		else
		{
			if(pCol)
				*pCol=-1;
			return HitTest(point,NULL);
		}
	}

	int InsertLVColumn(int iCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth)
	{
		ModifyStyle(0, LVS_REPORT);
		if(iCol==-1)
			iCol=m_aRow.GetCount();
		LVColumn lvColumn(nWidth);
		m_aColumn.InsertAt(iCol,lvColumn);
		if(m_aRow.GetCount())
		{
			LVRow::LVRowItem lvRowItem;
			int iRow,nRow;
			for(iRow=0,nRow=m_aRow.GetCount();iRow<nRow;iRow++)
				m_aRow[iRow].m_aItems.InsertAt(iCol,lvRowItem);
		}
		return baseClass::InsertColumn(iCol, lpszColumnHeading, nFormat, nWidth,iCol);
	}

	void InsertLVRow(int iRow)
	{
		ATLASSERT(m_aColumn.GetCount());
		LVRow lvRow;
		lvRow.m_aItems.SetCount(m_aColumn.GetCount());
		m_aRow.InsertAt(iRow,lvRow);
		InsertItem(iRow,_T(""));
	}
	
	void SetLVItemText(int iRow,int iCol,LPCTSTR lpszItemText,bool bUpdate=true)
	{
		ATLASSERT(lpszItemText);
		if(!m_aRow[iRow].m_aItems[iCol].m_pData)
			m_aRow[iRow].m_aItems[iCol].m_pData=m_aRow[iRow].m_aItems[iCol].m_pInfo->CreateData();
		m_aRow[iRow].m_aItems[iCol].m_pData->SetItemString(lpszItemText);
		if(bUpdate)
			UpdateRow(iRow);
	}

	void GetLVItemText(int iRow,int iCol,LPTSTR lpszItemText,int nChar)
	{
		ATLASSERT(lpszItemText);
		if(m_aRow[iRow].m_aItems[iCol].m_pData)
			lstrcpyn(lpszItemText,m_aRow[iRow].m_aItems[iCol].m_pData->m_strItem,nChar);
		else
			lpszItemText[0]=0;
	}

	void GetLVItemText(int iRow,int iCol,WTL::CString &strItemText)
	{
		if(m_aRow[iRow].m_aItems[iCol].m_pData)
			strItemText=m_aRow[iRow].m_aItems[iCol].m_pData->m_strItem;
		else
			strItemText=_T("");
	}

	void SetLVItemInfo(int iRow,int iCol,LVItemInfo *pInfo,bool bUpdate=true)
	{
		ATLASSERT(pInfo);
		if(!pInfo)
			return;
		m_aRow[iRow].m_aItems[iCol].SetInfo(pInfo);
		if(bUpdate)
			UpdateRow(iRow);
	}

	LVItemInfo *GetLVItemInfo(int iRow,int iCol)
	{
		return m_aRow[iRow].m_aItems[iCol].m_pInfo;
	}

	void SetLVRowInfo(int iRow,LVItemInfo *pInfo,bool bUpdate=true)
	{
		ATLASSERT(m_aRow.GetCount());
		ATLASSERT(pInfo);
		int iCol,nCol;
		for(iCol=0,nCol=m_aRow[0].m_aItems.GetCount();iCol<nCol;iCol++)
			SetLVItemInfo(iRow,iCol,pInfo,bUpdate);
	}

	void SetLVColumnInfo(int iCol,LVItemInfo *pInfo,bool bUpdate=true)
	{
		ATLASSERT(m_aColumn.GetCount());
		ATLASSERT(pInfo);
		int iRow,nRow;
		for(iRow=0,nRow=m_aRow.GetCount();iRow<nRow;iRow++)
			SetLVItemInfo(iRow,iCol,pInfo,bUpdate);
	}

	void UpdateRow(int iRow)
	{
		if(!m_aRow.GetCount())
			return;
		int iCol,nCol;
		for(iCol=0,nCol=m_aRow[0].m_aItems.GetCount();iCol<nCol;iCol++)
			UpdateItem(iRow,iCol);
	}

	void UpdateItem(int iRow,int iCol)
	{
		LVITEM lvitem;
		lvitem.mask=LVIF_TEXT;
		lvitem.iItem=iRow;
		lvitem.iSubItem=iCol;
		lvitem.pszText=(LPTSTR)((LPCTSTR)(m_aRow[iRow].m_aItems[iCol].m_pData->GetItemString()));
		SetItem(&lvitem);
	}

	void UpdateAllItems()
	{
		int iRow,nRow,iCol,nCol;
		for(iRow=0,nRow=m_aRow.GetCount();iRow<nRow;iRow++)
			for(iCol=0,nCol=m_aRow[iRow].m_aItems.GetCount();iCol<nCol;iCol++)
				UpdateItem(iRow,iCol);
	}

	void TranslateMouse(POINT &point,BOOL bDblClk)
	{
		SetFocus();
		int iRow,iCol;
		if(HitTestEx(point,&iRow,&iCol))
		{
			if(m_SelItem.m_iRow!=iRow||m_SelItem.m_iCol!=iCol)
			{
				if(m_SelItem.m_iRow!=-1&&m_SelItem.m_iCol!=-1)
					DeselectItem();
				delete m_SelItem.m_pItem;
				m_SelItem.m_pItem=m_aRow[iRow].m_aItems[iCol].m_pInfo->CreateItem();
				m_SelItem.Set(iRow,iCol);
			}
			if(bDblClk)
				SelectItem();
		}
	}

	void DeselectItem(BOOL bGetData=TRUE,bool bUpdate=true)
	{
		if(bGetData)
			m_SelItem.m_pItem->GetData(m_aRow[m_SelItem.m_iRow].m_aItems[m_SelItem.m_iCol].m_pData);
		m_SelItem.m_pItem->DeselectItem();
		if(bUpdate)
			UpdateItem(m_SelItem.m_iRow,m_SelItem.m_iCol);
	}

	void SelectItem()
	{
		RECT rect;
		GetSubItemRect(m_SelItem.m_iRow,m_SelItem.m_iCol, LVIR_LABEL, &rect);
		m_SelItem.m_pItem->SelectItem(m_hWnd,rect);
		m_SelItem.m_pItem->SetData(m_aRow[m_SelItem.m_iRow].m_aItems[m_SelItem.m_iCol].m_pData);
	}

	template<class TData>
	void SetLVItemData(int iRow,int iCol,TData *pData)
	{
		m_aRow[iRow].m_aItems[iCol].SetData(pData);
	}

	template<class TData>
	void GetLVItemData(int iRow,int iCol,TData *&pData)
	{
		ATLASSERT(pData);
		m_aRow[iRow].m_aItems[iCol].GetData(pData);
	}

	template<class TData>	
	void SetLVRowData(int iRow,TData *pData)
	{
		ATLASSERT(m_aRow.GetCount());
		int iCol,nCol;
		for(iCol=0,nCol=m_aRow[0].m_aItems.GetCount();iCol<nCol;iCol++)
			SetLVItemData(iRow,iCol,pData);
	}

	template<class TData>
	void GetLVRowData(int iRow,TData **&apData)
	{
		ATLASSERT(m_aRow.GetCount());
		int iCol,nCol;
		for(iCol=0,nCol=m_aRow[0].m_aItems.GetCount();iCol<nCol;iCol++)
			GetLVItemData(iRow,iCol,apData[iCol]);
	}

	template<class TData>
	void SetLVColumnData(int iCol,TData *pData)
	{
		ATLASSERT(m_aColumn.GetCount());
		int iRow,nRow;
		for(iRow=0,nRow=m_aRow.GetCount();iRow<nRow;iRow++)
			SetLVItemData(iRow,iCol,pData);
	}

	template<class TData>
	void GetLVColumnData(int iCol,TData **&apData)
	{
		ATLASSERT(m_aRow.GetCount());
		ATLASSERT(m_aColumn.GetCount());
		int iRow,nRow;
		for(iRow=0,nRow=m_aRow.GetCount();iRow<nRow;iRow++)
			GetLVItemData(iRow,iCol,pData[iRow]);
	}

	LVImpl() { }
	~LVImpl() {	}

	// Declare SuperClass
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())
	
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(LVM_ITEM_DESELECT, OnLVItemDeselect)
		NOTIFY_CODE_HANDLER(HDN_BEGINTRACKA, OnBeginTrack)
		NOTIFY_CODE_HANDLER(HDN_BEGINTRACKW, OnBeginTrack)
		NOTIFY_CODE_HANDLER(HDN_ENDTRACKA, OnEndTrack)
		NOTIFY_CODE_HANDLER(HDN_ENDTRACKW, OnEndTrack)
		// Chain Message Map
		CHAIN_MSG_MAP_ALT(WTL::CCustomDraw<T>,1)
	END_MSG_MAP()

	// Message
	// WM_CREATE
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		T* pT = static_cast<T*>(this);
		pT;
		SetExtendedListViewStyle(pT->GetExtendedLVStyle());
		WTL::CImageList m_ImageList; 
		m_ImageList.Create(1,20,ILC_COLOR,1,1); 
		SetImageList(m_ImageList,LVSIL_SMALL); 
		return lRet;
	}

	// WM_LBUTTONDOWN
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		POINT point={LOWORD(lParam),HIWORD(lParam)};
		TranslateMouse(point,FALSE);
		bHandled=FALSE;
		return FALSE;
	}

	// WM_LBUTTONDBLCLK
	LRESULT OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		POINT point={LOWORD(lParam),HIWORD(lParam)};
		TranslateMouse(point,TRUE);
		bHandled=FALSE;
		return FALSE;
	}

	// WM_KEYDOWN
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(wParam!=VK_LEFT&&
			wParam!=VK_RIGHT&&
			wParam!=VK_UP&&
			wParam!=VK_DOWN&&
			wParam!=VK_RETURN)
		{
			bHandled=FALSE;
			return FALSE;
		}
		if(m_SelItem.m_iCol!=-1&&m_SelItem.m_iRow!=-1)
			switch(wParam)
			{
				case VK_LEFT:
					DeselectItem();
					m_SelItem.m_iCol--;
					break;
				case VK_RIGHT:
					DeselectItem();
					m_SelItem.m_iCol++;
					break;
				case VK_UP:
					DeselectItem();
					m_SelItem.m_iRow--;
					break;
				case VK_DOWN:
					DeselectItem();
					m_SelItem.m_iRow++;
					break;
				case VK_RETURN:
					SelectItem();
					return FALSE;
			}
		else
		{
			SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			EnsureVisible(0, FALSE);
		}
		delete m_SelItem.m_pItem;
		if(m_SelItem.m_iCol<0)
			m_SelItem.m_iCol=0;
		else
		if(m_SelItem.m_iCol>=(int)m_aColumn.GetCount())
			m_SelItem.m_iCol=m_aColumn.GetCount()-1;
		if(m_SelItem.m_iRow<0)
			m_SelItem.m_iRow=0;
		else
		if(m_SelItem.m_iRow>=(int)m_aRow.GetCount())
			m_SelItem.m_iRow=m_aRow.GetCount()-1;
		m_SelItem.m_pItem=m_aRow[m_SelItem.m_iRow].m_aItems[m_SelItem.m_iCol].m_pInfo->CreateItem();
		bHandled=FALSE;
		return FALSE;
	}

	// LVM_ITEM_DESELECT
	LRESULT OnLVItemDeselect(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		DeselectItem((BOOL)wParam);
		bHandled=TRUE;
		return FALSE;
	}

	// HDN_BEGINTRACKA
	// HDN_BEGINTRACKW
	LRESULT OnBeginTrack(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& bHandled)
	{
		DeselectItem(FALSE,false);
		bHandled=FALSE;
		return FALSE;
	}

	// HDN_ENDTRACKA
	// HDN_ENDTRACKW
	LRESULT OnEndTrack(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
	{
		LPNMHEADER phdn=(LPNMHEADER)pnmh;
		if(phdn->pitem->fmt|HDI_WIDTH)
			m_aColumn[phdn->iItem].m_nWidth=phdn->pitem->cxy;
		bHandled=FALSE;
		return FALSE;
	}

	// NM_CUSTOMDRAW
	// CDDS_PREPAINT
	DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
	{
		return CDRF_NOTIFYITEMDRAW;
	}

	// NM_CUSTOMDRAW
	// CDDS_ITEMPREPAINT
	DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
	{
		LPNMLVCUSTOMDRAW lpNMLVCustomDraw = (LPNMLVCUSTOMDRAW)lpNMCustomDraw;
		CFont f;
		f.CreateFont(10,10,0,0,0,0,0,0,0,0,0,0,0,"Arial");
		SelectObject(lpNMLVCustomDraw->nmcd.hdc,(HGDIOBJ)(HFONT)f);
		return CDRF_NEWFONT;
	}

//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
//  NOTIFY_CODE_HANDLER(NM_CUSTOMDRAW, OnCustomDraw)
};

// LV
class LV : public LVImpl<LV>
{
	typedef LV thisClass;
	typedef LVImpl<LV> baseClass;
public:
	DECLARE_WND_SUPERCLASS(_T("NN::LV"), GetWndClassName())
	BEGIN_MSG_MAP(thisClass)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};

};

#endif // __LV_H__
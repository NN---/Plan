#ifndef __NN_LV_H__
#define __NN_LV_H__

#pragma once

namespace NN
{
#define LVM_ITEM_FIRST(x) (WM_USER+200+x)
#define LVM_ITEM_DESELECT LVM_ITEM_FIRST(1) // wParam - bGetData

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
typedef LVTraitsImpl<WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
	WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
	WS_EX_CLIENTEDGE, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT |
	//LVS_EX_HEADERDRAGDROP | LVS_EX_TRACKSELECT |
	LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES | LVS_EX_INFOTIP> LVTraits;

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

// LVItemDraw
struct LVItemDraw
{
	COLORREF m_clrText;
	COLORREF m_clrBk;
	LVItemDraw(COLORREF clrText=(COLORREF)-1,COLORREF clrBk=(COLORREF)-1)
	{
		m_clrText=clrText;
		m_clrBk=clrBk;
	}
};

// LVItemCtlColor
// Helper class for processing WM_CTL,OCM_CTL Messages
template<class T,bool t_bCtlColorBase=false>
class LVItemCtlColor
{
	typedef LVItemCtlColor<T,t_bCtlColorBase> thisClass;
public:
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_RANGE_HANDLER(WM_CTLCOLORMSGBOX,WM_CTLCOLORSTATIC,OnCtlColorBase)
		MESSAGE_RANGE_HANDLER(OCM_CTLCOLORMSGBOX,OCM_CTLCOLORSTATIC,OnCtlColorBase)
	END_MSG_MAP()

	LRESULT OnCtlColorBase(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
	{
		T *pT=static_cast<T*>(this);
		pT->SetMsgHandled(TRUE);
		HBRUSH hBrush=NULL;
		if(t_bCtlColorBase)
			hBrush=pT->OnCtlColor(uMsg,(HDC)wParam,(HWND)lParam);
		else
			switch(uMsg)
			{
				case WM_CTLCOLORBTN:
				case OCM_CTLCOLORBTN:
					hBrush=pT->OnCtlColorBtn((HDC)wParam,(HWND)lParam);
					break;
				case WM_CTLCOLORDLG:
				case OCM_CTLCOLORDLG:
					hBrush=pT->OnCtlColorDlg((HDC)wParam,(HWND)lParam);
					break;
				case WM_CTLCOLOREDIT:
				case OCM_CTLCOLOREDIT:
					hBrush=pT->OnCtlColorEdit((HDC)wParam,(HWND)lParam);
					break;
				case WM_CTLCOLORLISTBOX:
				case OCM_CTLCOLORLISTBOX:
					hBrush=pT->OnCtlColorListBox((HDC)wParam,(HWND)lParam);
					break;
				case WM_CTLCOLORMSGBOX:
				case OCM_CTLCOLORMSGBOX:
					hBrush=pT->OnCtlColorMsgBox((HDC)wParam,(HWND)lParam);
					break;
				case WM_CTLCOLORSCROLLBAR:
				case OCM_CTLCOLORSCROLLBAR:
					hBrush=pT->OnCtlColorScrollBar((HDC)wParam,(HWND)lParam);
					break;
				case WM_CTLCOLORSTATIC:
				case OCM_CTLCOLORSTATIC:
					hBrush=pT->OnCtlColorStatic((HDC)wParam,(HWND)lParam);
					break;
				default:
					hBrush=pT->OnCtlColor(uMsg,(HDC)wParam,(HWND)lParam);
					break;
			}
		bHandled = pT->IsMsgHandled();
		return hBrush?(LRESULT)hBrush:pT->DefWindowProc();
	}

	HBRUSH OnCtlColor(UINT /*uMsg*/,HDC /*hDC*/,HWND /*hWnd*/)
	{
		T *pT=static_cast<T*>(this);
		return (HBRUSH)pT->DefWindowProc();
	}

	HBRUSH OnCtlColorBtn(HDC /*hDC*/,HWND /*hWnd*/)
	{
		T *pT=static_cast<T*>(this);
		return (HBRUSH)pT->DefWindowProc();
	}

	HBRUSH OnCtlColorDlg(HDC /*hDC*/,HWND /*hWnd*/)
	{
		T *pT=static_cast<T*>(this);
		return (HBRUSH)pT->DefWindowProc();
	}

	HBRUSH OnCtlColorEdit(HDC /*hDC*/,HWND /*hWnd*/)
	{
		T *pT=static_cast<T*>(this);
		return (HBRUSH)pT->DefWindowProc();
	}

	HBRUSH OnCtlColorListBox(HDC /*hDC*/,HWND /*hWnd*/)
	{
		T *pT=static_cast<T*>(this);
		return (HBRUSH)pT->DefWindowProc();
	}

	HBRUSH OnCtlColorMsgBox(HDC /*hDC*/,HWND /*hWnd*/)
	{
		T *pT=static_cast<T*>(this);
		return (HBRUSH)pT->DefWindowProc();
	}

	HBRUSH OnCtlColorScrollBar(HDC /*hDC*/,HWND /*hWnd*/)
	{
		T *pT=static_cast<T*>(this);
		return (HBRUSH)pT->DefWindowProc();
	}

	HBRUSH OnCtlColorStatic(HDC /*hDC*/,HWND /*hWnd*/)
	{
		T *pT=static_cast<T*>(this);
		return (HBRUSH)pT->DefWindowProc();
	}
};

// LVItem
// Base class for Items
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
		Data() { }
		virtual ~Data(){}
		const Data operator=(const Data &rData)
		{
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
		void ModifyStyle(DWORD dwRemove,DWORD dwAdd)
		{ m_dwStyle=(m_dwStyle & ~dwRemove) | dwAdd; }
		DWORD GetExStyle() const
		{ return m_dwExStyle; }
		void ModifyExStyle(DWORD dwRemove,DWORD dwAdd)
		{ m_dwExStyle=(m_dwExStyle & ~dwRemove) | dwAdd; }
	};
	virtual void SetData(LVItem::Data * pData) {pData;}
	virtual void GetData(LVItem::Data *&pData) {pData;}
	static Data *CreateData() { return new Data; }
	// Select/Deselect
	virtual void SelectItem(HWND hWndParent,RECT &rectItem) {hWndParent;rectItem;}
	virtual void DeselectItem() {}
	// Destruction
	virtual ~LVItem() {}
protected:
	LVItem() { }
private:
	LVItem(LVItem &);
	void operator=(LVItem &);
};

// Helper macro for LVItemInfo
#define LVITEMINFO_NS(name_space,class_name) ((NN::LVItemInfo *)(&name_space::class_name::class##class_name))
#define LVITEMINFO_NN(class_name) LVITEMINFO_NS(NN,class_name)
#define LVITEMINFO(class_name) ((NN::LVItemInfo *)(&class_name::class##class_name))
#define LVITEMINFO_NS_DATA(name_space,class_name) (name_space::class_name::Data *)((LVITEMINFO_NS(name_space,class_name)->CreateData()))
#define LVITEMINFO_NN_DATA(class_name) LVITEMINFO_NS_DATA(NN,class_name)
#define LVITEMINFO_DATA(class_name) (class_name::Data *)((LVITEMINFO_NS(name_space,class_name)->CreateData()))

#define LVITEMINFO_DECLARE(class_name) \
	public : \
	static NN::LVItemInfo class##class_name; \
	static NN::LVItemInfo *GetItemInfo() \
	{ return LVITEMINFO(class_name); } \
	static NN::LVItem *CreateItem() \
	{ return new class_name; } \
	static NN::LVItem::Data *CreateData() \
	{ return new Data; } \

#define LVITEMINFO_IMPLEMENT(class_name) \
	NN::LVItemInfo class_name::class##class_name= \
	{ class_name::CreateItem, class_name::CreateData };

#define LVITEMINFO_IMPLEMENT_NS(name_space,class_name) \
	NN::LVItemInfo name_space::class_name::class##class_name= \
	{ name_space::class_name::CreateItem, name_space::class_name::CreateData };

#define LVITEMINFO_IMPLEMENT_NN(class_name) LVITEMINFO_IMPLEMENT_NS(NN,class_name)

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
// Just to not put NULL
class LVItemNull : public LVItem
{
	typedef LVItemNull thisClass;
	typedef LVItem baseClass;
public:
	// Data
	struct Data : public baseClass::Data
	{
		Data()
		{
			m_dwStyle=LVItemTraitsImpl::GetWndStyle(0);
			m_dwExStyle=LVItemTraitsImpl::GetWndExStyle(0);
		}
	};
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

// Helper macro for Set/Get Data
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

// Helper macro for Chain Message Map
#define CHAIN_MSG_MAP_BASE(thisClass,baseClass) \
	BEGIN_MSG_MAP(thisClass) \
		CHAIN_MSG_MAP(baseClass) \
	END_MSG_MAP()

#define CHAIN_MSG_MAP_BASE2(thisClass,baseClass1,baseClass2) \
	BEGIN_MSG_MAP(thisClass) \
		CHAIN_MSG_MAP(baseClass1) \
		CHAIN_MSG_MAP(baseClass2) \
	END_MSG_MAP()

// LVItemWindowImpl
// Window Item Implement
template<class T,class TBase=ATL::CWindow,class TWinTraits=LVItemTraits>
class ATL_NO_VTABLE LVItemWindowImpl : public LVItem,
					public ATL::CWindowImpl<T,TBase,TWinTraits>
{
	typedef LVItemWindowImpl thisClass;
	typedef LVItem baseClass;
public:
	// Data
	struct Data : public baseClass::Data
	{
		Data()
		{
			m_dwStyle=TWinTraits::GetWndStyle(0);
			m_dwExStyle=TWinTraits::GetWndExStyle(0);
		}
	};
	virtual void SetData(LVItem::Data *pData)
	{ LVITEMDATAWINDOWSTRSET(pData); }
	virtual void GetData(LVItem::Data *&pData)
	{ LVITEMDATAWINDOWSTRGET(pData); }
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
	virtual ~LVItemWindowImpl(){}

	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
		MESSAGE_HANDLER(WM_KILLFOCUS,OnKillFocus)
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

	// WM_KEYDOWN
	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		::PostMessage(GetParent(),LVM_ITEM_DESELECT,0,0);
		bHandled=TRUE;
		return FALSE;
	}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(NULL,TBase::GetWndClassName())
protected:
	LVItemWindowImpl(){}
};

// LVItemEditImpl
// Edit Item Implement
template<class T,class TWinTraits=LVItemTraitsOR<ES_AUTOHSCROLL> >
class ATL_NO_VTABLE LVItemEditImpl : public LVItemWindowImpl<T,WTL::CEdit,TWinTraits>
{
	typedef LVItemEditImpl thisClass;
	typedef LVItemWindowImpl<T,WTL::CEdit,TWinTraits> baseClass;
public:
	// Destruction
	~LVItemEditImpl(){}
	
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	// Message
	// WM_KEYDOWN
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = (wParam==VK_RETURN && (GetStyle()&ES_MULTILINE)) ? TRUE : FALSE;
		return FALSE;
	}
protected:
	LVItemEditImpl(){}
};

// LVItemEdit
// Edit Item
class LVItemEdit : public LVItemEditImpl<LVItemEdit>
{
public:
	// Destruction
	~LVItemEdit(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("NN::LVItemEdit"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemEdit)
protected:
	LVItemEdit(){}
};

// LVItemEditNumber
// Edit Item only numbers allowed
class LVItemEditNumber : public LVItemEditImpl<LVItemEditNumber,LVItemTraitsOR<ES_AUTOHSCROLL |	ES_NUMBER> >
{
public:
	// Destruction
	~LVItemEditNumber(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("NN::LVItemEditNumber"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemEditNumber)
protected:
	LVItemEditNumber(){}
};

// LVItemHotKeyImpl
// HotKey Item Implement
template<class T,class TWinTraits=LVItemTraits>
class ATL_NO_VTABLE LVItemHotKeyImpl : public LVItemWindowImpl<T,WTL::CHotKeyCtrl,TWinTraits>
{
	typedef LVItemHotKeyImpl thisClass;
	typedef LVItemWindowImpl<T,WTL::CHotKeyCtrl,TWinTraits> baseClass;
public:
	// Data
	struct Data : public baseClass::Data
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
		const Data operator=(const Data &rData)
		{
			*((baseClass::Data *)this)=(const baseClass::Data &)rData;
			m_wVirtualKeyCode=rData.m_wVirtualKeyCode;
			m_wModifiers=rData.m_wModifiers;
			return *this;
		}
		~Data() { }
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

	// Destruction
	~LVItemHotKeyImpl(){}

	// Message Map
	CHAIN_MSG_MAP_BASE(thisClass,baseClass)
protected:
	LVItemHotKeyImpl(){}
};

// LVItemHotKey
// HotKey Item
class LVItemHotKey : public LVItemHotKeyImpl<LVItemHotKey>
{
public:
	// Destruction
	~LVItemHotKey(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("NN::LVItemHotKey"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemHotKey)
protected:
	LVItemHotKey() {}
};

// LVItemDateTimeImpl
// DateTime Item Implement
template<class T,class TWinTraits=LVItemTraits>
class LVItemDateTimeImpl : public LVItemWindowImpl<T,WTL::CDateTimePickerCtrl,TWinTraits>
{
	typedef LVItemDateTimeImpl thisClass;
	typedef LVItemWindowImpl<T,WTL::CDateTimePickerCtrl,TWinTraits> baseClass;
public:
	// Data
	struct Data : public baseClass::Data
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
			baseClass::Data::SetStyle(dwNewStyle);
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
			*((baseClass::Data *)this)=(const baseClass::Data &)rData;
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

	// Destruction
	~LVItemDateTimeImpl(){}
	
	// Message Map
	CHAIN_MSG_MAP_BASE(thisClass,baseClass)
protected:
	LVItemDateTimeImpl(){}
};

// LVItemDateTime
// DateTime Item
class LVItemDateTime : public LVItemDateTimeImpl<LVItemDateTime>
{
public:
	// Destruction
	virtual ~LVItemDateTime() {}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("NN::LVItemDateTime"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemDateTime)
protected:
	LVItemDateTime() {}
};

// LVItemDateTime_UpDown
// DateTime Item with DTS_UPDOWN
class LVItemDateTime_UpDown : public LVItemDateTimeImpl<LVItemDateTime_UpDown,LVItemTraitsOR<DTS_UPDOWN> >
{
	typedef LVItemDateTime_UpDown thisClass;
	typedef LVItemDateTimeImpl<LVItemDateTime_UpDown,LVItemTraitsOR<DTS_UPDOWN> > baseClass;
public:
	// Destruction
	virtual ~LVItemDateTime_UpDown() {}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("NN::LVItemDateTime_UpDown"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemDateTime_UpDown)
protected:
	LVItemDateTime_UpDown() {}
};

// LVItemComboBoxImpl
// ComboBox Item Implement
template<class T,class TWinTraits=LVItemTraitsOR<CBS_DROPDOWNLIST|CBS_HASSTRINGS> >
class LVItemComboBoxImpl : public LVItemWindowImpl<T,WTL::CComboBox,TWinTraits>
{
	typedef LVItemComboBoxImpl thisClass;
	typedef LVItemWindowImpl<T,WTL::CComboBox,TWinTraits> baseClass;
public:
	// Data
	struct Data : public baseClass::Data
	{
	protected:
		ATL::CAtlArray<WTL::CString> m_astrItem;
		int m_nHeight;
		int m_iCurSel;
	public:
		Data() : m_nHeight(0), m_iCurSel(-1)
		{ }
		~Data() {}
		// Test
		Data(const Data &rData)
		{
			*((baseClass::Data *)this)=(const baseClass::Data &)rData;
			m_nHeight=rData.m_nHeight;
			m_astrItem.RemoveAll();
			int i,nCount=rData.m_astrItem.GetCount();
			m_astrItem.SetCount(nCount);
			for(i=0;i<nCount;i++)
				m_astrItem[i]=rData.m_astrItem[i];
			m_strItem=rData.m_strItem;
		}
		void AddItemString(LPCTSTR lpszItem)
		{ m_astrItem.Add(lpszItem); }
		void RemoveItemString(size_t iElement)
		{ m_astrItem.RemoveAt(iElement); }
		void RemoveAllItemString()
		{ m_astrItem.RemoveAll(); }
		void InsertItemString(size_t iElement,LPCTSTR lpszItem)
		{
			m_astrItem.InsertAt(iElement,lpszItem);
			m_strItem=m_astrItem[iElement];
		}
		LPCTSTR GetItemString(size_t iElement)
		{ return iElement==-1?_T(""):m_astrItem[iElement]; }
		size_t GetItemStringCount()
		{ return m_astrItem.GetCount(); }
		void SetItemHeight(const int nHeight)
		{ m_nHeight=nHeight; }
		const int GetItemHeight() const
		{ return m_nHeight; }
		void SetCurSel(const int iCurSel)
		{
			m_iCurSel=iCurSel;
			m_strItem=iCurSel==-1?_T(""):m_astrItem[m_iCurSel];
		}
		const int GetCurSel() const
		{ return m_iCurSel; }
		const Data operator=(const Data &rData)
		{
			*((baseClass::Data *)this)=(const baseClass::Data &)rData;
			m_nHeight=rData.m_nHeight;
			m_astrItem.RemoveAll();
			int i,nCount=rData.m_astrItem.GetCount();
			m_astrItem.SetCount(nCount);
			for(i=0;i<nCount;i++)
				m_astrItem[i]=rData.m_astrItem[i];
			m_strItem=rData.m_strItem;
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
		if(pItemData->GetItemHeight()&&GetParent())
		{
			RECT rect;
			GetWindowRect(&rect);
			CWindow(GetParent()).ScreenToClient(&rect);
			rect.bottom=rect.top+pItemData->GetItemHeight();
			MoveWindow(&rect,TRUE);
		}
		SetCurSel(pItemData->GetCurSel());
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
		pItemData->SetItemString(pItemData->GetItemString(GetCurSel()));
		pItemData->SetCurSel(GetCurSel());
	}

	// Destruction
	~LVItemComboBoxImpl(){}

	// Message Map
	CHAIN_MSG_MAP_BASE(thisClass,baseClass)
protected:
	LVItemComboBoxImpl(){}
};

// LVItemComboBox
// ComboBox Item
class LVItemComboBox : public LVItemComboBoxImpl<LVItemComboBox>
{
public:
	// Destruction
	virtual ~LVItemComboBox(){}

	// Declare Window SuperClass
	DECLARE_WND_SUPERCLASS(_T("NN::LVItemComboBox"),GetWndClassName())

	// LVItemInfo
	LVITEMINFO_DECLARE(LVItemComboBox)
protected:
	LVItemComboBox(){}
};

// LVImpl
template<class T,class TBase=WTL::CListViewCtrl,class TWinTraits=LVTraits>
class ATL_NO_VTABLE LVImpl : public ATL::CWindowImpl<T,TBase,TWinTraits>,
						public WTL::CCustomDraw<T>
{
	typedef LVImpl<T,TBase,TWinTraits> thisClass;
	typedef ATL::CWindowImpl<T,TBase,TWinTraits> baseClass;
protected:
	struct LVColumn
	{
		int m_nWidth;
		LVItemInfo *m_pInfo;
		LVColumn(int nWidth=-1,LVItemInfo *pInfo=LVITEMINFO(LVItemNull)) : m_nWidth(nWidth)
		{ m_pInfo=pInfo; }
	};
	ATL::CAtlArray<LVColumn> m_aColumn;
	struct LVRow
	{
		struct LVRowItem
		{
			LVItemInfo *m_pInfo;
			LVItem::Data *m_pData;
			LVRowItem(LVItemInfo *pInfo=LVITEMINFO(LVItemNull))
			{
				m_pInfo=pInfo;
				m_pData=m_pInfo->CreateData();
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
	LVItemDraw m_drawSelItem;
	LVItemDraw m_drawSelRow;
	LVItemDraw m_drawItem;
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
			// Set Height
			WTL::CImageList m_ImageList; 
			m_ImageList.Create(1,20,ILC_COLOR,1,1); 
			SetImageList(m_ImageList,LVSIL_SMALL); 
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

	int InsertLVColumn(int iCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem=-1, LVItemInfo *pInfo=LVITEMINFO(LVItemNull))
	{
		if(iCol==-1)
			iCol=m_aRow.GetCount();
		LVColumn lvColumn(nWidth,pInfo);
		m_aColumn.InsertAt(iCol,lvColumn);
		m_aColumn[iCol].m_pInfo=pInfo;
		if(m_aRow.GetCount())
		{
			LVRow::LVRowItem lvRowItem(pInfo);
			int iRow,nRow;
			for(iRow=0,nRow=m_aRow.GetCount();iRow<nRow;iRow++)
				m_aRow[iRow].m_aItems.InsertAt(iCol,lvRowItem);
		}
		if(nSubItem==-1)
			nSubItem=iCol;
		return baseClass::InsertColumn(iCol, lpszColumnHeading, nFormat, nWidth,nSubItem);
	}

	void InsertLVRow(int iRow,bool bUpdate=true)
	{
		ATLASSERT(m_aColumn.GetCount());
		LVRow lvRow;
		lvRow.m_aItems.SetCount(m_aColumn.GetCount());
		m_aRow.InsertAt(iRow,lvRow);
		InsertItem(iRow,_T(""));
		int iCol,nCol;
		for(iCol=0,nCol=(int)m_aColumn.GetCount();iCol<nCol;iCol++)
			SetLVItemInfo(iRow,iCol,m_aColumn[iCol].m_pInfo,bUpdate);
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

	void SetLVItemInfo(int iRow,int iCol,LVItemInfo *pInfo=LVITEMINFO(LVItemNull),bool bUpdate=true)
	{
		ATLASSERT(pInfo);
		m_aRow[iRow].m_aItems[iCol].SetInfo(pInfo);
		m_aColumn[iCol].m_pInfo=pInfo;
		if(bUpdate)
			UpdateRow(iRow);
	}

	LVItemInfo *GetLVItemInfo(int iRow,int iCol)
	{ return m_aRow[iRow].m_aItems[iCol].m_pInfo; }

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

	LVItemInfo* GetLVColumnInfo(int iCol)
	{ return m_aColumn[iCol].m_pInfo; }

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

	void DeleteLVRow(int iRow)
	{
		ATLASSERT(m_aColumn.GetCount());
		m_aRow.RemoveAt(iRow);
		baseClass::DeleteItem(iRow);
	}

	void DeleteLVColumn(int iCol)
	{
		if(m_aRow.GetCount())
		{
			int iRow,nRow;
			for(iRow=0,nRow=m_aRow.GetCount();iRow<nRow;iRow++)
				m_aRow[iRow].m_aItems.DeleteAt(iCol);
		}
		m_aColumn.RemoveAt(iCol);
		baseClass::DeleteColumn(iCol);
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

	void CheckSelectedItem(int nCurrItem)
	{
		LVITEM lvi;
		lvi.iItem = nCurrItem;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_STATE;
		lvi.stateMask = LVIS_SELECTED;
		GetItem(&lvi);
		if(!(lvi.state & LVIS_SELECTED))
			return;
		BOOL bCheck = !GetCheckState(nCurrItem);
		int nItem = -1;
		int nOldItem = -1;
		while((nItem = GetNextItem(nOldItem, LVNI_SELECTED)) != -1)
		{
			if(nItem != nCurrItem)
				SetCheckState(nItem, bCheck);
			nOldItem = nItem;
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
		GetSubItemRect(m_SelItem.m_iRow,m_SelItem.m_iCol,LVIR_LABEL,&rect);
		m_SelItem.m_pItem->SelectItem(m_hWnd,rect);
		m_SelItem.m_pItem->SetData(m_aRow[m_SelItem.m_iRow].m_aItems[m_SelItem.m_iCol].m_pData);
	}

	// Data
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

	// Draw Item
	void SetSelItemDraw(const LVItemDraw &rdrawItem)
	{
		m_drawSelItem=rdrawItem;
		LVItem::Data::SetItemDraw(rdrawItem);	
	}

	void GetSelItemDraw(LVItemDraw &drawItem)
	{ drawItem=m_drawSelItem; }

	void SetSelRowDraw(const LVItemDraw &rdrawItem)
	{ m_drawSelRow=rdrawItem; }

	void GetSelRowDraw(LVItemDraw &drawItem)
	{ drawItem=m_drawSelRow; }

	void SetItemDraw(const LVItemDraw &rdrawItem)
	{ m_drawItem=rdrawItem; }

	void GetItemDraw(LVItemDraw &drawItem)
	{ drawItem=m_drawItem; }

	// Construction
	LVImpl()
	{
		COLORREF clr=(COLORREF)-1;
		m_drawSelItem=clr;
		m_drawSelRow=clr;
		m_drawItem=clr;
	}
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
		// Reflect
		REFLECT_NOTIFICATIONS()
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	// Message
	// WM_CREATE
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		T* pT = static_cast<T*>(this);
		pT;
		SetExtendedListViewStyle(pT->GetExtendedLVStyle());
		// Set Height
		WTL::CImageList m_ImageList; 
		m_ImageList.Create(1,20,ILC_COLOR,1,1); 
		SetImageList(m_ImageList,LVSIL_SMALL); 
		return lRet;
	}

	// WM_LBUTTONDOWN
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		POINT point={LOWORD(lParam),HIWORD(lParam)};
		LVHITTESTINFO lvh;
		lvh.pt = point;
		if(HitTest(&lvh) != -1 && lvh.flags == LVHT_ONITEMSTATEICON && ::GetKeyState(VK_CONTROL) >= 0)
			CheckSelectedItem(lvh.iItem);
		else
			TranslateMouse(point,FALSE);
		bHandled=FALSE;
		return FALSE;
	}

	// WM_LBUTTONDBLCLK
	LRESULT OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		POINT point={LOWORD(lParam),HIWORD(lParam)};
		LVHITTESTINFO lvh;
		lvh.pt = point;
		if(HitTest(&lvh) != -1 && lvh.flags == LVHT_ONITEMSTATEICON && ::GetKeyState(VK_CONTROL) >= 0)
			CheckSelectedItem(lvh.iItem);
		else
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
					if(!m_SelItem.m_iCol)
						return FALSE;
					m_SelItem.m_iCol--;
					break;
				case VK_RIGHT:
					DeselectItem();
					if(m_SelItem.m_iCol==(int)m_aColumn.GetCount()-1)
						return FALSE;
					m_SelItem.m_iCol++;
					break;
				case VK_UP:
					DeselectItem();
					if(!m_SelItem.m_iRow)
						return FALSE;
					m_SelItem.m_iRow--;
					break;
				case VK_DOWN:
					DeselectItem();
					if(m_SelItem.m_iRow==(int)m_aRow.GetCount()-1)
						return FALSE;
					m_SelItem.m_iRow++;
					break;
				case VK_RETURN:
					SelectItem();
					return FALSE;
				case VK_SPACE:
				{
					int nCurrItem = GetNextItem(-1, LVNI_FOCUSED);
					if(nCurrItem != -1  && ::GetKeyState(VK_CONTROL) >= 0)
					CheckSelectedItem(nCurrItem);
				}
					break;
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

	// CCustomDraw functions
	// CDDS_PREPAINT
    DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
	{
        return CDRF_NOTIFYITEMDRAW;
    }

	// CDDS_ITEMPREPAINT
    DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
    {
		return CDRF_NOTIFYSUBITEMDRAW;
	}

	// CDDS_ITEMPREPAINT | CDDS_SUBITEM
	DWORD OnSubItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
	{
        LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lpNMCustomDraw;
        if(lplvcd->nmcd.dwItemSpec==(DWORD)m_SelItem.m_iRow)
		{
			if(lplvcd->iSubItem==m_SelItem.m_iCol)
			{
				if(m_drawSelItem.m_clrText!=(COLORREF)-1)
					lplvcd->clrText = m_drawSelItem.m_clrText;
				if(m_drawSelItem.m_clrBk!=(COLORREF)-1)
					lplvcd->clrTextBk = m_drawSelItem.m_clrBk;
			}
			else
			{
				if(m_drawSelRow.m_clrText!=(COLORREF)-1)
					lplvcd->clrText = m_drawSelRow.m_clrText;
				if(m_drawSelRow.m_clrBk!=(COLORREF)-1)
					lplvcd->clrTextBk = m_drawSelRow.m_clrBk;
			}
			lplvcd->nmcd.uItemState &= ~(CDIS_SELECTED | CDIS_FOCUS);
		}
		else
		{
			if(m_drawItem.m_clrText!=(COLORREF)-1)
				lplvcd->clrText = m_drawItem.m_clrText;
			if(m_drawItem.m_clrBk!=(COLORREF)-1)
				lplvcd->clrTextBk = m_drawItem.m_clrBk;
		}
		SetMsgHandled(TRUE);
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
public:
	DECLARE_WND_SUPERCLASS(_T("NN::LV"), GetWndClassName())
};

};

#endif // __NN_LV_H__
// LV.h
//

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
	class LVItemButton;
	class LVItemHotKey;
	class LVItemDateTime;

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
	LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED , WS_EX_CLIENTEDGE,
	/*LVS_EX_CHECKBOXES |*/ LVS_EX_FULLROWSELECT |
	//LVS_EX_HEADERDRAGDROP | LVS_EX_TRACKSELECT |
	LVS_EX_GRIDLINES/* | LVS_EX_INFOTIP*/> LVTraits;

typedef CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS , WS_EX_CLIENTEDGE> LVItemTraitsImpl;
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


//typedef CControlWinTraits LVItemTraits;

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
		const Data &operator=(const Data &rData)
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
		CString &GetItemString()
		{
			return m_strItem;
		}
		void SetStyle(DWORD dwNewStyle)
		{
			m_dwStyle=dwNewStyle;
		}
		void SetExStyle(DWORD dwNewStyle)
		{
			m_dwExStyle=dwNewStyle;
		}
		DWORD GetStyle() const
		{
			return m_dwStyle;
		}
		DWORD GetExStyle() const
		{
			return m_dwExStyle;
		}
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

#define LVITEMINFO_DECLARE(class_name) \
	public : \
	static LVItemInfo class##class_name; \
	static LVItemInfo *GetItemInfo() \
	{ return LVITEMINFO(class_name); } \
	static LVItem *CreateItem() \
	{ return new class_name; } \
	static LVItem::Data *CreateData() \
	{ return new Data; } \
	static int GetDataSize() \
	{ return sizeof(Data); }

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
public:
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
	nLength = GetWindowText(pData->GetItemString().GetBuffer(nLength+1), nLength+1); \
	pData->GetItemString().ReleaseBuffer(nLength)

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

class LVItemButton : public LVItem,
				public ATL::CWindowImpl<LVItemButton,WTL::CButton,LVItemTraits>
{
	typedef LVItemButton thisClass;
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
	virtual ~LVItemButton() {}

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
	LVITEMINFO_DECLARE(LVItemButton)
protected:
	LVItemButton() {}
};

LVITEMINFO_IMPLEMENT(LVItemButton)

class LVItemHotKey : public LVItem,
				public ATL::CWindowImpl<LVItemHotKey,WTL::CHotKeyCtrl,LVItemTraits>
{
	typedef LVItemHotKey thisClass;
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
			void SetData(LVItem::Data *pData)
			{
				ATLASSERT(pData);
				ATLASSERT(m_pData);
				*m_pData=*pData;
			}
			void GetData(LVItem::Data *pData)
			{
				ATLASSERT(pData);
				ATLASSERT(m_pData);
				*pData=*m_pData;
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

	void SetLVItemInfo(int iRow,int iCol,LVItemInfo *pInfo,bool bUpdate=true)
	{
		ATLASSERT(pInfo);
		if(!pInfo)
			return;
		m_aRow[iRow].m_aItems[iCol].SetInfo(pInfo);
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

	void GetLVItemText(int iRow,int iCol,CString &strItemText)
	{
		if(m_aRow[iRow].m_aItems[iCol].m_pData)
			strItemText=m_aRow[iRow].m_aItems[iCol].m_pData->m_strItem;
		else
			strItemText=_T("");
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

	void DeselectItem(BOOL bGetData=TRUE)
	{
		if(bGetData)
			m_SelItem.m_pItem->GetData(m_aRow[m_SelItem.m_iRow].m_aItems[m_SelItem.m_iCol].m_pData);
		m_SelItem.m_pItem->DeselectItem();
		UpdateItem(m_SelItem.m_iRow,m_SelItem.m_iCol);
	}

	void SelectItem()
	{
		RECT rect;
		GetSubItemRect(m_SelItem.m_iRow,m_SelItem.m_iCol, LVIR_LABEL, &rect);
		m_SelItem.m_pItem->SelectItem(m_hWnd,rect);
		m_SelItem.m_pItem->SetData(m_aRow[m_SelItem.m_iRow].m_aItems[m_SelItem.m_iCol].m_pData);
	}

	void SetLVItemData(int iRow,int iCol,LVItem::Data *pData)
	{
		m_aRow[iRow].m_aItems[iCol].SetData(pData);
	}

	void GetLVItemData(int iRow,int iCol,LVItem::Data *&pData)
	{
		m_aRow[iRow].m_aItems[iCol].GetData(pData);
	}

	void SetLVRowData(int iRow,LVItem::Data *pData)
	{
		ATLASSERT(m_aRow.GetCount());
		int iCol,nCol;
		for(iCol=0,nCol=m_aRow[0].m_aItems.GetCount();iCol<nCol;iCol++)
			SetLVItemData(iRow,iCol,pData);
	}

	void SetLVColumnData(int iCol,LVItem::Data *pData)
	{
		ATLASSERT(m_aColumn.GetCount());
		int iRow,nRow;
		for(iRow=0,nRow=m_aRow.GetCount();iRow<nRow;iRow++)
			SetLVItemData(iRow,iCol,pData);
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
		// Chain Message Map
		CHAIN_MSG_MAP(WTL::CCustomDraw<T>)
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
#ifndef __DOCVIEW_H__
#define __DOCVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Array.h"

// namespace DocView
// View
//  ViewWindow<T, TBase = CWindow, TWinTraits= CControlTraits>
// Document<T>
//  DocumentFrame<T, TBase = CWindow, TWinTraits= CControlTraits>

namespace DocView
{

// class View
class ATL_NO_VTABLE View
{
public:
	virtual int PreTranslateMessage(struct tagMSG *pMsg) = 0;
	virtual void UpdateView(void *pData) = 0;
	virtual void DestroyView() = 0;
};

template <typename T, typename TBase = CWindow, typename TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE ViewWindow : public CWindowImpl<T, TBase, TWinTraits>,
					public View
{ };

class ATL_NO_VTABLE Document
{
private:
	// View Array
	CSimpleValArrayR<View *> m_aView;
	// Active View
	View *m_pActiveView;
protected:
	virtual bool CreateDocument() { InitDocument(); return true; }
	virtual void InitDocument()
	{
		int i,n;
		for(i=0,n=m_aView.GetSize();i<n;i++)
		{
			View *pView=m_aView[i];
			if(pView)
				pView->DestroyView();
		}
		m_aView.RemoveAll();
		m_pActiveView = NULL;
	}
	virtual void DestroyDocument() { InitDocument(); }
	virtual void UpdateAllViews(void *pData)
	{
		int i,n;
		for(i=0,n=m_aView.GetSize();i<n;i++)
			m_aView[i]->UpdateView(pData);
	}
	View *GetActiveView() { return m_pActiveView; }
	void SetActiveView(View *pView) { ATLASSERT(pView); m_pActiveView=pView; }
	View *GetView(int iView) { return m_aView[iView]; }
	const CSimpleValArrayR<View *> *GetViewArray() const { return &m_aView; }
	
	void AddView(View *pView) { ATLASSERT(pView); m_aView.Add(pView); }
	void DeleteView(int iView) { m_aView[iView]->DestroyView(); 
		delete m_aView[iView]; m_aView.RemoveAt(iView); }
	void ReplaceView(int iView, View *pView) { ATLASSERT(pView); m_aView[iView]=pView; }
};

template <class T, class TBase = CWindow, class TWinTraits = CFrameWinTraits>
class ATL_NO_VTABLE DocumentFrame : public CFrameWindowImpl<T, TBase, TWinTraits>,
								public Document
{
	typedef DocumentFrame thisClass;
	typedef CFrameWindowImpl<T, TBase, TWinTraits> baseClass;
public:
	DECLARE_FRAME_WND_CLASS(NULL,0)
	BEGIN_MSG_MAP(thisClass)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};

};

#ifndef NO_DOCVIEW_NAMESPACE
using namespace DocView;
#endif // NO_DOCVIEW_NAMESPACE

#endif // __DOCVIEW_H__
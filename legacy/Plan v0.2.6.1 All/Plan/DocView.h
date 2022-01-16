#ifndef __DOCVIEW_H__
#define __DOCVIEW_H__

// CView
// CDocument
//
// Frame must be derived from CDocument
// View must be derived from CView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class CView
class ATL_NO_VTABLE CView
{
public:
	virtual void UpdateView(void *pData) = 0;
	virtual void DestroyView() = 0;
};

class ATL_NO_VTABLE CDocument
#ifndef NO_CDOCUMENT_MSG_MAP
: public CMessageMap
#endif // NO_CDOCUMENT_MSG_MAP
{
	typedef CDocument thisClass;
private:
	// View Array
	class pViewArray : public CSimpleValArray<CView *>
	{
	public:
		CView *& operator[] (int nIndex) const
		{
			return CSimpleArray<CView *>::operator[](nIndex);
		}
	};
	pViewArray m_aView;
	// Active View
	CView *m_pActiveView;
	// class CFrameWindow
	class CFrameWindow : public CFrameWindowImpl<CFrameWindow> {};
	// Active Frame Window
	CFrameWindow *m_pActiveFrameWnd;
protected:
	virtual bool CreateDocument() { InitDocument(); return true; }
	virtual void InitDocument()
	{
		int i,n;
		for(i=0,n=m_aView.GetSize();i<n;i++)
		{
			CView *pView=m_aView[i];
			if(pView)
				pView->DestroyView();
		}
		m_aView.RemoveAll();
		m_pActiveFrameWnd = NULL;
		m_pActiveView = NULL;
	}
	virtual void DestroyDocument() { InitDocument(); }
	virtual void UpdateAllViews(void *pData)
	{
		int i,n;
		for(i=0,n=m_aView.GetSize();i<n;i++)
			m_aView[i]->UpdateView(pData);
	}
	CView *GetActiveView() { return m_pActiveView; }
	void SetActiveView(CView *pView) { ATLASSERT(pView); m_pActiveView=pView; }
	CFrameWindow *GetActiveFrame() { return m_pActiveFrameWnd; }
	void SetActiveFrame(CFrameWindow *pFrame) { ATLASSERT(pFrame); m_pActiveFrameWnd=pFrame; }
	CView *GetView(int iView) { return m_aView[iView]; }
	const pViewArray *GetViewArray() const { return &m_aView; }
	void AddView(CView *pView) { ATLASSERT(pView); m_aView.Add(pView); }
	void DeleteView(int iView) { m_aView[iView]->DestroyView(); 
		delete m_aView[iView]; m_aView.RemoveAt(iView); }
	void ReplaceView(int iView, CView *pView) { ATLASSERT(pView); m_aView[iView]=pView; }
public:
	// Message Map
	BEGIN_MSG_MAP(thisClass)
	END_MSG_MAP()
};

#endif // __DOCVIEW_H__
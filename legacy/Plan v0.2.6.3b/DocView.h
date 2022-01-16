#ifndef __DOCVIEW_H__
#define __DOCVIEW_H__

#pragma once

#define     HINT_UPDATE_ALL_DOCUMENTS     -1
#define     HINT_DOCUMENT_MODIFIED        -2

namespace DocView
{
// Forward declarations
// Helper struct for Document creation
template <class TDoc, class TView>
struct CreateContext;
// Abstract class for all the Views belonging to a Document
template <class TDoc>
class View;
// Base class for View implementations
template <class T, class TView>
class ViewImpl;
// Empty class just for polymorphism
class DocumentBase;
// The Document
template <class T, class TDocTemplate /*= DocTemplateBase*/, class TDoc /*= DocumentBase*/>
class Document;
// The Document with Message Map
template <class T, class TDocTemplate /*= DocTemplateBase*/, class TDoc /*= DocumentBase*/>
class DocumentMsg;
// Abstract base class for Document Templates
class DocTemplateBase;
// Abstract base class for Document Templates with Message Map
class DocTemplateBaseMsg;
// The Document Template
template <class TDoc, class TView, class TFrame, int nID>
class DocTemplate;
// The Document Template with Message Map
template <class TDoc, class TView, class TFrame, int nID>
class DocTemplateMsg;
// The Document Manager
template <class TMainFrame>
class DocManager;
// The Document Manager with Message Map
template <class TMainFrame>
class DocManagerMsg;

// Helper struct for Document creation
template <class TDoc, class TView>
struct CreateContext
{
	TDoc* m_pCurrentDoc;
	TView* m_pCurrentView;
	DocTemplateBase* m_pNewDocTemplate;

	CreateContext()
	{
		::ZeroMemory(this,sizeof(*this));
	}

	template <class TOtherView>
	CreateContext(CreateContext <TDoc, TOtherView>* pContext)
	{
		m_pCurrentDoc = pContext->m_pCurrentDoc;
		m_pNewDocTemplate = pContext->m_pNewDocTemplate;
	}
};

// Abstract class for all the Views belonging to a Document
template <class TDoc>
class View
{
protected:
	TDoc* m_pDocument;
public:
	virtual void Update(View <TDoc>* pSender, LPARAM lHint, LPVOID pHint) = 0;
	
	TDoc* GetDocument()
	{
        return m_pDocument;
	}
	
	void SetDocument(TDoc* pDoc)
    {
		m_pDocument = pDoc;
	}
	
	void UpdateAllDocs()
	{
		DocTemplateBase* pDocTemplate = m_pDocument->GetDocTemplate();
		int nDocs = pDocTemplate->GetNumDocs();
		for(int i=0;i<nDocs;i++)
		{
            TDoc* pDoc = static_cast<TDoc*>(pDocTemplate->GetDocument(j));
			if(pDoc)
				pDoc->UpdateAllViews(this, HINT_UPDATE_ALL_DOCUMENTS);
		}
	}
};

// Base class for View implementations
template <class T, class TView>
class ViewImpl : public TView
{
public:
	virtual void Update(TView* pSender, LPARAM lHint, LPVOID pHint)
	{
		T* pT = static_cast<T*>(this);
		pT->OnUpdate(pSender, lHint, pHint);
	}
   
	virtual void OnInitialUpdate()
	{
		T* pT = static_cast<T*>(this);
		pT->OnUpdate(0, 0, 0);
	}

	HWND GetParentFrame()
	{
		T* pT = static_cast<T*>(this);
		HWND hWnd = pT->GetParent();
		while(!(GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD))
			hWnd = ::GetParent(hWnd);
		return hWnd;
	}
};

// Empty class just for polymorphism
class DocumentBase
{
};

// The Document
template <class T, class TDocTemplate = DocTemplateBase, class TDoc = DocumentBase>
class Document : public TDoc
{
public:
	Document()
	{
        m_bModified = FALSE;
	}
	
    void UpdateAllViews(View <T>* pSender, LPARAM lHint = 0,  LPVOID pHint = NULL)
	{
		int count = m_aViews.GetSize();
		for(int i = 0; i < count; i++)
		{
            View <T>* pView = m_aViews[i];
			if(pView != (View <T>*)pSender)
				pView->Update(pSender, lHint, pHint);
		}
	}
	
	View<T>* AddView(View<T>* pView)
	{
		pView->SetDocument(static_cast<T*>(this));
		View <T>* pV = static_cast<View <T>*>(pView);
		m_aViews.Add(pV);
		return pView;
	}

	void RemoveView(View<T>* pView)
	{
		int count = m_aViews.GetSize();
		for(int i = 0; i < count; i++)
		{
			if(m_aViews[i] == pView)
			{
				m_aViews.RemoveAt(i);
				pView->SetDocument(NULL);
				break;
			}
		}
	}
	
	int GetNumViews() const
	{
		return m_aViews.GetSize();
	}
	
	View<T>* GetView(const int pos)
	{
		ATLASSERT(pos < m_aViews.GetSize());
		return m_aViews[pos];
	}
	
	BOOL IsModified()
	{
		return m_bModified;
	}

	void SetModifiedFlag(BOOL bModified = TRUE)
	{
		m_bModified = bModified;
	}
	
	TDocTemplate* GetDocTemplate()
	{
		return m_pDocTemplate;
	}
protected:
	CSimpleArray<View<T>*>	m_aViews;
	BOOL					m_bModified;
public:
	TDocTemplate*			m_pDocTemplate;
};

// The Document with Message Map
template <class T, class TDocTemplate = DocTemplateBase, class TDoc = DocumentBase>
class DocumentMsg : public Document<T, TDocTemplate, TDoc>,
					public CMessageMap
{
};

// Abstract base class for Document Templates
class DocTemplateBase
{
public:
	virtual DocumentBase* GetDocument(int pos) = 0;
	virtual DocumentBase* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE) = 0;
	virtual DocumentBase* OpenDocument(BOOL bNew = FALSE) = 0;
	virtual int GetNumDocs() const = 0;
};

// Abstract base class for Document Templates with Message Map
class DocTemplateBaseMsg : public DocTemplateBase,
						public CMessageMap
{
};


// The Document Template
template <class TDoc, class TView, class TFrame, int nID>
class DocTemplate : public DocTemplateBase
{
private:
	CSimpleArray<TDoc*>	m_aDocuments;
public:
	HWND	m_hWndClient;
	~DocTemplate()
	{
		int nDocs = GetNumDocs();
		for(int i = 0; i < nDocs; i++)
		{
			delete m_aDocuments[i];
		}
		m_aDocuments.RemoveAll();
	}

	TFrame* CreateNewFrame(TDoc* pDoc, TFrame* pOther)
	{
		CreateContext <TDoc, TView> context;
		context.m_pCurrentDoc = pDoc;
		context.m_pNewDocTemplate = this;
		TFrame::GetWndClassInfo().m_uCommonResourceID = nID;
		TFrame* pFrame = new TFrame;
		pFrame->CreateEx(m_hWndClient, NULL, NULL, 0, 0, &context);
		return pFrame;
	}
	
    TDoc* CreateNewDocument()
	{
		TDoc* pDoc = new TDoc;
		AddDocument(pDoc);
		return pDoc;
	}
	
	DocumentBase* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE)
	{
        TDoc* pDoc = CreateNewDocument();
		TFrame* pFrame = CreateNewFrame(pDoc, 0);
		if(!pDoc->OnNewDocument())
			pFrame->DestroyWindow();
		return pDoc;
	}

	DocumentBase* GetDocument(int pos)
	{
		return m_aDocuments[pos];
	}

	virtual void AddDocument(TDoc* pDoc)
	{
		pDoc->m_pDocTemplate = this;
		m_aDocuments.Add(pDoc);
	}
	
	virtual TDoc* RemoveDocument(TDoc* pDoc)
	{
		int nDocs = GetNumDocs();
		for(int i = 0; i < nDocs; i++)
		{
			if(m_aDocuments[i] == pDoc)
			{
				m_aDocuments.RemoveAt(i);
				break;
			}
		}
		return pDoc;
	}

	int GetNumDocs() const
	{
		return m_aDocuments.GetSize();
	}
};

// The Document Template with Message Map
template <class TDoc, class TView, class TFrame, int nID>
class DocTemplateMsg : public DocTemplate<TDoc, TView, TFrame, nID>,
					public CMessageMap
{
	BEGIN_MSG_MAP(DocTemplateMsg)
		for(int i=0,int nDocs=GetNumDocs();i<nDocs;i++)
			// May be changed
			((TDoc *)(GetDocument(i)))->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID);
	END_MSG_MAP()
};

// The Document Manager
template <class TMainFrame>
class DocManager
{
public:
	template <class TDocTemplate>
	int AddDocTemplate(TDocTemplate* pDocTemplate)
	{
		ATLASSERT(pDocTemplate != NULL);
		pDocTemplate->m_hWndClient = 
		static_cast<TMainFrame*>(this)->m_hWndClient;
		DocTemplateBase* pDocBase = static_cast<DocTemplateBase*>(pDocTemplate);
		m_aTemplates.Add(pDocBase);
		return m_aTemplates.GetSize() - 1;
	}

	int GetNumTemplates() const
	{
		return m_aTemplates.GetSize();
	}

	DocTemplateBase* GetDocTemplate(const int pos)
	{
		ATLASSERT(pos < m_aTemplates.GetSize());
		return m_aTemplates[pos];
	}
protected:
   CSimpleArray<DocTemplateBase*> m_aTemplates;
};

template <class TMainFrame>
class DocManagerMsg : public DocManager<TMainFrame>,
					public CMessageMap
{
	BEGIN_MSG_MAP(DocManagerMsg)
		for(int i=0,int nDocs=GetNumTemplates();i<nDocs;i++)
			// May be changed
			((DocTemplateMsg *)(GetDocTemplate(i)))->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID);
	END_MSG_MAP()
};

};

/*

// namespace DoView
// View
//  ViewWindow<T, TBase = CWindow, TWinTraits= CControlTraits>
// Document<T>
//  DocumentFrame<T, TBase = CWindow, TWinTraits= CControlTraits>
namespace DocView
{
	class View;
	template <class T, class TBase, class TWinTraits> 
		class ViewWindow;
	class Document;
	template <class T, class TBase, class TWinTraits>
		class DocumentFrame;

// class View
class ATL_NO_VTABLE View
{
public:
	virtual int PreTranslateMessage(struct tagMSG *pMsg) = 0;
	virtual void UpdateView(void *pData) = 0;
	virtual void DestroyView() = 0;
};

// class ViewWindow
template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE ViewWindow : public CWindowImpl<T, TBase, TWinTraits>,
					public View
{ };

// class Document
class ATL_NO_VTABLE Document
{
private:
	// View Array
	CSimpleValArray<View *> m_aView;
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
	const CSimpleValArray<View *> *GetViewArray() const { return &m_aView; }
	
	void AddView(View *pView) { ATLASSERT(pView); m_aView.Add(pView); }
	void DeleteView(int iView) { m_aView[iView]->DestroyView(); 
		delete m_aView[iView]; m_aView.RemoveAt(iView); }
	void ReplaceView(int iView, View *pView) { ATLASSERT(pView); m_aView[iView]=pView; }
};

// class DocumentFrame
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

*/

#ifndef NO_DOCVIEW_NAMESPACE
using namespace DocView;
#endif // NO_DOCVIEW_NAMESPACE

#endif // __DOCVIEW_H__
//
// DocView v0.2.6.4
//

#pragma once

namespace NN
{

template <class TDocument>
class View
{
	TDocument *m_pDocument;
public:
	void SetDocument(TDocument *pDocument)
	{
		m_pDocument=pDocument;
	}
	TDocument *GetDocument()
	{
		return m_pDocument;
	}
	virtual void OnUpdate(View<TDocument>* pSender, void *pData) = 0;
	virtual void OnInitialUpdate() = 0;
//	virtual void OnDestroyView() = 0;
};

template <class T>
class Document
{
	CSimpleValArray<View<T> *> m_aView;
public:
	void UpdateAllViews(View<T>* pSender, void *pData)
	{
		int i,n;
		for(i=0,n=m_aView.GetSize();i<n;i++)
		{
            View <T>* pView = m_aView[i];
			if(pView!=(View <T>*)pSender)
				pView->OnUpdateView(pSender, pData);
		}
	}
	void InitialUpdateAllViews()
	{
		int i,n;
		for(i=0,n=m_aView.GetSize();i<n;i++)
			m_aView[i]->OnInitialUpdate();
	}
	View<T>* AddView(View<T>* pView)
	{
		pView->SetDocument(static_cast<T*>(this));
		View <T>* pV = static_cast<View <T>*>(pView);
		m_aView.Add(pV);
		return pView;
	}
	void RemoveView(View<T>* pView)
	{
		int count = m_aViews.GetSize();
		for(int i = 0; i < count; i++)
		{
			if(m_aViews[i] == pView)
			{
				m_aView.RemoveAt(i);
				pView->SetDocument(NULL);
				break;
			}
		}
	}
	int GetNumViews() const
	{
		return m_aView.GetSize();
	}
	View<T>* GetView(const int pos)
	{
		ATLASSERT(pos < m_aView.GetSize());
		return m_aViews[pos];
	}
};

};
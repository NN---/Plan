// PlanView.h : interface of the CPlanView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANVIEW_H__A369788E_82B8_11D6_8201_D823D40173C3__INCLUDED_)
#define AFX_PLANVIEW_H__A369788E_82B8_11D6_8201_D823D40173C3__INCLUDED_

#include "PlanDoc.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlanView : public CListView
{
protected: // create from serialization only
	CPlanView();
	DECLARE_DYNCREATE(CPlanView)

// Attributes
public:
	CPlanDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlanView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SortItems(int iSubItem,BYTE Sort);
	void AddUpdateEvent(BOOL bAdd);
	void SelectEventItem();
	void SetColumns();
	void InsertAllEvents();
	int FindEventItem(const Event *pEvent,BOOL bCast=FALSE);
	void SetEvent(Event &rEvent,int iItem=-1,BOOL bSave=TRUE);
	void SetCheckItem(int iItem=-1,int nCheck=-1);
	void MoveItem(int iItemFrom,int iItemTo,BOOL bSave=TRUE);
	virtual ~CPlanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CPlanView)
	afx_msg void OnDestroy();
	afx_msg void OnEventAddEvent();
	afx_msg void OnEventUpdateEvent();
	afx_msg void OnEventRemoveEvent();
	afx_msg void OnEventClearProperties();
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolsOptions();
	afx_msg void OnViewEnableDisable();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList *m_pStateImageList;
	BOOL		m_bDragging;
	int			m_iItemDrag;
	int			m_iItemDrop;
	friend void EXPORT CALLBACK Timer(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwCount);
};

#ifndef _DEBUG  // debug version in PlanView.cpp
inline CPlanDoc* CPlanView::GetDocument()
   { return (CPlanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLANVIEW_H__A369788E_82B8_11D6_8201_D823D40173C3__INCLUDED_)

// PlanView.h : interface of the CPlanView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANVIEW_H__B643696A_4CE6_4EC1_980E_EB478B02AAD2__INCLUDED_)
#define AFX_PLANVIEW_H__B643696A_4CE6_4EC1_980E_EB478B02AAD2__INCLUDED_

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
	int GetSelectedEvent();

private:
	enum ColumnStrings
	{
		stringFirstColumn=IDS_EVENT,
		stringLastColumn=IDS_SCHEDULE
	};
	enum Columns
	{
		columnEvent,
		columnDate,
		columnTime,
		columnSchedule
	};
	void SetColumns();
	void SetItems();
	void SetItem(CPlanDoc::Message::Types Action,int iItem,const CMainFrame::Data *pFrameData);
	void AddEvent(CPlanDoc::Message *pMessage);
	void UpdateEvent(CPlanDoc::Message *pMessage);
	void DeleteEvent(CPlanDoc::Message *pMessage);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlanView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPlanView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PlanView.cpp
inline CPlanDoc* CPlanView::GetDocument()
   { return (CPlanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLANVIEW_H__B643696A_4CE6_4EC1_980E_EB478B02AAD2__INCLUDED_)

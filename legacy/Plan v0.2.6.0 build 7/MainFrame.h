// MainFrame.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRAME_H__EC6A3B15_E527_44EA_A5DC_8F15DBB353EB__INCLUDED_)
#define AFX_MAINFRAME_H__EC6A3B15_E527_44EA_A5DC_8F15DBB353EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertiesBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	BOOL m_bFirstTime;

protected:  // control bar embedded members
	CStatusBar   m_wndStatusBar;
	CToolBar     m_wndToolBar;
	CSplitterWnd m_wndSplitter;
	CPropertiesBar m_PropertiesBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewProperties();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnLoadMe(UINT uMsg,WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__EC6A3B15_E527_44EA_A5DC_8F15DBB353EB__INCLUDED_)

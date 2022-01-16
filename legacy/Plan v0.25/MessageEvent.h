#if !defined(AFX_MESSAGEEVENT_H__1AFFC3AE_E45A_4C18_8FBD_FE60BEC47F69__INCLUDED_)
#define AFX_MESSAGEEVENT_H__1AFFC3AE_E45A_4C18_8FBD_FE60BEC47F69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageEvent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageEvent dialog

class CMessageEvent : public CDialog
{
// Construction
public:
	CMessageEvent(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMessageEvent)
	enum { IDD = IDD_MESSAGE };
	CString	m_Caption;
	CString	m_Text;
	//}}AFX_DATA
	void Clear();
	const BOOL IsClear();
	void GetEvent(Message *pMessage);
	void SetEvent(Message *pMessage);
	void DoEvent(Message &rMessage);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageEvent)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEEVENT_H__1AFFC3AE_E45A_4C18_8FBD_FE60BEC47F69__INCLUDED_)

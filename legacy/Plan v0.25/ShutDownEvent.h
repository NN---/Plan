#if !defined(AFX_SHUTDOWNEVENT_H__5663E8A0_AE3B_11D6_8805_85005B53DA5F__INCLUDED_)
#define AFX_SHUTDOWNEVENT_H__5663E8A0_AE3B_11D6_8805_85005B53DA5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShutDownEvent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShutDownEvent dialog

class CShutDownEvent : public CDialog
{
// Construction
public:
	CShutDownEvent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShutDownEvent)
	enum { IDD = IDD_SHUTDOWN };
	int		m_Operation;
	//}}AFX_DATA
	void Clear();
	const BOOL IsClear();
	void GetEvent(ShutDown *pShutDown);
	void SetEvent(ShutDown *pShutDown);
	void DoEvent(ShutDown &rShutDown);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShutDownEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShutDownEvent)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHUTDOWNEVENT_H__5663E8A0_AE3B_11D6_8805_85005B53DA5F__INCLUDED_)

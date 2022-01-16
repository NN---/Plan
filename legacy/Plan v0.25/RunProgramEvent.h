#if !defined(AFX_RUNPROGRAMEVENT_H__84CF5501_B86B_11D6_8805_BF5A53D4AB68__INCLUDED_)
#define AFX_RUNPROGRAMEVENT_H__84CF5501_B86B_11D6_8805_BF5A53D4AB68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunProgramEvent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunProgramEvent dialog

class CRunProgramEvent : public CDialog
{
// Construction
public:
	CRunProgramEvent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRunProgramEvent)
	enum { IDD = IDD_RUNPROGRAM };
	CString	m_Path;
	CString	m_Parameters;
	//}}AFX_DATA
	void GetEvent(RunProgram *pRunProgram);
	void SetEvent(RunProgram *pRunProgram);
	void DoEvent(RunProgram &rRunProgram);
	void Clear();
	const BOOL IsClear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunProgramEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRunProgramEvent)
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNPROGRAMEVENT_H__84CF5501_B86B_11D6_8805_BF5A53D4AB68__INCLUDED_)

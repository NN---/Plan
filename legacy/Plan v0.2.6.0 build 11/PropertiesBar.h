#if !defined(AFX_PROPERTIESBAR_H__46DC51BD_5AFF_43AA_A330_1BA609E5A82D__INCLUDED_)
#define AFX_PROPERTIESBAR_H__46DC51BD_5AFF_43AA_A330_1BA609E5A82D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar dialog

class CPropertiesBar : public CDialogBar
{
// Construction
public:
	CPropertiesBar();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertiesBar)
	enum { IDD = IDD_PROPERTIESBAR };
	CDateTimeCtrl	m_dtcDate;
	CListBox	m_lbDays;
	//}}AFX_DATA

// Overrides
	BOOL Create(CWnd *pParentWnd,UINT nStyle);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesBar)
	protected:
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void OnceDaily(BOOL bOnce);

// Implementation
protected:
	CTime m_Time;
	// Generated message map functions
	//{{AFX_MSG(CPropertiesBar)
	afx_msg void OnOnce();
	afx_msg void OnDaily();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESBAR_H__46DC51BD_5AFF_43AA_A330_1BA609E5A82D__INCLUDED_)

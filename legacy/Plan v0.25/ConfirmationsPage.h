#if !defined(AFX_CONFIRMATIONSPAGE_H__66DC0740_B9C5_11D6_A3A0_F77ECDC36F68__INCLUDED_)
#define AFX_CONFIRMATIONSPAGE_H__66DC0740_B9C5_11D6_A3A0_F77ECDC36F68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfirmationsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfirmationsPage dialog

class CConfirmationsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfirmationsPage)

// Construction
public:
	CConfirmationsPage();
	~CConfirmationsPage();

// Dialog Data
	//{{AFX_DATA(CConfirmationsPage)
	enum { IDD = IDD_CONFIRMATIONS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfirmationsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfirmationsPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIRMATIONSPAGE_H__66DC0740_B9C5_11D6_A3A0_F77ECDC36F68__INCLUDED_)

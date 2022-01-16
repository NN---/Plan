#if !defined(AFX_GENERALPAGE_H__E0F28320_B85C_11D6_8805_9BA4E6B8A205__INCLUDED_)
#define AFX_GENERALPAGE_H__E0F28320_B85C_11D6_8805_9BA4E6B8A205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage dialog

class CGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralPage)

// Construction
public:
	CGeneralPage();
	~CGeneralPage();

// Dialog Data
	//{{AFX_DATA(CGeneralPage)
	enum { IDD = IDD_GENERAL };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGE_H__E0F28320_B85C_11D6_8805_9BA4E6B8A205__INCLUDED_)

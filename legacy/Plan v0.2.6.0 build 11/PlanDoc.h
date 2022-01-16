// PlanDoc.h : interface of the CPlanDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANDOC_H__8F407545_D7C8_4EEB_94F6_CA17DF148207__INCLUDED_)
#define AFX_PLANDOC_H__8F407545_D7C8_4EEB_94F6_CA17DF148207__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlanDoc : public CDocument
{
protected: // create from serialization only
	CPlanDoc();
	DECLARE_DYNCREATE(CPlanDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlanDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPlanDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLANDOC_H__8F407545_D7C8_4EEB_94F6_CA17DF148207__INCLUDED_)

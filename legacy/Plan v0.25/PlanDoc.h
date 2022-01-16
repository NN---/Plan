// PlanDoc.h : interface of the CPlanDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANDOC_H__A369788C_82B8_11D6_8201_D823D40173C3__INCLUDED_)
#define AFX_PLANDOC_H__A369788C_82B8_11D6_8201_D823D40173C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Defines.h"

class CPlanDoc : public CDocument
{
protected: // create from serialization only
	CPlanDoc();
	DECLARE_DYNCREATE(CPlanDoc)

// Attributes
public:
	BYTE Sort;
	BYTE SortColumn;
	BYTE ColumnOrder[NUM_SUB_ITEMS];
	WORD ColumnWidth[NUM_SUB_ITEMS];
	CArray <Event,Event&> aItems;
	CArray <Message,Message&> aMessage;
	CArray <ShutDown,ShutDown&> aShutDown;
	CArray <RunProgram,RunProgram&> aRunProgram;
	
// Operations
public:
	void CannotOpenFile(LPCTSTR lpszPathName);
	void SetModifiedFlag(BOOL);
	BOOL PlanFileCheck(LPCTSTR lpszPathName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlanDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void OnCloseDocument();
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLANDOC_H__A369788C_82B8_11D6_8201_D823D40173C3__INCLUDED_)

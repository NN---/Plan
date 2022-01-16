// PlanDoc.h : interface of the CPlanDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANDOC_H__8F407545_D7C8_4EEB_94F6_CA17DF148207__INCLUDED_)
#define AFX_PLANDOC_H__8F407545_D7C8_4EEB_94F6_CA17DF148207__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrame.h"

typedef CArray<PCEventID, PCEventID> PCEventIDArray;
typedef CArray<LPBYTE,LPBYTE> LPBYTEArray;
typedef CMainFrame::Data FrameData;
typedef CArray<FrameData,FrameData &> FrameDataArray;

class CPlanDoc : public CDocument
{
protected: // create from serialization only
	CPlanDoc();
	DECLARE_DYNCREATE(CPlanDoc)

// Attributes
public:
	const PCEventIDArray &GetaEventID() const;
	const LPBYTEArray &GetaEventData() const;
	const CUIntArray &GetaEventDataSize() const;
	FrameDataArray &GetaFrameData();
	const int GetNumEvents() const;
private:
	PCEventIDArray aID;
	LPBYTEArray aData;
	CUIntArray aDataSize;
	FrameDataArray aFrameData;
	int m_numEvents;
	// static
	static const TCHAR s_strRegPath[];
	static const TCHAR s_strID[];
	static const TCHAR s_strData[];
	static const TCHAR s_strDataSize[];
	static const TCHAR s_strFrameData[];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlanDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void OnCloseDocument();
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
	afx_msg void OnEventAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLANDOC_H__8F407545_D7C8_4EEB_94F6_CA17DF148207__INCLUDED_)

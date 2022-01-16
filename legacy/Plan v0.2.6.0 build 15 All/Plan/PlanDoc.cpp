// PlanDoc.cpp : implementation of the CPlanDoc class
//

#include "stdafx.h"
#include "Plan.h"

#include "PlanDoc.h"

#include "MainFrame.h"
#include "PropertiesBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc

IMPLEMENT_DYNCREATE(CPlanDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlanDoc, CDocument)
	//{{AFX_MSG_MAP(CPlanDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc construction/destruction

CPlanDoc::CPlanDoc()
{
	// TODO: add one-time construction code here

}

CPlanDoc::~CPlanDoc()
{
}

BOOL CPlanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	return OnOpenDocument(NULL);
}

BOOL CPlanDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	/*if(!CheckPlanFile(lpszPathName))
		return FALSE;
	*/
	if(lpszPathName)
		return CDocument::OnOpenDocument(lpszPathName);
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CPlanDoc serialization

void CPlanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc diagnostics

#ifdef _DEBUG
void CPlanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPlanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc commands

void CPlanDoc::Add()
{
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();
	aID.Add(pApp->m_EventLib.GetEventID());
	char p[500];
	pApp->m_EventLib.Get((LPBYTE)p);
}

void CPlanDoc::Remove(int /*iItem*/)
{
}

void CPlanDoc::Update(int /*iItem*/)
{
}
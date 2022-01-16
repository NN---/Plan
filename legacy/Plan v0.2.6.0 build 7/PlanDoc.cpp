// PlanDoc.cpp : implementation of the CPlanDoc class
//

#include "stdafx.h"
#include "Plan.h"

#include "PlanDoc.h"

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

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

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

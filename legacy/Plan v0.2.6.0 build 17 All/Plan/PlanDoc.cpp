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
	
	DWORD i;
	RegKey regEvents,regEvent;
	regEvents.Open(HKEY_CURRENT_USER,_T("Software\\NN\\Plan\\Events"));
	if(!(HKEY)regEvents)
		return TRUE;
	
	unsigned iEvent;
	DWORD n;
	EventID ID;
	UINT uSize;
	DWORD dwSize;
	DWORD dwType;
	TCHAR szEvent[5];
	LPBYTE pData;
	regEvents.QueryInfoKey(NULL,NULL,(LPDWORD)&n);

	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->m_EventLib;

	for(i=0;i<n;i++)
	{
		wsprintf(szEvent,_T("%d"),i);
		if(regEvent.Open(regEvents,szEvent))
			continue;

		dwSize=sizeof(EventID);
		dwType=REG_BINARY;
		regEvent.QueryValue((LPBYTE)&ID,_T("ID"),&dwSize,&dwType);
		if(dwSize!=sizeof(EventID)||dwType!=REG_BINARY)
			continue;
		
		iEvent=rEventLib.Find(&ID);
		if(iEvent==(unsigned)-1)
			continue;

		uSize=0;
		regEvent.QueryValue((DWORD &)uSize,_T("Size"));
		if(!uSize)
			continue;

		pData=new BYTE[uSize];
		dwSize=uSize;
		dwType=REG_BINARY;
		regEvent.QueryValue(pData,_T("Data"),&dwSize,&dwType);
		if(dwSize!=uSize||dwType!=REG_BINARY)
		{
			delete [] pData;
			continue;
		}
		rEventLib.SetWorkEvent(iEvent);
		aID.Add(rEventLib.GetEventID());
		aDataSize.Add(uSize);
		aData.Add(pData);
	}

	return TRUE;
}

BOOL CPlanDoc::OnOpenDocument(LPCTSTR /*lpszPathName*/) 
{
	/*if(!CheckPlanFile(lpszPathName))
		return FALSE;
	*/
	
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
	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->m_EventLib;
	aID.Add(rEventLib.GetEventID());
	int size=rEventLib.DataSize();
	aDataSize.Add(size);
	LPBYTE p;
	p=new BYTE[size];
	rEventLib.GetData(p);
	aData.Add(p);
}

void CPlanDoc::Remove(int /*iItem*/)
{
}

void CPlanDoc::Update(int /*iItem*/)
{
}

void CPlanDoc::DeleteContents() 
{
	aID.RemoveAll();
	int i,n;
	for(i=0,n=aData.GetUpperBound();i<=n;i++)
		delete aData[i];
	aData.RemoveAll();
	aDataSize.RemoveAll();
	
	CDocument::DeleteContents();
}

void CPlanDoc::OnCloseDocument() 
{
	DWORD i,n=aData.GetUpperBound();
	if(n!=-1)
	{
		RegKey regEvents,regEvent;
		regEvents.Create(HKEY_CURRENT_USER,_T("Software\\NN\\Plan\\Events"));
		TCHAR szEvent[5];
		for(i=0;i<=n;i++)
		{
			wsprintf(szEvent,_T("%d"),i);
			regEvent.Create(regEvents,szEvent);
			regEvent.SetValue((const LPBYTE)aID[i],sizeof(EventID),_T("ID"));
			regEvent.SetValue(aDataSize[i],_T("Size"));
			regEvent.SetValue((const LPBYTE)aData[i],aDataSize[i],_T("Data"));
		}
	}
	CDocument::OnCloseDocument();
}

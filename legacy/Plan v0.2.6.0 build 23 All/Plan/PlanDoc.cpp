// PlanDoc.cpp : implementation of the CPlanDoc class
//

#include "stdafx.h"
#include "Plan.h"

#include "PlanDoc.h"

#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc

const TCHAR CPlanDoc::s_strID[]=_T("ID");
const TCHAR CPlanDoc::s_strData[]=_T("Data");
const TCHAR CPlanDoc::s_strDataSize[]=_T("DataSize");
const TCHAR CPlanDoc::s_strFrameData[]=_T("FrameData");
const TCHAR CPlanDoc::s_strRegPath[]=_T("Software\\NN\\Plan\\Events");

IMPLEMENT_DYNCREATE(CPlanDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlanDoc, CDocument)
	//{{AFX_MSG_MAP(CPlanDoc)
	ON_COMMAND(ID_EVENT_ADD, OnEventAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc construction/destruction

CPlanDoc::CPlanDoc()
{

}

CPlanDoc::~CPlanDoc()
{
}

BOOL CPlanDoc::OnNewDocument()
{
	if(!CDocument::OnNewDocument())
		return FALSE;
	
	RegKey regEvents,regEvent;
	regEvents.Create(HKEY_CURRENT_USER,s_strRegPath);
	if(!regEvents.GetHandle())
		return TRUE;

	DWORD dwSize=3;
	TCHAR szEvent[4]="";

	if(regEvents.EnumKey(0,szEvent,&dwSize)!=ERROR_SUCCESS)
		return TRUE;

	DWORD i;
	unsigned iEvent;
	DWORD n;
	EventID ID;
	UINT uSize;
	FrameData framedata;
	
	DWORD dwType;
	
	LPBYTE pData;
	LONG lRes;
	regEvents.QueryInfoKey(NULL,NULL,(LPDWORD)&n);

	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();

	for(i=0;i<n;i++)
	{
		// Open
		dwSize=3;
		regEvents.EnumKey(i,szEvent,&dwSize);
		lRes=regEvent.Open(regEvents,szEvent);
		if(lRes!=ERROR_SUCCESS)
			continue;

		// ID
		dwSize=sizeof(EventID);
		dwType=REG_BINARY;
		lRes=regEvent.QueryValue((LPBYTE)&ID,s_strID,&dwSize,&dwType);
		if(lRes!=ERROR_SUCCESS)
			continue;
		if(dwSize!=sizeof(EventID)||dwType!=REG_BINARY)
			continue;

		// Find ID
		iEvent=rEventLib.Find(&ID);
		if(iEvent==(unsigned)-1)
			continue;

		// FrameData
		dwSize=sizeof(FrameData);
		dwType=REG_BINARY;
		regEvent.QueryValue((LPBYTE)&framedata,s_strFrameData,&dwSize,&dwType);
		if(lRes!=ERROR_SUCCESS)
			continue;
		if(dwSize!=sizeof(FrameData)||dwType!=REG_BINARY)
			continue;
	
		// Size
		uSize=0;
		lRes=regEvent.QueryValue((DWORD &)uSize,s_strDataSize);
		if(lRes!=ERROR_SUCCESS)
			continue;
		if(!uSize)
			continue;

		// Data
		pData=new BYTE[uSize];
		if(!pData)
			continue;
		dwSize=uSize;
		dwType=REG_BINARY;
		lRes=regEvent.QueryValue(pData,s_strData,&dwSize,&dwType);
		if(lRes!=ERROR_SUCCESS)
			continue;
		if(dwSize!=uSize||dwType!=REG_BINARY)
		{
			delete [] pData;
			continue;
		}
		
		// Add this Event
		rEventLib.SetWorkEvent(iEvent);
		aID.Add(rEventLib.GetEventID());
		aFrameData.Add(framedata);
		aDataSize.Add(uSize);
		aData.Add(pData);
	}

	m_numEvents=aID.GetSize();

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

void CPlanDoc::DeleteContents() 
{
	aID.RemoveAll();
	aFrameData.RemoveAll();
	int i,n;
	for(i=0,n=aData.GetUpperBound();i<=n;i++)
		delete aData[i];
	aData.RemoveAll();
	aDataSize.RemoveAll();
	m_numEvents=0;
	
	CDocument::DeleteContents();
}

void CPlanDoc::OnCloseDocument() 
{
	DWORD i,n=aData.GetSize();
	if(n!=-1)
	{
		RegKey regEvents,regEvent;
		TCHAR szEvent[5];
		regEvents.Create(HKEY_CURRENT_USER,s_strRegPath);
		regEvents.DeleteAllSubKeys();
		for(i=0;i<n;i++)
		{
			wsprintf(szEvent,_T("%d"),i);
			regEvent.Create(regEvents,szEvent);
			regEvent.SetValue((const LPBYTE)aID[i],sizeof(EventID),s_strID);
			regEvent.SetValue((const LPBYTE)&aFrameData[i],sizeof(FrameData),s_strFrameData);
			regEvent.SetValue(aDataSize[i],s_strDataSize);
			regEvent.SetValue((const LPBYTE)aData[i],aDataSize[i],s_strData);
		}
	}
	CDocument::OnCloseDocument();
}

const PCEventIDArray &CPlanDoc::GetaEventID() const
{
	return aID;
}

const LPBYTEArray &CPlanDoc::GetaEventData() const
{
	return aData;
}

const CUIntArray &CPlanDoc::GetaEventDataSize() const
{
	return aDataSize;
}

FrameDataArray &CPlanDoc::GetaFrameData()
{
	return aFrameData;
}

const int CPlanDoc::GetNumEvents() const
{
	return m_numEvents;
}

void CPlanDoc::OnEventAdd() 
{
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();
	CMainFrame *pFrame=(CMainFrame *)AfxGetMainWnd();
	EventLib &rEventLib=pApp->GetEventLib();

	FrameData framedata;
	LPBYTE pData;
	UINT uSize;

	if(rEventLib.IsEmpty())
	{
		pApp->Error(IDE_EMPTY);
		return;
	}

	pFrame->GetData(framedata);
	uSize=rEventLib.GetDataSize();
	pData=new BYTE[uSize];
	if(!pData)
	{
		pApp->Error(IDE_UNKNOWN);
		return;
	}
	rEventLib.GetData(pData);
	aFrameData.Add(framedata);
	aData.Add(pData);
	aDataSize.Add(uSize);
	aID.Add(rEventLib.GetEventID());
	m_numEvents++;
	
	UpdateAllViews(NULL);
}

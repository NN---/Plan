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
	
	RegKey regEvents,regEvent;
	regEvents.Open(HKEY_CURRENT_USER,_T("Software\\NN\\Plan\\Events"));
	if(!(HKEY)regEvents)
		return TRUE;
	
	DWORD i;
	unsigned iEvent;
	DWORD n;
	EventID ID;
	UINT uSize;
	CTime time;
	Schedule schedule;
	DWORD dwSize;
	DWORD dwType;
	TCHAR szEvent[4]="";
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
		if(lRes)
			continue;

		// ID
		dwSize=sizeof(EventID);
		dwType=REG_BINARY;
		lRes=regEvent.QueryValue((LPBYTE)&ID,_T("ID"),&dwSize,&dwType);
		if(lRes)
			continue;
		if(dwSize!=sizeof(EventID)||dwType!=REG_BINARY)
			continue;
		
		// Find ID
		iEvent=rEventLib.Find(&ID);
		if(iEvent==(unsigned)-1)
			continue;

		// Time
		dwSize=sizeof(CTime);
		dwType=REG_BINARY;
		lRes=regEvent.QueryValue((LPBYTE)&time,_T("Time"),&dwSize,&dwType);
		if(lRes)
			continue;
		if(dwSize!=sizeof(CTime)||dwType!=REG_BINARY)
			continue;

		// Schedule
		dwSize=sizeof(Schedule);
		dwType=REG_BINARY;
		lRes=regEvent.QueryValue((LPBYTE)&schedule,_T("Schedule"),&dwSize,&dwType);
		if(lRes)
			continue;
		if(dwSize!=sizeof(Schedule)||dwType!=REG_BINARY)
			continue;

		// Size
		uSize=0;
		lRes=regEvent.QueryValue((DWORD &)uSize,_T("Size"));
		if(lRes)
			continue;
		if(!uSize)
			continue;

		// Data
		pData=new BYTE[uSize];
		dwSize=uSize;
		dwType=REG_BINARY;
		lRes=regEvent.QueryValue(pData,_T("Data"),&dwSize,&dwType);
		if(lRes)
			continue;
		if(dwSize!=uSize||dwType!=REG_BINARY)
		{
			delete [] pData;
			continue;
		}
		
		// Add this Event
		rEventLib.SetWorkEvent(iEvent);
		aID.Add(rEventLib.GetEventID());
		aTime.Add(time);
		aSchedule.Add(schedule);
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

void CPlanDoc::DeleteContents() 
{
	aID.RemoveAll();
	aTime.RemoveAll();
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
			regEvent.SetValue((const LPBYTE)&aTime[i],sizeof(CTime),_T("Time"));
			regEvent.SetValue((const LPBYTE)&aSchedule[i],sizeof(Schedule),_T("Schedule"));
			regEvent.SetValue(aDataSize[i],_T("Size"));
			regEvent.SetValue((const LPBYTE)aData[i],aDataSize[i],_T("Data"));
		}
	}
	CDocument::OnCloseDocument();
}

APCEventID &CPlanDoc::GetaID()
{
	return aID;
}

ALPBYTE &CPlanDoc::GetaData()
{
	return aData;
}

AUINT &CPlanDoc::GetaDataSize()
{
	return aDataSize;
}

ACTime &CPlanDoc::GetaTime()
{
	return aTime;
}

ASchedule &CPlanDoc::GetaSchedule()
{
	return aSchedule;
}
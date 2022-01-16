// PlanDoc.cpp : implementation of the CPlanDoc class
//

#include "stdafx.h"
#include "Plan.h"

#include "PlanDoc.h"
#include "MainFrame.h"

#include "RegKey.h"			// RegKey class

//
// Change
//
#include "PlanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc

// static members
const TCHAR CPlanDoc::s_strID[]=_T("ID");
const TCHAR CPlanDoc::s_strData[]=_T("Data");
const TCHAR CPlanDoc::s_strDataSize[]=_T("DataSize");
const TCHAR CPlanDoc::s_strFrameData[]=_T("FrameData");
const TCHAR CPlanDoc::s_strRegPath[]=_T("Software\\NN\\Plan\\Events");

IMPLEMENT_DYNCREATE(CPlanDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlanDoc, CDocument)
	//{{AFX_MSG_MAP(CPlanDoc)
	ON_COMMAND(ID_EVENT_ADD, OnEventAdd)
	ON_COMMAND(ID_EVENT_DELETE, OnEventDelete)
	ON_COMMAND(ID_EVENT_UPDATE, OnEventUpdate)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_VIEW_ENABLEDISABLE, OnViewEnableDisable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc construction/destruction

CPlanDoc::CPlanDoc()
{
	m_numEvents=0;
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
	{
		if(((CPlanApp *)AfxGetApp())->m_Options.bEnabled)
			AfxGetMainWnd()->SetTimer((UINT)this,TIMER_DELAY,&CPlanDoc::Timer);

		UpdateTitle();
		return TRUE;
	}

	DWORD dwSize=3;
	TCHAR szEvent[4]="";

	if(regEvents.EnumKey(0,szEvent,&dwSize)!=ERROR_SUCCESS)
	{
		if(((CPlanApp *)AfxGetApp())->m_Options.bEnabled)
			AfxGetMainWnd()->SetTimer((UINT)this,TIMER_DELAY,&CPlanDoc::Timer);

		UpdateTitle();
		return TRUE;
	}

	DWORD i;
	unsigned iEvent;
	DWORD n;
	EventID ID;
	UINT uSize;
	CMainFrame::Data framedata;
	
	DWORD dwType;
	
	LPBYTE pData;
	LONG lRes;
	regEvents.QueryInfoKey(NULL,NULL,&n);

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
		dwSize=sizeof(CMainFrame::Data);
		dwType=REG_BINARY;
		regEvent.QueryValue((LPBYTE)&framedata,s_strFrameData,&dwSize,&dwType);
		if(lRes!=ERROR_SUCCESS)
			continue;
		if(dwSize!=sizeof(CMainFrame::Data)||dwType!=REG_BINARY)
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

	if(((CPlanApp *)AfxGetApp())->m_Options.bEnabled)
		AfxGetMainWnd()->SetTimer((UINT)this,TIMER_DELAY,&CPlanDoc::Timer);

	UpdateTitle();

	return TRUE;
}

BOOL CPlanDoc::OnOpenDocument(LPCTSTR /*lpszPathName*/) 
{
	/*if(!CheckPlanFile(lpszPathName))
		return FALSE;
	*/
	ShowError(IDE_UNKNOWN);
	OnNewDocument();
	
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
	RegKey regEvents,regEvent;
	TCHAR szEvent[5];
	regEvents.Create(HKEY_CURRENT_USER,s_strRegPath);
	regEvents.DeleteAllSubKeys();
	
	for(i=0;i<n;i++)
	{
		wsprintf(szEvent,_T("%d"),i);
		regEvent.Create(regEvents,szEvent);
		regEvent.SetValue((const LPBYTE)aID[i],sizeof(EventID),s_strID);
		regEvent.SetValue((const LPBYTE)&aFrameData[i],sizeof(CMainFrame::Data),s_strFrameData);
		regEvent.SetValue(aDataSize[i],s_strDataSize);
		regEvent.SetValue((const LPBYTE)aData[i],aDataSize[i],s_strData);
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
	CMainFrame *pFrame=(CMainFrame *)AfxGetMainWnd();
	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();

	CMainFrame::Data framedata;
	LPBYTE pData;
	UINT uSize;

	if(rEventLib.IsEmpty())
	{
		ShowError(IDE_EMPTY);
		return;
	}

	pFrame->GetData(framedata);
	
	if(!framedata.m_Schedule.Once && framedata.m_Schedule.IsNoDays())
	{
		ShowError(IDE_UNKNOWN);
		return;
	}

	uSize=rEventLib.GetDataSize();
	pData=new BYTE[uSize];
	if(!pData)
	{
		ShowError(IDE_UNKNOWN);
		return;
	}
	rEventLib.GetData(pData);
	aData.Add(pData);
	aDataSize.Add(uSize);
	aID.Add(rEventLib.GetEventID());
	aFrameData.Add(framedata);
	
	Message message={Message::Types::AddEvent,m_numEvents};
	UpdateAllViews(NULL,(LPARAM)&message);

	m_numEvents++;
}

void CPlanDoc::OnEventDelete() 
{
	//
	// Replace CPlanView with smth else
	//
	int iSelEvent=((CPlanView *)((CMainFrame *)AfxGetMainWnd())->GetActiveView())->GetSelectedEvent();
	if(iSelEvent==-1)
	{
		ShowError(IDE_UNKNOWN);
		return;
	}

	aData.RemoveAt(iSelEvent);
	aDataSize.RemoveAt(iSelEvent);
	aID.RemoveAt(iSelEvent);
	aFrameData.RemoveAt(iSelEvent);

	Message message={Message::Types::DeleteEvent,iSelEvent};
	UpdateAllViews(NULL,(LPARAM)&message);

	if(m_numEvents)
		m_numEvents--;
}

void CPlanDoc::OnEventUpdate() 
{
	CMainFrame *pFrame=(CMainFrame *)AfxGetMainWnd();
	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();

	CMainFrame::Data framedata;
	LPBYTE pData;
	UINT uSize;

	//
	// Replace CPlanView with smth else
	//
	int iSelEvent=((CPlanView *)((CMainFrame *)AfxGetMainWnd())->GetActiveView())->GetSelectedEvent();
	if(iSelEvent==-1)
	{
		ShowError(IDE_UNKNOWN);
		return;
	}

	if(rEventLib.IsEmpty())
	{
		ShowError(IDE_EMPTY);
		return;
	}

	pFrame->GetData(framedata);
	//
	// Schedule not supported. yet...
	//
	if(!framedata.m_Schedule.Once)
	{
		ShowError(IDE_UNKNOWN);
		return;
	}
	// Find framedata
	int i;
	for(i=0;i<m_numEvents;i++)
	{
		if(aFrameData[i]==framedata)
			break;
	}
	if(i!=m_numEvents)
	{
		ShowError(IDE_UNKNOWN);
		return;
	}

	uSize=rEventLib.GetDataSize();
	delete aData[iSelEvent];
	pData=new BYTE[uSize];
	if(!pData)
	{
		ShowError(IDE_UNKNOWN);
		aDataSize.RemoveAt(iSelEvent);
		aID.RemoveAt(iSelEvent);
		aFrameData.RemoveAt(iSelEvent);
		return;
	}
	rEventLib.GetData(pData);
	aData[iSelEvent]=pData;
	aDataSize[iSelEvent]=uSize;
	aID[iSelEvent]=rEventLib.GetEventID();
	aFrameData[iSelEvent]=framedata;
	
	Message message={Message::Types::UpdateEvent,iSelEvent};
	UpdateAllViews(NULL,(LPARAM)&message);

	m_numEvents++;
}

void CPlanDoc::SelectEvent(int iEvent)
{
	((CMainFrame *)AfxGetMainWnd())->SetData(aFrameData[iEvent],aID[iEvent],aData[iEvent]);
}

/*	Structure of file .pln 
	----------------------
	NumEvents
	[Event]
		ID
		FrameData
		DataSize
		Data
*/
void CPlanDoc::OnFileExport() 
{
	if(!m_numEvents)
	{
		ShowError(IDE_UNKNOWN);
		return;
	}
	
	CString strExt;
	CString strFilter;
	GetDocTemplate()->GetDocString(strExt,CDocTemplate::filterExt);
	GetDocTemplate()->GetDocString(strFilter,CDocTemplate::filterName);
	strFilter+=_T("|*");
	strFilter+=strExt;
	strFilter+=_T("||\0");
	strExt.TrimLeft(_T("."));

	CFileDialog fd(FALSE,strExt,NULL,OFN_HIDEREADONLY |	OFN_OVERWRITEPROMPT
		| OFN_CREATEPROMPT,strFilter);
	if(fd.DoModal()==IDCANCEL)
		return;

	CFile file;
	if(!file.Open(fd.GetPathName(),CFile::modeCreate | CFile::modeWrite |
		CFile::shareExclusive))
	{
		ShowError(IDE_UNKNOWN);
		return;
	}

	file.SetLength(0);

	file.Write(&m_numEvents,sizeof(int));
	int i;
	UINT DataSize;
	for(i=0;i<m_numEvents;i++)
	{
		file.Write(aID[i],sizeof(EventID));
		file.Write(&aFrameData[i],sizeof(CMainFrame::Data));
		DataSize=aDataSize[i];
		file.Write(&DataSize,sizeof(UINT));
		file.Write(aData[i],DataSize);
	}
}

/*	Structure of file .pln 
	----------------------
	NumEvents
	[Event]
		ID
		FrameData
		DataSize
		Data
*/
void CPlanDoc::OnFileImport() 
{
	CString strExt;
	CString strFilter;
	GetDocTemplate()->GetDocString(strExt,CDocTemplate::filterExt);
	GetDocTemplate()->GetDocString(strFilter,CDocTemplate::filterName);
	strFilter+=_T("|*");
	strFilter+=strExt;
	strFilter+=_T("||\0");
	strExt.TrimLeft(_T("."));

	CFileDialog fd(TRUE,strExt,NULL,OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
		OFN_FILEMUSTEXIST,strFilter);
	if(fd.DoModal()==IDCANCEL)
		return;

	CFile file;
	if(!file.Open(fd.GetPathName(),CFile::modeRead |
		CFile::shareExclusive))
	{
		ShowError(IDE_UNKNOWN);
		return;
	}
	
	int numEvents;
	file.Read(&numEvents,sizeof(int));
	if(!numEvents)
		return;

	int i;
	int nEvents=m_numEvents;
	EventID eventID;
	CMainFrame::Data framedata;
	UINT uSize;
	LPBYTE pData;
	EventLib &rEventLib=((CPlanApp *)AfxGetApp())->GetEventLib();
	Message message={Message::AddEvent,0};
	for(i=0;i<numEvents;i++)
	{
		file.Read(&eventID,sizeof(EventID));
		if(rEventLib.Find(&eventID,TRUE)==-1)
			continue;

		file.Read(&framedata,sizeof(CMainFrame::Data));

		if(m_numEvents)
		{
			int j;
			for(j=0;j<m_numEvents;j++)
			{
				if(aFrameData[j]==framedata)
					break;
			}
			if(j!=m_numEvents)
				continue;
		}

		file.Read(&uSize,sizeof(UINT));
		pData=new BYTE [uSize];
		file.Read(pData,uSize);

		aID.Add(rEventLib.GetEventID());
		aFrameData.Add(framedata);
		aDataSize.Add(uSize);
		aData.Add(pData);

		message.iEvent=nEvents;
		UpdateAllViews(NULL,(LPARAM)&message);
		nEvents++;
	}	
}

void CPlanDoc::OnViewEnableDisable() 
{
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();
	CWnd *pMainWnd=AfxGetMainWnd();
	
	if(((CPlanApp *)AfxGetApp())->m_Options.bEnabled)
		pMainWnd->KillTimer((UINT)this);
	else
		pMainWnd->SetTimer((UINT)this,TIMER_DELAY,&CPlanDoc::Timer);
	
	pApp->m_Options.bEnabled=!pApp->m_Options.bEnabled;

	UpdateTitle();
}

void CPlanDoc::UpdateTitle()
{
	CString strTitle;
	CString strEnabledDisabled;
	
	GetDocTemplate()->GetDocString(strTitle,CDocTemplate::windowTitle);
	
	strEnabledDisabled.LoadString(((CPlanApp *)AfxGetApp())->m_Options.bEnabled?
		IDS_ENABLED:IDS_DISABLED);

	strTitle+=_T(" - ");
	strTitle+=strEnabledDisabled;

	AfxGetMainWnd()->SetWindowText(strTitle);
}

void WINAPI CPlanDoc::Timer(HWND /*hWnd*/, UINT /*nMsg*/, UINT nIDEvent, DWORD /*dwCount*/)
{
	CPlanDoc *pDoc=(CPlanDoc *)nIDEvent;
	if(!pDoc->m_numEvents)
		return;
	CTime curtime=CTime::GetCurrentTime();
	SYSTEMTIME stcurtime,stframedata;
	curtime.GetAsSystemTime(stcurtime);
	CTimeSpan timespan;
	CMainFrame::Data *pFrameData=NULL;
	int i;
	for(i=0;i<pDoc->m_numEvents;i++)
	{
		pFrameData=&pDoc->aFrameData[i];
		if(pFrameData->m_Schedule.Once)
		{
			timespan=pFrameData->m_Time-curtime;
			if(timespan.GetTotalSeconds()>=0&&
				timespan.GetTotalSeconds()<TIMER_DELAY/1000)
			{
				((CPlanApp *)AfxGetApp())->GetEventLib().Do((const LPBYTE)pDoc->aData[i]);
				break;
			}
		}
		else
		{
			if(!(1<<stcurtime.wDayOfWeek & pFrameData->m_Schedule.GetDayOfWeek()))
				continue;
			pFrameData->m_Time.GetAsSystemTime(stframedata);
			timespan=CTimeSpan(0,stcurtime.wHour - stframedata.
				wHour,stcurtime.wMinute-stframedata.wMinute,
				stcurtime.wSecond-stframedata.wSecond);
			if(timespan.GetTotalSeconds()>=0&&
				timespan.GetTotalSeconds()<TIMER_DELAY/1000)
			{
				((CPlanApp *)AfxGetApp())->GetEventLib().Do((const LPBYTE)pDoc->aData[i]);
				break;
			}
		}
	}
}
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

LRESULT CALLBACK NewFileDlg(HWND hDlg,UINT nMsg,WPARAM wParam,LPARAM /*lParam*/);

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc

IMPLEMENT_DYNCREATE(CPlanDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlanDoc, CDocument)
	//{{AFX_MSG_MAP(CPlanDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc construction/destruction

CPlanDoc::CPlanDoc()
{
	int i;
	for(i=0;i<NUM_SUB_ITEMS;i++)
	{
		*(ColumnOrder+i)=(BYTE)i;
		*(ColumnWidth+i)=0;
	}
	Sort=NOSORT;
}

CPlanDoc::~CPlanDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPlanDoc serialization

void CPlanDoc::Serialize(CArchive& ar)
{
	TCHAR str[PLAN_FILE_LEN+1];
	BYTE nItems;
	BYTE nMessage;
	BYTE nShutDown;
	BYTE nRunProgram;
	int i;

	if (ar.IsStoring())
	{
		lstrcpy(str,PLAN_FILE);

		ar.WriteString(str);
		
		ar.Write(ColumnOrder,NUM_SUB_ITEMS);
		ar.Write(ColumnWidth,NUM_SUB_ITEMS*sizeof(WORD));

		ar<<Sort;
		ar<<SortColumn;

		nItems=(BYTE)aItems.GetSize();
		nMessage=(BYTE)aMessage.GetSize();
		nShutDown=(BYTE)aShutDown.GetSize();
		nRunProgram=(BYTE)aRunProgram.GetSize();

		ar<<nItems;
		ar<<nMessage;
		ar<<nShutDown;
		ar<<nRunProgram;

		for(i=0;i<nItems;i++)
		{
			ar<<aItems[i].Type();
			ar<<aItems[i].State();
		}

		for(i=0;i<nMessage;i++)
			ar.Write(&aMessage[i],sizeof(Message));
		
		for(i=0;i<nShutDown;i++)
			ar.Write(&aShutDown[i],sizeof(ShutDown));

		for(i=0;i<nRunProgram;i++)
			ar.Write(&aRunProgram[i],sizeof(RunProgram));
		
		ar.Flush();

		TCHAR CurDir[_MAX_PATH];
		GetCurrentDirectory(_MAX_PATH,CurDir);
		AfxGetApp()->WriteProfileString(SETTINGS,CURRENTDIRECTORY,CurDir);
	}
	else
	{
		ar.Flush();
		
		ar.ReadString(str,PLAN_FILE_LEN);
		if(lstrcmp(str,PLAN_FILE))
		{
			CannotOpenFile(m_strPathName);
			return;
		}

		ar.Read(ColumnOrder,NUM_SUB_ITEMS);
		ar.Read(ColumnWidth,NUM_SUB_ITEMS*sizeof(WORD));
		
		ar>>Sort;
		ar>>SortColumn;

		ar>>nItems;
		ar>>nMessage;
		ar>>nShutDown;
		ar>>nRunProgram;
		
		aItems.SetSize(nItems);
		for(i=0;i<nItems;i++)
		{
			ar>>aItems[i].Type();
			ar>>aItems[i].State();
		}
		
		aMessage.SetSize(nMessage);
		for(i=0;i<nMessage;i++)
			ar.Read(&aMessage[i],sizeof(Message));
		
		aShutDown.SetSize(nShutDown);
		for(i=0;i<nShutDown;i++)
			ar.Read(&aShutDown[i],sizeof(ShutDown));

		aRunProgram.SetSize(nRunProgram);
		for(i=0;i<nRunProgram;i++)
			ar.Read(&aRunProgram[i],sizeof(RunProgram));
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

BOOL CPlanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CString strPathName;
	CString strUntitled;
				
	strPathName=AfxGetApp()->GetProfileString(SETTINGS,CURRENTDIRECTORY,_T(""));
	
	if(strPathName==_T(""))
	{
		GetCurrentDirectory(_MAX_PATH,strPathName.GetBuffer(_MAX_PATH));
		strPathName.ReleaseBuffer();
	}
	strPathName+=_T("\\");
		
	strUntitled.LoadString(AFX_IDS_UNTITLED);
	strPathName+=strUntitled;
	strUntitled.Empty();

	CString strCurDoc=AfxGetApp()->GetProfileString(SETTINGS,CURRENTDOCUMENT,_T(""));
	if(strCurDoc==_T(""))
	{
		int nRet;
		for(;;)
		{
			nRet=DialogBox(AfxGetInstanceHandle(),(LPCTSTR)IDD_NEWFILE,NULL,(DLGPROC)NewFileDlg);
			if(nRet==IDCANCEL)
				exit(1);
			else
			{
				if(!AfxGetApp()->m_pDocManager->DoPromptFileName(strPathName,
					nRet==ID_FILE_NEW?AFX_IDS_SAVEFILE:AFX_IDS_OPENFILE,
					nRet==ID_FILE_NEW?OFN_OVERWRITEPROMPT|OFN_CREATEPROMPT|OFN_HIDEREADONLY:OFN_HIDEREADONLY,
					nRet==ID_FILE_NEW?FALSE:TRUE,NULL))
					continue;

				if(nRet==ID_FILE_NEW)
					OnSaveDocument(strPathName);
				
				m_strPathName=strPathName;
				return OnOpenDocument(strPathName);
			}
		}
	}
	
	if(!AfxGetApp()->m_pDocManager->DoPromptFileName(strPathName,
		AFX_IDS_SAVEFILE,OFN_OVERWRITEPROMPT|OFN_CREATEPROMPT|
		OFN_HIDEREADONLY,FALSE,NULL))
	{
		m_strPathName=strCurDoc;
		return FALSE;
	}

	OnSaveDocument(strPathName);
	m_strPathName=strPathName;
	return OnOpenDocument(strPathName);
}

BOOL CPlanDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if(!PlanFileCheck(lpszPathName))
	{
		CannotOpenFile(lpszPathName);
		AfxGetApp()->WriteProfileString(SETTINGS,CURRENTDOCUMENT,_T(""));
		return OnNewDocument();
	}
	
	if(CDocument::OnOpenDocument(lpszPathName))
	{
		AfxGetApp()->WriteProfileString(SETTINGS,CURRENTDOCUMENT,lpszPathName);
		if(((CPlanApp *)AfxGetApp())->m_Options.BackupFile)
		{
			CString strPathName=lpszPathName;
			strPathName+=BACKUPFILE_EXTENTION;
			DeleteFile(strPathName);
			CopyFile(lpszPathName,strPathName,FALSE);
		}
		SetPathName(lpszPathName);
		return TRUE;
	}
	
	return FALSE;
}

void CPlanDoc::DeleteContents() 
{
	int i;
	for(i=0;i<NUM_SUB_ITEMS;i++)
	{
		*(ColumnOrder+i)=(BYTE)i;
		*(ColumnWidth+i)=0;
	}
	Sort=NOSORT;

	aItems.RemoveAll();
	aMessage.RemoveAll();
	aShutDown.RemoveAll();
	aRunProgram.RemoveAll();

	CDocument::DeleteContents();
}

void CPlanDoc::SetModifiedFlag(BOOL)
{
	CDocument::SetModifiedFlag(FALSE);
}

void CPlanDoc::CannotOpenFile(LPCTSTR lpszPathName)
{
	CString strText;
	
	strText.LoadString(IDS_CANNOTOPENFILE);
	strText+=_T(" ");
	strText+=lpszPathName;
	if(AfxGetMainWnd())
		MessageBox(AfxGetMainWnd()->m_hWnd,strText,PLAN,MB_ERRORE);	
	else
		MessageBox(NULL,strText,PLAN,MB_ERRORE);	
}

void CPlanDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CString strEnabled;
	CString strDisabled;

	strEnabled.LoadString(IDS_ENABLED);
	strDisabled.LoadString(IDS_DISABLED);

	if(lpszTitle!=NULL)
		m_strTitle=lpszTitle;

	if(m_strTitle.Right(strEnabled.GetLength())==strEnabled)
		m_strTitle.TrimRight(strEnabled);
	else
	if(m_strTitle.Right(strDisabled.GetLength())==strDisabled)
		m_strTitle.TrimRight(strDisabled);

	if(m_strTitle.Right(3)!=_T(" - "))
		m_strTitle+=_T(" - ");

	m_strTitle+=((CPlanApp *)AfxGetApp())->m_Options.Timer?strEnabled:strDisabled;

	CDocument::SetTitle(m_strTitle);
}

void CPlanDoc::OnCloseDocument() 
{
	DoFileSave();
	CDocument::OnCloseDocument();
}

BOOL CPlanDoc::PlanFileCheck(LPCTSTR lpszPathName)
{
	CFileException fe;
	CFile *pFile;
	TCHAR str[PLAN_FILE_LEN+1];
	BYTE nItems;
	BYTE nMessage;
	BYTE nShutDown;
	BYTE nRunProgram;

	pFile=GetFile(lpszPathName,CFile::modeRead|CFile::shareDenyWrite,&fe);
	if(fe.m_cause||pFile==NULL)
		return FALSE;
	
	if(pFile->GetLength()<MIN_FILE_LENGTH)
		return FALSE;
	
	pFile->Read(str,PLAN_FILE_LEN);
	str[PLAN_FILE_LEN]=0;
	if(lstrcmp(str,PLAN_FILE))
		return FALSE;
		
	pFile->Seek(FILE_DATA_SEEK,CFile::begin);
		
	pFile->Read(&nItems,sizeof(nItems));
	pFile->Read(&nMessage,sizeof(nMessage));
	pFile->Read(&nShutDown,sizeof(nShutDown));
	pFile->Read(&nRunProgram,sizeof(nRunProgram));

	if(pFile->GetLength()!=FILE_LENGTH(nItems,nMessage,nShutDown,nRunProgram))
		return FALSE;
	
	ReleaseFile(pFile,FALSE);
	return TRUE;
}

LRESULT CALLBACK NewFileDlg(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM /*lParam*/)
{
	if(uMsg==WM_COMMAND)
		switch(LOWORD(wParam))
		{
			case ID_FILE_NEW:
				EndDialog(hDlg,ID_FILE_NEW);
				return TRUE;
			case ID_FILE_OPEN:
				EndDialog(hDlg,ID_FILE_OPEN);
				return TRUE;	
			case IDCANCEL:
				EndDialog(hDlg,IDCANCEL);
				return TRUE;
		}
	return FALSE;
}

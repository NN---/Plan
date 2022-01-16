// Plan.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Plan.h"

#include "MainFrame.h"
#include "PlanDoc.h"
#include "PlanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlanApp

BEGIN_MESSAGE_MAP(CPlanApp, CWinApp)
	//{{AFX_MSG_MAP(CPlanApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanApp construction

CPlanApp::CPlanApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPlanApp object

CPlanApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlanApp initialization

BOOL CPlanApp::InitInstance()
{
	HANDLE hMutex;
	BOOL bFound=FALSE;
	hMutex = CreateMutex(NULL, TRUE,
        _T("Plan"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
        bFound = TRUE;
    if(hMutex)
        ReleaseMutex(hMutex);
    if(bFound) 	return FALSE;
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(PROFILE_NAME);

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Load Options
	LoadOptions();
	
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPlanDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CPlanView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	//Remove print and printto sections
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Plan.File\\shell\\print"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Plan.File\\shell\\printto"));

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	((CMainFrame *)m_pMainWnd)->WindowShow();
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	CString CurDir=GetProfileString(SETTINGS,CURRENTDIRECTORY,_T(""));
	if(CurDir!=_T(""))
		SetCurrentDirectory(CurDir);

	return TRUE;
}

// App command to run the dialog
void CPlanApp::OnAppAbout()
{
	CDialog(IDD_ABOUTBOX).DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPlanApp message handlers
void CPlanApp::ParseCommandLine(CCommandLineInfo& rCmdInfo)
{
	int i;
	BOOL bWindowState=FALSE;
	BOOL bOpen=FALSE;

	rCmdInfo.m_nShellCommand=CCommandLineInfo::FileOpen; // Default - Open file

	for(i=1;i<__argc;i++)
	{
		if(!lstrcmp(__targv[i],_T("-min")))
			if(!bWindowState)
			{
				WriteProfileInt(SETTINGS,WINDOWSTATE,SW_SHOWMINIMIZED);
				bWindowState=TRUE;
			}
		else
		if(!lstrcmp(__targv[i],_T("-max")))
			if(!bWindowState)
			{
				WriteProfileInt(SETTINGS,WINDOWSTATE,SW_SHOWMAXIMIZED);
				bWindowState=TRUE;
			}
		else
		{
			if(!bOpen)
			{
				TCHAR tmp[4];
				CString str;
				int j,k;
				CFile file;
				CFileException fe;

				for(k=0,j=lstrlen(__targv[i])-3;j<lstrlen(__targv[i]);j++,k++)
					*(tmp+k)=*(__targv[i]+j);
				str=__targv[i];
				if(lstrcmp(tmp,PLAN_EXTENTION))
					str+=PLAN_EXTENTION;
			
				file.Open(str,CFile::modeReadWrite|CFile::shareDenyWrite,&fe);
				if(!fe.m_cause)
					bOpen=TRUE;
			}
		}
	}
	
	if(!bOpen)
	{
		rCmdInfo.m_strFileName=GetProfileString(SETTINGS,CURRENTDOCUMENT,_T(""));
		if(rCmdInfo.m_strFileName==_T(""))
			rCmdInfo.m_nShellCommand=CCommandLineInfo::FileNew;
	}
}

int CPlanApp::ExitInstance() 
{
	SaveOptions();

	return CWinApp::ExitInstance();
}

void CPlanApp::SaveOptions()
{
	HKEY hKey;
	RegCreateKeyEx(HKEY_CURRENT_USER,REG_RUN,0,REG_NONE,REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,NULL,&hKey,NULL);
	
	if(m_Options.LoadOnStartUp)
	{
		TCHAR str[_MAX_PATH];

		lstrcpy(str,__targv[0]);
		lstrcat(str,_T(" -min"));
		RegSetValueEx(hKey,PLAN,0,REG_SZ,(CONST BYTE *)str,lstrlen(str));
	}
	else
		RegDeleteValue(hKey,PLAN);
	RegCloseKey(hKey);

	WriteProfileBinary(SETTINGS,OPTIONS,(LPBYTE)&m_Options,sizeof(m_Options));
}

void CPlanApp::LoadOptions()
{
	HKEY hKey;
	DWORD dwType;
	TCHAR str[_MAX_PATH];
	TCHAR strRun[_MAX_PATH];
	DWORD cbData;

	strRun[0]=0;
	dwType=REG_SZ;
	cbData=sizeof(strRun);

	RegOpenKeyEx(HKEY_CURRENT_USER,REG_RUN,0,
		KEY_ALL_ACCESS,&hKey);
	RegQueryValueEx(hKey,PLAN,NULL,&dwType,(LPBYTE)strRun,&cbData);
	RegCloseKey(hKey);
	
	lstrcpy(str,__targv[0]);
	lstrcat(str,_T(" -min"));

	if(strRun[0]!=0&&dwType==REG_SZ&&!lstrcmp(str,strRun))
		m_Options.LoadOnStartUp=1;
	else
	{
		m_Options.LoadOnStartUp=0;
		RegDeleteValue(hKey,PLAN);
	}
	
	UINT uBytes;
	BYTE pData[sizeof(m_Options)];
	LPBYTE *ppData=(LPBYTE *)&pData;

	if(!GetProfileBinary(SETTINGS,OPTIONS,ppData,&uBytes)||uBytes!=sizeof(m_Options))
	{
		m_Options.Add=ADD_DEFAULT;
		m_Options.Update=UPDATE_DEFAULT;
		m_Options.Remove=REMOVE_DEFAULT;
		m_Options.Exit=EXIT_DEFAULT;
		m_Options.Timer=TIMER_DEFAULT;
		m_Options.Check=CHECK_DEFAULT;
		m_Options.BackupFile=BACKUPFILE_DEFAULT;
	}
	else
	{
		m_Options=*((struct tagOptions *)*ppData);
		delete [] *ppData;
	}
}

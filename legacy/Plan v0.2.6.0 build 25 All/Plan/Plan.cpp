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

// static members
const CPlanApp::Options CPlanApp::defaultOptions=
{
	1// bEnabled
};
const TCHAR s_strSettings[]=_T("Settings");
const TCHAR CPlanApp::s_strOptions[]=_T("Options");

BEGIN_MESSAGE_MAP(CPlanApp, CWinApp)
	//{{AFX_MSG_MAP(CPlanApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

LRESULT CALLBACK AboutDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
	switch(uMsg)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_EMAIL:
					ShellExecute(hDlg,"open","mailto:NN<nn-mail@yandex.ru>",NULL,NULL,SW_SHOW);
					break;
				case IDC_SITE:
					ShellExecute(hDlg,"open","http://nnsite.narod.ru",NULL,NULL,SW_SHOW);
					break;
				case IDOK:
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					return TRUE;
				case IDCANCEL:
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					return TRUE;
			}
			break;
		case WM_CLOSE:
			EndDialog(hDlg,0);
			return TRUE;
	}
	return FALSE;
}

// App command to run the dialog
void CPlanApp::OnAppAbout()
{
	DialogBox(m_hInstance,MAKEINTRESOURCE(IDD_ABOUT),m_pMainWnd->m_hWnd,(DLGPROC)AboutDlg);
}


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
	HANDLE hMutex=NULL;
	BOOL bFound=FALSE;
	hMutex = ::CreateMutex(NULL, TRUE, _T("Plan"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
        bFound = TRUE;
    if(hMutex)
        ReleaseMutex(hMutex);
	if(bFound)
	{
		return FALSE;
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
*/
	// Check if there any Event Libraries
	HWND hWndButton = ::CreateWindow(_T("BUTTON"),
		_T(""),
		BS_PUSHBUTTON,
		0,0,0,0,
		HWND_DESKTOP,
		NULL,
		m_hInstance,
		NULL);

	// Set Current Directory for Exe
	TCHAR szFileName[_MAX_PATH]=_T("");
	GetModuleFileName(AfxGetInstanceHandle(),szFileName,_MAX_PATH);

	int i=_MAX_PATH-1;
	while(*(szFileName+i)!='\\') i--;
	szFileName[i]=0;
	  
	if(!m_EventLib.LoadEvents(hWndButton,szFileName))
	{
		::DestroyWindow(hWndButton);
		ShowError(IDE_EVENTLIBRARIES);
		return FALSE;
	}
	m_EventLib.UnloadEvents(TRUE);
	::DestroyWindow(hWndButton);
	
	// Change the registry key under which our settings are stored.
	SetRegistryKey("NN");
	
	SetRegistry();
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

	//
	//Remove print and printto sections
	//
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Plan.File\\shell\\print"));
	RegDeleteKey(HKEY_CLASSES_ROOT,_T("Plan.File\\shell\\printto"));

	if(__argc>1)
	{
		if(!OpenDocumentFile(__targv[1]))
			OnFileNew();
	}
	else
		OnFileNew();
	
	if(!m_pMainWnd)
	{
		ShowError(IDE_UNKNOWN);
		return FALSE;
	}

	// The one window has been already showed
	// in the ActivateFrame()
	//m_pMainWnd->ShowWindow(SW_SHOW);
	//m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPlanApp message handlers

int CPlanApp::ExitInstance()
{
	SaveOptions();
	return CWinApp::ExitInstance();
}

void CPlanApp::SetRegistry()
{
	// Write Version
	WriteProfileString("","Version","0.2.6.0");
}

void CPlanApp::LoadOptions()
{
	UINT nBytes=sizeof(m_Options);
	Options *pOptions;
	GetProfileBinary(s_strSettings,s_strOptions,(LPBYTE *)&pOptions,&nBytes);
	if(pOptions)
	{
		m_Options=*pOptions;
		delete pOptions;
	}
	if(nBytes!=sizeof(m_Options))
		m_Options=defaultOptions;
}

void CPlanApp::SaveOptions()
{
	WriteProfileBinary(s_strSettings,s_strOptions,(LPBYTE)&m_Options,sizeof(Options));
}

EventLib &CPlanApp::GetEventLib()
{
	return m_EventLib;
}

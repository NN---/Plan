#include <windows.h>
#include "resource.h"
#include "..\Event\Event.h"

static const EventID ID = {2,"NNRunFile"};
static const TCHAR Name[]="RunFile";
static HWND hWndEvent=NULL;

#define PATH_MAX_CHARS		MAX_PATH
#define PARAMETERS_MAX_CHARS	MAX_PATH

typedef struct tagRunFile
{
	char Path[PATH_MAX_CHARS];
	char Parameters[PARAMETERS_MAX_CHARS];
}RunFile;

// ------------- //
HINSTANCE hInstance;
LRESULT CALLBACK RunFileDlg(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(hDlg, IDC_PATH), EM_SETLIMITTEXT, (WPARAM)PATH_MAX_CHARS-1,0);
			SendMessage(GetDlgItem(hDlg, IDC_PARAMETERS), EM_SETLIMITTEXT, (WPARAM)PARAMETERS_MAX_CHARS-1,0);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_BROWSE:
					{
						OPENFILENAME ofn;
						char szFile[MAX_PATH];
						const char szFilter[]="Executables (*.exe,*.com,*.bat)\0 \
							*.exe;*.com;*.bat\0All Files (*.*)\0*.*\0";

						szFile[0]=0;
						
						ZeroMemory(&ofn, sizeof(OPENFILENAME));
						ofn.lStructSize = sizeof(OPENFILENAME);
						ofn.hwndOwner = hDlg;
						ofn.lpstrFile = szFile;
						ofn.nMaxFile = MAX_PATH;
						ofn.lpstrFilter = szFilter;
						ofn.nFilterIndex = 1;
						ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | 
							OFN_FILEMUSTEXIST | OFN_EXPLORER;

						if(GetOpenFileName(&ofn))
							SetWindowText(GetDlgItem(hDlg,IDC_PATH),ofn.lpstrFile);
					}
			}
			break;
		default:
			DefWindowProc(hDlg, uMsg, wParam, lParam);
			break;
	}
	return FALSE;
}
// DllMain
BOOL WINAPI DllMain(
	  HINSTANCE hInstDLL,  // handle to the DLL module
	  DWORD fdwReason,     // reason for calling function
	LPVOID lpvReserved   // reserved
					)

{
	// To avoid warning
	lpvReserved;
	 // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
			hInstance = hInstDLL;
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
			Unload();
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

BOOL Load(HWND hWndParent)
{
	HWND hWnd;
	if(!hWndParent)
		return FALSE;
	if(hWndEvent)
		return TRUE;
	hWnd=CreateDialog(hInstance,"RunFile",hWndParent,(DLGPROC)RunFileDlg);
	if(!hWnd)
		return FALSE;
	hWndEvent=hWnd;
	return TRUE;
}

BOOL Unload(void)
{
	BOOL bRes;
	if(!hWndEvent)
		return FALSE;
	bRes=DestroyWindow(hWndEvent);
	hWndEvent=NULL;
	return bRes;
}

BOOL Move(RECT rect, BOOL bRepaint)
{
	if(!hWndEvent)
		return FALSE;
	MoveWindow(hWndEvent,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,bRepaint);
	return TRUE;
}

BOOL Show(BOOL bShow)
{
	if(!hWndEvent)
		return FALSE;
	ShowWindow(hWndEvent,bShow ? SW_SHOW : SW_HIDE);
	return TRUE;
}

BOOL IsEmpty(void)
{
	if(!hWndEvent)
		return TRUE;
	return !SendMessage(GetDlgItem(hWndEvent, IDC_PATH), WM_GETTEXTLENGTH, 0, 0);
}

BOOL Empty(void)
{
	if(!hWndEvent)
		return FALSE;
	SetDlgItemText(hWndEvent, IDC_PATH, "");
	SetDlgItemText(hWndEvent, IDC_PARAMETERS, "");
	return TRUE;
}

BOOL Do(const LPBYTE pData)
{
	RunFile *pRunFile;
	char szDir[MAX_PATH];
	char *pPath,*pDir;
	if(!hWndEvent)
		return FALSE;
	if(!pData)
		return FALSE;
	
	pRunFile=(RunFile *)pData;
	
	pPath=pRunFile->Path;
	pDir=szDir;

	while(*pPath!='\\')
		*pDir++=*pPath++;

	*pDir=0;
	
	ShellExecute(HWND_DESKTOP,"open",pRunFile->Path,pRunFile->Parameters,
		szDir,SW_SHOW);
	if(GetLastError())
		return FALSE;
	return TRUE;
}

PCEventID GetID(void)
{
	return &ID;
}

LPCTSTR GetName(void)
{
	return Name;
}

BOOL GetData(LPBYTE pData)
{
	RunFile *pRunFile=(RunFile *)pData;
	if(!hWndEvent)
		return FALSE;
	if(!pData)
		return FALSE;
	GetDlgItemText(hWndEvent,IDC_PATH,pRunFile->Path,PATH_MAX_CHARS);
	GetDlgItemText(hWndEvent,IDC_PARAMETERS,pRunFile->Parameters,PARAMETERS_MAX_CHARS);
	return TRUE;
}

void SetData(const LPBYTE pData)
{
	RunFile *pRunFile=(RunFile *)pData;
	if(!hWndEvent)
		return;
	if(!pData)
		return;
	SetDlgItemText(hWndEvent,IDC_PATH,pRunFile->Path);
	SetDlgItemText(hWndEvent,IDC_PARAMETERS,pRunFile->Parameters);
}

UINT GetDataSize(void)
{
	return sizeof(RunFile);
}
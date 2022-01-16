#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0400
#include <windows.h>
#include "resource.h"
#include "..\Event\Event.h"

static const EventID ID = {1,"NNShutDown"};
static const TCHAR Name[]="ShutDown";
static HWND hWndEvent=NULL;

#define ACTION_FIRST ACTION_SHUTDOWN
#define ACTION_LAST	 ACTION_LOGOFF

typedef struct tagShutDown
{
	BYTE Action;
}ShutDown;

// ------------- //
HINSTANCE hInstance;
LRESULT CALLBACK ShutDownDlg(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				int i;
				char szAction[MAX_PATH];
				HWND hWndAction=GetDlgItem(hDlg,IDC_ACTION);
				for(i=ACTION_FIRST;i<=ACTION_LAST;i++)
				{
					LoadString(hInstance,i,szAction,MAX_PATH);
					SendMessage(hWndAction,CB_ADDSTRING,0,(LPARAM)szAction);
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
	hWnd=CreateDialog(hInstance,"ShutDown",hWndParent,(DLGPROC)ShutDownDlg);
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
	return SendMessage(GetDlgItem(hWndEvent,IDC_ACTION),CB_GETCURSEL,0,0)==-1;
}

BOOL Empty(void)
{
	if(!hWndEvent)
		return FALSE;
	SendMessage(GetDlgItem(hWndEvent,IDC_ACTION),CB_SETCURSEL,(WPARAM)-1,0);
	return TRUE;
}

BOOL Do(const LPBYTE pData)
{
	if(!hWndEvent)
		return FALSE;
	if(!pData)
		return FALSE;
	switch(((ShutDown *)pData)->Action)
	{
		case ACTION_SHUTDOWN:
			{
				HANDLE hToken; 
				TOKEN_PRIVILEGES tkp; 
 
				if(!OpenProcessToken(GetCurrentProcess(), 
					TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
				return FALSE; 
 
				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
				
				tkp.PrivilegeCount = 1;
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
 
				if(GetLastError() != ERROR_SUCCESS) 
					return FALSE; 
 
				if(!ExitWindowsEx(EWX_SHUTDOWN, 0)) 
					return FALSE; 
			}
			break;
		case ACTION_REBOOT:
			{
				HANDLE hToken; 
				TOKEN_PRIVILEGES tkp; 
 
				if(!OpenProcessToken(GetCurrentProcess(), 
					TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
				return FALSE; 
 
				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
				
				tkp.PrivilegeCount = 1;
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
 
				if(GetLastError() != ERROR_SUCCESS) 
					return FALSE; 
 
				if(!ExitWindowsEx(EWX_REBOOT, 0)) 
					return FALSE; 
			}
			break;
		case ACTION_LOGOFF:
			ExitWindowsEx(EWX_LOGOFF,0);
			break;
	}
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
	if(!hWndEvent)
		return FALSE;
	if(!pData)
		return FALSE;
	((ShutDown *)pData)->Action=(BYTE)SendMessage(GetDlgItem(hWndEvent,IDC_ACTION),CB_GETCURSEL,0,0);
	return TRUE;
}

void SetData(const LPBYTE pData)
{
	if(!hWndEvent)
		return;
	if(!pData)
		return;
	SendMessage(GetDlgItem(hWndEvent,IDC_ACTION),CB_SETCURSEL,(WPARAM)(((ShutDown *)pData)->Action),0);
}

UINT GetDataSize(void)
{
	return sizeof(ShutDown);
}
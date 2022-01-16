#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "resource.h"
#include "Event.h"

const EventID ID = {0,"NNMessage"};
const TCHAR Name[]="Message";
HWND hWndEvent=NULL;

#define CAPTION_MAX_CHARS	50
#define TEXT_MAX_CHARS		100

// ------------- //
HINSTANCE hInstance;
LRESULT CALLBACK MessageDlg(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(hDlg, IDC_CAPTION), EM_SETLIMITTEXT, (WPARAM)CAPTION_MAX_CHARS,0);
			SendMessage(GetDlgItem(hDlg, IDC_TEXT), EM_SETLIMITTEXT, (WPARAM)TEXT_MAX_CHARS,0);
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
			NF(Unload)();
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

BOOL NF(Load)(HWND hWndParent)
{
	HWND hWnd;
	if(!hWndParent)
		return FALSE;
	if(hWndEvent)
		return TRUE;
	hWnd=CreateDialog(hInstance,"Message",hWndParent,(DLGPROC)MessageDlg);
	if(!hWnd)
		return FALSE;
	hWndEvent=hWnd;
	return TRUE;
}

BOOL NF(Unload)(void)
{
	if(!hWndEvent)
		return FALSE;
	return DestroyWindow(hWndEvent);
}

BOOL NF(Move)(RECT rect, BOOL bRepaint)
{
	if(!hWndEvent)
		return FALSE;
	MoveWindow(hWndEvent,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,bRepaint);
	return TRUE;
}

BOOL NF(Show)(BOOL bShow)
{
	if(!hWndEvent)
		return FALSE;
	ShowWindow(hWndEvent,bShow ? SW_SHOW : SW_HIDE);
	return TRUE;
}


BOOL NF(IsEmpty)(void)
{
	if(!hWndEvent)
		return TRUE;
	if(SendMessage(GetDlgItem(hWndEvent, IDC_CAPTION), WM_GETTEXTLENGTH, 0, 0) &&
		SendMessage(GetDlgItem(hWndEvent, IDC_TEXT), WM_GETTEXTLENGTH, 0, 0))
		return FALSE;
	return TRUE;
}

BOOL NF(Empty)(void)
{
	if(!hWndEvent)
		return FALSE;
	SetDlgItemText(hWndEvent, IDC_CAPTION, "");
	SetDlgItemText(hWndEvent, IDC_TEXT, "");
	return TRUE;
}

BOOL NF(Do)(void)
{
	char szCaption[CAPTION_MAX_CHARS];
	char szText[TEXT_MAX_CHARS];
	if(!hWndEvent)
		return FALSE;
	if(NF(IsEmpty)())
		return FALSE;
	GetDlgItemText(hWndEvent, IDC_CAPTION, szCaption, CAPTION_MAX_CHARS);
	GetDlgItemText(hWndEvent, IDC_TEXT, szText, TEXT_MAX_CHARS);
	MessageBox(hWndEvent, szText, szCaption, MB_OK);
	if(GetLastError())
		return FALSE;
	return TRUE;
}

PCEventID NF(GetID)(void)
{
	return &ID;
}

LPCTSTR NF(GetName)(void)
{
	return Name;
}

BOOL NF(Write)(HKEY hKey)
{
	char szCaption[CAPTION_MAX_CHARS];
	char szText[TEXT_MAX_CHARS];
	if(!hKey)
		return FALSE;
	RegSetValueEx(hKey,"ID",0,REG_BINARY,(const BYTE *)&ID,sizeof(EventID));
	GetDlgItemText(hWndEvent, IDC_CAPTION, szCaption, CAPTION_MAX_CHARS);
	GetDlgItemText(hWndEvent, IDC_TEXT, szText, TEXT_MAX_CHARS);
	RegSetValueEx(hKey,"Caption",0,REG_SZ,szCaption,CAPTION_MAX_CHARS);
	RegSetValueEx(hKey,"Text",0,REG_SZ,szText,TEXT_MAX_CHARS);
	return TRUE;
}

BOOL NF(Read)(HKEY hKey)
{
	char szCaption[CAPTION_MAX_CHARS];
	char szText[TEXT_MAX_CHARS];
	EventID eventID;
	DWORD dwSize;
	DWORD dwType;
	LONG lRes;
	if(!hKey)
		return FALSE;
	dwSize=sizeof(EventID);
	dwType=REG_BINARY;
	lRes=RegQueryValueEx(hKey,"ID",NULL,&dwType,(LPBYTE)&eventID,&dwSize);
	if(lRes||dwSize!=sizeof(EventID)||dwType!=REG_BINARY||EventID_Cmp(&eventID,&ID))
		return FALSE;
	
	dwSize=CAPTION_MAX_CHARS;
	dwType=REG_SZ;
	RegQueryValueEx(hKey,"Caption",NULL,&dwType,szCaption,&dwSize);
	if(lRes||dwSize>CAPTION_MAX_CHARS||dwType!=REG_SZ)
		return FALSE;
	
	dwSize=TEXT_MAX_CHARS;
	//dwType=REG_SZ;
	RegQueryValueEx(hKey,"Text",NULL,&dwType,szText,&dwSize);
	if(lRes||dwSize>TEXT_MAX_CHARS||dwType!=REG_SZ)
		return FALSE;

	SetDlgItemText(hWndEvent, IDC_CAPTION, szCaption);
	SetDlgItemText(hWndEvent, IDC_TEXT, szText);
	return TRUE;
}
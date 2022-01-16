#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "resource.h"
#include "..\Event\Event.h"

static const EventID ID = {0,"NNMessage"};
static const TCHAR Name[]="Message";
static HWND hWndEvent=NULL;

#define CAPTION_MAX_CHARS	50
#define TEXT_MAX_CHARS		100

typedef struct tagMessage
{
	char Caption[CAPTION_MAX_CHARS];
	char Text[TEXT_MAX_CHARS];
}Message;

// ------------- //
HINSTANCE hInstance;
LRESULT CALLBACK MessageDlg(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(hDlg, IDC_CAPTION), EM_SETLIMITTEXT, (WPARAM)CAPTION_MAX_CHARS-1,0);
			SendMessage(GetDlgItem(hDlg, IDC_TEXT), EM_SETLIMITTEXT, (WPARAM)TEXT_MAX_CHARS-1,0);
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
	hWnd=CreateDialog(hInstance,"Message",hWndParent,(DLGPROC)MessageDlg);
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
	if(SendMessage(GetDlgItem(hWndEvent, IDC_CAPTION), WM_GETTEXTLENGTH, 0, 0) &&
		SendMessage(GetDlgItem(hWndEvent, IDC_TEXT), WM_GETTEXTLENGTH, 0, 0))
		return FALSE;
	return TRUE;
}

BOOL Empty(void)
{
	if(!hWndEvent)
		return FALSE;
	SetDlgItemText(hWndEvent, IDC_CAPTION, "");
	SetDlgItemText(hWndEvent, IDC_TEXT, "");
	return TRUE;
}

BOOL Do(const LPBYTE pData)
{
	Message *pMessage;
	if(!hWndEvent)
		return FALSE;
	if(!pData)
		return FALSE;
	pMessage=(Message *)pData;
	MessageBox(hWndEvent, pMessage->Text, pMessage->Caption, MB_OK);
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
	Message *pMessage=(Message *)pData;
	if(!hWndEvent)
		return FALSE;
	if(!pData)
		return FALSE;
	GetDlgItemText(hWndEvent,IDC_CAPTION,pMessage->Caption,CAPTION_MAX_CHARS);
	GetDlgItemText(hWndEvent,IDC_TEXT,pMessage->Text,TEXT_MAX_CHARS);
	return TRUE;
}

void SetData(const LPBYTE pData)
{
	Message *pMessage=(Message *)pData;
	if(!hWndEvent)
		return;
	if(!pData)
		return;
	SetDlgItemText(hWndEvent,IDC_CAPTION,pMessage->Caption);
	SetDlgItemText(hWndEvent,IDC_TEXT,pMessage->Text);
}

UINT GetDataSize(void)
{
	return sizeof(Message);
}
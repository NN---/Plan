// Plan.cpp : main source file for Plan.exe
//

#include "stdafx.h"

#include "Resource.h"
#include "Defines.h"

#include "CFile.h"
using namespace RSDN;
#include "MainFrame.h"

CAppModule _Module;

EventLib _EventLib;

#define CMDLINE_LAST 1

static TCHAR *strCommandLine[]=
{
	{_T("-min")},
	{_T("-max")}
};

static int CmdShow[]=
{SW_SHOWMINIMIZED,SW_SHOWMAXIMIZED};

int ParseCommandLine()
{
	if(__argc==1)
		return -1;
	int i,j,n=__argc;
	for(i=1;i<n;i++)
		for(j=0;j<=CMDLINE_LAST;j++)
			if(!lstrcmp(strCommandLine[j],__argv[i]/*__targv[i]*/))
				return CmdShow[j];
	return -1;
}

int Run()
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame mainWnd;

	if(!mainWnd.CreateEx())
	{
		return 0;
	}

	mainWnd.ShowWindow(ParseCommandLine());

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

#define PLAN_VERSION _T("0.2.6.1")
#define VERSION _T("Version")

void SetRegistry()
{
	RegKey reg;
	reg.Create(HKEY_PROG,STR_REG_PROG);
	reg.SetValue(PLAN_VERSION,VERSION);
	reg.Close();
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
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

	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES | 
		ICC_LISTVIEW_CLASSES );	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	SetRegistry();

	int nRet = Run();

	_Module.Term();
	::CoUninitialize();

	return nRet;
}

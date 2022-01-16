// Plan.cpp : main source file for Plan.exe
//

#include "StdAfx.h"
#include "PlanFrame.h"

CAppModule _Module;
PlanCommandLine _CommandLine;
EventDlls _EventDlls;

int Run()
{
	_CommandLine.SetCommandLine(GetCommandLine());
	_CommandLine.ParseCommandLine();

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	int nRet;

	CPlanFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(_CommandLine.GetCommandLineInfo().nCmdShow);

	nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpszCmdLine*/, int /*nCmdShow*/)
{
	// Only one instance !
	HANDLE hMutex=NULL;
	BOOL bFound=FALSE;
	hMutex = ::CreateMutex(NULL, TRUE, _T("Plan"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
        bFound = TRUE;
    if(hMutex)
        ReleaseMutex(hMutex);
	if(bFound)
		return FALSE;

	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(0x4FF);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run();

	_Module.Term();
	::CoUninitialize();

#ifdef _DEBUG
	// Destructor releases memory but we do not want to get message
	// about memory leaks
	_CommandLine.~PlanCommandLine();
	//_EventDlls.~EventDlls();
	_EventDlls.Destroy();
	_CrtDumpMemoryLeaks();
#endif

	return nRet;
}

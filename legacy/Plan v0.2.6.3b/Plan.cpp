// Plan.cpp : main source file for Plan.exe
//

#include "StdAtl.h"
#include "Resource.h"

#include "CFile.h"
using namespace RSDN;

#include "MainFrame.h"

#include "..\Event\Event.h"

CApp _Module;
EventLib _EventLib;

#define CMDLINE_LAST 2

int CmdShow[]=
{
	SW_MINIMIZE,
	SW_MAXIMIZE,
	SW_HIDE,
};

TCHAR *strCommandLine[]=
{
	{_T("-min")},
	{_T("-max")},
	{_T("-tray")},
};

int ParseCommandLine()
{
	int ArgC=_Module.GetArgC();
	if(ArgC==1)
		return SW_SHOW;
	int i,j;
	const TCHAR **ArgV=_Module.GetArgV();
	for(i=1;i<ArgC;i++)
		for(j=0;j<=CMDLINE_LAST;j++)
			if(!lstrcmp(strCommandLine[j],ArgV[i]))
				return CmdShow[j];
	return SW_SHOW;
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

	mainWnd.Show(ParseCommandLine());

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance,
					 HINSTANCE /*hPrevInstance*/,
					 LPTSTR /*lpszCmdLine*/,
					 int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(0x4F);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance, 0);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run();

	_Module.Term();
	::CoUninitialize();

	return nRet;
}

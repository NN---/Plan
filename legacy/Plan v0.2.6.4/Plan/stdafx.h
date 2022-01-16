// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlddx.h>
//#include <atldlgs.h>


// PlanCommandLine
#include "PlanCommandLine.h"
extern PlanCommandLine _CommandLine;

// EventDlls
#define EVENT_IMPORT
#include "..\Event\Event.h"

// Resource Header
#include "resource.h"

//Errors
#define ShowError(hWnd) AtlMessageBox(hWnd, IDE_UNKNOWN, IDS_ERROR, MB_OK | MB_ICONEXCLAMATION)
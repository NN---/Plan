// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#pragma once

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100
//#define OEMRESOURCE

//#define _WTL_NEW_PAGE_NOTIFY_HANDLERS
#define WTL_USE_CSTRING
#define _ATL_CSTRING_NO_CRT

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlframe.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlddx.h>
#include <atlcoll.h>

//#include <atlctrlx.h>
//#include <atldlgs.h>

// SystemTime/FileTime
#include "SysFileTime.h"

// PlanCommandLine
#include "PlanCommandLine.h"
extern PlanCommandLine _CommandLine;

// EventDlls
#define EVENT_IMPORT
#include "..\Event\Event.h"
extern EventDlls _EventDlls;

// Document/View
#include "DocumentView.h"

// Resource Header
#include "resource.h"

//Errors
#include "Error.h"

// Debug
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

#endif // __STDAFX_H__
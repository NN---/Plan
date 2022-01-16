#ifndef __STDATL_H__
#define __STDATL_H__

#pragma once

#define WINVER		0x0400
#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include <atlbase.h>
#include <atlapp.h>
#include "App.h"

extern CApp _Module;

#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlmisc.h>
#include <atlddx.h>
#include <atldlgs.h>

#endif // __STDATL_H__
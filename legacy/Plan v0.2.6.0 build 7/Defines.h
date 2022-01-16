// Defines.h
//

#ifndef __cplusplus
#error Must use cplusplus
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __DEFINES_INCLUDED__

#define	WM_SHOWME		  WM_USER+5

// Data Exchange
// Date and Time
void AFXAPI DDX_DateTime(CDataExchange *pDX, const int nIDDate, const int nIDTime, CTime &rTime);

#endif // __DEFINES_INCLUDED__

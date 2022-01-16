#ifndef __DEFINES_H
#define __DEFINES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ShowError() _ShowError(IDE_UNKNOWN,m_hWnd)
#define ShowError2() _ShowError(IDE_UNKNOWN,GetActiveFrame()->m_hWnd)

inline void _ShowError(UINT nIDError = IDE_UNKNOWN, HWND hWnd = NULL,
		UINT uType = MB_OK | MB_ICONEXCLAMATION)
{
	CString strText;
	CString strCaption;
	strText.LoadString(nIDError);
	strCaption.LoadString(IDS_ERROR);
	::MessageBox(hWnd,strText,strCaption,uType);
}

/*
inline const SYSTEMTIME operator+(const SYSTEMTIME &st1,const SYSTEMTIME &st2)
{
	ULARGE_INTEGER uli1,uli2,uli;
	SYSTEMTIME st;
	SystemTimeToFileTime(&st1,(LPFILETIME)&uli1);
	SystemTimeToFileTime(&st2,(LPFILETIME)&uli2);
	uli.QuadPart=uli1.QuadPart+uli2.QuadPart;
	FileTimeToSystemTime((const LPFILETIME)&uli,&st);
	return st;
}
*/
inline const SYSTEMTIME operator-(const SYSTEMTIME &st1,const SYSTEMTIME &st2)
{
	ULARGE_INTEGER uli1,uli2,uli;
	SYSTEMTIME st;
	SystemTimeToFileTime(&st1,(LPFILETIME)&uli1);
	SystemTimeToFileTime(&st2,(LPFILETIME)&uli2);
	uli.QuadPart=uli1.QuadPart-uli2.QuadPart;
	FileTimeToSystemTime((const LPFILETIME)&uli,&st);
	return st;
}

#endif // __DEFINES_H
#ifndef __ERROR_H__
#define __ERROR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

inline void ShowError(UINT nIDError = IDE_UNKNOWN, HWND hWnd = NULL,
		UINT uType = MB_OK | MB_ICONEXCLAMATION)
{
	CString strText;
	CString strCaption;
	strText.LoadString(nIDError);
	strCaption.LoadString(IDS_ERROR);
	::MessageBox(hWnd,strText,strCaption,uType);
}

#endif // __ERROR_H__
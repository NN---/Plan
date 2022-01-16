#ifndef __ERROR_H__
#define __ERROR_H__

#pragma once

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
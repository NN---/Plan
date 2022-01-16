#pragma once

inline void ShowError(UINT nIDError = IDE_UNKNOWN, HWND hWnd = NULL,
		UINT uType = MB_OK | MB_ICONEXCLAMATION)
{
	AtlMessageBox(hWnd, nIDError, IDS_ERROR, uType);
}
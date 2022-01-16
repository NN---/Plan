//
// Defines.cpp
#include "stdafx.h"
#include "Plan.h"
#include "Defines.h"

void ShowError(UINT nIDError,CWnd *pWnd /*= NULL*/,UINT uType /*= MB_OK | MB_ICONEXCLAMATION*/)
{
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();
	CString strText;
	CString strCaption;
	strText.LoadString(nIDError);
	strCaption.LoadString(IDS_ERROR);
	::MessageBox(pWnd?pWnd->m_hWnd:
		pApp->m_pMainWnd?pApp->m_pMainWnd->m_hWnd:
		NULL,strText,strCaption,uType);
}
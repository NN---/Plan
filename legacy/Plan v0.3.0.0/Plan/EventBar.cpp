#include "StdAfx.h"
#include "EventBar.h"

// WM_INITDIALOG
LRESULT CEventBar::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Change
	// Set Current Directory for Exe
	TCHAR szFileName[_MAX_PATH]=_T("");
	GetModuleFileName(_Module.GetModuleInstance(),szFileName,_MAX_PATH);

	int i=_MAX_PATH-1;
	while(*(szFileName+i)!='\\') i--;
	szFileName[i]=0;
	if(!_EventDlls.LoadDlls(m_hWnd,szFileName))
	{
		ShowError(IDE_UNKNOWN,m_hWnd);
		DestroyWindow();
		return NULL;
	}

	RECT rectEventPlace;
	::GetWindowRect(GetDlgItem(IDC_EVENTPLACE),&rectEventPlace);
	ScreenToClient(&rectEventPlace);

	size_t iDll,nDll;
	for(iDll=0,nDll=_EventDlls.GetDllCount();iDll<nDll;iDll++)
	{
		_EventDlls.SetWorkDll(iDll);
		_EventDlls.GetDll().Move(rectEventPlace,FALSE);
	}
//
	_EventDlls.SetWorkDll(0);
	_EventDlls.GetDll().Show(TRUE);

	return FALSE;
}
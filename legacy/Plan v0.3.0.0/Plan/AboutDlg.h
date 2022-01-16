#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__

#pragma once

class CAboutDlg : public CSimpleDialog<IDD_ABOUT>
{
	typedef CAboutDlg thisClass;
	typedef CSimpleDialog<IDD_ABOUT> baseClass;
public:
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_EMAIL, OnEmail)
		COMMAND_ID_HANDLER(IDC_SITE, OnSite)
		// Command
		// Chain Message Map
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEmail(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSite(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif // __ABOUTDLG_H__
#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__

#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
	typedef CAboutDlg thisClass;
public:
	enum { IDD = IDD_ABOUT };

	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Messages
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		// Commands
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_EMAIL, OnEmail)
		COMMAND_ID_HANDLER(IDC_SITE, OnSite)
	END_MSG_MAP()

	// Messages
	// WM_INITDIALOG
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		return FALSE;
	}

	// Commands
	// IDOK
	// IDCANCEL
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	// IDC_EMAIL
	LRESULT OnEmail(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ShellExecute(m_hWnd,_T("open"),_T("mailto:NN<nn-mail@yandex.ru>"),NULL,NULL,SW_SHOW);
		return 0;
	}

	// IDC_SITE
	LRESULT OnSite(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ShellExecute(m_hWnd,_T("open"),_T("http://nnsite.narod.ru"),NULL,NULL,SW_SHOW);
		return 0;
	}
};

#endif // __ABOUTDLG_H__
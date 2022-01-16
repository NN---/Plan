#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
	typedef CAboutDlg thisClass;
public:
	enum { IDD = IDD_ABOUT };
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		// Command
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_EMAIL, OnEmail)
		COMMAND_ID_HANDLER(IDC_SITE, OnSite)
	END_MSG_MAP()

	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEmail(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSite(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};
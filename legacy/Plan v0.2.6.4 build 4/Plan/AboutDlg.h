#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__

#pragma once

class CAboutDlg : public CSimpleDialog<IDD_ABOUT>
	//,
	//				public CWinDataExchange<CAboutDlg>
{
	typedef CAboutDlg thisClass;
	typedef CSimpleDialog<IDD_ABOUT> baseClass;
public:
//	CHyperLink m_hlSite;
//	CHyperLink m_hlEmail;
	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		// Command
		// Chain Message Map
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

//	BEGIN_DDX_MAP(thisClass)
//		DDX_CONTROL(IDC_SITE, m_hlSite)
//		DDX_CONTROL(IDC_EMAIL, m_hlEmail)
//	END_DDX_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif // __ABOUTDLG_H__
#include "StdAfx.h"
#include "AboutDlg.h"

// Message
// WM_INITDIALOG
LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow();
	return 0;
}

// WM_COMMAND
// IDC_EMAIL
LRESULT CAboutDlg::OnEmail(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd,_T("open"),_T("mailto:NN<nn-mail@bk.ru>"),NULL,NULL,SW_SHOW);
	return 0;
}

// IDC_SITE
LRESULT CAboutDlg::OnSite(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd,_T("open"),_T("http://nnsite.by.ru"),NULL,NULL,SW_SHOW);
	return 0;
}
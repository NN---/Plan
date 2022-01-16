#include "StdAfx.h"
#include "AboutDlg.h"

// Command
// IDOK
// IDCANCEL
LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

// IDC_EMAIL
LRESULT CAboutDlg::OnEmail(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd,_T("open"),_T("mailto:NN<nn-mail@yandex.ru>"),NULL,NULL,SW_SHOW);
	return 0;
}

// IDC_SITE
LRESULT CAboutDlg::OnSite(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd,_T("open"),_T("http://nnsite.narod.ru"),NULL,NULL,SW_SHOW);
	return 0;
}
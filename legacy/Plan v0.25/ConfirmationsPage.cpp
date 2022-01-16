// ConfirmationsPage.cpp : implementation file
//

#include "stdafx.h"
#include "plan.h"
#include "ConfirmationsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfirmationsPage property page

IMPLEMENT_DYNCREATE(CConfirmationsPage, CPropertyPage)

CConfirmationsPage::CConfirmationsPage() : CPropertyPage(CConfirmationsPage::IDD)
{
	//{{AFX_DATA_INIT(CConfirmationsPage)
	//}}AFX_DATA_INIT
}

CConfirmationsPage::~CConfirmationsPage()
{
}

void CConfirmationsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmationsPage)
	//}}AFX_DATA_MAP
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();
	if(pDX->m_bSaveAndValidate)
	{
		pApp->m_Options.Add=IsDlgButtonChecked(IDC_ADD);
		pApp->m_Options.Update=IsDlgButtonChecked(IDC_UPDATE);
		pApp->m_Options.Remove=IsDlgButtonChecked(IDC_REMOVE);
		pApp->m_Options.Exit=IsDlgButtonChecked(IDC_EXIT);
	}
	else
	{
		CheckDlgButton(IDC_ADD,pApp->m_Options.Add);
		CheckDlgButton(IDC_UPDATE,pApp->m_Options.Update);
		CheckDlgButton(IDC_REMOVE,pApp->m_Options.Remove);
		CheckDlgButton(IDC_EXIT,pApp->m_Options.Exit);
	}
}

BEGIN_MESSAGE_MAP(CConfirmationsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConfirmationsPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmationsPage message handlers

// GeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "plan.h"
#include "GeneralPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage property page

IMPLEMENT_DYNCREATE(CGeneralPage, CPropertyPage)

CGeneralPage::CGeneralPage() : CPropertyPage(CGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CGeneralPage)
	//}}AFX_DATA_INIT
}

CGeneralPage::~CGeneralPage()
{
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPage)
	//}}AFX_DATA_MAP
	CPlanApp *pApp=(CPlanApp *)AfxGetApp();
	if(pDX->m_bSaveAndValidate)
	{
		pApp->m_Options.LoadOnStartUp=IsDlgButtonChecked(IDC_LOADONSTARTUP);
		pApp->m_Options.BackupFile=IsDlgButtonChecked(IDC_BACKUPFILE);
		pApp->m_Options.Check=IsDlgButtonChecked(IDC_CHECKEVENT);
	}
	else
	{
		CheckDlgButton(IDC_LOADONSTARTUP,pApp->m_Options.LoadOnStartUp);
		CheckDlgButton(IDC_BACKUPFILE,pApp->m_Options.BackupFile);
		if(pApp->m_Options.Check)
			CheckDlgButton(IDC_CHECKEVENT,TRUE);
		else
			CheckDlgButton(IDC_REMOVEEVENT,TRUE);
	}
}


BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage message handlers

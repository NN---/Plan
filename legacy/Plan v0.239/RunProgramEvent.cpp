// RunProgramEvent.cpp : implementation file
//

#include "stdafx.h"
#include "plan.h"
#include "RunProgramEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunProgramEvent dialog


CRunProgramEvent::CRunProgramEvent(CWnd* pParent /*=NULL*/)
	: CDialog(CRunProgramEvent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunProgramEvent)
	m_Path = _T("");
	m_Parameters = _T("");
	//}}AFX_DATA_INIT
}


void CRunProgramEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunProgramEvent)
	DDX_Text(pDX, IDC_PATH, m_Path);
	DDV_MaxChars(pDX, m_Path, RUNPROGRAM_PATH_CHARS);
	DDX_Text(pDX, IDC_PARAMETERS, m_Parameters);
	DDV_MaxChars(pDX, m_Parameters, RUNPROGRAM_PARAMETERS_CHARS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunProgramEvent, CDialog)
	//{{AFX_MSG_MAP(CRunProgramEvent)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunProgramEvent message handlers

void CRunProgramEvent::OnOK()
{
}

void CRunProgramEvent::OnCancel()
{
}

void CRunProgramEvent::Clear()
{
	m_Path.Empty();
	m_Parameters.Empty();
	UpdateData(FALSE);
}

const BOOL CRunProgramEvent::IsClear()
{
	if(m_Path.GetLength())
		return FALSE;
	else
		return TRUE;
}
	
void CRunProgramEvent::GetEvent(RunProgram *pRunProgram)
{
	if(pRunProgram==NULL)
		return;
	
	UpdateData(TRUE);
	
	lstrcpy(pRunProgram->Path(),m_Path);
	lstrcpy(pRunProgram->Parameters(),m_Parameters);
}

void CRunProgramEvent::SetEvent(RunProgram *pRunProgram)
{
	if(pRunProgram==NULL)
		return;

	m_Path=pRunProgram->Path();
	m_Parameters=pRunProgram->Parameters();

	UpdateData(FALSE);
}

void CRunProgramEvent::DoEvent(RunProgram &rRunProgram)
{
	TCHAR Dir[_MAX_PATH];
	int i;

	for(i=lstrlen(rRunProgram.Path());i>0 && *(rRunProgram.Path()+i)!='\\';i--);

	if(i==0)
		return;

	lstrcpyn(Dir,rRunProgram.Path(),i+2);
	
	ShellExecute(HWND_DESKTOP,_T("open"),rRunProgram.Path(),rRunProgram.Parameters(),
		Dir,SW_SHOW);
}

void CRunProgramEvent::OnBrowse() 
{	
	CFileDialog fd(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
		_T("Executables (*.exe,*.com,*.bat)|*.exe;*.com;*.bat|All Files (*.*)|*.*||"),
		this);
	if(fd.DoModal()==IDOK)
	{
		m_Path=fd.GetPathName();
		UpdateData(FALSE);
	}
}

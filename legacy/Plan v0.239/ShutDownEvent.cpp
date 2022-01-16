// ShutDownEvent.cpp : implementation file
//

#include "stdafx.h"
#include "plan.h"
#include "ShutDownEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShutDownEvent dialog


CShutDownEvent::CShutDownEvent(CWnd* pParent /*=NULL*/)
	: CDialog(CShutDownEvent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShutDownEvent)
	m_Operation = -1;
	//}}AFX_DATA_INIT
}


void CShutDownEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShutDownEvent)
	DDX_CBIndex(pDX, IDC_OPERATION, m_Operation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShutDownEvent, CDialog)
	//{{AFX_MSG_MAP(CShutDownEvent)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShutDownEvent message handlers
void CShutDownEvent::OnOK()
{
}

void CShutDownEvent::OnCancel()
{
}

void CShutDownEvent::Clear()
{
	m_Operation=CB_ERR;
	UpdateData(FALSE);
}

const BOOL CShutDownEvent::IsClear()
{
	if(m_Operation==CB_ERR)
		return TRUE;
	else
		return FALSE;
}

void CShutDownEvent::GetEvent(ShutDown *pShutDown)
{
	if(pShutDown==NULL)
		return;
	
	UpdateData(TRUE);

	pShutDown->Operation()=(BYTE)m_Operation;
}

void CShutDownEvent::SetEvent(ShutDown *pShutDown)
{
	if(pShutDown==NULL)
		return;

	m_Operation=pShutDown->Operation();

	UpdateData(FALSE);
}

void CShutDownEvent::DoEvent(ShutDown &rShutDown)
{
	switch(rShutDown.Operation())
	{
		case ShutDown::opShutDown:
			ExitWindowsEx(EWX_SHUTDOWN,0);
			break;
		case ShutDown::opRestart:
			ExitWindowsEx(EWX_REBOOT,0);
			break;
		case ShutDown::opLogOff:
			ExitWindowsEx(EWX_LOGOFF,0);
			break;
	}
}

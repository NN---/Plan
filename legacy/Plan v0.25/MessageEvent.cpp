// MessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Plan.h"
#include "MessageEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageEvent dialog


CMessageEvent::CMessageEvent(CWnd* pParent)
	: CDialog(CMessageEvent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageEvent)
	m_Caption = _T("");
	m_Text = _T("");
	//}}AFX_DATA_INIT
}


void CMessageEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageEvent)
	DDX_Text(pDX, IDC_CAPTION, m_Caption);
	DDV_MaxChars(pDX, m_Caption, MESSAGE_CAPTION_CHARS);
	DDX_Text(pDX, IDC_TEXT, m_Text);
	DDV_MaxChars(pDX, m_Text, MESSAGE_TEXT_CHARS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageEvent, CDialog)
	//{{AFX_MSG_MAP(CMessageEvent)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageEvent message handlers

void CMessageEvent::OnOK()
{
}

void CMessageEvent::OnCancel()
{
}

void CMessageEvent::Clear()
{
	m_Text.Empty();
	m_Caption.Empty();
	UpdateData(FALSE);
}

const BOOL CMessageEvent::IsClear()
{
	if(m_Text.GetLength()&&
		m_Caption.GetLength())
		return FALSE;
	else
		return TRUE;
}

void CMessageEvent::GetEvent(Message *pMessage)
{
	if(pMessage==NULL)
		return;
	
	UpdateData(TRUE);

	lstrcpy(pMessage->Caption(),m_Caption);
	lstrcpy(pMessage->Text(),m_Text);
}

void CMessageEvent::SetEvent(Message *pMessage)
{
	if(pMessage==NULL)
		return;

	m_Caption=pMessage->Caption();
	m_Text=pMessage->Text();

	UpdateData(FALSE);
}

void CMessageEvent::DoEvent(Message &rMessage)
{
	MessageBox(rMessage.Text(),rMessage.Caption(),MB_OK|MB_APPLMODAL);
}

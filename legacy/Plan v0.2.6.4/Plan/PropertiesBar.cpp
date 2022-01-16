#include "StdAfx.h"
#include "PropertiesBar.h"

CPropertiesBar::CPropertiesBar()
{
	::GetLocalTime(&m_Data.m_Time);
	m_iCurSel = CB_ERR;
}

// Virtual Functions
void CPropertiesBar::GetData(Data &/*rData*/)
{
}

void CPropertiesBar::SetData(const Data &/*rData*/,PCEventID /*pcEventID*/, const LPBYTE /*pData*/)
{
}

// Message
// WM_INITDIALOG
BOOL CPropertiesBar::OnInitDialog(UINT /*uMsg*/,WPARAM /*wParam*/,LPARAM /*lParam*/,BOOL &/*bHandled*/)
{
	return 0;
}

// WM_DESTROY
BOOL CPropertiesBar::OnDestroy(UINT /*uMsg*/,WPARAM /*wParam*/,LPARAM /*lParam*/,BOOL &/*bHandled*/)
{
	return 0;
}
// Defines.cpp
//

#include "stdafx.h"
#include "Defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Data Exchange
// Date and Time
void AFXAPI DDX_DateTime(CDataExchange *pDX, const int nIDDate, const int nIDTime, CTime &rTime)
{
	HWND hTime = pDX->PrepareCtrl(nIDTime);
	HWND hDate = pDX->PrepareCtrl(nIDDate);
	BOOL bDate;

	CDateTimeCtrl* pTime = (CDateTimeCtrl*) CWnd::FromHandle(hTime);
	if(!pTime)
		return;
	CDateTimeCtrl* pDate = (CDateTimeCtrl*) CWnd::FromHandle(hDate);
	if(!pDate)
		return;
	bDate=pDate->IsWindowEnabled()?TRUE:FALSE;

	if(pDX->m_bSaveAndValidate)
	{
		CTime tmptime;
		pTime->GetTime(tmptime);
		if(bDate)
		{
			CTime tmpdate;
			pDate->GetTime(tmpdate);
			rTime=CTime(tmpdate.GetYear(),tmpdate.GetMonth(),tmpdate.GetDay(),
			tmptime.GetHour(),tmptime.GetMinute(),tmptime.GetSecond());
		}
		else
			rTime=tmptime;
	}
	else
	{
		pTime->SetTime(&rTime);
		if(bDate)
			pDate->SetTime(&rTime);
	}
}

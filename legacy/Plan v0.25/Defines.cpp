// Defines.cpp
//

#include "stdafx.h"
#include "Defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// DataExchange
// Time and Date
void AFXAPI DDX_DT(CDataExchange *pDX, const int nIDDate, const int nIDTime, CTime &rTime)
{
	HWND hTime = pDX->PrepareCtrl(nIDTime);
	HWND hDate = pDX->PrepareCtrl(nIDDate);

	CDateTimeCtrl* pTime = (CDateTimeCtrl*) CWnd::FromHandle(hTime);
	CDateTimeCtrl* pDate = (CDateTimeCtrl*) CWnd::FromHandle(hDate);

	CTime tmptime=rTime;
	CTime tmpdate=rTime;

	if(pDX->m_bSaveAndValidate)
	{
		pTime->GetTime(tmptime);
		pDate->GetTime(tmpdate);
		rTime=CTime(tmpdate.GetYear(),tmpdate.GetMonth(),tmpdate.GetDay(),
			tmptime.GetHour(),tmptime.GetMinute(),tmptime.GetSecond());
	}
	else
	{
		pTime->SetTime(&tmptime);
		pDate->SetTime(&tmpdate);
	}
}

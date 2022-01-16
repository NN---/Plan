// Defines.h
//

#ifndef __DEFINES_INCLUDED__
#define __DEFINES_INCLUDED__

// Data Exchange
// Date and Time
inline void AFXAPI DDX_DateTime(CDataExchange *pDX, const int nIDDate, const int nIDTime, CTime &rTime)
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

#endif // __DEFINES_INCLUDED__

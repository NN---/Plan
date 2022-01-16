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

// struct Schedule
struct Schedule
{
	unsigned Once:1;
	unsigned Sunday:1;
	unsigned Monday:1;
	unsigned Tuesday:1;
	unsigned Wednesday:1;
	unsigned Thursday:1;
	unsigned Friday:1;
	unsigned Saturday:1;

	void Set(unsigned uOnce=0,
		unsigned uSunday=0,
		unsigned uMonday=0,
		unsigned uTuesday=0,
		unsigned uWednesday=0,
		unsigned uThursday=0,
		unsigned uFriday=0,
		unsigned uSaturday=0)
	{
		Once=uOnce;
		Sunday=uSunday;
		Monday=uMonday;
		Tuesday=uTuesday;
		Wednesday=uWednesday;
		Thursday=uThursday;
		Friday=uFriday;
		Saturday=uSaturday;
	}

	Schedule &operator=(Schedule &rSchedule)
	{
		Once=rSchedule.Once;
		Sunday=rSchedule.Sunday;
		Monday=rSchedule.Monday;
		Tuesday=rSchedule.Tuesday;
		Wednesday=rSchedule.Wednesday;
		Thursday=rSchedule.Thursday;
		Friday=rSchedule.Friday;
		Saturday=rSchedule.Saturday;
		return *this;
	}

	void Get(unsigned *pOnce=NULL,
		unsigned *pSunday=NULL,
		unsigned *pMonday=NULL,
		unsigned *pTuesday=NULL,
		unsigned *pWednesday=NULL,
		unsigned *pThursday=NULL,
		unsigned *pFriday=NULL,
		unsigned *pSaturday=NULL)
	{
		if(pOnce)
			*pOnce=Once;
		if(pSunday)
			*pSunday=Sunday;
		if(pMonday)
			*pMonday=Monday;
		if(pTuesday)
			*pTuesday=Tuesday;
		if(pWednesday)
			*pWednesday=Wednesday;
		if(pThursday)
			*pThursday=Thursday;
		if(pFriday)
			*pFriday=Friday;
		if(pSaturday)
			*pSaturday=Saturday;
	}

};

#endif // __DEFINES_INCLUDED__

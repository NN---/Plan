#ifndef __SYSTEMTIME_H__
#define __SYSTEMTIME_H__

#pragma once

/*
inline const SYSTEMTIME operator+(const SYSTEMTIME &st1,const SYSTEMTIME &st2)
{
		ULARGE_INTEGER uli1,uli2,uli;
		SYSTEMTIME st;
		SystemTimeToFileTime(&st1,(LPFILETIME)&uli1);
		SystemTimeToFileTime(&st2,(LPFILETIME)&uli2);
		uli.QuadPart=uli1.QuadPart+uli2.QuadPart;
		FileTimeToSystemTime((const LPFILETIME)&uli,&st);
		return st;
}
*/

inline const SYSTEMTIME operator-(const SYSTEMTIME &st1,const SYSTEMTIME &st2)
{
		ULARGE_INTEGER uli1,uli2,uli;
		SYSTEMTIME st;
		SystemTimeToFileTime(&st1,(LPFILETIME)&uli1);
		SystemTimeToFileTime(&st2,(LPFILETIME)&uli2);
		uli.QuadPart=uli1.QuadPart-uli2.QuadPart;
		FileTimeToSystemTime((const LPFILETIME)&uli,&st);
		return st;
}

#endif // __SYSTEMTIME_H__
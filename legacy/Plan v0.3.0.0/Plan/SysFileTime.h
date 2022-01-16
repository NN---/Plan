#ifndef __NN_SYSFILETIME_H__
#define __NN_SYSFILETIME_H__

#pragma once

namespace NN
{

struct SystemTime : public SYSTEMTIME
{
	SystemTime() {}
	operator const FILETIME()
	{
		FILETIME ft;
		SystemTimeToFileTime(this,&ft);
		return ft;
	}
	operator const SYSTEMTIME() const
	{ return *this; }
	operator const SYSTEMTIME&() const
	{ return *this; }
	operator const SYSTEMTIME*() const
	{ return this; }
	const SYSTEMTIME operator=(const SYSTEMTIME &rST)
	{
		FILETIME ft;
		SystemTimeToFileTime(&rST,&ft);
		FileTimeToSystemTime(&ft,this);
		return *this;
	}
	const SYSTEMTIME &operator-(const SYSTEMTIME &rST)
	{
		FILETIME ftthis,ft;
		SystemTimeToFileTime(this,&ftthis);
		SystemTimeToFileTime(&rST,&ft);
		*((unsigned __int64 *)&ftthis)=*((unsigned __int64 *)&ftthis)-*((unsigned __int64 *)&ft);
		FileTimeToSystemTime(&ftthis,this);
		return *this;
	}
	const SYSTEMTIME &operator+(const SYSTEMTIME &rST)
	{
		FILETIME ftthis,ft;
		SystemTimeToFileTime(this,&ftthis);
		SystemTimeToFileTime(&rST,&ft);
		*((unsigned __int64 *)&ftthis)=*((unsigned __int64 *)&ftthis)+*((unsigned __int64 *)&ft);
		FileTimeToSystemTime(&ftthis,this);
		return *this;
	}
	const bool operator==(const SYSTEMTIME &rST)
	{
		FILETIME ftthis,ft;
		SystemTimeToFileTime(this,&ftthis);
		SystemTimeToFileTime(&rST,&ft);
		return *((unsigned __int64 *)&ftthis)==*((unsigned __int64 *)&ft);
	}
	const bool operator!=(const SYSTEMTIME &rST)
	{
		FILETIME ftthis,ft;
		SystemTimeToFileTime(this,&ftthis);
		SystemTimeToFileTime(&rST,&ft);
		return *((unsigned __int64 *)&ftthis)!=*((unsigned __int64 *)&ft);
	}
	const bool operator<(const SYSTEMTIME &rST)
	{
		FILETIME ftthis,ft;
		SystemTimeToFileTime(this,&ftthis);
		SystemTimeToFileTime(&rST,&ft);
		return *((unsigned __int64 *)&ftthis)<*((unsigned __int64 *)&ft);
	}
	const bool operator<=(const SYSTEMTIME &rST)
	{
		FILETIME ftthis,ft;
		SystemTimeToFileTime(this,&ftthis);
		SystemTimeToFileTime(&rST,&ft);
		return *((unsigned __int64 *)&ftthis)<=*((unsigned __int64 *)&ft);
	}
	const bool operator>(const SYSTEMTIME &rST)
	{
		FILETIME ftthis,ft;
		SystemTimeToFileTime(this,&ftthis);
		SystemTimeToFileTime(&rST,&ft);
		return *((unsigned __int64 *)&ftthis)>*((unsigned __int64 *)&ft);
	}
	const bool operator>=(const SYSTEMTIME &rST)
	{
		FILETIME ftthis,ft;
		SystemTimeToFileTime(this,&ftthis);
		SystemTimeToFileTime(&rST,&ft);
		return *((unsigned __int64 *)&ftthis)>=*((unsigned __int64 *)&ft);
	}
};

struct FileTime : public FILETIME
{
	FileTime() {}
	operator const FILETIME()
	{ return *this; }
	operator const FILETIME*() const
	{ return this; }
	operator const unsigned __int64() const 
	{ return *((const unsigned __int64 *)this); }
	operator const SYSTEMTIME()
	{
		SYSTEMTIME st;
		FileTimeToSystemTime(this,&st);
		return st;
	}
	const FILETIME operator=(const FILETIME &rFT)
	{
		*((unsigned __int64 *)this)=*((unsigned __int64 *)&rFT);
		return *this;
	}
	const FILETIME &operator-(const FILETIME &rFT)
	{ return *(const FILETIME *)(*((unsigned __int64 *)this)-*((unsigned __int64 *)&rFT)); }
	const FILETIME &operator+(const FILETIME &rFT)
	{ return *(const FILETIME *)(*((unsigned __int64 *)this)+*((unsigned __int64 *)&rFT)); }
	const bool operator==(const FILETIME &rFT)
	{ return *((unsigned __int64 *)this)==*((unsigned __int64 *)&rFT); }
	const bool operator!=(const FILETIME &rFT)
	{ return *((unsigned __int64 *)this)!=*((unsigned __int64 *)&rFT); }
	const bool operator<(const FILETIME &rFT)
	{ return *((unsigned __int64 *)this)<*((unsigned __int64 *)&rFT); }
	const bool operator<=(const FILETIME &rFT)
	{ return *((unsigned __int64 *)this)<=*((unsigned __int64 *)&rFT); }
	const bool operator>(const FILETIME &rFT)
	{ return *((unsigned __int64 *)this)>*((unsigned __int64 *)&rFT); }
	const bool operator>=(const FILETIME &rFT)
	{ return *((unsigned __int64 *)this)>=*((unsigned __int64 *)&rFT); }
};

}

#endif // __NN_SYSFILETIME_H__
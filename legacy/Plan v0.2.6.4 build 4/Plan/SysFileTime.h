#ifndef __SYSFILETIME_H__
#define __SYSFILETIME_H__

#pragma once

#ifndef __NOSYSFILETIME_OPERATORS
const SYSTEMTIME operator-(const SYSTEMTIME &rST1,const SYSTEMTIME &rST2)
{
	FILETIME ft1,ft2;
	SYSTEMTIME st;
	SystemTimeToFileTime(&rST1,&ft1);
	SystemTimeToFileTime(&rST2,&ft2);
	*((unsigned __int64 *)&ft1)=*((unsigned __int64 *)&ft1)-*((unsigned __int64 *)&ft2);
	FileTimeToSystemTime(&ft1,&st);
	return st;
}

const SYSTEMTIME operator+(const SYSTEMTIME &rST1,const SYSTEMTIME &rST2)
{
	FILETIME ft1,ft2;
	SYSTEMTIME st;
	SystemTimeToFileTime(&rST1,&ft1);
	SystemTimeToFileTime(&rST2,&ft2);
	*((unsigned __int64 *)&ft1)=*((unsigned __int64 *)&ft1)+*((unsigned __int64 *)&ft2);
	FileTimeToSystemTime(&ft1,&st);
	return st;
}

const FILETIME operator-(const FILETIME &rFT1,const FILETIME &rFT2)
{
	return *((FILETIME *)(*((unsigned __int64 *)&rFT1)-*((unsigned __int64 *)&rFT2)));
}

const FILETIME operator+(const FILETIME &rFT1,const FILETIME &rFT2)
{
	return *((FILETIME *)(*((unsigned __int64 *)&rFT1)+*((unsigned __int64 *)&rFT2)));
}
#endif // __NOSYSFILETIME_OPERATORS

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

#endif // __SYSFILETIME_H__
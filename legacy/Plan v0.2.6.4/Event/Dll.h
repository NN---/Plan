//
// Dll v0.2.6.4
//

#pragma once

#ifndef __DLLExportImport__
#ifdef __cplusplus
	#define DLLExport_C extern "C" __declspec(dllexport)
	#define DLLExport_CPP __declspec(dllexport)
	#define DLLImport_C extern "C" __declspec(dllimport)
	#define DLLImport_CPP __declspec(dllimport)
	#ifdef DLLC
		#define DLLExport DLLExport_C
		#define DLLImport DLLImport_C
	#else // !DLLC
		#define DLLExport DLLExport_CPP
		#define DLLImport DLLImport_CPP
	#endif // DLLC
#else // !__cplusplus
	#define DLLExport_C __declspec(dllexport)
	#define DLLExport_CPP DLLExport_C
	#define DLLExport DLLExport_C
	#define DLLImport_C __declspec(dllimport)
	#define DLLImport_CPP DLLImport_C
	#define DLLImport DLLImport_C
#endif // __cplusplus
#endif // __DLLExportImport__

#ifndef DLL_NO_DEFINE
// Name Pointer to Function
#define NPF(func) __p##func
// Type Pointer to Function
#define TPF(func) NPF(func)##Type__
// Define Pointer to Function
#define DPF(func) TPF(func) NPF(func)
// Name <= GetProcAddress
#define NGPA(hModule,func) NPF(func) = (TPF(func))GetProcAddress(hModule,#func)
// Define GetProcAddress
#define DGPA(hModule,func) DPF(func) = (TPF(func))GetProcAddress(hModule,#func)
// Return Define GetProcAddress
#define RDGPA(hModule,func) DGPA(hModule,func); return NPF(func)
// call Function Name GetProcAddress
#define FNGPA(hModule,func) NGPA(hModule,func); NPF(func)
// call Function Define GetProcAddress
#define FDGPA(hModule,func) DGPA(hModule,func); NPF(func)

// DEFine Pointer to Function
#define DEFPF(type,func, p) \
	typedef type (*TPF(func))( /p/ )

#define DEFPF0(type,func) \
	typedef type (*TPF(func))(void)

#define DEFPF1(type,func,p1) \
	typedef type (*TPF(func))(p1)

#define DEFPF2(type,func,p1,p2) \
	typedef type (*TPF(func))(p1,p2)
#endif // DLL_NO_DEFINE

#ifdef __cplusplus
namespace NN
{
#ifndef DLL_NO_DEFINE

#define DLL_DEFINEFUNC_EX(type, func, funcname, p) \
	type func( /p/ ) \
	{ \
		RDGPA(m_hModule,funcname)( /p/ ); \
	}

#define DLL_DEFINEFUNC(type, func, p) DLL_DEFINEFUNC_EX(type,func,func,p)

#define DLL_DEFINEFUNC0_EX(type, func, funcname) \
	type func( void ) \
	{ \
		RDGPA(m_hModule,funcname)(); \
	}

#define DLL_DEFINEFUNC0(type, func) DLL_DEFINEFUNC0_EX(type,func,func)

#define DLL_DEFINEFUNC1_EX(type, func, funcname, t1 ,p1) \
	type func( t1 p1 ) \
	{ \
		RDGPA(m_hModule,funcname)(p1); \
	}

#define DLL_DEFINEFUNC1(type, func, t1 ,p1) DLL_DEFINEFUNC1_EX(type, func,func,t1,p1)

#define DLL_DEFINEFUNC2_EX(type, func, funcname, t1 ,p1, t2 , p2) \
	type func( t1 p1 , t2 p2 ) \
	{ \
		RDGPA(m_hModule,funcname)(p1 ,p2); \
	}

#define DLL_DEFINEFUNC2(type, func, t1 ,p1, t2,p2) DLL_DEFINEFUNC2_EX(type, func,func,t1,p1,t2,p2)

#define DLL_DEFINEFUNC3_EX(type, func, funcname, t1 ,p1, t2 , p2, t3 ,p3) \
	type func( t1 p1 , t2 p2 , t3 p3) \
	{ \
		RDGPA(m_hModule,funcname)(p1, p2 ,p3); \
	}

#define DLL_DEFINEFUNC3(type, func, t1 ,p1, t2,p2,t3,p3) DLL_DEFINEFUNC3_EX(type, func,func,t1,p1,t2,p2,t3,p3)

#define DLL_DEFINEFUNC_VOID_EX(func, funcname, p) \
	void func( /p/ ) \
	{ \
		FDGPA(m_hModule,funcname)( /p/ ); \
	}

#define DLL_DEFINEFUNC_VOID(func,p) DLL_DEFINEFUNC_VOID_EX(func,func,p)

#define DLL_DEFINEFUNC0_VOID_EX(func,funcname) \
	void func( void ) \
	{ \
		FDGPA(m_hModule,funcname)(); \
	}

#define DLL_DEFINEFUNC0_VOID(func) DLL_DEFINEFUNC0_VOID_EX(func,func)

#define DLL_DEFINEFUNC1_VOID_EX(func, funcname, t1 ,p1) \
	void func( t1 p1 ) \
	{ \
		FDGPA(m_hModule,funcname)(p1); \
	}

#define DLL_DEFINEFUNC1_VOID(func, t1 ,p1) DLL_DEFINEFUNC1_VOID_EX(func,func,t1,p1)

#define DLL_DEFINEFUNC2_VOID_EX(func, funcname, t1 ,p1, t2 , p2) \
	void func( t1 p1 , t2 p2 ) \
	{ \
		FDGPA(m_hModule,funcname)(p1, p2); \
	}

#define DLL_DEFINEFUNC2_VOID(func, t1 ,p1,t2,p2) DLL_DEFINEFUNC2_VOID_EX(func,func,t1,p1,t2,p2)

#define DLL_DEFINEFUNC3_VOID_EX(func, funcname, t1 ,p1, t2 , p2, t3 ,p3) \
	void func( t1 p1 , t2 p2 , t3 p3) \
	{ \
		FDGPA(m_hModule,funcname)(p1, p2, p3); \
	}

#define DLL_DEFINEFUNC3_VOID(func, t1 ,p1,t2,p2,t3,p3) DLL_DEFINEFUNC3_VOID_EX(func,func,t1,p1,t2,p2,t3,p3)

#endif // DLL_NO_DEFINE

class Dll
{
protected:
	HMODULE m_hModule;
	virtual bool IsDllValid(const HMODULE /*hModule*/) { return true; }
public:
	Dll()
	{
		m_hModule = NULL;
	}
	Dll(const HMODULE hModule)
	{
		SetHandle(hModule);
	}
	Dll(const Dll &rDll)
	{
		SetHandle(rDll.m_hModule);
	}
	~Dll()
	{
		FreeLibrary();
	}
	bool LoadLibrary(LPCTSTR lpLibFileName)
	{
		HMODULE hModule;
		hModule=::LoadLibrary(lpLibFileName);
		if(!SetHandle(hModule))
		{
			::FreeLibrary(hModule);
			return false;
		}
		return true;
	}
	void FreeLibrary()
	{
		::FreeLibrary(m_hModule);
		m_hModule=NULL;
	}
	bool SetHandle(const HMODULE hModule)
	{
		if(!IsDllValid(hModule))
			return false;
		FreeLibrary();
		m_hModule = hModule;
		return true;
	}
	const HMODULE GetHandle()
	{
		return m_hModule;
	}
	operator HMODULE() const { return m_hModule; }
private:
	void operator=(Dll &) { }
};

template <class TDll = Dll>
class Dlls
{
	CSimpleArray<TDll> m_aDll;
	int m_iDll;
public:
	Dlls()
	{
		Init();
	}
	~Dlls()
	{
		Destroy();
	}
	virtual void Init()
	{
		m_aDll.RemoveAll();
		m_iDll=0;
	}
	virtual void Destroy()
	{
		UnloadDlls();
	}
	void SetWorkDll(int iDll)
	{
		ATLASSERT(iDll<m_aDll.GetSize());
		m_iDll = iDll;
	}
	int GetWorkDll(void)
	{
		return m_iDll;
	}
	void UnloadDlls()
	{
		int i,n;
		for(i=0,n=m_aDll.GetSize();i<n;i++)
			m_aDll[i].FreeLibrary();
		Init();
	}
	virtual int FindDll(HMODULE hModule, bool bSet = false)
	{
		int i,n;
		for(i=0,n=m_aDll.GetSize();i<n;i++)
			if(m_aDll[i].GetHandle()==hModule)
			{
				if(bSet)
					m_iDll=i;
				return i;
			}
		return -1;
	}
	int GetDllCount()
	{
		return m_aDll.GetSize();
	}
	TDll &GetDll()
	{
		return m_aDll[m_iDll];
	}
	void AddDll(const TDll &rDll)
	{
		m_aDll.Add(rDll);
	}
	void RemoveDll(const TDll &rDll)
	{
		int iDll=FindDll(rDll.GetHandle());
		if(iDll==-1)
			return;
		if(m_iDll&&dwDll==m_iDll)
			m_iDll--;
		m_aDll.RemoveAt(iDll);
	}
	void RemoveAll()
	{
		Destroy();
	}
private:
	void operator=(Dlls &) { }
	Dlls(const Dlls &) { }
};

};
#endif // __cplusplus
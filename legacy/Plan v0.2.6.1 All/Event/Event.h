// Event v0.2.6.1

#ifndef __EVENT_IMPORT_EXPORT__
#define __EVENT_IMPORT_EXPORT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
	#define DLLExport extern "C" __declspec(dllexport)
#else // !__cplusplus
	#define DLLExport __declspec(dllexport)
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// EventID
struct tagEventID
{
	BYTE nID;
	BYTE sID[11];
};

typedef struct tagEventID EventID;
typedef const struct tagEventID * PCEventID;

#ifdef __cplusplus
}
#endif // __cplusplus

#if !defined(EVENT_EXPORT) && !defined(EVENT_IMPORT)
	#define EVENT_EXPORT
#endif // !EVENT_EXPORT && !EVENT_IMPORT

#ifdef EVENT_EXPORT
	
	DLLExport BOOL Load(HWND hWndParent);
	DLLExport BOOL Unload(void);
	DLLExport BOOL Move(RECT rect, BOOL bRepaint);
	DLLExport BOOL Show(BOOL bShow);
	DLLExport BOOL IsEmpty(void);
	DLLExport BOOL Empty(void);
	DLLExport BOOL Do(const LPBYTE pData);
	DLLExport PCEventID GetID(void);
	DLLExport LPCTSTR GetName(void);
	DLLExport BOOL GetData(LPBYTE pData);
	DLLExport void SetData(const LPBYTE pData);
	DLLExport UINT GetDataSize(void);

#else  // defined(EVENT_IMPORT)
	// Helper Macros

	// Name Pointer to Function
	#define NPF(fname) p##fname
	// Type Pointer to Function
	#define TPF(fname) NPF(fname)##Type
	// Define Pointer to Function
	#define DPF(fname) TPF(fname) NPF(fname)
	// Name <= GetProcAddress
	#define NGPA(hModule,fname) NPF(fname) = (TPF(fname))GetProcAddress(hModule,#fname)
	// Define GetProcAddress
	#define DGPA(hModule,fname) DPF(fname) = (TPF(fname))GetProcAddress(hModule,#fname)
	// Return Define GetProcAddress
	#define RDGPA(hModule,fname) DGPA(hModule,fname); return NPF(fname)
	// call Function Name GetProcAddress
	#define FNGPA(hModule,fname) NGPA(hModule,fname); NPF(fname)
	// call Function Define GetProcAddress
	#define FDGPA(hModule,fname) DGPA(hModule,fname); NPF(fname)
	
	// DEFine Pointer to Function
	#define DEFPF0(type,fname) \
		typedef type (*TPF(fname))(void)

	#define DEFPF1(type,fname,p1) \
		typedef type (*TPF(fname))(p1)

	#define DEFPF2(type,fname,p1,p2) \
		typedef type (*TPF(fname))(p1,p2)

	DEFPF1(BOOL,Load,HWND hWndParent);
	DEFPF0(BOOL,Unload);
	DEFPF2(BOOL,Move,RECT rect,BOOL bRepaint);
	DEFPF1(BOOL,Show,BOOL bShow);
	DEFPF0(BOOL,IsEmpty);
	DEFPF0(BOOL,Empty);
	DEFPF1(BOOL,Do,const LPBYTE pData);
	DEFPF0(PCEventID,GetID);
	DEFPF0(LPCTSTR,GetName);
	DEFPF1(BOOL,GetData,LPBYTE pData);
	DEFPF1(void,SetData,const LPBYTE pData);
	DEFPF0(UINT,GetDataSize);

	// CheckEventLibrary
	#ifdef __cplusplus
		inline
	#endif
	BOOL CheckEventLibrary(HMODULE hModule)
	{
		DPF(Load);
		DPF(Unload);
		DPF(Move);
		DPF(Show);
		DPF(IsEmpty);
		DPF(Empty);
		DPF(Do);
		DPF(GetID);
		DPF(GetName);
		DPF(GetData);
		DPF(SetData);
		DPF(GetDataSize);

		if(!hModule)
			return FALSE;
		
		NGPA(hModule,Load);
		NGPA(hModule,Unload);
		NGPA(hModule,Move);
		NGPA(hModule,Show);
		NGPA(hModule,IsEmpty);
		NGPA(hModule,Empty);
		NGPA(hModule,Do);
		NGPA(hModule,GetID);
		NGPA(hModule,GetName);
		NGPA(hModule,GetData);
		NGPA(hModule,SetData);
		NGPA(hModule,GetDataSize);

		return
			pLoad	&&
			pUnload &&
			pMove	&&
			pShow	&&
			pIsEmpty&&
			pEmpty	&&
			pDo		&&
			pGetID  &&
			pGetName&&
			pGetData&&
			pSetData&&
			pGetDataSize;
	}

#if defined(__cplusplus)

#define EVENTLIB_FUNC0(type,fname) \
	type fname() \
	{ \
		ATLASSERT(dwEventLib<=nEventLib); \
		RDGPA(phEventLib[dwEventLib],fname)(); \
	}
#define EVENTLIB_FUNC1(type,fname,t1,p1) \
	type fname(t1 p1) \
	{ \
		ATLASSERT(dwEventLib<=nEventLib); \
		RDGPA(phEventLib[dwEventLib],fname)(p1); \
	}
#define EVENTLIB_FUNC2(type,fname,t1,p1,t2,p2) \
	type fname(t1 p1,t2 p2) \
	{ \
		ATLASSERT(dwEventLib<=nEventLib); \
		RDGPA(phEventLib[dwEventLib],fname)(p1,p2); \
	}

#define EVENTLIB_NFUNC0(fname) \
	void fname() \
	{ \
		ATLASSERT(dwEventLib<=nEventLib); \
		FDGPA(phEventLib[dwEventLib],fname)(); \
	}
#define EVENTLIB_NFUNC1(fname,t1,p1) \
	void fname(t1 p1) \
	{ \
		ATLASSERT(dwEventLib<=nEventLib); \
		FDGPA(phEventLib[dwEventLib],fname)(p1); \
	}
#define EVENTLIB_NFUNC2(fname,t1,p1,t2,p2) \
	void fname(t1 p1,t2 p2) \
	{ \
		ATLASSERT(dwEventLib<=nEventLib); \
		FDGPA(phEventLib[dwEventLib],fname)(p1,p2); \
	}


#define EVENTLIB_GET(type,fname,p) \
	type fname() \
	{ \
		return p; \
	}

#define EVENTLIB_GET_A(type,fname,p) \
	type fname() \
	{ \
		return p[dwEventLib]; \
	}

// class EventLib
class EventLib
{
private:
	DWORD dwEventLib;
	DWORD nEventLib;
	HMODULE *phEventLib;
	PCEventID *pEventID;
	LPCTSTR *pEventName;
public:
	EventLib();
	~EventLib();
	void Init();
	void Destroy();
	void SetWorkEvent(DWORD dwNewEventLib);
	DWORD GetWorkEvent();
	DWORD LoadEvents(HWND hWndParent, LPCTSTR lpszDirectory = NULL);
	void UnloadEvents();
	EVENTLIB_GET_A(PCEventID,GetEventID,pEventID)
	EVENTLIB_GET(DWORD,GetEventCount,nEventLib)
	EVENTLIB_GET_A(LPCTSTR,GetEventName,pEventName)
	DWORD Find(PCEventID pcEventID, BOOL bSet = FALSE);
	EVENTLIB_FUNC2(BOOL,Move,RECT,rect,BOOL,bRepaint)
	EVENTLIB_FUNC1(BOOL,Show,BOOL,bShow)
	EVENTLIB_FUNC0(BOOL,IsEmpty)
	EVENTLIB_FUNC0(BOOL,Empty)
	EVENTLIB_FUNC1(BOOL,Do,const LPBYTE,pData)
	EVENTLIB_FUNC1(BOOL,GetData,LPBYTE,pData)
	EVENTLIB_NFUNC1(SetData,const LPBYTE,pData)
	EVENTLIB_FUNC0(UINT,GetDataSize)
private:
	void operator=(EventLib &) const { };
	EventLib(EventLib &) { }
};

inline EventLib::EventLib()
{
	Init();
}

inline EventLib::~EventLib()
{
	Destroy();
}

inline void EventLib::Init()
{
	dwEventLib = 0;
	nEventLib = 0;
	phEventLib = NULL;
	pEventID = NULL;
	pEventName = NULL;
}

inline void EventLib::Destroy()
{
	UnloadEvents();
}

inline void EventLib::SetWorkEvent(DWORD dwNewEventLib)
{
	ATLASSERT(dwEventLib<=nEventLib);
	dwEventLib = dwNewEventLib;
}

inline DWORD EventLib::GetWorkEvent()
{
	return dwEventLib;
}

inline DWORD EventLib::LoadEvents(HWND hWndParent, LPCTSTR lpszDirectory /* = NULL*/)
{
	if(!hWndParent)
		return 0;

	TCHAR szCurDir[_MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile;

	if(lpszDirectory)
	{
		::GetCurrentDirectory(_MAX_PATH,szCurDir);
		::SetCurrentDirectory(lpszDirectory);
	}

	hFindFile = ::FindFirstFile(_T("*.dll"),&FindFileData);
	
	if(hFindFile!=INVALID_HANDLE_VALUE)
	{
		HMODULE hModule;
		DPF(Load);
		DPF(GetID);
		DPF(GetName);
		do{
			// LoadLibrary
			hModule=::LoadLibrary(FindFileData.cFileName);
			// Check return value
			if(!hModule)
				continue;
			// CheckEventLibrary
			if(!CheckEventLibrary(hModule))
			{
				::FreeLibrary(hModule);
				continue;
			}
			// LoadEvent
			NGPA(hModule,Load);
			if(!NPF(Load)(hWndParent))
			{
				::FreeLibrary(hModule);
				continue;
			}
			// Allocate memory for hModule , ID , Name
			nEventLib++;
			
			phEventLib=(HMODULE *)realloc((void *)phEventLib,nEventLib*sizeof(HMODULE));
			phEventLib[nEventLib-1]=hModule;

			NGPA(hModule,GetID);
			pEventID=(PCEventID *)realloc((void *)pEventID,nEventLib*sizeof(PCEventID));
			pEventID[nEventLib-1]=NPF(GetID)();

			NGPA(hModule,GetName);
			pEventName=(LPCTSTR *)realloc((void *)pEventName,nEventLib*sizeof(LPCTSTR));
			pEventName[nEventLib-1]=NPF(GetName)();

		}while(::FindNextFile(hFindFile,&FindFileData));
		::FindClose(hFindFile);
	}
	
	if(lpszDirectory)
		::SetCurrentDirectory(szCurDir);

	return nEventLib;
}

inline void EventLib::UnloadEvents()
{
	DPF(Unload);
	DWORD i;
	for(i=0;i<nEventLib;i++)
	{
		FNGPA(phEventLib[i],Unload)();
	}
	
	for(i=0;i<nEventLib;i++)
		::FreeLibrary(phEventLib[i]);
	free(phEventLib);
	free(pEventID);
	free(pEventName);
	Init();
}

inline DWORD EventLib::Find(PCEventID pcEventID, BOOL bSet)
{
	DWORD i;
	for(i=0;i<nEventLib;i++)
		if(!memcmp(pEventID[i],pcEventID,sizeof(EventID)))
		{
			if(bSet)
				SetWorkEvent(i);
			return i;
		}
	return (DWORD)-1;
}

#endif // __cplusplus
#endif // EVENT_EXPORT

#endif // __EVENT_IMPORT_EXPORT__

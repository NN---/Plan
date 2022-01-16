#ifndef __EVENT_IMPORT_EXPORT__
#define __EVENT_IMPORT_EXPORT__

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
		ASSERT(uEventLib<=nEventLib); \
		RDGPA(*(phEventLib+uEventLib),fname)(); \
	}
#define EVENTLIB_FUNC1(type,fname,t1,p1) \
	type fname(t1 p1) \
	{ \
		ASSERT(uEventLib<=nEventLib); \
		RDGPA(*(phEventLib+uEventLib),fname)(p1); \
	}
#define EVENTLIB_FUNC2(type,fname,t1,p1,t2,p2) \
	type fname(t1 p1,t2 p2) \
	{ \
		ASSERT(uEventLib<=nEventLib); \
		RDGPA(*(phEventLib+uEventLib),fname)(p1,p2); \
	}

#define EVENTLIB_NFUNC0(fname) \
	void fname() \
	{ \
		ASSERT(uEventLib<=nEventLib); \
		FDGPA(*(phEventLib+uEventLib),fname)(); \
	}
#define EVENTLIB_NFUNC1(fname,t1,p1) \
	void fname(t1 p1) \
	{ \
		ASSERT(uEventLib<=nEventLib); \
		FDGPA(*(phEventLib+uEventLib),fname)(p1); \
	}
#define EVENTLIB_NFUNC2(fname,t1,p1,t2,p2) \
	void fname(t1 p1,t2 p2) \
	{ \
		ASSERT(uEventLib<=nEventLib); \
		FDGPA(*(phEventLib+uEventLib),fname)(p1,p2); \
	}


// class EventLib
class EventLib
{
private:
	unsigned uEventLib;
	unsigned nEventLib;
	HMODULE *phEventLib;
	PCEventID *pEventID;
	LPCTSTR *pEventName;
public:
	EventLib();
	~EventLib();
	void Init();
	void Destroy();
	void SetWorkEvent(unsigned uNewEventLib);
	unsigned GetWorkEvent();
	int LoadEvents(HWND hWndParent, LPCTSTR lpszDirectory = NULL);
	int LoadEventLibraries(LPCTSTR lpszDirectory);
	void UnloadEvents(BOOL bKeepLibraries = FALSE);
	BOOL SetParent(HWND hWndParent);
	PCEventID GetEventID();
	unsigned GetEventCount();
	LPCTSTR GetEventName();
	unsigned Find(PCEventID pcEventID, BOOL bSet = FALSE);
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
	uEventLib = 0;
	nEventLib = 0;
	phEventLib = NULL;
	pEventID = NULL;
	pEventName = NULL;
}

inline void EventLib::Destroy()
{
	UnloadEvents();
}

inline void EventLib::SetWorkEvent(unsigned uNewEventLib)
{
	ASSERT(uEventLib<=nEventLib);
	uEventLib = uNewEventLib;
}

inline unsigned EventLib::GetWorkEvent()
{
	return uEventLib;
}

inline int EventLib::LoadEventLibraries(LPCTSTR lpszDirectory)
{
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
			// Allocate memory for hModule , ID , Name
			nEventLib++;
			
			phEventLib=(HMODULE *)realloc((void *)phEventLib,nEventLib*sizeof(HMODULE));
			*(phEventLib+nEventLib-1)=hModule;

			NGPA(hModule,GetID);
			pEventID=(PCEventID *)realloc((void *)pEventID,nEventLib*sizeof(PCEventID));
			*(pEventID+nEventLib-1)=NPF(GetID)();

			NGPA(hModule,GetName);
			pEventName=(LPCTSTR *)realloc((void *)pEventName,nEventLib*sizeof(LPCTSTR));
			*(pEventName+nEventLib-1)=NPF(GetName)();

		}while(::FindNextFile(hFindFile,&FindFileData));
		::FindClose(hFindFile);
	}
	
	if(lpszDirectory)
		::SetCurrentDirectory(szCurDir);

	return nEventLib;
}

inline BOOL EventLib::SetParent(HWND hWndParent)
{
	DPF(Load);
	DPF(Unload);
	unsigned i;

	if(!hWndParent)
		return FALSE;
	
	for(i=0;i<nEventLib;i++)
	{
		FNGPA(*(phEventLib+i),Unload)();
		FNGPA(*(phEventLib+i),Load)(hWndParent);
	}

	return TRUE;
}

inline int EventLib::LoadEvents(HWND hWndParent, LPCTSTR lpszDirectory)
{
	if(!hWndParent)
		return 0;
	if(!LoadEventLibraries(lpszDirectory))
		return 0;
	SetParent(hWndParent);
	return nEventLib;
}

inline void EventLib::UnloadEvents(BOOL bKeepLibraries /* = FALSE */)
{
	DPF(Unload);
	unsigned i;
	for(i=0;i<nEventLib;i++)
	{
		FNGPA(*(phEventLib+i),Unload)();
	}
	
	if(!bKeepLibraries)
	{
		for(i=0;i<nEventLib;i++)
			::FreeLibrary(*(phEventLib+i));
		free(phEventLib);
		free(pEventID);
		free(pEventName);
		Init();
	}
}

inline PCEventID EventLib::GetEventID()
{
	ASSERT(uEventLib<=nEventLib);
	return *(pEventID+uEventLib);
}

inline unsigned EventLib::GetEventCount()
{
	return nEventLib;
}

inline LPCTSTR EventLib::GetEventName()
{
	ASSERT(uEventLib<=nEventLib);
	return *(pEventName+uEventLib);
}

inline unsigned EventLib::Find(PCEventID pcEventID, BOOL bSet)
{
	unsigned i;
	for(i=0;i<nEventLib;i++)
		if(!memcmp(*(pEventID+i),pcEventID,sizeof(EventID)))
		{
			if(bSet)
				SetWorkEvent(i);
			return i;
		}
	return (unsigned)-1;
}

#endif // __cplusplus
#endif // EVENT_EXPORT

#endif // __EVENT_IMPORT_EXPORT__

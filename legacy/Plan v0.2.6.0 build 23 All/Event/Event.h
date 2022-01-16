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
	BYTE sID[12];
};

typedef struct tagEventID EventID;
typedef const struct tagEventID * PCEventID;

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef EVENT_IMPORT_DEF
	#define EVENT_IMPORT
	#define EVENT_DEF
#endif // EVENT_EXPORT_DEF

#ifdef EVENT_IMPORT_DEF_LIB
	#define EVENT_IMPORT
	#define EVENT_DEF
	#define EVENT_LIB
#endif // EVENT_EXPORT_DEF_LIB

#if !defined(EVENT_EXPORT) && !defined(EVENT_IMPORT)
	#define EVENT_EXPORT
#endif // !EVENT_EXPORT && !EVENT_IMPORT

#define NF(fname) Event##fname

#ifdef EVENT_EXPORT
	
	DLLExport BOOL NF(Load)(HWND hWndParent);
	DLLExport BOOL NF(Unload)(void);
	DLLExport BOOL NF(Move)(RECT rect, BOOL bRepaint);
	DLLExport BOOL NF(Show)(BOOL bShow);

	DLLExport BOOL NF(IsEmpty)(void);
	DLLExport BOOL NF(Empty)(void);
	DLLExport BOOL NF(Do)(void);

	DLLExport PCEventID NF(GetID)(void);
	DLLExport LPCTSTR NF(GetName)(void);

	DLLExport BOOL NF(GetData)(LPBYTE pData);
	DLLExport void NF(SetData)(const LPBYTE pData);
	DLLExport UINT NF(GetDataSize)(void);

#elif defined(EVENT_IMPORT)
	// p prefix
	#define _P(p) p
	// Name Pointer on Function
	#define NPF(fname) _P(p)NF(fname)
	// Type Pointer on Function
	#define TPF(fname) NPF(fname)##Type
	// Define Pointer on Function
	#define DPF(fname) TPF(fname) NPF(fname)
	// GetProcAddress
	#ifdef _UNICODE
		#define GPA(hModule,fname) (TPF(fname))GetProcAddress(hModule,L"Event"## #fname)
	#else
		#define GPA(hModule,fname) (TPF(fname))GetProcAddress(hModule,"Event"## #fname)
	#endif // UNICODE

	// Define + GetProcAddress
	#define DGPA(hModule,fname) DPF(fname) = GPA(hModule,fname)
	// Name + GetProcAddress
	#define NGPA(hModule,fname)	NPF(fname) = GPA(hModule,fname)
	// call Function + Define + GetProcAddress
	#define FDGPA(hModule,fname) DGPA(hModule,fname); NPF(fname)
	// call Function + Name + GetProcAddress
	#define FNGPA(hModule,fname) NGPA(hModule,fname); NPF(fname)
	// Return + Define + GetProcAddress
	#define RDGPA(hModule,fname) DGPA(hModule,fname); return NPF(fname)
	// Return + Name + GetProcAddress
	#define RNGPA(hModule,fname) NGPA(hModule,fname); return NPF(fname)

	// Define Pointer on Function 0
	#define DEFPF0(type,fname) \
		typedef type (*TPF(fname))(void)
	// Define Pointer on Function 1
	#define DEFPF1(type,fname,t1) \
		typedef type (*TPF(fname))(t1)
	// Define Pointer on Function 2
	#define DEFPF2(type,fname,t1,t2) \
		typedef type (*TPF(fname))(t1,t2)

	// Define Pointer on Function
	#ifdef EVENT_DEF
		DEFPF1(BOOL,Load,HWND);
		DEFPF0(BOOL,Unload);
		DEFPF2(BOOL,Move,RECT,BOOL);
		DEFPF1(BOOL,Show,BOOL);

		DEFPF0(BOOL,IsEmpty);
		DEFPF0(BOOL,Empty);
		DEFPF0(BOOL,Do);

		DEFPF0(PCEventID,GetID);
		DEFPF0(LPCTSTR,GetName);

		DEFPF1(BOOL,GetData,LPBYTE);
		DEFPF1(void,SetData,const LPBYTE);
		DEFPF0(UINT,GetDataSize);

		// CheckEventLibrary
		#ifdef __cplusplus
			inline
		#endif
		BOOL CheckEventLibrary(HMODULE hModule)
		{
			if(hModule)
			{
				DGPA(hModule,Load);
				DGPA(hModule,Unload);
				DGPA(hModule,Move);
				DGPA(hModule,Show);
				
				DGPA(hModule,IsEmpty);
				DGPA(hModule,Empty);
				DGPA(hModule,Do);
				
				DGPA(hModule,GetID);
				DGPA(hModule,GetName);

				DGPA(hModule,GetData);
				DGPA(hModule,SetData);
				DGPA(hModule,GetDataSize);

				if(!NPF(Load)     ||
					!NPF(Unload)  ||
					!NPF(Move)    ||
					!NPF(Show)    ||
					!NPF(IsEmpty) ||
					!NPF(Empty)   ||
					!NPF(Do)      ||
					!NPF(GetID)   ||
					!NPF(GetName) ||
					!NPF(GetData) ||
					!NPF(SetData) ||
					!NPF(GetDataSize))
					return FALSE;
				return TRUE;
			}
			return FALSE;
		}
	#endif // EVENT_DEF
#endif // EVENT_EXPORT

#if defined(__cplusplus) && defined(EVENT_DEF) && defined(EVENT_LIB)

#ifndef ASSERT
#define ASSERT(f) ((void)0)
#define __NOASSERT__
#endif // ASSERT

// class EventLib

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
	//
	void Init();
	void Destroy();
	//
	void SetWorkEvent(unsigned uNewEventLib);
	unsigned GetWorkEvent();
	//
	int LoadEvents(HWND hWndParent, LPCTSTR lpszDirectory = NULL);
	int LoadEventLibraries(LPCTSTR lpszDirectory);
	void UnloadEvents(BOOL bKeepLibraries = FALSE);
	BOOL SetParent(HWND hWndParent);
	//
	PCEventID GetEventID();
	unsigned GetEventCount();
	LPCTSTR GetEventName();
	//
	unsigned Find(PCEventID pcEventID, BOOL bSet = FALSE);
	//
	EVENTLIB_FUNC2(BOOL,Move,RECT,rect,BOOL,bRepaint)
	EVENTLIB_FUNC1(BOOL,Show,BOOL,bShow)
	//
	EVENTLIB_FUNC0(BOOL,IsEmpty)
	EVENTLIB_FUNC0(BOOL,Empty)
	EVENTLIB_FUNC0(BOOL,Do)
	//
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

//
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

//
inline void EventLib::SetWorkEvent(unsigned uNewEventLib)
{
	ASSERT(uEventLib<=nEventLib);
	uEventLib = uNewEventLib;
}

inline unsigned EventLib::GetWorkEvent()
{
	return uEventLib;
}

//
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

	hFindFile = ::FindFirstFile("*.dll",&FindFileData);
	
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

//
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

#ifdef __NOASSERT__
#undef __NOASSERT__
#undef ASSERT
#endif // __NOASSERT__

#endif // __cplusplus && EVENT_DEF && EVENT_LIB 

#endif // __EVENT_IMPORT_EXPORT__

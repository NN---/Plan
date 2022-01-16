#ifndef __EVENT_IMPORT_EXPORT__
#define __EVENT_IMPORT_EXPORT__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
	#define DLLExport extern "C" __declspec(dllexport)
	#define DLLImport extern "C" __declspec(dllimport)
#else // !__cplusplus
	#define DLLExport __declspec(dllexport)
	#define DLLImport __declspec(dllimport)
#endif // __cplusplus

#ifndef EVENTID
#define EVENTID
struct tagEventID
{
	BYTE nID;
	BYTE sID[12];
};

typedef struct tagEventID EventID;
typedef const struct tagEventID * PCEventID;

#endif // EVENTID

#define NF(fname) Event##fname

#ifdef EVENT_IMPORT_EXPLICIT_DEF_EVENTLIB
	#define EVENT_IMPORT_EXPLICIT_DEF
	#define EVENT_EVENTLIB
#endif

#ifdef EVENT_IMPORT_EXPLICIT_DEF
	#define EVENT_IMPORT_EXPLICIT
	#define EVENT_DEF
#endif // EVENT_IMPORT_EXPLICIT_DEF

#ifdef EVENT_IMPORT_EXPLICIT_EVENTLIB
	#define EVENT_IMPORT_EXPLICIT
	#define EVENT_EVENTLIB
#endif

#if !defined(EVENT_EXPORT) && !defined(EVENT_IMPORT_IMPLICIT) && !defined(EVENT_IMPORT_EXPLICIT)
	#define EVENT_EXPORT
#endif // !EVENT_EXPORT && !EVENT_IMPORT_IMPLICIT && !EVENT_IMPORT_EXPLICIT

#if defined(EVENT_EXPORT) || defined(EVENT_IMPORT_IMPLICIT)
	#ifdef EVENT_EXPORT
		#define EVENT_IMPORTEXPORT DLLExport
	#else // EVENT_IMPORT_IMPLICIT
		#define EVENT_IMPORTEXPORT DLLImport
	#endif // EVENT_EXPORT

	EVENT_IMPORTEXPORT BOOL NF(Load)(HWND hWndParent);
	EVENT_IMPORTEXPORT BOOL NF(Unload)(void);
	EVENT_IMPORTEXPORT BOOL NF(Move)(RECT rect, BOOL bRepaint);
	EVENT_IMPORTEXPORT BOOL NF(Show)(BOOL bShow);

	EVENT_IMPORTEXPORT BOOL NF(IsEmpty)(void);
	EVENT_IMPORTEXPORT BOOL NF(Empty)(void);
	EVENT_IMPORTEXPORT BOOL NF(Do)(void);

	EVENT_IMPORTEXPORT PCEventID NF(GetID)(void);
	EVENT_IMPORTEXPORT LPCTSTR NF(GetName)(void);

	EVENT_IMPORTEXPORT BOOL NF(GetData)(LPBYTE pData);
	EVENT_IMPORTEXPORT void NF(SetData)(const LPBYTE pData);
	EVENT_IMPORTEXPORT UINT NF(DataSize)(void);

#elif defined(EVENT_IMPORT_EXPLICIT)
	#define _P(p) p
	#define NPF(fname) _P(p)NF(fname)
	#define TPF(fname) NPF(fname)##Type
	#define DPF(fname) TPF(fname) NPF(fname)
	#define GPA(hModule,fname) (TPF(fname))GetProcAddress(hModule,"Event"## #fname)
	#define DGPA(hModule,fname) DPF(fname) = GPA(hModule,fname)
	#define NGPA(hModule,fname)	NPF(fname) = GPA(hModule,fname)
	#define FDGPA(hModule,fname) DGPA(hModule,fname); NPF(fname)
	#define FNGPA(hModule,fname) NGPA(hModule,fname); NPF(fname)
	#define RDGPA(hModule,fname) DGPA(hModule,fname); return NPF(fname)
	#define RNGPA(hModule,fname) NGPA(hModule,fname); return NPF(fname)

	#define DEFPF0(type,fname) \
		typedef type (*TPF(fname))(void)
	#define DEFPF1(type,fname,t1) \
		typedef type (*TPF(fname))(t1)
	#define DEFPF2(type,fname,t1,t2) \
		typedef type (*TPF(fname))(t1,t2)

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
		DEFPF0(UINT,DataSize);

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
				DGPA(hModule,DataSize);

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
					!NPF(DataSize))
					return FALSE;
				return TRUE;
			}
			return FALSE;
		}
	#endif // EVENT_DEF
#endif // EVENT_EXPORT || EVENT_IMPORT_IMPLICIT

#ifdef __cplusplus
}
#endif

#ifdef EVENT_EVENTLIB // EventLib struct

#ifndef ASSERT
#define ASSERT(f) ((void)0)
#define __NOASSERT__
#endif // ASSERT

#ifdef __cplusplus
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
	void UnloadEvents();
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
	EVENTLIB_FUNC0(UINT,DataSize)
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
inline int EventLib::LoadEvents(HWND hWndParent, LPCTSTR lpszDirectory)
{
	TCHAR szCurDir[_MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile;

	if(!hWndParent)
		return 0;

	if(lpszDirectory)
	{
		::GetCurrentDirectory(_MAX_PATH,szCurDir);
		::SetCurrentDirectory(lpszDirectory);
	}

	hFindFile = ::FindFirstFile("*.dll",&FindFileData);
	
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
			// Load
			NGPA(hModule,Load);
			if(!NPF(Load)(hWndParent))
			{
				::FreeLibrary(hModule);
				continue;
			}
			// Succeed
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

inline void EventLib::UnloadEvents()
{
	DPF(Unload);
	unsigned i;
	for(i=0;i<nEventLib;i++)
	{
		FNGPA(*(phEventLib+i),Unload)();
		::FreeLibrary(*(phEventLib+i));
	}
	free(phEventLib);
	free(pEventID);
	free(pEventName);
	Init();
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

#endif // __cplusplus

#ifdef __NOASSERT__
#undef __NOASSERT__
#undef ASSERT
#endif // __NOASSERT__

#endif // EVENT_LIB

#endif // __EVENT_IMPORT_EXPORT__

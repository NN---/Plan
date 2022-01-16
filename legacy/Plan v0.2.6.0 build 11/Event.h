#ifndef __EVENT_IMPORT_EXPORT__
#define __EVENT_IMPORT_EXPORT__

#ifdef __cplusplus
	#define DLLExport extern "C" __declspec(dllexport)
	#define DLLImport extern "C" __declspec(dllimport)
#else // !__cplusplus
	#define DLLExport __declspec(dllexport)
	#define DLLImport __declspec(dllimport)
#endif // __cplusplus

#ifdef EVENT_IMPORT_EXPLICIT_DEF
	#define EVENT_IMPORT_EXPLICIT
	#define EVENT_DEF
#endif // EVENT_IMPORT_EXPLICIT_DEF

#if !defined(EVENT_EXPORT) && !defined(EVENT_IMPORT_IMPLICIT) && !defined(EVENT_IMPORT_EXPLICIT)
	#define EVENT_EXPORT
#endif // !EVENT_EXPORT && !EVENT_IMPORT_IMPLICIT && !EVENT_IMPORT_EXPLICIT

#if defined(EVENT_EXPORT) || defined(EVENT_IMPORT_IMPLICIT)
	#ifdef EVENT_EXPORT
		#define EVENT_IMPORTEXPORT DLLExport
	#else // EVENT_IMPORT_IMPLICIT
		#define EVENT_IMPORTEXPORT DLLImport
	#endif // EVENT_EXPORT

	EVENT_IMPORTEXPORT BOOL Init(HWND hWndParent);
	EVENT_IMPORTEXPORT BOOL Destroy(void);
	EVENT_IMPORTEXPORT BOOL Move(RECT rect, BOOL bRepaint);
	EVENT_IMPORTEXPORT BOOL Show(BOOL bShow);
	EVENT_IMPORTEXPORT BOOL IsEmpty(void);
	EVENT_IMPORTEXPORT BOOL Empty(void);
	EVENT_IMPORTEXPORT BOOL DoEvent(void);

#elif defined(EVENT_IMPORT_EXPLICIT)
	#define NPF(fname) p##fname
	#define TPF(fname) NPF(fname)##Type
	#define DPF(fname) TPF(fname) NPF(fname)
	#define GPA(hModule,fname) (TPF(fname))GetProcAddress(hModule,#fname)
	#define DGPA(hModule,fname) DPF(fname) = GPA(hModule,fname)

	#define DEFPF0(type,fname) \
		typedef type (*TPF(fname))(void)
	#define DEFPF1(type,fname,t1) \
		typedef type (*TPF(fname))(t1)
	#define DEFPF2(type,fname,t1,t2) \
		typedef type (*TPF(fname))(t1,t2)

	#ifdef EVENT_DEF
		DEFPF1(BOOL,Init,HWND);
		DEFPF0(BOOL,Destroy);
		DEFPF2(BOOL,Move,RECT,BOOL);
		DEFPF1(BOOL,Show,BOOL);
		DEFPF0(BOOL,IsEmpty);
		DEFPF0(BOOL,Empty);
		DEFPF0(BOOL,DoEvent);
		
		#define EVENT_LIBRARY(hModule) \
			DGPA(hModule,Init); \
			DGPA(hModule,Destroy); \
			DGPA(hModule,Move); \
			DGPA(hModule,Show); \
			DGPA(hModule,IsEmpty); \
			DGPA(hModule,Empty); \
			DGPA(hModule,DoEvent)

		BOOL CheckEventLibrary(HMODULE hModule)
		{
			if(hModule)
			{
				EVENT_LIBRARY(hModule);
				if(!NPF(Init) ||
					!NPF(Destroy) ||
					!NPF(Move) ||
					!NPF(Show) ||
					!NPF(IsEmpty) ||
					!NPF(Empty) ||
					!NPF(DoEvent))
					return FALSE;
				return TRUE;
			}
			return FALSE;
		}
	#endif // EVENT_DEF
#endif // EVENT_EXPORT || EVENT_IMPORT_IMPLICIT

#endif // __EVENT_IMPORT_EXPORT__

#ifndef __EVENT_IMPORT_EXPORT__
#define __EVENT_IMPORT_EXPORT__

#ifdef __cplusplus
	#define DLLExport extern "C" __declspec(dllexport)
	#define DLLImport extern "C" __declspec(dllimport)
#else // !__cplusplus
	#define DLLExport __declspec(dllexport)
	#define DLLImport __declspec(dllimport)
#endif // __cplusplus

typedef struct tagEventID
{
	BYTE nID;
	BYTE sID[12];
}EventID;

#define NF(fname) Event##fname

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

	EVENT_IMPORTEXPORT BOOL NF(Load)(HWND hWndParent);
	EVENT_IMPORTEXPORT BOOL NF(Unload)(void);
	EVENT_IMPORTEXPORT BOOL NF(Move)(RECT rect, BOOL bRepaint);
	EVENT_IMPORTEXPORT BOOL NF(Show)(BOOL bShow);

	EVENT_IMPORTEXPORT BOOL NF(IsEmpty)(void);
	EVENT_IMPORTEXPORT BOOL NF(Empty)(void);
	EVENT_IMPORTEXPORT BOOL NF(Do)(void);

	EVENT_IMPORTEXPORT const EventID * NF(GetID)(void);
	EVENT_IMPORTEXPORT LPCTSTR NF(GetName)(void);

#elif defined(EVENT_IMPORT_EXPLICIT)
	#define NPF(fname) p##Event##fname //p##NF(fname)
	#define TPF(fname) NPF(fname)##Type
	#define DPF(fname) TPF(fname) NPF(fname)
	#define GPA(hModule,fname) (TPF(fname))GetProcAddress(hModule,"Event"## #fname)
	#define DGPA(hModule,fname) DPF(fname) = GPA(hModule,fname)
	#define NGPA(hModule,fname)	NPF(fname) = GPA(hModule,fname)
	#define FDGPA(hModule,fname) DGPA(hModule,fname); NPF(fname)
	#define FNGPA(hModule,fname) NGPA(hModule,fname); NPF(fname)

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

		DEFPF0(const EventID *,GetID);
		DEFPF0(LPCTSTR,GetName);

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
				
				if(!NPF(Load) ||
					!NPF(Unload) ||
					!NPF(Move) ||
					!NPF(Show) ||
					!NPF(IsEmpty) ||
					!NPF(Empty) ||
					!NPF(Do) ||
					!NPF(GetID)||
					!NPF(GetName))
					return FALSE;
				return TRUE;
			}
			return FALSE;
		}
	#endif // EVENT_DEF
#endif // EVENT_EXPORT || EVENT_IMPORT_IMPLICIT

#endif // __EVENT_IMPORT_EXPORT__

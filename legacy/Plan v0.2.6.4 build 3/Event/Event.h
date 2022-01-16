//
// Event Header v0.2.6.4
//

#pragma once

// EventID
struct tagEventID
{
	BYTE nID;
	BYTE sID[11];
};

typedef struct tagEventID EventID;
typedef const struct tagEventID * PCEventID;

#define DLLC
#include "Dll.h"

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

#else // EVENT_IMPORT

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

#ifdef __cplusplus

class EventDll : public NN::Dll
{
public:
	DLL_DEFINEFUNC1(BOOL,Load,HWND,hWndParent)
	DLL_DEFINEFUNC0_VOID(Unload)
	DLL_DEFINEFUNC2(BOOL,Move,RECT,rect,BOOL,bRepaint)
	DLL_DEFINEFUNC1(BOOL,Show,BOOL,bShow)
	DLL_DEFINEFUNC0(BOOL,IsEmpty)
	DLL_DEFINEFUNC0(BOOL,Empty)
	DLL_DEFINEFUNC1(BOOL,Do,const LPBYTE,pData)
	DLL_DEFINEFUNC0(PCEventID,GetID)
	DLL_DEFINEFUNC0(LPCTSTR,GetName)
	DLL_DEFINEFUNC1(BOOL,GetData,LPBYTE,pData)
	DLL_DEFINEFUNC1_VOID(SetData,const LPBYTE,pData)
	DLL_DEFINEFUNC0(UINT,GetDataSize)
protected:
	virtual bool IsDllValid(const HMODULE hModule)
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
			NPF(Load)	&&
			NPF(Unload) &&
			NPF(Move)	&&
			NPF(Show)	&&
			NPF(IsEmpty)&&
			NPF(Empty)	&&
			NPF(Do)		&&
			NPF(GetID)  &&
			NPF(GetName)&&
			NPF(GetData)&&
			NPF(SetData)&&
			NPF(GetDataSize);
	}
};

class EventDlls : public NN::Dlls<EventDll>
{
	typedef NN::Dlls<EventDll> baseClass;
	CSimpleValArray<PCEventID> m_aEventID;
	CSimpleValArray<LPCTSTR> m_aEventName;
public:
	PCEventID GetEventID()
	{
		return m_aEventID[GetWorkDll()];
	}
	LPCTSTR GetEventName()
	{
		return m_aEventName[GetWorkDll()];
	}
	DWORD LoadDlls(HWND hWndParent, LPCTSTR lpszDirectory)
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
			EventDll eventdll;
//			HMODULE hModule;
		//	DPF(Load);
		//	DPF(GetID);
		//	DPF(GetName);
			
			do{
				// LoadLibrary
				//hModule=::LoadLibrary(FindFileData.cFileName);
				if(!eventdll.LoadLibrary(FindFileData.cFileName))
					continue;
/*
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
*/

				if(!eventdll.Load(hWndParent))
					continue;

				AddDll(eventdll);

				m_aEventID.Add(eventdll.GetID());
				m_aEventName.Add(eventdll.GetName());

			}while(::FindNextFile(hFindFile,&FindFileData));
			::FindClose(hFindFile);
		}
	
		if(lpszDirectory)
			::SetCurrentDirectory(szCurDir);

		return GetDllCount();

	}
	virtual void Init()
	{
		m_aEventID.RemoveAll();
		m_aEventName.RemoveAll();
		baseClass::Init();
	}
	virtual void Destroy()
	{
		int i,n;
		for(i=0,n=GetDllCount();i<n;i++)
			GetDll().Unload();

		baseClass::Destroy();
	}
	int FindDll(PCEventID pcEventID,bool bSet = false)
	{
		int i,n;
		for(i=0,n=GetDllCount();i<n;i++)
			if(!memcmp(m_aEventID[i],pcEventID,sizeof(EventID)))
			{
				if(bSet)
					SetWorkDll(i);
				return i;
			}
		return -1;
	}
};

#endif // __cplusplus
#endif // EVENT_EXPORT
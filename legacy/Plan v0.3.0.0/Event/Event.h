#ifndef __EVENT_H__
#define __EVENT_H__

#pragma once

// EventID
struct tagEventID
{
	BYTE ID[12];
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
	DLLExport void SimpleText(void);
	DLLExport BOOL Test(const LPBYTE pData);

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
	DEFPF0(void,SimpleTest);
	DEFPF1(BOOL,Test,const LPBYTE pData);

#ifdef __cplusplus

class EventDll : public NN::DllHandle
{
	typedef NN::DllHandle baseClass;
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
	// New
	DLL_DEFINEFUNC0_VOID(SimpleTest)
	DLL_DEFINEFUNC1(BOOL,Test,const LPBYTE,pData)
	bool SetHandle(const HMODULE hModule)
	{
		if(!IsDllValid(hModule))
			return false;
		return baseClass::SetHandle(hModule);
	}
protected:
	bool IsDllValid(const HMODULE hModule)
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
	ATL::CAtlArray<PCEventID> m_aEventID;
	ATL::CAtlArray<LPCTSTR> m_aEventName;
public:
	EventDlls(){}
	~EventDlls(){}
	PCEventID GetEventID()
	{ return m_aEventID[GetWorkDll()]; }
	LPCTSTR GetEventName()
	{ return m_aEventName[GetWorkDll()]; }
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
			do{
				// LoadLibrary
				if(!eventdll.LoadLibrary(FindFileData.cFileName))
					continue;
				// Load
				if(!eventdll.Load(hWndParent))
					continue;
				if(GetDllCount()&&FindDll(eventdll.GetID())!=(size_t)-1)
				{
					eventdll.FreeLibrary();
					continue;
				}
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
	void Init()
	{
		m_aEventID.RemoveAll();
		m_aEventName.RemoveAll();
		baseClass::Init();
	}
	size_t FindDll(PCEventID pcEventID,bool bSet = false)
	{
		size_t iDll,nDll;
		for(iDll=0,nDll=GetDllCount();iDll<nDll;iDll++)
			if(!memcmp(m_aEventID[iDll],pcEventID,sizeof(EventID)))
			{
				if(bSet)
					SetWorkDll(iDll);
				return iDll;
			}
		return (size_t)-1;
	}
};

#endif // __cplusplus
#endif // EVENT_EXPORT

#endif // __EVENT_H_
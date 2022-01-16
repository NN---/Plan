#ifndef __APP_H__
#define __APP_H__

// Only WinXP
class ATL_NO_VTABLE CApp : public CAppModule
{
	typedef CAppModule baseClass;
private:
	int ArgC;
	TCHAR **ArgV;

	bool InitArg()
	{
		ATLASSERT(ArgC == 0);
		ATLASSERT(ArgV == NULL);
#ifdef UNICODE
		ArgV = ::CommandLineToArgvW(::GetCommandLineW(), &ArgC);
#else // !UNICODE
#if _DEBUG
		OSVERSIONINFO osVer;
		::GetVersionEx(&osVer);
		ATLASSERT(osVer.dwMajorVersion>=0x510);
#endif // _DEBUG
		LPWSTR *wArgV = ::CommandLineToArgvW(::GetCommandLineW(), &ArgC);
		ArgV = new TCHAR * [ArgC+1];
		int i;
		int result;
		for(i=0;i<ArgC;i++)
		{
			ArgV[i] = new TCHAR [_MAX_PATH];
			if(!ArgV[i])
			{
				int j;
				for(j=i-1;j>=0;j--)
					delete [] ArgV[j];
				delete [] ArgV;
				return false;
			}
			result = ::WideCharToMultiByte(CP_ACP, 0, wArgV[i], -1, ArgV[i], _MAX_PATH, NULL, NULL);
			ATLASSERT(ArgV[i] == NULL || result <= (_MAX_PATH));
			if(result > 0)
				ArgV[i][result - 1] = 0;
		}
		ArgV[ArgC] = NULL;
#endif // UNICODE
		return true;
	}
	void ReleaseArg()
	{
#ifndef UNICODE
		int i;
		for(i=ArgC-1;i>=0;i--)
			delete [] ArgV[i];
		delete [] ArgV;
#endif // !UNICODE
	}

public:
	
	CApp()
	{
		ArgC = 0;
		ArgV = NULL;
	}

	HRESULT Init(_ATL_OBJMAP_ENTRY *pObjMap, HINSTANCE hInstance, const GUID *pLibID = 0)
	{
		if(!InitArg())
			return -1;
		return baseClass::Init(pObjMap, hInstance, pLibID);
	}

	void Term()
	{
		ReleaseArg();
		baseClass::Term();
	}

	const int GetArgC() const
	{
		return ArgC;
	}

	const TCHAR **GetArgV() const
	{
		return (const TCHAR **)ArgV;
	}
};

#endif // __APP_H__
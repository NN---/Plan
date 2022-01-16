// RegKey class
//

#ifndef __REGKEY_CLASS__
#define __REGKEY_CLASS__


#ifndef __cplusplus
#error Must define __cplusplus
#endif

class RegKey
{
private:
	// No Problems
	RegKey(const RegKey&);
	RegKey& operator=(const RegKey&);

public:
	// Attributes
	HKEY m_hKey;
	operator HKEY();
	HKEY GetHandle();

	// Construction
	RegKey();
	
	// Destruction
	virtual ~RegKey();
	
	// Function
	LONG Close();
	HKEY Detach();
	LONG Attach(HKEY hKey, BOOL bOpen = FALSE, REGSAM samDesired = KEY_ALL_ACCESS);

	// Create
	LONG Create(HKEY hKey, LPCTSTR lpSubKey, LPTSTR lpClass = REG_NONE,
		DWORD dwOptions = REG_OPTION_NON_VOLATILE, REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, LPDWORD lpdwDisposition = NULL);
	// Open
	LONG Open(HKEY hKey, LPCTSTR lpSubKey,REGSAM samDesired = KEY_ALL_ACCESS);
	// Connect
	LONG Connect(HKEY hKey, LPCTSTR lpMachineName);
	// Flush
	LONG Flush();

	// Keys
	// SetKeyValue
	LONG SetKeyValue(LPCTSTR lpValue, LONG lSize = 0, LPCTSTR lpSubKey = NULL);
	
	// QueryKeyValue
	LONG QueryKeyValue(LPTSTR lpValue, PLONG lpCount, LPCTSTR lpSubKey = NULL);
			
	// QueryInfoKey
	LONG QueryInfoKey(LPTSTR lpClass = NULL, LPDWORD lpcbClass = NULL, LPDWORD lpcSubKeys = NULL,
		LPDWORD lpcbMaxSubKeyLen = NULL, LPDWORD lpcbMaxClassLen = NULL, LPDWORD lpcValues = NULL,
		LPDWORD lpcbMaxValueNameLen = NULL, LPDWORD lpcbMaxValueLen = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL, PFILETIME lpftLastWriteTime = NULL);
	
	// GetKeySecurity
	LONG GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL);
		
	// SetKeySecurity
	LONG SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor);
	
	// DeleteKey
	LONG DeleteSubKey(LPCTSTR lpSubKey);
	void DeleteAllSubKeys();
	
	// EnumKey
	LONG EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName, LPTSTR lpClass = NULL,
		PDWORD lpcbClass = NULL, PFILETIME lpftLastWriteTime = NULL);
		
	// LoadKey
	LONG LoadKey(LPCTSTR lpFile, LPCTSTR lpSubKey = NULL);
	
	// UnloadKey
	LONG UnloadKey(LPCTSTR lpSubKey);
	
	// SaveKey
	LONG SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	
	// ReplaceKey
	LONG ReplaceKey(LPCTSTR lpNewFile, LPCTSTR lpOldFile = NULL, LPCTSTR lpSubKey = NULL);
	
	// RestoreKey
	LONG RestoreKey(LPCTSTR lpFile, DWORD dwFlags = REG_OPTION_NON_VOLATILE);
		
	// NotifyChangeKeyValue
	LONG NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
		BOOL fAsynchronous);
	
	// Values
	// SetValue
	LONG SetValue(DWORD dwValue, LPCTSTR lpValueName);
	LONG SetValue(LPCTSTR lpValue, LPCTSTR lpValueName, LONG lSize = 0);
	LONG SetValue(const LPBYTE lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
		DWORD dwType = REG_BINARY);
		
	// QueryValue
	LONG QueryValue(DWORD &dwValue, LPCTSTR lpValueName);
	LONG QueryValue(LPTSTR lpValue, LPCTSTR lpValueName, LPDWORD lpCount = NULL);
	LONG QueryValue(LPBYTE lpValue, LPCTSTR lpValueName, LPDWORD lpCount, 
		LPDWORD lpType);
			
	// QueryMultipleValues
	LONG RegQueryMultipleValues(PVALENT val_list, DWORD num_vals,
		LPTSTR lpValueBuf, LPDWORD ldwTotsize);
			
	// DeleteValue
	LONG DeleteValue(LPCTSTR lpValueName);
		
	// EnumValue
	LONG EnumValue(DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
};

// No Problems
inline RegKey::RegKey(const RegKey&)
{
}

inline RegKey& RegKey::operator=(const RegKey&)
{
	return *this;
}

// Attributes
inline RegKey::operator HKEY()
{
	return GetHandle();
}

inline HKEY RegKey::GetHandle()
{
	return m_hKey;
}

// Construction
inline RegKey::RegKey()
{
	m_hKey = NULL;
}

// Destruction
inline RegKey::~RegKey()
{
	Close();
}

// Functions
// Close
inline LONG RegKey::Close()
{
	LONG lRes = ERROR_SUCCESS;
	if(m_hKey != NULL)
	{
		lRes = RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
	return lRes;
}

// Detach
inline HKEY RegKey::Detach()
{
	HKEY hKey = m_hKey;
	m_hKey = NULL;
	return hKey;
}

// Attach
inline LONG RegKey::Attach(HKEY hKey, BOOL bOpen, REGSAM samDesired)
{
	ASSERT(hKey != NULL);
	if(bOpen)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(hKey, NULL, NULL, samDesired, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		hKey = hkey;
	}
	m_hKey = hKey;
	return ERROR_SUCCESS;
}

// Create
inline LONG RegKey::Create(HKEY hKey, LPCTSTR lpSubKey, LPTSTR lpClass, DWORD dwOptions,
	REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, LPDWORD lpdwDisposition)
{
	DWORD dw;
	HKEY hkey;
	LONG lRes = RegCreateKeyEx(hKey, lpSubKey, 0,
		lpClass, dwOptions, samDesired, lpSecurityAttributes, &hkey, &dw);
	if(lpdwDisposition != NULL)
		*lpdwDisposition = dw;
	if(lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		m_hKey = hkey;
	}
	return lRes;
}

// Open
inline LONG RegKey::Open(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired)
{
	HKEY hkey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, 0, samDesired, &hkey);
	if(lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		ASSERT(lRes == ERROR_SUCCESS);
		m_hKey = hkey;
	}
	return lRes;
}

// Flush
inline LONG RegKey::Flush()
{
	ASSERT(m_hKey != NULL);
	return RegFlushKey(m_hKey);
}

// Connect
inline LONG RegKey::Connect(HKEY hKey, LPCTSTR lpMachineName)
{
	HKEY hkey;
	LONG lRes;

	lRes=RegConnectRegistry(lpMachineName, hKey, &hkey);
	if(lRes == ERROR_SUCCESS)
	{
		Close();
		m_hKey = hkey;
	}
}

// Keys
// SetKeyValue
inline LONG RegKey::SetKeyValue(LPCTSTR lpValue, LONG lSize, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValue(m_hKey, lpSubKey, REG_SZ, lpValue,
		lSize ? lSize : (lstrlen(lpValue)+1)*sizeof(TCHAR));
}

// QueryKeyValue
inline LONG RegKey::QueryKeyValue(LPTSTR lpValue, PLONG lpCount, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpCount != NULL);
	return RegQueryValue(m_hKey, lpSubKey, lpValue, lpCount);
}

// QueryInfoKey
inline LONG RegKey::QueryInfoKey(LPTSTR lpClass, LPDWORD lpcbClass, LPDWORD lpcSubKeys,
	LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues,
	LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor,
	PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	return RegQueryInfoKey(m_hKey, lpClass, lpcbClass, NULL, lpcSubKeys, lpcbMaxSubKeyLen,
		lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor,
		lpftLastWriteTime);
}

// GetKeySecurity
inline LONG RegKey::GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor)
{
	ASSERT(m_hKey != NULL);
	return RegGetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor,
		lpcbSecurityDescriptor);
}

// SetKeySecurity
inline LONG RegKey::SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	ASSERT(m_hKey != NULL);
	return RegSetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor);
}

// DeleteKey
inline LONG RegKey::DeleteSubKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpSubKey != NULL);
	return RegDeleteKey(m_hKey, lpSubKey);
}

inline void RegKey::DeleteAllSubKeys()
{
	ASSERT(m_hKey != NULL);
	FILETIME time;
	DWORD dwSize = MAX_PATH+1;
	TCHAR szBuffer[MAX_PATH+1];
	while(RegEnumKeyEx(m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time)==ERROR_SUCCESS)
	{
		RegDeleteKey(m_hKey, szBuffer);
		dwSize = MAX_PATH+1;
	}
}


// EnumKey
inline LONG RegKey::EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName,
	LPTSTR lpClass, PDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	return RegEnumKeyEx(m_hKey, dwIndex, lpName, lpcbName, NULL, lpClass, lpcbClass,
		lpftLastWriteTime);
}

// LoadKey
inline LONG RegKey::LoadKey(LPCTSTR lpFile, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != NULL);
	return RegLoadKey(m_hKey, lpSubKey, lpFile);
}

// UnloadKey
inline LONG RegKey::UnloadKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	return RegUnLoadKey(m_hKey, lpSubKey);
}

// SaveKey
inline LONG RegKey::SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != NULL);
	return RegSaveKey(m_hKey, lpFile, lpSecurityAttributes);
}

// ReplaceKey
inline LONG RegKey::ReplaceKey(LPCTSTR lpNewFile, LPCTSTR lpOldFile, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpNewFile != NULL);
	ASSERT(lpOldFile != NULL);
	return RegReplaceKey(m_hKey, lpSubKey, lpNewFile, lpOldFile);
}

// RestoreKey
inline LONG RegKey::RestoreKey(LPCTSTR lpFile, DWORD dwFlags)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != lpFile);
	return RegRestoreKey(m_hKey, lpFile, dwFlags);
}

// NotifyChangeKeyValue
inline LONG RegKey::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
	BOOL fAsynchronous)
{
	ASSERT(m_hKey != NULL);
	return RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent,
		fAsynchronous);
}

// Values
// SetValue
inline LONG RegKey::SetValue(DWORD dwValue, LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
			sizeof(DWORD));

}

inline LONG RegKey::SetValue(LPCTSTR lpValue, LPCTSTR lpValueName, LONG lSize)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpValue,
		lSize ? lSize : (lstrlen(lpValue)+1)*sizeof(TCHAR));
}

inline LONG RegKey::SetValue(const LPBYTE lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
	DWORD dwType)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, dwType, lpValue, dwValueSize);
}

// QueryValue
inline LONG RegKey::QueryValue(DWORD &dwValue, LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	LONG lRes;
	
	lRes = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType,
		(LPBYTE)&dwValue,&dwCount);
	
	ASSERT(lRes != ERROR_SUCCESS || dwType != REG_DWORD || dwCount == sizeof(DWORD));
	return lRes;
}

inline LONG RegKey::QueryValue(LPTSTR lpValue, LPCTSTR lpValueName,	LPDWORD lpCount)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	DWORD dwType =NULL;
	DWORD dwCount = lpCount ? *lpCount : MAX_PATH+1;
	LONG lRes;

	lRes = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (LPBYTE)lpValue,
		&dwCount);
	if(lpCount)
		*lpCount = dwCount;
	
	ASSERT(lRes!=ERROR_SUCCESS || (dwType != REG_SZ || dwType != REG_MULTI_SZ ||
		dwType != REG_EXPAND_SZ));
	return lRes;
}

inline LONG RegKey::QueryValue(LPBYTE lpValue, LPCTSTR lpValueName, LPDWORD lpCount,
	LPDWORD lpType)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	ASSERT(lpValue != NULL);
	DWORD dwType;
	DWORD dwCount = lpCount ? *lpCount : MAX_PATH+1;
	LONG lRes;

	lRes = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, lpValue, &dwCount);
	if(lpCount)
		*lpCount = dwCount;
	if(lpType)
		*lpType = dwType;

	return lRes;

}

// DeleteValue
inline LONG RegKey::DeleteValue(LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	return RegDeleteValue(m_hKey, lpValueName);
}

// EnumValue
inline LONG RegKey::EnumValue(DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	ASSERT(m_hKey != NULL);
	return RegEnumValue(m_hKey, dwIndex, lpValueName, lpcbValueName, NULL, lpType, lpData,
		lpcbData);
}

#endif // __REGKEY_CLASS__
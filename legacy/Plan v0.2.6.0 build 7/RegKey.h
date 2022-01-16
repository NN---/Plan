// RegKey class
//

#ifndef __REGKEY_CLASS__
#define __REGKEY_CLASS__

#ifndef ASSERT
#define ASSERT ((void)0)
#define NOASSERT
#endif // ASSERT

class RegKey
{
public:
	// Attributes
	HKEY m_hKey;
	operator HKEY() const;

	// Construction
	RegKey();

	// Destruction
	~RegKey();
	
	// Function
	LONG Close();
	HKEY Detach();
	void Attach(HKEY hKey);

	// Create
	LONG Create(HKEY hKeyParent, LPCTSTR lpSubKey,
		LPTSTR lpClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
		LPDWORD lpdwDisposition = NULL);
	// Open
	LONG Open(HKEY hKeyParent, LPCTSTR lpSubKey,REGSAM samDesired = KEY_ALL_ACCESS);
	// Flush
	LONG Flush();
	static LONG WINAPI Flush(HKEY hKey);

	// Keys
	// SetKeyValue
	LONG SetKeyValue(LPCTSTR lpValue);
	LONG SetKeyValue(LPCTSTR lpSubKey, LPCTSTR lpValue);
	static LONG WINAPI SetKeyValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValue);
	
	// QueryInfoKey
	LONG QueryInfoKey(LPTSTR lpClass = NULL, LPDWORD lpcbClass = NULL,
		LPDWORD lpcSubKeys = NULL, LPDWORD lpcbMaxSubKeyLen = NULL,
		LPDWORD lpcbMaxClassLen = NULL, LPDWORD lpcValues = NULL,
		LPDWORD lpcbMaxValueNameLen = NULL, LPDWORD lpcbMaxValueLen = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL, PFILETIME lpftLastWriteTime = NULL);
	LONG QueryInfoKey(LPCTSTR lpSubKey ,LPTSTR lpClass = NULL, LPDWORD lpcbClass = NULL,
		LPDWORD lpcSubKeys = NULL, LPDWORD lpcbMaxSubKeyLen = NULL,
		LPDWORD lpcbMaxClassLen = NULL, LPDWORD lpcValues = NULL,
		LPDWORD lpcbMaxValueNameLen = NULL, LPDWORD lpcbMaxValueLen = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL, PFILETIME lpftLastWriteTime = NULL);
	static LONG WINAPI QueryInfoKey(HKEY hKey, LPCTSTR lpSubKey ,LPTSTR lpClass = NULL,
		LPDWORD lpcbClass = NULL, LPDWORD lpcSubKeys = NULL, LPDWORD lpcbMaxSubKeyLen = NULL,
		LPDWORD lpcbMaxClassLen = NULL, LPDWORD lpcValues = NULL,
		LPDWORD lpcbMaxValueNameLen = NULL, LPDWORD lpcbMaxValueLen = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL, PFILETIME lpftLastWriteTime = NULL);
	
	// GetKeySecurity
	LONG GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL);
	LONG GetKeySecurity(LPCTSTR lpSubKey, SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL);
	static LONG WINAPI GetKeySecurity(HKEY hKey, LPCTSTR lpSubKey,
		SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL);
	
	// SetKeySecurity
	LONG SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor);
	LONG SetKeySecurity(LPCTSTR lpSubKey, SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor);
	static LONG WINAPI SetKeySecurity(HKEY hKey, LPCTSTR lpSubKey,
		SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor);

	// DeleteKey
	LONG DeleteSubKey(LPCTSTR lpSubKey);
	static LONG WINAPI DeleteKey(HKEY hKey, LPCTSTR lpSubKey);
	LONG RecurseDeleteKey(LPCTSTR lpSubKey);
	static LONG WINAPI RescurseDeleteKey(HKEY hKey, LPCTSTR lpSubKey);
	
	// EnumKey
	LONG EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName, LPTSTR lpClass = NULL,
		PDWORD lpcbClass = NULL, PFILETIME lpftLastWriteTime = NULL);
	LONG EnumKey(LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName,
		LPTSTR lpClass = NULL, PDWORD lpcbClass = NULL, PFILETIME lpftLastWriteTime = NULL);
	LONG EnumKey(DWORD dwIndex, LPTSTR lpName, DWORD cbName, LPTSTR lpClass = NULL,
		LPDWORD lpcbClass = NULL, PFILETIME lpftLastWriteTime=NULL);
	LONG EnumKey(LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpName, DWORD cbName,
		LPTSTR lpClass = NULL, LPDWORD lpcbClass = NULL, PFILETIME lpftLastWriteTime=NULL);
	static LONG WINAPI EnumKey(HKEY hKey, LPCTSTR lpSubKey, DWORD dwIndex,LPTSTR lpName,
		LPDWORD lpcbName, LPTSTR lpClass = NULL, LPDWORD lpcbClass = NULL,
		PFILETIME lpftLastWriteTime = NULL);
	static LONG WINAPI EnumKey(HKEY hKey, LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpName,
		DWORD cbName, LPTSTR lpClass = NULL, LPDWORD lpcbClass = NULL,
		PFILETIME lpftLastWriteTime=NULL);
/*
	// LoadKey
	LONG LoadKey(LPCTSTR lpSubKey, LPCTSTR lpFile);
	static LONG WINAPI LoadKey(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpFile);

	// UnloadKey
	LONG UnloadKey(LPCTSTR lpSubKey);
	static LONG WINAPI UnloadKey(HKEY hKey, LPCTSTR lpSubKey);

	// SaveKey
	LONG SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	LONG SaveKey(LPCTSTR lpSubKey, LPCTSTR lpFile,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	static LONG WINAPI SaveKey(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpFile, 
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);

	// ReplaceKey
	LONG ReplaceKey(LPCTSTR lpSubKey, LPCTSTR lpNewFile, LPCTSTR lpOldFile = NULL);
	static LONG WINAPI ReplaceKey(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpNewFile,
		LPCTSTR lpOldFile = NULL);

	// RestoreKey
	LONG RestoreKey(LPCTSTR lpFile, DWORD dwFlags = REG_OPTION_NON_VOLATILE);
	LONG RestoreKey(LPCTSTR lpSubKey, LPCTSTR lpFile, DWORD dwFlags = REG_OPTION_NON_VOLATILE);
	static LONG WINAPI RestoreKey(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpFile, 
		DWORD dwFlags = REG_OPTION_NON_VOLATILE);
*/
	// NotifyChangeKeyValue
	LONG NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
		BOOL fAsynchronous);
	LONG NotifyChangeKeyValue(LPCTSTR lpSubKey, BOOL bWatchSubtree, DWORD dwNotifyFilter,
		HANDLE hEvent, BOOL fAsynchronous);
	static LONG WINAPI NotifyChangeKeyValue(HKEY hKey, LPCTSTR lpSubKey, BOOL bWatchSubtree,
		DWORD dwNotifyFilter, HANDLE hEvent, BOOL fAsynchronous);

	// Values
	// SetValue
	LONG SetValue(DWORD dwValue, LPCTSTR lpValueName);
	LONG SetValue(LPCTSTR lpSubKey, DWORD dwValue, LPCTSTR lpValueName);
	LONG SetValue(LPCTSTR lpszValue, LPCTSTR lpValueName);
	LONG SetValue(LPCTSTR lpSubKey, LPCTSTR lpszValue, LPCTSTR lpValueName);
	LONG SetValue(CONST BYTE *lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
		DWORD dwType = REG_BINARY);
	LONG SetValue(LPCTSTR lpSubKey, CONST BYTE *lpValue, DWORD dwValueSize,
		LPCTSTR lpValueName, DWORD dwType = REG_BINARY);
	static LONG WINAPI SetValue(HKEY hKey, LPCTSTR lpSubKey,DWORD dwValue,
		LPCTSTR lpValueName);
	static LONG WINAPI SetValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpszValue, 
		LPCTSTR lpValueName = NULL);
	static LONG WINAPI SetValue(HKEY hKey, LPCTSTR lpSubKey, CONST BYTE *lpValue, 
		DWORD dwValueSize, LPCTSTR lpValueName, DWORD dwType = REG_BINARY);
	
	// QueryValue
	LONG QueryValue(DWORD &dwValue, LPCTSTR lpValueName);
	LONG QueryValue(LPCTSTR lpSubKey, DWORD &dwValue, LPCTSTR lpValueName);
	LONG QueryValue(LPTSTR lpValue, LPCTSTR lpValueName = NULL, LPDWORD lpCount = NULL);
	LONG QueryValue(LPCTSTR lpSubKey, LPTSTR lpValue, LPCTSTR lpValueName = NULL, 
		LPDWORD lpCount = NULL);
	LONG QueryValue(LPBYTE lpValue, LPCTSTR lpValueName, LPDWORD lpValueSize = NULL,
		LPDWORD lpCount = NULL);
	LONG QueryValue(LPCTSTR lpSubKey, LPBYTE lpValue, LPCTSTR lpValueName,
		LPDWORD lpValueSize = NULL,	LPDWORD lpType = NULL);
	static LONG WINAPI QueryValue(HKEY hKey, LPCTSTR lpSubKey, LPTSTR lpValue,
		DWORD dwValueSize = 0, LPCTSTR lpValueName = NULL);
	static LONG WINAPI QueryValue(HKEY hKey, LPCTSTR lpSubKey, DWORD &dwValue, 
		LPCTSTR lpValueName);
	
	// QueryMultipleValues
	LONG RegQueryMultipleValues(PVALENT val_list, DWORD num_vals,
		LPTSTR lpValueBuf, LPDWORD ldwTotsize);
	LONG RegQueryMultipleValues(LPCTSTR lpSubKey, PVALENT val_list, DWORD num_vals,
		LPTSTR lpValueBuf, LPDWORD ldwTotsize);
	static LONG WINAPI RegQueryMultipleValues(HKEY hKey, LPCTSTR lpSubKey, PVALENT val_list,
		DWORD num_vals,	LPTSTR lpValueBuf, LPDWORD ldwTotsize);
		
	// DeleteValue
	LONG DeleteValue(LPCTSTR lpValueName);
	LONG DeleteValue(LPCTSTR lpSubKey, LPCTSTR lpValueName);
	static LONG WINAPI DeleteValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName);

	// EnumValue
	LONG EnumValue(DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	LONG EnumValue(LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	LONG EnumValue(DWORD dwIndex, LPTSTR lpValueName, DWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	LONG EnumValue(LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpValueName, DWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	static LONG WINAPI EnumValue(HKEY hKey, LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpValueName,
		LPDWORD lpcbValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	static LONG WINAPI EnumValue(HKEY hKey, LPCTSTR lpSubKey, DWORD dwIndex,LPTSTR lpValueName,
		DWORD cbValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
};

// Attributes
inline RegKey::operator HKEY() const
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

inline HKEY RegKey::Detach()
{
	HKEY hKey = m_hKey;
	m_hKey = NULL;
	return hKey;
}

inline void RegKey::Attach(HKEY hKey)
{
	ASSERT(m_hKey == NULL);
	m_hKey = hKey;
}

// Create
inline LONG RegKey::Create(HKEY hKeyParent, LPCTSTR lpSubKey,
	LPTSTR lpClass, DWORD dwOptions, REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, LPDWORD lpdwDisposition)
{
	ASSERT(hKeyParent != NULL);
	DWORD dw;
	HKEY hKey = NULL;
	LONG lRes = RegCreateKeyEx(hKeyParent, lpSubKey, 0,
		lpClass, dwOptions, samDesired, lpSecurityAttributes, &hKey, &dw);
	if(lpdwDisposition != NULL)
		*lpdwDisposition = dw;
	if(lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		m_hKey = hKey;
	}
	return lRes;
}

// Open
inline LONG RegKey::Open(HKEY hKeyParent, LPCTSTR lpSubKey, REGSAM samDesired)
{
	ASSERT(hKeyParent != NULL);
	HKEY hKey = NULL;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpSubKey, 0, samDesired, &hKey);
	if(lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		ASSERT(lRes == ERROR_SUCCESS);
		m_hKey = hKey;
	}
	return lRes;
}

// Flush
inline LONG RegKey::Flush()
{
	ASSERT(m_hKey != NULL);
	return RegFlushKey(m_hKey);
}

inline LONG WINAPI RegKey::Flush(HKEY hKey)
{
	ASSERT(hKey != NULL);
	return RegFlushKey(hKey);
}

// Keys
// SetKeyValue
inline LONG RegKey::SetKeyValue(LPCTSTR lpValue)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValue(m_hKey, NULL, REG_SZ, lpValue, (lstrlen(lpValue)+1)*sizeof(TCHAR));
}

inline LONG RegKey::SetKeyValue(LPCSTR lpSubKey, LPCTSTR lpValue)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValue(m_hKey, lpSubKey, REG_SZ, lpValue, (lstrlen(lpValue)+1)*sizeof(TCHAR));
}

inline LONG WINAPI RegKey::SetKeyValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValue)
{
	ASSERT(hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValue(hKey, lpSubKey, REG_SZ, lpValue, (lstrlen(lpValue)+1)*sizeof(TCHAR));
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

inline LONG RegKey::QueryInfoKey(LPCTSTR lpSubKey ,LPTSTR lpClass, LPDWORD lpcbClass, 
	LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues,
	LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor,
	PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpSubKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegQueryInfoKey(hKey, lpClass, lpcbClass, NULL, lpcSubKeys, lpcbMaxSubKeyLen,
		lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor,
		lpftLastWriteTime);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG WINAPI RegKey::QueryInfoKey(HKEY hKey, LPCTSTR lpSubKey ,LPTSTR lpClass,
	LPDWORD lpcbClass, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen,
	LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen,
	LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	ASSERT(hKey != NULL);
	ASSERT(lpSubKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegQueryInfoKey(_hKey, lpClass, lpcbClass, NULL, lpcSubKeys, lpcbMaxSubKeyLen,
		lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor,
		lpftLastWriteTime);
	RegCloseKey(_hKey);
	return lRes;
}

// GetKeySecurity
inline LONG RegKey::GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor)
{
	ASSERT(m_hKey != NULL);
	return RegGetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor,
		lpcbSecurityDescriptor);
}

inline LONG RegKey::GetKeySecurity(LPCTSTR lpSubKey, SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegGetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor,
		lpcbSecurityDescriptor);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG WINAPI RegKey::GetKeySecurity(HKEY hKey, LPCTSTR lpSubKey, 
	SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, 
	LPDWORD lpcbSecurityDescriptor)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_READ, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegGetKeySecurity(_hKey, SecurityInformation, pSecurityDescriptor,
		lpcbSecurityDescriptor);
	RegCloseKey(hKey);
	return lRes;
}

// SetKeySecurity
inline LONG RegKey::SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	ASSERT(m_hKey != NULL);
	return RegSetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor);
}

inline LONG RegKey::SetKeySecurity(LPCTSTR lpSubKey, SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG WINAPI RegKey::SetKeySecurity(HKEY hKey, LPCTSTR lpSubKey, 
	SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_WRITE, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSetKeySecurity(_hKey, SecurityInformation, pSecurityDescriptor);
	RegCloseKey(hKey);
	return lRes;
}

// DeleteKey
inline LONG RegKey::DeleteSubKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	return RegDeleteKey(m_hKey, lpSubKey);
}

inline LONG WINAPI RegKey::DeleteKey(HKEY hKey, LPCTSTR lpSubKey)
{
	ASSERT(hKey != NULL);
	return RegDeleteKey(hKey, lpSubKey);
}

inline LONG RegKey::RecurseDeleteKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ | KEY_WRITE, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	DWORD dwSize = MAX_PATH+1;
	TCHAR szBuffer[MAX_PATH+1];
	while (RegEnumKeyEx(hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
	{
		RegDeleteKey(hKey, szBuffer);
		dwSize = MAX_PATH+1;
	}
	RegCloseKey(hKey);
}

inline LONG WINAPI RegKey::RescurseDeleteKey(HKEY hKey, LPCTSTR lpSubKey)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_READ | KEY_WRITE, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	DWORD dwSize = MAX_PATH+1;
	TCHAR szBuffer[MAX_PATH+1];
	while (RegEnumKeyEx(_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
	{
		RegDeleteKey(_hKey, szBuffer);
		dwSize = MAX_PATH+1;
	}
	RegCloseKey(_hKey);
}

// EnumKey
inline LONG RegKey::EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName, LPTSTR lpClass,
	PDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	return RegEnumKeyEx(m_hKey, dwIndex, lpName, lpcbName, NULL, lpClass, lpcbClass,
		lpftLastWriteTime);
}

inline LONG RegKey::EnumKey(LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName,
	LPTSTR lpClass, PDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_ENUMERATE_SUB_KEYS, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegEnumKeyEx(hKey, dwIndex, lpName, lpcbName, NULL, lpClass, lpcbClass,
		lpftLastWriteTime);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG RegKey::EnumKey(DWORD dwIndex, LPTSTR lpName, DWORD cbName,LPTSTR lpClass,
	LPDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	return RegEnumKeyEx(m_hKey, dwIndex, lpName, &cbName, NULL, lpClass, lpcbClass,
		lpftLastWriteTime);
}

inline LONG RegKey::EnumKey(LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpName, DWORD cbName,
	LPTSTR lpClass, LPDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_ENUMERATE_SUB_KEYS, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegEnumKeyEx(hKey, dwIndex, lpName, &cbName, NULL, lpClass, lpcbClass,
		lpftLastWriteTime);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG WINAPI RegKey::EnumKey(HKEY hKey, LPCTSTR lpSubKey, DWORD dwIndex,LPTSTR lpName,
	LPDWORD lpcbName,LPTSTR lpClass ,LPDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_ENUMERATE_SUB_KEYS, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegEnumKeyEx(_hKey, dwIndex, lpName, lpcbName, NULL, lpClass, lpcbClass,
		lpftLastWriteTime);
	RegCloseKey(_hKey);
	return lRes;
}

inline LONG WINAPI RegKey::EnumKey(HKEY hKey, LPCTSTR lpSubKey, DWORD dwIndex, LPTSTR lpName,
	DWORD cbName,LPTSTR lpClass,LPDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_ENUMERATE_SUB_KEYS, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegEnumKeyEx(_hKey, dwIndex, lpName, &cbName, NULL, lpClass, lpcbClass,
		lpftLastWriteTime);
	RegCloseKey(_hKey);
	return lRes;
}
/*
// LoadKey
inline LONG RegKey::LoadKey(LPCTSTR lpSubKey, LPCTSTR lpFile)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpSubKey != NULL);
	return RegLoadKey(m_hKey, lpSubKey, lpFile);
}

inline LONG WINAPI RegKey::LoadKey(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpFile)
{
	ASSERT(hKey != NULL);
	return RegLoadKey(hKey, lpSubKey, lpFile);
}

// UnloadKey
inline LONG RegKey::UnloadKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	return RegUnLoadKey(m_hKey, lpSubKey);
}

inline LONG WINAPI RegKey::UnloadKey(HKEY hKey, LPCTSTR lpSubKey)
{
	ASSERT(hKey != NULL);
	return RegUnLoadKey(hKey, lpSubKey);
}

// SaveKey
inline LONG RegKey::SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	ASSERT(m_hKey != NULL);
	return RegSaveKey(m_hKey, lpFile, lpSecurityAttributes);
}

inline LONG RegKey::SaveKey(LPCTSTR lpSubKey, LPCTSTR lpFile,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSaveKey(m_hKey, lpFile, lpSecurityAttributes);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG WINAPI RegKey::SaveKey(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpFile,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_ALL_ACCESS, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSaveKey(_hKey, lpFile, lpSecurityAttributes);
	RegCloseKey(_hKey);
	return lRes;
}

// ReplaceKey
inline LONG RegKey::ReplaceKey(LPCTSTR lpSubKey, LPCTSTR lpNewFile, LPCTSTR lpOldFile)
{
	ASSERT(m_hKey != NULL);
	return RegReplaceKey(m_hKey, lpSubKey, lpNewFile, lpOldFile);
}

inline LONG WINAPI RegKey::ReplaceKey(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpNewFile,
	LPCTSTR lpOldFile)
{
	ASSERT(hKey != NULL);
	return RegReplaceKey(hKey, lpSubKey, lpNewFile, lpOldFile);
}

// RestoreKey
inline LONG RegKey::RestoreKey(LPCTSTR lpFile, DWORD dwFlags)
{
	ASSERT(m_hKey != NULL);
	return RegRestoreKey(m_hKey, lpFile, dwFlags);
}

inline LONG RegKey::RestoreKey(LPCTSTR lpSubKey, LPCTSTR lpFile, DWORD dwFlags)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegRestoreKey(m_hKey, lpFile, dwFlags);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG WINAPI RegKey::RestoreKey(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpFile, 
	DWORD dwFlags)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_WRITE, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegRestoreKey(_hKey, lpFile, dwFlags);
	RegCloseKey(_hKey);
	return lRes;
}

// NotifyChangeKeyValue
inline LONG RegKey::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
		BOOL fAsynchronous)
{
	ASSERT(m_hKey != NULL);
	return RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent,
		fAsynchronous);
}
*/
inline LONG RegKey::NotifyChangeKeyValue(LPCTSTR lpSubKey, BOOL bWatchSubtree, DWORD dwNotifyFilter,
	HANDLE hEvent, BOOL fAsynchronous)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG WINAPI RegKey::NotifyChangeKeyValue(HKEY hKey, LPCTSTR lpSubKey, BOOL bWatchSubtree,
	DWORD dwNotifyFilter, HANDLE hEvent, BOOL fAsynchronous)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_WRITE, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegNotifyChangeKeyValue(_hKey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
	RegCloseKey(_hKey);
	return lRes;
}

/*inline LONG WINAPI RegKey::SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPCTSTR lpszValue,LPCTSTR lpszValueName)
{
	ASSERT(lpszValue != NULL);
	RegKey key;
	LONG lRes = key.Create(hKeyParent, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetValue(lpszValue, lpszValueName);
	return lRes;
}
*/
/*
inline LONG WINAPI RegKey::SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName,
		DWORD dwValue, LPCTSTR lpszValueName)
{
	RegKey key;
	LONG lRes = key.Create(hKeyParent, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetValue(dwValue, lpszValueName);
	return lRes;
}

inline LONG RegKey::SetValue(DWORD dwValue, LPCTSTR lpszValueName)
{
	ASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey, lpszValueName, NULL, REG_DWORD,
		(BYTE * const)&dwValue, sizeof(DWORD));
}

inline LONG RegKey::SetValue(CONST BYTE *lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
	DWORD dwType)
{
	ASSERT(lpValue != NULL);
	ASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey,lpValueName,NULL,dwType,lpValue,dwValueSize);
}

inline LONG RegKey::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	ASSERT(lpszValue != NULL);
	ASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey, lpszValueName, NULL, REG_SZ,
		(BYTE * const)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
}


*/

// Value
// SetValue
inline LONG RegKey::SetValue(DWORD dwValue, LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
		sizeof(DWORD));
}

inline LONG RegKey::SetValue(LPCTSTR lpSubKey, DWORD dwValue, LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSetValueEx(hKey,lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
		sizeof(DWORD));
	RegCloseKey(hKey);
	return lRes;
}

inline LONG RegKey::SetValue(LPCTSTR lpszValue, LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpszValue != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpszValue,
		(lstrlen(lpszValue)+1)*sizeof(TCHAR));
}

inline LONG RegKey::SetValue(LPCTSTR lpSubKey, LPCTSTR lpszValue, LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpszValue != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSetValueEx(m_hKey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpszValue,
		(lstrlen(lpszValue)+1)*sizeof(TCHAR));
	RegCloseKey(hKey);
	return lRes;
}

inline LONG RegKey::SetValue(CONST BYTE *lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
	DWORD dwType)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, dwType, lpValue, dwValueSize);
}

inline LONG RegKey::SetValue(LPCTSTR lpSubKey, CONST BYTE *lpValue, DWORD dwValueSize, 
	LPCTSTR lpValueName, DWORD dwType)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	HKEY hKey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSetValueEx(m_hKey, lpValueName, NULL, dwType, lpValue, dwValueSize);
	RegCloseKey(hKey);
	return lRes;
}

inline LONG WINAPI RegKey::SetValue(HKEY hKey, LPCTSTR lpSubKey, DWORD dwValue,
	LPCTSTR lpValueName)
{
	ASSERT(hKey != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_SET_VALUE, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSetValueEx(_hKey,lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
		sizeof(DWORD));
	RegCloseKey(_hKey);
	return lRes;
}

inline LONG WINAPI RegKey::SetValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpszValue,
	LPCTSTR lpValueName)
{
	ASSERT(hKey != NULL);
	ASSERT(lpszValue != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_SET_VALUE, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSetValueEx(_hKey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpszValue,
		(lstrlen(lpszValue)+1)*sizeof(TCHAR));
	RegCloseKey(_hKey);
	return lRes;
}

inline LONG WINAPI RegKey::SetValue(HKEY hKey, LPCTSTR lpSubKey, CONST BYTE *lpValue,
	DWORD dwValueSize, LPCTSTR lpValueName, DWORD dwType)
{
	ASSERT(hKey != NULL);
	ASSERT(lpValue != NULL);
	HKEY _hKey;
	LONG lRes = RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_SET_VALUE, &_hKey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	lRes = RegSetValueEx(_hKey, lpValueName, NULL, dwType, lpValue, dwValueSize);
	RegCloseKey(_hKey);
	return lRes;
}

inline LONG RegKey::QueryValue(DWORD &dwValue, LPCTSTR lpszValueName)
{
	ASSERT(lpszValueName != NULL);
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		(LPBYTE)dwValue, &dwCount);
	ASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_DWORD));
	ASSERT((lRes!=ERROR_SUCCESS) || (dwCount == sizeof(DWORD)));
	return lRes;
}

inline LONG RegKey::QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount)
{
	ASSERT(pdwCount != NULL);
	DWORD dwType = NULL;
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		(LPBYTE)szValue, pdwCount);
	ASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_SZ) ||
			 (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));
	return lRes;
}

inline LONG WINAPI RegKey::QueryValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPTSTR szValue, DWORD dwLen, LPCTSTR lpszValueName)
{
	ASSERT(szValue != NULL);
	RegKey key;
	LONG lRes=key.Open(hKeyParent,lpszKeyName);
	if(lRes!=ERROR_SUCCESS)
		return lRes;
	if(!dwLen) 
		dwLen=MAX_PATH+1;
	return key.QueryValue(szValue,lpszValueName,&dwLen);
}

inline LONG WINAPI RegKey::QueryValue(HKEY hKeyParent, LPCTSTR lpszKeyName, DWORD &dwValue,
	LPCTSTR lpszValueName)
{
	ASSERT(lpszValueName != NULL);
	RegKey key;
	LONG lRes=key.Open(hKeyParent,lpszKeyName);
	if(lRes!=ERROR_SUCCESS)
		return lRes;
	return key.QueryValue(dwValue,lpszValueName);
}

inline LONG RegKey::DeleteValue(LPCTSTR lpszValue)
{
	ASSERT(m_hKey != NULL);
	return RegDeleteValue(m_hKey, (LPTSTR)lpszValue);
}

/*
inline LONG RegKey::EnumKey(DWORD dwIndex,LPTSTR lpName,LPDWORD lpcbName, LPTSTR lpClass,
	LPDWORD lpcbClass,PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpcbName != NULL);
	return RegEnumKeyEx(m_hKey,dwIndex,lpName,lpcbName,NULL,lpClass,
	lpcbClass,lpftLastWriteTime);
}

inline LONG RegKey::EnumKey(DWORD dwIndex,LPTSTR lpName,DWORD cbName, LPTSTR lpClass,
	LPDWORD lpcbClass,PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	return RegEnumKeyEx(m_hKey,dwIndex,lpName,&cbName,
		0,lpClass,lpcbClass,lpftLastWriteTime);
}*/

inline LONG RegKey::EnumValue(DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcbValueName,
		LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData)
{
	ASSERT(lpValueName != NULL);
	return RegEnumValue(m_hKey,dwIndex,lpValueName,lpcbValueName,NULL,lpType,
		lpData,lpcbData);
}

inline LONG RegKey::EnumValue(DWORD dwIndex,LPTSTR lpValueName,DWORD lpcbValueName,
		LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData)
{
	ASSERT(lpValueName != NULL);
	return RegEnumValue(m_hKey,dwIndex,lpValueName,&lpcbValueName,NULL,lpType,
		lpData,lpcbData);
}

/*inline LONG RegKey::DeleteSubKey(LPCTSTR lpszSubKey)
{
	ASSERT(m_hKey != NULL);
	return RegDeleteKey(m_hKey, lpszSubKey);
}

inline LONG RegKey::RecurseDeleteKey(LPCTSTR lpszKey)
{
	RegKey key;
	LONG lRes = key.Open(m_hKey, lpszKey, KEY_READ | KEY_WRITE);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	DWORD dwSize = MAX_PATH+1;
	TCHAR szBuffer[MAX_PATH+1];
	while (RegEnumKeyEx(key.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
	{
		lRes = key.RecurseDeleteKey(szBuffer);
		if (lRes != ERROR_SUCCESS)
			return lRes;
		dwSize = MAX_PATH;
	}
	key.Close();
	return DeleteSubKey(lpszKey);
}
*/


#ifdef NOASSERT
#undef NOASSERT
#undef ASSERT
#endif // NOASSERT

#endif // __REGKEY_CLASS__
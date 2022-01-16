// RegKey class
//

#ifndef __REGKEY_CLASS__
#define __REGKEY_CLASS__

#ifndef __cplusplus
#error Must define __cplusplus
#endif

#ifndef ASSERT
#define ASSERT ((void)0)
#define __NOASSERT__
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
	LONG Attach(HKEY hKey, BOOL bOpen = FALSE, REGSAM samDesired = KEY_ALL_ACCESS);

	// Create
	LONG Create(HKEY hKey, LPCTSTR lpSubKey, LPTSTR lpClass = REG_NONE,
		DWORD dwOptions = REG_OPTION_NON_VOLATILE, REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, LPDWORD lpdwDisposition = NULL);
	// Open
	LONG Open(HKEY hKey, LPCTSTR lpSubKey,REGSAM samDesired = KEY_ALL_ACCESS);
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
		LPDWORD lpcbSecurityDescriptor = NULL, PFILETIME lpftLastWriteTime = NULL,
		LPCTSTR lpSubKey = NULL);
	
	// GetKeySecurity
	LONG GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL, LPCTSTR lpSubKey = NULL);
		
	// SetKeySecurity
	LONG SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor, LPCTSTR lpSubKey = NULL);
	
	// DeleteKey
	LONG DeleteSubKey(LPCTSTR lpSubKey);
	LONG RecurseDeleteKey(LPCTSTR lpSubKey);
	
	// EnumKey
	LONG EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName, LPTSTR lpClass = NULL,
		PDWORD lpcbClass = NULL, PFILETIME lpftLastWriteTime = NULL, LPCTSTR lpSubKey = NULL);
		
	// LoadKey
	LONG LoadKey(LPCTSTR lpFile, LPCTSTR lpSubKey = NULL);
	
	// UnloadKey
	LONG UnloadKey(LPCTSTR lpSubKey);
	
	// SaveKey
	LONG SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, LPCTSTR lpSubKey = NULL);
	
	// ReplaceKey
	LONG ReplaceKey(LPCTSTR lpNewFile, LPCTSTR lpOldFile = NULL, LPCTSTR lpSubKey = NULL);
	
	// RestoreKey
	LONG RestoreKey(LPCTSTR lpFile, DWORD dwFlags = REG_OPTION_NON_VOLATILE, LPCTSTR lpSubKey = NULL);
		
	// NotifyChangeKeyValue
	LONG NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
		BOOL fAsynchronous, LPCTSTR lpSubKey = NULL);
	
	// Values
	// SetValue
	LONG SetValue(DWORD dwValue, LPCTSTR lpValueName, LPCTSTR lpSubKey = NULL);
	LONG SetValue(LPCTSTR lpValue, LPCTSTR lpValueName, LONG lSize = 0, LPCTSTR lpSubKey = NULL);
	LONG SetValue(CONST BYTE *lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
		DWORD dwType = REG_BINARY, LPCTSTR lpSubKey = NULL);
		
	// QueryValue
	LONG QueryValue(DWORD &dwValue, LPCTSTR lpValueName, LPCTSTR lpSubKey = NULL);
	LONG QueryValue(LPTSTR lpValue, LPCTSTR lpValueName, LPDWORD lpCount = NULL, 
		LPCTSTR lpSubKey = NULL);
	LONG QueryValue(LPBYTE lpValue, LPCTSTR lpValueName, LPDWORD lpCount, 
		LPDWORD lpType, LPCTSTR lpSubKey = NULL);
			
	// QueryMultipleValues
	LONG RegQueryMultipleValues(PVALENT val_list, DWORD num_vals,
		LPTSTR lpValueBuf, LPDWORD ldwTotsize, LPCTSTR lpSubKey = NULL);
			
	// DeleteValue
	LONG DeleteValue(LPCTSTR lpValueName, LPCTSTR lpSubKey = NULL);
		
	// EnumValue
	LONG EnumValue(DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData, LPCTSTR lpSubKey = NULL);
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
	ASSERT(hKey != NULL);
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
	ASSERT(hKey != NULL);
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
	PFILETIME lpftLastWriteTime, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegQueryInfoKey(hkey, lpClass, lpcbClass, NULL, lpcSubKeys, lpcbMaxSubKeyLen,
			lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor,
			lpftLastWriteTime);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegQueryInfoKey(m_hKey, lpClass, lpcbClass, NULL, lpcSubKeys, lpcbMaxSubKeyLen,
			lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor,
			lpftLastWriteTime);
}

// GetKeySecurity
inline LONG RegKey::GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegGetKeySecurity(hkey, SecurityInformation, pSecurityDescriptor,
			lpcbSecurityDescriptor);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegGetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor,
			lpcbSecurityDescriptor);
}

// SetKeySecurity
inline LONG RegKey::SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSetKeySecurity(hkey, SecurityInformation, pSecurityDescriptor);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegSetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor);
}

// DeleteKey
inline LONG RegKey::DeleteSubKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpSubKey != NULL);
	return RegDeleteKey(m_hKey, lpSubKey);
}

inline LONG RegKey::RecurseDeleteKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpSubKey != NULL);
	HKEY hkey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ | KEY_WRITE, &hkey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	DWORD dwSize = MAX_PATH+1;
	TCHAR szBuffer[MAX_PATH+1];
	while (RegEnumKeyEx(hkey, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time)==ERROR_SUCCESS)
	{
		RegDeleteKey(hkey, szBuffer);
		dwSize = MAX_PATH+1;
	}
	RegCloseKey(hkey);
}

// EnumKey
inline LONG RegKey::EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName,
	LPTSTR lpClass, PDWORD lpcbClass, PFILETIME lpftLastWriteTime, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_ENUMERATE_SUB_KEYS, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegEnumKeyEx(hkey, dwIndex, lpName, lpcbName, NULL, lpClass, lpcbClass,
			lpftLastWriteTime);
		RegCloseKey(hkey);
		return lRes;
	}
	else
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
inline LONG RegKey::SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSaveKey(hkey, lpFile, lpSecurityAttributes);
		RegCloseKey(hkey);
		return lRes;
	}
	else
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
inline LONG RegKey::RestoreKey(LPCTSTR lpFile, DWORD dwFlags, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != lpFile);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegRestoreKey(hkey, lpFile, dwFlags);
		RegCloseKey(hkey);
		return lRes;
	}
	return RegRestoreKey(m_hKey, lpFile, dwFlags);
}

// NotifyChangeKeyValue
inline LONG RegKey::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
	BOOL fAsynchronous, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegNotifyChangeKeyValue(hkey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent,
			fAsynchronous);
}

// Values
// SetValue
inline LONG RegKey::SetValue(DWORD dwValue, LPCTSTR lpValueName, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSetValueEx(hkey,lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
			sizeof(DWORD));
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegSetValueEx(m_hKey, lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
			sizeof(DWORD));

}

inline LONG RegKey::SetValue(LPCTSTR lpValue, LPCTSTR lpValueName, LONG lSize, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSetValueEx(hkey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpValue,
			lSize ? lSize : (lstrlen(lpValue)+1)*sizeof(TCHAR));
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegSetValueEx(m_hKey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpValue,
			lSize ? lSize : (lstrlen(lpValue)+1)*sizeof(TCHAR));
}

inline LONG RegKey::SetValue(CONST BYTE *lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
	DWORD dwType, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSetValueEx(hkey, lpValueName, NULL, dwType, lpValue, dwValueSize);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegSetValueEx(m_hKey, lpValueName, NULL, dwType, lpValue, dwValueSize);
}

// QueryValue
inline LONG RegKey::QueryValue(DWORD &dwValue, LPCTSTR lpValueName, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	HKEY hkey = NULL;
	LONG lRes;
	
	if(lpSubKey)
	{
		lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
	}
	lRes = RegQueryValueEx(lpSubKey ? hkey : m_hKey, lpValueName, NULL, &dwType,
		(LPBYTE)&dwValue,&dwCount);
	
	ASSERT(lRes != ERROR_SUCCESS || dwType != REG_DWORD || dwCount == sizeof(DWORD));
	if(lpSubKey)
		RegCloseKey(hkey);
	return lRes;
}

inline LONG RegKey::QueryValue(LPTSTR lpValue, LPCTSTR lpValueName,	LPDWORD lpCount,
	LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	DWORD dwType =NULL;
	DWORD dwCount = lpCount ? *lpCount : MAX_PATH+1;
	HKEY hkey = NULL;
	LONG lRes;

	if(lpSubKey)
	{
		lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
	}
	
	lRes = RegQueryValueEx(lpSubKey ? hkey : m_hKey, lpValueName, NULL, &dwType, (LPBYTE)lpValue,
		&dwCount);
	if(lpCount)
		*lpCount = dwCount;
	
	ASSERT(lRes!=ERROR_SUCCESS || (dwType != REG_SZ || dwType != REG_MULTI_SZ ||
		dwType != REG_EXPAND_SZ));
	if(lpSubKey)
		RegCloseKey(hkey);
	return lRes;
}

inline LONG RegKey::QueryValue(LPBYTE lpValue, LPCTSTR lpValueName, LPDWORD lpCount,
	LPDWORD lpType, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	ASSERT(lpValue != NULL);
	DWORD dwType;
	DWORD dwCount = lpCount ? *lpCount : MAX_PATH+1;
	HKEY hkey=NULL;
	LONG lRes;

	if(lpSubKey)
	{
		lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
	}

	lRes = RegQueryValueEx(lpSubKey ? hkey : m_hKey, lpValueName, NULL, &dwType, (LPBYTE)lpValue,
		&dwCount);
	if(lpCount)
		*lpCount = dwCount;
	if(lpType)
		*lpType = dwType;

	if(lpSubKey)
		RegCloseKey(hkey);

	return lRes;

}

// DeleteValue
inline LONG RegKey::DeleteValue(LPCTSTR lpValueName, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegDeleteValue(hkey, lpValueName);
		RegCloseKey(hkey);
		return lRes;
	}
	return RegDeleteValue(m_hKey, lpValueName);
}

// EnumValue
inline LONG RegKey::EnumValue(DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes= RegEnumValue(hkey, dwIndex, lpValueName, lpcbValueName, NULL, lpType, lpData,
			lpcbData);
		RegCloseKey(hkey);
		return lRes;
	}
	else	
		return RegEnumValue(m_hKey, dwIndex, lpValueName, lpcbValueName, NULL, lpType, lpData,
			lpcbData);
}

#else // ! __cplusplus

typedef struct tagRegKey
{
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
	LONG Attach(HKEY hKey, BOOL bOpen = FALSE, REGSAM samDesired = KEY_ALL_ACCESS);

	// Create
	LONG Create(HKEY hKey, LPCTSTR lpSubKey, LPTSTR lpClass = REG_NONE,
		DWORD dwOptions = REG_OPTION_NON_VOLATILE, REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, LPDWORD lpdwDisposition = NULL);
	// Open
	LONG Open(HKEY hKey, LPCTSTR lpSubKey,REGSAM samDesired = KEY_ALL_ACCESS);
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
		LPDWORD lpcbSecurityDescriptor = NULL, PFILETIME lpftLastWriteTime = NULL,
		LPCTSTR lpSubKey = NULL);
	
	// GetKeySecurity
	LONG GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL,
		LPDWORD lpcbSecurityDescriptor = NULL, LPCTSTR lpSubKey = NULL);
		
	// SetKeySecurity
	LONG SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor, LPCTSTR lpSubKey = NULL);
	
	// DeleteKey
	LONG DeleteSubKey(LPCTSTR lpSubKey);
	LONG RecurseDeleteKey(LPCTSTR lpSubKey);
	
	// EnumKey
	LONG EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName, LPTSTR lpClass = NULL,
		PDWORD lpcbClass = NULL, PFILETIME lpftLastWriteTime = NULL, LPCTSTR lpSubKey = NULL);
		
	// LoadKey
	LONG LoadKey(LPCTSTR lpFile, LPCTSTR lpSubKey = NULL);
	
	// UnloadKey
	LONG UnloadKey(LPCTSTR lpSubKey);
	
	// SaveKey
	LONG SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, LPCTSTR lpSubKey = NULL);
	
	// ReplaceKey
	LONG ReplaceKey(LPCTSTR lpNewFile, LPCTSTR lpOldFile = NULL, LPCTSTR lpSubKey = NULL);
	
	// RestoreKey
	LONG RestoreKey(LPCTSTR lpFile, DWORD dwFlags = REG_OPTION_NON_VOLATILE, LPCTSTR lpSubKey = NULL);
		
	// NotifyChangeKeyValue
	LONG NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
		BOOL fAsynchronous, LPCTSTR lpSubKey = NULL);
	
	// Values
	// SetValue
	LONG SetValue(DWORD dwValue, LPCTSTR lpValueName, LPCTSTR lpSubKey = NULL);
	LONG SetValue(LPCTSTR lpValue, LPCTSTR lpValueName, LONG lSize = 0, LPCTSTR lpSubKey = NULL);
	LONG SetValue(CONST BYTE *lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
		DWORD dwType = REG_BINARY, LPCTSTR lpSubKey = NULL);
		
	// QueryValue
	LONG QueryValue(DWORD &dwValue, LPCTSTR lpValueName, LPCTSTR lpSubKey = NULL);
	LONG QueryValue(LPTSTR lpValue, LPCTSTR lpValueName, LPDWORD lpCount = NULL, 
		LPCTSTR lpSubKey = NULL);
	LONG QueryValue(LPBYTE lpValue, LPCTSTR lpValueName, LPDWORD lpCount, LPDWORD lpType,
		LPCTSTR lpSubKey = NULL);
			
	// QueryMultipleValues
	LONG RegQueryMultipleValues(PVALENT val_list, DWORD num_vals,
		LPTSTR lpValueBuf, LPDWORD ldwTotsize, LPCTSTR lpSubKey = NULL);
			
	// DeleteValue
	LONG DeleteValue(LPCTSTR lpValueName, LPCTSTR lpSubKey = NULL);
		
	// EnumValue
	LONG EnumValue(DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData, LPCTSTR lpSubKey = NULL);
}RegKey;

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
	ASSERT(hKey != NULL);
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
	ASSERT(hKey != NULL);
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
	PFILETIME lpftLastWriteTime, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegQueryInfoKey(hkey, lpClass, lpcbClass, NULL, lpcSubKeys, lpcbMaxSubKeyLen,
			lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor,
			lpftLastWriteTime);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegQueryInfoKey(m_hKey, lpClass, lpcbClass, NULL, lpcSubKeys, lpcbMaxSubKeyLen,
			lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor,
			lpftLastWriteTime);
}

// GetKeySecurity
inline LONG RegKey::GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegGetKeySecurity(hkey, SecurityInformation, pSecurityDescriptor,
			lpcbSecurityDescriptor);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegGetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor,
			lpcbSecurityDescriptor);
}

// SetKeySecurity
inline LONG RegKey::SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSetKeySecurity(hkey, SecurityInformation, pSecurityDescriptor);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegSetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor);
}

// DeleteKey
inline LONG RegKey::DeleteSubKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpSubKey != NULL);
	return RegDeleteKey(m_hKey, lpSubKey);
}

inline LONG RegKey::RecurseDeleteKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpSubKey != NULL);
	HKEY hkey;
	LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ | KEY_WRITE, &hkey);
	if(lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	DWORD dwSize = MAX_PATH+1;
	TCHAR szBuffer[MAX_PATH+1];
	while (RegEnumKeyEx(hkey, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time)==ERROR_SUCCESS)
	{
		RegDeleteKey(hkey, szBuffer);
		dwSize = MAX_PATH+1;
	}
	RegCloseKey(hkey);
}

// EnumKey
inline LONG RegKey::EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName,
	LPTSTR lpClass, PDWORD lpcbClass, PFILETIME lpftLastWriteTime, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_ENUMERATE_SUB_KEYS, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegEnumKeyEx(hkey, dwIndex, lpName, lpcbName, NULL, lpClass, lpcbClass,
			lpftLastWriteTime);
		RegCloseKey(hkey);
		return lRes;
	}
	else
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
inline LONG RegKey::SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_READ, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSaveKey(hkey, lpFile, lpSecurityAttributes);
		RegCloseKey(hkey);
		return lRes;
	}
	else
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
inline LONG RegKey::RestoreKey(LPCTSTR lpFile, DWORD dwFlags, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != lpFile);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegRestoreKey(hkey, lpFile, dwFlags);
		RegCloseKey(hkey);
		return lRes;
	}
	return RegRestoreKey(m_hKey, lpFile, dwFlags);
}

// NotifyChangeKeyValue
inline LONG RegKey::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
	BOOL fAsynchronous, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_WRITE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegNotifyChangeKeyValue(hkey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent,
			fAsynchronous);
}

// Values
// SetValue
inline LONG RegKey::SetValue(DWORD dwValue, LPCTSTR lpValueName, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSetValueEx(hkey,lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
			sizeof(DWORD));
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegSetValueEx(m_hKey, lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
			sizeof(DWORD));

}

inline LONG RegKey::SetValue(LPCTSTR lpValue, LPCTSTR lpValueName, LONG lSize, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSetValueEx(hkey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpValue,
			lSize ? lSize : (lstrlen(lpValue)+1)*sizeof(TCHAR));
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegSetValueEx(m_hKey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpValue,
			lSize ? lSize : (lstrlen(lpValue)+1)*sizeof(TCHAR));
}

inline LONG RegKey::SetValue(CONST BYTE *lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
	DWORD dwType, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegSetValueEx(hkey, lpValueName, NULL, dwType, lpValue, dwValueSize);
		RegCloseKey(hkey);
		return lRes;
	}
	else
		return RegSetValueEx(m_hKey, lpValueName, NULL, dwType, lpValue, dwValueSize);
}

// QueryValue
inline LONG RegKey::QueryValue(DWORD &dwValue, LPCTSTR lpValueName, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	HKEY hkey = NULL;
	LONG lRes;
	
	if(lpSubKey)
	{
		lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
	}
	lRes = RegQueryValueEx(lpSubKey ? hkey : m_hKey, lpValueName, NULL, &dwType,
		(LPBYTE)&dwValue,&dwCount);
	
	ASSERT(lRes != ERROR_SUCCESS || dwType != REG_DWORD || dwCount == sizeof(DWORD));
	if(lpSubKey)
		RegCloseKey(hkey);
	return lRes;
}

inline LONG RegKey::QueryValue(LPTSTR lpValue, LPCTSTR lpValueName,	LPDWORD lpCount,
	LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	DWORD dwType =NULL;
	DWORD dwCount = lpCount ? *lpCount : MAX_PATH+1;
	HKEY hkey = NULL;
	LONG lRes;

	if(lpSubKey)
	{
		lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
	}
	
	lRes = RegQueryValueEx(lpSubKey ? hkey : m_hKey, lpValueName, NULL, &dwType, (LPBYTE)lpValue,
		&dwCount);
	if(lpCount)
		*lpCount = dwCount;
	
	ASSERT(lRes!=ERROR_SUCCESS || (dwType != REG_SZ || dwType != REG_MULTI_SZ ||
		dwType != REG_EXPAND_SZ));
	if(lpSubKey)
		RegCloseKey(hkey);
	return lRes;
}

inline LONG RegKey::QueryValue(LPBYTE lpValue, LPCTSTR lpValueName, LPDWORD lpCount,
	LPDWORD lpType, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	ASSERT(lpValue != NULL);
	DWORD dwType;
	DWORD dwCount = lpCount ? *lpCount : MAX_PATH+1;
	HKEY hkey;
	LONG lRes;

	if(lpSubKey)
	{
		lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
	}

	lRes = RegQueryValueEx(lpSubKey ? hkey : m_hKey, lpValueName, NULL, &dwType, (LPBYTE)lpValue,
		&dwCount);
	if(lpCount)
		*lpCount = dwCount;
	if(lpType)
		*lpType = dwType;

	if(lpSubKey)
		RegCloseKey(hkey);

	return lRes;

}

// DeleteValue
inline LONG RegKey::DeleteValue(LPCTSTR lpValueName, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_SET_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes = RegDeleteValue(hkey, lpValueName);
		RegCloseKey(hkey);
		return lRes;
	}
	return RegDeleteValue(m_hKey, lpValueName);
}

// EnumValue
inline LONG RegKey::EnumValue(DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	if(lpSubKey)
	{
		HKEY hkey;
		LONG lRes = RegOpenKeyEx(m_hKey, lpSubKey, NULL, KEY_QUERY_VALUE, &hkey);
		if(lRes != ERROR_SUCCESS)
			return lRes;
		lRes= RegEnumValue(hkey, dwIndex, lpValueName, lpcbValueName, NULL, lpType, lpData,
			lpcbData);
		RegCloseKey(hkey);
		return lRes;
	}
	else	
		return RegEnumValue(m_hKey, dwIndex, lpValueName, lpcbValueName, NULL, lpType, lpData,
			lpcbData);
}

#ifdef __NOASSERT__
#undef __NOASSERT__
#undef ASSERT
#endif // __NOASSERT__

#endif // __REGKEY_CLASS__
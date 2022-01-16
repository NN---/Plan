// RegKey class
//

#include "stdafx.h"
#include "RegKey.h"

// No Problems
RegKey::RegKey(const RegKey&)
{
}

RegKey& RegKey::operator=(const RegKey&)
{
	return *this;
}

// Attributes
RegKey::operator HKEY()
{
	return GetHandle();
}

HKEY RegKey::GetHandle()
{
	return m_hKey;
}

// Construction
RegKey::RegKey()
{
	m_hKey = NULL;
}

// Destruction
RegKey::~RegKey()
{
	Close();
}

// Functions
// Close
LONG RegKey::Close()
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
HKEY RegKey::Detach()
{
	HKEY hKey = m_hKey;
	m_hKey = NULL;
	return hKey;
}

// Attach
LONG RegKey::Attach(HKEY hKey, BOOL bOpen, REGSAM samDesired)
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
LONG RegKey::Create(HKEY hKey, LPCTSTR lpSubKey, LPTSTR lpClass, DWORD dwOptions,
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
LONG RegKey::Open(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired)
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
LONG RegKey::Flush()
{
	ASSERT(m_hKey != NULL);
	return RegFlushKey(m_hKey);
}

// Connect
LONG RegKey::Connect(HKEY hKey, LPCTSTR lpMachineName)
{
	HKEY hkey;
	LONG lRes;

	lRes=RegConnectRegistry(lpMachineName, hKey, &hkey);
	if(lRes == ERROR_SUCCESS)
	{
		Close();
		m_hKey = hkey;
	}
	return lRes;
}

// Keys
// SetKeyValue
LONG RegKey::SetKeyValue(LPCTSTR lpValue, LONG lSize, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValue(m_hKey, lpSubKey, REG_SZ, lpValue,
		lSize ? lSize : (lstrlen(lpValue)+1)*sizeof(TCHAR));
}

// QueryKeyValue
LONG RegKey::QueryKeyValue(LPTSTR lpValue, PLONG lpCount, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpCount != NULL);
	return RegQueryValue(m_hKey, lpSubKey, lpValue, lpCount);
}

// QueryInfoKey
LONG RegKey::QueryInfoKey(LPTSTR lpClass, LPDWORD lpcbClass, LPDWORD lpcSubKeys,
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
LONG RegKey::GetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor)
{
	ASSERT(m_hKey != NULL);
	return RegGetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor,
		lpcbSecurityDescriptor);
}

// SetKeySecurity
LONG RegKey::SetKeySecurity(SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	ASSERT(m_hKey != NULL);
	return RegSetKeySecurity(m_hKey, SecurityInformation, pSecurityDescriptor);
}

// DeleteKey
LONG RegKey::DeleteSubKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpSubKey != NULL);
	return RegDeleteKey(m_hKey, lpSubKey);
}

LONG RegKey::RecurseDeleteKey(LPCTSTR lpSubKey)
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
	return ERROR_SUCCESS;
}

// EnumKey
LONG RegKey::EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcbName,
	LPTSTR lpClass, PDWORD lpcbClass, PFILETIME lpftLastWriteTime)
{
	ASSERT(m_hKey != NULL);
	return RegEnumKeyEx(m_hKey, dwIndex, lpName, lpcbName, NULL, lpClass, lpcbClass,
		lpftLastWriteTime);
}

// LoadKey
LONG RegKey::LoadKey(LPCTSTR lpFile, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != NULL);
	return RegLoadKey(m_hKey, lpSubKey, lpFile);
}

// UnloadKey
LONG RegKey::UnloadKey(LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	return RegUnLoadKey(m_hKey, lpSubKey);
}

// SaveKey
LONG RegKey::SaveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != NULL);
	return RegSaveKey(m_hKey, lpFile, lpSecurityAttributes);
}

// ReplaceKey
LONG RegKey::ReplaceKey(LPCTSTR lpNewFile, LPCTSTR lpOldFile, LPCTSTR lpSubKey)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpNewFile != NULL);
	ASSERT(lpOldFile != NULL);
	return RegReplaceKey(m_hKey, lpSubKey, lpNewFile, lpOldFile);
}

// RestoreKey
LONG RegKey::RestoreKey(LPCTSTR lpFile, DWORD dwFlags)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpFile != lpFile);
	return RegRestoreKey(m_hKey, lpFile, dwFlags);
}

// NotifyChangeKeyValue
LONG RegKey::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent,
	BOOL fAsynchronous)
{
	ASSERT(m_hKey != NULL);
	return RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent,
		fAsynchronous);
}

// Values
// SetValue
LONG RegKey::SetValue(DWORD dwValue, LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, REG_DWORD, (CONST BYTE *)&dwValue,
			sizeof(DWORD));

}

LONG RegKey::SetValue(LPCTSTR lpValue, LPCTSTR lpValueName, LONG lSize)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, REG_SZ, (CONST BYTE *)lpValue,
		lSize ? lSize : (lstrlen(lpValue)+1)*sizeof(TCHAR));
}

LONG RegKey::SetValue(const LPBYTE lpValue, DWORD dwValueSize, LPCTSTR lpValueName,
	DWORD dwType)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValue != NULL);
	return RegSetValueEx(m_hKey, lpValueName, NULL, dwType, lpValue, dwValueSize);
}

// QueryValue
LONG RegKey::QueryValue(DWORD &dwValue, LPCTSTR lpValueName)
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

LONG RegKey::QueryValue(LPTSTR lpValue, LPCTSTR lpValueName,	LPDWORD lpCount)
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

LONG RegKey::QueryValue(LPBYTE lpValue, LPCTSTR lpValueName, LPDWORD lpCount,
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
LONG RegKey::DeleteValue(LPCTSTR lpValueName)
{
	ASSERT(m_hKey != NULL);
	ASSERT(lpValueName != NULL);
	return RegDeleteValue(m_hKey, lpValueName);
}

// EnumValue
LONG RegKey::EnumValue(DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcbValueName,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	ASSERT(m_hKey != NULL);
	return RegEnumValue(m_hKey, dwIndex, lpValueName, lpcbValueName, NULL, lpType, lpData,
		lpcbData);
}
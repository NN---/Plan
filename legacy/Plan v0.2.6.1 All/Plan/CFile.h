//	Project: RSDN
//	File:    CFile.h
//	Author:  Alexander Korsukov, Paul Bludov
//	Date:    6/24/2002
//
//	Description:
//		C++ wrapper on Win32 file, named pipe, mailsot, comm & tape devices
//
//	Update History:
//		PB 10/30/2002
//			* Output values now pointers instead of references
//		PB 11/04/2002 
//			+ A lot of methods
//			+ CConsole, CDevice
//		
/////////////////////////////////////////////////////////////////////////////

#ifndef __RSDN_CFILE_H__
#define __RSDN_CFILE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "chandle.h"

/////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CFileT<t_bManaged>
// CConsoleT<t_bManaged>
// CCompletionPortT<t_bManaged>
// CFileMappingT<t_bManaged>
// CNamedPipeT<t_bManaged>
// CMailslotT<t_bManaged>
// CDeviceT<t_bManaged>
// CCommFileT<t_bManaged>
// CTapeT<t_bManaged>
//

namespace RSDN
{

/////////////////////////////////////////////////////////////////////////////
// File

template<bool t_bManaged>
class CFileT
	: public CHandleT<t_bManaged>
{
	typedef CHandleT<t_bManaged> Base;
public:
#if _MSC_VER>=7000
	using RSDN::CHandleT<t_bManaged>::GetInformation;
#endif // _MSC_VER>=7000

	CFileT()
	{ }
	CFileT
		( const CFileT<true>& rFile
		)
		: Base(rFile)
	{ }
	CFileT
		( const CFileT<false>& rFile
		)
		: Base(rFile)
	{ }
	explicit CFileT
		( HANDLE hFile
		)
		: Base((INVALID_HANDLE_VALUE != hFile) ? hFile : NULL)
	{ }

	CFileT
		( LPCSTR pchFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(pchFilename, nDesiredAccess, nShareMode, nCreationDisposition
			, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}
	CFileT
		( LPCWSTR pwchFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(pwchFilename, nDesiredAccess, nShareMode, nCreationDisposition
			, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}


#if (_WIN32_WINNT >= 0x0502)
	CFileT
		( HANDLE hOriginalFile
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		)
	{
		ReOpen(hOriginalFile, nDesiredAccess, nShareMode
			, nFlagsAndAttributes);
		_ASSERTE(IsValid());
	}
#endif // _WIN32_WINNT >= 0x0502

	CFileT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=((INVALID_HANDLE_VALUE != h) ? h : NULL);
		return *this;
	}

	CFileT<t_bManaged>& operator=(const CFileT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CFileT<t_bManaged>& operator=(const CFileT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	HANDLE Create
		( LPCSTR pchFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		_ASSERTE(!IsValid());
		HANDLE h;

		h = ::CreateFileA(pchFilename, nDesiredAccess, nShareMode, lpsa
			, nCreationDisposition, nFlagsAndAttributes, hTemplateFile);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}
	HANDLE Create
		( LPCWSTR pwchFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		_ASSERTE(!IsValid());
		HANDLE h;

		h = ::CreateFileW(pwchFilename, nDesiredAccess, nShareMode, lpsa
			, nCreationDisposition, nFlagsAndAttributes, hTemplateFile);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}

#if (_WIN32_WINNT >= 0x0502)
	HANDLE ReOpen
		( HANDLE hOriginalFile
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		)
	{
		_ASSERTE(!IsValid());
		_ASSERTE(IsValidHandle(hOriginalFile));
		HANDLE h;

		h = ::ReOpenFile(hOriginalFile, nDesiredAccess, nShareMode
			, nFlagsAndAttributes);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}
#endif // _WIN32_WINNT >= 0x0502

	// Read/write

	int Read
		( LPVOID pBuffer
		, DWORD nBufSize
		, LPDWORD lpnBytesRead = NULL
		)
	{
		_ASSERTE(IsValid());
		DWORD nBytesRead = 0;

		if (lpnBytesRead == NULL)
			lpnBytesRead = &nBytesRead;

		if (!::ReadFile(m_h, pBuffer, nBufSize, lpnBytesRead, NULL))
			return FALSE;
		if (*lpnBytesRead != nBufSize)
			return ERROR_HANDLE_EOF;
		return TRUE;
	}

	// this function will usually return FALSE
	// indicating succesful queueing of the operation
	BOOL Read
		( LPVOID lpBuffer
		, DWORD nBufSize
		, LPOVERLAPPED lpOverlapped
		)
	{
		_ASSERTE(IsValid());

		return ::ReadFile(m_h, lpBuffer, nBufSize, NULL, lpOverlapped);
	}

	BOOL Read
		( LPVOID lpBuffer
		, DWORD nBufSize
		, LPOVERLAPPED lpOverlapped
		, LPOVERLAPPED_COMPLETION_ROUTINE lpfnCompletionRoutine
		)
	{
		_ASSERTE(IsValid());

		return ::ReadFileEx(m_h, lpBuffer, nBufSize, lpOverlapped
			, lpfnCompletionRoutine);
	}

	BOOL ReadScatter
		( FILE_SEGMENT_ELEMENT rgnSegmentArray[]
		, DWORD nNumberOfBytesToRead
		, LPOVERLAPPED lpOverlapped
		)
	{
		_ASSERTE(lpOverlapped);
		_ASSERTE(IsValid());

		return ::ReadFileScatter(m_h, rgnSegmentArray, nNumberOfBytesToRead
				, NULL, lpOverlapped);
	}

	int Write
		( LPCVOID pBuffer
		, DWORD nBufSize
		, LPDWORD lpnBytesWritten = NULL
		)
	{
		_ASSERTE(IsValid());
		DWORD nBytesWritten;

		if (lpnBytesWritten == NULL)
			lpnBytesWritten = &nBytesWritten;

		if (!::WriteFile(m_h, pBuffer, nBufSize, lpnBytesWritten, NULL))
			return FALSE;
		if (*lpnBytesWritten != nBufSize)
			return ERROR_HANDLE_DISK_FULL;
		return TRUE;
	}

	// this function will usually return FALSE
	// indicating succesful queueing of the operation
	BOOL Write
		( LPCVOID pBuffer
		, DWORD nBufSize
		, LPOVERLAPPED pOverlapped
		)
	{
		_ASSERTE(IsValid());

		return ::WriteFile(m_h, pBuffer, nBufSize, NULL, pOverlapped);
	}

	BOOL Write
		( LPCVOID pBuffer
		, DWORD nBufSize
		, LPOVERLAPPED pOverlapped
		, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine
		)
	{
		_ASSERTE(IsValid());

		return ::WriteFileEx(m_h, pBuffer, nBufSize, pOverlapped
			, pfnCompletionRoutine);
	}

	BOOL WriteGather
		( FILE_SEGMENT_ELEMENT rgnSegmentArray[]
		, DWORD nNumberOfBytesToWrite
		, LPOVERLAPPED lpOverlapped
		)
	{
		_ASSERTE(lpOverlapped);
		_ASSERTE(IsValid());

		return ::WriteFileGather(m_h, rgnSegmentArray, nNumberOfBytesToWrite
				, NULL, lpOverlapped);
	}

	// this function returns FALSE
	// if bWait is FALSE and the operation is still pending
	BOOL GetOverlappedResult
		( LPOVERLAPPED pOverlapped
		, BOOL bWait = FALSE
		, LPDWORD lpnBytesTransferred = NULL
		)
	{
		_ASSERTE(IsValid());
		DWORD nBytesTransferred;

		if (lpnBytesTransferred == NULL)
			lpnBytesTransferred = &nBytesTransferred;

		return ::GetOverlappedResult(m_h, pOverlapped, lpnBytesTransferred
			, bWait);
	}

	// File pointer & size

	ULONGLONG Seek
		( LONGLONG nOffset
		, DWORD nFrom = FILE_CURRENT
		)
	{
		_ASSERTE(FILE_BEGIN == nFrom || FILE_END == nFrom
			|| FILE_CURRENT == nFrom);
		_ASSERTE(IsValid());
		LARGE_INTEGER liOffset;

		liOffset.QuadPart = nOffset;
		liOffset.LowPart = ::SetFilePointer(m_h, liOffset.LowPart
			, &liOffset.HighPart, nFrom);
		if (INVALID_SET_FILE_POINTER == liOffset.LowPart
			&& NOERROR != ::GetLastError())
		{
			liOffset.HighPart = INVALID_SET_FILE_POINTER;
		}
		return liOffset.QuadPart;
	}

	ULONGLONG GetPosition() const
	{
		_ASSERTE(IsValid());
		LARGE_INTEGER liOffset;

		liOffset.QuadPart = 0;
		liOffset.LowPart = ::SetFilePointer(m_h, 0, &liOffset.HighPart
			, FILE_CURRENT);
		if (INVALID_SET_FILE_POINTER == liOffset.LowPart
			&& NOERROR != ::GetLastError())
		{
			liOffset.HighPart = INVALID_SET_FILE_POINTER;
		}
		return liOffset.QuadPart;
	}

	BOOL SetEndOfFile()
	{
		_ASSERTE(IsValid());

		return ::SetEndOfFile(m_h);
	}

	BOOL SetSize
		( ULONGLONG nNewLen
		)
	{
		_ASSERTE(IsValid());
		LARGE_INTEGER liOffset;

		liOffset.QuadPart = Seek(nNewLen, FILE_BEGIN);

		if (INVALID_SET_FILE_POINTER == liOffset.LowPart
			&& INVALID_SET_FILE_POINTER == liOffset.HighPart)
		{
			return FALSE;
		}

		return ::SetEndOfFile(m_h);
	}

	ULONGLONG GetSize() const
	{
		_ASSERTE(IsValid());
		ULARGE_INTEGER liFileSize;

		liFileSize.LowPart = ::GetFileSize(m_h, &liFileSize.HighPart);
		if (INVALID_FILE_SIZE == liFileSize.LowPart
			&& NOERROR != ::GetLastError())
		{
			liFileSize.HighPart = INVALID_FILE_SIZE;
		}

		return liFileSize.QuadPart;
	}

	// IO Completion operations

	BOOL CancelIo()
	{
		_ASSERTE(IsValid());

		return ::CancelIo(m_h);
	}

	HANDLE CreateIoCompletionPort
		( HANDLE hExistingCompletionPort
		, ULONG_PTR pnCompletionKey
		, DWORD nNumberOfConcurrentThreads = 0
		)
	{
		_ASSERTE(IsValid());

		return ::CreateIoCompletionPort(m_h, hExistingCompletionPort
				, pnCompletionKey, nNumberOfConcurrentThreads);
	}

#if (_WIN32_WINNT >= 0x0500)
	BOOL BindIoCompletionCallback
		( LPOVERLAPPED_COMPLETION_ROUTINE pfnFunction
		)
	{
		_ASSERTE(pfnFunction);
		_ASSERTE(IsValid());

		return ::BindIoCompletionCallback(m_h, pfnFunction, 0);
	}
#endif

	BOOL Flush()
	{
		_ASSERTE(IsValid());

		return ::FlushFileBuffers(m_h);
	}

	// Locking

	BOOL LockRange
		( ULONGLONG nPos
		, ULONGLONG nCount
		)
	{
		_ASSERTE(IsValid());
		ULARGE_INTEGER liPos;
		ULARGE_INTEGER liCount;

		liPos.QuadPart = nPos;
		liCount.QuadPart = nCount;
		return ::LockFile(m_h, liPos.LowPart, liPos.HighPart
				, liCount.LowPart, liCount.HighPart);
	}

	BOOL LockRangeEx
		( ULONGLONG nCount
		, DWORD nFlags
		, LPOVERLAPPED lpOverlapped = NULL
		)
	{
		_ASSERTE(0 == (nFlags & 
			~(LOCKFILE_FAIL_IMMEDIATELY | LOCKFILE_EXCLUSIVE_LOCK)));
		_ASSERTE(lpOverlapped);
		_ASSERTE(IsValid());
		ULARGE_INTEGER liCount;

		liCount.QuadPart = nCount;
		return ::LockFileEx(m_h, nFlags, 0, liCount.LowPart
				, liCount.HighPart, lpOverlapped);
	}

	BOOL UnlockRange
		( ULONGLONG nPos
		, ULONGLONG nCount
		)
	{
		_ASSERTE(IsValid());
		ULARGE_INTEGER liPos;
		ULARGE_INTEGER liCount;

		liPos.QuadPart = nPos;
		liCount.QuadPart = nCount;
		return ::UnlockFile(m_h, liPos.LowPart, liPos.HighPart
				, liCount.LowPart, liCount.HighPart);
	}

	BOOL UnlockRangeEx
		( ULONGLONG nCount
		, LPOVERLAPPED lpOverlapped = NULL
		)
	{
		_ASSERTE(lpOverlapped);
		_ASSERTE(IsValid());
		ULARGE_INTEGER liCount;

		liCount.QuadPart = nCount;
		return ::UnlockFileEx(m_h, 0, liCount.LowPart
				, liCount.HighPart, lpOverlapped);
	}

	// File attributes & info

	BOOL GetInformation
		( LPBY_HANDLE_FILE_INFORMATION lpFileInformation
		) const
	{
		_ASSERTE(lpFileInformation);
		_ASSERTE(IsValid());

		return ::GetFileInformationByHandle(m_h, lpFileInformation);
	}

	DWORD GetType() const
	{
		_ASSERTE(IsValid());

		return ::GetFileType(m_h);
	}

	BOOL GetTime
		( LPFILETIME lpCreationTime
		, LPFILETIME lpLastAccessTime = NULL
		, LPFILETIME lpLastWriteTime = NULL
		) const
	{
		_ASSERTE(IsValid());

		return ::GetFileTime(m_h, lpCreationTime, lpLastAccessTime
			, lpLastWriteTime);
	}

	BOOL SetTime
		( CONST LPFILETIME lpCreationTime
		, CONST LPFILETIME lpLastAccessTime = NULL
		, CONST LPFILETIME lpLastWriteTime = NULL
		)
	{
		_ASSERTE(IsValid());

		return ::SetFileTime(m_h, lpCreationTime, lpLastAccessTime
			, lpLastWriteTime);
	}

	BOOL SetShortName
		( LPCSTR pchShortName
		)
	{
		_ASSERTE(pchShortName);
		_ASSERTE(IsValid());

		return ::SetFileShortNameA(m_h, pchShortName);
	}
	BOOL SetShortName
		( LPCWSTR pwchShortName
		)
	{
		_ASSERTE(pwchShortName);
		_ASSERTE(IsValid());

		return ::SetFileShortNameW(m_h, pwchShortName);
	}

	BOOL SetValidData
		( LONGLONG nValidDataLength
		)
	{
		_ASSERTE(IsValid());

		return ::SetFileValidData(m_h, nValidDataLength);
	}

	// Backup

	BOOL BackupRead
		( LPBYTE lpBuffer
		, DWORD nNumberOfBytesToRead
		, LPDWORD lpnNumberOfBytesRead
		, BOOL bAbort
		, BOOL bProcessSecurity
		, LPVOID *lplpContext
		)
	{
		_ASSERTE(lpnNumberOfBytesRead);
		_ASSERTE(lplpContext);
		_ASSERTE(IsValid());

		return ::BackupRead(m_h, lpBuffer, nNumberOfBytesToRead
			, lpnNumberOfBytesRead, bAbort, bProcessSecurity, lplpContext);
	}

	BOOL BackupSeek
		( ULONGLONG nBytesToSeek
		, PULONGLONG lpnByteSeeked
		, LPVOID *lplpContext
		)
	{
		_ASSERTE(lpnByteSeeked);
		_ASSERTE(lplpContext);
		_ASSERTE(IsValid());
		ULARGE_INTEGER liSeek;
		ULARGE_INTEGER liSeeked;

		liSeek.QuadPart = nBytesToSeek;
		if (!::BackupSeek(m_h, liSeek.LowPart, liSeek.HighPart
				, &liSeeked.LowPart, &liSeeked.HighPart, lplpContext))
		{
			*lpnByteSeeked = (ULONGLONG)-1;
			return FALSE;
		}

		*lpnByteSeeked = liSeeked.QuadPart;
		return TRUE;
	}

	BOOL BackupWrite
		( LPBYTE lpBuffer
		, DWORD nNumberOfBytesToWrite
		, LPDWORD lpnNumberOfBytesWritten
		, BOOL bAbort
		, BOOL bProcessSecurity
		, LPVOID *lplpContext
		)
	{
		_ASSERTE(lpnNumberOfBytesWritten);
		_ASSERTE(lplpContext);
		_ASSERTE(IsValid());

		return ::BackupWrite(m_h, lpBuffer, nNumberOfBytesToWrite
				, lpnNumberOfBytesWritten, bAbort, bProcessSecurity
				, lplpContext);
	}
};

typedef CFileT<true> CFile;
typedef CFileT<false> CFileHandle;

//@//////////////////////////////////////////////////////////////////////////
// StdFile

template<bool t_bManaged>
class CConsoleT
	: public CFileT<t_bManaged>
{
	typedef CFileT<t_bManaged> Base;
public:
#if _MSC_VER>=7000
	using RSDN::CFileT<t_bManaged>::Read;
	using RSDN::CFileT<t_bManaged>::Write;
#endif // _MSC_VER>=7000

	CConsoleT()
	{ }
	CConsoleT
		( const CConsoleT<true>& rFile
		)
		: Base(rFile)
	{ }
	CConsoleT
		( const CConsoleT<false>& rFile
		)
		: Base(rFile)
	{ }
	explicit CConsoleT
		( HANDLE hFile
		)
		: Base((INVALID_HANDLE_VALUE != hFile) ? hFile : NULL)
	{ }

	CConsoleT
		( LPCSTR szStdFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szStdFilename, nDesiredAccess, nShareMode
			, nCreationDisposition, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CConsoleT
		( LPCWSTR szStdFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szStdFilename, nDesiredAccess, nShareMode
			, nCreationDisposition, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	explicit CConsoleT
		( DWORD nStd
		)
	{
		GetStd(nStd);
		_ASSERTE(IsValid());
	}

	CConsoleT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=((INVALID_HANDLE_VALUE != h) ? h : NULL);
		return *this;
	}

	CConsoleT<t_bManaged>& operator=(const CConsoleT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CConsoleT<t_bManaged>& operator=(const CConsoleT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	HANDLE GetStd
		( DWORD nStdHandle
		)
	{
		_ASSERTE(!IsValid());
		HANDLE h;

		h = ::GetStdHandle(nStdHandle);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}

	BOOL SetStd
		( DWORD nStdHandle
		)
	{
		_ASSERTE(IsValid());

		return ::SetStdHandle(nStdHandle, m_h);
	}

	HANDLE CreateScreenBuffer
		( DWORD nDesiredAccess
		, DWORD nShareMode
		, CONST SECURITY_ATTRIBUTES *lpSecurityAttributes = NULL
		)
	{
		_ASSERTE(!IsValid());
		HANDLE h;

		h = ::CreateConsoleScreenBuffer(nDesiredAccess, nShareMode
			, CONSOLE_TEXTMODE_BUFFER, NULL);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}

	BOOL SetActiveScreenBuffer()
	{
		_ASSERTE(IsValid());

		return ::SetConsoleActiveScreenBuffer(m_h);
	}

	BOOL GetScreenBufferInfo
		( PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo
		) const
	{
		_ASSERTE(lpConsoleScreenBufferInfo);
		_ASSERTE(IsValid());

		return ::GetConsoleScreenBufferInfo(m_h, lpConsoleScreenBufferInfo);
	}

	BOOL SetScreenBufferSize
		( COORD nSize
		)
	{
		_ASSERTE(IsValid());

		return ::SetConsoleScreenBufferSize(m_h, nSize);
	}

	BOOL ScrollScreenBuffer
		( CONST SMALL_RECT *lpScrollRectangle
		, CONST SMALL_RECT *lpClipRectangle
		, COORD nDestinationOrigin
		, CONST CHAR_INFO *lpFill
		)
	{
		_ASSERTE(lpScrollRectangle);
		_ASSERTE(lpClipRectangle);
		_ASSERTE(lpFill);
		_ASSERTE(IsValid());

		return ::ScrollConsoleScreenBuffer(m_h, lpScrollRectangle
			, lpClipRectangle, nDestinationOrigin, lpFill);
	}

	BOOL PeekInput
		( PINPUT_RECORD lpBuffer
		, DWORD nLength
		, LPDWORD lpnNumberOfEventsRead
		)
	{
		_ASSERTE(IsValid());

		return ::PeekConsoleInput(m_h, lpBuffer, nLength
			, lpnNumberOfEventsRead);
	}
	
	BOOL ReadInput
		( PINPUT_RECORD lpBuffer
		, DWORD nLength
		, LPDWORD lpnNumberOfEventsRead
		)
	{
		_ASSERTE(IsValid());

		return ::ReadConsoleInput(m_h, lpBuffer, nLength
			, lpnNumberOfEventsRead);
	}

	BOOL WriteInput
		( CONST INPUT_RECORD *lpBuffer
		, DWORD nLength
		, LPDWORD lpnNumberOfEventsWritten
		)
	{
		_ASSERTE(IsValid());

		return ::WriteConsoleInput(m_h, lpBuffer, nLength
			, lpnNumberOfEventsWritten);
	}

	BOOL ReadOutput
		( PCHAR_INFO lpBuffer
		, COORD nBufferSize
		, COORD nBufferCoord
		, PSMALL_RECT lpReadRegion
		)
	{
		_ASSERTE(IsValid());

		return ::ReadConsoleOutput(m_h, lpBuffer, nBufferSize
			, nBufferCoord, lpReadRegion);
	}

	BOOL WriteOutput
		( CONST CHAR_INFO *lpBuffer
		, COORD nBufferSize
		, COORD nBufferCoord
		, PSMALL_RECT lpWriteRegion
		)
	{
		_ASSERTE(IsValid());

		return ::WriteConsoleOutput(m_h, lpBuffer, nBufferSize
			, nBufferCoord, lpWriteRegion);
	}

	BOOL ReadOutputCharacter
		( LPSTR lpCharacter
		, DWORD nLength
		, COORD nReadCoord
		, LPDWORD lpnNumberOfCharsRead
		)
	{
		_ASSERTE(IsValid());

		return ::ReadConsoleOutputCharacterA(m_h, lpCharacter, nLength
			, nReadCoord, lpnNumberOfCharsRead);
	}

	BOOL ReadOutputCharacter
		( LPWSTR lpCharacter
		, DWORD nLength
		, COORD nReadCoord
		, LPDWORD lpnNumberOfCharsRead
		)
	{
		_ASSERTE(IsValid());

		return ::ReadConsoleOutputCharacterW(m_h, lpCharacter, nLength
			, nReadCoord, lpnNumberOfCharsRead);
	}

	BOOL WriteOutputCharacter
		( LPCSTR lpCharacter
		, DWORD nLength
		, COORD nWriteCoord
		, LPDWORD lpnNumberOfCharsWritten
		)
	{
		_ASSERTE(IsValid());

		return ::WriteConsoleOutputCharacterA(m_h, lpCharacter, nLength
			, nWriteCoord, lpnNumberOfCharsWritten);
	}

	BOOL WriteOutputCharacter
		( LPCWSTR lpCharacter
		, DWORD nLength
		, COORD nWriteCoord
		, LPDWORD lpnNumberOfCharsWritten
		)
	{
		_ASSERTE(IsValid());

		return ::WriteConsoleOutputCharacterW(m_h, lpCharacter, nLength
			, nWriteCoord, lpnNumberOfCharsWritten);
	}

	BOOL ReadOutputAttribute
		( LPWORD lpAttribute
		, DWORD nLength
		, COORD nReadCoord
		, LPDWORD lpnNumberOfAttrsRead
		)
	{
		_ASSERTE(IsValid());

		return ::ReadConsoleOutputAttribute(m_h, lpAttribute, nLength
			, nReadCoord, lpnNumberOfAttrsRead);
	}

	BOOL WriteOutputAttribute
		( CONST WORD *lpAttribute
		, DWORD nLength
		, COORD nWriteCoord
		, LPDWORD lpnNumberOfAttrsWritten
		)
	{
		_ASSERTE(IsValid());

		return ::WriteConsoleOutputAttribute(m_h, lpAttribute, nLength
			, nWriteCoord, lpnNumberOfAttrsWritten);
	}

	BOOL FillOutputCharacter
		( TCHAR cCharacter
		, DWORD nLength
		, COORD nWriteCoord
		, LPDWORD lpnNumberOfCharsWritten
		)
	{
		_ASSERTE(IsValid());

		return ::FillConsoleOutputCharacter(m_h, cCharacter, nLength
			, nWriteCoord, lpnNumberOfCharsWritten);
	}

	BOOL FillOutputAttribute
		( WORD wAttribute
		, DWORD nLength
		, COORD nWriteCoord
		, LPDWORD lpnNumberOfAttrsWritten
		)
	{
		_ASSERTE(IsValid());

		return ::FillConsoleOutputAttribute(m_h, wAttribute, nLength
			, nWriteCoord, lpnNumberOfAttrsWritten);
	}

	BOOL GetMode
		( LPDWORD lpnMode
		) const
	{
		_ASSERTE(lpnMode);
		_ASSERTE(IsValid());

		return ::GetConsoleMode(m_h, lpnMode);
	}

	DWORD GetMode() const
	{
		_ASSERTE(IsValid());
		DWORD nMode;

		if (::GetConsoleMode(m_h, &nMode))
			return nMode;
		return (DWORD)-1;
	}

	BOOL SetMode
		( DWORD nMode
		)
	{
		_ASSERTE(IsValid());

		return ::SetConsoleMode(m_h, nMode);
	}

	BOOL GetNumberOfInputEvents
		( LPDWORD lpnNumberOfEvents
		) const
	{
		_ASSERTE(lpnNumberOfEvents);
		_ASSERTE(IsValid());

		return ::GetNumberOfConsoleInputEvents(m_h, lpnNumberOfEvents);
	}

	DWORD GetNumberOfInputEvents() const
	{
		_ASSERTE(IsValid());
		DWORD nNumberOfEvents;

		if (::GetNumberOfConsoleInputEvents(m_h, &nNumberOfEvents))
			return nNumberOfEvents;
		return (DWORD)-1;
	}

	BOOL FlushInputBuffer()
	{
		_ASSERTE(IsValid());

		return ::FlushConsoleInputBuffer(m_h);
	}

	COORD GetLargestWindowSize() const
	{
		_ASSERTE(IsValid());

		return ::GetLargestConsoleWindowSize(m_h);
	}

	BOOL SetWindowInfo
		( BOOL bAbsolute
		, CONST SMALL_RECT *lpConsoleWindow
		)
	{
		_ASSERTE(lpConsoleWindow);
		_ASSERTE(IsValid());

		return ::SetConsoleWindowInfo(m_h, bAbsolute, lpConsoleWindow);
	}

	BOOL GetCursorInfo
		( PCONSOLE_CURSOR_INFO lpConsoleCursorInfo
		) const
	{
		_ASSERTE(lpConsoleCursorInfo);
		_ASSERTE(IsValid());

		return ::GetConsoleCursorInfo(m_h, lpConsoleCursorInfo);
	}

	BOOL SetCursorInfo
		( CONST CONSOLE_CURSOR_INFO *lpConsoleCursorInfo
		) const
	{
		_ASSERTE(lpConsoleCursorInfo);
		_ASSERTE(IsValid());

		return ::SetConsoleCursorInfo(m_h, lpConsoleCursorInfo);
	}

	BOOL SetCursorPosition
		( COORD nCursorPosition
		)
	{
		_ASSERTE(IsValid());

		return ::SetConsoleCursorPosition(m_h, nCursorPosition);
	}

	BOOL SetTextAttribute
		( WORD wAttributes
		)
	{
		_ASSERTE(IsValid());

		return ::SetConsoleTextAttribute(m_h, wAttributes);
	}

	BOOL Read
		( LPVOID lpBuffer
		, DWORD nNumberOfCharsToRead
		, LPDWORD lpnNumberOfCharsRead
	)
	{
		_ASSERTE(IsValid());

		return ::ReadConsole(m_h, lpBuffer, nNumberOfCharsToRead
			, lpnNumberOfCharsRead, NULL);
	}

	BOOL Write
		( CONST VOID *lpBuffer
		, DWORD nNumberOfCharsToWrite
		, LPDWORD lpNumberOfCharsWritten
		)
	{
		_ASSERTE(IsValid());

		return ::WriteConsole(m_h, lpBuffer, nNumberOfCharsToWrite
			, lpNumberOfCharsWritten, NULL);
	}


#if _WIN32_WINNT >= 0x0500
	BOOL GetCurrentFont
		( BOOL bMaximumWindow
		, PCONSOLE_FONT_INFO lpConsoleCurrentFont
		) const
	{
		_ASSERTE(lpConsoleCurrentFont);
		_ASSERTE(IsValid());

		return ::GetCurrentConsoleFont(m_h, bMaximumWindow
			, lpConsoleCurrentFont);
	}
	COORD GetFontSize
		( DWORD nFont
		) const
	{
		_ASSERTE(IsValid());

		return ::GetConsoleFontSize(m_h, nFont);
	}


#endif // _WIN32_WINNT >= 0x0500
};

typedef CConsoleT<true> CConsole;
typedef CConsoleT<false> CConsoleHandle;

//@//////////////////////////////////////////////////////////////////////////
// Completion Port

template<bool t_bManaged>
class CCompletionPortT
	: public CHandleT<t_bManaged>
{
	typedef CHandleT<t_bManaged> Base;
public:
	CCompletionPortT()
	{ }
	CCompletionPortT(const CCompletionPortT<true>& rCompletionPort)
		: Base(rCompletionPort)
	{ }
	CCompletionPortT(const CCompletionPortT<false>& rCompletionPort)
		: Base(rCompletionPort)
	{ }
	explicit CCompletionPortT(HANDLE hCompletionPort)
		: Base(hCompletionPort)
	{ }
	CCompletionPortT
		( HANDLE hFileHandle
		, HANDLE hExistingCompletionPort
		, ULONG_PTR pnCompletionKey = NULL
		, DWORD nNumberOfConcurrentThreads = 0
		)
	{
		Create(hFileHandle, hExistingCompletionPort, pnCompletionKey
			, nNumberOfConcurrentThreads);
		_ASSERTE(IsValid());
	}

	CCompletionPortT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=(h);
		return *this;
	}

	CCompletionPortT<t_bManaged>& operator=(const CCompletionPortT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CCompletionPortT<t_bManaged>& operator=(const CCompletionPortT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	HANDLE Create
		( HANDLE hFileHandle
		, HANDLE hExistingCompletionPort
		, ULONG_PTR pnCompletionKey = 0
		, DWORD nNumberOfConcurrentThreads = 0
		)
	{
		_ASSERTE(IsValidHandle(hFileHandle));
		_ASSERTE(!IsValid());

		m_h = ::CreateIoCompletionPort(hFileHandle, hExistingCompletionPort
			, pnCompletionKey, nNumberOfConcurrentThreads);

		return m_h;
	}

	BOOL GetStatus
		( LPDWORD lpnNumberOfBytes
		, PULONG_PTR ppnCompletionKey
		, LPOVERLAPPED *lplpOverlapped
		, DWORD nMilliseconds = INFINITE
		) const
	{
		_ASSERTE(lpnNumberOfBytes);
		_ASSERTE(ppnCompletionKey);
		_ASSERTE(lplpOverlapped);
		_ASSERTE(IsValid());

		return ::GetQueuedCompletionStatus(m_h, lpnNumberOfBytes
				, ppnCompletionKey, lplpOverlapped, nMilliseconds);
	}

	BOOL PostStatus
		( DWORD nNumberOfBytesTransferred
		, ULONG_PTR pnCompletionKey
		, LPOVERLAPPED lpOverlapped
		)
	{
		_ASSERTE(IsValid());

		return ::PostQueuedCompletionStatus(m_h, nNumberOfBytesTransferred
				, pnCompletionKey, lpOverlapped);
	}
};

typedef CCompletionPortT<true> CCompletionPort;
typedef CCompletionPortT<false> CCompletionPortHandle;

/////////////////////////////////////////////////////////////////////////////
// FileMapping

template<bool t_bManaged>
class CFileMappingT
	: public CHandleT<t_bManaged>
{
	typedef CHandleT<t_bManaged> Base;
public:
	CFileMappingT()
	{ }
	CFileMappingT
		( const CFileMappingT<true>& rFileMapping
		)
		: Base(rFileMapping)
	{ }
	CFileMappingT
		( const CFileMappingT<false>& rFileMapping
		)
		: Base(rFileMapping)
	{ }
	explicit CFileMappingT
		( HANDLE hFileMapping
		)
		: Base(hFileMapping)
	{ }

	CFileMappingT
		( HANDLE hFile
		, DWORD nProtect
		, ULONGLONG nMaximumSize = 0
		)
	{
		Create(hFile, nProtect, nMaximumSize);
		_ASSERTE(IsValid());
	}

	CFileMappingT
		( HANDLE hFile
		, DWORD nProtect
		, ULONGLONG nMaximumSize
		, LPCSTR lpName
		, LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL
		)
	{
		Create(hFile, nProtect, nMaximumSize, lpName
			, lpFileMappingAttributes);
		_ASSERTE(IsValid());
	}

	CFileMappingT
		( HANDLE hFile
		, DWORD nProtect
		, ULONGLONG nMaximumSize
		, LPCWSTR lpName
		, LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL
		)
	{
		Create(hFile, nProtect, nMaximumSize, lpName
			, lpFileMappingAttributes);
		_ASSERTE(IsValid());
	}

	CFileMappingT
		( LPCSTR lpName
		, DWORD nDesiredAccess
		, BOOL bInheritHandle = FALSE
		)
	{
		Open(lpName, nDesiredAccess, bInheritHandle);
		_ASSERTE(IsValid());
	}

	CFileMappingT
		( LPCWSTR lpName
		, DWORD nDesiredAccess
		, BOOL bInheritHandle = FALSE
		)
	{
		Open(lpName, nDesiredAccess, bInheritHandle);
		_ASSERTE(IsValid());
	}

	CFileMappingT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=(h);
		return *this;
	}

	CFileMappingT<t_bManaged>& operator=(const CFileMappingT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CFileMappingT<t_bManaged>& operator=(const CFileMappingT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	HANDLE Create
		( HANDLE hFile
		, DWORD nProtect
		, ULONGLONG nMaximumSize = 0
		)
	{
		_ASSERTE(!IsValid());
		ULARGE_INTEGER liMaximumSize;

		liMaximumSize.QuadPart = nMaximumSize;
		m_h = ::CreateFileMapping(hFile, NULL, nProtect
			, liMaximumSize.HighPart, liMaximumSize.LowPart, NULL);
		return m_h;
	}

	HANDLE Create
		( HANDLE hFile
		, DWORD nProtect
		, ULONGLONG nMaximumSize
		, LPCSTR lpName
		, LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL
		)
	{
		_ASSERTE(!IsValid());
		ULARGE_INTEGER liMaximumSize;

		liMaximumSize.QuadPart = nMaximumSize;
		m_h = ::CreateFileMappingA(hFile, lpFileMappingAttributes, nProtect
			, liMaximumSize.HighPart, liMaximumSize.LowPart, lpName);
		return m_h;
	}

	HANDLE Create
		( HANDLE hFile
		, DWORD nProtect
		, ULONGLONG nMaximumSize
		, LPCWSTR lpName
		, LPSECURITY_ATTRIBUTES lpFileMappingAttributes = NULL
		)
	{
		_ASSERTE(!IsValid());
		ULARGE_INTEGER liMaximumSize;

		liMaximumSize.QuadPart = nMaximumSize;
		m_h = ::CreateFileMappingW(hFile, lpFileMappingAttributes, nProtect
			, liMaximumSize.HighPart, liMaximumSize.LowPart, lpName);
		return m_h;
	}

	HANDLE Open
		( LPCSTR lpName
		, DWORD nDesiredAccess = FILE_MAP_ALL_ACCESS
		, BOOL bInheritHandle = FALSE
		)
	{
		_ASSERTE(!IsValid());

		m_h = ::OpenFileMappingA(nDesiredAccess, bInheritHandle, lpName);
		return m_h;
	}

	HANDLE Open
		( LPCWSTR lpName
		, DWORD nDesiredAccess = FILE_MAP_ALL_ACCESS
		, BOOL bInheritHandle = FALSE
		)
	{
		_ASSERTE(!IsValid());

		m_h = ::OpenFileMappingW(nDesiredAccess, bInheritHandle, lpName);
		return m_h;
	}

	LPVOID MapViewOfFile
		( DWORD nDesiredAccess
		, ULONGLONG nFileOffset
		, SIZE_T nNumberOfBytesToMap
		)
	{
		_ASSERTE(!IsValid());
		ULARGE_INTEGER liFileOffset;

		liFileOffset.QuadPart = nFileOffset;
		return ::MapViewOfFile(m_h, nDesiredAccess, liFileOffset.HighPart
			, liFileOffset.LowPart, nNumberOfBytesToMap);
	}

	LPVOID MapViewOfFileEx
		( DWORD nDesiredAccess
		, ULONGLONG nFileOffset
		, SIZE_T nNumberOfBytesToMap
		, LPVOID lpBaseAddress
		)
	{
		_ASSERTE(!IsValid());
		ULARGE_INTEGER liFileOffset;

		liFileOffset.QuadPart = nFileOffset;
		return ::MapViewOfFileEx(m_h, nDesiredAccess, liFileOffset.HighPart
			, liFileOffset.LowPart, nNumberOfBytesToMap, lpBaseAddress);
	}

	static BOOL UnmapViewOfFile
		( LPCVOID lpBaseAddress
		)
	{
		return ::UnmapViewOfFile(lpBaseAddress);
	}
};

typedef CFileMappingT<true> CFileMapping;
typedef CFileMappingT<false> CFileMappingHandle;

/////////////////////////////////////////////////////////////////////////////
// Named pipe

template<bool t_bManaged>
class CNamedPipeT
	: public CFileT<t_bManaged>
{
	typedef CFileT<t_bManaged> Base;
public:
#if _MSC_VER>=7000
	using RSDN::CFileT<t_bManaged>::Create;
	using RSDN::CFileT<t_bManaged>::GetInformation;
	using RSDN::CFileT<t_bManaged>::SetInformation;
#endif // _MSC_VER>=7000

	CNamedPipeT()
	{ }
	CNamedPipeT
		( const CNamedPipeT<true>& rPipe
		)
		: Base(rPipe)
	{ }
	CNamedPipeT
		( const CNamedPipeT<false>& rPipe
		)
		: Base(rPipe)
	{ }
	explicit CNamedPipeT
		( HANDLE hPipe
		)
		: Base((INVALID_HANDLE_VALUE != hPipe) ? hPipe : NULL)
	{ }

	CNamedPipeT
		( LPCSTR szFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szFilename, nDesiredAccess, nShareMode, nCreationDisposition
			, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CNamedPipeT
		( LPCWSTR szFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szFilename, nDesiredAccess, nShareMode, nCreationDisposition
			, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CNamedPipeT
		( LPCSTR lpName
		, DWORD nOpenMode
		, DWORD nPipeMode
		, DWORD nMaxInstances
		, DWORD nOutBufferSize
		, DWORD nInBufferSize
		, DWORD nDefaultTimeOut
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL
		)
	{
		Create(lpName, nOpenMode, nPipeMode, nMaxInstances, nOutBufferSize
			, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
		_ASSERTE(IsValid());
	}

	CNamedPipeT
		( LPCWSTR lpName
		, DWORD nOpenMode
		, DWORD nPipeMode
		, DWORD nMaxInstances
		, DWORD nOutBufferSize
		, DWORD nInBufferSize
		, DWORD nDefaultTimeOut
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL
		)
	{
		Create(lpName, nOpenMode, nPipeMode, nMaxInstances, nOutBufferSize
			, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
		_ASSERTE(IsValid());
	}

	CNamedPipeT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=((INVALID_HANDLE_VALUE != h) ? h : NULL);
		return *this;
	}

	CNamedPipeT<t_bManaged>& operator=(const CNamedPipeT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CNamedPipeT<t_bManaged>& operator=(const CNamedPipeT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	HANDLE Create
		( LPCSTR lpName
		, DWORD nOpenMode
		, DWORD nPipeMode
		, DWORD nMaxInstances
		, DWORD nOutBufferSize
		, DWORD nInBufferSize
		, DWORD nDefaultTimeOut = INFINITE
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL
		)
	{
		_ASSERTE(!IsValid());
		HANDLE h;

		h = ::CreateNamedPipeA(lpName, nOpenMode, nPipeMode, nMaxInstances
			, nOutBufferSize, nInBufferSize, nDefaultTimeOut
			, lpSecurityAttributes);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}

	HANDLE Create
		( LPCWSTR lpName
		, DWORD nOpenMode
		, DWORD nPipeMode
		, DWORD nMaxInstances
		, DWORD nOutBufferSize
		, DWORD nInBufferSize
		, DWORD nDefaultTimeOut = INFINITE
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL
		)
	{
		_ASSERTE(!IsValid());
		HANDLE h;

		h = ::CreateNamedPipeW(lpName, nOpenMode, nPipeMode, nMaxInstances
			, nOutBufferSize, nInBufferSize, nDefaultTimeOut
			, lpSecurityAttributes);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}

	BOOL Connect
		( LPOVERLAPPED lpOverlapped = NULL
		)
	{
		_ASSERTE(IsValid());

		return ::ConnectNamedPipe(m_h, lpOverlapped);
	}

	BOOL Disconnect()
	{
		_ASSERTE(IsValid());

		return ::DisconnectNamedPipe(m_h);
	}

	BOOL GetState
		( LPDWORD lpnState
		, LPDWORD lpnCurInstances = NULL
		, LPDWORD lpnMaxCollectionCount = NULL
		, LPDWORD lpnCollectDataTimeout = NULL
		, LPSTR lpUserName = NULL
		, DWORD nMaxUserNameSize = 0
		) const
	{
		_ASSERTE(IsValid());

		return ::GetNamedPipeHandleStateA(m_h, lpnState, lpnCurInstances
				, lpnMaxCollectionCount, lpnCollectDataTimeout
				, lpUserName, nMaxUserNameSize);
	}

	BOOL GetState
		( LPDWORD lpnState
		, LPDWORD lpnCurInstances = NULL
		, LPDWORD lpnMaxCollectionCount = NULL
		, LPDWORD lpnCollectDataTimeout = NULL
		, LPWSTR lpUserName = NULL
		, DWORD nMaxUserNameSize = 0
		) const
	{
		_ASSERTE(IsValid());

		return ::GetNamedPipeHandleStateW(m_h, lpnState, lpnCurInstances
				, lpnMaxCollectionCount, lpnCollectDataTimeout
				, lpUserName, nMaxUserNameSize);
	}

	BOOL SetState
		( LPDWORD lpnMode
		, LPDWORD lpnMaxCollectionCount = NULL
		, LPDWORD lpnCollectDataTimeout = NULL
		)
	{
		_ASSERTE(IsValid());

		return ::SetNamedPipeHandleState(m_h, lpnMode, lpnMaxCollectionCount
			, lpnCollectDataTimeout);
	}

	BOOL GetInformation
		( LPDWORD lpnFlags
		, LPDWORD lpnOutBufferSize
		, LPDWORD lpnInBufferSize
		, LPDWORD lpnMaxInstances
		) const
	{
		_ASSERTE(IsValid());

		return ::GetNamedPipeInfo(m_h, lpnFlags, lpnOutBufferSize
				, lpnInBufferSize, lpnMaxInstances);
	}

	BOOL Peek
		( LPVOID lpBuffer
		, DWORD nBufferSize
		, LPDWORD lpBytesRead = NULL
		, LPDWORD lpTotalBytesAvail = NULL
		, LPDWORD lpBytesLeftThisMessage = NULL
		)
	{
		_ASSERTE(IsValid());

		return ::PeekNamedPipe(m_h, lpBuffer, nBufferSize, lpBytesRead
				, lpTotalBytesAvail, lpBytesLeftThisMessage);
	}

	int Transact
		( LPVOID lpInBuffer
		, DWORD nInBufferSize
		, LPVOID lpOutBuffer
		, DWORD nOutBufferSize
		, LPDWORD lpnBytesRead = NULL
		, LPOVERLAPPED lpOverlapped = NULL
		)
	{
		_ASSERTE(IsValid());
		DWORD nBytesRead = 0;

		if (lpnBytesRead == NULL)
			lpnBytesRead = &nBytesRead;

		if (!::TransactNamedPipe(m_h, lpInBuffer, nInBufferSize
				, lpOutBuffer, nOutBufferSize, lpnBytesRead, lpOverlapped))
		{
			return FALSE;
		}
		if (*lpnBytesRead != nInBufferSize)
			return ERROR_HANDLE_EOF;
		return TRUE;
	}

	BOOL ImpersonateClient()
	{
		_ASSERTE(IsValid());

		return ::ImpersonateNamedPipeClient(m_h);
	}
};

typedef CNamedPipeT<true> CNamedPipe;
typedef CNamedPipeT<false> CNamedPipeHandle;

//@//////////////////////////////////////////////////////////////////////////
// Mail slot

template<bool t_bManaged>
class CMailslotT
	: public CFileT<t_bManaged>
{
	typedef CFileT<t_bManaged> Base;
public:
#if _MSC_VER>=7000
	using RSDN::CFileT<t_bManaged>::Create;
	using RSDN::CFileT<t_bManaged>::GetInformation;
	using RSDN::CFileT<t_bManaged>::SetInformation;
#endif // _MSC_VER>=7000

	CMailslotT()
	{ }
	CMailslotT
		( const CMailslotT<true>& rMailSlot
		)
		: Base(rMailSlot)
	{ }
	CMailslotT
		( const CMailslotT<false>& rMailSlot
		)
		: Base(rMailSlot)
	{ }
	explicit CMailslotT
		( HANDLE hMailSlot
		)
		: Base((INVALID_HANDLE_VALUE != hMailSlot) ? hMailSlot : NULL)
	{ }

	CMailslotT
		( LPCSTR lpName
		, DWORD nMaxMessageSize
		, DWORD nReadTimeout = INFINITE
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL
		)
	{
		Create(lpName, nMaxMessageSize, nReadTimeout, lpSecurityAttributes);
		_ASSERTE(IsValid());
	}

	CMailslotT
		( LPCWSTR lpName
		, DWORD nMaxMessageSize
		, DWORD nReadTimeout = INFINITE
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL
		)
	{
		Create(lpName, nMaxMessageSize, nReadTimeout, lpSecurityAttributes);
		_ASSERTE(IsValid());
	}

	CMailslotT
		( LPCSTR szFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szFilename, nDesiredAccess, nShareMode, nCreationDisposition
			, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CMailslotT
		( LPCWSTR szFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szFilename, nDesiredAccess, nShareMode, nCreationDisposition
			, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CMailslotT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=((INVALID_HANDLE_VALUE != h) ? h : NULL);
		return *this;
	}

	CMailslotT<t_bManaged>& operator=(const CMailslotT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CMailslotT<t_bManaged>& operator=(const CMailslotT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	HANDLE Create
		( LPCSTR lpName
		, DWORD nMaxMessageSize
		, DWORD nReadTimeout = INFINITE
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL
		)
	{
		_ASSERTE(!IsValid());
		HANDLE h;

		h = ::CreateMailslotA(lpName, nMaxMessageSize, nReadTimeout
			, lpSecurityAttributes);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}

	HANDLE Create
		( LPCWSTR lpName
		, DWORD nMaxMessageSize
		, DWORD nReadTimeout = INFINITE
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL
		)
	{
		_ASSERTE(!IsValid());
		HANDLE h;

		h = ::CreateMailslotW(lpName, nMaxMessageSize, nReadTimeout
			, lpSecurityAttributes);
		if (INVALID_HANDLE_VALUE == h)
			h = NULL;
		return m_h = h;
	}

	BOOL GetInformation
		( LPDWORD lpnMaxMessageSize
		, LPDWORD lpnNextSize
		, LPDWORD lpnMessageCount
		, LPDWORD lpnReadTimeout
		) const
	{
		_ASSERTE(IsValid());

		return ::GetMailslotInfo(m_h, lpnMaxMessageSize, lpnNextSize
				, lpnMessageCount, lpnReadTimeout);
	}

	BOOL SetInformation
		( DWORD nReadTimeout
		)
	{
		_ASSERTE(IsValid());

		return ::SetMailslotInfo(m_h, nReadTimeout);
	}
};

typedef CMailslotT<true> CMailslot;
typedef CMailslotT<false> CMailslotHandle;

//@//////////////////////////////////////////////////////////////////////////
// Device

template<bool t_bManaged>
class CDeviceT
	: public CFileT<t_bManaged>
{
	typedef CFileT<t_bManaged> Base;
public:
	CDeviceT()
	{ }
	CDeviceT(const CDeviceT<true>& rDevice)
		: Base(rDevice)
	{ }
	CDeviceT(const CDeviceT<false>& rDevice)
		: Base(rDevice)
	{ }
	explicit CDeviceT(HANDLE hDevice)
		: Base(hDevice)
	{ }
	CDeviceT
		( LPCSTR szDevicename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szDevicename, nDesiredAccess, nShareMode
			, nCreationDisposition, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CDeviceT
		( LPCWSTR szDevicename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szDevicename, nDesiredAccess, nShareMode
			, nCreationDisposition, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CDeviceT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=(h);
		return *this;
	}

	CDeviceT<t_bManaged>& operator=(const CDeviceT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CDeviceT<t_bManaged>& operator=(const CDeviceT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	BOOL IoControl
		( DWORD nIoControlCode
		, LPVOID lpInBuffer
		, DWORD nInBufferSize
		, LPVOID lpOutBuffer
		, DWORD nOutBufferSize
		, LPDWORD lpnBytesReturned = NULL
		, LPOVERLAPPED lpOverlapped = NULL
		)
	{
		_ASSERTE(IsValid());
		DWORD nBytesReturned = 0;

		if (lpnBytesReturned == NULL)
			lpnBytesReturned = &nBytesReturned;

		if (!::DeviceIoControl(m_h, nIoControlCode
				, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize
				, lpnBytesReturned, lpOverlapped))
		{
			return FALSE;
		}
		if (*lpnBytesReturned != nOutBufferSize)
			return ERROR_HANDLE_EOF;
		return TRUE;
	}

	BOOL RequestWakeup()
	{
		_ASSERTE(IsValid());

		return ::RequestDeviceWakeup(m_h);
	}

	BOOL CancelWakeupRequest()
	{
		_ASSERTE(IsValid());

		return ::CancelDeviceWakeupRequest(m_h);
	}

	BOOL GetPowerState
		( BOOL *pbOn
		) const
	{
		_ASSERTE(pbOn);
		_ASSERTE(IsValid());

		return ::GetDevicePowerState(m_h, pbOn);
	}

	int GetPowerState() const
	{
		_ASSERTE(IsValid());
		BOOL bOn;

		if (::GetDevicePowerState(m_h, &bOn))
			return !!bOn;
		return -1;
	}

	BOOL SetMessageWaitingIndicator
		( ULONG ulMsgCount
		) const
	{
		_ASSERTE(IsValid());

		return ::SetMessageWaitingIndicator(m_h, ulMsgCount);
	}
};

typedef CDeviceT<true> CDevice;
typedef CDeviceT<false> CDeviceHandle;

//@//////////////////////////////////////////////////////////////////////////
// Communication file

template<bool t_bManaged>
class CCommFileT
	: public CDeviceT<t_bManaged>
{
	typedef CDeviceT<t_bManaged> Base;
public:
	CCommFileT()
	{ }
	CCommFileT
		( const CCommFileT<true>& rCommFile
		)
		: Base(rCommFile)
	{ }
	CCommFileT
		( const CCommFileT<false>& rCommFile
		)
		: Base(rCommFile)
	{ }
	explicit CCommFileT
		( HANDLE hCommFile
		)
		: Base(hCommFile)
	{ }

	CCommFileT
		( LPCSTR szCommFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szCommFilename, nDesiredAccess, nShareMode
			, nCreationDisposition, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CCommFileT
		( LPCWSTR szCommFilename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szCommFilename, nDesiredAccess, nShareMode
			, nCreationDisposition, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CCommFileT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=(h);
		return *this;
	}

	CCommFileT<t_bManaged>& operator=(const CCommFileT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CCommFileT<t_bManaged>& operator=(const CCommFileT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	BOOL ClearBreak()
	{
		_ASSERTE(IsValid());

		return ::ClearCommBreak(m_h);
	}

	BOOL ClearError
		( LPDWORD lpErrors
		, LPCOMSTAT lpStat = NULL
		)
	{
		_ASSERTE(lpErrors);
		_ASSERTE(IsValid());

		return ::ClearCommError(m_h, lpErrors, lpStat);
	}

	BOOL EscapeFunction
		( DWORD nFunc
		)
	{
		_ASSERTE(IsValid());

		return ::EscapeCommFunction(m_h, nFunc);
	}

	BOOL GetConfig
		( LPCOMMCONFIG lpCC
		, LPDWORD lpnSize
		) const
	{
		_ASSERTE(lpCC);
		_ASSERTE(lpnSize);
		_ASSERTE(IsValid());

		return ::GetCommConfig(m_h, lpCC, lpnSize);
	}

	BOOL GetMask
		( LPDWORD lpnMask
		) const
	{
		_ASSERTE(lpnMask);
		_ASSERTE(IsValid());

		return ::GetCommMask(m_h, lpnMask);
	}

	BOOL GetModemStatus
		( LPDWORD lpnModemStatus
		) const
	{
		_ASSERTE(lpnModemStatus);
		_ASSERTE(IsValid());

		return ::GetCommModemStatus(m_h, lpnModemStatus);
	}

	BOOL GetProperties
		( LPCOMMPROP lpCommProp
		) const
	{
		_ASSERTE(lpCommProp);
		_ASSERTE(IsValid());

		return ::GetCommProperties(m_h, lpCommProp);
	}

	BOOL GetState
		( LPDCB lpDCB
		) const
	{
		_ASSERTE(lpDCB);
		_ASSERTE(IsValid());

		return ::GetCommState(m_h, lpDCB);
	}

	BOOL GetTimeouts
		( LPCOMMTIMEOUTS lpCommTimeouts
		) const
	{
		_ASSERTE(lpCommTimeouts);
		_ASSERTE(IsValid());

		return ::GetCommTimeouts(m_h, lpCommTimeouts);
	}

	BOOL Purge
		( DWORD nFlags
		)
	{
		_ASSERTE(IsValid());

		return ::PurgeComm(m_h, nFlags);
	}

	BOOL SetBreak()
	{
		_ASSERTE(IsValid());

		return ::SetCommBreak(m_h);
	}

	BOOL SetConfig
		( LPCOMMCONFIG lpCC
		, DWORD nSize
		)
	{
		_ASSERTE(lpCC);
		_ASSERTE(IsValid());

		return ::SetCommConfig(m_h, lpCC, nSize);
	}

	BOOL SetMask
		( DWORD nMask
		)
	{
		_ASSERTE(IsValid());

		return ::SetCommMask(m_h, nMask);
	}

	BOOL SetState
		( LPDCB lpDCB
		)
	{
		_ASSERTE(lpDCB);
		_ASSERTE(IsValid());

		return ::SetCommState(m_h, lpDCB);
	}

	BOOL SetTimeouts
		( LPCOMMTIMEOUTS lpCommTimeouts
		)
	{
		_ASSERTE(lpCommTimeouts);
		_ASSERTE(IsValid());

		return ::SetCommTimeouts(m_h, lpCommTimeouts);
	}

	BOOL Setup
		( DWORD nInQueue
		, DWORD nOutQueue 
		)
	{
		_ASSERTE(IsValid());

		return ::SetupComm(m_h, nInQueue, nOutQueue);
	}

	BOOL TransmitChar
		( char cChar
		)
	{
		_ASSERTE(IsValid());

		return ::TransmitCommChar(m_h, cChar);
	}

	BOOL WaitEvent
		( LPDWORD lpEvtMask
		, LPOVERLAPPED lpOverlapped = NULL
		)
	{
		_ASSERTE(lpEvtMask);
		_ASSERTE(IsValid());

		return ::WaitCommEvent(m_h, lpEvtMask, lpOverlapped);
	}
};

typedef CCommFileT<true> CCommFile;
typedef CCommFileT<false> CCommFileHandle;

//@//////////////////////////////////////////////////////////////////////////
// Tape device

template<bool t_bManaged>
class CTapeT
	: public CDeviceT<t_bManaged>
{
	typedef CDeviceT<t_bManaged> Base;
public:
#if _MSC_VER>=7000
	using RSDN::CDeviceT<t_bManaged>::GetPosition;
#endif // _MSC_VER>=7000

	CTapeT()
	{ }
	CTapeT
		( const CTapeT<true>& rTape
		)
		: Base(rTape)
	{ }
	CTapeT
		( const CTapeT<false>& rTape
		)
		: Base(rTape)
	{ }
	explicit CTapeT
		( HANDLE hTape
		)
		: Base(hTape)
	{ }

	CTapeT
		( LPCSTR szTapeDevicename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szTapeDevicename, nDesiredAccess, nShareMode
			, nCreationDisposition, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CTapeT
		( LPCWSTR szTapeDevicename
		, DWORD nDesiredAccess
		, DWORD nShareMode
		, DWORD nCreationDisposition
		, DWORD nFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL
		, LPSECURITY_ATTRIBUTES lpsa = NULL
		, HANDLE hTemplateFile = NULL
		)
	{
		Create(szTapeDevicename, nDesiredAccess, nShareMode
			, nCreationDisposition, nFlagsAndAttributes, lpsa, hTemplateFile);
		_ASSERTE(IsValid());
	}

	CTapeT<t_bManaged>& operator=(HANDLE h)
	{
		Base::operator=(h);
		return *this;
	}

	CTapeT<t_bManaged>& operator=(const CTapeT<true>& other)
	{
		Base::operator=(other);
		return *this;
	}

	CTapeT<t_bManaged>& operator=(const CTapeT<false>& other)
	{
		Base::operator=(other);
		return *this;
	}

	DWORD CreatePartition
		( DWORD nPartitionMethod
		, DWORD nCount
		, DWORD nSize
		)
	{
		_ASSERTE(TAPE_FIXED_PARTITIONS == nPartitionMethod
			|| TAPE_INITIATOR_PARTITIONS == nPartitionMethod
			|| TAPE_SELECT_PARTITIONS == nPartitionMethod);
		_ASSERTE(IsValid());

		return ::CreateTapePartition(m_h, nPartitionMethod, nCount, nSize);
	}

	DWORD Erase
		( DWORD nEraseType
		, BOOL bImmediate = FALSE
		)
	{
		_ASSERTE(TAPE_ERASE_LONG == nEraseType
			|| TAPE_ERASE_SHORT == nEraseType);
		_ASSERTE(IsValid());

		return ::EraseTape(m_h, nEraseType, bImmediate);
	}

	DWORD GetParameters
		( DWORD nOperation
		, LPDWORD lpnSize
		, LPVOID lpTapeInformation
		) const
	{
		_ASSERTE(GET_TAPE_MEDIA_INFORMATION == nOperation
			|| GET_TAPE_DRIVE_INFORMATION == nOperation);
		_ASSERTE(lpnSize);
		_ASSERTE(lpTapeInformation);
		_ASSERTE(IsValid());

		return ::GetTapeParameters(m_h, nOperation, lpnSize, lpTapeInformation);
	}

	DWORD GetPosition
		( DWORD nPositionType
		, LPDWORD lpnPartition
		, PULONGLONG pnOffset
		) const
	{
		_ASSERTE(TAPE_ABSOLUTE_POSITION == nPositionType
			|| TAPE_LOGICAL_POSITION == nPositionType);
		_ASSERTE(lpnPartition);
		_ASSERTE(pnOffset);
		_ASSERTE(IsValid());
		DWORD nRet;
		ULARGE_INTEGER liOffset;

		nRet = ::GetTapePosition(m_h, nPositionType, lpnPartition
			, &liOffset.LowPart, &liOffset.HighPart);
		if (NOERROR != nRet)
			liOffset.QuadPart = (ULONGLONG)-1;
		*pnOffset = liOffset.QuadPart;
		return nRet;
	}

	DWORD GetStatus() const
	{
		_ASSERTE(IsValid());

		return ::GetTapeStatus(m_h);
	}

	DWORD Prepare
		( DWORD nOperation
		, BOOL bImmediate = FALSE
		)
	{
		_ASSERTE(TAPE_FORMAT == nOperation || TAPE_LOAD == nOperation
			|| TAPE_LOCK == nOperation || TAPE_TENSION == nOperation
			|| TAPE_UNLOAD == nOperation || TAPE_UNLOCK == nOperation);
		_ASSERTE(IsValid());

		return ::PrepareTape(m_h, nOperation, bImmediate);
	}

	DWORD SetParameters
		( DWORD nOperation
		, LPVOID lpTapeInformation
		)
	{
		_ASSERTE(SET_TAPE_MEDIA_INFORMATION == nOperation
			|| SET_TAPE_DRIVE_INFORMATION == nOperation);
		_ASSERTE(lpTapeInformation);
		_ASSERTE(IsValid());

		return ::SetTapeParameters(m_h, nOperation, lpTapeInformation);
	}

	DWORD SetPosition
		( DWORD nPositionMethod
		, DWORD nPartition
		, ULONGLONG nOffset
		, BOOL bImmediate = FALSE
		)
	{
		_ASSERTE(nPositionMethod >= TAPE_REWIND
			&& nPositionMethod <= TAPE_SPACE_SEQUENTIAL_SMKS);
		_ASSERTE(IsValid());
		ULARGE_INTEGER liOffset;

		liOffset.QuadPart = nOffset;
		return ::SetTapePosition(m_h, nPositionMethod, nPartition
			, liOffset.LowPart, liOffset.HighPart, bImmediate);
	}

	DWORD WriteMark
		( DWORD nTapemarkType
		, DWORD nTapemarkCount
		, BOOL bImmediate = FALSE
		)
	{
		_ASSERTE(nTapemarkType >= TAPE_SETMARKS
			&& nTapemarkType <= TAPE_LONG_FILEMARKS);
		_ASSERTE(IsValid());

		return ::WriteTapemark(m_h, nTapemarkType, nTapemarkCount
			, bImmediate);
	}
};

typedef CTapeT<true> CTape;
typedef CTapeT<false> CTapeHandle;

/////////////////////////////////////////////////////////////////////////////

}	// namespace RSDN

/////////////////////////////////////////////////////////////////////////////

#endif	// #ifndef __RSDN_CFILE_H__

/////////////////////////////////////////////////////////////////////////////
// End Of File CFile.h

//	Project: RSDN
//	File:	 CHandle.h
//	Author:  Paul Bludov
//	Date:	 11/04/2001
//
//	Description:
//		C++ wrapper on Win32 HANDLE
//
//	Update History:
//		PB 10/22/2002 
//			+ Rsdn namespace
//			* Wait status instead of true/false as result of
//				 Wait() & WaitWithMessageLoop()
//		PB 11/04/2002
//			+ DuplicateTo(), Get/SetInformation()
//
//@//////////////////////////////////////////////////////////////////////////

#ifndef __RSDN_CHANDLE_H__
#define __RSDN_CHANDLE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CHandleT<t_bManaged>
//

namespace RSDN
{

/////////////////////////////////////////////////////////////////////////////

//@//////////////////////////////////////////////////////////////////////////
//	Helpers

static DWORD WaitForSingleObjectWithMessageLoop
	( HANDLE hEvent
	, DWORD nTimeout = INFINITE
	)
{

	// Wait for the event to be signalled, or for the
	// timeout (in MS) to expire.  allow SENT messages
	// to be processed while we wait
	DWORD	nWait;
	// Make the compiler happy (suppress warning C4701)
	DWORD	nStartTime = 0;

	// Set the waiting period.
	DWORD	nWaitTime = nTimeout;

	// Repost WM_QUIT if it was removed from the queue
#ifndef RSDN_CHANDLE_NO_WMQUITCHECK
	BOOL	bGotQuit = FALSE;
	int		nExitCode = 0;
#endif // RSDN_CHANDLE_NO_WMQUITCHECK

	// The timeout will eventually run down as we iterate
	// processing messages.  grab the start time so that
	// we can calculate elapsed times.
	if (INFINITE != nTimeout)
		nStartTime = ::GetTickCount();

	do
	{
		nWait = ::MsgWaitForMultipleObjects(1, &hEvent, FALSE, nWaitTime
			, QS_ALLINPUT);
		
		if ((WAIT_OBJECT_0 + 1) == nWait)
		{
			// A windows message has been arrived
			MSG msg;
			while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
#ifndef RSDN_CHANDLE_NO_WMQUITCHECK
				if (WM_QUIT != msg.message)
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				else
				{
					nExitCode = (int)msg.wParam;
					bGotQuit = TRUE;
				}
#else
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
#endif // RSDN_CHANDLE_NO_WMQUITCHECK

				if (WAIT_OBJECT_0 == ::WaitForSingleObject(hEvent, 0))
				{
					nWait = WAIT_OBJECT_0; // Event is now signaled
					break;
				}

				// If we have an explicit length of time to wait calculate
				// the next wake up point - which might be now.
				// If nTimeout is INFINITE, it stays INFINITE
				if (INFINITE != nTimeout)
				{
					DWORD nElapsed = ::GetTickCount() - nStartTime;

					if (nElapsed >= nTimeout)
					{
						nWait = WAIT_TIMEOUT; // Event was timed out
						break;
					}

					nWaitTime = nTimeout - nElapsed;
				}
			}
		}
	}while((WAIT_OBJECT_0 + 1) == nWait);

#ifndef RSDN_CHANDLE_NO_WMQUITCHECK
	if (bGotQuit)
		::PostQuitMessage(nExitCode);
#endif // RSDN_CHANDLE_NO_WMQUITCHECK

	return nWait;
}

inline bool IsValidHandle
	( HANDLE h
	)
{
#ifdef _DEBUG
	if (NULL != h)
	{
		DWORD nFlags, nError = ::GetLastError();
		// Win98 has no ::GetHandleInformation()
		_ASSERTE(::GetHandleInformation(h, &nFlags) ||
			ERROR_CALL_NOT_IMPLEMENTED == ::GetLastError());
		::SetLastError(nError);
	}
#endif
	return NULL != h;
}

// AddRef the handle in the current process
inline BOOL DuplicateHandle
	( HANDLE hSource
	, BOOL bInherit
	, LPHANDLE lpDest
	)
{
	_ASSERTE(IsValidHandle(hSource));
	_ASSERTE(lpDest);

	HANDLE hThisProcess = ::GetCurrentProcess();
	return ::DuplicateHandle(hThisProcess, hSource, hThisProcess, lpDest
		, DUPLICATE_SAME_ACCESS, bInherit, DUPLICATE_SAME_ACCESS);
}

/////////////////////////////////////////////////////////////////////////////

//@//////////////////////////////////////////////////////////////////////////
// CHandleT
// A HANDLE wrapper class

template<bool t_bManaged>
class CHandleT
{
public:
// Data members
	HANDLE m_h;

// Constructor/destructor/operators
	CHandleT()
		: m_h(NULL)
	{ }

	CHandleT
		( HANDLE h
		)
		: m_h(h)
	{
		_ASSERTE(IsValid());
	}

	CHandleT
		( const CHandleT<true>& rh
		)
		: m_h(NULL)
	{
		// Avoid using this method,
		// use Attach(other.Detach() || Duplicate(other) instead

		if (false == t_bManaged)
			m_h = rh.m_h;
		else if (rh.IsValid() && FALSE == DuplicateFrom(rh.m_h))
			m_h = NULL;
	}

	CHandleT
		( const CHandleT<false>& rh
		)
		: m_h(NULL)
	{
		// Avoid using this method,
		// use Attach(other.Detach() || Duplicate(other) instead

		if (false == t_bManaged)
			m_h = rh.m_h;
		else if (rh.IsValid() && FALSE == DuplicateFrom(rh.m_h))
			m_h = NULL;
	}

	~CHandleT()
	{
		if (t_bManaged)
			Close();
	}

	bool IsValid() const
	{
		return RSDN::IsValidHandle(m_h);
	}

	void Close()
	{
		// Close the handle
		if (IsValid())
		{
			BOOL b;
			b = ::CloseHandle(m_h);
			_ASSERTE(b);
		}
		m_h = NULL;
	}

	bool operator!() const
	{
		return !IsValid();
	}

	operator HANDLE() const
	{
		return m_h;
	}

	operator bool() const
	{
		return IsValid();
	}

	LPHANDLE operator&()
	{
		// The assert on operator& usually indicates a bug.	If this is really
		// what is needed, take the address of the m_h member explicitly

		_ASSERTE(!IsValid());
		return &m_h;
	}

	CHandleT<t_bManaged>& operator=(HANDLE h)
	{
		// Try m_Handle.Attach(other.Detach())
		// if you got an assertion here.

		_ASSERTE(!IsValid() || false == t_bManaged);
		m_h = h;
		return *this;
	}

	CHandleT<t_bManaged>& operator=(const CHandleT<true>& other)
	{
		// Avoid using this method,
		// use Attach(other.Detach() || Duplicate(other) instead

		_ASSERTE(!IsValid() || false == t_bManaged);
		if (false == t_bManaged)
			m_h = other.m_h;
		else if (!other.IsValid() || FALSE == DuplicateFrom(other.m_h))
			m_h = NULL;
		return *this;
	}

	CHandleT<t_bManaged>& operator=(const CHandleT<false>& other)
	{
		// Avoid using this method,
		// use Attach(other.Detach() || Duplicate(other) instead

		_ASSERTE(!IsValid() || false == t_bManaged);
		if (false == t_bManaged)
			m_h = other.m_h;
		else if (!other.IsValid() || FALSE == DuplicateFrom(other.m_h))
			m_h = NULL;
		return *this;
	}

	void Attach
		( HANDLE h
		)
	{
		// The main difference between operator= & Attach
		// that Attach releases the handle.

		if (t_bManaged && IsValid())
			::CloseHandle(m_h);
		m_h = h;
	}

	HANDLE Detach()
	{
		HANDLE h = m_h;
		m_h = NULL;
		return h;
	}

	BOOL DuplicateFrom
		( HANDLE hSource
		, BOOL bInherit = FALSE
		)
	{
		_ASSERTE(IsValidHandle(hSource));
		_ASSERTE(!IsValid());

		return RSDN::DuplicateHandle(hSource, bInherit, &m_h);
	}

	BOOL DuplicateFrom
		( HANDLE hSourceProcess
		, HANDLE hSource
		, HANDLE hDestProcess
		, DWORD nDesiredAccess = 0
		, BOOL bInherit = FALSE
		, DWORD nOptions = DUPLICATE_SAME_ACCESS
		)
	{
		_ASSERTE(IsValidHandle(hSource));
		_ASSERTE(!IsValid());

		return ::DuplicateHandle(hSourceProcess, hSource
				, hDestProcess, &m_h, nDesiredAccess, bInherit, nOptions);
	}

	BOOL DuplicateTo
		( LPHANDLE lphDest
		, BOOL bInherit = FALSE
		) const
	{
		_ASSERTE(lphDest);
		_ASSERTE(IsValid());

		return RSDN::DuplicateHandle(m_h, bInherit, lphDest);
	}

	BOOL DuplicateTo
		( HANDLE hSourceProcess
		, HANDLE hDestProcess
		, LPHANDLE lphDest
		, DWORD nDesiredAccess = 0
		, BOOL bInherit = FALSE
		, DWORD nOptions = DUPLICATE_SAME_ACCESS
		) const
	{
		_ASSERTE(IsValid());

		return ::DuplicateHandle(hSourceProcess, m_h, hDestProcess, lphDest
			, nDesiredAccess, bInherit, nOptions);
	}

	BOOL GetInformation
		( LPDWORD lpnFlags
		) const
	{
		_ASSERTE(lpnFlags);
		_ASSERTE(IsValid());

		return ::GetHandleInformation(m_h, lpnFlags);
	}

	BOOL SetInformation
		( DWORD nMask
		, DWORD nFlags
		)
	{
		_ASSERTE(0 == (nMask & 
			~(HANDLE_FLAG_INHERIT | HANDLE_FLAG_PROTECT_FROM_CLOSE)));
		_ASSERTE(0 == (nFlags & 
			~(HANDLE_FLAG_INHERIT | HANDLE_FLAG_PROTECT_FROM_CLOSE)));
		_ASSERTE(IsValid());

		return ::SetHandleInformation(m_h, nMask, nFlags);
	}

	DWORD Wait
		( DWORD nTimeout = INFINITE
		, BOOL bAlterable = FALSE
		) const
	{
		_ASSERTE(IsValid());

		return ::WaitForSingleObjectEx(m_h, nTimeout, bAlterable);
	}

	DWORD MsgWait
		( DWORD nTimeout = INFINITE
		, DWORD nWakeMask = QS_ALLEVENTS
		, DWORD nFlags = 0
		) const
	{
		_ASSERTE(IsValid());

		return ::MsgWaitForMultipleObjectsEx(1, &m_h, nTimeout, nWakeMask
			, nFlags);
	}

	DWORD WaitWithMessageLoop
		( DWORD nTimeout = INFINITE
		) const
	{
		_ASSERTE(IsValid());

		return RSDN::WaitForSingleObjectWithMessageLoop(m_h, nTimeout);
	}

#if(_WIN32_WINNT >= 0x0400)

	DWORD SignalAndWait
		( HANDLE hObjectToSignal
		, DWORD nMilliseconds = INFINITE
		, BOOL bAlertable = FALSE
		)
	{
		_ASSERTE(IsValidHandle(hObjectToSignal));
		_ASSERTE(IsValid());

		return ::SignalObjectAndWait(hObjectToSignal, m_h, nMilliseconds
			, bAlertable);
	}

#endif // (_WIN32_WINNT >= 0x0400)

#if (_WIN32_WINNT >= 0x0500)

	BOOL RegisterWait
		( PDWORD_PTR pnCookie
		, WAITORTIMERCALLBACK pfnCallback
		, PVOID pContext = NULL
		, ULONG nMilliseconds = INFINITE
		, ULONG nFlags = WT_EXECUTEDEFAULT
		)
	{
		_ASSERTE(IsValid());
		
		return ::RegisterWaitForSingleObject((PHANDLE)pnCookie, m_h
			, pfnCallback, pContext, nMilliseconds, nFlags);
	}

	static BOOL UnregisterWait(DWORD_PTR nCookie)
	{
		// nCookie should be from RegisterWait()
		return ::UnregisterWait((HANDLE)nCookie);
	}
	
	static BOOL UnregisterWait
		( DWORD_PTR nCookie
		, HANDLE hCompletionEvent
		)
	{
		// nCookie should be from RegisterWait()
		return ::UnregisterWaitEx((HANDLE)nCookie, hCompletionEvent);
	}
#endif // (_WIN32_WINNT >= 0x0500)

	BOOL GetKernelObjectSecurity
		( SECURITY_INFORMATION RequestedInformation
		, PSECURITY_DESCRIPTOR pSecurityDescriptor
		, DWORD nLength
		, LPDWORD lpnLengthNeeded
		) const
	{
		_ASSERTE(IsValid());

		return ::GetKernelObjectSecurity(m_h, RequestedInformation
			, pSecurityDescriptor, nLength, lpnLengthNeeded);
	}

	BOOL SetKernelObjectSecurity
		( SECURITY_INFORMATION SecurityInformation
		, PSECURITY_DESCRIPTOR pSecurityDescriptor
		)
	{
		_ASSERTE(IsValid());

		return ::SetKernelObjectSecurity(m_h, SecurityInformation
			, pSecurityDescriptor);
	}
};

typedef CHandleT<true> CHandle;
typedef CHandleT<false> CHandleHandle;

/////////////////////////////////////////////////////////////////////////////

}	// namespace RSDN

/////////////////////////////////////////////////////////////////////////////

#endif	// #ifndef __RSDN_CHANDLE_H__

/////////////////////////////////////////////////////////////////////////////
//	End Of File CHandle.h

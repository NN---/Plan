// Event.h
//

#ifndef __EVENT_H
#define __EVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Defenitions
// Number of Events
#ifndef NUM_EVENTS
#define NUM_EVENTS		3
#endif

// Events IDs
#define EVENT_ID_FIRST	IDD_MESSAGE
#define EVENT_ID(i)		(EVENT_ID_FIRST+i)

// Events
#define EVENT_MESSAGE		0
#define EVENT_SHUTDOWN		1
#define EVENT_RUNPROGRAM	2

// Events strings
#define STR_EVENT_MESSAGE		IDS_MESSAGE
#define STR_EVENT_SHUTDOWN		IDS_SHUTDOWN
#define STR_EVENT_RUNPROGRAM	IDS_RUNPROGRAM
#define STR_EVENT_FIRST			STR_EVENT_MESSAGE
#define STR_EVENT(i)			(STR_EVENT_FIRST+i)

// Classes
class Event;			// Base class for events
	class Message;		// Message event
	class ShutDown;		// ShutDown event
	class RunProgram;	// Run Program event

// Event - Base class for events
class Event
{
	// Attributes
protected:
	BYTE m_Type;
	CTime m_Time;
	BYTE m_State;
	// Construction
public:
	Event();
	Event(const BYTE tType,const CTime &rTime=CTime(),const BYTE tState=0);
	Event(const CTime &rTime,const BYTE tType=0,const BYTE tState=0);
	Event(const Event &rEvent);
	// Functions
protected:
	Event &GetEvent();
	const Event GetEvent() const;
public:
	BYTE &Type();
	const BYTE Type() const;
	CTime &Time();
	const CTime Time() const;
	BYTE &State();
	const BYTE State() const;
	// Operators
	operator CTime&();
	operator BYTE&();
	Event operator=(const Event &rEvent);
	Event operator=(const CTime &rTime);
	Event operator=(const BYTE tType);
	const BOOL operator==(const Event &rEvent) const;
	const BOOL operator==(const CTime &rTime) const;
	const BOOL operator==(const BYTE tType) const;
	const BOOL operator!=(const Event &rEvent) const;
	const BOOL operator!=(const CTime &rTime) const;
	const BOOL operator!=(const BYTE tType) const;
	const BOOL operator<(const Event &rEvent) const;
	const BOOL operator<(const CTime &rTime) const;
	const BOOL operator<(const BYTE tType) const;
	const BOOL operator<=(const Event &rEvent) const;
	const BOOL operator<=(const CTime &rTime) const;
	const BOOL operator<=(const BYTE tType) const;
	const BOOL operator>(const Event &rEvent) const;
	const BOOL operator>(const CTime &rTime) const;
	const BOOL operator>(const BYTE tType) const;
	const BOOL operator>=(const Event &rEvent) const;
	const BOOL operator>=(const CTime &rTime) const;
	const BOOL operator>=(const BYTE tType) const;
	const CTime operator-(const CTimeSpan &rTimeSpan) const;
	const CTimeSpan operator-(const Event &rEvent) const;
	const CTimeSpan operator-(const CTime &rTime) const;
	const Event operator-=(const CTimeSpan &rTimeSpan);
	const CTime operator+(const CTimeSpan &rTimeSpan) const;
	const Event operator+=(const CTimeSpan &rTimeSpan);
};

// Message - Message event
#define MESSAGE_TEXT_CHARS 150
#define MESSAGE_CAPTION_CHARS 50
class Message : public Event
{
	// Attributes
protected:
	TCHAR m_Caption[MESSAGE_CAPTION_CHARS];
	TCHAR m_Text[MESSAGE_TEXT_CHARS];
	// Construction
public:
	Message(const Event &rEvent);
	Message(const Message &rMessage);
	Message(LPCTSTR Caption=NULL,LPCTSTR Text=NULL,const CTime &rTime=CTime());
	// Functions
protected:
	Message &GetMessage();
	const Message GetMessage() const;
public:
	TCHAR *Caption();
	const TCHAR *Caption() const;
	TCHAR *Text();
	const TCHAR *Text() const;
	// Operators
	operator Event&();
	Message operator=(const Message &rMessage);
	Message operator=(const Event &rEvent);
	Message operator=(const CTime &rTime);
	const BOOL operator==(const Message &rMessage) const;
	const BOOL operator!=(const Message &rMessage) const;
	const BOOL operator<(const Message &rMessage) const;
	const BOOL operator<=(const Message &rMessage) const;
	const BOOL operator>(const Message &rMessage) const;
	const BOOL operator>=(const Message &rMessage) const;
	const CTimeSpan operator-(const Message &rMessage) const;
};

// ShutDown - ShutDown event
class ShutDown : public Event
{
	// Attributes
public:
	enum {opShutDown, opRestart, opLogOff};
protected:
	BYTE m_Operation;
	// Construction
public:
	ShutDown(const Event &rEvent);
	ShutDown(const ShutDown &rShutDown);
	ShutDown(const BYTE Operation=ShutDown::opShutDown,const CTime &rTime=CTime());
	// Functions
protected:
	ShutDown &GetShutDown();
	const ShutDown GetShutDown() const;
public:
	BYTE &Operation();
	const BYTE Operation() const;
	// Operators
	operator Event&();
	ShutDown operator=(const ShutDown &rShutDown);
	ShutDown operator=(const Event &rEvent);
	ShutDown operator=(const CTime &rTime);
	const BOOL operator==(const ShutDown &rShutDown) const;
	const BOOL operator!=(const ShutDown &rShutDown) const;
	const BOOL operator<(const ShutDown &rShutDown) const;
	const BOOL operator<=(const ShutDown &rShutDown) const;
	const BOOL operator>(const ShutDown &rShutDown) const;
	const BOOL operator>=(const ShutDown &rShutDown) const;
	const CTimeSpan operator-(const ShutDown &rShutDown) const;	
};

// RunProgram - Run Program event
#define RUNPROGRAM_PATH_CHARS		_MAX_PATH
#define RUNPROGRAM_PARAMETERS_CHARS	50
class RunProgram : public Event
{
	// Attributes
protected:
	TCHAR m_Path[RUNPROGRAM_PATH_CHARS];
	TCHAR m_Parameters[RUNPROGRAM_PARAMETERS_CHARS];
	// Construction
public:
	RunProgram(const Event &rEvent);
	RunProgram(const RunProgram &rRunProgram);
	RunProgram(LPCTSTR Path=NULL,LPCTSTR Parameters=NULL,const CTime &rTime=CTime());
	// Functions
protected:
	RunProgram &GetRunProgram();
	const RunProgram GetRunProgram() const;
public:
	TCHAR *Path();
	const TCHAR *Path() const;
	TCHAR *Parameters();
	const TCHAR *Parameters() const;
	// Operators
	operator Event&();
	RunProgram operator=(const RunProgram &rRunProgram);
	RunProgram operator=(const Event &rEvent);
	RunProgram operator=(const CTime &rTime);
	const BOOL operator==(const RunProgram &rRunProgram) const;
	const BOOL operator!=(const RunProgram &rRunProgram) const;
	const BOOL operator<(const RunProgram &rRunProgram) const;
	const BOOL operator<=(const RunProgram &rRunProgram) const;
	const BOOL operator>(const RunProgram &rRunProgram) const;
	const BOOL operator>=(const RunProgram &rRunProgram) const;
	const CTimeSpan operator-(const RunProgram &rRunProgram) const;
};

#endif // __EVENT_H
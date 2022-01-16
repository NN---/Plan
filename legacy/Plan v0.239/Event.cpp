// Event.cpp
//

#include "stdafx.h"
#include "Event.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Operators
Event::operator CTime&()
{return m_Time;}

Event::operator BYTE&()
{return m_Type;}

Message::operator Event&()
{return (GetEvent());}

ShutDown::operator Event&()
{return (GetEvent());}

RunProgram::operator Event&()
{return (GetEvent());}

// -----------------------------------
// Event class - Base class for events
// -----------------------------------

// Construction
Event::Event()
{ }

Event::Event(const BYTE tType,const CTime &rTime/*=CTime()*/,const BYTE tState/*=0*/)
{m_Type=tType;m_Time=rTime;m_State=tState;}

Event::Event(const CTime &rTime,const BYTE tType/*=0*/,const BYTE tState/*=0*/)
{m_Type=tType;m_Time=rTime;m_State=tState;}

Event::Event(const Event &rEvent)
{m_Type=rEvent.m_Type;m_Time=rEvent.m_Time;m_State=rEvent.m_State;}

// Functions
inline Event &Event::GetEvent()
{return *this;}

inline const Event Event::GetEvent() const
{return *this;}

BYTE &Event::Type()
{return m_Type;}

const BYTE Event::Type() const
{return m_Type;}

CTime &Event::Time()
{return m_Time;}

const CTime Event::Time() const
{return m_Time;}

BYTE &Event::State()
{return m_State;}

const BYTE Event::State() const
{return m_State;}

// Operators
Event Event::operator=(const Event &rEvent)
{m_Type=rEvent.m_Type;m_Time=rEvent.m_Time;m_State=rEvent.m_State;return *this;}

Event Event::operator=(const BYTE tType)
{m_Type=tType;return *this;}

Event Event::operator=(const CTime &rTime)
{m_Time=rTime;return *this;}

const BOOL Event::operator==(const Event &rEvent) const
{return (m_Type==rEvent.m_Type&&m_Time==rEvent.m_Time);}

const BOOL Event::operator==(const CTime &rTime) const
{return (m_Time==rTime);}

const BOOL Event::operator==(const BYTE tType) const
{return (m_Type==tType);}

const BOOL Event::operator!=(const Event &rEvent) const
{return (m_Type!=rEvent.m_Type&&m_Time!=rEvent.m_Time);}

const BOOL Event::operator!=(const CTime &rTime) const
{return (m_Time!=rTime);}

const BOOL Event::operator!=(const BYTE tType) const
{return (m_Type!=tType);}

const BOOL Event::operator<(const Event &rEvent) const
{return (m_Time<rEvent.m_Time);}

const BOOL Event::operator<(const CTime &rTime) const
{return (m_Time<rTime);}

const BOOL Event::operator<(const BYTE tType) const
{return (m_Type<tType);}

const BOOL Event::operator<=(const Event &rEvent) const
{return (m_Time<=rEvent.m_Time);}

const BOOL Event::operator<=(const CTime &rTime) const
{return (m_Time<=rTime);}

const BOOL Event::operator<=(const BYTE tType) const
{return (m_Type<=tType);}

const BOOL Event::operator>(const Event &rEvent) const
{return (m_Time>rEvent.m_Time);}

const BOOL Event::operator>(const CTime &rTime) const
{return (m_Time>rTime);}

const BOOL Event::operator>=(const Event &rEvent) const
{return (m_Time>=rEvent.m_Time);}

const BOOL Event::operator>(const BYTE tType) const
{return (m_Type>tType);}

const BOOL Event::operator>=(const CTime &rTime) const
{return (m_Time>=rTime);}

const BOOL Event::operator>=(const BYTE tType) const
{return (m_Type>=tType);}

const CTime Event::operator-(const CTimeSpan &rTimeSpan) const 
{return (m_Time-rTimeSpan);}

const CTimeSpan Event::operator-(const Event &rEvent) const
{return (m_Time-rEvent.m_Time);}

const CTimeSpan Event::operator-(const CTime &rTime) const
{return (m_Time-rTime);}

const Event Event::operator-=(const CTimeSpan &rTimeSpan)
{m_Time-=rTimeSpan;return *this;}

const CTime Event::operator+(const CTimeSpan &rTimeSpan) const
{return (m_Time+rTimeSpan);}

const Event Event::operator+=(const CTimeSpan &rTimeSpan)
{m_Time+=rTimeSpan;return *this;}

// -------------
// Message class
// -------------

// Construction
Message::Message(const Event &rEvent) : Event(rEvent)
{GetEvent()=EVENT_MESSAGE;m_Caption[0]=0;m_Text[0]=0;}

Message::Message(const Message &rMessage) : Event(rMessage)
{
	GetEvent()=EVENT_MESSAGE;
	
	if(rMessage.m_Caption!=NULL)
		lstrcpy(m_Caption,rMessage.m_Caption);
	else
		m_Caption[0]=0;

	if(rMessage.m_Text!=NULL)
		lstrcpy(m_Text,rMessage.m_Text);
	else
		m_Text[0]=0;
}

Message::Message(LPCTSTR Caption/*=NULL*/,LPCTSTR Text/*=NULL*/,const CTime &rTime/*=CTime()*/) : Event(rTime)
{
	GetEvent()=EVENT_MESSAGE;
	
	if(Caption!=NULL)
		lstrcpy(m_Caption,Caption);
	else
		m_Caption[0]=0;

	if(Text!=NULL)
		lstrcpy(m_Text,Text);
	else
		m_Text[0]=0;
}

// Functions
inline Message &Message::GetMessage()
{return *this;}

inline const Message Message::GetMessage() const
{return *this;}

TCHAR *Message::Caption()
{return m_Caption;}

const TCHAR *Message::Caption() const
{return m_Caption;}

TCHAR *Message::Text()
{return m_Text;}

const TCHAR *Message::Text() const
{return m_Text;}

// Operators
Message Message::operator=(const Message &rMessage)
{
	Event::operator=(rMessage);
	lstrcpy(m_Caption,rMessage.m_Caption);
	lstrcpy(m_Text,rMessage.m_Text);
	return *this;
}

Message Message::operator=(const Event &rEvent)
{
	Event::operator=(rEvent);
	return *this;
}

Message Message::operator=(const CTime &rTime)
{
	Event::operator=(rTime);
	return *this;
}

const BOOL Message::operator==(const Message &rMessage) const
{
	return (Event::operator==(rMessage)&&
		!lstrcmp(m_Caption,rMessage.m_Caption)&&
		!lstrcmp(m_Text,rMessage.m_Text));
}

const BOOL Message::operator!=(const Message &rMessage) const 
{
	return (Event::operator!=(rMessage)&&
		lstrcmp(m_Caption,rMessage.m_Caption)&&
		lstrcmp(m_Text,rMessage.m_Text));
}

const BOOL Message::operator<(const Message &rMessage) const
{return (Event::operator<(rMessage));}

const BOOL Message::operator<=(const Message &rMessage) const
{return (Event::operator<=(rMessage));}

const BOOL Message::operator>(const Message &rMessage) const
{return (Event::operator>(rMessage));}

const BOOL Message::operator>=(const Message &rMessage) const
{return (Event::operator>=(rMessage));}

const CTimeSpan Message::operator-(const Message &rMessage) const
{return (Event::operator-(rMessage));}

// --------------
// ShutDown class
// --------------

// Construction
ShutDown::ShutDown(const Event &rEvent) : Event(rEvent)
{GetEvent()=EVENT_SHUTDOWN;}

ShutDown::ShutDown(const ShutDown &rShutDown) : Event(rShutDown)
{GetEvent()=EVENT_SHUTDOWN;m_Operation=rShutDown.m_Operation;}

ShutDown::ShutDown(const BYTE Operation/*=ShutDown::opShutDown*/,const CTime &rTime/*=CTime()*/) : Event(rTime)
{
	GetEvent()=EVENT_SHUTDOWN;
	m_Operation=Operation;
}

// Functions
inline ShutDown &ShutDown::GetShutDown()
{return *this;}

inline const ShutDown ShutDown::GetShutDown() const
{return *this;}

BYTE &ShutDown::Operation()
{return m_Operation;}

const BYTE ShutDown::Operation() const
{return m_Operation;}

// Operators
ShutDown ShutDown::operator=(const ShutDown &rShutDown)
{
	Event::operator=(rShutDown);
	m_Operation=rShutDown.m_Operation;
	return *this;
}

ShutDown ShutDown::operator=(const Event &rEvent)
{
	Event::operator=(rEvent);
	return *this;
}

ShutDown ShutDown::operator=(const CTime &rTime)
{
	Event::operator=(rTime);
	return *this;
}

const BOOL ShutDown::operator==(const ShutDown &rShutDown) const
{return (Event::operator==(rShutDown)&&m_Operation==rShutDown.m_Operation);}

const BOOL ShutDown::operator!=(const ShutDown &rShutDown) const
{return (Event::operator!=(rShutDown)&&m_Operation!=rShutDown.m_Operation);}

const BOOL ShutDown::operator<(const ShutDown &rShutDown) const
{return (Event::operator<(rShutDown));}

const BOOL ShutDown::operator<=(const ShutDown &rShutDown) const
{return (Event::operator<=(rShutDown));}

const BOOL ShutDown::operator>(const ShutDown &rShutDown) const
{return (Event::operator>(rShutDown));}

const BOOL ShutDown::operator>=(const ShutDown &rShutDown) const
{return (Event::operator>=(rShutDown));}

const CTimeSpan ShutDown::operator-(const ShutDown &rShutDown) const
{return (Event::operator-(rShutDown));}

// ----------------
// RunProgram class
// ----------------

// Construction
RunProgram::RunProgram(const Event &rEvent) : Event(rEvent)
{GetEvent()=EVENT_RUNPROGRAM;m_Path[0]=0;m_Parameters[0]=0;}

RunProgram::RunProgram(const RunProgram &rRunProgram) : Event(rRunProgram)
{
	GetEvent()=EVENT_RUNPROGRAM;
	
	if(rRunProgram.m_Path!=NULL)
		lstrcpy(m_Path,rRunProgram.m_Path);
	else
		m_Path[0]=0;

	if(rRunProgram.m_Parameters!=NULL)
		lstrcpy(m_Parameters,rRunProgram.m_Parameters);
	else
		m_Parameters[0]=0;
}

RunProgram::RunProgram(LPCTSTR Path/*=NULL*/,LPCTSTR Parameters/*=NULL*/,const CTime &rTime/*=CTime()*/) : Event(rTime)
{
	GetEvent()=EVENT_RUNPROGRAM;
	
	if(Path!=NULL)
		lstrcpy(m_Path,Path);
	else
		m_Path[0]=0;

	if(Parameters!=NULL)
		lstrcpy(m_Parameters,Parameters);
	else
		m_Parameters[0]=0;
}

// Functions
inline RunProgram &RunProgram::GetRunProgram()
{return *this;}

inline const RunProgram RunProgram::GetRunProgram() const
{return *this;}

TCHAR *RunProgram::Path()
{return m_Path;}

const TCHAR *RunProgram::Path() const
{return m_Path;}

TCHAR *RunProgram::Parameters()
{return m_Parameters;}

const TCHAR *RunProgram::Parameters() const
{return m_Parameters;}

// Operators
RunProgram RunProgram::operator=(const RunProgram &rRunProgram)
{
	Event::operator=(rRunProgram);
	lstrcpy(m_Path,rRunProgram.m_Path);
	lstrcpy(m_Parameters,rRunProgram.m_Parameters);
	return *this;
}

RunProgram RunProgram::operator=(const Event &rEvent)
{
	Event::operator=(rEvent);
	return *this;
}

RunProgram RunProgram::operator=(const CTime &rTime)
{
	Event::operator=(rTime);
	return *this;
}

const BOOL RunProgram::operator==(const RunProgram &rRunProgram) const
{
	return (Event::operator==(rRunProgram)&&
		!lstrcmp(m_Path,rRunProgram.m_Path)&&
		!lstrcmp(m_Parameters,rRunProgram.m_Parameters));
}

const BOOL RunProgram::operator!=(const RunProgram &rRunProgram) const
{
	return (Event::operator!=(rRunProgram)&&
		!lstrcmp(m_Path,rRunProgram.m_Path)&&
		!lstrcmp(m_Parameters,rRunProgram.m_Parameters));
}

const BOOL RunProgram::operator<(const RunProgram &rRunProgram) const
{return (Event::operator<(rRunProgram));}

const BOOL RunProgram::operator<=(const RunProgram &rRunProgram) const
{return (Event::operator<=(rRunProgram));}

const BOOL RunProgram::operator>(const RunProgram &rRunProgram) const
{return (Event::operator>(rRunProgram));}

const BOOL RunProgram::operator>=(const RunProgram &rRunProgram) const
{return (Event::operator>=(rRunProgram));}

const CTimeSpan RunProgram::operator-(const RunProgram &rRunProgram) const
{return (Event::operator-(rRunProgram));}
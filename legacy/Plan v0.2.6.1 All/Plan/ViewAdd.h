#ifndef __VIEWADD_H__
#define __VIEWADD_H__

#include "DocView.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ATL_NO_VTABLE CViewAdd : public CView
{
public:
	struct Message
	{
		enum Types
		{
			Init,
			AddEvent,
			UpdateEvent,
			DeleteEvent,
			GetSelectedEvent
		};
		Types Type;
		int iEvent;
	};
	virtual int Init(Message *pMessage) = 0;
	virtual int AddEvent(Message *pMessage) = 0;
	virtual int UpdateEvent(Message *pMessage) = 0;
	virtual int DeleteEvent(Message *pMessage) = 0;
	virtual int GetSelectedEvent(Message *pMessage=NULL) = 0;
	static int (CViewAdd::*VTable[])(Message *);
};

int (CViewAdd::*VTable[])(CViewAdd::Message *)=
{
	&CViewAdd::Init,
	&CViewAdd::AddEvent,
	&CViewAdd::UpdateEvent,
	&CViewAdd::DeleteEvent,
	&CViewAdd::GetSelectedEvent
};

#endif // __VIEWADD_H__

#ifndef __VIEWMESSAGE_H__
#define __VIEWMESSAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ViewMessage
{
	enum Types
	{
		AddEvent,
		UpdateEvent,
		DeleteEvent,
		Init,
		GetSelectedEvent,
	};
	Types Type;
	int iEvent;
};

#endif // __VIEWMESSAGE_H__
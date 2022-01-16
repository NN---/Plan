#ifndef __EVENTBAR_H__
#define __EVENTBAR_H__

#pragma once

class CEventBar : public CDialogImpl<CEventBar>
{
	typedef CEventBar thisClass;
	typedef CDialogImpl<thisClass> baseClass;
public:
	class Data
	{};
	enum { IDD = IDD_EVENTBAR };

	CEventBar();

	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
	END_MSG_MAP()
};

#endif // __EVENTBAR_H__
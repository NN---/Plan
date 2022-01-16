#ifndef __EVENTBAR_H__
#define __EVENTBAR_H__

#pragma once

class CEventBar : public CDialogImpl<CEventBar>
{
	typedef CEventBar thisClass;
	//typedef CDialogImpl<thisClass> baseClass;
public:
	enum { IDD = IDD_EVENTBAR };

	// Message Map
	BEGIN_MSG_MAP(thisClass)
		// Message
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif // __EVENTBAR_H__
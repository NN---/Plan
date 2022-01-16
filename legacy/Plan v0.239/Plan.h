// Plan.h : main header file for the PLAN application
//

#if !defined(AFX_PLAN_H__A3697886_82B8_11D6_8201_D823D40173C3__INCLUDED_)
#define AFX_PLAN_H__A3697886_82B8_11D6_8201_D823D40173C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Defines.h"
#include "Event.h"

/////////////////////////////////////////////////////////////////////////////
// CPlanApp:
// See Plan.cpp for the implementation of this class
//

class CPlanApp : public CWinApp
{
public:
	// Options
	struct tagOptions
	{
		signed LoadOnStartUp:1;
		signed Add:1;
		signed Update:1;
		signed Remove:1;
		signed Exit:1;
		signed Timer:1;
		signed Check:1;
		signed BackupFile:1;
	}m_Options;
	
	// Functions
	CPlanApp();

	void SaveOptions();
	void LoadOptions();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlanApp)
	public:
	virtual BOOL InitInstance();
	virtual void ParseCommandLine(CCommandLineInfo& rCmdInfo);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL


// Implementation
	//{{AFX_MSG(CPlanApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAN_H__A3697886_82B8_11D6_8201_D823D40173C3__INCLUDED_)

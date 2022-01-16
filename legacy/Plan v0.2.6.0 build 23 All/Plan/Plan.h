// Plan.h : main header file for the PLAN application
//

#if !defined(AFX_PLAN_H__4CE9C7E3_D9FC_4BDA_B409_3BECF194F784__INCLUDED_)
#define AFX_PLAN_H__4CE9C7E3_D9FC_4BDA_B409_3BECF194F784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Defines.h"		// Defines
#include "RegKey.h"			// RegKey class
#define EVENT_IMPORT_DEF_LIB
#include "..\Event\Event.h"	// Event

/////////////////////////////////////////////////////////////////////////////
// CPlanApp:
// See Plan.cpp for the implementation of this class
//

class CPlanApp : public CWinApp
{
public:
	CPlanApp();

	EventLib &GetEventLib();
	void Error(UINT nIDError,CWnd *pWnd = NULL,UINT uType = MB_OK|MB_ICONEXCLAMATION);

private:
	EventLib m_EventLib;
	void SetRegistry();
	void LoadOptions();
	void SaveOptions();
	void ParseCommandLine(CCommandLineInfo &rCmdInfo);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlanApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPlanApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAN_H__4CE9C7E3_D9FC_4BDA_B409_3BECF194F784__INCLUDED_)

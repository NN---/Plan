#if !defined(AFX_PROPERTIESBAR_H__AF77AE00_CCA3_11D6_A4A4_D828A9243502__INCLUDED_)
#define AFX_PROPERTIESBAR_H__AF77AE00_CCA3_11D6_A4A4_D828A9243502__INCLUDED_

#include "Defines.h"		// Added by ClassView
#include "MessageEvent.h"	// Added by ClassView
#include "ShutDownEvent.h"	// Added by ClassView
#include "RunProgramEvent.h"// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar dialog

class CPropertiesBar : public CDialogBar
{
// Construction
public:
	CPropertiesBar();   // standard constructor
	~CPropertiesBar();
	void Initialize();

// Functions
	void Clear();
	BOOL IsEventClear(int iEvent=-1);
	void GetEvent(Event *pEvent,int iType=-1);
	void GetMessage(Message *pMessage);
	void GetShutDown(ShutDown *pShutDown);
	void GetRunProgram(RunProgram *pRunProgram);
	void SetEvent(Event *pEvent,short iType=-1);
	CDialog *GetEventClass(int iEvent);
	CMessageEvent *GetMessageClass();
	CShutDownEvent *GetShutDownClass();
	CRunProgramEvent *GetRunProgramClass();
	void UpdateEvents(BOOL bSaveAndValidate=TRUE);
	void UpdateAllData(BOOL bSaveAndValidate=TRUE);
	void HideEvent(BYTE Type);
	void HideAllEvents();
	void ShowEvent(BYTE Type);

// Dialog Data
	//{{AFX_DATA(CPropertiesBar)
	enum { IDD = IDD_PROPERTIES };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
public:
	CPropertiesBar::Create(CWnd *pParentWnd,UINT nStyle);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Data
	int m_iCurSel;
	CTime m_Time;
	CDialog *m_pdlgEvents[NUM_EVENTS];
	CComboBox	m_cmbEvents;
	// Generated message map functions
	//{{AFX_MSG(CPropertiesBar)
	afx_msg void OnDestroy();
	afx_msg void OnSelChangeEvents();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESBAR_H__AF77AE00_CCA3_11D6_A4A4_D828A9243502__INCLUDED_)

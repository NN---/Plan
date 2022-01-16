// PropertiesBar.cpp : implementation file
//

#include "stdafx.h"
#include "plan.h"
#include "PropertiesBar.h"

#include "MessageEvent.h"
#include "ShutDownEvent.h"
#include "RunProgramEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar dialog


CPropertiesBar::CPropertiesBar()
	: CDialogBar()
{
	int i;
	for(i=0;i<NUM_EVENTS;i++)
		m_pdlgEvents[i]=NULL;
	m_iCurSel = CB_ERR;
	m_Time = CTime::GetCurrentTime();
	//{{AFX_DATA_INIT(CPropertiesBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropertiesBar::~CPropertiesBar()
{
	int i;
	for(i=0;i<NUM_EVENTS;i++)
		delete m_pdlgEvents[i];
}

void CPropertiesBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_DT(pDX, IDC_DATE, IDC_TIME, m_Time);
	//{{AFX_DATA_MAP(CPropertiesBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_EVENTS, m_cmbEvents);
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CPropertiesBar, CDialogBar)
	//{{AFX_MSG_MAP(CPropertiesBar)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_EVENTS, OnSelChangeEvents)
	ON_COMMAND(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar message handlers

BOOL CPropertiesBar::Create(CWnd *pParentWnd,UINT nStyle)
{
	return CDialogBar::Create(pParentWnd,CPropertiesBar::IDD,nStyle,CPropertiesBar::IDD);
}

void CPropertiesBar::Initialize()
{
	RECT rect;
	//CTime MinTime(1970,1,2,0,0,0);
	CTime MinTime=CTime::GetCurrentTime();
	CTime MaxTime(2038,1,18,23,59,59);

	UpdateData(FALSE);

	((CDateTimeCtrl *)GetDlgItem(IDC_DATE))->SetRange(&MinTime,&MaxTime);
	m_pdlgEvents[EVENT_MESSAGE]=new CMessageEvent(this);
	m_pdlgEvents[EVENT_SHUTDOWN]=new CShutDownEvent(this);
	m_pdlgEvents[EVENT_RUNPROGRAM]=new CRunProgramEvent(this);

	GetDlgItem(IDC_EVENTPLACE)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	int i;
	for(i=0;i<NUM_EVENTS;i++)
	{
		m_pdlgEvents[i]->Create(EVENT_ID(i),this);
		m_pdlgEvents[i]->MoveWindow(&rect);
	}
}

void CPropertiesBar::OnDestroy() 
{
	int i;
	for(i=0;i<NUM_EVENTS;i++)
		m_pdlgEvents[i]->DestroyWindow();
	
	CDialogBar::OnDestroy();
}

void CPropertiesBar::OnSelChangeEvents() 
{
	int iCurSel=m_cmbEvents.GetCurSel();
	if(iCurSel!=CB_ERR&&iCurSel!=m_iCurSel)
	{
		if(m_iCurSel!=CB_ERR)
			m_pdlgEvents[m_iCurSel]->ShowWindow(SW_HIDE);
		m_iCurSel=iCurSel;
		m_pdlgEvents[m_iCurSel]->ShowWindow(SW_SHOW);
	}
	
}

BOOL CPropertiesBar::IsEventClear(int iEvent/*=-1*/)
{
	int _iEvent=iEvent==-1?m_iCurSel:iEvent;
	int i;
	if(_iEvent<0)
		return TRUE;
	for(i=0;i<NUM_EVENTS;i++)
		m_pdlgEvents[i]->UpdateData(TRUE);
	switch(_iEvent)
	{
		case EVENT_MESSAGE:
			return ((CMessageEvent *)m_pdlgEvents[EVENT_MESSAGE])->IsClear();
		case EVENT_SHUTDOWN:
			return ((CShutDownEvent *)m_pdlgEvents[EVENT_SHUTDOWN])->IsClear();
		case EVENT_RUNPROGRAM:
			return ((CRunProgramEvent *)m_pdlgEvents[EVENT_RUNPROGRAM])->IsClear();
		default:
			return TRUE;
	}
}

void CPropertiesBar::Clear()
{
	((CMessageEvent *)m_pdlgEvents[EVENT_MESSAGE])->Clear();
	((CShutDownEvent *)m_pdlgEvents[EVENT_SHUTDOWN])->Clear();
	((CRunProgramEvent *)m_pdlgEvents[EVENT_RUNPROGRAM])->Clear();
	if(m_iCurSel!=CB_ERR)
		HideEvent((BYTE)m_iCurSel);
	m_iCurSel=CB_ERR;
	m_cmbEvents.SetCurSel(CB_ERR);
	UpdateData(FALSE);
}

void CPropertiesBar::GetEvent(Event *pEvent,int iType/*=-1*/)
{
	if(pEvent==NULL)
		return;
	
	*pEvent=Event(m_Time,(BYTE)m_iCurSel);
	
	switch(iType)
	{
		case EVENT_MESSAGE:
			((CMessageEvent *)m_pdlgEvents[EVENT_MESSAGE])->GetEvent((Message *)pEvent);
			break;
		case EVENT_SHUTDOWN:
			((CShutDownEvent *)m_pdlgEvents[EVENT_SHUTDOWN])->GetEvent((ShutDown *)pEvent);
			break;
		case EVENT_RUNPROGRAM:
			((CRunProgramEvent *)m_pdlgEvents[EVENT_RUNPROGRAM])->GetEvent((RunProgram *)pEvent);
			break;
	}
}

void CPropertiesBar::GetMessage(Message *pMessage)
{
	GetEvent(pMessage,EVENT_MESSAGE);
}

void CPropertiesBar::GetShutDown(ShutDown *pShutDown)
{
	GetEvent(pShutDown,EVENT_SHUTDOWN);
}

void CPropertiesBar::GetRunProgram(RunProgram *pRunProgram)
{
	GetEvent(pRunProgram,EVENT_RUNPROGRAM);
}

CDialog *CPropertiesBar::GetEventClass(int iEvent)
{
	return m_pdlgEvents[iEvent];
}

CMessageEvent *CPropertiesBar::GetMessageClass()
{
	return ((CMessageEvent *)m_pdlgEvents[EVENT_MESSAGE]);
}

CShutDownEvent *CPropertiesBar::GetShutDownClass()
{
	return ((CShutDownEvent *)m_pdlgEvents[EVENT_SHUTDOWN]);
}

CRunProgramEvent *CPropertiesBar::GetRunProgramClass()
{
	return ((CRunProgramEvent *)m_pdlgEvents[EVENT_RUNPROGRAM]);
}

void CPropertiesBar::SetEvent(Event *pEvent,short iType/*=-1*/)
{
	Message *pMessage;
	ShutDown *pShutDown;
	RunProgram *pRunProgram;
	
	if(pEvent==NULL)
		return;

	if(iType>=NUM_EVENTS||iType<0)
		if((iType=pEvent->Type())>=NUM_EVENTS)
			return;

	if(m_iCurSel!=CB_ERR)
		HideEvent((BYTE)m_iCurSel);

	m_Time=pEvent->Time();
	m_cmbEvents.SetCurSel(iType);
	m_iCurSel=CB_ERR;

	UpdateData(FALSE);

	switch(iType)
	{
		case EVENT_MESSAGE:
			pMessage=(Message *)pEvent;
			((CMessageEvent *)m_pdlgEvents[EVENT_MESSAGE])->SetEvent(pMessage);
			ShowEvent(EVENT_MESSAGE);
			break;
		case EVENT_SHUTDOWN:
			pShutDown=(ShutDown *)pEvent;
			((CShutDownEvent *)m_pdlgEvents[EVENT_SHUTDOWN])->SetEvent(pShutDown);
			ShowEvent(EVENT_SHUTDOWN);
			break;
		case EVENT_RUNPROGRAM:
			pRunProgram=(RunProgram *)pEvent;
			((CRunProgramEvent *)m_pdlgEvents[EVENT_RUNPROGRAM])->SetEvent(pRunProgram);
			ShowEvent(EVENT_RUNPROGRAM);
			break;
	}
}

void CPropertiesBar::UpdateEvents(BOOL bSaveAndValidate/*=TRUE*/)
{
	int i;
	for(i=0;i<NUM_EVENTS;i++)
		if(m_pdlgEvents[i]!=NULL)
			m_pdlgEvents[i]->UpdateData(bSaveAndValidate);
}

void CPropertiesBar::UpdateAllData(BOOL bSaveAndValidate/*=TRUE*/)
{
	UpdateData(bSaveAndValidate);
	UpdateEvents(bSaveAndValidate);
}

void CPropertiesBar::HideAllEvents()
{
	int i;
	for(i=0;i<NUM_EVENTS;i++)
		if(m_pdlgEvents[i]!=NULL)
			m_pdlgEvents[i]->ShowWindow(SW_HIDE);
}

void CPropertiesBar::HideEvent(BYTE Type)
{
	if(Type<NUM_EVENTS)
		m_pdlgEvents[Type]->ShowWindow(SW_HIDE);
	m_cmbEvents.SetCurSel(CB_ERR);
	m_iCurSel=CB_ERR;
}

void CPropertiesBar::ShowEvent(BYTE Type)
{
	if(m_iCurSel!=CB_ERR)
		m_pdlgEvents[m_iCurSel]->ShowWindow(SW_HIDE);
	if(Type<NUM_EVENTS)
		m_pdlgEvents[Type]->ShowWindow(SW_SHOW);
	m_cmbEvents.SetCurSel(Type);
	m_iCurSel=Type;
}

void CPropertiesBar::OnCancel()
{
	GetParentFrame()->SetFocus();
}

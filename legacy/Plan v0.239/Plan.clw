; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPlanView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "plan.h"
LastPage=0

ClassCount=10
Class1=CConfirmationsPage
Class2=CGeneralPage
Class3=CMainFrame
Class4=CMessageEvent
Class5=CPlanApp
Class6=CPlanDoc
Class7=CPlanView
Class8=CPropertiesBar
Class9=CRunProgramEvent
Class10=CShutDownEvent

ResourceCount=10
Resource1=IDD_PROPERTIES (English (U.S.))
Resource2=IDD_MESSAGE (English (U.S.))
Resource3=IDD_GENERAL (English (U.S.))
Resource4=IDD_CONFIRMATIONS (English (U.S.))
Resource5=IDD_SHUTDOWN (English (U.S.))
Resource6=IDD_ABOUTBOX (English (U.S.))
Resource7=IDD_RUNPROGRAM (English (U.S.))
Resource8=IDD_MESSAGEBOX (English (U.S.))
Resource9=IDR_MAINFRAME (English (U.S.))
Resource10=IDD_NEWFILE (English (U.S.))

[CLS:CConfirmationsPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=ConfirmationsPage.h
ImplementationFile=ConfirmationsPage.cpp
LastObject=CConfirmationsPage

[CLS:CGeneralPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=GeneralPage.h
ImplementationFile=GeneralPage.cpp
LastObject=CGeneralPage

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrame.h
ImplementationFile=MainFrame.cpp
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:CMessageEvent]
Type=0
BaseClass=CDialog
HeaderFile=MessageEvent.h
ImplementationFile=MessageEvent.cpp
LastObject=CMessageEvent

[CLS:CPlanApp]
Type=0
BaseClass=CWinApp
HeaderFile=Plan.h
ImplementationFile=Plan.cpp
LastObject=CPlanApp

[CLS:CPlanDoc]
Type=0
BaseClass=CDocument
HeaderFile=PlanDoc.h
ImplementationFile=PlanDoc.cpp
LastObject=CPlanDoc

[CLS:CPlanView]
Type=0
BaseClass=CListView
HeaderFile=PlanView.h
ImplementationFile=PlanView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_EVENT_CLEARPROPERTIES

[CLS:CPropertiesBar]
Type=0
BaseClass=CDialogBar
HeaderFile=PropertiesBar.h
ImplementationFile=PropertiesBar.cpp
LastObject=CPropertiesBar
Filter=D
VirtualFilter=dWC

[CLS:CRunProgramEvent]
Type=0
BaseClass=CDialog
HeaderFile=RunProgramEvent.h
ImplementationFile=RunProgramEvent.cpp

[CLS:CShutDownEvent]
Type=0
BaseClass=CDialog
HeaderFile=ShutDownEvent.h
ImplementationFile=ShutDownEvent.cpp

[DLG:IDD_CONFIRMATIONS]
Type=1
Class=CConfirmationsPage

[DLG:IDD_GENERAL]
Type=1
Class=CGeneralPage

[DLG:IDD_MESSAGE]
Type=1
Class=CMessageEvent

[DLG:IDD_PROPERTIES]
Type=1
Class=CPropertiesBar

[DLG:IDD_RUNPROGRAM]
Type=1
Class=CRunProgramEvent

[DLG:IDD_SHUTDOWN]
Type=1
Class=CShutDownEvent

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EVENT_ADDEVENT
Command2=ID_EVENT_UPDATEEVENT
Command3=ID_EVENT_REMOVEEVENT
Command4=ID_TOOLS_OPTIONS
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE_AS
Command4=ID_APP_EXIT
Command5=ID_FILE_MRU_FILE1
Command6=ID_EVENT_ADDEVENT
Command7=ID_EVENT_UPDATEEVENT
Command8=ID_EVENT_REMOVEEVENT
Command9=ID_EVENT_CLEARPROPERTIES
Command10=ID_VIEW_TOOLBAR
Command11=ID_VIEW_STATUS_BAR
Command12=ID_VIEW_PROPERTIES
Command13=ID_VIEW_ENABLEDISABLE
Command14=ID_TOOLS_OPTIONS
Command15=ID_APP_ABOUT
CommandCount=15

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EVENT_ADDEVENT
Command2=ID_EVENT_CLEARPROPERTIES
Command3=ID_FILE_NEW
Command4=ID_FILE_OPEN
Command5=ID_TOOLS_OPTIONS
Command6=ID_EVENT_REMOVEEVENT
Command7=ID_VIEW_STATUS_BAR
Command8=ID_VIEW_TOOLBAR
Command9=ID_EVENT_UPDATEEVENT
Command10=ID_VIEW_ENABLEDISABLE
Command11=ID_APP_EXIT
Command12=ID_VIEW_PROPERTIES
CommandCount=12

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDOK,button,1342373889
Control3=IDC_STATIC,static,1342308480
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_PROPERTIES (English (U.S.))]
Type=1
Class=?
ControlCount=11
Control1=IDC_EVENTS,combobox,1344339971
Control2=IDC_DATE,SysDateTimePick32,1342242848
Control3=IDC_TIME,SysDateTimePick32,1342242857
Control4=ID_EVENT_ADDEVENT,button,1342242816
Control5=ID_EVENT_UPDATEEVENT,button,1342242816
Control6=ID_EVENT_REMOVEEVENT,button,1342242816
Control7=ID_EVENT_CLEARPROPERTIES,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EVENTPLACE,static,1073872896

[DLG:IDD_MESSAGE (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_CAPTION,edit,1350631552
Control2=IDC_TEXT,edit,1350635972
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_SHUTDOWN (English (U.S.))]
Type=1
Class=?
ControlCount=2
Control1=IDC_OPERATION,combobox,1344339971
Control2=IDC_STATIC,static,1342308352

[DLG:IDD_GENERAL (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_LOADONSTARTUP,button,1342242819
Control2=IDC_REMOVEEVENT,button,1342242825
Control3=IDC_CHECKEVENT,button,1342242825
Control4=IDC_STATIC,button,1342177287
Control5=IDC_BACKUPFILE,button,1342242819

[DLG:IDD_RUNPROGRAM (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_PATH,edit,1350631552
Control2=IDC_BROWSE,button,1342242816
Control3=IDC_PARAMETERS,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_CONFIRMATIONS (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_ADD,button,1342242819
Control2=IDC_REMOVE,button,1342242819
Control3=IDC_EXIT,button,1342242819
Control4=IDC_UPDATE,button,1342242819

[DLG:IDD_NEWFILE (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=ID_FILE_NEW,button,1342242817
Control2=ID_FILE_OPEN,button,1342242816
Control3=IDCANCEL,button,1342242816

[DLG:IDD_MESSAGEBOX (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_BUTTON2,button,1342242816
Control3=IDC_BUTTON3,button,1342242816
Control4=IDC_CHECK,button,1342251011
Control5=IDC_TEXT,static,1342308352


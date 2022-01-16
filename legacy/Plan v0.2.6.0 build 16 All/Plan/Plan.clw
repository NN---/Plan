; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPropertiesBar
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Plan.h"
LastPage=0

ClassCount=5
Class1=CPlanApp
Class2=CPlanDoc
Class3=CPlanView
Class4=CMainFrame

ResourceCount=8
Resource1=IDD_ABOUTBOX
Resource2=IDD_ABOUT (English (U.S.))
Class5=CPropertiesBar
Resource3=IDD_DIALOG1
Resource4=IDD_PROPERTIESBAR (English (U.S.))
Resource5=IDD_PROPPAGE_SMALL (English (U.S.))
Resource6=IDD_MESSAGEEVENT (English (U.S.))
Resource7="IDD_EVENT" (English (U.S.))
Resource8=IDR_MAINFRAME (English (U.S.))

[CLS:CPlanApp]
Type=0
HeaderFile=Plan.h
ImplementationFile=Plan.cpp
Filter=N
LastObject=CPlanApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CPlanDoc]
Type=0
HeaderFile=PlanDoc.h
ImplementationFile=PlanDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CPlanDoc

[CLS:CPlanView]
Type=0
HeaderFile=PlanView.h
ImplementationFile=PlanView.cpp
Filter=C
LastObject=CPlanView
BaseClass=CListView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrame.h
ImplementationFile=MainFrame.cpp
Filter=T
LastObject=ID_EVENT_EMPTY
BaseClass=CFrameWnd
VirtualFilter=fWC




[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=?

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_IMPORT
Command2=ID_FILE_EXPORT
Command3=ID_VIEW_OPTIONS
Command4=ID_APP_ABOUT
CommandCount=4

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_EXPORT
Command2=ID_FILE_IMPORT
Command3=ID_APP_EXIT
Command4=ID_EVENT_ADD
Command5=ID_EVENT_UPDATE
Command6=ID_EVENT_DELETE
Command7=ID_EVENT_EMPTY
Command8=ID_VIEW_TOOLBAR
Command9=ID_VIEW_STATUS_BAR
Command10=ID_VIEW_PROPERTIES
Command11=ID_VIEW_OPTIONS
Command12=ID_APP_ABOUT
CommandCount=12

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EVENT_ADD
Command2=ID_EVENT_DELETE
Command3=ID_FILE_EXPORT
Command4=ID_FILE_IMPORT
Command5=ID_VIEW_OPTIONS
Command6=ID_VIEW_STATUS_BAR
Command7=ID_VIEW_TOOLBAR
Command8=ID_EVENT_UPDATE
Command9=ID_APP_ABOUT
Command10=ID_EVENT_EMPTY
Command11=ID_VIEW_PROPERTIES
CommandCount=11

[DLG:IDD_ABOUT (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342312576
Control3=IDOK,button,1342373889
Control4=IDC_EMAIL,button,1342242816
Control5=IDC_SITE,button,1342242816

[DLG:IDD_PROPERTIESBAR (English (U.S.))]
Type=1
Class=CPropertiesBar
ControlCount=13
Control1=IDC_ONCE,button,1342373897
Control2=IDC_DAILY,button,1342373897
Control3=IDC_DAYS,listbox,1352728841
Control4=IDC_TIME,SysDateTimePick32,1342242857
Control5=IDC_DATE,SysDateTimePick32,1342242848
Control6=IDC_STATIC,static,1342177284
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EVENTS,combobox,1344339971
Control9=ID_EVENT_ADD,button,1342242816
Control10=ID_EVENT_UPDATE,button,1342242816
Control11=ID_EVENT_DELETE,button,1342242816
Control12=ID_EVENT_EMPTY,button,1342242816
Control13=IDC_EVENTPLACE,button,1207959559

[CLS:CPropertiesBar]
Type=0
HeaderFile=PropertiesBar.h
ImplementationFile=PropertiesBar.cpp
BaseClass=CDialogBar
Filter=D
LastObject=IDC_ONCE
VirtualFilter=dWC

[DLG:IDD_MESSAGEEVENT (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_CAPTION,edit,1350631552
Control4=IDC_TEXT,edit,1350635716

[DLG:IDD_PROPPAGE_SMALL (English (U.S.))]
Type=1
Class=?
ControlCount=0

[DLG:"IDD_EVENT" (English (U.S.))]
Type=1
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDC_BUTTON1,button,1342177280
Control2=IDC_BUTTON2,button,1342177280


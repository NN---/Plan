; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "plan.h"
LastPage=0

ClassCount=6
Class1=CMainFrame
Class2=CMenuBar
Class3=CPlanApp
Class4=CPlanDoc
Class5=CPlanView
Class6=CPropertiesBar

ResourceCount=3
Resource1=IDD_PROPERTIESBAR (English (U.S.))
Resource2=IDD_ABOUT (English (U.S.))
Resource3=IDR_MAINFRAME (English (U.S.))

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrame.h
ImplementationFile=MainFrame.cpp
LastObject=CMainFrame

[CLS:CMenuBar]
Type=0
BaseClass=CControlBar
HeaderFile=MenuBar.h
ImplementationFile=MenuBar.cpp

[CLS:CPlanApp]
Type=0
BaseClass=CWinApp
HeaderFile=Plan.h
ImplementationFile=Plan.cpp

[CLS:CPlanDoc]
Type=0
BaseClass=CDocument
HeaderFile=PlanDoc.h
ImplementationFile=PlanDoc.cpp

[CLS:CPlanView]
Type=0
BaseClass=CListView
HeaderFile=PlanView.h
ImplementationFile=PlanView.cpp

[CLS:CPropertiesBar]
Type=0
BaseClass=CDialogBar
HeaderFile=PropertiesBar.h
ImplementationFile=PropertiesBar.cpp

[DLG:IDD_PROPERTIESBAR]
Type=1
Class=CPropertiesBar

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_IMPORT
Command2=ID_FILE_EXPORT
Command3=ID_VIEW_OPTIONS
Command4=ID_APP_ABOUT
CommandCount=4

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
Command10=ID_VIEW_ENABLEDISABLE
Command11=ID_EVENT_EMPTY
Command12=ID_VIEW_PROPERTIES
CommandCount=12

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
Class=?
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
Command10=ID_VIEW_ENABLEDISABLE
Command11=ID_VIEW_PROPERTIES
Command12=ID_VIEW_OPTIONS
Command13=ID_APP_ABOUT
CommandCount=13


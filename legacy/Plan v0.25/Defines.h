// Defines.h
//

#ifndef __DEFINES_H

#ifndef __cplusplus
#error Must use cplusplus
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define __DEFINES_H

// Timer
#define TIMER_TIME 1000
#define TIMER_SEC  1
#define ID_TIMER   100

// Profile
#define PLAN			_T("Plan")
#define PLAN_FILE		_T("Plan.File")
#define PLAN_FILE_LEN	(9*sizeof(TCHAR))
#define PLAN_EXTENTION	_T(".pln")
#define PLAN_FILTER		_T("Plan Files (*.pln)")
#define PROFILE_NAME	_T("NN")

// Settings
#define SETTINGS			_T("Settings")
#define CURRENTDOCUMENT		_T("CurrentDocument")
#define CURRENTDIRECTORY	_T("CurrentDirectory")
#define OPTIONS				_T("Options")

// Default Settings
#define ADD_DEFAULT			1
#define UPDATE_DEFAULT		1
#define REMOVE_DEFAULT		1
#define EXIT_DEFAULT		1
#define TIMER_DEFAULT		1
#define CHECK_DEFAULT		1
#define BACKUPFILE_DEFAULT	1

// Window Position
#define WINDOWTOP		_T("WindowTop")
#define WINDOWBOTTOM	_T("WindowBottom")
#define WINDOWLEFT		_T("WindowLeft")
#define WINDOWRIGHT		_T("WindowRight")
#define WINDOWSTATE		_T("WindowState")

// Registry
#define REG_RUN		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run")

// Defines to MessageBox
#define MB_ERROR	 (MB_OK|MB_APPLMODAL)
#define MB_ERRORE	 (MB_ERROR|MB_ICONEXCLAMATION)
#define MB_ERRORI	 (MB_ERROR|MB_ICONINFORMATION)

// Backup File
#define BACKUPFILE_EXTENTION	_T(".bak")

// Sort
#define NOSORT			0
#define SORT_ASCENDING	1
#define SORT_DESCENDING	2

// Number of Items
#define MAX_ITEMS 20

// SubItems
#define NUM_SUB_ITEMS	3
#define SUB_ITEM_EVENT	0
#define SUB_ITEM_DATE	1
#define SUB_ITEM_TIME	2
#define SUB_ITEM_FIRST	SUB_ITEM_EVENT
#define SUB_ITEM(i)		(SUB_ITEM_FIRST+i)

// Strings for SubItems
#define STR_SUB_ITEM_FIRST	IDS_EVENT
#define STR_SUB_ITEM_EVENT	IDS_EVENT
#define STR_SUB_ITEM_DATE	IDS_DATE
#define STR_SUB_ITEM_TIME	IDS_TIME
#define STR_SUB_ITEM(i)		(STR_SUB_ITEM_FIRST+i)

// State ImageList
#define STATEIMAGE_WIDTH		16
#define STATEIMAGE_COLORMASK	RGB(255,0,0)
#define STATEIMAGE_ON			(BYTE)2
#define STATEIMAGE_OFF			(BYTE)1

// Number of Events
#ifndef NUM_EVENTS
#define NUM_EVENTS	3
#endif

// Header Styles
#define HEADER_MASK			HDI_FORMAT|HDI_TEXT|HDI_WIDTH
#define HEADER_FMT			HDF_STRING|HDF_LEFT

// File Size
// Columns
#define COLUMN_ORDER_SIZE NUM_SUB_ITEMS*sizeof(BYTE)
#define COLUMN_WIDTH_SIZE NUM_SUB_ITEMS*sizeof(WORD)

// Sort
#define SORT_SIZE 2*sizeof(BYTE)

// File Length
#define FILE_LENGTH(nItems,nMessage,nShutDown,nRunProgram) \
	(PLAN_FILE_LEN+COLUMN_ORDER_SIZE+COLUMN_WIDTH_SIZE+SORT_SIZE+ \
	sizeof(BYTE)+nItems*2*sizeof(BYTE)+ \
	sizeof(BYTE)+nMessage*sizeof(Message)+ \
	sizeof(BYTE)+nShutDown*sizeof(ShutDown)+ \
	sizeof(BYTE)+nRunProgram*sizeof(RunProgram))

#define MIN_FILE_LENGTH FILE_LENGTH(0,0,0,0)
#define FILE_DATA_SEEK (PLAN_FILE_LEN+COLUMN_ORDER_SIZE+COLUMN_WIDTH_SIZE+SORT_SIZE)

// DataExchange
// Time and Date
void AFXAPI DDX_DT(CDataExchange *pDX, const int nIDDate, const int nIDTime, CTime &rTime);

#endif // __DEFINES_H
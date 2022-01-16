#if !defined(AFX_PROPERTIESBAR_H__46DC51BD_5AFF_43AA_A330_1BA609E5A82D__INCLUDED_)
#define AFX_PROPERTIESBAR_H__46DC51BD_5AFF_43AA_A330_1BA609E5A82D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar dialog

class CPropertiesBar : public CDialogBar
{
public:
	// struct Data
	struct Data
	{
		struct Schedule
		{
			unsigned Once:1;
			unsigned Sunday:1;
			unsigned Monday:1;
			unsigned Tuesday:1;
			unsigned Wednesday:1;
			unsigned Thursday:1;
			unsigned Friday:1;
			unsigned Saturday:1;

			void Set(const unsigned uOnce=0,
				const unsigned uSunday=0,
				const unsigned uMonday=0,
				const unsigned uTuesday=0,
				const unsigned uWednesday=0,
				const unsigned uThursday=0,
				const unsigned uFriday=0,
				const unsigned uSaturday=0)
			{
					Once=uOnce;
					Sunday=uSunday;
					Monday=uMonday;
					Tuesday=uTuesday;
					Wednesday=uWednesday;
					Thursday=uThursday;
					Friday=uFriday;
					Saturday=uSaturday;
			}
			
			void SetOnce(unsigned uOnce)
			{
				Once=uOnce;
			}

			bool IsNoDays()
			{
				return !(Sunday | Monday |
					Tuesday | Wednesday |
					Thursday | Friday |
					Saturday);
			}

			WORD GetDayOfWeek()
			{
				return (WORD)(*((char *)this)>>1);
			}

		};

		CTime m_Time;
		Schedule m_Schedule;
	};
// Construction
public:
	CPropertiesBar();   // standard constructor
	void Empty();
	void GetData(Data &rData);
	void SetData(const Data &rData, PCEventID pcEventID, const LPBYTE pData);

// Dialog Data
	//{{AFX_DATA(CPropertiesBar)
	enum { IDD = IDD_PROPERTIESBAR };
	CComboBox	m_cmbEvents;
	CListBox	m_lbDays;
	//}}AFX_DATA
	int m_iCurSel;

// Overrides
	BOOL Create(CWnd *pParentWnd,UINT nStyle);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesBar)
	protected:
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void OnceDaily(BOOL bOnce);
	void FillData();
	Data m_Data;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertiesBar)
	afx_msg void OnOnce();
	afx_msg void OnDaily();
	afx_msg void OnSelChangeEvents();
	afx_msg void OnSelChangeDays();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void DDX_PropertiesBar(CDataExchange *pDX);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESBAR_H__46DC51BD_5AFF_43AA_A330_1BA609E5A82D__INCLUDED_)


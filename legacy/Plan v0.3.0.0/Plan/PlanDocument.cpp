#include "StdAfx.h"
#include "PlanDocument.h"

// Static members
const CPlanDocument::Options CPlanDocument::s_defaultOptions=
{

};

const CPlanDocument::Version CPlanDocument::s_currentVersion=
{
	0, // WORD V1
	3, // WORD V2
	0, // WORD V3
	0, // WORD V4
};
const CPlanDocument::Version CPlanDocument::s_validVersion[]=
{
	{
		0, // WORD V1
		3, // WORD V2
		0, // WORD V3
		0, // WORD V4
	},
};

const CPlanDocument::WindowPosition CPlanDocument::s_defaultWindowPosition=
{
	false, // bool bMin
	false, // bool bMax
	false, // bool bHide
	{ // WINDOWPLACEMENT wndpl
		0, // UINT length
		0, // UINT flags
		SW_SHOW, // UINT showCmd
		{ // POINT ptMinPosition
			0, // LONG x
			0, // LONG y
		},
		{ // POINT ptMaxPosition
			0, // LONG x
			0, // LONG y
		},
		{ // RECT rcNormalPosition
			140, // LONG left
			100, // LONG top
			610, // LONG right
			480, // LONG bottom
		}
	}
};

void CPlanDocument::SelectEvent(size_t /*nEvent*/)
{
//	ATLASSERT(nEvent>=0 && nEvent<m_aData.GetCount());
	//SetData(aEventData[iEvent].FData,aEventData[iEvent].pcEventID,aEventData[iEvent].pData);
}
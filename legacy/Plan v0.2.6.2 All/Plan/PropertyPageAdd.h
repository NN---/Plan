#ifndef __PROPERTYPAGEADD_H__
#define __PROPERTYPAGEADD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrameBase.h"

template <typename T, int ID>
class ATL_NO_VTABLE CPropertyPageAdd : public CPropertyPage<ID>,
										public CWinDataExchange<T>
{
public:
	void SetOptions(CMainFrameBase::Options *pOptions)
	{
		ATLASSERT(pOptions);
		m_pOptions=pOptions;
	}
	CMainFrameBase::Options *m_pOptions;
};

#endif // __PROPERTYPAGEADD_H__
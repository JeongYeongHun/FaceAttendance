// eDeuProject.h : main header file for the EDEUPROJECT application
//

#if !defined(AFX_EDEUPROJECT_H__EFCE615E_B4BB_4462_BD26_1596A299AB70__INCLUDED_)
#define AFX_EDEUPROJECT_H__EFCE615E_B4BB_4462_BD26_1596A299AB70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CeDeuProjectApp:
// See eDeuProject.cpp for the implementation of this class
//

class CeDeuProjectApp : public CWinApp
{
public:
	CeDeuProjectApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CeDeuProjectApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	// Implementation

	//{{AFX_MSG(CeDeuProjectApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDEUPROJECT_H__EFCE615E_B4BB_4462_BD26_1596A299AB70__INCLUDED_)
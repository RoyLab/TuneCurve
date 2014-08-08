
// drawer.h : main header file for the drawer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CdrawerApp:
// See drawer.cpp for the implementation of this class
//

class CdrawerApp : public CWinApp
{
public:
	CdrawerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CdrawerApp theApp;

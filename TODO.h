
// TODO.h : main header file for the TODO application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTODOApp:
// See TODO.cpp for the implementation of this class
//

class CTODOApp : public CWinApp
{
public:
	CTODOApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTODOApp theApp;

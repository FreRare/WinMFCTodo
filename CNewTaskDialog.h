#pragma once
#include "afxdialogex.h"


// CNewTaskDialog dialog

class CNewTaskDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNewTaskDialog)

public:
	CNewTaskDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CNewTaskDialog();
	CString m_label;
	int m_prioIndex;
	BOOL m_completed = false;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW_TASK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

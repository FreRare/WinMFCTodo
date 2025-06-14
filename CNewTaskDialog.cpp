// CNewTaskDialog.cpp : implementation file
//

#include "pch.h"
#include "TODO.h"
#include "afxdialogex.h"
#include "CNewTaskDialog.h"


// CNewTaskDialog dialog

IMPLEMENT_DYNAMIC(CNewTaskDialog, CDialogEx)

CNewTaskDialog::CNewTaskDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEW_TASK_DIALOG, pParent)
{

}

CNewTaskDialog::~CNewTaskDialog()
{
}

void CNewTaskDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TASK_LABEL, m_label);
	DDX_CBIndex(pDX, IDC_COMBO_PRIO, m_prioIndex);
	DDX_Check(pDX, IDC_CHECK_COMPL, m_completed);
}

BOOL CNewTaskDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CStatic* complTitle = (CStatic*)GetDlgItem(IDC_STATIC_CHECKED);
	if (complTitle) {
		complTitle->SetWindowTextW(_T("Finished:"));
	}

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRIO);
	if (pCombo)
	{
		pCombo->AddString(_T("Can wait"));
		pCombo->AddString(_T("Medium"));
		pCombo->AddString(_T("Urgent"));
		pCombo->SetCurSel(1); // default to Medium
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CNewTaskDialog::OnOK()
{
	UpdateData(TRUE); // transfer data from controls to variables

	m_label.Trim();
	if (m_label.IsEmpty())
	{
		AfxMessageBox(_T("Please enter a task name."), MB_ICONWARNING);
		return; // prevent dialog from closing
	}

	CDialogEx::OnOK(); // proceed to close
}

BEGIN_MESSAGE_MAP(CNewTaskDialog, CDialogEx)
END_MESSAGE_MAP()


// CNewTaskDialog message handlers

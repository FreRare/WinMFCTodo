
// TODOView.cpp : implementation of the CTODOView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TODO.h"
#endif

#include "TODODoc.h"
#include "TODOView.h"
#include "CNewTaskDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTODOView

IMPLEMENT_DYNCREATE(CTODOView, CFormView)

BEGIN_MESSAGE_MAP(CTODOView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_UPDATE, &CFormView::OnInitialUpdate)
	//ON_COMMAND(ID_FILE_NEW, &CTODOView::OnNewDocument)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CTODOView::OnNewTask)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_CHECKED, &CTODOView::OnRemoveAllFinishedTasks)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TASK_LIST, &CTODOView::OnLvnItemchangedTaskList)
	ON_NOTIFY(NM_DBLCLK, IDC_TASK_LIST, &CTODOView::OnNMDblClickTaskList)
END_MESSAGE_MAP()

// CTODOView construction/destruction

CTODOView::CTODOView() noexcept : CFormView(IDD_TODO_FORM), m_bIsPopulating(true)
{
	// TODO: add construction code here

}

CTODOView::~CTODOView()
{
}

BOOL CTODOView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTODOView drawing

void CTODOView::OnDraw(CDC* /*pDC*/)
{
	CTODODoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTODOView printing

BOOL CTODOView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTODOView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTODOView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTODOView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_taskList.SetExtendedStyle(m_taskList.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_taskList.ModifyStyle(0, LVS_REPORT);
	if (m_taskList.GetHeaderCtrl()->GetItemCount() == 0)  // only add columns if none exist
	{
		m_taskList.InsertColumn(0, _T("Task"), LVCFMT_LEFT, 200);
		m_taskList.InsertColumn(1, _T("Priority"), LVCFMT_LEFT, 100);
	}
	LoadTasksFromDocument();
}

void CTODOView::LoadTasksFromDocument()
{
	m_bIsPopulating = true;

	CTODODoc* pDoc = GetDocument();

	m_taskList.DeleteAllItems();

	int count = pDoc->m_tasks.GetSize();

	CString msg;
	msg.Format(_T("Loading task... total: %d\n"), count);
	AfxOutputDebugString(msg);

	for (int i = 0; i < count; ++i) {
		const Task& task = pDoc->m_tasks.GetAt(i);

		m_taskList.InsertItem(i, task.label);

		CString prioStr;
		switch (task.prio) {
		case Task::LOW: prioStr = _T("Can wait"); break;
		case Task::MEDIUM: prioStr = _T("Medium"); break;
		case Task::HIGH: prioStr = _T("Urgent"); break;
		}

		CString msg;
		OutputDebugString(msg);
		m_taskList.SetItemText(i, 1, prioStr);
		m_taskList.SetCheck(i, task.completed);
	}
	m_bIsPopulating = false;
}

void CTODOView::OnLvnItemchangedTaskList(NMHDR* pNMHDR, LRESULT* pResult)
{

	if (m_bIsPopulating) {
		*pResult = 0;
		return;
	}

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ((pNMLV->uChanged & LVIF_STATE) &&
		((pNMLV->uNewState & LVIS_STATEIMAGEMASK) != (pNMLV->uOldState & LVIS_STATEIMAGEMASK)))
	{
		int index = pNMLV->iItem;
		CTODODoc* pDoc = GetDocument();
		if (index >= 0 && index < pDoc->m_tasks.GetSize()){
		
			BOOL checked = m_taskList.GetCheck(index);

			pDoc->m_tasks[index].completed = checked;
			CString msg;
			msg.Format(_T("Checkbox changed on item %d to %d\n"), index, checked);
			OutputDebugString(msg);

			pDoc->SetModifiedFlag(TRUE); // mark doc as modified for saving
		}
	}

	*pResult = 0;
}

void CTODOView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASK_LIST, m_taskList);
}


void CTODOView::OnNewTask()
{
	CNewTaskDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		Task::TaskPriority prio = static_cast<Task::TaskPriority>(dlg.m_prioIndex);
		Task newTask(dlg.m_label, prio, dlg.m_completed);

		CTODODoc* pDoc = GetDocument();
		pDoc->m_tasks.Add(newTask);
		pDoc->SetModifiedFlag(TRUE);
		LoadTasksFromDocument();
	}
}

void CTODOView::OnRemoveAllFinishedTasks()
{
	// Make sure we want to do this
	if (AfxMessageBox(_T("Are you sure you want to remove all checked tasks?"), MB_YESNO | MB_ICONQUESTION) != IDYES)
		return;

	CTODODoc* pDoc = GetDocument();

	// Use a temporary array to hold remaining tasks
	CArray<Task, Task&> remainingTasks;

	int itemCount = m_taskList.GetItemCount();

	for (int i = 0; i < itemCount; ++i)
	{
		if (!m_taskList.GetCheck(i)) // Keep only unchecked tasks
		{
			if (i < pDoc->m_tasks.GetSize())
				remainingTasks.Add(pDoc->m_tasks[i]);
		}
	}

	// Replace original task array
	pDoc->m_tasks.RemoveAll();
	pDoc->m_tasks.Append(remainingTasks);

	pDoc->SetModifiedFlag(TRUE); // Mark document dirty for saving
	LoadTasksFromDocument();     // Refresh the list

}

void CTODOView::OnNMDblClickTaskList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItem = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int index = pNMItem->iItem;

	if (index >= 0)
	{
		CTODODoc* pDoc = GetDocument();
		OutputDebugString(_T("Saving...\n"));
		if (index < pDoc->m_tasks.GetSize())
		{
			OutputDebugString(_T("Got tasks\n"));
			Task& task = pDoc->m_tasks[index];

			CNewTaskDialog dlg;
			// dlg.SetWindowTextW(_T("Edit a task")); // For some reason this is not handled well, maybe because this operation is blocked
			dlg.m_label = task.label;
			dlg.m_prioIndex = static_cast<int>(task.prio);
			dlg.m_completed = task.completed;

			if (dlg.DoModal() == IDOK)
			{
				// Update the task
				task.label = dlg.m_label;
				task.prio = static_cast<Task::TaskPriority>(dlg.m_prioIndex);
				task.completed = dlg.m_completed;

				CString msg;
				msg.Format(_T("Saving task with %d and %d and %d\n"), index, dlg.m_completed, dlg.m_prioIndex);
				OutputDebugString(msg);

				pDoc->SetModifiedFlag(TRUE);
				LoadTasksFromDocument();
			}
		}
	}

	*pResult = 0;
}


// CTODOView diagnostics

#ifdef _DEBUG
void CTODOView::AssertValid() const
{
	CView::AssertValid();
}

void CTODOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTODODoc* CTODOView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTODODoc)));
	return (CTODODoc*)m_pDocument;
}
#endif //_DEBUG


// CTODOView message handlers

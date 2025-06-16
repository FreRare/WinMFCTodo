
// TODOView.h : interface of the CTODOView class
//

#pragma once


class CTODOView : public CFormView
{
protected: // create from serialization only
	CTODOView() noexcept;
	DECLARE_DYNCREATE(CTODOView)

	// Attributes
public:
	CTODODoc* GetDocument() const;
	CListCtrl m_taskList;
	BOOL m_bIsPopulating;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();
	virtual void OnLvnItemchangedTaskList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnNMDblClickTaskList(NMHDR* pNMHDR, LRESULT* pResult);
	void LoadTasksFromDocument();
	void OnNewDocument();
	void OnNewTask();
	void OnRemoveAllFinishedTasks();


	// Implementation
public:
	virtual ~CTODOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TODOView.cpp
inline CTODODoc* CTODOView::GetDocument() const
{
	return reinterpret_cast<CTODODoc*>(m_pDocument);
}
#endif


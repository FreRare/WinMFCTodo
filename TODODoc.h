
// TODODoc.h : interface of the CTODODoc class
//


#pragma once

class Task {
public:
	enum TaskPriority {
		LOW,
		MEDIUM,
		HIGH,
	};

	CString label;
	TaskPriority prio;
	BOOL completed;

	Task(const CString t = _T(""), TaskPriority p = TaskPriority::MEDIUM, BOOL c = false) : label(t), prio(p), completed(c) {}
};


class CTODODoc : public CDocument
{
protected: // create from serialization only
	CTODODoc() noexcept;
	DECLARE_DYNCREATE(CTODODoc)

// Attributes
public:
	CArray<Task, Task&> m_tasks;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CTODODoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

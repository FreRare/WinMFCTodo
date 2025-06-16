
// TODODoc.cpp : implementation of the CTODODoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TODO.h"
#endif

#include "TODODoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTODODoc

IMPLEMENT_DYNCREATE(CTODODoc, CDocument)

BEGIN_MESSAGE_MAP(CTODODoc, CDocument)
END_MESSAGE_MAP()


// CTODODoc construction/destruction

CTODODoc::CTODODoc() noexcept
{
	// TODO: add one-time construction code here
}

CTODODoc::~CTODODoc()
{
}

BOOL CTODODoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)


	return TRUE;
}




// CTODODoc serialization

void CTODODoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		int count = m_tasks.GetSize();
		ar << count;
		for (int i = 0; i < count; ++i)
		{
			ar << m_tasks[i].label;
			ar << static_cast<int>(m_tasks[i].prio);
			ar << m_tasks[i].completed;
		}
	}
	else
	{
		m_tasks.RemoveAll();
		int count;
		ar >> count;
		for (int i = 0; i < count; ++i)
		{
			CString text;
			int prioInt;
			bool completed;
			ar >> text;
			ar >> prioInt;
			ar >> completed;
			m_tasks.Add(Task(text, static_cast<Task::TaskPriority>(prioInt), completed));
		}
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CTODODoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CTODODoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTODODoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTODODoc diagnostics

#ifdef _DEBUG
void CTODODoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTODODoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTODODoc commands

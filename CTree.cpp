// CTree.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "CTree.h"


// CTree

IMPLEMENT_DYNCREATE(CTree, CView)

CTree::CTree()
{

}

CTree::~CTree()
{
}

BEGIN_MESSAGE_MAP(CTree, CView)
END_MESSAGE_MAP()


// CTree 그리기

void CTree::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CTree 진단

#ifdef _DEBUG
void CTree::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTree::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTree 메시지 처리기

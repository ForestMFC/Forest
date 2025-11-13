// CDay.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "CDay.h"


// CDay

IMPLEMENT_DYNCREATE(CDay, CView)

CDay::CDay()
{

}

CDay::~CDay()
{
}

BEGIN_MESSAGE_MAP(CDay, CView)
END_MESSAGE_MAP()


// CDay 그리기

void CDay::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CDay 진단

#ifdef _DEBUG
void CDay::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDay::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDay 메시지 처리기

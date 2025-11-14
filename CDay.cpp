// CDay.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "CDay.h"


// CDay

IMPLEMENT_DYNCREATE(CDay, CView)

CDay::CDay()
	: m_nYear(0), m_nMonth(0), m_nDay(0) // 0으로 초기화

{
}

CDay::~CDay()
{
}

BEGIN_MESSAGE_MAP(CDay, CView)
	// ★★★ 1. 이 줄이 반드시 있어야 메시지를 받습니다! ★★★
	ON_MESSAGE(WM_UPDATE_DATE_INFO, &CDay::OnUpdateDateInfo)
END_MESSAGE_MAP()


// CDay 그리기

void CDay::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	// 날짜가 선택되지 않았으면 안내 문구 출력
	if (m_nYear == 0)
	{
		pDC->TextOutW(10, 10, _T("날짜를 선택해 주세요."));
		return;
	}

	// 선택된 날짜 정보 출력
	CString strDate;
	strDate.Format(_T("선택된 날짜: %04d년 %02d월 %02d일"), m_nYear, m_nMonth, m_nDay);

	// 글자 크기나 색상은 자유롭게 꾸미세요
	pDC->TextOutW(10, 10, strDate);
	pDC->TextOutW(10, 40, _T("여기에 해당 날짜의 상세 일정을 표시합니다."));
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

// ★ 메시지 처리기 구현
// WPARAM에는 연도, LPARAM에는 월/일을 담아서 보낼 예정입니다.
LRESULT CDay::OnUpdateDateInfo(WPARAM wParam, LPARAM lParam)
{
	// 1. 데이터 해독 (Unpack)
	m_nYear = (int)wParam;           // 연도
	m_nMonth = LOWORD(lParam);       // 월 (LPARAM의 하위 16비트)
	m_nDay = HIWORD(lParam);         // 일 (LPARAM의 상위 16비트)

	// 2. 화면 갱신 요청 (OnDraw 호출됨)
	Invalidate();

	return 0;
}

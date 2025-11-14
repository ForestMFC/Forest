
// CalendarAppView.cpp: CCalendarAppView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "CalendarApp.h"
#endif

#include "CalendarAppDoc.h"
#include "CalendarAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCalendarAppView

IMPLEMENT_DYNCREATE(CCalendarAppView, CView)

BEGIN_MESSAGE_MAP(CCalendarAppView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CCalendarAppView 생성/소멸

CCalendarAppView::CCalendarAppView() noexcept
    : m_nYear(0)
    , m_nMonth(0)
    , m_nSelDay(0)
{
    // 1) 요일 이름 초기화
    m_weekNames[0] = L"일";
    m_weekNames[1] = L"월";
    m_weekNames[2] = L"화";
    m_weekNames[3] = L"수";
    m_weekNames[4] = L"목";
    m_weekNames[5] = L"금";
    m_weekNames[6] = L"토";

    // 2) 각 월의 기본 일 수 (2월은 추후 UpdateFebruaryDays로 수정)
    int temp[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    for (int i = 0; i < 12; ++i)
        m_daysInMonth[i] = temp[i];

    // 3) 요일별 텍스트 색 (일요일 빨강, 토요일 파랑)
    m_dayTextColor[0] = RGB(255, 0, 0);   // 일
    m_dayTextColor[1] = RGB(0, 0, 0);     // 월
    m_dayTextColor[2] = RGB(0, 0, 0);     // 화
    m_dayTextColor[3] = RGB(0, 0, 0);     // 수
    m_dayTextColor[4] = RGB(0, 0, 0);     // 목
    m_dayTextColor[5] = RGB(0, 0, 0);     // 금
    m_dayTextColor[6] = RGB(0, 0, 255);   // 토

    // 4) 오늘 날짜로 초기화
    CTime now = CTime::GetCurrentTime();
    m_nYear = now.GetYear();
    m_nMonth = now.GetMonth();
    m_nSelDay = now.GetDay();   // 오늘 날짜를 선택 상태로 사용

    // 5) 윤년 반영해서 2월 일수 조정
    UpdateFebruaryDays(m_nYear);
}


CCalendarAppView::~CCalendarAppView()
{
}

// ===== 달력 계산용 헬퍼 함수들 =====

bool CCalendarAppView::IsLeapYear(int year)
{
    // (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)
    if (!(year % 4) && year % 100 || !(year % 400))
        return true;
    return false;
}

void CCalendarAppView::UpdateFebruaryDays(int year)
{
    if (IsLeapYear(year))
        m_daysInMonth[1] = 29;  // 2월
    else
        m_daysInMonth[1] = 28;
}

int CCalendarAppView::GetFirstDayOfMonth(int year, int month)
{
    // year년 month월 1일이 무슨 요일인지 (0:일 ~ 6:토)
    UpdateFebruaryDays(year);

    int a_year = year - 1;

    // 작년 12/31까지의 일수
    int days = a_year * 365 + a_year / 4 - a_year / 100 + a_year / 400;

    // 올해 1월부터 (month-1)월까지 일수
    for (int i = 0; i < month - 1; ++i)
        days += m_daysInMonth[i];

    // 이번 달 1일
    days += 1;

    return days % 7; // 0:일, 1:월, ..., 6:토
}

bool CCalendarAppView::HitTestDayCell(CPoint pt, int& outDay)
{
    CRect rcClient;
    GetClientRect(&rcClient);

    // OnDraw와 동일한 달력 영역 계산
    CRect rcCal = rcClient;
    rcCal.top += 50;             // 제목 아래로 50px
    rcCal.DeflateRect(20, 0, 20, 20);

    int calWidth = rcCal.Width();
    int calHeight = rcCal.Height();

    int cellWidth = calWidth / 7;   // 7열
    int cellHeight = calHeight / 7;  // 1행 요일, 나머지 날짜

    int firstDay = GetFirstDayOfMonth(m_nYear, m_nMonth);
    int daysInMonth = m_daysInMonth[m_nMonth - 1];

    for (int day = 1; day <= daysInMonth; ++day)
    {
        int idx = firstDay + (day - 1);
        int row = idx / 7 + 1;  // 0행은 요일, 1행부터 날짜
        int col = idx % 7;

        CRect rcCell(
            rcCal.left + col * cellWidth,
            rcCal.top + row * cellHeight,
            rcCal.left + (col + 1) * cellWidth,
            rcCal.top + (row + 1) * cellHeight
        );

        if (rcCell.PtInRect(pt))
        {
            outDay = day;
            return true;
        }
    }

    return false;
}


BOOL CCalendarAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CCalendarAppView 그리기

void CCalendarAppView::OnDraw(CDC* pDC)
{
    CCalendarAppDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    CDC& dc = *pDC;

    // ----- 1) 전체 영역 계산 -----
    CRect rcClient;
    GetClientRect(&rcClient);

    // 제목(연/월) 영역
    CRect rcTitle = rcClient;
    rcTitle.top = 10;
    rcTitle.bottom = 40;

    // 달력(요일+날짜) 영역
    CRect rcCal = rcClient;
    rcCal.top += 50;             // 제목 아래로 50px 내려서 시작
    rcCal.DeflateRect(20, 0, 20, 20);  // 좌우 여백 20

    int calWidth = rcCal.Width();
    int calHeight = rcCal.Height();

    int cellWidth = calWidth / 7;   // 7열 (일~토)
    int cellHeight = calHeight / 7;  // 1행 요일, 6행 날짜

    dc.SetBkMode(TRANSPARENT);

    // ----- 2) 제목: "YYYY년 MM월" -----
    CString strTitle;
    strTitle.Format(_T("%04d년 %02d월"), m_nYear, m_nMonth);
    dc.SetTextColor(RGB(0, 0, 0));
    dc.DrawText(strTitle, rcTitle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // ----- 2-1) 좌우 화살표 영역 계산 -----
    int arrowWidth = 20;
    int arrowHeight = 20;

    // "<" 왼쪽 화살표: 제목 왼쪽에
    m_rcPrev = CRect(
        rcTitle.left + 10,
        rcTitle.top,
        rcTitle.left + 10 + arrowWidth,
        rcTitle.top + arrowHeight
    );

    // ">" 오른쪽 화살표: 제목 오른쪽에
    m_rcNext = CRect(
        rcTitle.right - 10 - arrowWidth,
        rcTitle.top,
        rcTitle.right - 10,
        rcTitle.top + arrowHeight
    );

    // 화살표 텍스트 그리기
    dc.DrawText(_T("<"), m_rcPrev, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    dc.DrawText(_T(">"), m_rcNext, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // ----- 3) 요일 헤더 -----
    for (int i = 0; i < 7; ++i)
    {
        CRect rcCell(
            rcCal.left + i * cellWidth,
            rcCal.top,
            rcCal.left + (i + 1) * cellWidth,
            rcCal.top + cellHeight
        );

        dc.Rectangle(rcCell);

        dc.SetTextColor(m_dayTextColor[i]);   // 요일에 맞는 색
        dc.DrawText(m_weekNames[i], -1, rcCell,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // ----- 4) 날짜 숫자들 -----
    int firstDay = GetFirstDayOfMonth(m_nYear, m_nMonth);   // 0~6
    int daysInMonth = m_daysInMonth[m_nMonth - 1];

    dc.SetTextColor(RGB(0, 0, 0));

    for (int day = 1; day <= daysInMonth; ++day)
    {
        int idx = firstDay + (day - 1); // 0부터 시작
        int row = idx / 7 + 1;          // 0행은 요일, 날짜는 1행부터
        int col = idx % 7;

        CRect rcCell(
            rcCal.left + col * cellWidth,
            rcCal.top + row * cellHeight,
            rcCal.left + (col + 1) * cellWidth,
            rcCal.top + (row + 1) * cellHeight
        );

        CBrush brushSel(RGB(255, 255, 200));      // 선택된 날짜
        CBrush brushNormal(RGB(255, 255, 255));  // 선택 안 된 날짜(흰색)

        CBrush* pOldBrush = nullptr;

        if (day == m_nSelDay && m_nSelDay != 0)
        {
            // 선택된 날짜: 파란 브러시로 Rectangle
            pOldBrush = dc.SelectObject(&brushSel);
            dc.SetTextColor(RGB(0, 0, 0));  // 글자는 흰색
        }
        else
        {
            // 일반 날짜: 흰 브러시로 Rectangle
            pOldBrush = dc.SelectObject(&brushNormal);
            dc.SetTextColor(RGB(0, 0, 0));        // 글자는 검정
        }

        // 🔹 이 Rectangle이 배경 + 테두리를 한 번에 그려줌
        dc.Rectangle(rcCell);

        // 🔹 브러시 원래 걸로 복원
        if (pOldBrush)
            dc.SelectObject(pOldBrush);

        // 날짜 숫자 그리기
        CString s;
        s.Format(_T("%d"), day);
        dc.DrawText(s, rcCell, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}



// CCalendarAppView 인쇄

BOOL CCalendarAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CCalendarAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CCalendarAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CCalendarAppView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // "<" 이전 달 클릭?
    if (m_rcPrev.PtInRect(point))
    {
        m_nMonth--;
        if (m_nMonth < 1)
        {
            m_nMonth = 12;
            m_nYear--;
        }

        // 달이 바뀌면 2월 일수 다시 계산
        UpdateFebruaryDays(m_nYear);

        // 다른 달에서는 오늘 강조를 없애고 싶으면 0으로
        m_nSelDay = 0;

        Invalidate(); // 다시 그리기
        return;
    }

    // ">" 다음 달 클릭?
    if (m_rcNext.PtInRect(point))
    {
        m_nMonth++;
        if (m_nMonth > 12)
        {
            m_nMonth = 1;
            m_nYear++;
        }

        UpdateFebruaryDays(m_nYear);
        m_nSelDay = 0;      // 다른 달에서는 선택 해제

        Invalidate();
        return;
    }

    int day = 0;
    if (HitTestDayCell(point, day))
    {
        m_nSelDay = day;   // 선택된 날짜 갱신

        Invalidate();

        // TODO: 여기서 스케줄 상세 다이얼로그를 띄우면 된다.
        // 예시로 일단 메시지박스로 날짜를 확인해보자:

        // ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
        // [추가된 부분] 다른 뷰(CDay)에게 데이터 전송하기
        // ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

        // 1. 데이터 포장 (Pack)
        // WPARAM에 연도, LPARAM에 월(Low)과 일(High)을 담습니다.
        WPARAM wp = (WPARAM)m_nYear;
        LPARAM lp = MAKELPARAM(m_nMonth, m_nSelDay);

        // 2. 메인 프레임 산하의 모든 자식 창에게 메시지 방송
        // (이렇게 하면 CDay도 이 메시지를 받게 됩니다)
        AfxGetMainWnd()->SendMessageToDescendants(WM_UPDATE_DATE_INFO, wp, lp, TRUE, TRUE);


        CString msg;
        msg.Format(_T("%04d-%02d-%02d 날짜가 클릭되었습니다."),
            m_nYear, m_nMonth, m_nSelDay);
        AfxMessageBox(msg);

        return;
    }

    CView::OnLButtonDown(nFlags, point);
}


// CCalendarAppView 진단

#ifdef _DEBUG
void CCalendarAppView::AssertValid() const
{
	CView::AssertValid();
}

void CCalendarAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCalendarAppDoc* CCalendarAppView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalendarAppDoc)));
	return (CCalendarAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CCalendarAppView 메시지 처리기

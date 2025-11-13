
// CalendarAppView.h: CCalendarAppView 클래스의 인터페이스
//

#pragma once


class CCalendarAppView : public CView
{
protected: // serialization에서만 만들어집니다.
	CCalendarAppView() noexcept;
	DECLARE_DYNCREATE(CCalendarAppView)

// 특성입니다.
public:
	CCalendarAppDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CCalendarAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수

private:
    // 요일 텍스트
    const wchar_t* m_weekNames[7];

    // 각 월의 일 수 (2월은 나중에 윤년 계산으로 수정)
    int m_daysInMonth[12];

    // 요일별 글자 색
    COLORREF m_dayTextColor[7];

    // 현재 표시 중인 년/월, 선택된 날짜
    int m_nYear;    // 예: 2025
    int m_nMonth;   // 1~12
    int m_nSelDay;  // 1~31 (오늘 날짜), 0이면 선택 없음

    CRect m_rcPrev;   // "<"
    CRect m_rcNext;   // ">"

    // 달력 계산용 헬퍼 함수
    bool IsLeapYear(int year);
    void UpdateFebruaryDays(int year);
    int  GetFirstDayOfMonth(int year, int month); // 0:일 ~ 6:토
    bool HitTestDayCell(CPoint pt, int& outDay);

protected:
    // 마우스 왼쪽 버튼 핸들러 선언
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CalendarAppView.cpp의 디버그 버전
inline CCalendarAppDoc* CCalendarAppView::GetDocument() const
   { return reinterpret_cast<CCalendarAppDoc*>(m_pDocument); }
#endif


// CDay.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "CDay.h"
#include "CAddDlg.h"
#include "CalendarAppDoc.h"


// CDay

IMPLEMENT_DYNCREATE(CDay, CView)

CDay::CDay()
{
    m_pDlgManager = nullptr;
}

CDay::~CDay()
{
}

void CDay::OnDestroy()
{
    CView::OnDestroy();
    if (m_pDlgManager)
    {
        m_pDlgManager->DestroyWindow(); // 창 닫기
        delete m_pDlgManager;           // 메모리 해제
        m_pDlgManager = nullptr;
    }
}

BEGIN_MESSAGE_MAP(CDay, CView)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDay 그리기

// CDay.cpp


void CDay::OnDraw(CDC* pDC)
{
    CCalendarAppDoc* pDoc = (CCalendarAppDoc*)GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc) return;

    CRect rcClient;
    GetClientRect(&rcClient);

    // 1. 배경 초기화
    pDC->FillSolidRect(&rcClient, RGB(255, 255, 255));

    if (pDoc->m_selDay == 0) {
        CString msg = _T("달력에서 날짜를 선택하세요.");
        CRect rcText = rcClient;
        pDC->DrawText(msg, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return;
    }

    // =========================================================
    // 1. 날짜 제목 (상대적 값으로 변경)
    // =========================================================
    CString strDate;
    strDate.Format(_T("%04d년 %02d월 %02d일"), pDoc->m_selYear, pDoc->m_selMonth, pDoc->m_selDay);

    CFont fontDate;
    // 폰트 설정 (200 포인트 -> 클라이언트 높이의 5%에 해당하는 포인트)
    // (예: 500px 높이 창 -> 25pt)
    fontDate.CreatePointFont((int)(rcClient.Height() * 0.5), _T("맑은 고딕"));
    CFont* pOldFont = pDC->SelectObject(&fontDate);
    pDC->SetTextColor(RGB(0, 0, 255));
    // 절대 좌표 (20, 10) -> 상대 좌표 (너비 2%, 높이 1%)
    pDC->TextOut((int)(rcClient.Width() * 0.02), (int)(rcClient.Height() * 0.01), strDate);
    pDC->SelectObject(pOldFont);


    // =========================================================
    // 2. 가로 타임라인 그리기 (상대적 값으로 변경)
    // =========================================================
    // int headerHeight = 60; // 절대값 -> 상대값 (클라이언트 높이의 8%)
    int headerHeight = (int)(rcClient.Height() * 0.1);

    // int timeStartX = 20; // 절대값 -> 상대값 (클라이언트 너비의 2%)
    // int timeEndX = rcClient.right - 20; // 절대값 -> 상대값 (클라이언트 너비의 98%)
    int timeStartX = (int)(rcClient.Width() * 0.02);
    int timeEndX = (int)(rcClient.Width() * 0.98);

    int timeTotalW = timeEndX - timeStartX;
    if (timeTotalW < (int)(rcClient.Width() * 0.2)) timeTotalW = (int)(rcClient.Width() * 0.2); // 최소 너비도 상대적으로

    double pxPerHour = (double)timeTotalW / 24.0;

    // 타임라인 위치/높이 (상대값)
    // int barTop = headerHeight + 100; -> headerHeight + 클라이언트 높이의 15%
    // int barBottom = barTop + 80; -> 바 높이 8%
    int barTop = headerHeight + (int)(rcClient.Height() * 0.3);
    int barHeight = (int)(rcClient.Height() * 0.3);
    int barBottom = barTop + barHeight;

    CPen penLine(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&penLine);
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);

    for (int h = 0; h <= 24; h += 6)
    {
        int x = timeStartX + (int)(h * pxPerHour);
        CString strTime;
        strTime.Format(_T("%d시"), h);

        // 시간 텍스트 출력 위치 (절대 좌표 -> 상대 좌표)
        // pDC->TextOut(x - 10, barTop - 25, strTime);
        pDC->TextOut(x - (int)(rcClient.Width() * 0.01), barTop - (int)(rcClient.Height() * 0.03), strTime);

        // 눈금선 길이 조정 (절대 좌표 -> 상대 좌표)
        // pDC->MoveTo(x, barTop - 5);
        // pDC->LineTo(x, barBottom + 5);
        pDC->MoveTo(x, barTop - (int)(rcClient.Height() * 0.005));
        pDC->LineTo(x, barBottom + (int)(rcClient.Height() * 0.005));
    }

    // =========================================================
    // 3. 일정 데이터 그리기 (가로)
    // =========================================================
    CBrush brushSch(RGB(255, 255, 0));
    CBrush* pOldBrush = pDC->SelectObject(&brushSch);
    CPen penBox(PS_SOLID, 3, RGB(0, 0, 0));
    pDC->SelectObject(&penBox);

    for (size_t i = 0; i < pDoc->m_scheduleList.size(); ++i)
    {
        ScheduleInfo& sch = pDoc->m_scheduleList[i];

        if (sch.year == pDoc->m_selYear &&
            sch.month == pDoc->m_selMonth &&
            sch.day == pDoc->m_selDay)
        {
            int x1 = timeStartX + (int)(sch.startHour * pxPerHour);
            int x2 = timeStartX + (int)(sch.endHour * pxPerHour);
            // 최소 너비 (20) -> 상대값 (타임라인 전체 너비의 3% 또는 20 중 큰 값)
            int minWidth = max((int)(timeTotalW * 0.03), 20);
            if (x2 - x1 < minWidth) x2 = x1 + minWidth;

            CRect rcSch(x1, barTop, x2, barBottom);
            pDC->Rectangle(rcSch);

            CString strText = sch.content;
            if (sch.isCompleted) strText = _T("[완료] ") + strText;
            pDC->DrawText(strText, rcSch, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}

void CDay::OnLButtonDown(UINT nFlags, CPoint point)
{
    CCalendarAppDoc* pDoc = (CCalendarAppDoc*)GetDocument();
    if (pDoc->m_selDay == 0) {
        AfxMessageBox(_T("날짜를 먼저 선택하세요.")); return;
    }

    // 1. 대화상자 생성
    if (m_pDlgManager == nullptr)
    {
        m_pDlgManager = new CAddDlg();
        m_pDlgManager->Create(IDD_DIALOG_ADD, this);
    }

    // 2. ★ 핵심: 그냥 "오늘 날짜 정보 가져가서 리스트 띄워라"고 명령
    m_pDlgManager->SetDay(pDoc);


    m_pDlgManager->ShowWindow(SW_SHOW);

    CView::OnLButtonDown(nFlags, point);
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
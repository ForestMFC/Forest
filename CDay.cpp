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

}

CDay::~CDay()
{
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
    // 1. 날짜 제목
    // =========================================================
    CString strDate;
    strDate.Format(_T("%04d년 %02d월 %02d일"), pDoc->m_selYear, pDoc->m_selMonth, pDoc->m_selDay);

    CFont fontDate;
    fontDate.CreatePointFont(200, _T("맑은 고딕"));
    CFont* pOldFont = pDC->SelectObject(&fontDate);
    pDC->SetTextColor(RGB(0, 0, 255));
    pDC->TextOut(20, 10, strDate);
    pDC->SelectObject(pOldFont);


    // =========================================================
    // 2. 가로 타임라인 그리기 (이전 설정 유지)
    // =========================================================
    int headerHeight = 60;
    int timeStartX = 20;
    int timeEndX = rcClient.right - 20;
    int timeTotalW = timeEndX - timeStartX;
    if (timeTotalW < 100) timeTotalW = 100;

    double pxPerHour = (double)timeTotalW / 24.0;

    // 이전에 설정하신 대로 높이 유지 (100)
    int barTop = headerHeight + 100;
    int barBottom = barTop + 80;

    CPen penLine(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&penLine);
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);

    for (int h = 0; h <= 24; h += 6)
    {
        int x = timeStartX + (int)(h * pxPerHour);
        CString strTime;
        strTime.Format(_T("%d시"), h);
        pDC->TextOut(x - 10, barTop - 25, strTime);
        pDC->MoveTo(x, barTop - 5);
        pDC->LineTo(x, barBottom + 5);
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
            if (x2 - x1 < 20) x2 = x1 + 20;

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

    // 0. 날짜 선택 안됐으면 리턴
    if (pDoc->m_selDay == 0) {
        AfxMessageBox(_T("먼저 달력에서 날짜를 선택해주세요."));
        return;
    }

    // 1. 좌표 계산 (OnDraw와 동일하게)
    CRect rcClient;
    GetClientRect(&rcClient);

    int headerHeight = 60;
    int timeStartX = 20;
    int timeEndX = rcClient.right - 20;
    int timeTotalW = timeEndX - timeStartX;
    if (timeTotalW < 100) timeTotalW = 100;

    double pxPerHour = (double)timeTotalW / 24.0;
    int barTop = headerHeight + 100;
    int barBottom = barTop + 80;

    // 타임라인 영역 밖을 클릭했으면 무시 (선택사항)
    // if (point.y < barTop || point.y > barBottom) return;


    // ---------------------------------------------------------
    // 2. 클릭한 곳에 '기존 일정'이 있는지 확인 (수정 모드)
    // ---------------------------------------------------------
    for (size_t i = 0; i < pDoc->m_scheduleList.size(); ++i)
    {
        ScheduleInfo& sch = pDoc->m_scheduleList[i];

        if (sch.year == pDoc->m_selYear &&
            sch.month == pDoc->m_selMonth &&
            sch.day == pDoc->m_selDay)
        {
            // 좌표 계산
            int x1 = timeStartX + (int)(sch.startHour * pxPerHour);
            int x2 = timeStartX + (int)(sch.endHour * pxPerHour);
            if (x2 - x1 < 20) x2 = x1 + 20;
            CRect rcSch(x1, barTop, x2, barBottom);

            // ★ 일정을 클릭했다! -> 수정/관리 창 열기
            if (rcSch.PtInRect(point))
            {
                CAddDlg dlg;
                // 기존 데이터 넣어주기 (TRUE = 수정 모드)
                dlg.SetData(sch.content, sch.startHour, sch.endHour, TRUE);

                if (dlg.DoModal() == IDOK)
                {
                    // 1) 삭제 요청 처리
                    if (dlg.m_bDeleteReq) {
                        pDoc->m_scheduleList.erase(pDoc->m_scheduleList.begin() + i);
                    }
                    // 2) 완료 요청 처리
                    else if (dlg.m_bCompleteReq) {
                        sch.isCompleted = true;
                        // 나무 키우기 메시지 전송
                        AfxGetMainWnd()->SendMessageToDescendants(WM_ADD_ONE_TREE, 0, 0, TRUE, TRUE);
                    }
                    // 3) 내용 수정 (시간, 내용 업데이트)
                    else {
                        sch.content = dlg.m_strContent;
                        sch.startHour = dlg.m_nStart;
                        sch.endHour = dlg.m_nEnd;
                    }
                    pDoc->UpdateAllViews(NULL);
                }
                return; // 처리 끝났으니 리턴
            }
        }
    }


    // ---------------------------------------------------------
    // 3. 일정이 없는 빈 공간 클릭 -> '새 일정 추가' 모드
    // ---------------------------------------------------------

    // 클릭한 X좌표를 시간으로 역계산 (대략 몇 시를 눌렀는지)
    int clickHour = 0;
    if (point.x > timeStartX) {
        clickHour = (int)((point.x - timeStartX) / pxPerHour);
    }
    if (clickHour < 0) clickHour = 0;
    if (clickHour > 23) clickHour = 23;

    // 대화상자 띄우기 (FALSE = 추가 모드)
    CAddDlg dlg;
    dlg.SetData(_T(""), clickHour, 0, FALSE);

    if (dlg.DoModal() == IDOK)
    {
        // 유효성 검사
        if (dlg.m_nStart < 0 || dlg.m_nStart > 23 || dlg.m_nEnd <= dlg.m_nStart) {
            AfxMessageBox(_T("시간 설정 오류")); return;
        }
        if (dlg.m_strContent.IsEmpty()) {
            AfxMessageBox(_T("내용을 입력하세요")); return;
        }

        // 새 일정 추가
        ScheduleInfo newSch(pDoc->m_selYear, pDoc->m_selMonth, pDoc->m_selDay,
            dlg.m_nStart, dlg.m_nEnd, dlg.m_strContent);

        pDoc->m_scheduleList.push_back(newSch);
        pDoc->UpdateAllViews(NULL);
    }
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

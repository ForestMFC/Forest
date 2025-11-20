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
    // 1) 날짜 제목
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
    // 2) 버튼 그리기  [+] 버튼만 남김
    // =========================================================
    int btnSize = 30;
    int topMargin = 10;
    int rightMargin = 20;

    // [+] 버튼을 오른쪽 끝으로 배치
    m_rcAdd = CRect(rcClient.right - rightMargin - btnSize, topMargin,
        rcClient.right - rightMargin, topMargin + btnSize);

    // 기존의 [-] 버튼(m_rcDel) 관련 코드는 삭제했습니다.

    pDC->Rectangle(m_rcAdd);
    pDC->DrawText(_T("+"), m_rcAdd, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


    // =========================================================
    // 3. 가로 타임라인 그리기 (이전 설정 유지)
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
    // 4. 일정 데이터 그리기 (가로)
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

    // ---------------------------------------------------------
    // 1. [+] 버튼 클릭: 대화상자 띄우기 (기존 코드 유지)
    // ---------------------------------------------------------
    if (m_rcAdd.PtInRect(point))
    {
        if (pDoc->m_selDay == 0) {
            AfxMessageBox(_T("먼저 달력에서 날짜를 선택해주세요."));
            return;
        }

        CAddDlg dlg;
        if (dlg.DoModal() == IDOK)
        {
            if (dlg.m_nStart < 0 || dlg.m_nStart > 23 || dlg.m_nEnd <= dlg.m_nStart || dlg.m_nEnd > 24) {
                AfxMessageBox(_T("시간 설정이 올바르지 않습니다."));
                return;
            }
            if (dlg.m_strContent.IsEmpty()) {
                AfxMessageBox(_T("일정 내용을 입력해주세요."));
                return;
            }

            ScheduleInfo newSch(pDoc->m_selYear, pDoc->m_selMonth, pDoc->m_selDay,
                dlg.m_nStart, dlg.m_nEnd, dlg.m_strContent);
            pDoc->m_scheduleList.push_back(newSch);
            pDoc->UpdateAllViews(NULL);
        }
        return;
    }

    // ([-] 버튼 클릭 로직은 삭제했습니다)


    // ---------------------------------------------------------
    // 2. 일정 박스 클릭 감지 및 처리 (완료/삭제/취소)
    // ---------------------------------------------------------
    CRect rcClient;
    GetClientRect(&rcClient);

    int headerHeight = 60;
    int timeStartX = 20;
    int timeEndX = rcClient.right - 20;
    int timeTotalW = timeEndX - timeStartX;
    if (timeTotalW < 100) timeTotalW = 100;
    double pxPerHour = (double)timeTotalW / 24.0;

    int barTop = headerHeight + 100; // 위치 맞춤
    int barBottom = barTop + 80;

    // 반복문을 돌면서 어떤 박스를 눌렀는지 확인
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

            // ★ 클릭된 일정을 찾았다!
            if (rcSch.PtInRect(point))
            {
                // 상세 내용 표시 문자열 생성
                CString msg;
                msg.Format(_T("[일정 상세 정보]\n\n내용: %s\n시간: %d시 ~ %d시\n상태: %s\n\n----------------------------\n이 일정을 어떻게 하시겠습니까?\n\n[예(Y)] -> 완료 처리\n[아니요(N)] -> 일정 삭제\n[취소(C)] -> 닫기"),
                    sch.content, sch.startHour, sch.endHour,
                    sch.isCompleted ? _T("완료됨") : _T("진행 중"));

                // 3가지 버튼이 있는 메시지 박스 띄우기 (Yes / No / Cancel)
                int nResult = AfxMessageBox(msg, MB_YESNOCANCEL | MB_ICONQUESTION);

                switch (nResult)
                {
                case IDYES: // [완료 처리]
                    sch.isCompleted = true;
                    AfxMessageBox(_T("일정이 완료 처리되었습니다."));

                    // ★★★ [수정] 아주 단순하게 신호만 보냅니다 ★★★
                    // 나무 하나 추가!" (인자값 필요 없음)
                    AfxGetMainWnd()->SendMessageToDescendants(WM_ADD_ONE_TREE, 0, 0, TRUE, TRUE);



                    break;

                    //여기서 나무데이터 확인

                case IDNO:  // [삭제]
                    // 벡터에서 해당 항목 삭제 (삭제 후 바로 리턴하여 인덱스 오류 방지)
                    pDoc->m_scheduleList.erase(pDoc->m_scheduleList.begin() + i);
                    AfxMessageBox(_T("일정이 삭제되었습니다."));
                    break;

                case IDCANCEL: // [취소]
                    // 아무것도 안 함
                    return;
                }

                // 데이터가 바뀌었으니 화면 갱신
                pDoc->UpdateAllViews(NULL);
                return; // 처리를 마쳤으니 함수 종료
            }
        }
    }

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

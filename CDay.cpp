// CDay.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "CDay.h"

#include "CalendarAppDoc.h" // Doc 접근용
#include "CInputDlg.h"      // 입력창 접근용 //구 dlg
#include "CScheduleDlg.h"   // 새 dlg
#include "CScheduleItem.h"  // 데이터 구조


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
	// 사용자 정의 메시지 (날짜 클릭 수신)
	ON_MESSAGE(WM_UPDATE_DATE_INFO, &CDay::OnUpdateDateInfo)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDay 그리기

void CDay::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	// TODO: 여기에 그리기 코드를 추가합니다.
	// (선택사항) 가독성을 위해 폰트를 설정합니다.

	CFont font;
	font.CreatePointFont(120, _T("맑은 고딕")); // 12pt 폰트
	CFont* pOldFont = pDC->SelectObject(&font);

	// 1. 날짜가 선택되었는지 확인
	if (m_nYear == 0)
	{
		// --- 날짜가 선택되지 않았을 때 ---
		pDC->TextOutW(10, 10, _T("달력에서 날짜를 선택해 주세요."));
	}
	else
	{
		// --- 날짜가 선택되었을 때 ---

		// 2. 선택된 날짜 정보 출력 (★ 한 번만 출력합니다)
		CString strDate;
		strDate.Format(_T("선택: %04d년 %02d월 %02d일"), m_nYear, m_nMonth, m_nDay);
		pDC->TextOutW(10, 10, strDate);

		// 3. 제목 출력
		pDC->SetTextColor(RGB(0, 0, 255)); // 파란색
		pDC->TextOut(10, 45, _T("[오늘의 할 일] (클릭하여 관리)"));
		pDC->SetTextColor(RGB(0, 0, 0));   // 검은색 복귀

		// 4. ★ [수정] 일정 목록 출력 (반복문)
		if (m_arrSchedule.IsEmpty())
		{
			pDC->TextOut(10, 75, _T("등록된 일정이 없습니다."));
		}
		else
		{
			int nY = 75; // 시작 Y 좌표
			for (int i = 0; i < m_arrSchedule.GetCount(); i++)
			{
				CScheduleItem& item = m_arrSchedule.GetAt(i);

				// 체크 상태에 따라 다르게 표시
				CString strOutput;
				if (item.m_bChecked)
					strOutput.Format(_T("[V] %s (완료)"), item.m_strText);
				else
					strOutput.Format(_T("[  ] %s"), item.m_strText);

				pDC->TextOut(10, nY, strOutput);
				nY += 25; // 다음 줄로 이동
			}
		}
	}

	// 폰트 복원
	pDC->SelectObject(pOldFont);
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


	// 2. ★ [수정] Doc에서 일정 '목록(Array)' 가져오기
	// (이전에는 GetSchedule이었지만 이제 GetSchedules 사용)
	CCalendarAppDoc* pDoc = (CCalendarAppDoc*)GetDocument();
	pDoc->GetSchedules(m_nYear, m_nMonth, m_nDay, m_arrSchedule);

	// 3. 화면 갱신
	Invalidate();

	return 0;
}

// ★ 화면 클릭 시 (일정 관리창 띄우기)
void CDay::OnLButtonDown(UINT nFlags, CPoint point)
{

	// 날짜가 선택된 상태인지 확인
	if (m_nYear == 0) return;

	// 1. ★ [수정] 일정 관리 대화상자(CScheduleDlg) 생성
	CScheduleDlg dlg;

	// 2. 현재 목록을 대화상자에 복사해 줌 (기존 데이터 유지)
	dlg.m_arrSchedule.Copy(m_arrSchedule);

	// 3. 대화상자 띄우기
	if (dlg.DoModal() == IDOK)
	{
		// 4. [확인]을 눌렀다면, 수정된 목록을 다시 가져옴
		m_arrSchedule.Copy(dlg.m_arrSchedule);

		// 5. ★ [수정] 변경된 목록을 Doc에 영구 저장
		// (이전에는 SetSchedule이었지만 이제 SetSchedules 사용)
		CCalendarAppDoc* pDoc = (CCalendarAppDoc*)GetDocument();
		pDoc->SetSchedules(m_nYear, m_nMonth, m_nDay, m_arrSchedule);

		// 6. 화면 갱신
		Invalidate();
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}

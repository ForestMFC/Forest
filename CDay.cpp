// CDay.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "CDay.h"

#include "CalendarAppDoc.h" // Doc 접근용
#include "CInputDlg.h"      // 입력창 접근용


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

		// 3. 일정 내용 출력 (Y좌표를 겹치지 않게 수정했습니다)
		pDC->SetTextColor(RGB(0, 0, 255));
		pDC->TextOut(10, 55, _T("[일정 내용]")); // Y좌표 40 -> 45

		pDC->SetTextColor(RGB(0, 0, 0));

		if (m_strSchedule.IsEmpty())
			pDC->TextOut(10, 100, _T("등록된 일정이 없습니다. (클릭하여 추가)")); // Y좌표 95 -> 75
		else
			pDC->TextOut(10, 100, m_strSchedule);
	}

	// 4. 원래 폰트로 복원
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


	// ★ [추가] Doc에서 저장된 일정 가져오기
	CCalendarAppDoc* pDoc = (CCalendarAppDoc*)GetDocument();
	m_strSchedule = pDoc->GetSchedule(m_nYear, m_nMonth, m_nDay);

	// 2. 화면 갱신 요청 (OnDraw 호출됨)
	Invalidate();

	return 0;
}


void CDay::OnLButtonDown(UINT nFlags, CPoint point)
{

	// 날짜가 선택된 상태인지 확인
	if (m_nYear == 0) return;

	// 1. 입력 다이얼로그 생성
	CInputDlg dlg;
	dlg.m_strValue = m_strSchedule; // 기존 일정 내용을 입력창에 채워둠

	// 2. 다이얼로그 띄우기 (IDOK는 '확인' 버튼 눌렀을 때)
	if (dlg.DoModal() == IDOK)
	{
		// 3. 입력받은 값을 내 변수에 저장
		m_strSchedule = dlg.m_strValue;

		// 4. ★ 중요! Doc(문서)에도 영구 저장
		CCalendarAppDoc* pDoc = (CCalendarAppDoc*)GetDocument();
		pDoc->SetSchedule(m_nYear, m_nMonth, m_nDay, m_strSchedule);

		// 5. 화면 다시 그리기
		Invalidate();
	}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}

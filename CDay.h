#pragma once
// ★★★ [중요] 이 헤더를 포함해야 CScheduleItemArray를 알 수 있습니다.
#include "CScheduleItem.h"

// CDay 보기

class CDay : public CView
{
	DECLARE_DYNCREATE(CDay)



protected:
	CDay();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CDay();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

// ★★★ [멤버 변수]
protected:
	int m_nYear;
	int m_nMonth;
	int m_nDay;

	// [수정] 기존 CString m_strSchedule 삭제하고 배열로 변경
	CScheduleItemArray m_arrSchedule;


// ★★★ [메시지 처리 함수]
protected:
	afx_msg LRESULT OnUpdateDateInfo(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};



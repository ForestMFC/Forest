#pragma once


// CDay 보기

class CDay : public CView
{
	DECLARE_DYNCREATE(CDay)


	// 멤버 변수 추가 (날짜 저장용)
protected:
	int m_nYear;
	int m_nMonth;
	int m_nDay;


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

	// ★ 메시지 처리 함수 선언
	afx_msg LRESULT OnUpdateDateInfo(WPARAM wParam, LPARAM lParam);
};



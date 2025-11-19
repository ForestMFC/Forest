#pragma once


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
	CRect m_rcAdd; // [+] 버튼
	CRect m_rcDel; // [-] 버튼

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};



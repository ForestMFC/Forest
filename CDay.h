// CDay.h
#pragma once
#include "CAddDlg.h" // 헤더 포함

class CDay : public CView
{
    DECLARE_DYNCREATE(CDay)
protected:
    CDay();
    virtual ~CDay();

public:
    virtual void OnDraw(CDC* pDC);

protected:
    CAddDlg* m_pDlgManager;

    virtual void OnDestroy();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    // ★ [추가] 이 부분이 빠져서 C2509 오류가 발생했습니다.
#ifdef _DEBUG
    virtual void AssertValid() const;
#ifndef _WIN32_WCE
    virtual void Dump(CDumpContext& dc) const;
#endif
#endif

    DECLARE_MESSAGE_MAP()
};
#pragma once

#include <atlimage.h>

class CTree : public CView
{
	DECLARE_DYNCREATE(CTree)

protected:
	CTree();
	virtual ~CTree();

public:
	int   m_nTreeCount;
	void SetTreeCount(int count);

protected:
	CImage m_imgTree;
	BOOL   m_bImageLoaded;

protected:
	virtual void OnInitialUpdate() override;
	virtual void OnDraw(CDC* pDC) override;

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

	DECLARE_MESSAGE_MAP()
	// ★ [추가] 신호 받을 함수
	afx_msg LRESULT OnAddOneTree(WPARAM wParam, LPARAM lParam);
};

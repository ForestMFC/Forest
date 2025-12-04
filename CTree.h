#pragma once
#include <atlimage.h> // 이미지 처리를 위해 필수

class CTree : public CView
{
	DECLARE_DYNCREATE(CTree)

protected:
	CTree();
	virtual ~CTree();

public:
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate(); // 이미지 로드

protected:
	DECLARE_MESSAGE_MAP()

	// ★★★ [변수] 상태 및 이미지 관리 ★★★
protected:
	int m_nLevel; // 현재 단계 (0 ~ 4)
	int m_nExp;   // 현재 경험치 (0 ~ 100)

	CImage m_imgBack;    // 배경 이미지 (ground.png)
	CImage m_imgTree[6]; // 단계별 나무 이미지 (level0 ~ level5)
	
	BOOL m_bImagesLoaded; // 로드 성공 여부
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	// 메시지 수신 함수
	afx_msg LRESULT OnAddOneTree(WPARAM wParam, LPARAM lParam);
};
#include "pch.h"
#include "framework.h"  
#include "CalendarApp.h"
#include "CTree.h"
#include <atlimage.h>


IMPLEMENT_DYNCREATE(CTree, CView)

CTree::CTree()
{
    m_nTreeCount = 5; // 테스트용. 실행하면 5그루 나옴.
    m_bImageLoaded = FALSE;
}

CTree::~CTree()
{
}

BEGIN_MESSAGE_MAP(CTree, CView)
END_MESSAGE_MAP()

// --------------------  추가되는 부분 --------------------

void CTree::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // 실행 폴더 기준 tree.png 로드
    HRESULT hr = m_imgTree.Load(L"tree.png");

    if (SUCCEEDED(hr))
    {
        m_bImageLoaded = TRUE;
    }
    else
    {
        m_bImageLoaded = FALSE;
        MessageBox(L"tree.png 를 불러오지 못했습니다.\n경로를 확인하세요.", L"이미지 로드 실패", MB_ICONERROR);
    }
}


// CTree 그리기
void CTree::OnDraw(CDC* pDC)
{
    CDocument* pDoc = GetDocument();
    UNREFERENCED_PARAMETER(pDoc);

    CRect rc;
    GetClientRect(rc);
    pDC->FillSolidRect(rc, RGB(255, 255, 255));

    if (!m_bImageLoaded || m_nTreeCount <= 0)
        return;

    int imgW = m_imgTree.GetWidth();
    int imgH = m_imgTree.GetHeight();

    double scale = 0.2;                // 여기 숫자 조절해서 크기 바꾸면 됨
    int drawW = (int)(imgW * scale);
    int drawH = (int)(imgH * scale);

    int margin = 20;
    int cols = 4;

    for (int i = 0; i < m_nTreeCount; i++)
    {
        int row = i / cols;
        int col = i % cols;

        int x = margin + col * (drawW + margin);
        int y = margin + row * (drawH + margin);

        m_imgTree.Draw(pDC->m_hDC, x, y, drawW, drawH);  // ← 축소된 크기
    }
}



// 외부에서 나무 개수 바꿀 때 호출할 함수
void CTree::SetTreeCount(int count)
{
    m_nTreeCount = count;
    Invalidate(); // 다시 그리기
}

#ifdef _DEBUG
void CTree::AssertValid() const
{
    CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTree::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif
#endif //_DEBUG

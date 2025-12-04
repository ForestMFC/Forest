#include "pch.h"
#include "framework.h"  
#include "CalendarApp.h"
#include "CTree.h"
#include <atlimage.h>


IMPLEMENT_DYNCREATE(CTree, CView)

CTree::CTree()
{
	m_nLevel = 0; // 레벨 0 (씨앗)부터 시작
	m_nExp = 0;   // 경험치 0
	m_bImagesLoaded = FALSE;
	m_nTreeCnt = 0;
}

CTree::~CTree()
{
}

BEGIN_MESSAGE_MAP(CTree, CView)
	ON_MESSAGE(WM_ADD_ONE_TREE, &CTree::OnAddOneTree)
END_MESSAGE_MAP()

// --------------------  추가되는 부분 --------------------

void CTree::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// 1. 배경 이미지 로드
	HRESULT hrBack = m_imgBack.Load(L"ground.png");

	// 2. 단계별 나무 이미지 로드 (level0.png ~ level5.png)
	// 파일 이름: level0(씨앗), level1(새싹)...
	HRESULT hrTree = S_OK;
	CString strFile;
	for (int i = 0; i < 6; i++)
	{
		strFile.Format(L"level %d.png", i);
		if (FAILED(m_imgTree[i].Load(strFile)))
		{
			hrTree = E_FAIL; // 하나라도 실패하면 실패 처리
		}
	}

	if (SUCCEEDED(hrBack) && SUCCEEDED(hrTree))
	{
		m_bImagesLoaded = TRUE;
	}
	else
	{
		MessageBox(L"이미지 파일(ground.png 또는 level0~5.png)을 찾을 수 없습니다.\n프로젝트 폴더를 확인하세요.", L"로딩 실패", MB_ICONERROR);
	}
}

// ★★★ [2] 경험치 증가 및 레벨업 로직 ★★★
LRESULT CTree::OnAddOneTree(WPARAM wParam, LPARAM lParam)
{
	// 1. 경험치 20 증가 (5번 하면 100됨)
	m_nExp += 20;

	// 2. 경험치가 꽉 찼나요?
	if (m_nExp >= 100)
	{
		m_nExp = 0; // 경험치 초기화
		m_nLevel++; // 다음 단계로 성장!

		// 3. 마지막 단계(4)를 넘어가면 다시 씨앗(0)으로 순환
		if (m_nLevel > 5)
		{
			m_nLevel = 0;
			// (선택사항) 한 바퀴 돌았다는 메시지를 띄울 수도 있음
			AfxMessageBox(L"축하합니다! 나무를 수확하고 새로운 씨앗을 심습니다.");
			m_nTreeCnt++;
		}
	}

	// 4. 화면 다시 그리기
	Invalidate();

	return 0;
}

// ★★★ [3] 화면 그리기 (상대적 값으로 변경) ★★★
void CTree::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	CRect rcClient;
	GetClientRect(rcClient);

	// 이미지가 없으면 흰 배경만 칠하고 끝냄
	if (!m_bImagesLoaded)
	{
		pDC->FillSolidRect(rcClient, RGB(255, 255, 255));
		pDC->TextOut(10, 10, L"이미지 로딩 실패");
		return;
	}

	// =========================================================
	// 1. 배경 그리기 (ground.png) - 화면 꽉 차게
	// =========================================================
	// SetStretchBltMode를 설정하면 이미지를 늘려도 품질이 덜 깨집니다.
	pDC->SetStretchBltMode(HALFTONE);
	m_imgBack.Draw(pDC->m_hDC, 0, 0, rcClient.Width(), rcClient.Height());


	// =========================================================
	// 2. 현재 단계의 나무 그리기 (가운데 정렬)
	// =========================================================
	// 현재 레벨 이미지 선택
	int idx = m_nLevel;
	if (idx < 0) idx = 0; if (idx > 5) idx = 5; // 안전장치

	// 원본 이미지 크기를 가져옵니다.
	int originalImgW = m_imgTree[idx].GetWidth();
	int originalImgH = m_imgTree[idx].GetHeight();

	// ★★★ 이미지 크기를 클라이언트 영역에 비례하여 조정 ★★★
	// 뷰 너비의 40% 또는 뷰 높이의 60%를 목표 크기로 설정합니다.
	double targetRatioW = 0.60; // 뷰 너비의 40%
	double targetRatioH = 0.80; // 뷰 높이의 60%

	int targetW = (int)(rcClient.Width() * targetRatioW);
	int targetH = (int)(rcClient.Height() * targetRatioH);

	// 원본 이미지의 비율을 유지하며 목표 크기에 맞도록 조정합니다.
	double ratioW = (double)targetW / originalImgW;
	double ratioH = (double)targetH / originalImgH;

	// 가로/세로 중 작은 비율을 선택하여 이미지 비율을 유지하고 영역 안에 맞춥니다.
	double scale = min(ratioW, ratioH);

	int imgW = (int)(originalImgW * scale);
	int imgH = (int)(originalImgH * scale);
	// ★★★ 이미지 크기 조정 끝 ★★★

	// 화면 중앙 하단 쪽에 배치 (좌표 계산)
	int x = (rcClient.Width() - imgW) / 2;

	// 상대적인 y 위치: 클라이언트 높이의 50% 지점부터 나무 이미지 높이의 절반 위
	int y = rcClient.Height() / 2 - imgH / 2;

	// (선택사항) 씨앗(level0)은 작으니까 좀 더 아래로 내리고 싶다면?
	// 상대적인 값으로 변경: 클라이언트 높이의 5% 정도 추가
	if (m_nLevel == 0) y += rcClient.Height() / 20;

	// Draw 함수에 조정된 크기(imgW, imgH)를 전달하여 이미지를 그립니다.
	m_imgTree[idx].Draw(pDC->m_hDC, x, y, imgW, imgH);


	// =========================================================
	// 3. 경험치 바 & 텍스트 그리기 (기존 상대적 값 유지)
	// =========================================================
	// 위치: 나무 아래쪽
	int barW = (int)(rcClient.Width() * 0.7); // 클라이언트 너비의 70%
	int barH = (int)(rcClient.Height() * 0.05);  // 클라이언트 높이의 5%

	int barX = (rcClient.Width() - barW) / 2;
	int barY = y + imgH + (int)(rcClient.Height() * 0.02); // 나무 이미지 아래 뷰 높이의 2% 추가

	CRect rcBar(barX, barY, barX + barW, barY + barH);

	// (1) 바 배경 (반투명 흰색 느낌)
	pDC->FillSolidRect(rcBar, RGB(200, 200, 200));
	pDC->FrameRect(rcBar, &CBrush(RGB(100, 100, 100))); // 테두리

	// (2) 경험치 채우기 (초록색)
	if (m_nExp > 0)
	{
		int fillW = (int)((double)barW * (m_nExp / 100.0));
		CRect rcFill = rcBar;
		rcFill.right = rcBar.left + fillW;
		rcFill.DeflateRect(1, 1);
		pDC->FillSolidRect(rcFill, RGB(0, 200, 0));
	}

	// (3) 텍스트 정보 (레벨 & 퍼센트)
	// 폰트 설정 (Level/EXP 표시 - 바 높이에 비례)
	CFont fontExp;
	fontExp.CreateFont(
		(int)(barH * 0.6),          // 폰트 높이 (바 높이의 60%)
		0, 0, 0,
		FW_BOLD,                    // 굵게
		FALSE, FALSE, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		L"맑은 고딕"
	);

	// 원래 폰트를 저장하고, 새로운 폰트 선택
	CFont* pOldFont = pDC->SelectObject(&fontExp);

	// 글자 배경을 투명하게 해서 배경 그림이 보이게 함
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0)); // 검은 글씨

	CString strInfo;
	strInfo.Format(L"Lv.%d  (EXP: %d%%)", m_nLevel, m_nExp);

	// 바 위에 글씨 쓰기 (레벨/경험치)
	pDC->DrawText(strInfo, rcBar, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	// =========================================================
	// 4. 수확한 나무 그루수 표시 (폰트 크기 증가) (상대적 값으로 변경)
	// =========================================================

	// 폰트 설정 (수확 개수 표시 - 클라이언트 높이에 비례)
	CFont fontHarvest;
	fontHarvest.CreateFont(
		(int)(rcClient.Height() * 0.05), // 폰트 높이 (클라이언트 높이의 5%)
		0, 0, 0,
		FW_HEAVY,                   // 매우 굵게
		FALSE, FALSE, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		L"맑은 고딕"
	);
	pDC->SelectObject(&fontHarvest); // 수확 개수 폰트로 변경

	pDC->SetBkMode(TRANSPARENT); // 배경 투명
	pDC->SetTextColor(RGB(0, 0, 0)); // 검은 글씨

	CString strHarvest;
	strHarvest.Format(L"🌳 수확한 나무: %d그루", m_nTreeCnt);

	// 화면 상단 중앙에 출력
	CRect rcTop(0, (int)(rcClient.Height() * 0.01), rcClient.Width(), (int)(rcClient.Height() * 0.1));
	pDC->DrawText(strHarvest, rcTop, DT_CENTER | DT_SINGLELINE);

	// 폰트 원상 복구 (가장 중요!)
	pDC->SelectObject(pOldFont);

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
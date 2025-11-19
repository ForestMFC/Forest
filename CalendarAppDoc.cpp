
// CalendarAppDoc.cpp: CCalendarAppDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "CalendarApp.h"
#endif

#include "CalendarAppDoc.h"

#include <propkey.h>

#include "CScheduleItem.h" // ★ 1단계에서 만든 헤더

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCalendarAppDoc

IMPLEMENT_DYNCREATE(CCalendarAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CCalendarAppDoc, CDocument)
END_MESSAGE_MAP()


// CCalendarAppDoc 생성/소멸

CCalendarAppDoc::CCalendarAppDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CCalendarAppDoc::~CCalendarAppDoc()
{
}

BOOL CCalendarAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CCalendarAppDoc serialization

void CCalendarAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CCalendarAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CCalendarAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCalendarAppDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCalendarAppDoc 진단

#ifdef _DEBUG
void CCalendarAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCalendarAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCalendarAppDoc 명령

// ★★★ [수정] 일정 저장 함수 (포인터 사용)
void CCalendarAppDoc::SetSchedules(int y, int m, int d, const CScheduleItemArray& arrContent)
{
	CString strKey;
	strKey.Format(_T("%04d%02d%02d"), y, m, d);

	CScheduleItemArray* pArray = NULL;

	// 1. 이미 해당 날짜에 데이터가 있는지 확인
	if (m_mapSchedules.Lookup(strKey, pArray))
	{
		// 있으면 기존 내용만 비웁니다. (메모리는 재사용)
		pArray->RemoveAll();
	}
	else
	{
		// 없으면 새로 메모리 할당(new) 하고 맵에 등록
		pArray = new CScheduleItemArray;
		m_mapSchedules.SetAt(strKey, pArray);
	}

	// 2. 데이터 복사 (Append 이용)
	pArray->Append(arrContent);

	SetModifiedFlag(TRUE);
}

// ★★★ [수정] 일정 가져오기 함수 (포인터 사용)
bool CCalendarAppDoc::GetSchedules(int y, int m, int d, CScheduleItemArray& outArrContent)
{
	// 일단 출력 변수 비우기
	outArrContent.RemoveAll();

	CString strKey;
	strKey.Format(_T("%04d%02d%02d"), y, m, d);

	CScheduleItemArray* pArray = NULL;

	// 맵에서 포인터를 찾음
	if (m_mapSchedules.Lookup(strKey, pArray))
	{
		// 포인터가 가리키는 내용을 복사해서 내보냄
		outArrContent.Append(*pArray);
		return true;
	}

	return false;
}


// ★★★ [추가] 메모리 해제 함수
// 프로그램 종료 시나 '새 파일' 시 메모리 누수 방지
void CCalendarAppDoc::DeleteContents()
{
	// 맵에 들어있는 모든 배열 포인터를 delete 해줘야 합니다.
	CString strKey;
	CScheduleItemArray* pArray = NULL;

	POSITION pos = m_mapSchedules.GetStartPosition();
	while (pos != NULL)
	{
		m_mapSchedules.GetNextAssoc(pos, strKey, pArray);
		if (pArray != NULL)
		{
			delete pArray; // ★ 중요: 힙 메모리 해제
		}
	}
	m_mapSchedules.RemoveAll(); // 맵 비우기

	CDocument::DeleteContents();
}

// CalendarAppDoc.h: CCalendarAppDoc 클래스의 인터페이스
//


#pragma once

#include "CScheduleItem.h" // ★ 1단계에서 만든 헤더
#include <afxtempl.h>     // CArray, CMap 사용



class CCalendarAppDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CCalendarAppDoc() noexcept;
	DECLARE_DYNCREATE(CCalendarAppDoc)

// ★★★ [추가] 일정 저장용 변수와 함수 선언 ★★★
protected:
	// Key: 날짜문자열, Value: 배열의 주소(포인터)
	// 3번째, 4번째 인자가 포인터(*)로 바뀐 것을 꼭 확인하세요!
	CMap<CString, LPCTSTR, CScheduleItemArray*, CScheduleItemArray*> m_mapSchedules;


// 특성입니다.
public:
	// ★ [수정] 함수들의 인자 타입을 CScheduleItemArray로 변경
	void SetSchedules(int y, int m, int d, const CScheduleItemArray& arrContent);
	bool GetSchedules(int y, int m, int d, CScheduleItemArray& outArrContent);


	// ★★★ [추가] 메모리 해제 함수 (필수!) ★★★
	virtual void DeleteContents();

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CCalendarAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

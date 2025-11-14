
// CalendarAppDoc.h: CCalendarAppDoc 클래스의 인터페이스
//


#pragma once


class CCalendarAppDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CCalendarAppDoc() noexcept;
	DECLARE_DYNCREATE(CCalendarAppDoc)

// ★★★ [추가] 일정 저장용 변수와 함수 선언 ★★★
protected:
	// Key: "20251114", Value: "오후 2시 미팅"
	CMapStringToString m_mapSchedule;

// 특성입니다.
public:
	// 일정을 저장하는 함수
	void SetSchedule(int y, int m, int d, CString strContent);

	// 일정을 가져오는 함수
	CString GetSchedule(int y, int m, int d);

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

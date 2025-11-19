
// CalendarAppDoc.h: CCalendarAppDoc 클래스의 인터페이스
//
#pragma once
#include <vector>

//일정(Schedule) 정보를 담을 구조체 정의
struct ScheduleInfo {
	int year, month, day;   // 날짜
	int startHour;          // 시작 시간 (0~23)
	int endHour;            // 종료 시간 (1~24)
	CString content;        // 할 일 내용
	bool isCompleted;       // 완료 여부

	// 생성자 (초기화)
	ScheduleInfo(int y, int m, int d, int sh, int eh, CString txt)
	{
		year = y; month = m; day = d;
		startHour = sh; endHour = eh;
		content = txt;
		isCompleted = false;
	}
};

class CCalendarAppDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CCalendarAppDoc() noexcept;
	DECLARE_DYNCREATE(CCalendarAppDoc)

// 특성입니다.
public:
	int m_selYear;
	int m_selMonth;
	int m_selDay;

	std::vector<ScheduleInfo> m_scheduleList;

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

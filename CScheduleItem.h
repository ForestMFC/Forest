#pragma once
#include <afx.h>
class CScheduleItem :
    public CObject
{
public:
    DECLARE_SERIAL(CScheduleItem) // CArray에서 사용하기 위해 필요

        CString m_strText; // 일정 내용
    BOOL m_bChecked;   // 체크 상태 (TRUE / FALSE)

    // 생성자
    CScheduleItem() : m_strText(_T("")), m_bChecked(FALSE) {}
    // 편의용 생성자
    CScheduleItem(CString text, BOOL checked) : m_strText(text), m_bChecked(checked) {}


    // ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
    // [추가] 1. 복사 생성자 (Copy Constructor)
    // 다른 CScheduleItem을 복사해서 새로 만들 때 호출됨
    // ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
    CScheduleItem(const CScheduleItem& src)
    {
        m_strText = src.m_strText;
        m_bChecked = src.m_bChecked;
    }

    // ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
    // [추가] 2. 대입 연산자 (Assignment Operator)
    // 이미 있는 객체에 다른 객체의 값을 덮어쓸 때(=) 호출됨
    // ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
    const CScheduleItem& operator=(const CScheduleItem& src)
    {
        m_strText = src.m_strText;
        m_bChecked = src.m_bChecked;
        return *this;
    }
    // CArray에서 사용하기 위해 Serialize 함수가 필요합니다.
    void Serialize(CArchive& ar)
    {
        CObject::Serialize(ar);
        if (ar.IsStoring())
        {
            ar << m_strText;
            ar << m_bChecked;
        }
        else
        {
            ar >> m_strText;
            ar >> m_bChecked;
        }
    }
};

// ★ CArray의 새 타입을 미리 정의해 둡니다. (매우 유용함)
typedef CArray<CScheduleItem, CScheduleItem&> CScheduleItemArray;
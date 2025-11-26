// CAddDlg.h
#pragma once
#include "afxdialogex.h"

struct ScheduleInfo;
class CCalendarAppDoc;

class CAddDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CAddDlg)

public:
    CAddDlg(CWnd* pParent = nullptr);
    virtual ~CAddDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnCancel(); // 숨기기 기능

    DECLARE_MESSAGE_MAP()

public:
    // UI 컨트롤 변수
    CListBox m_listSchedule; // ★ [추가] 일정 목록 리스트

    // 값 변수
    CString m_strContent;
    CString m_strMemo;
    int m_nStart;
    int m_nEnd;

    // 데이터 포인터
    CCalendarAppDoc* m_pDoc;
    ScheduleInfo* m_pCurrentSch; // 현재 선택된(수정 중인) 일정. NULL이면 새 일정.

    // 기능 함수
    void SetDay(CCalendarAppDoc* pDoc); // ★ 날짜만 받으면 리스트를 쫙 뿌려주는 함수
    void RefreshList();                 // 리스트 새로고침
    void ClearInputFields();            // 입력칸 비우기

    // 이벤트 핸들러
    afx_msg void OnLbnSelchangeListSchedules(); // ★ 리스트 클릭 시
    afx_msg void OnBnClickedButtonNew();        // [새로 만들기] 버튼
    afx_msg void OnBnClickedButtonSave();       // [저장]
    afx_msg void OnBnClickedButtonDelete();     // [삭제]
    afx_msg void OnBnClickedButtonComplete();   // [완료]
    virtual BOOL OnInitDialog();
};
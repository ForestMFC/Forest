#pragma once
#include "afxdialogex.h"

#include "CScheduleItem.h" // 1단계에서 만든 헤더


// CScheduleDlg 대화 상자

class CScheduleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScheduleDlg)

public:
	CScheduleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScheduleDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEDULE_LIST };
#endif

// ★★★ [변수] 외부와 데이터를 주고받을 배열
public:
	CScheduleItemArray m_arrSchedule;

// ★★★ [변수] 화면 컨트롤과 연결될 변수들
protected:
	CListCtrl m_ctrlListSchedules; // 리스트 컨트롤 (체크박스 포함)
	CEdit m_ctrlEditNewItem;

// ★★★ [함수] MFC 기본 오버라이드
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog(); // 초기화 (컬럼 추가 등)
	virtual void OnOK();         // 확인 버튼 (데이터 저장)

// ★★★ [함수] 버튼 및 클릭 이벤트 처리기
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdd();    // 추가 버튼
	afx_msg void OnBnClickedButtonRemove(); // 삭제 버튼
	// 리스트 클릭 (체크박스 변경 감지용)
	afx_msg void OnNMClickListSchedules(NMHDR* pNMHDR, LRESULT* pResult);


};

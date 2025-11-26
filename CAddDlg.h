#pragma once
#include "afxdialogex.h"

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = nullptr);
	virtual ~CAddDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	// 입력 컨트롤 변수들
	CString m_strContent;
	int m_nStart;
	int m_nEnd;

	// ★ [추가] 상태 관리용 변수
	BOOL m_bEditMode;     // TRUE면 수정 모드, FALSE면 새 추가
	BOOL m_bDeleteReq;    // 삭제 요청 여부
	BOOL m_bCompleteReq;  // 완료 요청 여부

	// 초기화 함수 (뷰에서 호출)
	void SetData(CString content, int start, int end, BOOL bEdit);

	// 버튼 이벤트 핸들러
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonComplete();
	virtual BOOL OnInitDialog(); // 초기화 때 버튼 숨기기/보이기
};
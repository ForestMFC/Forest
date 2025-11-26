#include "pch.h"
#include "CalendarApp.h"
#include "afxdialogex.h"
#include "CAddDlg.h"

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD, pParent)
	, m_strContent(_T(""))
	, m_nStart(0)
	, m_nEnd(0)
{
	// 기본값 초기화
	m_bEditMode = FALSE;
	m_bDeleteReq = FALSE;
	m_bCompleteReq = FALSE;
}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_strContent);
	DDX_Text(pDX, IDC_EDIT_START, m_nStart);
	DDX_Text(pDX, IDC_EDIT_END, m_nEnd);
}

BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CAddDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_COMPLETE, &CAddDlg::OnBnClickedButtonComplete)
END_MESSAGE_MAP()


// ★ [추가] 외부(View)에서 데이터를 넣어주는 함수
void CAddDlg::SetData(CString content, int start, int end, BOOL bEdit)
{
	m_strContent = content;
	m_nStart = start;
	m_nEnd = end;
	m_bEditMode = bEdit;
}

// ★ [추가] 대화상자가 켜질 때 화면 세팅
BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 수정 모드가 아니면(새 추가면) 삭제/완료 버튼 숨기기
	if (!m_bEditMode)
	{
		GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_COMPLETE)->ShowWindow(SW_HIDE);
		SetWindowText(_T("일정 추가")); // 창 제목 변경
	}
	else
	{
		SetWindowText(_T("일정 관리")); // 창 제목 변경
	}

	return TRUE;
}

// ★ [추가] 삭제 버튼 클릭 시
void CAddDlg::OnBnClickedButtonDelete()
{
	if (AfxMessageBox(_T("정말 이 일정을 삭제하시겠습니까?"), MB_YESNO) == IDYES)
	{
		m_bDeleteReq = TRUE;
		EndDialog(IDOK); // OK를 리턴하며 창 닫기 (처리는 View에서 함)
	}
}

// ★ [추가] 완료 버튼 클릭 시
void CAddDlg::OnBnClickedButtonComplete()
{
	m_bCompleteReq = TRUE;
	AfxMessageBox(_T("고생하셨습니다! 나무가 자랍니다."));
	EndDialog(IDOK); // OK를 리턴하며 창 닫기
}
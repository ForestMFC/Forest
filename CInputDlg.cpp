// CInputDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "afxdialogex.h"
#include "CInputDlg.h"


// CInputDlg 대화 상자

IMPLEMENT_DYNAMIC(CInputDlg, CDialogEx)

CInputDlg::CInputDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INPUT, pParent)
{

}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// ★ Edit Control(IDC_EDIT_SCHEDULE)과 변수(m_strValue)를 연결
	DDX_Text(pDX, IDC_EDIT_SCHEDULE, m_strValue);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
END_MESSAGE_MAP()


// CInputDlg 메시지 처리기

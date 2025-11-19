// CAddDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "afxdialogex.h"
#include "CAddDlg.h"


// CAddDlg 대화 상자

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD, pParent)
	, m_strContent(_T(""))
	, m_nStart(0)
	, m_nEnd(0)
{
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
END_MESSAGE_MAP()


// CAddDlg 메시지 처리기

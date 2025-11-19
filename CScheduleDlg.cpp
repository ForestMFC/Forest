// CScheduleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CalendarApp.h"
#include "afxdialogex.h"
#include "CScheduleDlg.h"

// ★★★ [중요] 데이터 구조를 알기 위해 헤더 포함 ★★★
#include "CScheduleItem.h"


// CScheduleDlg 대화 상자

IMPLEMENT_DYNAMIC(CScheduleDlg, CDialogEx)

CScheduleDlg::CScheduleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCHEDULE_LIST, pParent)
{

}

CScheduleDlg::~CScheduleDlg()
{
}

// ★★★ 1. 화면 컨트롤과 변수 연결 (DoDataExchange) ★★★
void CScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// 리스트 컨트롤 연결
	DDX_Control(pDX, IDC_LIST_SCHEDULES, m_ctrlListSchedules);
	// 입력창 연결
	DDX_Control(pDX, IDC_EDIT_NEW_ITEM, m_ctrlEditNewItem);
}


// ★★★ 2. 메시지 맵 (버튼 클릭 연결) ★★★
BEGIN_MESSAGE_MAP(CScheduleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CScheduleDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CScheduleDlg::OnBnClickedButtonRemove)
	// 리스트 클릭 이벤트 (체크박스 감지용)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCHEDULES, &CScheduleDlg::OnNMClickListSchedules)
END_MESSAGE_MAP()



// CScheduleDlg 메시지 처리기

// ★★★ 3. 대화상자가 처음 뜰 때 (초기화) ★★★
BOOL CScheduleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// (1) 리스트 컨트롤 스타일 설정 (체크박스 + 한 줄 전체 선택)
	m_ctrlListSchedules.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	// (2) 컬럼(열) 추가 (Report 뷰에서는 컬럼이 있어야 데이터가 보임)
	//     "일정 내용"이라는 제목으로 너비 300px 컬럼 추가
	m_ctrlListSchedules.InsertColumn(0, _T("일정 내용"), LVCFMT_LEFT, 300);

	// (3) CDay로부터 넘겨받은 데이터(m_arrSchedule)를 화면에 뿌리기
	for (int i = 0; i < m_arrSchedule.GetCount(); i++)
	{
		CScheduleItem& item = m_arrSchedule.GetAt(i);

		// 텍스트 추가
		int nIndex = m_ctrlListSchedules.InsertItem(i, item.m_strText);

		// 체크 상태 설정 (TRUE/FALSE)
		m_ctrlListSchedules.SetCheck(nIndex, item.m_bChecked);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}



// ★★★ 4. [추가] 버튼 클릭 시 ★★★
void CScheduleDlg::OnBnClickedButtonAdd()
{
	CString strItem;
	m_ctrlEditNewItem.GetWindowText(strItem); // 입력창 내용 가져오기

	if (!strItem.IsEmpty())
	{
		// 리스트 맨 끝에 아이템 추가
		int nCount = m_ctrlListSchedules.GetItemCount();
		int nIndex = m_ctrlListSchedules.InsertItem(nCount, strItem);

		// 기본적으로 체크 해제 상태
		m_ctrlListSchedules.SetCheck(nIndex, FALSE);

		// 입력창 비우고 포커스 주기
		m_ctrlEditNewItem.SetWindowText(_T(""));
		m_ctrlEditNewItem.SetFocus();
	}
}


// ★★★ 5. [삭제] 버튼 클릭 시 ★★★
void CScheduleDlg::OnBnClickedButtonRemove()
{
	// 현재 선택된 항목(파란색)의 위치를 찾음
	POSITION pos = m_ctrlListSchedules.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("삭제할 항목을 선택해주세요."));
		return;
	}

	// 선택된 항목의 인덱스를 얻어서 삭제
	int nItem = m_ctrlListSchedules.GetNextSelectedItem(pos);
	m_ctrlListSchedules.DeleteItem(nItem);
}



// ★★★ 6. [확인(OK)] 버튼 클릭 시 (데이터 저장) ★★★
void CScheduleDlg::OnOK()
{
	// 화면에 있는 내용을 m_arrSchedule 변수에 업데이트합니다.

	m_arrSchedule.RemoveAll(); // 기존 내용 비우기

	int nTotal = m_ctrlListSchedules.GetItemCount();
	for (int i = 0; i < nTotal; i++)
	{
		// 텍스트 가져오기
		CString strText = m_ctrlListSchedules.GetItemText(i, 0);
		// 체크 상태 가져오기
		BOOL bChecked = m_ctrlListSchedules.GetCheck(i);

		// 배열에 추가 (CScheduleItem 생성자 사용)
		m_arrSchedule.Add(CScheduleItem(strText, bChecked));
	}

	// 부모 클래스의 OnOK를 호출하여 창을 닫습니다.
	CDialogEx::OnOK();
}


// ★★★ 7. 리스트 클릭 시 (체크박스 변경 감지) ★★★
void CScheduleDlg::OnNMClickListSchedules(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// 클릭한 곳이 상태(체크박스) 영역인지 확인
	if (pNMItemActivate->uChanged & LVIF_STATE)
	{
		// 체크 상태가 변경되었는지 확인 (State Image Mask 체크)
		// (사용자가 체크박스를 눌렀을 때만 반응하도록)
		/*
		  [참고] CListCtrl의 체크박스 클릭 감지는 꽤 까다롭습니다.
		  여기서는 단순히 아이템을 클릭했을 때 체크 상태를 확인하는 로직을 넣습니다.
		  정밀한 제어를 위해서는 LVN_ITEMCHANGED 이벤트를 쓰는 것이 더 정확할 수 있으나,
		  지금 단계에서는 NM_CLICK으로 클릭된 아이템 정보를 확인하는 것으로 충분합니다.
		*/

		int nItem = pNMItemActivate->iItem;
		if (nItem != -1)
		{
			// 현재 체크 상태 읽기 (클릭 직후의 상태가 아닐 수 있으므로 주의)
			// 여기서는 예시로 메시지만 띄웁니다.
			// CString str = m_ctrlListSchedules.GetItemText(nItem, 0);
			// AfxMessageBox(str + _T(" 항목을 클릭했습니다."));
		}
	}

	*pResult = 0;
}

 
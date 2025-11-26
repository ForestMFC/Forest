// CAddDlg.cpp
#include "pch.h"
#include "CalendarApp.h"
#include "CAddDlg.h"
#include "CalendarAppDoc.h"

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_ADD, pParent)
    , m_strContent(_T(""))
    , m_strMemo(_T(""))
    , m_nStart(0)
    , m_nEnd(0)
{
    m_pDoc = nullptr;
    m_pCurrentSch = nullptr;
}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // ★ 리스트 박스 연결 (리소스에 IDC_LIST_SCHEDULES가 있어야 함)
    DDX_Control(pDX, IDC_LIST_SCHEDULES, m_listSchedule);
    
    DDX_Text(pDX, IDC_EDIT_CONTENT, m_strContent);
    DDX_Text(pDX, IDC_EDIT_START, m_nStart);
    DDX_Text(pDX, IDC_EDIT_END, m_nEnd);
    DDX_Text(pDX, IDC_EDIT_MEMO, m_strMemo);
}

BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
    ON_LBN_SELCHANGE(IDC_LIST_SCHEDULES, &CAddDlg::OnLbnSelchangeListSchedules) // 리스트 선택
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CAddDlg::OnBnClickedButtonSave) // 저장 버튼 (IDOK 가정)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CAddDlg::OnBnClickedButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON_COMPLETE, &CAddDlg::OnBnClickedButtonComplete)
    // [새로 만들기] 버튼이 있다면 추가: ON_BN_CLICKED(IDC_BUTTON_NEW, &CAddDlg::OnBnClickedButtonNew)
END_MESSAGE_MAP()

BOOL CAddDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    return TRUE;
}

// 1. 뷰에서 호출: "오늘 날짜의 데이터를 보여줘"
void CAddDlg::SetDay(CCalendarAppDoc* pDoc)
{
    m_pDoc = pDoc;
    RefreshList();      // 리스트 채우기
    ClearInputFields(); // 입력칸은 비워두기 (새 일정 준비)
    
    // 제목 설정
    if(pDoc) {
        CString title;
        title.Format(_T("%d년 %d월 %d일 일정 관리"), pDoc->m_selYear, pDoc->m_selMonth, pDoc->m_selDay);
        SetWindowText(title);
    }
}

// 2. 리스트 박스 새로고침 (핵심!)
void CAddDlg::RefreshList()
{
    if (!m_pDoc) return;

    m_listSchedule.ResetContent(); // 기존 목록 싹 지움

    for (size_t i = 0; i < m_pDoc->m_scheduleList.size(); ++i)
    {
        ScheduleInfo& sch = m_pDoc->m_scheduleList[i];

        // 선택된 날짜의 일정만 골라냄
        if (sch.year == m_pDoc->m_selYear && 
            sch.month == m_pDoc->m_selMonth && 
            sch.day == m_pDoc->m_selDay)
        {
            // 리스트에 보여줄 문자열 만들기 (예: "10시~12시: 팀 회의")
            CString strItem;
            strItem.Format(_T("[%02d:00 ~ %02d:00] %s"), sch.startHour, sch.endHour, sch.content);
            if (sch.isCompleted) strItem = _T("[완료] ") + strItem;

            // 리스트에 추가
            int idx = m_listSchedule.AddString(strItem);

            // ★ 중요: 리스트 아이템에 실제 데이터의 주소(포인터)를 숨겨둠
            m_listSchedule.SetItemDataPtr(idx, &m_pDoc->m_scheduleList[i]);
        }
    }
}

// 3. 리스트에서 항목을 클릭했을 때 -> 상세 내용 보여주기
void CAddDlg::OnLbnSelchangeListSchedules()
{
    int idx = m_listSchedule.GetCurSel();
    if (idx != LB_ERR)
    {
        // 숨겨둔 포인터를 꺼냄
        m_pCurrentSch = (ScheduleInfo*)m_listSchedule.GetItemDataPtr(idx);

        // 화면에 뿌리기
        m_strContent = m_pCurrentSch->content;
        m_strMemo = m_pCurrentSch->memo;
        m_nStart = m_pCurrentSch->startHour;
        m_nEnd = m_pCurrentSch->endHour;

        UpdateData(FALSE); // 변수 -> 화면
        
        // 버튼 활성화
        GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_COMPLETE)->EnableWindow(TRUE);
    }
}

// 4. [새로 만들기] 버튼 (입력칸 초기화)
void CAddDlg::OnBnClickedButtonNew()
{
    ClearInputFields();
    // 리스트 선택 해제
    m_listSchedule.SetCurSel(-1); 
}

void CAddDlg::ClearInputFields()
{
    m_pCurrentSch = nullptr; // 선택된 것 없음
    m_strContent = _T("");
    m_strMemo = _T("");
    m_nStart = 0;
    m_nEnd = 0;
    
    UpdateData(FALSE);
    
    // 삭제/완료 버튼 비활성화 (새 항목이니까)
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_COMPLETE)->EnableWindow(FALSE);
}

// 5. [저장] 버튼 (수정 또는 추가)
void CAddDlg::OnBnClickedButtonSave()
{
    UpdateData(TRUE); // 화면 -> 변수

    // 유효성 검사 생략 (이전과 동일)
    if (!m_pDoc) return;

    if (m_pCurrentSch != nullptr)
    {
        // === 수정 모드 ===
        m_pCurrentSch->content = m_strContent;
        m_pCurrentSch->memo = m_strMemo;
        m_pCurrentSch->startHour = m_nStart;
        m_pCurrentSch->endHour = m_nEnd;
        AfxMessageBox(_T("수정되었습니다."));
    }
    else
    {
        // === 새 일정 추가 모드 ===
        if (m_pDoc->m_selDay == 0) return;

        ScheduleInfo newSch(m_pDoc->m_selYear, m_pDoc->m_selMonth, m_pDoc->m_selDay,
            m_nStart, m_nEnd, m_strContent, m_strMemo);
        
        m_pDoc->m_scheduleList.push_back(newSch);
        AfxMessageBox(_T("추가되었습니다."));
        ClearInputFields(); // 저장 후 초기화
    }

    // 리스트 새로고침 (중요!)
    RefreshList();
    
    // 뷰(타임라인)도 갱신
    m_pDoc->UpdateAllViews(NULL);
}

// 6. 삭제 버튼
void CAddDlg::OnBnClickedButtonDelete()
{
    if (m_pCurrentSch && m_pDoc)
    {
        if (AfxMessageBox(_T("정말 삭제하시겠습니까?"), MB_YESNO) == IDYES)
        {
            // 포인터로 찾아서 삭제
            auto& list = m_pDoc->m_scheduleList;
            for (auto it = list.begin(); it != list.end(); ++it)
            {
                if (&(*it) == m_pCurrentSch)
                {
                    list.erase(it);
                    break;
                }
            }
            ClearInputFields();
            RefreshList();
            m_pDoc->UpdateAllViews(NULL);
        }
    }
}

// 7. 완료 버튼
void CAddDlg::OnBnClickedButtonComplete()
{
    if (m_pCurrentSch)
    {
        m_pCurrentSch->isCompleted = true;
        AfxGetMainWnd()->SendMessageToDescendants(WM_ADD_ONE_TREE, 0, 0, TRUE, TRUE);
        
        RefreshList(); // [완료] 표시 갱신을 위해
        m_pDoc->UpdateAllViews(NULL);
    }
}

void CAddDlg::OnCancel() { ShowWindow(SW_HIDE); }
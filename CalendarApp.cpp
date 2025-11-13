
// CalendarApp.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CalendarApp.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "CalendarAppDoc.h"
#include "CalendarAppView.h"
#include "CDay.h"
#include "CTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCalendarAppApp

BEGIN_MESSAGE_MAP(CCalendarAppApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCalendarAppApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCalendarAppApp 생성

CCalendarAppApp::CCalendarAppApp() noexcept
{

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 애플리케이션을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CalendarApp.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CCalendarAppApp 개체입니다.

CCalendarAppApp theApp;


// CCalendarAppApp 초기화

BOOL CCalendarAppApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	// 애플리케이션의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CMultiDocTemplate* pDocTemplate1;
	CMultiDocTemplate* pDocTemplate2;
	CMultiDocTemplate* pDocTemplate3;

	// 템플릿 1: CCalenderAppView (기존 뷰)
	pDocTemplate1 = new CMultiDocTemplate(IDR_CalendarAppTYPE, // 기존 ID
		RUNTIME_CLASS(CCalendarAppDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CCalendarAppView)); // <- 역할 1 뷰
	if (!pDocTemplate1)
		return FALSE;
	AddDocTemplate(pDocTemplate1);

	// 템플릿 2: CDay (새 뷰)
	pDocTemplate2 = new CMultiDocTemplate(IDR_DayTYPE, // ★1단계에서 만든 새 ID 1
		RUNTIME_CLASS(CCalendarAppDoc),   // 문서는 동일하게 공유
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CDay));          // <- 역할 2 뷰
	if (!pDocTemplate2)
		return FALSE;
	AddDocTemplate(pDocTemplate2);

	// 템플릿 3: CTree (새 뷰)
	pDocTemplate3 = new CMultiDocTemplate(IDR_TreeTYPE, // ★1단계에서 만든 새 ID 2
		RUNTIME_CLASS(CCalendarAppDoc),   // 문서는 동일하게 공유
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CTree));      // <- 역할 3 뷰
	if (!pDocTemplate3)
		return FALSE;
	AddDocTemplate(pDocTemplate3);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 1. 주 창을 먼저 화면에 표시합니다.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// 2. 프로그램이 아이콘으로 실행되었을 경우 (새 파일 모드)
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		// --- 1. 3개의 창을 생성하고 각 창의 CChildFrame 포인터를 얻어옵니다. ---

		CDocument* pDoc1 = pDocTemplate1->OpenDocumentFile(NULL); // 역할1 (오른쪽)
		POSITION pos1 = pDoc1->GetFirstViewPosition();
		CView* pView1 = pDoc1->GetNextView(pos1);
		CChildFrame* pChild1 = (CChildFrame*)pView1->GetParentFrame();

		CDocument* pDoc2 = pDocTemplate2->OpenDocumentFile(NULL); // 역할2 (왼쪽 위)
		POSITION pos2 = pDoc2->GetFirstViewPosition();
		CView* pView2 = pDoc2->GetNextView(pos2);
		CChildFrame* pChild2 = (CChildFrame*)pView2->GetParentFrame();

		CDocument* pDoc3 = pDocTemplate3->OpenDocumentFile(NULL); // 역할3 (왼쪽 아래)
		POSITION pos3 = pDoc3->GetFirstViewPosition();
		CView* pView3 = pDoc3->GetNextView(pos3);
		CChildFrame* pChild3 = (CChildFrame*)pView3->GetParentFrame();

		// --- 2. 메인 프레임의 MDI 클라이언트 영역 크기를 얻어옵니다. ---
		// (툴바, 상태바를 제외한 실제 MDI 자식 창이 표시되는 회색 영역)
		CRect rectMDIClient;
		CWnd* pMDIClient = CWnd::FromHandle(pMainFrame->m_hWndMDIClient);
		pMDIClient->GetClientRect(&rectMDIClient);

		int width = rectMDIClient.Width();
		int height = rectMDIClient.Height();

		// --- 3. 이미지 레이아웃에 맞춰 3개의 창 위치/크기를 수동 계산합니다. ---
		// (비율은 6:4, 5:5 등으로 원하시는 대로 조절하세요)

		// 창 1 (오른쪽 40% 영역, 전체 높이)
		int nLeft1 = 0;
		int nTop1 = height * 0.4; // 50% 지점에서 시작
		int nWidth1 = width * 0.6; // 60% 너비
		int nHeight1 = height * 0.7; // 50% 높이
		pChild1->MoveWindow(nLeft1, nTop1, nWidth1, nHeight1);

		// 창 2 (왼쪽 60% 영역, 위쪽 50% 높이)
		int nLeft2 = 0;
		int nTop2 = 0;
		int nWidth2 = width * 0.6; // 60% 너비
		int nHeight2 = height * 0.4; // 50% 높이
		pChild2->MoveWindow(nLeft2, nTop2, nWidth2, nHeight2);

		// 창 3 (왼쪽 60% 영역, 아래쪽 50% 높이)
		int nLeft3 = width * 0.6; // 60% 지점에서 시작
		int nTop3 = 0;
		int nWidth3 = width * 0.4; // 40% 너비
		int nHeight3 = height;
		pChild3->MoveWindow(nLeft3, nTop3, nWidth3, nHeight3);
	}
	// 3. 사용자가 특정 파일을 더블클릭해서 연 경우
	else
	{
		// ProcessShellCommand를 호출하여 해당 파일을 열도록 합니다.
		// (이 코드가 없으면 파일 더블클릭으로 열기가 안 됩니다)
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	return TRUE;
}

int CCalendarAppApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CCalendarAppApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CCalendarAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCalendarAppApp 메시지 처리기





// MyToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "MyToolsDlg.h"
#include "afxdialogex.h"

#define __USE_COMM_CONFIG
#include "CommonConfig.h"


#include "ComTaskModel.h"
#include "CanTaskModel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyToolsDlg 对话框



CMyToolsDlg::CMyToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyToolsDlg::IDD, pParent)
	, m_splashDlg(NULL)
	, m_pComSettingDlg(NULL)
	, m_pComLogDlg(NULL)
	, m_pControlsDlg(NULL)
	, m_pCanSettingDlg(NULL)
	, m_pCanLogDlg(NULL)
	, m_pCanDbcSettingDlg(NULL)
	, m_mainTabSelInx(0)
	, m_pCanBtnSetupDlg(NULL)
	, m_CurSelPrj(_T(""))
	, m_pEolDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tab_main);
	DDX_Control(pDX, IDC_MFCBUTTON_COM_CONN, m_btn_com_conn);
	DDX_Control(pDX, IDC_MFCBUTTON_CAN_CONN, m_btn_can_conn);
	DDX_Control(pDX, IDC_COMBO_ACTIVE_CAN, m_avtive_candb);
}

BEGIN_MESSAGE_MAP(CMyToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)//使工具栏显示提示信息
	ON_MESSAGE(WM_MIAN_DLG_RESOTRE, &CMyToolsDlg::OnMianDlgResotre)
	ON_COMMAND(ID_TOOLBAR_BUTTON_COM_SETTING, &CMyToolsDlg::OnToolbarButtonComSetting)
	ON_COMMAND(ID_TOOLBAR_BUTTON_COMCONNECT, &CMyToolsDlg::OnToolbarButtonComconnect)
	ON_COMMAND(ID_MENU_DEVICE_COM, &CMyToolsDlg::OnMenuDeviceCom)
	ON_COMMAND(ID_MENU_DEVICE_CAN, &CMyToolsDlg::OnMenuDeviceCan)
	ON_COMMAND(ID_MENU_VIEW_COM, &CMyToolsDlg::OnMenuViewCom)
	ON_COMMAND(ID_MENU_VIEW_CAN, &CMyToolsDlg::OnMenuViewCan)
	ON_WM_SIZE()
	ON_COMMAND(ID_TOOLBAR_BUTTON_COMDISCONNECT, &CMyToolsDlg::OnToolbarButtonComdisconnect)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CMyToolsDlg::OnTcnSelchangeTabMain)
	ON_COMMAND(ID_DBC_OPEN, &CMyToolsDlg::OnDbcOpen)
	ON_COMMAND(ID_BUTTON_TOOL_CANSETTING, &CMyToolsDlg::OnButtonToolCansetting)
	ON_COMMAND(ID_BUTTON_CANCONNECT, &CMyToolsDlg::OnButtonCanconnect)
	ON_COMMAND(ID_BUTTON_CANDISCONNECT, &CMyToolsDlg::OnButtonCandisconnect)
	ON_BN_CLICKED(IDC_MFCBUTTON_COM_CONN, &CMyToolsDlg::OnBnClickedMfcbuttonComConn)
	ON_BN_CLICKED(IDC_MFCBUTTON_COM_LOG, &CMyToolsDlg::OnBnClickedMfcbuttonComLog)
	ON_BN_CLICKED(IDC_MFCBUTTON_CAN_CONN, &CMyToolsDlg::OnBnClickedMfcbuttonCanConn)
	ON_BN_CLICKED(IDC_MFCBUTTON_CAN_LOG, &CMyToolsDlg::OnBnClickedMfcbuttonCanLog)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTIVE_CAN, &CMyToolsDlg::OnCbnSelchangedComboActiveCan)
	ON_MESSAGE(WM_MINE_MAIN_COM_CONNECT, &CMyToolsDlg::OnMineMainComConnect)
	ON_MESSAGE(WM_MINE_MAIN_COM_DISCONNECT, &CMyToolsDlg::OnMineMainComDisconnect)
	ON_MESSAGE(WM_MINE_MAIN_CAN_CONNECT, &CMyToolsDlg::OnMineMainCanConnect)
	ON_MESSAGE(WM_MINE_MAIN_CAN_DISCONNECT, &CMyToolsDlg::OnMineMainCanDisconnect)
	ON_NOTIFY(NM_CLICK, IDC_TAB_MAIN, &CMyToolsDlg::OnNMClickTabMain)
END_MESSAGE_MAP()



// CMyToolsDlg 消息处理程序

BOOL CMyToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

#if 1// 显示启动窗口
	// 隐藏主窗口
	HideMainWnd();// 初始化 启动界面
	if (m_splashDlg == NULL)
	{
		m_splashDlg = new CSplashDialog;
		m_splashDlg->Create(IDD_SPLASH);
		// 显示启动界面, 并居中
		m_splashDlg->ShowWindow(SW_SHOW);
		m_splashDlg->CenterWindow();
	}
#endif


#if 0// 工具栏
	if (m_MainToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC)
		&& m_MainToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
		m_MainToolBar.MoveWindow(1, 1, 500, 24, 1);


		//使工具栏图标显示256色
		//HBITMAP hbm = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		//	MAKEINTRESOURCE(IDR_TOOLBAR1), //加载IDB_TOOLBAR
		//	IMAGE_BITMAP, //按位图格式
		//	0, 0, // cx,cy
		//	LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

		//CBitmap bm;
		//bm.Attach(hbm);
		//m_ilToolBar.Create(21, 18, ILC_COLOR16, 1, 1);
		//m_ilToolBar.Add(&bm, (CBitmap*)NULL);
		//m_MainToolBar.GetToolBarCtrl().SetImageList(&m_ilToolBar);


		//HBRUSH newBr = CreateSolidBrush(RGB(255, 0, 255));
		//SetClassLong(m_MainToolBar.m_hWnd, GCL_HBRBACKGROUND, (long)newBr);

		/*
		CImageList img变量; 
		在ImageList中加上自己想要的图标. 
		Img.Create(22, 22, ILC_COLOR8|ILC_MASK,2,2); 
		Img.SetBkColor(::GetSysColor(COLOR_BTNFACE)); 
		img.Add(AfxGetApp()->LoadIcon(IDI_ICON1)); 
		img.Add(AfxGetApp()->LoadIcon(IDI_ICON2)); 
		img.Add(AfxGetApp()->LoadIcon(IDI_ICON3)); 
		img.Add(AfxGetApp()->LoadIcon(IDI_ICON4)); 
		m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img); 
		img.Detach();
		*/
	}
#endif
	
	InitAllDialogs();

	InitControls();

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMyToolsDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	// UNICODE消息
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//TCHAR szFullText[512];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom为工具条的HWND
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) //不为分隔符	
	{
		strTipText.LoadString(nID);
		strTipText = strTipText.Mid(strTipText.Find('\n', 0) + 1);

#ifndef _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#endif

		*pResult = 0;

		// 使工具条提示窗口在最上面
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE |
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
		return TRUE;
	}
	return TRUE;
}



void CMyToolsDlg::HideMainWnd(void)
{
#if 1
	//方法1:
	GetWindowRect(&m_initWinRect);
	SetWindowPos(&this->wndTop, 0, 0, 0, 0, SWP_HIDEWINDOW);

	//方法2:
#else
	GetWindowRect(&_last_dlg_pos); //移动窗口到屏幕区域之外，达到消失的目的

	MoveWindow(-_last_dlg_pos.left,
		-_last_dlg_pos.right,
		_last_dlg_pos.top,
		_last_dlg_pos.bottom);
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
#endif
}


void CMyToolsDlg::RestoreMainWnd(void)
{
#if 1
	// 方法1:
	//wndBottom 将窗口放在Z轴次序的底部。如果这个CWnd是一个顶层窗口，则窗口将失去它的顶层状态；系统将这个窗口放在其它所有窗口的底部。
	//	wndTop 将窗口放在Z轴次序的顶部。
	//	wndTopMost 将窗口放在所有非顶层窗口的上面。这个窗口将保持它的顶层位置，即使它失去了活动状态。
	//	wndNoTopMost 将窗口重新定位到所有非顶层窗口的顶部（这意味着在所有的顶层窗口之下）。这个标志对那些已经是非顶层窗口的窗口没有作用。有关这个函数以及这些参数的使用规则参见说明部分。
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	int dx = m_initWinRect.Width();
	int dy = m_initWinRect.Height();

	SetWindowPos(&wndTop, (cx - dx) / 2, (cy - dy) / 2, dx, dy, SWP_SHOWWINDOW);
	CenterWindow();

	GetClientRect(&m_rect);
#else
	//方法2:
	CRect rectNow;
	GetWindowRect(&rectNow);

	if (rectNow != _last_dlg_pos
		|| !IsWindowVisible())
	{
		MoveWindow(_last_dlg_pos);
		ShowWindow(SW_SHOW);

		//如果对话框程序有最小化窗口时的特殊处理，否则最小化时不能正确恢复窗口
		if (IsIconic())
			ShowWindow(SW_RESTORE);

		ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);// 修改对话框的风格, 使消失的任务栏图标正常
		CenterWindow();
	}
	SendMessage(WM_SIZE);
	SetForegroundWindow();
#endif
}

afx_msg LRESULT CMyToolsDlg::OnMianDlgResotre(WPARAM wParam, LPARAM lParam)
{
	RestoreMainWnd();
	if (m_splashDlg != NULL)
	{
		m_splashDlg->DestroyWindow();
		delete m_splashDlg;
		m_splashDlg = NULL;
	}
	
	// 显示启动界面, 并居中
	TestFunc();
	return 0;
}



BOOL CMyToolsDlg::DestroyWindow()
{
	DeleteAllDialogs();

	return CDialogEx::DestroyWindow();
}


void CMyToolsDlg::InitAllDialogs()
{
	if (m_pComSettingDlg == NULL)
	{
		m_pComSettingDlg = new CComSettingDialog(this);
		m_pComSettingDlg->Create(IDD_DIALOG_COM_SETTING);
		m_pComSettingDlg->CenterWindow();
		m_pComSettingDlg->ShowWindow(SW_HIDE);
	}

	if (m_pComLogDlg == NULL)
	{
		m_pComLogDlg = new CComLogDialog(this);
		m_pComLogDlg->Create(IDD_DIALOG_COM_LOG, GetDesktopWindow()); // 确保点击主窗口的时候可以吧主窗体调到子窗口前面
		m_pComLogDlg->CenterWindow();
		m_pComLogDlg->ShowWindow(SW_HIDE);
	}

	if (m_pCanSettingDlg == NULL)
	{
		m_pCanSettingDlg = new CCanSettingDialog(this);
		m_pCanSettingDlg->Create(IDD_DIALOG_CAN_SETTING);
		m_pCanSettingDlg->CenterWindow();
		m_pCanSettingDlg->ShowWindow(SW_HIDE);
	}
	if (m_pCanLogDlg == NULL)
	{
		m_pCanLogDlg = new CCanLogDialog(this);
		m_pCanLogDlg->Create(IDD_DIALOG_CAN_BUS_LOG, GetDesktopWindow()); // 确保点击主窗口的时候可以吧主窗体调到子窗口前面
		m_pCanLogDlg->CenterWindow();
		m_pCanLogDlg->ShowWindow(SW_HIDE);
	}

	
}

void CMyToolsDlg::DeleteAllDialogs()
{
	IComTaskModel * pComTaskModel = ComTaskModel::GetInstance();
	delete pComTaskModel;

	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	delete pCanTaskModel;

	if (m_pComSettingDlg != NULL)
	{
		m_pComSettingDlg->DestroyWindow();
		delete m_pComSettingDlg;
	}

	if (m_pComLogDlg != NULL)
	{
		m_pComLogDlg->DestroyWindow();
		delete m_pComLogDlg;
	}
	if (m_pCanSettingDlg != NULL)
	{
		m_pCanSettingDlg->DestroyWindow();
		delete m_pCanSettingDlg;
	}
	if (m_pCanLogDlg != NULL)
	{
		m_pCanLogDlg->DestroyWindow();
		delete m_pCanLogDlg;
	}
	if (m_pControlsDlg != NULL)
	{
		m_pControlsDlg->DestroyWindow();
		delete m_pControlsDlg;
	}

	if (m_pCanDbcSettingDlg != NULL)
	{
		m_pCanDbcSettingDlg->DestroyWindow();
		delete m_pCanDbcSettingDlg;
	}

	if (m_pCanBtnSetupDlg != NULL)
	{
		m_pCanBtnSetupDlg->DestroyWindow();
		delete m_pCanBtnSetupDlg;
	}

	if (m_pEolDlg != NULL)
	{
		m_pEolDlg->DestroyWindow();
		delete m_pEolDlg;
	}

	CCommonConfig *tt = CCommonConfig::GetInstance();
	tt->saveConfig();
	delete tt;
}



void CMyToolsDlg::InitControls()
{
	// init tab
	CRect tabRect;
	m_tab_main.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 3;
	tabRect.top += 23;
	tabRect.bottom -= 3;
	m_tab_main.InsertItem(0, L"Controls");
	m_tab_main.InsertItem(1, L"Can Dbc File");
	m_tab_main.InsertItem(2, L"CAN Buttons Setup");
	m_tab_main.InsertItem(3, L"EOL Test");

	m_mainTabSelInx = 0;

	// init uart dlg
	m_pControlsDlg = new CControlsDlg(m_pComLogDlg,m_pCanLogDlg,this);
	m_pControlsDlg->Create(IDD_DIALOG_CONTROLS, GetDlgItem(IDC_TAB_MAIN));
	m_pControlsDlg->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_pControlsDlg->ShowWindow(SW_SHOW);

	//
	m_pCanDbcSettingDlg = new CCanDbcSettingDialog();
	m_pCanDbcSettingDlg->Create(IDD_DIALOG_CAN_DBC_SETTING, GetDlgItem(IDC_TAB_MAIN));
	m_pCanDbcSettingDlg->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_pCanDbcSettingDlg->ShowWindow(SW_HIDE);


	m_pCanBtnSetupDlg = new CCanButtonsSetUpDialog();
	m_pCanBtnSetupDlg->Create(IDD_DIALOG_CAN_BUTTONS, GetDlgItem(IDC_TAB_MAIN));
	m_pCanBtnSetupDlg->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_pCanBtnSetupDlg->ShowWindow(SW_HIDE);


	m_pEolDlg = new CEolTestDialog();
	m_pEolDlg->Create(IDD_DIALOG_EOL, GetDlgItem(IDC_TAB_MAIN));
	m_pEolDlg->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_pEolDlg->ShowWindow(SW_HIDE);


	m_avtive_candb.AddString(L"请选择项目名");
	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	CanDbc * data = pCanTaskModel->GetCanMatrix();
	m_prjList.clear();
	data->GetModelListFromDb(m_prjList);
	for (unsigned int i = 0; i < m_prjList.size(); i++)
	{
		m_avtive_candb.AddString(m_prjList[i]);
	}
	m_avtive_candb.SetCurSel(0);
}


int CMyToolsDlg::TestFunc()
{
	//m_pComSettingDlg->ShowWindow(SW_SHOW);
	//m_pComLogDlg->ShowWindow(SW_SHOW);s
	return 0;
}



//.//////toolbar buttons /////////////////
void CMyToolsDlg::OnToolbarButtonComSetting()
{
	m_pComSettingDlg->ShowWindow(SW_SHOW);
}


void CMyToolsDlg::OnToolbarButtonComconnect()
{
	m_pComSettingDlg->OnBnClickedBtnComOpenPort();
}


void CMyToolsDlg::OnToolbarButtonComdisconnect()
{
	m_pComSettingDlg->OnBnClickedBtnComClosePort();
}


void CMyToolsDlg::OnButtonToolCansetting()
{
	m_pCanSettingDlg->ShowWindow(SW_SHOW);
}


void CMyToolsDlg::OnButtonCanconnect()
{
	m_pCanSettingDlg->OnBnClickedBtnCanOpenPort();
}


void CMyToolsDlg::OnButtonCandisconnect()
{
	m_pCanSettingDlg->OnBnClickedBtnCanClosePort();
}



//.//////menu buttons /////////////////
void CMyToolsDlg::OnMenuDeviceCom()
{
	m_pComSettingDlg->ShowWindow(SW_SHOW);
}


void CMyToolsDlg::OnMenuDeviceCan()
{
	m_pCanSettingDlg->ShowWindow(SW_SHOW);
}


void CMyToolsDlg::OnMenuViewCom()
{
	m_pComLogDlg->ShowWindow(SW_SHOW);
}


void CMyToolsDlg::OnMenuViewCan()
{
	m_pCanLogDlg->ShowWindow(SW_SHOW);
}


void CMyToolsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (cx != 0 && cy != 0)
	{
		ChangeSize(IDC_TAB_MAIN, cx, cy);
		GetClientRect(&m_rect);
	}
}


void CMyToolsDlg::ChangeSize(UINT nID, int x, int y)  //nID为控件ID，x,y分别为对话框的当前长和宽
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL && m_rect.Height() != 0 && m_rect.Width()!=0)     //判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	{
		CRect rec;
		pWnd->GetWindowRect(&rec);  //获取控件变化前的大小
		ScreenToClient(&rec);   //将控件大小装换位在对话框中的区域坐标
		//rec.left = rec.left*x / m_rect.Width();  //按照比例调整空间的新位置
		//rec.top = rec.top*y / m_rect.Height();
		rec.bottom = rec.bottom*y / m_rect.Height();
		rec.right = rec.right*x / m_rect.Width();
		pWnd->MoveWindow(rec);   //伸缩控件

		// init tab
		CRect tabRect;
		m_tab_main.GetClientRect(&tabRect);
		tabRect.left += 1;
		tabRect.right -= 3;
		tabRect.top += 23;
		tabRect.bottom -= 3;

		m_pControlsDlg->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_pCanDbcSettingDlg->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(),SWP_HIDEWINDOW);
		m_pCanBtnSetupDlg->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_pEolDlg->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		SetMainTabSel(m_mainTabSelInx);
	}
}



void CMyToolsDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetMainTabSel(m_tab_main.GetCurSel());
	*pResult = 0;
}


void CMyToolsDlg::OnDbcOpen()
{
	CString strFile;
	WCHAR szFilter[] = _T("dbc 文件|*.dbc|所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("选择dbc文件..."), NULL, 0, szFilter, this);
	fileDlg.m_ofn.lpstrTitle = L"选择dbc文件";
	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		strFile = fileDlg.GetPathName(); 
		ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
		pCanTaskModel->LoadCanMatrixFromDbc(strFile);
		m_pCanDbcSettingDlg->refreshTree();
		m_tab_main.SetCurSel(1);
		SetMainTabSel(1);
	}
	else
	{
		return;
	}
}


void CMyToolsDlg::OnBnClickedMfcbuttonComConn()
{
	m_pComSettingDlg->ComPortOpenClose();
}


void CMyToolsDlg::OnBnClickedMfcbuttonComLog()
{
	m_pComLogDlg->ShowWindow(SW_SHOW);
	m_pComLogDlg->SetFocus();
}


void CMyToolsDlg::OnBnClickedMfcbuttonCanConn()
{
	m_pCanSettingDlg->CanBusOpenClose();
}


void CMyToolsDlg::OnBnClickedMfcbuttonCanLog()
{
	m_pCanLogDlg->ShowWindow(SW_SHOW);
	m_pCanLogDlg->SetFocus();
}


void CMyToolsDlg::OnCbnSelchangedComboActiveCan()
{
	CString tmp;
	m_avtive_candb.GetLBText(m_avtive_candb.GetCurSel(), tmp);
		
	if (m_avtive_candb.GetCurSel() != 0)
	{
		m_pControlsDlg->m_bInited = true;
		m_pCanLogDlg->SetCurPrj(tmp);
		ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
		CanDbc * data = pCanTaskModel->GetCanMatrix();
		data->ReLoadCanMatrixFromDb(tmp);
		m_CurSelPrj = tmp;
	}
	else
	{
		m_pControlsDlg->m_bInited = false;
		m_CurSelPrj = L"";
	}	
}
void CMyToolsDlg::SetMainTabVisible(bool vis)
{
	if (!vis)
	{
		m_pControlsDlg->ShowWindow(SW_HIDE);
		m_pCanDbcSettingDlg->ShowWindow(SW_HIDE);
		m_pCanBtnSetupDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		SetMainTabSel(m_mainTabSelInx);
	}
}

void CMyToolsDlg::SetMainTabSel(int idx)
{
	switch (idx)
	{
	case 0:
		m_pControlsDlg->ShowWindow(SW_SHOW);
		m_pCanDbcSettingDlg->ShowWindow(SW_HIDE); 
		m_pCanBtnSetupDlg->ShowWindow(SW_HIDE);
		m_pEolDlg->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pControlsDlg->ShowWindow(SW_HIDE);
		m_pCanDbcSettingDlg->ShowWindow(SW_SHOW); 
		m_pCanBtnSetupDlg->ShowWindow(SW_HIDE);
		m_pEolDlg->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pControlsDlg->ShowWindow(SW_HIDE);
		m_pCanDbcSettingDlg->ShowWindow(SW_HIDE);
		m_pCanBtnSetupDlg->ShowWindow(SW_SHOW);
		m_pEolDlg->ShowWindow(SW_HIDE);
		break;
	case 3:
		m_pControlsDlg->ShowWindow(SW_HIDE);
		m_pCanDbcSettingDlg->ShowWindow(SW_HIDE);
		m_pCanBtnSetupDlg->ShowWindow(SW_HIDE);
		m_pEolDlg->ShowWindow(SW_SHOW);
		break;
	}

	m_mainTabSelInx = idx;
}


afx_msg LRESULT CMyToolsDlg::OnMineMainComConnect(WPARAM wParam, LPARAM lParam)
{
	m_btn_com_conn.SetWindowTextW(L"断开串口");
	m_btn_com_conn.SetTooltip(L"串口已经连接， 是否断开？");
	m_btn_com_conn.SetTextColor(RGB(255, 0, 255));
	return 0;
}


afx_msg LRESULT CMyToolsDlg::OnMineMainComDisconnect(WPARAM wParam, LPARAM lParam)
{
	m_btn_com_conn.SetWindowTextW(L"连接串口");
	m_btn_com_conn.SetTooltip(L"串口已经断开， 是否连接？");
	m_btn_com_conn.SetTextColor(RGB(0, 0, 0));
	return 0;
}


afx_msg LRESULT CMyToolsDlg::OnMineMainCanConnect(WPARAM wParam, LPARAM lParam)
{
	m_btn_can_conn.SetWindowTextW(L"断开CAN");
	m_btn_can_conn.SetTooltip(L"CAN已经连接， 是否断开？");
	m_btn_can_conn.SetTextColor(RGB(100, 155, 255));

	if (m_CurSelPrj.IsEmpty())
	{
		m_avtive_candb.SetFocus();
		AfxMessageBox(L"请选择项目");
	}

	m_avtive_candb.EnableWindow(false);
	return 0;
}


afx_msg LRESULT CMyToolsDlg::OnMineMainCanDisconnect(WPARAM wParam, LPARAM lParam)
{
	m_btn_can_conn.SetWindowTextW(L"连接CAN");
	m_btn_can_conn.SetTooltip(L"CAN已经断开， 是否连接？");
	m_btn_can_conn.SetTextColor(RGB(0, 0, 0));

	m_avtive_candb.EnableWindow(true);
	return 0;
}


void CMyToolsDlg::OnNMClickTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

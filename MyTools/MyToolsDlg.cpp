
// MyToolsDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMyToolsDlg �Ի���



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

	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)//ʹ��������ʾ��ʾ��Ϣ
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



// CMyToolsDlg ��Ϣ�������

BOOL CMyToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

#if 1// ��ʾ��������
	// ����������
	HideMainWnd();// ��ʼ�� ��������
	if (m_splashDlg == NULL)
	{
		m_splashDlg = new CSplashDialog;
		m_splashDlg->Create(IDD_SPLASH);
		// ��ʾ��������, ������
		m_splashDlg->ShowWindow(SW_SHOW);
		m_splashDlg->CenterWindow();
	}
#endif


#if 0// ������
	if (m_MainToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC)
		&& m_MainToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
		m_MainToolBar.MoveWindow(1, 1, 500, 24, 1);


		//ʹ������ͼ����ʾ256ɫ
		//HBITMAP hbm = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		//	MAKEINTRESOURCE(IDR_TOOLBAR1), //����IDB_TOOLBAR
		//	IMAGE_BITMAP, //��λͼ��ʽ
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
		CImageList img����; 
		��ImageList�м����Լ���Ҫ��ͼ��. 
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

	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMyToolsDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	// UNICODE��Ϣ
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//TCHAR szFullText[512];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFromΪ��������HWND
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) //��Ϊ�ָ���	
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

		// ʹ��������ʾ������������
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE |
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
		return TRUE;
	}
	return TRUE;
}



void CMyToolsDlg::HideMainWnd(void)
{
#if 1
	//����1:
	GetWindowRect(&m_initWinRect);
	SetWindowPos(&this->wndTop, 0, 0, 0, 0, SWP_HIDEWINDOW);

	//����2:
#else
	GetWindowRect(&_last_dlg_pos); //�ƶ����ڵ���Ļ����֮�⣬�ﵽ��ʧ��Ŀ��

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
	// ����1:
	//wndBottom �����ڷ���Z�����ĵײ���������CWnd��һ�����㴰�ڣ��򴰿ڽ�ʧȥ���Ķ���״̬��ϵͳ��������ڷ����������д��ڵĵײ���
	//	wndTop �����ڷ���Z�����Ķ�����
	//	wndTopMost �����ڷ������зǶ��㴰�ڵ����档������ڽ��������Ķ���λ�ã���ʹ��ʧȥ�˻״̬��
	//	wndNoTopMost ���������¶�λ�����зǶ��㴰�ڵĶ���������ζ�������еĶ��㴰��֮�£��������־����Щ�Ѿ��ǷǶ��㴰�ڵĴ���û�����á��й���������Լ���Щ������ʹ�ù���μ�˵�����֡�
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	int dx = m_initWinRect.Width();
	int dy = m_initWinRect.Height();

	SetWindowPos(&wndTop, (cx - dx) / 2, (cy - dy) / 2, dx, dy, SWP_SHOWWINDOW);
	CenterWindow();

	GetClientRect(&m_rect);
#else
	//����2:
	CRect rectNow;
	GetWindowRect(&rectNow);

	if (rectNow != _last_dlg_pos
		|| !IsWindowVisible())
	{
		MoveWindow(_last_dlg_pos);
		ShowWindow(SW_SHOW);

		//����Ի����������С������ʱ�����⴦��������С��ʱ������ȷ�ָ�����
		if (IsIconic())
			ShowWindow(SW_RESTORE);

		ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);// �޸ĶԻ���ķ��, ʹ��ʧ��������ͼ������
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
	
	// ��ʾ��������, ������
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
		m_pComLogDlg->Create(IDD_DIALOG_COM_LOG, GetDesktopWindow()); // ȷ����������ڵ�ʱ����԰�����������Ӵ���ǰ��
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
		m_pCanLogDlg->Create(IDD_DIALOG_CAN_BUS_LOG, GetDesktopWindow()); // ȷ����������ڵ�ʱ����԰�����������Ӵ���ǰ��
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


	m_avtive_candb.AddString(L"��ѡ����Ŀ��");
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


void CMyToolsDlg::ChangeSize(UINT nID, int x, int y)  //nIDΪ�ؼ�ID��x,y�ֱ�Ϊ�Ի���ĵ�ǰ���Ϳ�
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL && m_rect.Height() != 0 && m_rect.Width()!=0)     //�ж��Ƿ�Ϊ�գ���Ϊ�ڴ��ڴ�����ʱ��Ҳ�����OnSize���������Ǵ�ʱ�����ؼ���û�д�����PwndΪ��
	{
		CRect rec;
		pWnd->GetWindowRect(&rec);  //��ȡ�ؼ��仯ǰ�Ĵ�С
		ScreenToClient(&rec);   //���ؼ���Сװ��λ�ڶԻ����е���������
		//rec.left = rec.left*x / m_rect.Width();  //���ձ��������ռ����λ��
		//rec.top = rec.top*y / m_rect.Height();
		rec.bottom = rec.bottom*y / m_rect.Height();
		rec.right = rec.right*x / m_rect.Width();
		pWnd->MoveWindow(rec);   //�����ؼ�

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
	WCHAR szFilter[] = _T("dbc �ļ�|*.dbc|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("ѡ��dbc�ļ�..."), NULL, 0, szFilter, this);
	fileDlg.m_ofn.lpstrTitle = L"ѡ��dbc�ļ�";
	// ��ʾ���ļ��Ի���   
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
	m_btn_com_conn.SetWindowTextW(L"�Ͽ�����");
	m_btn_com_conn.SetTooltip(L"�����Ѿ����ӣ� �Ƿ�Ͽ���");
	m_btn_com_conn.SetTextColor(RGB(255, 0, 255));
	return 0;
}


afx_msg LRESULT CMyToolsDlg::OnMineMainComDisconnect(WPARAM wParam, LPARAM lParam)
{
	m_btn_com_conn.SetWindowTextW(L"���Ӵ���");
	m_btn_com_conn.SetTooltip(L"�����Ѿ��Ͽ��� �Ƿ����ӣ�");
	m_btn_com_conn.SetTextColor(RGB(0, 0, 0));
	return 0;
}


afx_msg LRESULT CMyToolsDlg::OnMineMainCanConnect(WPARAM wParam, LPARAM lParam)
{
	m_btn_can_conn.SetWindowTextW(L"�Ͽ�CAN");
	m_btn_can_conn.SetTooltip(L"CAN�Ѿ����ӣ� �Ƿ�Ͽ���");
	m_btn_can_conn.SetTextColor(RGB(100, 155, 255));

	if (m_CurSelPrj.IsEmpty())
	{
		m_avtive_candb.SetFocus();
		AfxMessageBox(L"��ѡ����Ŀ");
	}

	m_avtive_candb.EnableWindow(false);
	return 0;
}


afx_msg LRESULT CMyToolsDlg::OnMineMainCanDisconnect(WPARAM wParam, LPARAM lParam)
{
	m_btn_can_conn.SetWindowTextW(L"����CAN");
	m_btn_can_conn.SetTooltip(L"CAN�Ѿ��Ͽ��� �Ƿ����ӣ�");
	m_btn_can_conn.SetTextColor(RGB(0, 0, 0));

	m_avtive_candb.EnableWindow(true);
	return 0;
}


void CMyToolsDlg::OnNMClickTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

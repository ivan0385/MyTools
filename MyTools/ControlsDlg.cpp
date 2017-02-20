// ControlsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "ControlsDlg.h"
#include "afxdialogex.h"
#include "common.h"
// CControlsDlg 对话框

IMPLEMENT_DYNAMIC(CControlsDlg, CDialogEx)

CControlsDlg::CControlsDlg(CComLogDialog* pcom /*=NULL*/, CCanLogDialog *pcan /*=NULL*/, CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlsDlg::IDD, pParent)
	, m_edit_value_can_radar_level_fl(_T(""))
	, m_edit_value_can_radar_level_fml(_T(""))
	, m_edit_value_can_radar_level_fmr(_T(""))
	, m_edit_value_can_radar_level_fr(_T(""))
	, m_edit_value_can_radar_level_rl(_T(""))
	, m_edit_value_can_radar_level_rml(_T(""))
	, m_edit_value_can_radar_level_rmr(_T(""))
	, m_edit_value_can_radar_level_rr(_T(""))
	, m_edit_value_can_angle(_T(""))
	, m_edit_value_can_speed(_T(""))
	, m_pComLogDlg(pcom)
	, m_pCanLogDlg(pcan)
	, m_bInited(false)
{

}

CControlsDlg::~CControlsDlg()
{
}

void CControlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITCAN_RADAR_LEVEL_FL, m_edit_value_can_radar_level_fl);
	DDX_Text(pDX, IDC_EDITCAN_RADAR_LEVEL_FML, m_edit_value_can_radar_level_fml);
	DDX_Text(pDX, IDC_EDITCAN_RADAR_LEVEL_FMR, m_edit_value_can_radar_level_fmr);
	DDX_Text(pDX, IDC_EDITCAN_RADAR_LEVEL_FR, m_edit_value_can_radar_level_fr);
	DDX_Text(pDX, IDC_EDITCAN_RADAR_LEVEL_RL, m_edit_value_can_radar_level_rl);
	DDX_Text(pDX, IDC_EDITCAN_RADAR_LEVEL_RML, m_edit_value_can_radar_level_rml);
	DDX_Text(pDX, IDC_EDITCAN_RADAR_LEVEL_RMR, m_edit_value_can_radar_level_rmr);
	DDX_Text(pDX, IDC_EDITCAN_RADAR_LEVEL_RR, m_edit_value_can_radar_level_rr);
	DDX_Control(pDX, IDC_COMBO_CAN_RADAR_NUMBER, m_cbox_radar_number);
	DDX_Control(pDX, IDC_COMBO_CAN_RADAR_MIN, m_cbox_radar_min);
	DDX_Text(pDX, IDC_EDIT_CAN_ANGLE, m_edit_value_can_angle);
	DDX_Text(pDX, IDC_EDIT_CAN_SPEED, m_edit_value_can_speed);
	DDX_Control(pDX, IDC_SLIDER_CAN_SPEED, m_slider_can_speed);
	DDX_Control(pDX, IDC_SLIDER_CAN_ANGLE, m_slider_can_angle);
	DDX_Control(pDX, IDC_COMBO_CAN_COLOR, m_cbox_can_color);
	DDX_Control(pDX, IDC_EDIT_CAN_SPEED, m_edit_canSpd);
	DDX_Control(pDX, IDC_EDIT_CAN_ANGLE, m_edit_canAngle);
	DDX_Control(pDX, IDC_SLIDER_CAN_SCRN_X, m_slider_src_x);
	DDX_Control(pDX, IDC_SLIDER_CAN_SCRN_Y, m_slider_src_y);
	DDX_Control(pDX, IDC_EDIT__CAN_SCRN_X, m_edit_src_x);
	DDX_Control(pDX, IDC_EDIT_CAN_SCRN_Y, m_edit_src_y);
}


BEGIN_MESSAGE_MAP(CControlsDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAN_RADAR_LEVEL_FL, &CControlsDlg::OnDeltaposSpinCanRadarLevelFl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAN_RADAR_LEVEL_FML, &CControlsDlg::OnDeltaposSpinCanRadarLevelFml)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAN_RADAR_LEVEL_FMR, &CControlsDlg::OnDeltaposSpinCanRadarLevelFmr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAN_RADAR_LEVEL_FR, &CControlsDlg::OnDeltaposSpinCanRadarLevelFr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAN_RADAR_LEVEL_RL, &CControlsDlg::OnDeltaposSpinCanRadarLevelRl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAN_RADAR_LEVEL_RML, &CControlsDlg::OnDeltaposSpinCanRadarLevelRml)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAN_RADAR_LEVEL_RMR, &CControlsDlg::OnDeltaposSpinCanRadarLevelRmr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAN_RADAR_LEVEL_RR, &CControlsDlg::OnDeltaposSpinCanRadarLevelRr)
	ON_BN_CLICKED(IDC_RADIO_CAN_IGN_ON, &CControlsDlg::OnBnClickedRadioCanIgn)
	ON_BN_CLICKED(IDC_RADIO_CAN_IGN_ACC, &CControlsDlg::OnBnClickedRadioCanIgn)
	ON_BN_CLICKED(IDC_RADIO_CAN_IGN_OFF, &CControlsDlg::OnBnClickedRadioCanIgn)
	ON_BN_CLICKED(IDC_RADIO_CAN_LAMP_LEFT, &CControlsDlg::OnBnClickedRadioCanLamp)
	ON_BN_CLICKED(IDC_RADIO_CAN_LAMP_RIGHT, &CControlsDlg::OnBnClickedRadioCanLamp)
	ON_BN_CLICKED(IDC_RADIO_CAN_LAMP_BOTH, &CControlsDlg::OnBnClickedRadioCanLamp)
	ON_BN_CLICKED(IDC_RADIO_CAN_LAMP_OFF, &CControlsDlg::OnBnClickedRadioCanLamp)
	ON_BN_CLICKED(IDC_RADIO_CAN_RERVSE_ON, &CControlsDlg::OnBnClickedRadioCanRervse)
	ON_BN_CLICKED(IDC_RADIO_CAN_RERVSE_OFF, &CControlsDlg::OnBnClickedRadioCanRervse)
	ON_BN_CLICKED(IDC_RADIO_CAN_SCREEN_1, &CControlsDlg::OnBnClickedRadioCanScreen)
	ON_BN_CLICKED(IDC_RADIO_CAN_SCREEN_2, &CControlsDlg::OnBnClickedRadioCanScreen)
	ON_BN_CLICKED(IDC_RADIO_CAN_3DSIDE_ON, &CControlsDlg::OnBnClickedRadioCan3dside)
	ON_BN_CLICKED(IDC_RADIO_CAN_3DSIDE_OFF, &CControlsDlg::OnBnClickedRadioCan3dside)
	ON_BN_CLICKED(IDC_RADIO_CAN_PDC_ON, &CControlsDlg::OnBnClickedRadioCanPdc)
	ON_BN_CLICKED(IDC_RADIO_CAN_PDC_OFF, &CControlsDlg::OnBnClickedRadioCanPdc)
	ON_BN_CLICKED(IDC_RADIO_CAN_TRAJ_ON, &CControlsDlg::OnBnClickedRadioCanTraj)
	ON_BN_CLICKED(IDC_RADIO_CAN_TRAJ_OFF, &CControlsDlg::OnBnClickedRadioCanTraj)
	ON_BN_CLICKED(IDC_BUTTON_CAN_CALIB, &CControlsDlg::OnBnClickedButtonCanCalib)
	ON_BN_CLICKED(IDC_BUTTON_CAN_AVM, &CControlsDlg::OnBnClickedButtonCanAvm)
	ON_EN_CHANGE(IDC_EDIT_CAN_SPEED, &CControlsDlg::OnEnChangeEditCanSpeed)
	ON_EN_CHANGE(IDC_EDIT_CAN_ANGLE, &CControlsDlg::OnEnChangeEditCanAngle)
	ON_CBN_SELCHANGE(IDC_COMBO_CAN_RADAR_NUMBER, &CControlsDlg::OnCbnSelchangeComboCanRadarNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_CAN_RADAR_MIN, &CControlsDlg::OnCbnSelchangeComboCanRadarMin)
	ON_EN_CHANGE(IDC_EDITCAN_RADAR_LEVEL_FL, &CControlsDlg::OnEnChangeEditcanRadarLevelFl)
	ON_EN_CHANGE(IDC_EDITCAN_RADAR_LEVEL_FML, &CControlsDlg::OnEnChangeEditcanRadarLevelFml)
	ON_EN_CHANGE(IDC_EDITCAN_RADAR_LEVEL_FMR, &CControlsDlg::OnEnChangeEditcanRadarLevelFmr)
	ON_EN_CHANGE(IDC_EDITCAN_RADAR_LEVEL_FR, &CControlsDlg::OnEnChangeEditcanRadarLevelFr)
	ON_EN_CHANGE(IDC_EDITCAN_RADAR_LEVEL_RL, &CControlsDlg::OnEnChangeEditcanRadarLevelRl)
	ON_EN_CHANGE(IDC_EDITCAN_RADAR_LEVEL_RML, &CControlsDlg::OnEnChangeEditcanRadarLevelRml)
	ON_EN_CHANGE(IDC_EDITCAN_RADAR_LEVEL_RMR, &CControlsDlg::OnEnChangeEditcanRadarLevelRmr)
	ON_EN_CHANGE(IDC_EDITCAN_RADAR_LEVEL_RR, &CControlsDlg::OnEnChangeEditcanRadarLevelRr)
	ON_BN_CLICKED(IDC_RADIO_CAN_LDW_ON, &CControlsDlg::OnBnClickedRadioCanLdw)
	ON_BN_CLICKED(IDC_RADIO_CAN_LDW_OFF, &CControlsDlg::OnBnClickedRadioCanLdw)
	ON_BN_CLICKED(IDC_RADIO_CAN_LDW_SEN1, &CControlsDlg::OnBnClickedRadioCanLdwSen)
	ON_BN_CLICKED(IDC_RADIO_CAN_LDW_SEN2, &CControlsDlg::OnBnClickedRadioCanLdwSen)
	ON_BN_CLICKED(IDC_RADIO_CAN_LDW_SEN3, &CControlsDlg::OnBnClickedRadioCanLdwSen)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_ERR_FL, &CControlsDlg::OnBnClickedCheckCanRadarErrFl)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_ERR_FML, &CControlsDlg::OnBnClickedCheckCanRadarErrFml)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_ERR_RL, &CControlsDlg::OnBnClickedCheckCanRadarErrRl)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_ERR_RML, &CControlsDlg::OnBnClickedCheckCanRadarErrRml)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_ERR_FMR, &CControlsDlg::OnBnClickedCheckCanRadarErrFmr)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_RMR, &CControlsDlg::OnBnClickedCheckCanRadarRmr)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_ERR_FR, &CControlsDlg::OnBnClickedCheckCanRadarErrFr)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_RR, &CControlsDlg::OnBnClickedCheckCanRadarRr)
	ON_BN_CLICKED(IDC_CHECK_CAN_RADAR_ERR_SYS, &CControlsDlg::OnBnClickedCheckCanRadarErrSys)
	ON_BN_CLICKED(IDC_BUTTON_COM_ENV_SETUP, &CControlsDlg::OnBnClickedButtonComEnvSetup)
	ON_BN_CLICKED(IDC_BUTTON_COM_REBOOT, &CControlsDlg::OnBnClickedButtonComReboot)
	ON_BN_CLICKED(IDC_BUTTON_COM_CPBIN, &CControlsDlg::OnBnClickedButtonComCpbin)
	ON_BN_CLICKED(IDC_BUTTON_COM_CPLIB, &CControlsDlg::OnBnClickedButtonComCplib)
	ON_BN_CLICKED(IDC_BUTTON_COM_TARSVP, &CControlsDlg::OnBnClickedButtonComTarsvp)
	ON_BN_CLICKED(IDC_BUTTON_COM_ERASECALIB, &CControlsDlg::OnBnClickedButtonComErasecalib)
	ON_BN_CLICKED(IDC_BUTTON_COM_ERASE_CACHE, &CControlsDlg::OnBnClickedButtonComEraseCache)
	ON_BN_CLICKED(IDC_BUTTON_COM_ERASE_DATA, &CControlsDlg::OnBnClickedButtonComEraseData)
	ON_BN_CLICKED(IDC_BUTTON_COM_ERASE_NOR, &CControlsDlg::OnBnClickedButtonComEraseNor)
	ON_BN_CLICKED(IDC_BUTTON_COM_NFS, &CControlsDlg::OnBnClickedButtonComNfs)
	ON_BN_CLICKED(IDC_BUTTON_COM_NFSSVP, &CControlsDlg::OnBnClickedButtonComNfssvp)
	ON_BN_CLICKED(IDC_BUTTON_COM_CP_CALIBTOUSB, &CControlsDlg::OnBnClickedButtonComCpCalibtousb)
	ON_BN_CLICKED(IDC_BUTTON_COM_CP_CALIBTOUSB3, &CControlsDlg::OnBnClickedButtonComCpCalibtousb3)
	ON_CBN_SELCHANGE(IDC_COMBO_CAN_COLOR, &CControlsDlg::OnCbnSelchangeComboCanColor)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CAN_SPEED, &CControlsDlg::OnNMCustomdrawSliderCanSpeed)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CAN_ANGLE, &CControlsDlg::OnNMCustomdrawSliderCanAngle)
	ON_BN_CLICKED(IDC_BUTTON_CAN_SCRN, &CControlsDlg::OnBnClickedButtonCanScrn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CAN_SCRN_X, &CControlsDlg::OnNMCustomdrawSliderCanScrnX)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CAN_SCRN_Y, &CControlsDlg::OnNMCustomdrawSliderCanScrnY)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CControlsDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_BUTTON_CAN_DVR, &CControlsDlg::OnBnClickedButtonCanDvr)
END_MESSAGE_MAP()


// CControlsDlg 消息处理程序



int CControlsDlg::_InitRadarLevelString()
{
	UpdateData(true);
	m_edit_value_can_radar_level_fl = L"0";
	m_edit_value_can_radar_level_fml = L"0";
	m_edit_value_can_radar_level_fmr = L"0";
	m_edit_value_can_radar_level_fr = L"0";
	m_edit_value_can_radar_level_rl = L"0";
	m_edit_value_can_radar_level_rml = L"0";
	m_edit_value_can_radar_level_rmr = L"0";
	m_edit_value_can_radar_level_rr = L"0";
	UpdateData(false);
	return 0;
}

void CControlsDlg::_SetRadarLevelString(int which, int add)
{
	static CString *editlist[8] = { &m_edit_value_can_radar_level_fl, 
		&m_edit_value_can_radar_level_fml,
		&m_edit_value_can_radar_level_fmr,
		&m_edit_value_can_radar_level_fr,
		&m_edit_value_can_radar_level_rl,
		&m_edit_value_can_radar_level_rml,
		&m_edit_value_can_radar_level_rmr,
		&m_edit_value_can_radar_level_rr,
	};


	unsigned int id[] = {
		IDC_EDITCAN_RADAR_LEVEL_FL,
		IDC_EDITCAN_RADAR_LEVEL_FML,
		IDC_EDITCAN_RADAR_LEVEL_FMR,
		IDC_EDITCAN_RADAR_LEVEL_FR,
		IDC_EDITCAN_RADAR_LEVEL_RL,
		IDC_EDITCAN_RADAR_LEVEL_RML,
		IDC_EDITCAN_RADAR_LEVEL_RMR,
		IDC_EDITCAN_RADAR_LEVEL_RR,
	};
	UpdateData(true);
	int value = _ttoi(*editlist[which]);

	if (add == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头  
	{
		value--;
	}
	else if (add == 1) // 如果此值为1, 说明点击了Spin的往上的箭头  
	{
		value++;
	}

	if (value < 0)
		value = 0;
	else if (value > 9)
		value = 9;

	editlist[which]->Format(L"%d", value);
	GetDlgItem(id[which])->SetWindowText(*editlist[which]);
	UpdateData(false);
}


void CControlsDlg::OnDeltaposSpinCanRadarLevelFl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_SetRadarLevelString(0, pNMUpDown->iDelta);
	*pResult = 0;
}



void CControlsDlg::OnDeltaposSpinCanRadarLevelFml(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_SetRadarLevelString(1, pNMUpDown->iDelta);
	*pResult = 0;
}


void CControlsDlg::OnDeltaposSpinCanRadarLevelFmr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_SetRadarLevelString(2, pNMUpDown->iDelta);
	*pResult = 0;
}


void CControlsDlg::OnDeltaposSpinCanRadarLevelFr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_SetRadarLevelString(3, pNMUpDown->iDelta);
	*pResult = 0;
}


void CControlsDlg::OnDeltaposSpinCanRadarLevelRl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_SetRadarLevelString(4, pNMUpDown->iDelta);
	*pResult = 0;
}


void CControlsDlg::OnDeltaposSpinCanRadarLevelRml(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_SetRadarLevelString(5, pNMUpDown->iDelta);
	*pResult = 0;
}


void CControlsDlg::OnDeltaposSpinCanRadarLevelRmr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_SetRadarLevelString(6, pNMUpDown->iDelta);
	*pResult = 0;
}


void CControlsDlg::OnDeltaposSpinCanRadarLevelRr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	_SetRadarLevelString(7, pNMUpDown->iDelta);
	*pResult = 0;
}


BOOL CControlsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_InitRadarLevelString();

	m_cbox_radar_min.AddString(L"Long");
	m_cbox_radar_min.AddString(L"40");
	m_cbox_radar_min.AddString(L"60");
	m_cbox_radar_min.AddString(L"80");
	m_cbox_radar_min.AddString(L"100");
	m_cbox_radar_number.AddString(L"F0_R3");
	m_cbox_radar_number.AddString(L"F0_R4");
	m_cbox_radar_number.AddString(L"F2_R3");
	m_cbox_radar_number.AddString(L"F2_R4");
	m_cbox_radar_number.AddString(L"F4_R4");

	m_slider_can_speed.SetRange(0, 180);
	m_slider_can_angle.SetRange(0, 3000);

	m_slider_src_x.SetRange(0, 1920);
	m_slider_src_y.SetRange(0, 1080);

#if 0
	((CButton *)GetDlgItem(IDC_RADIO_CAN_IGN_OFF))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_CAN_LAMP_OFF))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_CAN_RERVSE_OFF))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_CAN_SCREEN_1))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_CAN_3DSIDE_OFF))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_CAN_PDC_OFF))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_CAN_TRAJ_OFF))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_CAN_LDW_OFF))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_CAN_LDW_SEN3))->SetCheck(1);

	m_cbox_radar_min.SetCurSel(0);
	m_cbox_radar_number.SetCurSel(0);

	UpdateData(true);
	m_edit_value_can_angle = L"0";
	m_edit_value_can_speed = L"0";
	UpdateData(false);
#endif


	this->SetTimer(TIMER_MINE_CAN_CONTROL_CYCLE_QUERY, 100, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CControlsDlg::OnBnClickedRadioCanIgn()
{
	if (!m_bInited)
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_IGN_ON))
	{
		m_pCanLogDlg->SendData(L"IGN ON",-1);
	}

	else if (IsDlgButtonChecked(IDC_RADIO_CAN_IGN_ACC))
	{
		m_pCanLogDlg->SendData(L"IGN ACC", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_IGN_OFF))
	{
		m_pCanLogDlg->SendData(L"IGN OFF", -1);
	}
}


void CControlsDlg::OnBnClickedRadioCanLamp()
{
	if (!m_bInited)
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LAMP_LEFT))
	{
		m_pCanLogDlg->SendData(L"TURN LEFT", -1,false);
		m_pCanLogDlg->SendData(L"TURN RIGHT OFF", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LAMP_RIGHT))
	{
		m_pCanLogDlg->SendData(L"TURN LEFT OFF", -1, false);
		m_pCanLogDlg->SendData(L"TURN RIGHT", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LAMP_BOTH))
	{
		m_pCanLogDlg->SendData(L"TURN LEFT", -1, false);
		m_pCanLogDlg->SendData(L"TURN RIGHT", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LAMP_OFF))
	{
		m_pCanLogDlg->SendData(L"TURN LEFT OFF", -1, false);
		m_pCanLogDlg->SendData(L"TURN RIGHT OFF", -1);
	}
}

void CControlsDlg::OnBnClickedRadioCanRervse()
{
	if (!m_bInited) 
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_RERVSE_ON))
	{
		m_pCanLogDlg->SendData(L"REVERSE ON", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_RERVSE_OFF))
	{
		m_pCanLogDlg->SendData(L"REVERSE OFF", -1);
	}
}

void CControlsDlg::OnBnClickedRadioCanScreen()
{
	if (!m_bInited)
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_SCREEN_1))
	{
		m_pCanLogDlg->SendData(L"SCREEN 1", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_SCREEN_2))
	{
		m_pCanLogDlg->SendData(L"SCREEN 2", -1);
	}

	
}

void CControlsDlg::OnBnClickedRadioCan3dside()
{
	if (!m_bInited)
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_3DSIDE_ON))
	{
		m_pCanLogDlg->SendData(L"3DSIDE ON", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_3DSIDE_OFF))
	{
		m_pCanLogDlg->SendData(L"3DSIDE OFF", -1);
	}	
}

void CControlsDlg::OnBnClickedRadioCanPdc()
{	
	if (!m_bInited)
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_PDC_ON))
	{
		m_pCanLogDlg->SendData(L"PDCVIEW ON", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_PDC_OFF))
	{
		m_pCanLogDlg->SendData(L"PDCVIEW OFF", -1);
	}
}

void CControlsDlg::OnBnClickedRadioCanTraj()
{
	if (!m_bInited)
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_TRAJ_ON))
	{
		m_pCanLogDlg->SendData(L"TRAJ ON", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_TRAJ_OFF))
	{
		m_pCanLogDlg->SendData(L"TRAJ OFF", -1);
	}
}


void CControlsDlg::OnBnClickedButtonCanCalib()
{
	if(m_bInited)
		m_pCanLogDlg->SendData(L"CALIBKEY", -1);
}


void CControlsDlg::OnBnClickedButtonCanAvm()
{
	if (m_bInited)
		m_pCanLogDlg->SendData(L"AVMKEY", -1);
}


void CControlsDlg::OnBnClickedButtonCanDvr()
{
	if (m_bInited)
		m_pCanLogDlg->SendData(L"DVRKEY", -1);
}


void CControlsDlg::OnEnChangeEditCanSpeed()
{
	UpdateData(true);
	int speed = 0;
	if (m_bInited)
	{
		speed = CMyString::toInt(m_edit_value_can_speed);
	}
	UpdateData(false);

	m_slider_can_speed.SetPos(speed);
}


void CControlsDlg::OnEnChangeEditCanAngle()
{
	UpdateData(true);
	int angle = 0;
	if (m_bInited)
	{
		angle = CMyString::toInt(m_edit_value_can_angle);
	}
	UpdateData(false);

	m_slider_can_angle.SetPos(angle);
}


void CControlsDlg::OnCbnSelchangeComboCanRadarNumber()
{
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_RADAR_NUMBER", m_cbox_radar_number.GetCurSel());
	}
}


void CControlsDlg::OnCbnSelchangeComboCanRadarMin()
{
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_MIN", m_cbox_radar_min.GetCurSel());
	}
}


void CControlsDlg::OnEnChangeEditcanRadarLevelFl()
{
	UpdateData(true);
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_FL", CMyString::toInt(m_edit_value_can_radar_level_fl));
	}
	UpdateData(false);
}


void CControlsDlg::OnEnChangeEditcanRadarLevelFml()
{
	UpdateData(true);
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_FML", CMyString::toInt(m_edit_value_can_radar_level_fml));
	}
	UpdateData(false);
}


void CControlsDlg::OnEnChangeEditcanRadarLevelFmr()
{
	UpdateData(true);
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_FMR", CMyString::toInt(m_edit_value_can_radar_level_fmr));
	}
	UpdateData(false);
}


void CControlsDlg::OnEnChangeEditcanRadarLevelFr()
{
	UpdateData(true);
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_FR", CMyString::toInt(m_edit_value_can_radar_level_fr));
	}
	UpdateData(false);
}


void CControlsDlg::OnEnChangeEditcanRadarLevelRl()
{
	UpdateData(true);
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_RL", CMyString::toInt(m_edit_value_can_radar_level_rl));
	}
	UpdateData(false);
}


void CControlsDlg::OnEnChangeEditcanRadarLevelRml()
{
	UpdateData(true);
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_RML", CMyString::toInt(m_edit_value_can_radar_level_rml));
	}
	UpdateData(false);
}


void CControlsDlg::OnEnChangeEditcanRadarLevelRmr()
{
	UpdateData(true);
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_RMR", CMyString::toInt(m_edit_value_can_radar_level_rmr));
	}
	UpdateData(false);
}


void CControlsDlg::OnEnChangeEditcanRadarLevelRr()
{
	UpdateData(true);
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"PDC_DIST_LVL_RR", CMyString::toInt(m_edit_value_can_radar_level_rr));
	}
	UpdateData(false);
}


void CControlsDlg::OnBnClickedRadioCanLdw()
{
	if (!m_bInited)
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LDW_ON))
	{
		m_pCanLogDlg->SendData(L"LDW_ON", -1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LDW_OFF))
	{
		m_pCanLogDlg->SendData(L"LDW_OFF", -1);
	}
}


void CControlsDlg::OnBnClickedRadioCanLdwSen()
{
	if (!m_bInited)
	{
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LDW_SEN1))
	{
		m_pCanLogDlg->SendData(L"LDW_SENSOR1", 2);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LDW_SEN2))
	{
		m_pCanLogDlg->SendData(L"LDW_SENSOR2", 1);
	}
	else if (IsDlgButtonChecked(IDC_RADIO_CAN_LDW_SEN3))
	{
		m_pCanLogDlg->SendData(L"LDW_SENSOR3", 0);
	}
}

void CControlsDlg::OnBnClickedCheckCanRadarErrFl()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_ERR_FL))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_FL_SENS_ERR", ret);
	}
}


void CControlsDlg::OnBnClickedCheckCanRadarErrFml()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_ERR_FML))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_FML_SENS_ERR", ret);
	}
}


void CControlsDlg::OnBnClickedCheckCanRadarErrRl()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_ERR_RL))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_RL_SENS_ERR", ret);
	}
}


void CControlsDlg::OnBnClickedCheckCanRadarErrRml()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_ERR_RML))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_RML_SENS_ERR", ret);
	}
}


void CControlsDlg::OnBnClickedCheckCanRadarErrFmr()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_ERR_FMR))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_FMR_SENS_ERR", ret);
	}
}


void CControlsDlg::OnBnClickedCheckCanRadarRmr()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_RMR))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_RMR_SENS_ERR", ret);
	}
}


void CControlsDlg::OnBnClickedCheckCanRadarErrFr()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_ERR_FR))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_FR_SENS_ERR", ret);
	}
}


void CControlsDlg::OnBnClickedCheckCanRadarRr()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_RR))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_RR_SENS_ERR", ret);
	}
}


void CControlsDlg::OnBnClickedCheckCanRadarErrSys()
{
	int ret = 0;

	if (m_bInited)
	{
		if (IsDlgButtonChecked(IDC_CHECK_CAN_RADAR_ERR_SYS))
		{
			ret = 1;
		}
		m_pCanLogDlg->SendData(L"PDC_SYS_SENS_ERR", ret);
	}
}



void CControlsDlg::OnCbnSelchangeComboCanColor()
{
	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"CARCOLOR", m_cbox_can_color.GetCurSel());
	}
}




///// 串口事件、

static CString comCmd[] = {
	L". /svp/etc/env_setup.sh",
	L"sync && reboot -f",
	L"ls /media/sda1>/.null && export myusbpath=/media/sda1 || export myusbpath=/media/hub_usbstorage0; ls $myusbpath>/.null  || export myusbpath=/media/usbstorage1; ls $myusbpath>/.null  && cp -rf $myusbpath/bin /svp && sync && reboot -f || echo '!!!!! ERRORR!!!!!'",
	L"ls /media/sda1>/.null  && export myusbpath=/media/sda1 || export myusbpath=/media/hub_usbstorage0; ls $myusbpath>/.null  || export myusbpath=/media/usbstorage1; ls $myusbpath>/.null  && cp -rf $myusbpath/lib /svp && sync && reboot -f || echo '!!!!! ERRORR!!!!!'",
	L"ls /media/sda1>/.null  && export myusbpath=/media/sda1 || export myusbpath=/media/hub_usbstorage0; ls $myusbpath>/.null  || export myusbpath=/media/usbstorage1; ls $myusbpath>/.null  && rm -rf /svp && sync && mkdir /svp && tar -zxvf $myusbpath/svp.bin -C/svp && sync && reboot -f || echo '!!!!! ERRORR!!!!!'",
	L"flash_erase /dev/mtd6 0 0",
	L"rm -rf /svp/etc/zm/cache && sync",
	L"rm -rf /svp/etc/zm/data && sync",
	L"/svp/test/svp-persist-test erase",
	L"cd /nfs && . nfs.sh",
	L"cd /nfs && . nfssvp.sh",
};
enum
{
	COM_ENV_SETUP,
	COM_REBOOT,
	COM_CPBIN,
	COM_CPLIBS,
	COM_TARSVP,
	COM_ERASE_CALIB,
	COM_ERASE_CACHE,
	COM_ERASE_DATA,
	COM_ERASE_NOR,
	COM_NFS,
	COM_NFSSVP,
};
void CControlsDlg::OnBnClickedButtonComEnvSetup()
{
	m_pComLogDlg->SendData(comCmd[COM_ENV_SETUP]);
}


void CControlsDlg::OnBnClickedButtonComReboot()
{
	m_pComLogDlg->SendData(comCmd[COM_REBOOT]);
}


void CControlsDlg::OnBnClickedButtonComCpbin()
{
	m_pComLogDlg->SendData(comCmd[COM_CPBIN]);
}


void CControlsDlg::OnBnClickedButtonComCplib()
{
	m_pComLogDlg->SendData(comCmd[COM_CPLIBS]);
}


void CControlsDlg::OnBnClickedButtonComTarsvp()
{
	m_pComLogDlg->SendData(comCmd[COM_TARSVP]);
}


void CControlsDlg::OnBnClickedButtonComErasecalib()
{
	m_pComLogDlg->SendData(comCmd[COM_ERASE_CALIB]);
}


void CControlsDlg::OnBnClickedButtonComEraseCache()
{
	m_pComLogDlg->SendData(comCmd[COM_ERASE_CACHE]);
}


void CControlsDlg::OnBnClickedButtonComEraseData()
{
	m_pComLogDlg->SendData(comCmd[COM_ERASE_DATA]);
}


void CControlsDlg::OnBnClickedButtonComEraseNor()
{
	m_pComLogDlg->SendData(comCmd[COM_ERASE_NOR]);
}


void CControlsDlg::OnBnClickedButtonComNfs()
{
	m_pComLogDlg->SendData(comCmd[COM_NFS]);
}


void CControlsDlg::OnBnClickedButtonComNfssvp()
{
	m_pComLogDlg->SendData(comCmd[COM_NFSSVP]);
}


void CControlsDlg::OnBnClickedButtonComCpCalibtousb()
{
	AfxMessageBox(L"not yet");
}


void CControlsDlg::OnBnClickedButtonComCpCalibtousb3()
{
	AfxMessageBox(L"not yet");
}


void CControlsDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_MINE_CAN_CAPTURE_SCREEN:
		m_pCanLogDlg->SendData(L"TOUCHSCRN_X", 790,false);
		m_pCanLogDlg->SendData(L"TOUCHSCRN_Y", 470, false);

		m_pCanLogDlg->SendData(L"TOUCHSCRNSTATUS", 0);
		KillTimer(TIMER_MINE_CAN_CAPTURE_SCREEN);
		GetDlgItem(IDC_BUTTON_CAPTURE)->EnableWindow(true);
		break;
	case TIMER_MINE_CAN_CONTROL_CYCLE_QUERY:
		if (m_bInited)
		{
			static int mavm = -1, m3d = -1, mpdc = -1, mldw = -1;
			static int msen = -1;

			if ((int)m_pCanLogDlg->GetAvmVideoOnOffStatus() != mavm)
			{
				if (m_pCanLogDlg->GetAvmVideoOnOffStatus())
					SetDlgItemText(IDC_TXT_RCV1, L"AVM");
				else
					SetDlgItemText(IDC_TXT_RCV1, L"导航");

				mavm = m_pCanLogDlg->GetAvmVideoOnOffStatus();
			}

			if ((int)m_pCanLogDlg->GetSideViewOnOff() != m3d)
			{
				if (m_pCanLogDlg->GetSideViewOnOff())
					SetDlgItemText(IDC_TXT_RCV2, L"开");
				else
					SetDlgItemText(IDC_TXT_RCV2, L"关");

				m3d = m_pCanLogDlg->GetSideViewOnOff();
			}


			if ((int)m_pCanLogDlg->GetPdcOnOff() != mpdc)
			{
				if (m_pCanLogDlg->GetPdcOnOff())
					SetDlgItemText(IDC_TXT_RCV3, L"开");
				else
					SetDlgItemText(IDC_TXT_RCV3, L"关");

				mpdc = m_pCanLogDlg->GetPdcOnOff();
			}


			if ((int)m_pCanLogDlg->GetLdwOnOff() != mldw)
			{
				if (m_pCanLogDlg->GetLdwOnOff())
					SetDlgItemText(IDC_TXT_RCV4, L"开");
				else
					SetDlgItemText(IDC_TXT_RCV4, L"关");

				mldw = m_pCanLogDlg->GetLdwOnOff();
			}

			if (m_pCanLogDlg->GetLdwSen() != msen)
			{
				CString tmp;
				msen = m_pCanLogDlg->GetLdwSen();
				tmp.Format(L"%d", msen);
				SetDlgItemText(IDC_TXT_RCV5, tmp);

			}
		}
		break;
	}
}


void CControlsDlg::OnOK()
{
}


void CControlsDlg::OnNMCustomdrawSliderCanSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	static int i = -1;
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	CString strSpd;
	int speed = m_slider_can_speed.GetPos();
	if (i != speed)
	{
		strSpd.Format(L"%d", speed);
		UpdateData(true);
		m_edit_value_can_speed = strSpd;
		UpdateData(false);
		if (m_bInited)
			m_pCanLogDlg->SendData(L"VEHICLESPEED", speed);
		i = speed;
	}
}


void CControlsDlg::OnNMCustomdrawSliderCanAngle(NMHDR *pNMHDR, LRESULT *pResult)
{
	static int i = -1;
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	CString strAngle;
	int angle = m_slider_can_angle.GetPos();
#if 1
	if (i != angle)
	{
		strAngle.Format(L"%d", angle);
		UpdateData(true);
		m_edit_value_can_angle = strAngle;
		UpdateData(false);
		if (m_bInited)
			m_pCanLogDlg->SendData(L"STEERINGANGLE", angle);
		i = angle;
	}
#endif
}



void CControlsDlg::OnBnClickedButtonCanScrn()
{
	CString x, y;
	int ix, iy;
	m_edit_src_x.GetWindowText(x);
	m_edit_src_y.GetWindowText(y);

	ix = CMyString::toInt(x);
	iy = CMyString::toInt(y);

	if (m_bInited)
	{
		m_pCanLogDlg->SendData(L"TOUCHSCRN_X", ix, false);
		m_pCanLogDlg->SendData(L"TOUCHSCRN_Y", iy, false);


		m_pCanLogDlg->SendData(L"TOUCHSCRNSTATUS", 1);
		Sleep(50);
		m_pCanLogDlg->SendData(L"TOUCHSCRNSTATUS", 0);
		
	}
}


void CControlsDlg::OnNMCustomdrawSliderCanScrnX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);	
	*pResult = 0;

	CString str;
	int pos = m_slider_src_x.GetPos();	
	str.Format(L"%d", pos);
	m_edit_src_x.SetWindowText(str);
		
}


void CControlsDlg::OnNMCustomdrawSliderCanScrnY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	CString str;
	int pos = m_slider_src_y.GetPos();
	str.Format(L"%d", pos);
	m_edit_src_y.SetWindowText(str);
}


void CControlsDlg::OnBnClickedButtonCapture()
{
	m_pCanLogDlg->SendData(L"TOUCHSCRN_X", 10, false);
	m_pCanLogDlg->SendData(L"TOUCHSCRN_Y", 10, false);

	m_pCanLogDlg->SendData(L"TOUCHSCRNSTATUS", 1);

	this->SetTimer(TIMER_MINE_CAN_CAPTURE_SCREEN,6000,NULL);

	GetDlgItem(IDC_BUTTON_CAPTURE)->EnableWindow(false);
}


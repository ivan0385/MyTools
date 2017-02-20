// CanSettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "CanSettingDialog.h"
#include "afxdialogex.h"
#include "CanSettingPresenter.h"
#include "common.h"

// CCanSettingDialog 对话框

IMPLEMENT_DYNAMIC(CCanSettingDialog, CDialogEx)

CCanSettingDialog::CCanSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCanSettingDialog::IDD, pParent)
	, m_pCanSettingPresenter(NULL)
{
	m_pCanSettingPresenter = new CanSettingPresenter(this);
}

CCanSettingDialog::~CCanSettingDialog()
{
	delete m_pCanSettingPresenter;
}

void CCanSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_CAN_CHANNEL, m_cBoxChannel);
	DDX_Control(pDX, IDC_CB_CAN_RATE, m_cBoxBitRate);
}


BEGIN_MESSAGE_MAP(CCanSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CAN_OPEN_PORT, &CCanSettingDialog::OnBnClickedBtnCanOpenPort)
	ON_BN_CLICKED(IDC_BTN_CAN_CLOSE_PORT, &CCanSettingDialog::OnBnClickedBtnCanClosePort)
END_MESSAGE_MAP()


// CCanSettingDialog 消息处理程序


CanBus CCanSettingDialog::GetCanBus()
{
	CanBus can;
	CString value;
	m_cBoxChannel.GetWindowText(value);
	can.SetChannelNo(value);

	m_cBoxBitRate.GetWindowText(value);
	can.SetBitRate(value);

	return can;
}

bool CCanSettingDialog::SetCanBus(CanBus info)
{
	m_cBoxChannel.SetWindowText(info.GetChannelNo());
	m_cBoxBitRate.SetWindowText(info.GetBitRate());
	return true;
}
bool CCanSettingDialog::CanDeivceIsOpened()
{
	(CButton *)this->GetDlgItem(IDC_BTN_CAN_OPEN_PORT)->EnableWindow(false);
	(CButton *)this->GetDlgItem(IDC_BTN_CAN_CLOSE_PORT)->EnableWindow(true);
	::SendMessage(this->GetParent()->m_hWnd, WM_MINE_MAIN_CAN_CONNECT, 0, 0);
	return true;
}
bool CCanSettingDialog::CanDeivceIsClosed()
{
	(CButton *)this->GetDlgItem(IDC_BTN_CAN_OPEN_PORT)->EnableWindow(true);
	(CButton *)this->GetDlgItem(IDC_BTN_CAN_CLOSE_PORT)->EnableWindow(false);

	::SendMessage(this->GetParent()->m_hWnd,WM_MINE_MAIN_CAN_DISCONNECT,0,0);
	return true;
}
bool CCanSettingDialog::CanDeivceIsError()
{
	return CanDeivceIsClosed();
}


void CCanSettingDialog::CanBusOpenClose()
{
	if ((CButton *)this->GetDlgItem(IDC_BTN_CAN_OPEN_PORT)->IsWindowEnabled())
	{
		m_pCanSettingPresenter->OpenCanDevice(GetCanBus());
	}
	else
	{
		m_pCanSettingPresenter->CloseCanDevice();
	}
}



void CCanSettingDialog::OnBnClickedBtnCanOpenPort()
{
	m_pCanSettingPresenter->OpenCanDevice(GetCanBus());
}


void CCanSettingDialog::OnBnClickedBtnCanClosePort()
{
	m_pCanSettingPresenter->CloseCanDevice();
}


BOOL CCanSettingDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pCanSettingPresenter->Init();

	CComboBox * pcb = (CComboBox *)(GetDlgItem(IDC_CB_CAN_WORK_MODE));
	pcb->SetWindowText(L"正常工作");

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CCanSettingDialog::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	__super::OnOK();
}

// ComSettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "ComSettingDialog.h"
#include "afxdialogex.h"
#include "common.h"

// CComSettingDialog 对话框

IMPLEMENT_DYNAMIC(CComSettingDialog, CDialogEx)

CComSettingDialog::CComSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CComSettingDialog::IDD, pParent)
{
	m_pComSettingPresenter = new ComSettingPresenter(this);
}

CComSettingDialog::~CComSettingDialog()
{
	delete m_pComSettingPresenter;
}

void CComSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_COM_PORT, m_PortNumber);
	DDX_Control(pDX, IDC_CB_COM_RATE, m_PortRate);
	DDX_Control(pDX, IDC_CB_COM_BITRATE, m_PortDataBit);
	DDX_Control(pDX, IDC_CB_COM_STOPBIT, m_PortStopBit);
	DDX_Control(pDX, IDC_CB_COM_PARITY, m_PortParity);
}


BEGIN_MESSAGE_MAP(CComSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_COM_OPEN_PORT, &CComSettingDialog::OnBnClickedBtnComOpenPort)
	ON_BN_CLICKED(IDC_BTN_COM_CLOSE_PORT, &CComSettingDialog::OnBnClickedBtnComClosePort)
END_MESSAGE_MAP()


// CComSettingDialog 消息处理程序


BOOL CComSettingDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pComSettingPresenter->Init();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CComSettingDialog::OnBnClickedBtnComOpenPort()
{
	m_pComSettingPresenter->OpenComPort(GetComPort());
}


void CComSettingDialog::OnBnClickedBtnComClosePort()
{
	m_pComSettingPresenter->CloseComPort();
}


bool CComSettingDialog::InitComPortName(CArray<CString> * list)
{
	for (int i=0; i < list->GetSize();i++)
		m_PortNumber.AddString(list->GetAt(i));
	m_PortNumber.SetDroppedWidth(150);
	return true;
}

ComPort CComSettingDialog::GetComPort()
{
	ComPort port;
	CString value;
	m_PortNumber.GetWindowText(value);
	port.SetPortName(value);

	m_PortRate.GetWindowText(value);
	port.SetBaudRate(value);

	m_PortDataBit.GetWindowText(value);
	port.SetDataBit(value);

	m_PortStopBit.GetWindowText(value);
	port.SetStopBit(value);

	m_PortParity.GetWindowText(value);
	port.SetParity(value);
	return port;
}

bool CComSettingDialog::SetComPort(ComPort info)
{
	m_PortNumber.SetWindowText(info.GetPortName());
	m_PortRate.SetWindowText(info.GetBaudRate());
	m_PortDataBit.SetWindowText(info.GetDataBit());
	m_PortStopBit.SetWindowText(info.GetStopBit());
	m_PortParity.SetWindowText(info.GetParity());
	return true;
}

bool CComSettingDialog::ComPortIsOpened()
{
	(CButton *)this->GetDlgItem(IDC_BTN_COM_OPEN_PORT)->EnableWindow(false);
	(CButton *)this->GetDlgItem(IDC_BTN_COM_CLOSE_PORT)->EnableWindow(true);
	::SendMessage(this->GetParent()->m_hWnd, WM_MINE_MAIN_COM_CONNECT, 0, 0);
	return true;
}

bool CComSettingDialog::ComPortIsClosed()
{
	(CButton *)this->GetDlgItem(IDC_BTN_COM_OPEN_PORT)->EnableWindow(true);
	(CButton *)this->GetDlgItem(IDC_BTN_COM_CLOSE_PORT)->EnableWindow(false);
	::SendMessage(this->GetParent()->m_hWnd, WM_MINE_MAIN_COM_DISCONNECT, 0, 0);
	return true;
}

bool CComSettingDialog::ComPortIsError()
{
	return ComPortIsClosed();
}



void CComSettingDialog::ComPortOpenClose()
{
	if ((CButton *)this->GetDlgItem(IDC_BTN_COM_OPEN_PORT)->IsWindowEnabled())
	{
		m_pComSettingPresenter->OpenComPort(GetComPort());
	}
	else
	{
		m_pComSettingPresenter->CloseComPort();
	}
}


void CComSettingDialog::OnOK()
{
}

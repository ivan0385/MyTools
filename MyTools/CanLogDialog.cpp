// CanLogDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "CanLogDialog.h"
#include "afxdialogex.h"
#include "CanLogPresenter.h"
#include "common.h"

// CCanLogDialog 对话框

IMPLEMENT_DYNAMIC(CCanLogDialog, CDialogEx)

CCanLogDialog::CCanLogDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCanLogDialog::IDD, pParent)
	, m_value(0)
	, m_valueStep(0)
{
	m_pCanLogPresenter = new CanLogPresenter(this);
}

CCanLogDialog::~CCanLogDialog()
{
	delete m_pCanLogPresenter;
}

void CCanLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CAN_SND, m_list_can_snd);
	DDX_Control(pDX, IDC_LIST_CAN_RCV, m_list_can_rcv);
	DDX_Control(pDX, IDC_EDIT_CANLOG_SEND, m_editCanLogMsgData);
	DDX_Control(pDX, IDC_EDITCAN_LOG_SEND_ID, m_editCanLogMsgID);
	DDX_Control(pDX, IDC_EDIT_CANLOG_SEND_TM1, m_editCanLogMsgCycle);
	DDX_Control(pDX, IDC_EDIT_CANLOG_SIGV1, m_editCanLogSigV1);
	DDX_Control(pDX, IDC_EDIT_CANLOG_SIGV2, m_editCanLogSigV2);
	DDX_Control(pDX, IDC_EDIT_CANLOG_SEND_TM2, m_editCanLogSigTestCycle);
	DDX_Control(pDX, IDC_COMBO_CANLOG_MSG, m_cboxCanLogMsg);
	DDX_Control(pDX, IDC_COMBO_CANLOG_SIG, m_cboxCanLogSig);
	DDX_Control(pDX, IDC_EDIT_CAN_MSG_ID, m_editCanLogSndMsgId);
	DDX_Control(pDX, IDC_EDIT_CANLOG_SIG_STEP, m_editCanLogSigStep);
}


BEGIN_MESSAGE_MAP(CCanLogDialog, CDialogEx)
	ON_MESSAGE(WM_MINE_CANLOG_RCV_MSG, &CCanLogDialog::OnMineCanlogRcvMsg)
	ON_MESSAGE(WM_MINE_CANLOG_SND_MSG, &CCanLogDialog::OnMineCanlogSndMsg)
	ON_BN_CLICKED(IDC_BUTTON_CANLOG_SND, &CCanLogDialog::OnBnClickedButtonCanlogSnd)
	ON_BN_CLICKED(IDC_BUTTON_CANLOG_SNDSTOP, &CCanLogDialog::OnBnClickedButtonCanlogSndstop)
	ON_BN_CLICKED(IDC_BUTTON_CANLOG_REFRESH, &CCanLogDialog::OnBnClickedButtonCanlogRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CAN_LOG_START, &CCanLogDialog::OnBnClickedButtonCanLogStart)
	ON_BN_CLICKED(IDC_BUTTON_CANLOG_STOP, &CCanLogDialog::OnBnClickedButtonCanlogStop)
	ON_CBN_SELCHANGE(IDC_COMBO_CANLOG_MSG, &CCanLogDialog::OnCbnSelchangeComboCanlogMsg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLN_1, &CCanLogDialog::OnBnClickedButtonCln1)
	ON_BN_CLICKED(IDC_BUTTON_CLN_2, &CCanLogDialog::OnBnClickedButtonCln2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCanLogDialog 消息处理程序

bool CCanLogDialog::SetRcvMsg(const VCI_CAN_OBJ * msg, int size)
{
	m_locker.lock();
	for (int i = 0; i < size; i++)
		m_rcvBuffer.AddTail(msg[i]);
	m_locker.unlock();
	this->PostMessage(WM_MINE_CANLOG_RCV_MSG, size, 0);
	return true;
}

bool CCanLogDialog::SetSendMsg(const VCI_CAN_OBJ * ba, int cb)
{
	m_sndlocker.lock();
	for (int i = 0; i < cb; i++)
		m_sndBuffer.AddTail(ba[i]);
	m_sndlocker.unlock();
	this->PostMessage(WM_MINE_CANLOG_SND_MSG, cb, 0);
	return true;
}

void CCanLogDialog::SendData(CString msg, int value, bool bsend )
{
	m_pCanLogPresenter->SendData(msg, value, bsend);
}


void CCanLogDialog::SetCurPrj(CString vallue)
{
	m_pCanLogPresenter->SetCurPrj(vallue);
}


void CCanLogDialog::OnOK()
{
}


BOOL CCanLogDialog::OnInitDialog()
{
	__super::OnInitDialog();

	RECT  m_rect;
	DWORD dwStyle = m_list_can_snd.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_list_can_snd.SetExtendedStyle(dwStyle); //设置扩展风格
	m_list_can_snd.GetClientRect(&m_rect); //获取list的客户区,方便调节每一列的宽度  
	m_list_can_snd.InsertColumn(0, _T("时间"), LVCFMT_LEFT, 90);
	m_list_can_snd.InsertColumn(1, _T("帧ID"), LVCFMT_LEFT, 80);
	m_list_can_snd.InsertColumn(2, _T("长度"), LVCFMT_LEFT, 80);
	m_list_can_snd.InsertColumn(3, _T("数据"), LVCFMT_LEFT, 250);

	dwStyle = m_list_can_rcv.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_list_can_rcv.SetExtendedStyle(dwStyle); //设置扩展风格
	m_list_can_rcv.GetClientRect(&m_rect); //获取list的客户区,方便调节每一列的宽度  
	m_list_can_rcv.InsertColumn(0, _T("时间"), LVCFMT_LEFT, 90);
	m_list_can_rcv.InsertColumn(1, _T("帧ID"), LVCFMT_LEFT, 80);
	m_list_can_rcv.InsertColumn(2, _T("长度"), LVCFMT_LEFT, 80);
	m_list_can_rcv.InsertColumn(3, _T("数据"), LVCFMT_LEFT, 250);
	
	m_editCanLogMsgData.SetWindowText(L"00 00 00 00 00 00 00 00");
	m_editCanLogMsgID.SetWindowText(L"00 00 00 00");
	m_editCanLogMsgCycle.SetWindowText(L"0");
	m_editCanLogSigV1.SetWindowText(L"0");
	m_editCanLogSigV2.SetWindowText(L"0");
	m_editCanLogSigTestCycle.SetWindowText(L"0");
	m_editCanLogSigStep.SetWindowText(L"1");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


afx_msg LRESULT CCanLogDialog::OnMineCanlogRcvMsg(WPARAM wParam, LPARAM lParam)
{
	int nRow = 0, nCol = 1;
	CString tmp;
	CString data;
	while (wParam-->=0)
	{
		if (m_rcvBuffer.IsEmpty())
			break;
		m_locker.lock();
		VCI_CAN_OBJ msg = m_rcvBuffer.RemoveHead();
		m_locker.unlock();
		if (!IsDlgButtonChecked(IDC_CHECK_RCV))
			continue;
		nCol = 1;
		nRow = m_list_can_rcv.GetItemCount();
		nRow = m_list_can_rcv.InsertItem(nRow, CMyString::GetNowTime());
		tmp.Format(L"%d(0x%x)", msg.ID, msg.ID);
		m_list_can_rcv.SetItemText(nRow, nCol++, tmp);
		tmp.Format(L"%d", msg.DataLen);
		m_list_can_rcv.SetItemText(nRow, nCol++, tmp);

		tmp = L"0x";
		for (int i=0; i < msg.DataLen; i++)
		{
			data.Format(L":%02x", msg.Data[i]);
			tmp.Append(data);
		}
		m_list_can_rcv.SetItemText(nRow, nCol++, tmp);
	}
	
	return 0;
}


afx_msg LRESULT CCanLogDialog::OnMineCanlogSndMsg(WPARAM wParam, LPARAM lParam)
{
	int ret = 0;
	int nRow = 0, nCol = 1;
	CString tmp;
	CString data;
	while (wParam-->=0)
	{
		if (m_sndBuffer.IsEmpty())
			break;
		m_sndlocker.lock();
		VCI_CAN_OBJ msg = m_sndBuffer.RemoveHead();
		m_sndlocker.unlock();
		if (!IsDlgButtonChecked(IDC_CHECK_SND))
			continue;

		nCol = 1;
		nRow = m_list_can_snd.GetItemCount();
		nRow = m_list_can_snd.InsertItem(nRow, CMyString::GetNowTime());
		tmp.Format(L"%d(0x%x)", msg.ID, msg.ID);
		m_list_can_snd.SetItemText(nRow, nCol++, tmp);
		tmp.Format(L"%d", msg.DataLen);
		m_list_can_snd.SetItemText(nRow, nCol++, tmp);

		tmp = L"0x";
		for (int i = 0; i < msg.DataLen; i++)
		{
			data.Format(L":%02x", msg.Data[i]);
			tmp.Append(data);
		}
		m_list_can_snd.SetItemText(nRow, nCol++, tmp);
		ret = 1;
	}
	if (ret)
	{
		//m_list_can_snd.EnsureVisible(m_list_can_snd.GetItemCount()-1, TRUE);//滚动到某一行
		//m_list_can_snd.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED); // 选中某一行
	}
	return 0;
}


void CCanLogDialog::OnBnClickedButtonCanlogSnd()
{
	ST_PACKET packetBuf;
	CString id, data, cycle;
	BYTE hex[10];
	
	m_editCanLogMsgData.GetWindowTextW(data);
	m_editCanLogMsgID.GetWindowTextW(id);
	m_editCanLogMsgCycle.GetWindowTextW(cycle);

	CMyString::String2Hex(id, hex);
	packetBuf.id = hex[0] << 8 | hex[1];

	CMyString::String2Hex(data, hex);
	for (int i = 0; i < 8; i++)
		packetBuf.data[i] = hex[i];

	packetBuf.tmInterval = _ttoi(cycle);

	packetBuf.enable = 1;

	if (m_pCanLogPresenter->SendData(packetBuf))
	{
		m_editCanLogMsgData.EnableWindow(false);
		m_editCanLogMsgID.EnableWindow(false);
		m_editCanLogMsgCycle.EnableWindow(false);
		(CButton*)GetDlgItem(IDC_BUTTON_CANLOG_SND)->EnableWindow(false);
		(CButton*)GetDlgItem(IDC_BUTTON_CANLOG_SNDSTOP)->EnableWindow(true);
	}
}


void CCanLogDialog::OnBnClickedButtonCanlogSndstop()
{
	ST_PACKET packetBuf;
	CString id, data, cycle;
	BYTE hex[10];

	m_editCanLogMsgData.GetWindowTextW(data);
	m_editCanLogMsgID.GetWindowTextW(id);
	m_editCanLogMsgCycle.GetWindowTextW(cycle);

	CMyString::String2Hex(id, hex);
	packetBuf.id = hex[0] << 8 | hex[1];

	CMyString::String2Hex(data, hex);
	for (int i = 0; i < 8; i++)
		packetBuf.data[i] = hex[i];

	packetBuf.tmInterval = _ttoi(cycle);

	packetBuf.enable = 0;

	m_editCanLogMsgData.EnableWindow(true);
	m_editCanLogMsgID.EnableWindow(true);
	m_editCanLogMsgCycle.EnableWindow(true);
	(CButton*)GetDlgItem(IDC_BUTTON_CANLOG_SND)->EnableWindow(true);
	(CButton*)GetDlgItem(IDC_BUTTON_CANLOG_SNDSTOP)->EnableWindow(false);
}


void CCanLogDialog::OnBnClickedButtonCanlogRefresh()
{
	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	CanDbc * data = pCanTaskModel->GetCanMatrix();

	CAN_MATRIX matrix = data->GetCanMatrix();
	m_cboxCanLogMsg.ResetContent();	
	m_cboxCanLogSig.ResetContent();
	m_editCanLogSndMsgId.SetWindowText(L"");
	for (unsigned int i = 0; i < matrix.msgList.size(); i++)
	{
		if (matrix.msgList[i].msgIsEnable)
		{
			m_cboxCanLogMsg.AddString(CMyString::toCString(matrix.msgList[i].msgName));
		}
	}
}


void CCanLogDialog::OnCbnSelchangeComboCanlogMsg()
{

#if 1
	CString tmp;
	m_cboxCanLogMsg.GetLBText(m_cboxCanLogMsg.GetCurSel(), tmp);
	std::string msgName = CMyString::toString(tmp);
#endif

	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	CanDbc * data = pCanTaskModel->GetCanMatrix();

	CAN_MATRIX matrix = data->GetCanMatrix();
	m_cboxCanLogSig.ResetContent();
	for (unsigned int i = 0; i < matrix.msgList.size(); i++)
	{
		if (matrix.msgList[i].msgName == msgName)
		{
			for (unsigned int j = 0; j< matrix.msgList[i].signalList.size(); j++)
				m_cboxCanLogSig.AddString(CMyString::toCString(matrix.msgList[i].signalList[j].sgnlName));

			tmp.Format(L"%d", matrix.msgList[i].msgId);
			m_editCanLogSndMsgId.SetWindowText(tmp);

			break;
		}
	}
}


void CCanLogDialog::OnBnClickedButtonCanLogStart()
{
	CString tmp;

	m_editCanLogSigTestCycle.GetWindowText(tmp);
	int cycle = CMyString::toInt(tmp);
	m_editCanLogSigV1.GetWindowText(tmp);
	int min = CMyString::toInt(tmp);
	m_editCanLogSigV2.GetWindowText(tmp);
	int max = CMyString::toInt(tmp);
	m_editCanLogSndMsgId.GetWindowText(tmp);
	int msgid = CMyString::toInt(tmp);
	m_editCanLogSigStep.GetWindowText(tmp);
	int step = CMyString::toInt(tmp);
	m_cboxCanLogSig.GetWindowText(tmp);

	m_valueStep = step;

	if (msgid <= 0)
	{
		AfxMessageBox(L"请选择报文");
		m_cboxCanLogMsg.SetFocus();
		return;
	}

	if (tmp.IsEmpty())
	{
		AfxMessageBox(L"请选择信号");
		m_cboxCanLogSig.SetFocus();
		return;
	}

	if (cycle < 10)
	{
		AfxMessageBox(L"重复周期不能小于10ms");
		m_editCanLogSigTestCycle.SetFocus();
		return;
	}

	if (max <= 0)
	{
		AfxMessageBox(L"最大值必须大于0");
		m_editCanLogSigV2.SetFocus();
		return;
	}

	m_value = min;
	if (m_pCanLogPresenter->SendData(msgid, tmp, m_value))
	{
		m_cboxCanLogMsg.EnableWindow(false);
		m_cboxCanLogSig.EnableWindow(false);
		m_editCanLogSigV1.EnableWindow(false);
		m_editCanLogSigV2.EnableWindow(false);
		m_editCanLogSigTestCycle.EnableWindow(false);
		(CButton*)GetDlgItem(IDC_BUTTON_CANLOG_REFRESH)->EnableWindow(false);
		(CButton*)GetDlgItem(IDC_BUTTON_CAN_LOG_START)->EnableWindow(false);
		(CButton*)GetDlgItem(IDC_BUTTON_CANLOG_STOP)->EnableWindow(true);
		SetTimer(TIMER_MINE_CAN_SND_CYCLE_TEST, cycle, 0);
	}	
}


void CCanLogDialog::OnBnClickedButtonCanlogStop()
{
	m_cboxCanLogMsg.EnableWindow(true);
	m_cboxCanLogSig.EnableWindow(true);
	m_editCanLogSigV1.EnableWindow(true);
	m_editCanLogSigV2.EnableWindow(true);
	m_editCanLogSigTestCycle.EnableWindow(true);
	(CButton*)GetDlgItem(IDC_BUTTON_CANLOG_REFRESH)->EnableWindow(true);
	(CButton*)GetDlgItem(IDC_BUTTON_CAN_LOG_START)->EnableWindow(true);
	(CButton*)GetDlgItem(IDC_BUTTON_CANLOG_STOP)->EnableWindow(false);
	KillTimer(TIMER_MINE_CAN_SND_CYCLE_TEST);
}



void CCanLogDialog::OnTimer(UINT_PTR nIDEvent)
{	
	if (TIMER_MINE_CAN_SND_CYCLE_TEST == nIDEvent)
	{
		CString tmp;
		m_editCanLogSigV1.GetWindowText(tmp);
		int min = CMyString::toInt(tmp);
		m_editCanLogSigV2.GetWindowText(tmp);
		int max = CMyString::toInt(tmp);
		m_editCanLogSndMsgId.GetWindowText(tmp);
		int msgid = CMyString::toInt(tmp);
		m_cboxCanLogSig.GetWindowText(tmp);

		m_value += m_valueStep;
		if (m_value > max)
		{
			OnBnClickedButtonCanlogStop();
			m_value = min;
		}
		if(!m_pCanLogPresenter->SendData(msgid, tmp, m_value))
			KillTimer(TIMER_MINE_CAN_SND_CYCLE_TEST);
	}
}


bool CCanLogDialog::GetAvmVideoOnOffStatus()
{
	
	return m_pCanLogPresenter->GetAvmVideoOnOffStatus();
}


bool CCanLogDialog::GetPdcOnOff()
{
	return m_pCanLogPresenter->GetPdcOnOff();
}


bool CCanLogDialog::GetSideViewOnOff()
{
	return m_pCanLogPresenter->GetSideViewOnOff();
}


bool CCanLogDialog::GetLdwOnOff()
{
	return m_pCanLogPresenter->GetLdwOnOff();
}


int CCanLogDialog::GetLdwSen()
{
	return m_pCanLogPresenter->GetLdwSen();
}



void CCanLogDialog::OnBnClickedButtonCln1()
{
	m_list_can_rcv.DeleteAllItems();
}


void CCanLogDialog::OnBnClickedButtonCln2()
{
	m_list_can_snd.DeleteAllItems();
}


void CCanLogDialog::OnClose()
{
	this->m_pParentWnd->SetFocus();

	__super::OnClose();
}

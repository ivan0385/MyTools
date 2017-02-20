// EolTestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "EolTestDialog.h"
#include "afxdialogex.h"
#include "common.h"


// CEolTestDialog 对话框

IMPLEMENT_DYNAMIC(CEolTestDialog, CDialogEx)

CEolTestDialog::CEolTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEolTestDialog::IDD, pParent)
	, m_bRuning(false)
{
	m_pPresenter = new EolTestPresenter(this);
	m_pFont = NULL;
}

CEolTestDialog::~CEolTestDialog()
{
	if (m_pFont)
		delete m_pFont;
	delete m_pPresenter;
}

void CEolTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_EOL_LOG, m_edit_log);
	DDX_Control(pDX, IDC_PROGRESS_EOL, m_progress);
	DDX_Control(pDX, IDC_LIST_EOL, m_list);
	DDX_Control(pDX, IDC_EDIT_EOL_CUR, m_edit_cur);
	DDX_Control(pDX, IDC_EDIT_EOL_RST, m_edit_result);
	DDX_Control(pDX, IDC_EDIT_EOL_SND, m_edit_snd);
	DDX_Control(pDX, IDC_MFCBUTTON_EOL_SND, m_btn_eol_snd);
}


BEGIN_MESSAGE_MAP(CEolTestDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_EOL_START, &CEolTestDialog::OnBnClickedButtonEolStart)
	ON_BN_CLICKED(IDC_BUTTON_EOL_PAUSE, &CEolTestDialog::OnBnClickedButtonEolPause)
	ON_BN_CLICKED(IDC_BUTTON_EOL_STOP, &CEolTestDialog::OnBnClickedButtonEolStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MFCBUTTON_EOL_SND, &CEolTestDialog::OnBnClickedMfcbuttonEolSnd)
	ON_EN_SETFOCUS(IDC_EDIT_EOL_SND, &CEolTestDialog::OnEnSetfocusEditEolSnd)
END_MESSAGE_MAP()


// CEolTestDialog 消息处理程序



void CEolTestDialog::initTcList()
{
	tcList.clear();
	tcList.push_back({ L"请15秒内重启盒子", L"", L"05 FF 00 FF FF FA", -1, 15000, 100 });

	tcList.push_back({ L"REQ_TO_ENTER_EOL", L"FF 01 FF FF", L"FF 01 FF FF", 0, 4000, 7000 });

	tcList.push_back({ L"GET_M3_VERSION", L"FF 30 FF FF", L"FF 30 FF FF", 2, 2000, 1000 });
	tcList.push_back({ L"GET_J6_OS_VERSION", L"FF 32 FF FF", L"FF 32 FF FF", 2, 2000, 1000 });
	tcList.push_back({ L"GET_J6_SVP_VERSION", L"FF 33 FF FF", L"FF 33 FF FF", 2, 2000, 1000 });
	tcList.push_back({ L"GET_J6_MAIN_VERSION", L"FF 3C FF FF", L"FF 3C FF FF", 2, 2000, 1000 });
	tcList.push_back({ L"GET_J6_BSP_VERSION", L"FF 3D FF FF", L"FF 3D FF FF", 2, 2000, 1000 });

	tcList.push_back({ L"GET_ECUSerialNumber", L"FF 35 FF FF", L"FF 35 FF FF", 2, 2000, 1000 });

	//tcList.push_back({ L"READ_M3_DTC",  L"FF 36 FF FF", L"FF 36 FF FF", 1, 2000, 100 });
	//tcList.push_back({ L"CLEAR_M3_DTC", L"FF 37 FF FF", L"FF 37 FF FF", 1, 2000, 100 });

	tcList.push_back({ L"SET_VEHICLE", L"FF 3A FF FF 02", L"FF 3A FF FF", 0, 2000, 1000 });
	tcList.push_back({ L"GET_VEHICLE", L"FF 3B FF FF", L"FF 3B FF FF", 1, 2000, 1000 });

	tcList.push_back({ L"ENTER_AVM_PRESS_IN", L"FF 51 FF FF 01", L"FF 51 FF FF", 0, 2000, 2000 });
	tcList.push_back({ L"ENTER_AVM_PRESS_OUT", L"FF 51 FF FF 00", L"FF 51 FF FF", 0, 2000, 2000 });

	tcList.push_back({ L"ENTER_REVERSE_ON", L"FF 52 FF FF 01", L"FF 52 FF FF", 0, 2000, 2000 });
	tcList.push_back({ L"ENTER_REVERSE_OFF", L"FF 52 FF FF 00", L"FF 52 FF FF", 0, 2000, 2000 });

	tcList.push_back({ L"Control_Camera_FRONT", L"FF 55 FF FF 01", L"FF 55 FF FF", 0, 2000, 2000 });
	tcList.push_back({ L"Control_Camera_REAR", L"FF 55 FF FF 02", L"FF 55 FF FF", 0, 2000, 2000 });
	tcList.push_back({ L"Control_Camera_LEFT", L"FF 55 FF FF 03", L"FF 55 FF FF", 0, 2000, 2000 });
	tcList.push_back({ L"Control_Camera_RIGHT", L"FF 55 FF FF 04", L"FF 55 FF FF", 0, 2000, 2000 });
	tcList.push_back({ L"Control_Camera_OFF", L"FF 55 FF FF 00", L"FF 55 FF FF", 0, 2000, 1000 });

	tcList.push_back({ L"HW_TEST_USB", L"FF 65 FF FF 00", L"FF 65 FF FF", 0, 2000, 1000 });
	tcList.push_back({ L"HW_TEST_PWR_DET", L"FF 60 FF FF 00", L"FF 60 FF FF", 0, 2000, 1000 });
	tcList.push_back({ L"HW_TEST_CAN_DET", L"FF 61 FF FF 00", L"FF 61 FF FF", 0, 2000, 1000 });
	tcList.push_back({ L"HW_TEST_IGN_ON", L"FF 62 FF FF 00", L"FF 62 FF FF", 0, 2000, 1000 });
	tcList.push_back({ L"HW_TEST_AVM_IN_DET", L"FF 64 FF FF 00", L"FF 64 FF FF", 0, 2000, 1000 });

	tcList.push_back({ L"REQ_TO_EXIT_EOL", L"FF 05 FF FF", L"FF 05 FF FF", false, 2000, 100 });
}



bool CEolTestDialog::SetRcvMsg(CString msg)
{
	if (m_bRuning)
	{
		CString tmp = L"R: ";
		tmp.Append(msg);
		tmp.Append(L"\r\n");

		m_msglist.push_back(msg);

		m_edit_log.SetSel(-1);
		m_edit_log.ReplaceSel(tmp);
	}
	return true;
}

bool CEolTestDialog::SetRcvMsg(const unsigned char * msg, int size)
{
	return false;
}

bool CEolTestDialog::SetSndMsg(CString msg)
{
	CString tmp = L"S: ";
	tmp.Append(msg);
	tmp.Append(L"\r\n");

	m_edit_log.SetSel(-1);
	m_edit_log.ReplaceSel(tmp);
	return true;
}



BOOL CEolTestDialog::OnInitDialog()
{
	__super::OnInitDialog();
	

	m_pFont = new CFont;
	m_pFont->CreatePointFont(90, L"Courier New");
	m_edit_log.SetFont(m_pFont);


	m_progress.SetRange(0, 100);
	m_progress.SetPos(0);

	RECT  m_rect;
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_list.SetExtendedStyle(dwStyle); //设置扩展风格
	m_list.GetClientRect(&m_rect); //获取list的客户区,方便调节每一列的宽度  a
	m_list.InsertColumn(0, _T("Step"), LVCFMT_LEFT, 40);
	m_list.InsertColumn(1, _T("TimeStamp"), LVCFMT_LEFT, 90);
	m_list.InsertColumn(2, _T("Result"), LVCFMT_LEFT, 60);
	m_list.InsertColumn(3, _T("TestName"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(4, _T("Output"), LVCFMT_LEFT, 150);

	initTcList();

	m_btn_eol_snd.SetTooltip(L"不包括长度和校验字节");
	m_edit_snd.SetWindowText(L"FF 01 FF FF");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEolTestDialog::OnBnClickedButtonEolStart()
{
	int nRows = 0;
	CString StrRows;
	m_list.DeleteAllItems();
	for (unsigned int j = 0; j<tcList.size(); j++) // 多少行
	{
		StrRows.Format(L"%02d", nRows + 1);
		nRows = m_list.InsertItem(nRows, StrRows);
		m_list.SetItemText(nRows, 1, L"");
		m_list.SetItemText(nRows, 2, L"");
		m_list.SetItemText(nRows, 3, tcList[j].testName);
		m_list.SetItemText(nRows, 4, L"");
		nRows++;
	}

	if (!m_pPresenter->IsComPortOpened())
	{
		AfxMessageBox(L"请打开串口，选择19200 BaudRate！");
		return;
	}

	GetDlgItem(IDC_BUTTON_EOL_START)->EnableWindow(FALSE);

	m_bRuning = true;
#if 1
	_StartStep(true);
#else
	

	m_TestStep = 0;


	/// start....
	CString outStr;
	int ret;
	for (unsigned int i = 0; i<tcList.size(); i++) // 多少行
	{
		m_list.EnsureVisible(i, FALSE);
		m_list.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		m_progress.SetPos((i+1) * 100 / tcList.size());

		if (!tcList[i].sndCmd.IsEmpty())
			m_pPresenter->SendEolMsg(tcList[i].sndCmd);
		ret = ssut(tcList[i].checkCmd, tcList[i].waitTime, tcList[i].stillTime, outStr);
		if (ret == 1) // find
		{
			if (tcList[i].bNeedStr == 0) // status
			{
				if (CMyString::toHex(CMyString::toString(outStr.Mid(15, 2))) == 1)
				{
					m_list.SetItemText(i, 2, L"PASS");
				}
				else
				{
					m_list.SetItemText(i, 2, L"FAILED");
				}
			}
			else if (tcList[i].bNeedStr == 1) // hex
			{
				m_list.SetItemText(i, 2, L"PASS");
				m_list.SetItemText(i, 4, outStr.Mid(15, 2));
			}
			else if (tcList[i].bNeedStr == 2) // ascii
			{
				int len = CMyString::toHex(CMyString::toString(outStr.Mid(0, 2)));
				CString tmp;
				CString str;
				m_list.SetItemText(i, 2, L"PASS");

				for (int m = 5; m < len; m++)
				{
					tmp.Format(L"%c", CMyString::toHex(CMyString::toString(outStr.Mid(m*3, 3))));
					str.Append(tmp);
				}
				m_list.SetItemText(i, 4, str);
			}
		}
		else
		{
			m_list.SetItemText(i, 2, L"TIMEOUT");
		}
		m_TestStep = i;
	}
#endif
}


void CEolTestDialog::OnBnClickedButtonEolPause()
{
	CString str;
	GetDlgItemText(IDC_BUTTON_EOL_PAUSE, str);

	if (str.Find(L"Pause") >= 0)
	{
		SetDlgItemText(IDC_BUTTON_EOL_PAUSE, L"Continue");
		KillTimer(TIMER_MINE_EOL_STEP_IN);
		KillTimer(TIMER_MINE_EOL_STEP_CHECKING);
		KillTimer(TIMER_MINE_EOL_STEP_TIMEOUT);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_EOL_PAUSE, L"Pause");
		_StartStep(false);
	}
}


void CEolTestDialog::OnBnClickedButtonEolStop()
{
	SetDlgItemText(IDC_BUTTON_EOL_PAUSE, L"Pause");
	GetDlgItem(IDC_BUTTON_EOL_START)->EnableWindow(TRUE);
	KillTimer(TIMER_MINE_EOL_STEP_IN);
	KillTimer(TIMER_MINE_EOL_STEP_CHECKING);
	KillTimer(TIMER_MINE_EOL_STEP_TIMEOUT);
	m_bRuning = false;
}



int CEolTestDialog::ssut(CString Search_STR,long Search_Time, long delay_time, CString &output)
{
	BOOL Search_Result = 0;
	clock_t goal;
	if (!Search_STR.IsEmpty())
	{
		Search_STR = Search_STR.Trim();
	}
	goal = Search_Time + clock();
	while (goal > clock())
	{
		MSG message;
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(10);
		if (!Search_STR.IsEmpty() && findStr(Search_STR, output))
		{
			Search_Result = 1; // OK
			Sleep(10);
			break;
		}
		else
		{
			Search_Result = 0; // TimeOut
		}
	}


	clock_t clk_start = clock();
	if (1)
	{
		long lGoal = 0;
		if (delay_time > 0)
		{
			lGoal = delay_time + clk_start; 
			while (clock() < lGoal)
			{
				MSG message;
				if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
				{
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
			}
		}
		
	}
	return Search_Result;
}


bool CEolTestDialog::findStr(CString &dest, CString &which)
{
	for (unsigned int i = 0; i < m_msglist.size(); i++)
	{
		if (m_msglist[i].Find(dest) >= 0)
		{
			which = m_msglist[i];
			return true;
		}
	}
	return false;
}



void CEolTestDialog::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_MINE_EOL_STEP_IN:
	{
		KillTimer(nIDEvent);
		m_list.EnsureVisible(m_TestStep, FALSE);
		m_list.SetItemState(m_TestStep, LVIS_SELECTED, LVIS_SELECTED);
		m_list.SetItemText(m_TestStep, 1, CMyString::GetNowTime());
		m_edit_cur.SetWindowText(tcList[m_TestStep].testName);
		m_edit_result.SetWindowText(L"");
		m_progress.SetPos((m_TestStep + 1) * 100 / tcList.size());

		if (!tcList[m_TestStep].sndCmd.IsEmpty())
			m_pPresenter->SendEolMsg(tcList[m_TestStep].sndCmd);

		SetTimer(TIMER_MINE_EOL_STEP_CHECKING, 50, NULL);
		SetTimer(TIMER_MINE_EOL_STEP_TIMEOUT, tcList[m_TestStep].waitTime, NULL);
	}
		break;
	case TIMER_MINE_EOL_STEP_CHECKING:
	{
		CString outStr;										 
		int ret = findStr(tcList[m_TestStep].checkCmd, outStr);

		if (ret == 1) // find
		{
			KillTimer(nIDEvent);
			KillTimer(TIMER_MINE_EOL_STEP_TIMEOUT);
			if (tcList[m_TestStep].bNeedStr == -1) // just continue
			{
				m_list.SetItemText(m_TestStep, 2, L"PASS");
				m_edit_result.SetWindowText(L"PASS");
			}
			else if (tcList[m_TestStep].bNeedStr == 0) // status
			{
				if (CMyString::toHex(CMyString::toString(outStr.Mid(15, 2))) == 1)
				{
					m_list.SetItemText(m_TestStep, 2, L"PASS");
					m_edit_result.SetWindowText(L"PASS");
				}
				else
				{
					m_list.SetItemText(m_TestStep, 2, L"FAILED");
					m_edit_result.SetWindowText(L"FAILED");
				}
			}
			else if (tcList[m_TestStep].bNeedStr == 1) // hex
			{
				m_list.SetItemText(m_TestStep, 2, L"PASS");
				m_edit_result.SetWindowText(L"PASS");
				m_list.SetItemText(m_TestStep, 4, outStr.Mid(15, 2));
			}
			else if (tcList[m_TestStep].bNeedStr == 2) // ascii
			{
				int len = CMyString::toHex(CMyString::toString(outStr.Mid(0, 2)));
				CString tmp;
				CString str;
				m_list.SetItemText(m_TestStep, 2, L"PASS");
				m_edit_result.SetWindowText(L"PASS");

				for (int m = 5; m < len; m++)
				{
					tmp.Format(L"%c", CMyString::toHex(CMyString::toString(outStr.Mid(m * 3, 3))));
					str.Append(tmp);
				}
				m_list.SetItemText(m_TestStep, 4, str);
			}

			_NextStep();
		}
	}
		break;
	case TIMER_MINE_EOL_STEP_TIMEOUT:
		m_list.SetItemText(m_TestStep, 2, L"TIMEOUT");
		m_edit_result.SetWindowText(L"TIMEOUT");
		_NextStep();
		break;
	}
}

void CEolTestDialog::_NextStep()
{
	KillTimer(TIMER_MINE_EOL_STEP_TIMEOUT);
	m_TestStep++;
	m_msglist.clear();
	if (m_TestStep < tcList.size())
		SetTimer(TIMER_MINE_EOL_STEP_IN, tcList[m_TestStep-1].stillTime, NULL);
	else
	{
		GetDlgItem(IDC_BUTTON_EOL_START)->EnableWindow(TRUE);
		m_bRuning = false;
	}
}

void CEolTestDialog::_StartStep(bool bReset)
{
	KillTimer(TIMER_MINE_EOL_STEP_TIMEOUT);
	m_msglist.clear();
	if (bReset)
		m_TestStep = 0;

	if (m_TestStep < tcList.size())
		SetTimer(TIMER_MINE_EOL_STEP_IN, 100, NULL);
	else
	{
		GetDlgItem(IDC_BUTTON_EOL_START)->EnableWindow(TRUE);
		m_bRuning = false;
	}
}


BOOL CEolTestDialog::DestroyWindow()
{
	KillTimer(TIMER_MINE_EOL_STEP_IN);
	KillTimer(TIMER_MINE_EOL_STEP_CHECKING);
	KillTimer(TIMER_MINE_EOL_STEP_TIMEOUT);

	m_bRuning = false;

	return __super::DestroyWindow();
}


void CEolTestDialog::OnBnClickedMfcbuttonEolSnd()
{
	CString msg;
	m_edit_snd.GetWindowText(msg);
	if (msg.IsEmpty() || msg.GetLength() < 11)
		AfxMessageBox(L"请输入十六进制数字，空格隔开，不要包括长度和校验码字节");
	else
		m_pPresenter->SendEolMsg(msg);
}


void CEolTestDialog::OnEnSetfocusEditEolSnd()
{
	m_edit_snd.ShowBalloonTip(L"", L"输入十六进制，不包括长度和checksum");
}

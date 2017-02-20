// ComLogDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "ComLogDialog.h"
#include "afxdialogex.h"

#include "CanDbc.h"

#include "CommonConfig.h"

#include "common.h"

// CComLogDialog 对话框
#define COM_SNDMSG_SIZE 20
IMPLEMENT_DYNAMIC(CComLogDialog, CDialogEx)

CComLogDialog::CComLogDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CComLogDialog::IDD, pParent)
	, m_IdxOfComMsgSnd(0)
	, m_bPlatform(0)
	, m_bRcv(true)
{
	m_pComLogPresenter = new ComLogPresenter(this);
}

CComLogDialog::~CComLogDialog()
{
	delete m_pComLogPresenter;
}

void CComLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_COMLOG, m_editComLog);
	DDX_Control(pDX, IDC_EDIT_COM_SND1, m_edit_com_snd1);
	DDX_Control(pDX, IDC_EDIT_COM_SND2, m_edit_com_snd2);
	DDX_Control(pDX, IDC_EDIT_COM_SND3, m_edit_com_snd3);
	DDX_Control(pDX, IDC_EDIT_COM_SND4, m_edit_com_snd4);
	DDX_Control(pDX, IDC_EDIT_COM_SND5, m_edit_com_snd5);
	DDX_Control(pDX, IDC_EDIT_COM_SND6, m_edit_com_snd6);
	DDX_Control(pDX, IDC_EDIT_COM_SND7, m_edit_com_snd7);
	DDX_Control(pDX, IDC_EDIT_COM_SND8, m_edit_com_snd8);
	DDX_Control(pDX, IDC_EDIT_COM_SND9, m_edit_com_snd9);
	DDX_Control(pDX, IDC_EDIT_COM_SND10, m_edit_com_snd10);
	DDX_Control(pDX, IDC_EDIT_COM_SND11, m_edit_com_snd11);
	DDX_Control(pDX, IDC_EDIT_COM_SND12, m_edit_com_snd12);
	DDX_Control(pDX, IDC_EDIT_COM_SND13, m_edit_com_snd13);
	DDX_Control(pDX, IDC_EDIT_COM_SND14, m_edit_com_snd14);
	DDX_Control(pDX, IDC_EDIT_COM_SND15, m_edit_com_snd15);
	DDX_Control(pDX, IDC_EDIT_COM_SND16, m_edit_com_snd16);
	DDX_Control(pDX, IDC_EDIT_COM_SND17, m_edit_com_snd17);
	DDX_Control(pDX, IDC_EDIT_COM_SND18, m_edit_com_snd18);
	DDX_Control(pDX, IDC_RICHEDIT2_COM_SND, m_editComSnd);
}


BEGIN_MESSAGE_MAP(CComLogDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COM_TEST, &CComLogDialog::OnBnClickedButtonComTest)
	ON_BN_CLICKED(IDC_BUTTON_F1, &CComLogDialog::OnBnClickedButtonF1)
	ON_BN_CLICKED(IDC_BUTTON_F2, &CComLogDialog::OnBnClickedButtonF2)
	ON_BN_CLICKED(IDC_BUTTON_F3, &CComLogDialog::OnBnClickedButtonF3)
	ON_BN_CLICKED(IDC_BUTTON_F4, &CComLogDialog::OnBnClickedButtonF4)
	ON_BN_CLICKED(IDC_BUTTON_F5, &CComLogDialog::OnBnClickedButtonF5)
	ON_BN_CLICKED(IDC_BUTTON_F6, &CComLogDialog::OnBnClickedButtonF6)
	ON_BN_CLICKED(IDC_BUTTON_F7, &CComLogDialog::OnBnClickedButtonF7)
	ON_BN_CLICKED(IDC_BUTTON_F8, &CComLogDialog::OnBnClickedButtonF8)
	ON_BN_CLICKED(IDC_BUTTON_F9, &CComLogDialog::OnBnClickedButtonF9)
	ON_BN_CLICKED(IDC_BUTTON_F10, &CComLogDialog::OnBnClickedButtonF10)
	ON_BN_CLICKED(IDC_BUTTON_F11, &CComLogDialog::OnBnClickedButtonF11)
	ON_BN_CLICKED(IDC_BUTTON_F12, &CComLogDialog::OnBnClickedButtonF12)
	ON_BN_CLICKED(IDC_BUTTON_F13, &CComLogDialog::OnBnClickedButtonF13)
	ON_BN_CLICKED(IDC_BUTTON_F14, &CComLogDialog::OnBnClickedButtonF14)
	ON_BN_CLICKED(IDC_BUTTON_F15, &CComLogDialog::OnBnClickedButtonF15)
	ON_BN_CLICKED(IDC_BUTTON_F16, &CComLogDialog::OnBnClickedButtonF16)
	ON_BN_CLICKED(IDC_BUTTON_F17, &CComLogDialog::OnBnClickedButtonF17)
	ON_BN_CLICKED(IDC_BUTTON_F18, &CComLogDialog::OnBnClickedButtonF18)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_COM_LOG_CLEAN, &CComLogDialog::OnBnClickedButtonComLogClean)
	ON_BN_CLICKED(IDC_BUTTON_COM_LOG_COPY, &CComLogDialog::OnBnClickedButtonComLogCopy)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COM_PAUSE, &CComLogDialog::OnBnClickedButtonComPause)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CComLogDialog 消息处理程序

BOOL CComLogDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_editComLog.Initlize(1);
	m_editComSnd.Initlize(2);

	GetClientRect(&m_rect);



#if 1// load config
	CCommonConfig *tt = CCommonConfig::GetInstance();
	tt->loadConfig();

	CWnd * editlist[18] = { &m_edit_com_snd1,
		&m_edit_com_snd2,
		&m_edit_com_snd3,
		&m_edit_com_snd4,
		&m_edit_com_snd5,
		&m_edit_com_snd6,
		&m_edit_com_snd7,
		&m_edit_com_snd8,
		&m_edit_com_snd9,
		&m_edit_com_snd10,
		&m_edit_com_snd11,
		&m_edit_com_snd12,
		&m_edit_com_snd13,
		&m_edit_com_snd14,
		&m_edit_com_snd15,
		&m_edit_com_snd16,
		&m_edit_com_snd17,
		&m_edit_com_snd18,
	};

	CString key;
	for (int i = 0; i < 18; i++)
	{
		key.Format(L"COM_EDIT%d", i + 1);
		editlist[i]->SetWindowText(tt->getKey(key));
	}
#endif	
	return TRUE;  // return TRUE unless you set the focus to a control
}















////////////////////////////实现的接收数据处理函数、、、、、、、、、、、、、、、、、、、
bool CComLogDialog::SetRcvMsg(const unsigned char * msg, int size)
{
	if (!m_bRcv)
		return false;
#if 1
	CString tmp;
	CString msgStr;
	for (int i = 0; i < size; i++)
	{
		if (msg[i] == '\r')
			continue;
		else if (msg[i] == '\n')
		{
			msgStr.Append(L"\r\n");
			continue;
		}
		tmp.Format(L"%c",msg[i]);
		msgStr.Append(tmp);
	}

	if (msgStr.Find(L"SiRFatlas6 login:") >= 0)
	{
		this->SetTimer(TIMER_MINE_COM_LOGIN_A6, 100, NULL);
	}
	else if(msgStr.Find(L"dra7xx-evm login:") >= 0)
	{
		this->SetTimer(TIMER_MINE_COM_LOGIN_J6, 100, NULL);
	}	
	else if (msgStr.Find(L"Password:") >= 0)
	{
		if (m_bPlatform == 1)
		{
			Sleep(30);
			m_pComLogPresenter->SendAsciiData(L"adas-ab01");
		}
		else if (m_bPlatform == 2)
		{
			Sleep(30);
			m_pComLogPresenter->SendAsciiData(L"adas-ab02");
		}

		m_bPlatform = -1;
	}
	

	m_editComLog.Append(msgStr);
#endif
	return 0;
}
bool CComLogDialog::SetRcvMsg(CString msg)
{
	return false;
}


void CComLogDialog::OnBnClickedButtonComTest()
{
	//int fff = 0;
	//for (int i = 0; i < 20;i++)
	//m_pComLogPresenter->SendAsciiData(L"ll");

	//fff++;


	//CanDbc dbc;
	//dbc.ParseDbcFile(L"test.dbc");

//	AfxMessageBox(CMyUtility::GetModuleFolder());


	return;
}


void CComLogDialog::SendData(CString msg)
{
	m_pComLogPresenter->SendAsciiData(msg);
}


void CComLogDialog::ButtonsClick(int id)
{
	if (id > 18 || id < 0)
		return;
	CWnd * editlist[] = { &m_edit_com_snd1,
		&m_edit_com_snd2,
		&m_edit_com_snd3,
		&m_edit_com_snd4,
		&m_edit_com_snd5,
		&m_edit_com_snd6,
		&m_edit_com_snd7,
		&m_edit_com_snd8,
		&m_edit_com_snd9,
		&m_edit_com_snd10,
		&m_edit_com_snd11,
		&m_edit_com_snd12,
		&m_edit_com_snd13,
		&m_edit_com_snd14,
		&m_edit_com_snd15,
		&m_edit_com_snd16,
		&m_edit_com_snd17,
		&m_edit_com_snd18,
		&m_editComSnd,
	};
	CString text;
	editlist[id]->GetWindowText(text);
	m_pComLogPresenter->SendAsciiData(text);
}

void CComLogDialog::OnBnClickedButtonF1() { ButtonsClick(0); }
void CComLogDialog::OnBnClickedButtonF2() { ButtonsClick(1); }
void CComLogDialog::OnBnClickedButtonF3() { ButtonsClick(2); }
void CComLogDialog::OnBnClickedButtonF4() { ButtonsClick(3); }
void CComLogDialog::OnBnClickedButtonF5() { ButtonsClick(4); }
void CComLogDialog::OnBnClickedButtonF6() { ButtonsClick(5); }
void CComLogDialog::OnBnClickedButtonF7() { ButtonsClick(6); }
void CComLogDialog::OnBnClickedButtonF8() { ButtonsClick(7); }
void CComLogDialog::OnBnClickedButtonF9() { ButtonsClick(8); }
void CComLogDialog::OnBnClickedButtonF10() { ButtonsClick(9); }
void CComLogDialog::OnBnClickedButtonF11() { ButtonsClick(10); }
void CComLogDialog::OnBnClickedButtonF12() { ButtonsClick(11); }
void CComLogDialog::OnBnClickedButtonF13() { ButtonsClick(12); }
void CComLogDialog::OnBnClickedButtonF14() { ButtonsClick(13); }
void CComLogDialog::OnBnClickedButtonF15() { ButtonsClick(14); }
void CComLogDialog::OnBnClickedButtonF16() { ButtonsClick(15); }
void CComLogDialog::OnBnClickedButtonF17() { ButtonsClick(16); }
void CComLogDialog::OnBnClickedButtonF18() { ButtonsClick(17); }


void CComLogDialog::OnOK()
{
}


BOOL CComLogDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	CWnd * editlist[] = { &m_edit_com_snd1,
		&m_edit_com_snd2,
		&m_edit_com_snd3,
		&m_edit_com_snd4,
		&m_edit_com_snd5,
		&m_edit_com_snd6,
		&m_edit_com_snd7,
		&m_edit_com_snd8,
		&m_edit_com_snd9,
		&m_edit_com_snd10,
		&m_edit_com_snd11,
		&m_edit_com_snd12,
		&m_edit_com_snd13,
		&m_edit_com_snd14,
		&m_edit_com_snd15,
		&m_edit_com_snd16,
		&m_edit_com_snd17,
		&m_edit_com_snd18,
		&m_editComSnd,
	};

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			CWnd * focus =  GetFocus();
			for (int i = 0; i < 19; i++)
			{
				if (focus == editlist[i])
				{
					ButtonsClick(i);
					break;
				}
			}
			
			if (focus == &m_editComSnd)
			{
				CString value;
				m_editComSnd.GetWindowText(value);
				if (!value.IsEmpty())
				{
					m_editComSnd.SetWindowText(L"");
					m_ComMsgSndList.push_back(value);
					if (COM_SNDMSG_SIZE < m_ComMsgSndList.size())
						m_ComMsgSndList.pop_front();

					m_IdxOfComMsgSnd = m_ComMsgSndList.size() - 1;
				}
			}
		}
		else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_UP && GetFocus() == &m_editComSnd)
		{
			if (m_IdxOfComMsgSnd < m_ComMsgSndList.size())
			{
				std::list<CString>::iterator it; 
				unsigned int idx = 0;
				for (it = m_ComMsgSndList.begin(), idx = 0; it != m_ComMsgSndList.end(); it++, idx++)
				{
					if (idx >= m_IdxOfComMsgSnd)
						break;
				}
				if (idx == m_IdxOfComMsgSnd)
				{
					m_editComSnd.SetWindowText(*it);
					if (m_IdxOfComMsgSnd>0)
					m_IdxOfComMsgSnd--;
				} 
			}
			else 
				m_IdxOfComMsgSnd--;
		}
		else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN && GetFocus() == &m_editComSnd)
		{
			if (m_IdxOfComMsgSnd < m_ComMsgSndList.size())
			{
				std::list<CString>::iterator it;
				unsigned int idx = 0;
				for (it = m_ComMsgSndList.begin(), idx = 0; it != m_ComMsgSndList.end(); it++, idx++)
				{
					if (idx >= m_IdxOfComMsgSnd)
						break;
				}
				if (idx == m_IdxOfComMsgSnd)
				{
					m_editComSnd.SetWindowText(*it);
					if (m_IdxOfComMsgSnd<m_ComMsgSndList.size())
					m_IdxOfComMsgSnd++;
				}
				else
					m_editComSnd.SetWindowText(L"");
			}
			else
				m_editComSnd.SetWindowText(L"");
		}
	}


	
	return __super::PreTranslateMessage(pMsg);
}


void CComLogDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED) return;

	GetClientRect(&m_rect);
	ChangeSize(IDC_RICHEDIT2_COMLOG, cx, cy);
	ChangeSize(IDC_RICHEDIT2_COM_SND, cx, cy);

	
}

void CComLogDialog::ChangeSize(UINT nID, int x, int y)  //nID为控件ID，x,y分别为对话框的当前长和宽
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL)     //判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	{
		CRect rec;
		pWnd->GetWindowRect(&rec);  //获取控件变化前的大小
		ScreenToClient(&rec);   //将控件大小装换位在对话框中的区域坐标
		if (IDC_RICHEDIT2_COM_SND == nID)
		{
			//rec.left = rec.left*x / m_rect.Width();  //按照比例调整空间的新位置
			rec.bottom = m_rect.Height()-10;
			rec.right = m_rect.Width()-5;
			rec.top = rec.bottom-30;
		}
		else
		{
			//rec.left = rec.left*x / m_rect.Width();  //按照比例调整空间的新位置
			//rec.top = rec.top*y / m_rect.Height();
			rec.bottom = m_rect.Height()-45;
			rec.right = m_rect.Width()-5;
		}
		pWnd->MoveWindow(rec);   //伸缩控件
	}
}

BOOL CComLogDialog::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类


#if 1// load config
	CCommonConfig *tt = CCommonConfig::GetInstance();

	CWnd * editlist[18] = { &m_edit_com_snd1,
		&m_edit_com_snd2,
		&m_edit_com_snd3,
		&m_edit_com_snd4,
		&m_edit_com_snd5,
		&m_edit_com_snd6,
		&m_edit_com_snd7,
		&m_edit_com_snd8,
		&m_edit_com_snd9,
		&m_edit_com_snd10,
		&m_edit_com_snd11,
		&m_edit_com_snd12,
		&m_edit_com_snd13,
		&m_edit_com_snd14,
		&m_edit_com_snd15,
		&m_edit_com_snd16,
		&m_edit_com_snd17,
		&m_edit_com_snd18,
	};

	CString key;
	CString value;
	for (int i = 0; i < 18; i++)
	{
		key.Format(L"COM_EDIT%d", i + 1);
		editlist[i]->GetWindowText(value);
		tt->addKey(key, value);
	}
#endif	

	return __super::DestroyWindow();
}


void CComLogDialog::OnBnClickedButtonComLogClean()
{
	m_editComLog.SetWindowText(L"");
}


void CComLogDialog::OnBnClickedButtonComLogCopy()
{
	CString source;
	m_editComLog.GetWindowText(source);
	if (OpenClipboard())
	{
		//防止非ASCII语言复制到剪切板为乱码
		CStringW strWide = CStringW(source);
		int nLen = strWide.GetLength();
		//将剪切板置空
		::EmptyClipboard();
		HANDLE clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, (nLen + 1) * 2);
		if (!clipbuffer)
		{
			::CloseClipboard();
			return;
		}
		char* buffer = (char*)::GlobalLock(clipbuffer);
		memset(buffer, 0, (nLen + 1) * 2);
		memcpy_s(buffer, nLen * 2, strWide.GetBuffer(0), nLen * 2);
		strWide.ReleaseBuffer();
		::GlobalUnlock(clipbuffer);
		::SetClipboardData(CF_UNICODETEXT, clipbuffer);
		::CloseClipboard();
	}
}


void CComLogDialog::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TIMER_MINE_COM_LOGIN_J6:
		m_bPlatform = 2;
		m_pComLogPresenter->SendAsciiData(L"root");
		break;

	case TIMER_MINE_COM_LOGIN_A6:
		m_bPlatform = 1;
		m_pComLogPresenter->SendAsciiData(L"root");
		break;

	default:
		m_bPlatform = -1;
		break;
	}
}


void CComLogDialog::OnBnClickedButtonComPause()
{
	CString tmp;
	this->GetDlgItemText(IDC_BUTTON_COM_PAUSE,tmp);

	if (tmp.Find(L"暂停")>=0)
	{
		m_bRcv = false;
		SetDlgItemText(IDC_BUTTON_COM_PAUSE, L"继续");
	}
	else
	{
		m_bRcv = true;
		SetDlgItemText(IDC_BUTTON_COM_PAUSE, L"暂停");
	}
}


void CComLogDialog::OnClose()
{
	this->m_pParentWnd->SetFocus();

	__super::OnClose();
}

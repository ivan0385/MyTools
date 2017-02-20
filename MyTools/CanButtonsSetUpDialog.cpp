// CanButtonsSetUp.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "CanButtonsSetUpDialog.h"
#include "afxdialogex.h"

// CCanButtonsSetUp 对话框

IMPLEMENT_DYNAMIC(CCanButtonsSetUpDialog, CDialogEx)

CCanButtonsSetUpDialog::CCanButtonsSetUpDialog(CWnd* pParent /*=NULL*/)
: CDialogEx(CCanButtonsSetUpDialog::IDD, pParent)
, m_pPresenter(NULL)
{
	m_pPresenter = new CanButtonsPresenter(this);
}

CCanButtonsSetUpDialog::~CCanButtonsSetUpDialog()
{
	delete m_pPresenter;
}

void CCanButtonsSetUpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CTL_BTN, m_cbox_btns);
	DDX_Control(pDX, IDC_COMBO_CTL_MSG, m_cbox_msgs);
	DDX_Control(pDX, IDC_COMBO_CTL_SGN, m_cbox_sigs);
	DDX_Control(pDX, IDC_LIST_CTL, m_list);
}


BEGIN_MESSAGE_MAP(CCanButtonsSetUpDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CCanButtonsSetUpDialog::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CCanButtonsSetUpDialog::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCanButtonsSetUpDialog::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCanButtonsSetUpDialog::OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_CTL_MSG, &CCanButtonsSetUpDialog::OnCbnSelchangeComboCtlMsg)
	ON_BN_CLICKED(IDC_CHECK_CTRL, &CCanButtonsSetUpDialog::OnBnClickedCheckCtrl)
END_MESSAGE_MAP()


// CCanButtonsSetUp 消息处理程序


void CCanButtonsSetUpDialog::OnOK()
{
}


BOOL CCanButtonsSetUpDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pPresenter->Init();


	RECT  m_rect;
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件

	m_list.SetExtendedStyle(dwStyle); //设置扩展风格
	m_list.GetClientRect(&m_rect); //获取list的客户区,方便调节每一列的宽度  a
	m_list.InsertColumn(0, _T("Caption"), LVCFMT_LEFT, 160);
	m_list.InsertColumn(1, _T("报文"), LVCFMT_LEFT, 180);
	m_list.InsertColumn(2, _T("报文ID"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(3, _T("信号"), LVCFMT_LEFT, 220);
	m_list.InsertColumn(4, _T("信号数值"), LVCFMT_LEFT, 60);
	SetDlgItemText(IDC_EDIT_CTL_VALUE, L"0");
	return TRUE;
}


void CCanButtonsSetUpDialog::SetCaptionsData(const CString * data)
{
	int i = 0;
	m_cbox_btns.ResetContent();
	while (true)
	{
		if (data[i].IsEmpty())
			break;

		m_cbox_btns.AddString(data[i++]);
	}
}


void CCanButtonsSetUpDialog::OnBnClickedButtonRefresh()
{
	this->SetDlgItemText(IDC_EDIT_PRJ, m_pPresenter->GetPrjName());
	std::vector<CAN_MSG> *msglist = m_pPresenter->GetCanMsgList();
	m_cbox_msgs.ResetContent();
	m_cbox_sigs.ResetContent(); 
	this->SetDlgItemText(IDC_EDIT_CTL_MSG, L"");
	if (msglist != NULL)
	{
		std::vector<CAN_MSG>::iterator it;
		for (it = msglist->begin(); it<msglist->end(); it++)
		{
			m_cbox_msgs.AddString(CMyString::toCString(it->msgName));
		}
	}

	std::vector<std::vector<CString>> rec;
	m_custSetup.GetSavedUserButtonDef(m_pPresenter->GetPrjName(),rec);
	m_list.DeleteAllItems();
	for (unsigned int i = 0; i < rec.size(); i++)
	{
		if(rec[i].size()>=5)
		{
			ListCtrlAdd(rec[i][0], rec[i][1], rec[i][2], rec[i][3], rec[i][4]);
		}
	}
}


void CCanButtonsSetUpDialog::OnBnClickedButtonNew()
{
	CString btns;
	if (m_cbox_btns.GetCurSel()>=0)
		m_cbox_btns.GetLBText(m_cbox_btns.GetCurSel(), btns);

	CString msgName;
	if (m_cbox_msgs.GetCurSel() >= 0)
		m_cbox_msgs.GetLBText(m_cbox_msgs.GetCurSel(), msgName);
	
	CString msgId;
	GetDlgItemText(IDC_EDIT_CTL_MSG, msgId);

	CString sig;
	if (m_cbox_sigs.GetCurSel() >= 0)
		m_cbox_sigs.GetLBText(m_cbox_sigs.GetCurSel(), sig);

	CString sigval;
	GetDlgItemText(IDC_EDIT_CTL_VALUE, sigval);


	if (btns.IsEmpty())
	{
		AfxMessageBox(L"请选择标签");
		m_cbox_btns.SetFocus();
		return;
	}

	if (msgName.IsEmpty())
	{
		AfxMessageBox(L"请选择报文");
		m_cbox_msgs.SetFocus();
		return;
	}

	if (sig.IsEmpty())
	{
		AfxMessageBox(L"请选择信号");
		m_cbox_sigs.SetFocus();
		return;
	}

	if (sigval.IsEmpty())
	{
		AfxMessageBox(L"请填写数值");
		return;
	}

	ListCtrlAdd(btns, msgName, msgId, sig, sigval);

}


void CCanButtonsSetUpDialog::OnBnClickedButtonDel()
{
	int i = 0;
	int iRows = m_list.GetItemCount(); // 获取多少行

	for (i = iRows - 1; i >= 0; i--)
	{
		if (m_list.GetCheck(i))
		{
			m_list.DeleteItem(i);
		}
	}
}


void CCanButtonsSetUpDialog::OnBnClickedButtonSave()
{	
	CString prj;
	CString cap; 
	CString msg; 
	CString id; 
	CString sig; 
	CString val;

	this->GetDlgItemText(IDC_EDIT_PRJ, prj);
	if (prj.IsEmpty())
	{
		AfxMessageBox(L"没有选中项目名");
		return;
	}

	int iRows = m_list.GetItemCount(); // 获取多少行	
	int j;
	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		j = 0;
		cap = m_list.GetItemText(i, j++);
		msg = m_list.GetItemText(i, j++);
		id =  m_list.GetItemText(i, j++);
		sig = m_list.GetItemText(i, j++);
		val = m_list.GetItemText(i, j++);

		m_custSetup.InsertUserButtonDef(prj, cap, msg, id, sig, val);
	}
	AfxMessageBox(L"完成");
}


void CCanButtonsSetUpDialog::OnCbnSelchangeComboCtlMsg()
{
#if 1
	CString tmp;
	m_cbox_msgs.GetLBText(m_cbox_msgs.GetCurSel(), tmp);
	std::string msgName = CMyString::toString(tmp);
#endif
	std::vector<CAN_MSG> *msglist = m_pPresenter->GetCanMsgList();

	m_cbox_sigs.ResetContent();
	std::vector<CAN_MSG>::iterator it;
	for (it = msglist->begin(); it<msglist->end(); it++)
	{
		if (it->msgName == msgName)
		{
			std::vector<CAN_SIGNAL> *siglist = m_pPresenter->GetCanSigList(it->msgId);

			std::vector<CAN_SIGNAL>::iterator it_sig;
			for (it_sig = siglist->begin(); it_sig<siglist->end(); it_sig++)
				m_cbox_sigs.AddString(CMyString::toCString(it_sig->sgnlName));

			tmp.Format(L"%d", it->msgId);
			this->SetDlgItemText(IDC_EDIT_CTL_MSG, tmp);
			break;
		}
	}
}

void CCanButtonsSetUpDialog::OnBnClickedCheckCtrl()
{
	int rows = m_list.GetItemCount();
	int i = 0;
	BOOL bCheck = FALSE;

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_CTRL))
	{
		bCheck = TRUE;
		if (rows == 0)
		{
			((CButton *)GetDlgItem(IDC_CHECK_CTRL))->SetCheck(FALSE);
		}
	}

	for (i = 0; i<rows; i++)
	{
		m_list.SetCheck(i, bCheck);
	}
}


bool CCanButtonsSetUpDialog::ListCtrlAdd(CString cap, CString msg, CString id, CString sig, CString val)
{	
	int nCounts = m_list.GetItemCount();
	int nRow = nCounts;
	int nCol = 1;


	for (int i = 0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetItemText(i, 0) == cap)
		{
			m_list.DeleteItem(i);
		}
	}

	nRow = m_list.GetItemCount();
	nRow = m_list.InsertItem(nRow, cap);
	m_list.SetItemText(nRow, nCol++, msg);
	m_list.SetItemText(nRow, nCol++, id);
	m_list.SetItemText(nRow, nCol++, sig);
	m_list.SetItemText(nRow, nCol++, val);

	m_list.EnsureVisible(nRow, FALSE);
	m_list.SetItemState(nRow, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED);
	m_list.SetFocus();

	return true;
}


bool CCanButtonsSetUpDialog::ListCtrlRemove()
{
	return false;
}

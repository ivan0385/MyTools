// CanDbcSettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTools.h"
#include "CanDbcSettingDialog.h"
#include "afxdialogex.h"

#include "CanTaskModel.h"

// CCanDbcSettingDialog 对话框

IMPLEMENT_DYNAMIC(CCanDbcSettingDialog, CDialogEx)

CCanDbcSettingDialog::CCanDbcSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCanDbcSettingDialog::IDD, pParent)
	, e_Item(0)
	, e_SubItem(0)
	, haveeditcreate(false)
	, m_treeCurrentSel(0)
{

}

CCanDbcSettingDialog::~CCanDbcSettingDialog()
{
}

void CCanDbcSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DBC, m_treeCanDbc);
	DDX_Control(pDX, IDC_LIST1, m_listDetail);
}


BEGIN_MESSAGE_MAP(CCanDbcSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCanDbcSettingDialog::OnBnClickedButton1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DBC, &CCanDbcSettingDialog::OnTvnSelchangedTreeDbc)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CCanDbcSettingDialog::OnNMClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CCanDbcSettingDialog::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCanDbcSettingDialog::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCanDbcSettingDialog 消息处理程序


BOOL CCanDbcSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	RECT  m_rect;
	DWORD dwStyle = m_listDetail.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listDetail.SetExtendedStyle(dwStyle); //设置扩展风格
	m_listDetail.GetClientRect(&m_rect); //获取list的客户区,方便调节每一列的宽度  
	m_listDetail.InsertColumn(0, _T("报文信息"), LVCFMT_LEFT, 90);
	m_listDetail.InsertColumn(1, _T("内容"), LVCFMT_LEFT, 180);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CCanDbcSettingDialog::refreshTree()
{
	// TODO:  在此添加额外的初始化
	HTREEITEM hRoot;     // 树的根节点的句柄   
	HTREEITEM hMsgItem; // 可表示任一报文节点的句柄   
	HTREEITEM hSigItem;  // 可表示任一信号节点的句柄   

	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	CanDbc * data = pCanTaskModel->GetCanMatrix();
	unsigned int ibk;

	m_treeCanDbc.DeleteAllItems();

	std::vector<CAN_MSG> msgList = data->GetCanMatrix().msgList;

	hRoot = m_treeCanDbc.InsertItem(CString(data->GetCanMatrix().nodeName.c_str()), 0, 0);
	m_treeCanDbc.SetItemData(hRoot, 0xffff);
	for (unsigned int i = 0; i < msgList.size(); i++)
	{
		hMsgItem = m_treeCanDbc.InsertItem(CString(msgList[i].msgName.c_str()), 1, 1, hRoot, TVI_LAST);
		ibk = i << 8;
		m_treeCanDbc.SetItemData(hMsgItem,ibk|0xff);
		for (unsigned int j = 0; j < msgList[i].signalList.size(); j++)
		{
			hSigItem = m_treeCanDbc.InsertItem(CString(msgList[i].signalList[j].sgnlName.c_str()), 2, 2, hMsgItem, TVI_LAST);
			m_treeCanDbc.SetItemData(hSigItem, ibk | j);
		}
	}
	m_treeCanDbc.Expand(hRoot, TVE_EXPAND);

	m_treeCurrentSel = 0xff0000;
}


void CCanDbcSettingDialog::OnBnClickedButton1()
{
	refreshTree();
}


void CCanDbcSettingDialog::OnTvnSelchangedTreeDbc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;


	CString strText; // 树节点的标签文本字符串   
	unsigned int idx;

	// 获取当前选中节点的句柄   
	HTREEITEM hItem = m_treeCanDbc.GetSelectedItem();
	// 获取选中节点的标签文本字符串   
	strText = m_treeCanDbc.GetItemText(hItem);
	idx = m_treeCanDbc.GetItemData(hItem);
	m_treeCurrentSel = idx;
	UpdateListControl(idx);	
}

static CString lableForList1[] = {
	L"项目名",
	L"节点名"
};
enum{
	LST1_PRJ,
	LIT1_NODE
};
static CString lableForList2[] = {
	L"报文名字",
	L"报文ID",
	L"报文长度",
	L"报文周期",
	L"发送类型",
	L"发送方",
	L"IsEnabled",
	L"信号数目",
};
enum{
	LST2_NAME,
	LST2_ID,
	LST2_LENGTH,
	LST2_CYCLE,
	LST2_SENDTYPE,
	LST2_SENDER,
	LST2_ENABLE
};
static CString lableForList3[] = {
	L"信号名字",
	L"位数",
	L"起始位",
	L"顺序",
	L"单位",
	L"Fator",
	L"Offset",
	L"最小值",
	L"最大值",
	L"发送类型",
	L"ValueType",
	L"接收方",
	L"默认数值",
	L"当前数值"
};
enum{
	LST3_NAME,
	LST3_BITS,
	LST3_STARTBIT,
	LST3_ORDER,
	LST3_UNIT,
	LST3_FATOR,
	LST3_OFFSET, 
	LST3_MIN,
	LST3_MAX,
	LST3_SENDTYPE,
	LST3_VALUETYPE,
	LST3_RECEIVER,
	LST3_DEFAULTVAL,
	LST3_USERVAL,
};


bool CCanDbcSettingDialog::updateMatrix()
{
	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	CanDbc * data = pCanTaskModel->GetCanMatrix();

	if (m_treeCurrentSel >= 0xff0000)
	{
		return true;//nothing to do
	}
	else if (m_treeCurrentSel == 0xffff)//根节点
	{
		data->GetCanMatrix().name = CMyString::toString(m_listDetail.GetItemText(LST1_PRJ, 1));
		data->GetCanMatrix().nodeName = CMyString::toString(m_listDetail.GetItemText(LIT1_NODE, 1));
	}
	else if ((m_treeCurrentSel & 0xff) == 0xff)//报文节点
	{
		//AfxMessageBox(m_listDetail.GetItemText(LST2_ID, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].msgName = CMyString::toString(m_listDetail.GetItemText(LST2_NAME, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].msgId = _ttoi(m_listDetail.GetItemText(LST2_ID, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].msgLength = _ttoi(m_listDetail.GetItemText(LST2_LENGTH, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].msgCycleTime = _ttoi(m_listDetail.GetItemText(LST2_CYCLE, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].msgSendType = _ttoi(m_listDetail.GetItemText(LST2_SENDTYPE, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].msgTranmistter = CMyString::toString(m_listDetail.GetItemText(LST2_SENDER, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].msgIsEnable = !!_ttoi(m_listDetail.GetItemText(LST2_ENABLE, 1));
	}
	else // 信号节点
	{
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnlName = CMyString::toString(m_listDetail.GetItemText(LST3_NAME, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnBitSize = _ttoi(m_listDetail.GetItemText(LST3_BITS, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnStartBit = _ttoi(m_listDetail.GetItemText(LST3_STARTBIT, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnByteOrder = _ttoi(m_listDetail.GetItemText(LST3_ORDER, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnUnit = CMyString::toString(m_listDetail.GetItemText(LST3_UNIT, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnFator = _ttof(m_listDetail.GetItemText(LST3_FATOR, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnOffset = _ttof(m_listDetail.GetItemText(LST3_OFFSET, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnMin = _ttoi(m_listDetail.GetItemText(LST3_MIN, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnMax = _ttoi(m_listDetail.GetItemText(LST3_MAX, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnSendType = _ttoi(m_listDetail.GetItemText(LST3_SENDTYPE, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnValueType = CMyString::toString(m_listDetail.GetItemText(LST3_VALUETYPE, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnReceiver = CMyString::toString(m_listDetail.GetItemText(LST3_RECEIVER, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnDefValue = _ttoi(m_listDetail.GetItemText(LST3_DEFAULTVAL, 1));
		data->GetCanMatrix().msgList[m_treeCurrentSel >> 8].signalList[m_treeCurrentSel & 0xff].sgnUserValue = _ttoi(m_listDetail.GetItemText(LST3_USERVAL, 1));
	}
	return false;
}


void CCanDbcSettingDialog::UpdateListControl(int idx)
{
	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	CanDbc * data = pCanTaskModel->GetCanMatrix();
	int index = 0;

	CString temp;

	if (idx == 0xffff)//根节点
	{
		index = 0;
		m_listDetail.DeleteAllItems();
		m_listDetail.DeleteColumn(0);
		m_listDetail.InsertColumn(0, _T("项目信息"), LVCFMT_LEFT, 80);

		int nRow = 0;
		nRow = m_listDetail.InsertItem(nRow, lableForList1[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().name.c_str()));

		nRow = m_listDetail.InsertItem(nRow, lableForList1[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().nodeName.c_str()));
	}
	else if ((idx & 0xff) == 0xff)//报文节点
	{
		//AfxMessageBox(CString(data->GetCanMatrix().msgList[idx >> 8].msgName.c_str()));

		index = 0;
		m_listDetail.DeleteAllItems();
		m_listDetail.DeleteColumn(0);
		m_listDetail.InsertColumn(0, _T("报文信息"), LVCFMT_LEFT, 80);

		int nRow = 0;		
		nRow = m_listDetail.InsertItem(nRow, lableForList2[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().msgList[idx >> 8].msgName.c_str()));

		nRow = m_listDetail.InsertItem(nRow, lableForList2[index++]);
		temp.Format(L"%u",data->GetCanMatrix().msgList[idx >> 8].msgId);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList2[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].msgLength);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList2[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].msgCycleTime);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList2[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].msgSendType);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList2[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().msgList[idx >> 8].msgTranmistter.c_str()));

		nRow = m_listDetail.InsertItem(nRow, lableForList2[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].msgIsEnable);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList2[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList.size());
		m_listDetail.SetItemText(nRow++, 1, temp);

	}
	else // 信号节点
	{

		//AfxMessageBox(CString(data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnlName.c_str()));
		
		index = 0;
		m_listDetail.DeleteAllItems();
		m_listDetail.DeleteColumn(0);
		m_listDetail.InsertColumn(0, _T("信号信息"), LVCFMT_LEFT, 80);

		int nRow = 0;
		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnlName.c_str()));

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnBitSize);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnStartBit);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnByteOrder);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnUnit.c_str()));

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%f", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnFator);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%f", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnOffset);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnMin);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnMax);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnSendType);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnValueType.c_str()));
		
		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnReceiver.c_str()));

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnDefValue);
		m_listDetail.SetItemText(nRow++, 1, temp);

		nRow = m_listDetail.InsertItem(nRow, lableForList3[index++]);
		temp.Format(L"%d", data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnUserValue);
		m_listDetail.SetItemText(nRow++, 1, temp);
	}
}


void CCanDbcSettingDialog::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;

	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
	{
		distroyEdit(&m_listDetail, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}
}


void CCanDbcSettingDialog::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//printf(L"行：%d，列：%d\n", pEditCtrl->iItem, pEditCtrl->iSubItem);
	if (pEditCtrl->iItem == -1 || pEditCtrl->iSubItem == 0 )//点击到非工作区
	{
		if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
		{
			distroyEdit(&m_listDetail, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
			haveeditcreate = false;
		}
	}
	else if (pEditCtrl->iSubItem == 1)// 第二列为下拉列表
	{
		if (haveeditcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//如果点中的单元格不是之前创建好的
			{
				distroyEdit(&m_listDetail, &m_Edit, e_Item, e_SubItem);
				haveeditcreate = false;
				createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框
			}
			else//点中的单元格是之前创建好的
			{
				m_Edit.SetFocus();//设置为焦点 
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框
		}
	}
	else// 第二列为下拉列表
	{
		if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
		{
			distroyEdit(&m_listDetail, &m_Edit, e_Item, e_SubItem);
			haveeditcreate = false;
		}
	}

}


void CCanDbcSettingDialog::createEdit(NM_LISTVIEW  * pEditCtrl, CEdit * createdit, int & Item, int & SubItem, bool & havecreat)
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, 1);//创建编辑框对象,IDC_EDIT_CREATEID为控件ID号3000，在文章开头定义
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//设置字体,不设置这里的话上面的字会很突兀的感觉
	createdit->SetParent(&m_listDetail);//将list control设置为父窗口,生成的Edit才能正确定位,这个也很重要
	CRect  EditRect;
	m_listDetail.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_listDetail.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线
	CString strItem = m_listDetail.GetItemText(e_Item, e_SubItem);//获得相应单元格字符
	createdit->SetWindowText(strItem);//将单元格字符显示在编辑框上
	createdit->MoveWindow(&EditRect);//将编辑框位置放在相应单元格上
	createdit->ShowWindow(SW_SHOW);//显示编辑框在单元格上面
	createdit->SetFocus();//设置为焦点 
	createdit->SetSel(-1);//设置光标在文本框文字的最后
}


void CCanDbcSettingDialog::distroyEdit(CListCtrl * list, CEdit* distroyedit, int & Item, int & SubItem)
{
	CString meditdata;
	distroyedit->GetWindowTextW(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//获得相应单元格字符
	distroyedit->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错

	updateMatrix();
}

void CCanDbcSettingDialog::OnBnClickedButton2()
{
	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	CanDbc * data = pCanTaskModel->GetCanMatrix();
	if (CMyString::toCString(data->GetCanMatrix().name.c_str()).IsEmpty())
	{
		AfxMessageBox(L"项目名字不能为空！");
		return;
	}
	data->SaveMatrixToDb();
}



void CCanDbcSettingDialog::OnOK()
{
}

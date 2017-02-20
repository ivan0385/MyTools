// CanDbcSettingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTools.h"
#include "CanDbcSettingDialog.h"
#include "afxdialogex.h"

#include "CanTaskModel.h"

// CCanDbcSettingDialog �Ի���

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


// CCanDbcSettingDialog ��Ϣ�������


BOOL CCanDbcSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	RECT  m_rect;
	DWORD dwStyle = m_listDetail.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_listDetail.SetExtendedStyle(dwStyle); //������չ���
	m_listDetail.GetClientRect(&m_rect); //��ȡlist�Ŀͻ���,�������ÿһ�еĿ��  
	m_listDetail.InsertColumn(0, _T("������Ϣ"), LVCFMT_LEFT, 90);
	m_listDetail.InsertColumn(1, _T("����"), LVCFMT_LEFT, 180);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CCanDbcSettingDialog::refreshTree()
{
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HTREEITEM hRoot;     // ���ĸ��ڵ�ľ��   
	HTREEITEM hMsgItem; // �ɱ�ʾ��һ���Ľڵ�ľ��   
	HTREEITEM hSigItem;  // �ɱ�ʾ��һ�źŽڵ�ľ��   

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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;


	CString strText; // ���ڵ�ı�ǩ�ı��ַ���   
	unsigned int idx;

	// ��ȡ��ǰѡ�нڵ�ľ��   
	HTREEITEM hItem = m_treeCanDbc.GetSelectedItem();
	// ��ȡѡ�нڵ�ı�ǩ�ı��ַ���   
	strText = m_treeCanDbc.GetItemText(hItem);
	idx = m_treeCanDbc.GetItemData(hItem);
	m_treeCurrentSel = idx;
	UpdateListControl(idx);	
}

static CString lableForList1[] = {
	L"��Ŀ��",
	L"�ڵ���"
};
enum{
	LST1_PRJ,
	LIT1_NODE
};
static CString lableForList2[] = {
	L"��������",
	L"����ID",
	L"���ĳ���",
	L"��������",
	L"��������",
	L"���ͷ�",
	L"IsEnabled",
	L"�ź���Ŀ",
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
	L"�ź�����",
	L"λ��",
	L"��ʼλ",
	L"˳��",
	L"��λ",
	L"Fator",
	L"Offset",
	L"��Сֵ",
	L"���ֵ",
	L"��������",
	L"ValueType",
	L"���շ�",
	L"Ĭ����ֵ",
	L"��ǰ��ֵ"
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
	else if (m_treeCurrentSel == 0xffff)//���ڵ�
	{
		data->GetCanMatrix().name = CMyString::toString(m_listDetail.GetItemText(LST1_PRJ, 1));
		data->GetCanMatrix().nodeName = CMyString::toString(m_listDetail.GetItemText(LIT1_NODE, 1));
	}
	else if ((m_treeCurrentSel & 0xff) == 0xff)//���Ľڵ�
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
	else // �źŽڵ�
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

	if (idx == 0xffff)//���ڵ�
	{
		index = 0;
		m_listDetail.DeleteAllItems();
		m_listDetail.DeleteColumn(0);
		m_listDetail.InsertColumn(0, _T("��Ŀ��Ϣ"), LVCFMT_LEFT, 80);

		int nRow = 0;
		nRow = m_listDetail.InsertItem(nRow, lableForList1[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().name.c_str()));

		nRow = m_listDetail.InsertItem(nRow, lableForList1[index++]);
		m_listDetail.SetItemText(nRow++, 1, CString(data->GetCanMatrix().nodeName.c_str()));
	}
	else if ((idx & 0xff) == 0xff)//���Ľڵ�
	{
		//AfxMessageBox(CString(data->GetCanMatrix().msgList[idx >> 8].msgName.c_str()));

		index = 0;
		m_listDetail.DeleteAllItems();
		m_listDetail.DeleteColumn(0);
		m_listDetail.InsertColumn(0, _T("������Ϣ"), LVCFMT_LEFT, 80);

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
	else // �źŽڵ�
	{

		//AfxMessageBox(CString(data->GetCanMatrix().msgList[idx >> 8].signalList[idx & 0xff].sgnlName.c_str()));
		
		index = 0;
		m_listDetail.DeleteAllItems();
		m_listDetail.DeleteColumn(0);
		m_listDetail.InsertColumn(0, _T("�ź���Ϣ"), LVCFMT_LEFT, 80);

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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;

	if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
	{
		distroyEdit(&m_listDetail, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����
		haveeditcreate = false;
	}
}


void CCanDbcSettingDialog::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//printf(L"�У�%d���У�%d\n", pEditCtrl->iItem, pEditCtrl->iSubItem);
	if (pEditCtrl->iItem == -1 || pEditCtrl->iSubItem == 0 )//������ǹ�����
	{
		if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
		{
			distroyEdit(&m_listDetail, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����
			haveeditcreate = false;
		}
	}
	else if (pEditCtrl->iSubItem == 1)// �ڶ���Ϊ�����б�
	{
		if (haveeditcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//������еĵ�Ԫ����֮ǰ�����õ�
			{
				distroyEdit(&m_listDetail, &m_Edit, e_Item, e_SubItem);
				haveeditcreate = false;
				createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//�����༭��
			}
			else//���еĵ�Ԫ����֮ǰ�����õ�
			{
				m_Edit.SetFocus();//����Ϊ���� 
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
			e_SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//�����༭��
		}
	}
	else// �ڶ���Ϊ�����б�
	{
		if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
		{
			distroyEdit(&m_listDetail, &m_Edit, e_Item, e_SubItem);
			haveeditcreate = false;
		}
	}

}


void CCanDbcSettingDialog::createEdit(NM_LISTVIEW  * pEditCtrl, CEdit * createdit, int & Item, int & SubItem, bool & havecreat)
{
	Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
	SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, 1);//�����༭�����,IDC_EDIT_CREATEIDΪ�ؼ�ID��3000�������¿�ͷ����
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//��������,����������Ļ�������ֻ��ͻأ�ĸо�
	createdit->SetParent(&m_listDetail);//��list control����Ϊ������,���ɵ�Edit������ȷ��λ,���Ҳ����Ҫ
	CRect  EditRect;
	m_listDetail.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//��ȡ��Ԫ��Ŀռ�λ����Ϣ
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_listDetail.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1��-1�����ñ༭�����ڵ�ס�б���е�������
	CString strItem = m_listDetail.GetItemText(e_Item, e_SubItem);//�����Ӧ��Ԫ���ַ�
	createdit->SetWindowText(strItem);//����Ԫ���ַ���ʾ�ڱ༭����
	createdit->MoveWindow(&EditRect);//���༭��λ�÷�����Ӧ��Ԫ����
	createdit->ShowWindow(SW_SHOW);//��ʾ�༭���ڵ�Ԫ������
	createdit->SetFocus();//����Ϊ���� 
	createdit->SetSel(-1);//���ù�����ı������ֵ����
}


void CCanDbcSettingDialog::distroyEdit(CListCtrl * list, CEdit* distroyedit, int & Item, int & SubItem)
{
	CString meditdata;
	distroyedit->GetWindowTextW(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//�����Ӧ��Ԫ���ַ�
	distroyedit->DestroyWindow();//���ٶ����д�����Ҫ�����٣���Ȼ�ᱨ��

	updateMatrix();
}

void CCanDbcSettingDialog::OnBnClickedButton2()
{
	ICanTaskModel * pCanTaskModel = CanTaskModel::GetInstance();
	CanDbc * data = pCanTaskModel->GetCanMatrix();
	if (CMyString::toCString(data->GetCanMatrix().name.c_str()).IsEmpty())
	{
		AfxMessageBox(L"��Ŀ���ֲ���Ϊ�գ�");
		return;
	}
	data->SaveMatrixToDb();
}



void CCanDbcSettingDialog::OnOK()
{
}

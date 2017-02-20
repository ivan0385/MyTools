#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CCanDbcSettingDialog 对话框

class CCanDbcSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCanDbcSettingDialog)

public:
	CCanDbcSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCanDbcSettingDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAN_DBC_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeCanDbc;
	virtual BOOL OnInitDialog();
	void refreshTree();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTvnSelchangedTreeDbc(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listDetail;
	void UpdateListControl(int idx);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	int e_Item;
	int e_SubItem;
	bool haveeditcreate;
	void createEdit(NM_LISTVIEW  * pEditCtrl, CEdit * createdit, int & Item, int & SubItem, bool & havecreat);
	void distroyEdit(CListCtrl * list, CEdit* distroyedit, int & Item, int & SubItem);
	CEdit m_Edit;
	afx_msg void OnBnClickedButton2();
	unsigned int m_treeCurrentSel;
	bool updateMatrix();
	virtual void OnOK();
};

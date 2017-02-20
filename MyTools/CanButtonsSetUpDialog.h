#pragma once
#include "afxwin.h"
#include "ICanButtonsSetupView.h"
#include "CanButtonsPresenter.h"
#include "afxcmn.h"

#include "CustSetup.h"

// CCanButtonsSetUpDialog �Ի���

class CCanButtonsSetUpDialog : public CDialogEx, ICanButtonsSetupView
{
	DECLARE_DYNAMIC(CCanButtonsSetUpDialog)

public:
	CCanButtonsSetUpDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCanButtonsSetUpDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_CAN_BUTTONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnCbnSelchangeComboCtlMsg();
	CComboBox m_cbox_btns;
	CComboBox m_cbox_msgs;
	CComboBox m_cbox_sigs;

public:
	virtual void SetCaptionsData(const CString * data);
private:
	CustSetup m_custSetup;
public:
	CanButtonsPresenter *m_pPresenter;
	CListCtrl m_list;
	afx_msg void OnBnClickedCheckCtrl();
	bool ListCtrlAdd(CString cap, CString msg, CString id, CString sig, CString val);
	bool ListCtrlRemove();
};

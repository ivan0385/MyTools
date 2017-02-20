#pragma once
#include "ICanSettingView.h"
#include "afxwin.h"
#include "CanSettingPresenter.h"
// CCanSettingDialog 对话框

class CCanSettingDialog : public CDialogEx, ICanSettingView
{
	DECLARE_DYNAMIC(CCanSettingDialog)

public:
	CCanSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCanSettingDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_cBoxChannel;
	CComboBox m_cBoxBitRate;

public:
	virtual CanBus GetCanBus();
	virtual bool SetCanBus(CanBus info);
	virtual bool CanDeivceIsOpened();
	virtual bool CanDeivceIsClosed();
	virtual bool CanDeivceIsError();
	void CanBusOpenClose();
	afx_msg void OnBnClickedBtnCanOpenPort();
	afx_msg void OnBnClickedBtnCanClosePort();
	virtual BOOL OnInitDialog();
	CanSettingPresenter* m_pCanSettingPresenter;
	virtual void OnOK();
};

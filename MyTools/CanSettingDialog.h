#pragma once
#include "ICanSettingView.h"
#include "afxwin.h"
#include "CanSettingPresenter.h"
// CCanSettingDialog �Ի���

class CCanSettingDialog : public CDialogEx, ICanSettingView
{
	DECLARE_DYNAMIC(CCanSettingDialog)

public:
	CCanSettingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCanSettingDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_CAN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

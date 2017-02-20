#pragma once
#include "afxwin.h"

#include "IComSettingView.h"
#include "ComSettingPresenter.h"
// CComSettingDialog 对话框

class CComSettingDialog : public CDialogEx, IComSettingView
{
	DECLARE_DYNAMIC(CComSettingDialog)

public:
	CComSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComSettingDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_COM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnComOpenPort();
	afx_msg void OnBnClickedBtnComClosePort();
	CComboBox m_PortNumber;
	CComboBox m_PortRate;
	CComboBox m_PortDataBit;
	CComboBox m_PortStopBit;
	CComboBox m_PortParity;

	ComSettingPresenter* m_pComSettingPresenter;

	virtual bool InitComPortName(CArray<CString> * list);
	virtual ComPort GetComPort();
	virtual bool SetComPort(ComPort info);
	virtual bool ComPortIsOpened();
	virtual bool ComPortIsClosed();
	virtual bool ComPortIsError();
	virtual BOOL OnInitDialog();
	void ComPortOpenClose();
	virtual void OnOK();
};

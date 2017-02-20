#pragma once
#include "IComSettingView.h"
#include "IComTaskModel.h"

class ComSettingPresenter : IComPortStatusListener
{
public:
	ComSettingPresenter(IComSettingView * view=NULL);
	~ComSettingPresenter();
	void Init();
	void OpenComPort(ComPort port);
	void CloseComPort();

private:
	IComSettingView* m_pComSetView;
	IComTaskModel * m_pComTaskModel;
public:
	bool ComPortOpenSuccess();
	bool ComPortCloseSuccess();
};


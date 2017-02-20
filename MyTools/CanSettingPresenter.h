#pragma once
#include "ICanDeviceStatusListener.h"
#include "ICanSettingView.h"
#include "ICanTaskModel.h"
#include "CanTaskModel.h"
class CanSettingPresenter :public ICanDeviceStatusListener
{
public:

	CanSettingPresenter(ICanSettingView * view)
		: m_pCanSettingView(NULL)
	{
		m_pCanTaskModel = CanTaskModel::GetInstance();
		m_pCanSettingView = view;
	}

	~CanSettingPresenter()
	{
	}
private:
	ICanSettingView *m_pCanSettingView;
	ICanTaskModel* m_pCanTaskModel;
public:
	bool CanDeviceOpenSuccess()
	{
		return m_pCanSettingView->CanDeivceIsOpened();
	}
	bool CanDeviceCloseSuccess()
	{
		return m_pCanSettingView->CanDeivceIsClosed();
	}

	bool OpenCanDevice(CanBus info)
	{
		return m_pCanTaskModel->OpenCanDevice(info, this);
	}

	bool CloseCanDevice()
	{
		return m_pCanTaskModel->CloseCanDevice();
	}

	void Init()
	{
		m_pCanSettingView->SetCanBus(m_pCanTaskModel->GetSavedCanDeviceInfo());
	}
};


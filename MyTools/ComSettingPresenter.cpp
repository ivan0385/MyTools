#include "stdafx.h"
#include "ComSettingPresenter.h"
#include "ComTaskModel.h"

ComSettingPresenter::ComSettingPresenter(IComSettingView * view)
: m_pComSetView(NULL)
{
	m_pComTaskModel = ComTaskModel::GetInstance(); 
	m_pComSetView = view;
}

ComSettingPresenter::~ComSettingPresenter()
{
}


void ComSettingPresenter::Init()
{
	CArray<CString> list;
	if(m_pComTaskModel->GetComportList(&list))
		m_pComSetView->InitComPortName(&list);

	m_pComSetView->SetComPort(m_pComTaskModel->GetSavedComportInfo());
}


void ComSettingPresenter::OpenComPort(ComPort port)
{
	m_pComTaskModel->OpenComport(port,this);
}


void ComSettingPresenter::CloseComPort()
{
	m_pComTaskModel->CloseComport();
}


bool ComSettingPresenter::ComPortOpenSuccess()
{	
	return m_pComSetView->ComPortIsOpened();
}


bool ComSettingPresenter::ComPortCloseSuccess()
{
	return m_pComSetView->ComPortIsClosed();
}

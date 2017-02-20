#pragma once
#include "ICanButtonsSetupView.h"
#include "CanTaskModel.h"
class CanButtonsPresenter
{
public:

	CanButtonsPresenter(ICanButtonsSetupView * view)
	{
		m_pModel = CanTaskModel::GetInstance();
		m_pView = view;
	}

	~CanButtonsPresenter()
	{
	}

	void GetButtonCaptions()
	{
		static const CString TheCaptions[] = {
			 L"IGN ON",
			 L"IGN ACC",
			 L"IGN OFF",
			 L"TURN LEFT",
			 L"TURN LEFT OFF",
			 L"TURN RIGHT",
			 L"TURN RIGHT OFF",
			 L"REVERSE ON",
			 L"REVERSE OFF",

			 L"SCREEN 1",
			 L"SCREEN 2",

			 L"3DSIDE ON",
			 L"3DSIDE OFF",
			 L"PDCVIEW ON",
			 L"PDCVIEW OFF",
			 L"CARCOLOR",
			 L"AVMKEY",
			 L"CALIBKEY",
			 L"DVRKEY",

			 L"TOUCHSCRNSTATUS",
			 L"TOUCHSCRN_X",
			 L"TOUCHSCRN_Y",

			 L"VEHICLESPEED",
			 L"STEERINGANGLE",

			 L"PDC_FL_SENS_ERR",
			 L"PDC_FR_SENS_ERR",
			 L"PDC_FML_SENS_ERR",
			 L"PDC_FMR_SENS_ERR",
			 L"PDC_RL_SENS_ERR",
			 L"PDC_RML_SENS_ERR",
			 L"PDC_RMR_SENS_ERR",
			 L"PDC_RR_SENS_ERR",

			 L"PDC_SYS_SENS_ERR",
			 L"PDC_DIST_LVL_FL",
			 L"PDC_DIST_LVL_FML",
			 L"PDC_DIST_LVL_FMR",
			 L"PDC_DIST_LVL_FR",
			 L"PDC_DIST_LVL_RL",
			 L"PDC_DIST_LVL_RML",
			 L"PDC_DIST_LVL_RMR",
			 L"PDC_DIST_LVL_RR",
			 L"PDC_DIST_LVL_MIN",
			 L"PDC_RADAR_NUMBER",

			 L"LDW_ON",
			 L"LDW_OFF",
			 L"LDW_SENSOR1",
			 L"LDW_SENSOR2",
			 L"LDW_SENSOR3",

			 L"ST_AVM_VIDEO_ONOFF",
			 L"ST_PDC_VIEW_ONOFF",
			 L"ST_SIDEVIEW_ONOFF",
			 L"ST_LDW_ONOFF",
			 L"ST_LDW_SEN_ONOFF",

			 L"" //last one
		};

		m_pView->SetCaptionsData(TheCaptions);
	}

	std::vector<CAN_MSG> * GetCanMsgList()
	{
		return &(m_pModel->GetCanMatrix()->GetCanMatrix().msgList);
	}

	std::vector<CAN_SIGNAL> * GetCanSigList(int msgid)
	{
		for (unsigned int i = 0; i < m_pModel->GetCanMatrix()->GetCanMatrix().msgList.size(); i++)
		{
			if (m_pModel->GetCanMatrix()->GetCanMatrix().msgList[i].msgId == msgid)
			{
				return &(m_pModel->GetCanMatrix()->GetCanMatrix().msgList[i].signalList);
			}
		}
		return NULL;
	}
	CString GetPrjName()
	{
		return CMyString::toCString(m_pModel->GetCanMatrix()->GetCanMatrix().name);
	}

	void Init()
	{
		GetButtonCaptions();
	}
private:

private:
	ICanButtonsSetupView *m_pView;
	ICanTaskModel* m_pModel;
};


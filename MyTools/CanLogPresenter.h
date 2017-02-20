#pragma once
#include "ICanLogView.h"
#include "ICanTaskModel.h"
#include "CanTaskModel.h"
#include "CustSetup.h"

typedef struct
{
	CString which;
	unsigned int id;
	CString signame;
	int value;
}MY_CAN_PRE_UI_DATA;
class CanLogPresenter:i_data_receiver_can
{
public:

	CanLogPresenter(ICanLogView * view)
	{
		m_pCanLogDlg = view;
		m_pCanTaskModel = CanTaskModel::GetInstance();
		m_pCanTaskModel->SetDataReceiver(this);
	}

	virtual ~CanLogPresenter()
	{
	}

private:
	ICanLogView* m_pCanLogDlg;
	ICanTaskModel * m_pCanTaskModel;
	CString m_curPrj;
	std::vector<MY_CAN_PRE_UI_DATA> _my_ui_data;
public:
	virtual void receive(const VCI_CAN_OBJ* ba, int size)
	{
		m_pCanLogDlg->SetRcvMsg(ba, size);
	}

	virtual void send(const VCI_CAN_OBJ * ba, int cb)
	{
		m_pCanLogDlg->SetSendMsg(ba,cb);
	}

	void ParseMsg(VCI_CAN_OBJ &ba)
	{
	}

	bool SendData(CString inputStr, int value, bool bsend=true)
	{
		unsigned int i = 0;
		for (i = 0; i < _my_ui_data.size();i++)
		{
			if (_my_ui_data[i].which.IsEmpty())
				continue;
			if (inputStr == _my_ui_data[i].which)
			{
				if (value == -1)
					value = _my_ui_data[i].value;
				return m_pCanTaskModel->SendData(_my_ui_data[i].id, _my_ui_data[i].signame, value, bsend);
			}
		}
		return true;
	}


	bool SendData(ST_PACKET packetBuf)
	{
		return m_pCanTaskModel->SendData(packetBuf);
	}
	bool SendData(unsigned int msgid, CString sigName, int value)
	{
		return m_pCanTaskModel->SendData(msgid, sigName, value, true);
	}

	void SetCurPrj(CString value)
	{
		m_curPrj = value;
		CustSetup cs;
		std::vector<std::vector<CString>> rec;
		cs.GetSavedUserButtonDef(value, rec);
		_my_ui_data.clear();

		for (unsigned int i = 0; i < rec.size(); i++)
		{
			if (rec[i].size() >= 5)
			{
				MY_CAN_PRE_UI_DATA t;
				t.which = rec[i][0];
				t.id = (unsigned int)CMyString::toInt(rec[i][2]);
				t.signame = rec[i][3];
				t.value = CMyString::toInt(rec[i][4]);
				_my_ui_data.push_back(t);
			}
		}
	}

	bool getSigValue(int msgid, std::string sgnName,unsigned int &ret)
	{
		return m_pCanTaskModel->GetCanMatrix()->GetSignalValue(msgid, sgnName, ret);
	}

	bool GetAvmVideoOnOffStatus()
	{
		unsigned int ret = 0;

		unsigned int i = 0;
		for (i = 0; i < _my_ui_data.size(); i++)
		{
			if (L"ST_AVM_VIDEO_ONOFF" == _my_ui_data[i].which)
			{
				if (getSigValue(_my_ui_data[i].id, CMyString::toString(_my_ui_data[i].signame), ret))
					return !!ret;
				break;
			}
		}
		
		return false;
	}
	bool GetPdcOnOff()
	{
		unsigned int ret = 0;
		unsigned int i = 0;
		for (i = 0; i < _my_ui_data.size(); i++)
		{
			if (L"ST_PDC_VIEW_ONOFF" == _my_ui_data[i].which)
			{
				if (getSigValue(_my_ui_data[i].id, CMyString::toString(_my_ui_data[i].signame), ret))
					return !!ret;
				break;
			}
		}

		return false;
	}
	bool GetSideViewOnOff()
	{
		unsigned int ret=0;
		unsigned int i = 0;
		for (i = 0; i < _my_ui_data.size(); i++)
		{
			if (L"ST_SIDEVIEW_ONOFF" == _my_ui_data[i].which)
			{
				if (getSigValue(_my_ui_data[i].id, CMyString::toString(_my_ui_data[i].signame), ret))
					return !!ret;
				break;
			}
		}
		
		return false;
	}
	bool GetLdwOnOff()
	{
		unsigned int ret = 0;
		unsigned int i = 0;
		for (i = 0; i < _my_ui_data.size(); i++)
		{
			if (L"ST_LDW_ONOFF" == _my_ui_data[i].which)
			{
				if (getSigValue(_my_ui_data[i].id, CMyString::toString(_my_ui_data[i].signame), ret))
					return !!ret;
				break;
			}
		}
				
		return false;
	}
	int GetLdwSen()
	{
		unsigned int ret = 0;
		unsigned int i = 0;
		for (i = 0; i < _my_ui_data.size(); i++)
		{
			if (L"ST_LDW_SEN_ONOFF" == _my_ui_data[i].which)
			{
				if (getSigValue(_my_ui_data[i].id, CMyString::toString(_my_ui_data[i].signame), ret))
					return ret;

				break;
			}
		}

		
		return -1;
	}
};


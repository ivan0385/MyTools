#pragma once

#include "DataRcvIf.h"
#include "CanBus.h"
#include "CanDbc.h"
#include "ICanDeviceStatusListener.h"
#include "CanMatrixDef.h"
class ICanTaskModel
{
public:

	ICanTaskModel()
	{
	}

	virtual ~ICanTaskModel()
	{
	}

	virtual bool OpenCanDevice(CanBus port, ICanDeviceStatusListener * lis) = 0;
	virtual bool CloseCanDevice() = 0;
	virtual CanBus GetSavedCanDeviceInfo() = 0;
	virtual bool SetDataReceiver(i_data_receiver_can * rcv) = 0;
	virtual bool SendData(unsigned int msgid, CString sigName, int value, bool bSend) = 0;
	virtual bool SendData(ST_PACKET packetBuf)=0;
	virtual void LoadCanMatrixFromDbc(CString file) = 0;
	virtual CanDbc *GetCanMatrix() = 0;
};


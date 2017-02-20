#pragma once
#include "ControlCAN.h"
class ICanLogView
{
public:

	ICanLogView()
	{
	}

	virtual ~ICanLogView()
	{
	}
	virtual bool SetRcvMsg(const VCI_CAN_OBJ * msg, int size) = 0;
	virtual bool SetSendMsg(const VCI_CAN_OBJ * ba, int cb) = 0;
};


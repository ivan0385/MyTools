#pragma once
class ICanDeviceStatusListener
{
public:

	ICanDeviceStatusListener()
	{
	}

	virtual ~ICanDeviceStatusListener()
	{
	}

	virtual bool CanDeviceOpenSuccess()=0;
	virtual bool CanDeviceCloseSuccess()=0;
};


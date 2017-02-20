#pragma once
class IComPortStatusListener
{
public:
	IComPortStatusListener() {};
	virtual ~IComPortStatusListener() {};
	virtual bool ComPortOpenSuccess() = 0;
	virtual bool ComPortCloseSuccess() = 0;
};


#pragma once
#include "ComPort.h"
class IComSettingView
{
public:
	IComSettingView() {};
	virtual ~IComSettingView() {};
	virtual bool InitComPortName(CArray<CString> * list) = 0;
	virtual ComPort GetComPort() = 0;
	virtual bool SetComPort(ComPort info) = 0;
	virtual bool ComPortIsOpened() = 0;
	virtual bool ComPortIsClosed() = 0;
	virtual bool ComPortIsError() = 0;
};


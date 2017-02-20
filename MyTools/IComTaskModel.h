#pragma once
#include "IComPortStatusListener.h"
#include "ComPort.h"
#include "DataRcvIf.h"
class IComTaskModel
{
public:
	IComTaskModel() {};
	virtual ~IComTaskModel() {}; // 声明为虚析构函数，保证调用子类的虚构函数。
	virtual int OpenComport(ComPort port, IComPortStatusListener * lis) = 0;
	virtual int CloseComport() = 0;
	virtual bool GetComportList(CArray<CString> *list) = 0;
	virtual ComPort GetSavedComportInfo() = 0;
	virtual bool SetDataReceiver(i_data_receiver * rcv) = 0;
	virtual bool SendData(unsigned char *msg,int size) = 0;
	virtual bool IsDeviceOpen() = 0;
};


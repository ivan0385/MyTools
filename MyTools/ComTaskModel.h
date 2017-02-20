#pragma once
#include "IComTaskModel.h"
#include "ComPort.h"
//#include "mysingleton.h"
#include "CustEvent.h"
#include "CustUtils.h"
#include "DataRcvIf.h"
#include <list>
class c_overlapped : public OVERLAPPED
{
public:
	c_overlapped(bool manual, bool sigaled)
	{
		Internal = 0;
		InternalHigh = 0;
		Offset = 0;
		OffsetHigh = 0;
		hEvent = ::CreateEvent(nullptr, manual, sigaled ? TRUE : FALSE, nullptr);
	}
	~c_overlapped()
	{
		::CloseHandle(hEvent);
	}
};


class ComTaskModel : IComTaskModel
{
public:
	ComTaskModel();
	~ComTaskModel();
	struct thread_helper_context
	{
		ComTaskModel* that;
		enum class e_which
		{
			kRead,
			kWrite,
			kEvent,
		};
		e_which which;
	};

public:
	//MY_SINGLETON_CLASS(ComTaskModel);
	static IComTaskModel* GetInstance();
	virtual int OpenComport(ComPort port, IComPortStatusListener * lis);
	virtual int CloseComport();
	virtual bool GetComportList(CArray<CString> * list);
	virtual ComPort GetSavedComportInfo();
	virtual bool SetDataReceiver(i_data_receiver * rcv);
	virtual bool SendData(unsigned char *msg, int size);

public:
	bool IsDeviceOpen();

private:
	static ComTaskModel *pComTask; // 单态
	IComPortStatusListener *m_pComPortStatusListener; // 回调事件，通知串口状态。

private:
	bool GetComDeviceList(CArray<CString> * list);
	bool m_IsDeviceOpen;
	std::list<std::list<BYTE>> m_SendDataList; // 待发送数据列表。
	c_critical_locker _snd_lock;		// 多线程锁
private:
	CString byIndexComPort(CString value);
	DWORD byIndexBaud(CString value);
	BYTE byIndexData(CString value);
	BYTE byIndexStop(CString value);
	BYTE byIndexParity(CString value);
	bool TryOpenDevice(ComPort &comPort);
	bool TryCloseDevice();


	//内部工作流畅函数， 不要更改。
	HANDLE			_hComPort;
	thread_state	_thread_read;
	thread_state	_thread_write;
	HANDLE			_thread_write_tx_event;
	thread_state	_thread_event;
	// 串口事件监听
	c_com_event_listener	_event_listener;
	unsigned int thread_read();
	unsigned int thread_write();
	unsigned int thread_event();	
	static unsigned int __stdcall thread_helper(void* pv);
	bool _begin_threads();
	bool _end_threads();
	bool begin_threads();
	bool end_threads();
	// 数据接收器
	void add_data_receiver(i_data_receiver* receiver);
	void remove_data_receiver(i_data_receiver* receiver);
	void call_data_receivers(const unsigned char* ba, int cb);
	c_ptr_array<i_data_receiver>	_data_receivers;
	c_critical_locker				_data_receiver_lock;
public:
	bool m_bThreadRunning;
};
#pragma once

#include <list>
#include "ControlCAN.h"

#include "ICanTaskModel.h"
#include "ICanDeviceStatusListener.h"
#include "CustEvent.h"
#include "CustUtils.h"
#include "DataRcvIf.h"
#include "CanBus.h"
#include "CanDbc.h"


#define CAN_CHANNEL_0	0
#define CAN_CHANNEL_1	1


class CanTaskModel : public ICanTaskModel
{
public:
	CanTaskModel();
	~CanTaskModel();

public:
	static ICanTaskModel* GetInstance();
	virtual bool OpenCanDevice(CanBus port, ICanDeviceStatusListener * lis);
	virtual bool CloseCanDevice();
	virtual CanBus GetSavedCanDeviceInfo();
	virtual bool SetDataReceiver(i_data_receiver_can * rcv);
	virtual bool SendData(unsigned int msgid, CString sigName, int value, bool bSend);
	virtual bool SendData(ST_PACKET packetBuf);
private:
	static CanTaskModel *pCanTask; // 单态
	ICanDeviceStatusListener *m_pCanDeviceStatusListener; // 回调事件，通知串口状态。

private:
	int m_DevType;
	int m_DevIndex;
	int m_ActiveChannel;
	bool m_IsDeviceOpen;
	bool IsDeviceOpen();
	bool TryOpenDevice(CanBus info);
	bool TryCloseDevice();
	bool begin_threads();
	bool end_threads();

	bool CanSend(int iChannel, VCI_CAN_OBJ * sendBuf, int frames);
	bool CanSendNormalData(ST_PACKET packetBuf);

	int _getChannel(CString info);
	int _getbps(CString info);


	void add_data_receiver(i_data_receiver_can* receiver);
	void remove_data_receiver(i_data_receiver_can* receiver);
	void call_data_receivers(const VCI_CAN_OBJ* ba, int cb);
	void call_data_sender(const VCI_CAN_OBJ * ba, int cb);
	c_ptr_array<i_data_receiver_can>	_data_receivers;
	c_critical_locker	_data_receiver_lock;

	struct thread_helper_context
	{
		CanTaskModel* that;
		enum class e_which
		{
			kRead,
			kWrite,
		};
		e_which which;
	};
	unsigned int thread_read();
	unsigned int thread_write();
	static unsigned int __stdcall thread_helper(void* pv);

	thread_state	_thread_read;
	thread_state	_thread_write;

	bool _begin_threads();

	typedef struct
	{
		int msInterval; // 0:发送一次, 100ms:100ms循环一次
		bool enable;
		VCI_CAN_OBJ data;
	}CAN_SEND_DATA_PACKETS;
	std::list<CAN_SEND_DATA_PACKETS> _SendList;
	c_critical_locker _snd_lock;		// 多线程锁
public:
	CanDbc m_canDbc;
	virtual void LoadCanMatrixFromDbc(CString file);
	virtual CanDbc *GetCanMatrix();
	int parse_rcv_msg(const VCI_CAN_OBJ * ba, int cb);
};


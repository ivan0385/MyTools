#include "stdafx.h"
#include "CanTaskModel.h"

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib") 
#pragma comment(lib, "ControlCAN.lib") // ����ⲿ������

#include "common.h"
#include "MyString.h"
#include "CommonConfig.h"

CanTaskModel * CanTaskModel::pCanTask = NULL;

CanTaskModel::CanTaskModel()
: m_DevType(VCI_USBCAN2)
, m_DevIndex(0) // �ڼ��������can ����. ��һ��Ϊ0
, m_IsDeviceOpen(false)
, m_ActiveChannel(0) // ʹ���ĸ�channel
{
	//m_canDbc.GetSaveCanMatrixFromDb();
}


CanTaskModel::~CanTaskModel()
{
	TryCloseDevice();
}



ICanTaskModel* CanTaskModel::GetInstance()
{
	if (pCanTask == NULL)
		pCanTask = new CanTaskModel();
	return pCanTask;// MySingleton<ComTaskModel>::Get_Instance();
}


bool CanTaskModel::OpenCanDevice(CanBus port, ICanDeviceStatusListener * lis)
{
	m_pCanDeviceStatusListener = lis;
#if 1// save config
	CanBus info;
	CCommonConfig *tt = CCommonConfig::GetInstance();
	tt->addKey(L"CANBUS_RATE",port.GetBitRate());
	tt->addKey(L"CANBUS_CHANNEL", port.GetChannelNo());
#endif
	return TryOpenDevice(port);
}
bool CanTaskModel::CloseCanDevice()
{
	return TryCloseDevice();
}
CanBus CanTaskModel::GetSavedCanDeviceInfo()
{
#if 1// load config
	CanBus info;
	CCommonConfig *tt = CCommonConfig::GetInstance();
	tt->loadConfig();
	info.SetBitRate(tt->getKey(L"CANBUS_RATE"));
	info.SetChannelNo(tt->getKey(L"CANBUS_CHANNEL"));
#endif
	return info;
}
bool CanTaskModel::SetDataReceiver(i_data_receiver_can * rcv)
{
	add_data_receiver(rcv);
	return true;
}
bool CanTaskModel::SendData(unsigned int msgid, CString sigName, int value, bool bSend)
{
	CAN_MSG_DATA data;
	ST_PACKET packetBuf;

	if (!IsDeviceOpen())
	{
		AfxMessageBox(L"Can �˿�û�д�");
		return false;
	}

	m_canDbc.SetSignalValue(msgid, CMyString::toString(sigName), value, data);

	if (!bSend)
		return true;

	packetBuf.enable = data.msg->msgIsEnable;
	packetBuf.id = data.msg->msgId;
	if (data.msg->msgCycleTime > 0 && data.msg->msgSendType == 0)
		packetBuf.tmInterval = data.msg->msgCycleTime;
	else
		packetBuf.tmInterval = 0;
	packetBuf.data = data.msgData;
	
	CanSendNormalData(packetBuf);

	return true;
}


//////////
bool CanTaskModel::IsDeviceOpen()
{
	return m_IsDeviceOpen;
}


bool CanTaskModel::TryOpenDevice(CanBus info)
{
	if (m_canDbc.GetCanMatrix().msgList.size() == 0)
	{
		//AfxMessageBox(L"û��CAN���ݣ�������load can dbc�ļ�!");
		//return false;
	}
	else if (IsDeviceOpen())
	{
		//AfxMessageBox(L"CAN�Ѿ�����!");
		if (m_pCanDeviceStatusListener)
			m_pCanDeviceStatusListener->CanDeviceOpenSuccess();
		return true;
	}
	//���豸
	if (VCI_OpenDevice(m_DevType, m_DevIndex, 0) != 1)
	{
		AfxMessageBox(L"open failed");
		return false;
	}

	m_ActiveChannel = _getChannel(info.GetChannelNo());
	unsigned int bps = _getbps(info.GetBitRate());

	VCI_INIT_CONFIG InitInfo[1];
	InitInfo->Timing0 = (UCHAR)((bps & 0xff00) >> 8);
	InitInfo->Timing1 = (UCHAR)(bps & 0xff) ;
	InitInfo->Filter = 0;
	InitInfo->AccCode = 0x80000000;
	InitInfo->AccMask = 0xFFFFFFFF;
	InitInfo->Mode = 0; // 0:��������ģʽ

	if (m_ActiveChannel == 0)//��ʼ��ͨ��1
	{
		if (VCI_InitCAN(m_DevType, m_DevIndex, 0, InitInfo) != 1)
		{
			AfxMessageBox(L"Init-CAN Channel 0 failed!");
			return false;
		}
		Sleep(100);
		if (VCI_StartCAN(m_DevType, m_DevIndex, 0) != 1)
		{
			AfxMessageBox(L"Start-CAN Channel 0 failed!");
			return false;
		}
	}

	else if (m_ActiveChannel == 1)//��ʼ��ͨ��2
	{
		if (VCI_InitCAN(m_DevType, m_DevIndex, 1, InitInfo) != 1)
		{
			AfxMessageBox(L"Init-CAN Channel 1 failed!");
			return false;
		}
		Sleep(100);
		if (VCI_StartCAN(m_DevType, m_DevIndex, 1) != 1)
		{
			AfxMessageBox(L"Start-CAN Channel 1 failed!");
			return false;
		}
	}

	//AfxMessageBox(L"CAN�����ɹ�!");
	if (m_pCanDeviceStatusListener)
		m_pCanDeviceStatusListener->CanDeviceOpenSuccess();
	_begin_threads();

	m_IsDeviceOpen = true;
	return true;
}

bool CanTaskModel::TryCloseDevice()
{
	if (!IsDeviceOpen())
	{
		//AfxMessageBox(L"CAN�豸�ѹر�!");
		if (m_pCanDeviceStatusListener)
			m_pCanDeviceStatusListener->CanDeviceCloseSuccess();
		return true;
	}
	_snd_lock.lock();
	_SendList.clear();
	_snd_lock.unlock();
	end_threads();

	if (VCI_CloseDevice(m_DevType, m_DevIndex) != 1)
	{
		AfxMessageBox(L"CAN Close failed��");
		return false;
	}

	//AfxMessageBox(L"CAN�豸�ѹر�!");
	if (m_pCanDeviceStatusListener)
		m_pCanDeviceStatusListener->CanDeviceCloseSuccess();

	m_IsDeviceOpen = false;
	return true;
}



bool CanTaskModel::_begin_threads()
{
	thread_helper_context* pctx = nullptr;

	// �������߳�
	_thread_read.hEventToBegin = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	_thread_read.hEventToExit = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

	pctx = new thread_helper_context;
	pctx->that = this;
	pctx->which = thread_helper_context::e_which::kRead;
	_thread_read.hThread = (HANDLE)::_beginthreadex(nullptr, 0, thread_helper, pctx, 0, nullptr);

	if (!_thread_read.hEventToBegin || !_thread_read.hEventToExit || !_thread_read.hThread)
	{
		::MessageBox(NULL, L"Ӧ�ó����ʼ��ʧ��, �����˳�!", NULL, MB_ICONHAND);
		::exit(1);
	}

	// ����д�߳�
	_thread_write.hEventToBegin = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	_thread_write.hEventToExit = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

	pctx = new thread_helper_context;
	pctx->that = this;
	pctx->which = thread_helper_context::e_which::kWrite;
	_thread_write.hThread = (HANDLE)::_beginthreadex(nullptr, 0, thread_helper, pctx, 0, nullptr);

	if (!_thread_write.hEventToBegin || !_thread_write.hEventToExit || !_thread_write.hThread)
	{
		::MessageBox(NULL, L"Ӧ�ó����ʼ��ʧ��, �����˳�!", NULL, MB_ICONHAND);
		::exit(1);
	}

	begin_threads();

	return true;
}


unsigned int __stdcall CanTaskModel::thread_helper(void* pv)
{
	auto pctx = reinterpret_cast<thread_helper_context*>(pv);
	auto comm = pctx->that;
	auto which = pctx->which;

	delete pctx;

	switch (which)
	{
	case thread_helper_context::e_which::kRead:
		return comm->thread_read();
	case thread_helper_context::e_which::kWrite:
		return comm->thread_write();
	default:
		return 1;
	}
}

unsigned int CanTaskModel::thread_read()
{
	debug_out(("[CAN-���߳�] ����\n"));
	::WaitForSingleObject(_thread_read.hEventToBegin, INFINITE);

	debug_out(("[CAN-���߳�] ��ʼ����...\n"));
	if (!IsDeviceOpen())
	{
		debug_out(("[CAN-���߳�] �豸û�д�, �˳���...\n"));
		::ResetEvent(_thread_read.hEventToExit);
		return 0;
	}

	HANDLE handles[1];
	handles[0] = _thread_read.hEventToExit;
	debug_out(("[CAN-���߳�] start!\n"));

_get_packet:
	switch (::WaitForMultipleObjects(_countof(handles), handles, FALSE, 0))
	{
	case WAIT_OBJECT_0 + 0:
		debug_out(("[CAN-���߳�] �յ��˳��¼�!\n"));
		::ResetEvent(_thread_read.hEventToExit);
		return 0;
	}
	if (!IsDeviceOpen())
	{
		debug_out(("[CAN-���߳�] û�й���, �˳���...\n"));
		::ResetEvent(_thread_read.hEventToExit);
		return 0;
	}
	int NumValue;
	VCI_CAN_OBJ pCanObj[100];

	for (int kCanIndex = 0; kCanIndex<2; kCanIndex++)
	{
		//���ö�̬���ӿ����պ���
		NumValue = VCI_Receive(m_DevType, m_DevIndex, kCanIndex, pCanObj, 100, 0);
		//������Ϣ�б���ʾ
		if (NumValue>0)
		{
			call_data_receivers(pCanObj, NumValue);
			parse_rcv_msg(pCanObj, NumValue);
		}
	}
	Sleep(30);
	goto _get_packet;
}


unsigned int CanTaskModel::thread_write()
{
	DWORD icount;

	DWORD t1 = 0, t2 = 0, t3 = 0,t4=0,t5=0;
	bool bTm500 = false;
	bool bTm100 = false;
	bool bTm20 = false;
	bool bTm1000 = false;

_wait_for_work:
	debug_out(("[CAN-д�߳�] ����\n"));
	::WaitForSingleObject(_thread_write.hEventToBegin, INFINITE);

	debug_out(("[CAN-д�߳�] ��ʼ����...\n"));
	if (!IsDeviceOpen())
	{
		debug_out(("[CAN-д�߳�] û�й���, �˳���...\n"));
		::SetEvent(_thread_write.hEventToExit);
		return 0;
	}

	VCI_CAN_OBJ *sendBuf = (VCI_CAN_OBJ *)malloc(sizeof(VCI_CAN_OBJ)* 48);
	HANDLE handles[1];
	handles[0] = _thread_write.hEventToExit;
	debug_out(("[CAN-д�߳�] ȡ���ݰ���...\n"));

_get_packet:

	while (timeGetTime() - t3 < 10)
	{
		t3 = timeGetTime();
		Sleep(1);
	}

	if (timeGetTime() - t1 > 100)
	{
		t1 = timeGetTime();
		bTm100 = true;
	}
	if (timeGetTime() - t2 > 20)
	{
		t2 = timeGetTime();
		bTm20 = true;
	}
	if (timeGetTime() - t4 > 500)
	{
		t4 = timeGetTime();
		bTm500 = true;
	}

	if (timeGetTime() - t5 > 1000)
	{
		t5 = timeGetTime();
		bTm1000 = true;
	}
	
	switch (::WaitForMultipleObjects(_countof(handles), handles, FALSE, 1))
	{
	case WAIT_OBJECT_0 + 0:
		debug_out(("[CAN-д�߳�] �յ��˳��¼�!\n"));
		::ResetEvent(_thread_write.hEventToExit);
		free(sendBuf);
		return 0;
	}

	icount = 0;
	std::list<CAN_SEND_DATA_PACKETS>::iterator pack;
	_snd_lock.lock();
	for (pack = _SendList.begin(); pack != _SendList.end();)
	{
		if (pack->enable)
		{
			if (pack->msInterval == 0)
				sendBuf[icount++] = pack->data;
			else if (pack->msInterval == 10)
				sendBuf[icount++] = pack->data;
			else if (bTm20 && pack->msInterval == 20)
				sendBuf[icount++] = pack->data;
			else if (bTm100 && pack->msInterval == 100)
				sendBuf[icount++] = pack->data;
			else if (bTm500 && pack->msInterval == 500)
				sendBuf[icount++] = pack->data;
			else if (bTm1000 && pack->msInterval == 1000)
				sendBuf[icount++] = pack->data;
		}
		if (pack->msInterval < 10 || !pack->enable)
		{
			pack = _SendList.erase(pack);
		}
		else
			pack++;
	}
	_snd_lock.unlock();
	if (icount > 0)
	{
		CanSend(m_ActiveChannel, sendBuf, icount);
		call_data_sender(sendBuf, icount);
	}
	bTm1000 = false;
	bTm500 = false;
	bTm100 = false;
	bTm20 = false;

	goto _get_packet;
}

bool CanTaskModel::begin_threads()
{
	::ResetEvent(_thread_read.hEventToExit);
	::ResetEvent(_thread_write.hEventToExit);

	::SetEvent(_thread_read.hEventToBegin);
	::SetEvent(_thread_write.hEventToBegin);

	return true;
}

bool CanTaskModel::end_threads()
{
	::ResetEvent(_thread_read.hEventToBegin);
	::ResetEvent(_thread_write.hEventToBegin);

	::SetEvent(_thread_write.hEventToExit);
	::SetEvent(_thread_read.hEventToExit);
	// �ڶ�д�߳��˳�֮ǰ, ����end��Ϊ����״̬
	// ����ȵ������߳̾��˳�����״̬��������������
	debug_out(("CAN-�ȴ� [���߳�] ����...\n"));
	while (::WaitForSingleObject(_thread_read.hEventToExit, 0) == WAIT_OBJECT_0);


	debug_out(("CAN-�ȴ� [д�߳�] ����...\n"));
	while (::WaitForSingleObject(_thread_write.hEventToExit, 0) == WAIT_OBJECT_0);

	return true;
}

void CanTaskModel::call_data_sender(const VCI_CAN_OBJ * ba, int cb)
{
	_data_receiver_lock.lock();
	for (int i = 0; i < _data_receivers.size(); i++)
	{
		_data_receivers[i]->send(ba,cb);
	}
	_data_receiver_lock.unlock();
}
void CanTaskModel::call_data_receivers(const VCI_CAN_OBJ * ba, int cb)
{
	_data_receiver_lock.lock();
	for (int i = 0; i < _data_receivers.size(); i++)
	{
		_data_receivers[i]->receive(ba, cb);
	}
	_data_receiver_lock.unlock();
}

void CanTaskModel::remove_data_receiver(i_data_receiver_can* receiver)
{
	_data_receiver_lock.lock();
	_data_receivers.remove(receiver);
	_data_receiver_lock.unlock();

}

void CanTaskModel::add_data_receiver(i_data_receiver_can* receiver)
{
	_data_receiver_lock.lock();
	_data_receivers.add(receiver);
	_data_receiver_lock.unlock();
}


bool CanTaskModel::CanSend(int iChannel, VCI_CAN_OBJ * sendBuf, int frames)
{
	if (sendBuf == nullptr)
		return false;

	//���ö�̬���ӿⷢ�ͺ���
	int flag = VCI_Transmit(m_DevType, m_DevIndex, iChannel, sendBuf, frames);//CAN message send
	if (flag<1)
	{
#if 0//def _DEBUG
		if (flag == -1)
			AfxMessageBox(L"failed- device not open\n");
		else if (flag == 0)
			AfxMessageBox(L"send error\n");
#endif
		return false;
	}

	return true;
}


bool CanTaskModel::CanSendNormalData(ST_PACKET packetBuf) // CAN2.0A data frame
{
	CAN_SEND_DATA_PACKETS packet;
	VCI_CAN_OBJ sendBuf[1] = { 0, };

	if (!IsDeviceOpen())
		return false;

	sendBuf->ExternFlag = 0;
	sendBuf->DataLen = packetBuf.data.size();
	sendBuf->RemoteFlag = 0;
	sendBuf->ID = packetBuf.id;
	sendBuf->SendType = 0; // 0: ʧ���ظ�, 1:���η���
	for (int i = 0; i<sendBuf->DataLen; i++)
		sendBuf->Data[i] = packetBuf.data[i];

	packet.data = sendBuf[0];
	packet.enable = packetBuf.enable;
	packet.msInterval = packetBuf.tmInterval;

	// add to write list
	_snd_lock.lock();
	if (packet.msInterval>=10)
	{
		std::list<CAN_SEND_DATA_PACKETS>::iterator pack;
		for (pack = _SendList.begin(); pack != _SendList.end();)
		{
			if (packetBuf.id == pack->data.ID)
				pack = _SendList.erase(pack);
			else
				pack++;
		}
	}
	_SendList.push_back(packet);
	debug_out(("CAN-send:(enable=%d,id=0x%x)0x%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n", packetBuf.enable, packet.data.ID, sendBuf->Data[0], sendBuf->Data[1], sendBuf->Data[2], sendBuf->Data[3], sendBuf->Data[4], sendBuf->Data[5], sendBuf->Data[6], sendBuf->Data[7]));

	_snd_lock.unlock();
	return true;
}

int CanTaskModel::_getChannel(CString info)
{
	CString arr[] = { L"ͨ�� 1", L"ͨ�� 2", NULL };
	static DWORD iarr[] = { CAN_CHANNEL_0, CAN_CHANNEL_1,-1 };
	int i = 0;

	for (i = 0;; i++)
	{
		if (arr[i].IsEmpty())
			break;
		else if (info == arr[i])
			break;
	}
	return iarr[i];
}

int CanTaskModel::_getbps(CString info)
{
	CString arr[] = { L"100 kbps", L"125 kbps", L"200 kpbs", L"250 kbps", L"400 kbps", L"500 kbps", L"666 kbps", L"800 kbps", L"1000 kbps", NULL };
	static DWORD iarr[] = { 0x041c, 0x031c,0x81fa,0x011c,0x08fa,0x001c,0x80b6,0x0016,0014, -1 };
	int i = 0;

	for (i = 0;; i++)
	{
		if (arr[i].IsEmpty())
			break;
		else if (info == arr[i])
			break;
	}
	return iarr[i];
}

void CanTaskModel::LoadCanMatrixFromDbc(CString file)
{
	m_canDbc.ParseDbcFile(file);
}

CanDbc *CanTaskModel::GetCanMatrix()
{
	return &m_canDbc;
}


bool CanTaskModel::SendData(ST_PACKET packetBuf)
{
	return CanSendNormalData(packetBuf);
}


int CanTaskModel::parse_rcv_msg(const VCI_CAN_OBJ * ba, int cb)
{
	for (int i = 0; i < cb; i++)
	{
		m_canDbc.UpdateMsgByRcv(ba[i].ID, ba[i].Data, ba[i].DataLen);
	}
	return 0;
}

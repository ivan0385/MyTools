#include "stdafx.h"
#include "ComTaskModel.h"

#include <SetupAPI.h>
#include <WinIoCtl.h>

#include "CommonConfig.h"
#include "common.h"

#pragma comment (lib,"setupapi.lib")

ComTaskModel * ComTaskModel::pComTask = NULL;

ComTaskModel::ComTaskModel()
: m_pComPortStatusListener(NULL)
, m_IsDeviceOpen(false)
, _hComPort(NULL)
, m_bThreadRunning(false)
{
	m_SendDataList.empty();
}


ComTaskModel::~ComTaskModel()
{
	_data_receiver_lock.lock();
	_data_receivers.empty();
	_data_receiver_lock.unlock();

	TryCloseDevice();
	_end_threads();
}




////////////////// singleton interface////////////////////////
IComTaskModel* ComTaskModel::GetInstance()
{
	if (pComTask == NULL)
		pComTask = new ComTaskModel;
	return pComTask;// MySingleton<ComTaskModel>::Get_Instance();
}


int ComTaskModel::OpenComport(ComPort port, IComPortStatusListener * lis)
{
	m_pComPortStatusListener = lis;

	TryOpenDevice(port);

#if 1// save config
	CCommonConfig *tt = CCommonConfig::GetInstance();
	tt->addKey(L"com_port", port.GetPortName());
	tt->addKey(L"com_rate", port.GetBaudRate());
	tt->addKey(L"com_databit", port.GetDataBit());
	tt->addKey(L"com_stopbit", port.GetStopBit());
	tt->addKey(L"com_parity", port.GetParity());

#endif
	return 0;
}


int ComTaskModel::CloseComport()
{
	TryCloseDevice();
	return 0;
}


bool ComTaskModel::GetComportList(CArray<CString> *list)
{
	return GetComDeviceList(list);
}


ComPort ComTaskModel::GetSavedComportInfo()
{
#if 1// load config
	ComPort port;
	CCommonConfig *tt = CCommonConfig::GetInstance();
	tt->loadConfig();
	port.SetPortName(tt->getKey(L"com_port"));
	port.SetBaudRate(tt->getKey(L"com_rate"));
	port.SetDataBit(tt->getKey(L"com_databit"));
	port.SetStopBit(tt->getKey(L"com_stopbit"));
	port.SetParity(tt->getKey(L"com_parity"));
#endif
	return port;
}

bool ComTaskModel::SetDataReceiver(i_data_receiver * rcv)
{
	add_data_receiver(rcv);
	return true;
}

bool ComTaskModel::SendData(unsigned char *msg, int size)
{
	if (!IsDeviceOpen())
		return false;

	std::list<BYTE> pkg;
	pkg.empty();
	for (int i = 0; i < size; i++)
	{
		pkg.push_back(msg[i]);
	}
	_snd_lock.lock();
	m_SendDataList.push_back(pkg);
	::SetEvent(_thread_write_tx_event);
	_snd_lock.unlock();
	return true;
}


///////////////public functions///////////////////////////
bool ComTaskModel::IsDeviceOpen()
{
	return !!_hComPort;
}







///////////////////////private functions////////////////////

bool ComTaskModel::TryOpenDevice(ComPort &comPort)
{
	if (IsDeviceOpen())
	{
		m_pComPortStatusListener->ComPortOpenSuccess();
		AfxMessageBox(L"COM 端口已经打开了!");
		return true;
	}
	//打开端口


	COMMTIMEOUTS		_timeouts;
	DCB					_dcb;
	_timeouts.ReadIntervalTimeout = MAXDWORD;
	_timeouts.ReadTotalTimeoutMultiplier = 0;
	_timeouts.ReadTotalTimeoutConstant = 0;
	_timeouts.WriteTotalTimeoutMultiplier = 0;
	_timeouts.WriteTotalTimeoutConstant = 0;

	_hComPort = ::CreateFile(byIndexComPort(comPort.GetPortName()), GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (_hComPort == INVALID_HANDLE_VALUE)
	{
		_hComPort = NULL;
		DWORD dwErr = ::GetLastError();
		if (dwErr == ERROR_FILE_NOT_FOUND)
		{
			//TODO
		}
		return false;
	}

	if (!::GetCommState(_hComPort, &_dcb))
	{
		return false;
	}

	_dcb.fBinary = TRUE;
	_dcb.BaudRate = byIndexBaud(comPort.GetBaudRate());
	_dcb.fParity = byIndexParity(comPort.GetParity()) == NOPARITY ? FALSE : TRUE;;
	_dcb.Parity = byIndexParity(comPort.GetParity());
	_dcb.ByteSize = byIndexData(comPort.GetDataBit());
	_dcb.StopBits = byIndexStop(comPort.GetStopBit());
	if (!::SetCommState(_hComPort, &_dcb))
	{
		//_notifier->msgerr("SetCommState()错误");
		return false;
	}
	if (!::SetCommMask(_hComPort,
		EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY
		| EV_CTS | EV_DSR | EV_RLSD
		| EV_BREAK | EV_ERR
		| EV_RING
		| EV_PERR | EV_RX80FULL))
	{
		//_notifier->msgerr("SetCommMask()错误");
		return false;
	}
	if (!::SetCommTimeouts(_hComPort, &_timeouts))
	{
		//_notifier->msgerr("设置串口超时错误");
		return false;
	}
	PurgeComm(_hComPort, PURGE_TXCLEAR | PURGE_TXABORT);
	PurgeComm(_hComPort, PURGE_RXCLEAR | PURGE_RXABORT);
	
	_begin_threads();

	m_IsDeviceOpen = true;
	m_pComPortStatusListener->ComPortOpenSuccess();

	return true;
}

bool ComTaskModel::TryCloseDevice()
{
	_snd_lock.lock();
	m_SendDataList.clear();
	::ResetEvent(_thread_write_tx_event);
	_snd_lock.unlock();

	if (!IsDeviceOpen())
	{
		if (m_pComPortStatusListener)
			m_pComPortStatusListener->ComPortCloseSuccess();
		return true;
	}

	end_threads();
	if(::CloseHandle(_hComPort)==TRUE)
		_hComPort = NULL;

	if (m_pComPortStatusListener)
		m_pComPortStatusListener->ComPortCloseSuccess();
	return true;
}

bool ComTaskModel::GetComDeviceList(CArray<CString> * list)
{
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	SP_DEVINFO_DATA spdata = { 0 };
	GUID guid = GUID_DEVINTERFACE_COMPORT;
	
	hDevInfo = SetupDiGetClassDevs(&guid, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	list->RemoveAll();

	spdata.cbSize = sizeof(spdata);
	for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spdata); i++)
	{
		wchar_t buff[1024] = { 0, };
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spdata, SPDRP_FRIENDLYNAME, NULL,
			PBYTE(buff), _countof(buff), NULL))
		{
			CString tmp(buff);
			CString tmp1(buff);
			int idx = tmp.Find(L"(COM");
			if (idx >= 0)
			{
				int id = _wtoi(tmp.Right(tmp.GetLength() - idx - 4));

				tmp = tmp.SpanExcluding(L"(COM");
				tmp1.Format(L"COM%-13d:", id); 
				tmp1.Append(tmp);
				list->Add(tmp1);
			}
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return true;
}






//////////////////////////////////////////////////////////////////////////
void ComTaskModel::call_data_receivers(const unsigned char* ba, int cb)
{
	_data_receiver_lock.lock();
	for (int i = 0; i < _data_receivers.size(); i++)
	{
		_data_receivers[i]->receive(ba, cb);
	}
	_data_receiver_lock.unlock();
}

void ComTaskModel::remove_data_receiver(i_data_receiver* receiver)
{
	_data_receiver_lock.lock();
	_data_receivers.remove(receiver);
	_data_receiver_lock.unlock();

}

void ComTaskModel::add_data_receiver(i_data_receiver* receiver)
{
	_data_receiver_lock.lock();
	_data_receivers.add(receiver);
	_data_receiver_lock.unlock();
}
//////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
////// parse the comport property strings.
///////////////////////////////////////
#if 1// parse the comport property strings.
CString ComTaskModel::byIndexComPort(CString value)
{
	CString PortName;	
	int idx = value.Find(L"COM");
	if (idx >= 0)
	{
		int id = _wtoi(value.Right(value.GetLength() - idx - 3));
		PortName.Format(L"\\\\.\\COM%d", id);
	}
	return PortName;
}

DWORD ComTaskModel::byIndexBaud(CString value)
{
	CString aBaudRate[] = { L"9600", L"19200", L"38400", L"57600", L"115200", NULL };
	static DWORD iBaudRate[] = { CBR_9600, CBR_19200, CBR_38400, CBR_57600, CBR_115200, -1 };
	
	int i = 0;
	
	for (i = 0;; i++)
	{
		if (aBaudRate[i].IsEmpty())
			break;
		else if (value == aBaudRate[i])
			break;
	}
	return iBaudRate[i];
}

BYTE ComTaskModel::byIndexData(CString value)
{
	CString aDataSize[] = { L"8 BIT", L"7 BIT", L"6 BIT", L"5 BIT", NULL };
	static BYTE iDataSize[] = { 8, 7, 6, 5, -1 };

	int i = 0;

	for (i = 0;; i++)
	{
		if (aDataSize[i].IsEmpty())
			break;
		else if (value == aDataSize[i])
			break;
	}
	
	return iDataSize[i];
}

BYTE ComTaskModel::byIndexStop(CString value)
{
	CString aStopBit[] = { L"1 BIT",L"2 BIT", NULL };
	static BYTE iStopBit[] = { ONESTOPBIT, TWOSTOPBITS,-1 };
	
	int i = 0;

	for (i = 0;; i++)
	{
		if (aStopBit[i].IsEmpty())
			break;
		else if (value == aStopBit[i])
			break;
	}
	return iStopBit[i];
}

BYTE ComTaskModel::byIndexParity(CString value)
{
	CString aParity[] = { L"None", L"Odd", L"Even",NULL };
	static BYTE iParity[] = { NOPARITY, ODDPARITY, EVENPARITY, -1 };
	
	int i = 0;

	for (i = 0;; i++)
	{
		if (aParity[i].IsEmpty())
			break;
		else if (value == aParity[i])
			break;
	}

	return iParity[i];
}
#endif


///////////////////////////////////////
////// threads control.
///////////////////////////////////////
bool ComTaskModel::_begin_threads()
{
	thread_helper_context* pctx = nullptr;

	// 开启读线程
	_thread_read.hEventToBegin = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	_thread_read.hEventToExit = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

	pctx = new thread_helper_context;
	pctx->that = this;
	pctx->which = thread_helper_context::e_which::kRead;
	_thread_read.hThread = (HANDLE)::_beginthreadex(nullptr, 0, thread_helper, pctx, 0, nullptr);

	if (!_thread_read.hEventToBegin || !_thread_read.hEventToExit || !_thread_read.hThread)
	{
		::MessageBox(NULL, L"应用程序初始化失败, 即将退出!", NULL, MB_ICONHAND);
		::exit(1);
	}

	// 开启写线程
	_thread_write.hEventToBegin = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	_thread_write.hEventToExit = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
	_thread_write_tx_event = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
	pctx = new thread_helper_context;
	pctx->that = this;
	pctx->which = thread_helper_context::e_which::kWrite;
	_thread_write.hThread = (HANDLE)::_beginthreadex(nullptr, 0, thread_helper, pctx, 0, nullptr);

	if (!_thread_write.hEventToBegin || !_thread_write.hEventToExit || !_thread_write.hThread)
	{
		::MessageBox(NULL, L"应用程序初始化失败, 即将退出!", NULL, MB_ICONHAND);
		::exit(1);
	}

	// 开启事件线程
	_thread_event.hEventToBegin = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	_thread_event.hEventToExit = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

	pctx = new thread_helper_context;
	pctx->that = this;
	pctx->which = thread_helper_context::e_which::kEvent;
	_thread_event.hThread = (HANDLE)::_beginthreadex(nullptr, 0, thread_helper, pctx, 0, nullptr);

	if (!_thread_event.hEventToBegin || !_thread_event.hEventToExit || !_thread_event.hThread)
	{
		::MessageBox(NULL, L"应用程序初始化失败, 即将退出!", NULL, MB_ICONHAND);
		::exit(1);
	}

	begin_threads();
	m_bThreadRunning = true;
	return true;
}

bool ComTaskModel::begin_threads()
{
	::ResetEvent(_thread_read.hEventToExit);
	::ResetEvent(_thread_write.hEventToExit);
	::ResetEvent(_thread_event.hEventToExit);
	::ResetEvent(_thread_write_tx_event);
	

	::SetEvent(_thread_read.hEventToBegin);
	::SetEvent(_thread_write.hEventToBegin);
	::SetEvent(_thread_event.hEventToBegin);

	return true;
}

bool ComTaskModel::end_threads()
{
	if(m_bThreadRunning)
	{
		::ResetEvent(_thread_read.hEventToBegin);
		::ResetEvent(_thread_write.hEventToBegin);
		::ResetEvent(_thread_event.hEventToBegin);

		::SetEvent(_thread_read.hEventToExit);
		::SetEvent(_thread_write.hEventToExit);
		::SetEvent(_thread_event.hEventToExit);
		::SetEvent(_thread_write_tx_event);

		// 在读写线程退出之前, 两个end均为激发状态
		// 必须等到两个线程均退出工作状态才能有其它操作
		debug_out(("COM-等待 [读线程] 结束...\n"));
		while (::WaitForSingleObject(_thread_read.hEventToExit, 0) == WAIT_OBJECT_0);
		debug_out(("COM-等待 [写线程] 结束...\n"));
		//while (::WaitForSingleObject(_thread_write.hEventToExit, 0) == WAIT_OBJECT_0);
		debug_out(("COM-等待 [事件线程] 结束...\n"));
		while (::WaitForSingleObject(_thread_event.hEventToExit, 0) == WAIT_OBJECT_0);
	}
	return true;
}

bool ComTaskModel::_end_threads()
{
	if(m_bThreadRunning)
	{
		// 由线程在退出之前设置并让当前线程等待他们的结束
		::ResetEvent(_thread_read.hEventToExit);
		::ResetEvent(_thread_write.hEventToExit);
		::ResetEvent(_thread_event.hEventToExit);

		// 此时串口是关闭的, 收到此事件即准备退出线程
		::SetEvent(_thread_read.hEventToBegin);
		::SetEvent(_thread_write.hEventToBegin);
		::SetEvent(_thread_event.hEventToBegin);
		::SetEvent(_thread_write_tx_event);

		// 等待线程完全退出
		::WaitForSingleObject(_thread_read.hEventToExit, INFINITE);
		//::WaitForSingleObject(_thread_write.hEventToExit, INFINITE);
		::WaitForSingleObject(_thread_event.hEventToExit, INFINITE);

		::CloseHandle(_thread_read.hEventToBegin);
		::CloseHandle(_thread_read.hEventToExit);
		::CloseHandle(_thread_write.hEventToBegin);
		::CloseHandle(_thread_write.hEventToExit);
		::CloseHandle(_thread_event.hEventToBegin);
		::CloseHandle(_thread_event.hEventToExit);

		::CloseHandle(_thread_read.hThread);
		::CloseHandle(_thread_write.hThread);
		::CloseHandle(_thread_event.hThread);
	}

	return false;
}

unsigned int __stdcall ComTaskModel::thread_helper(void* pv)
{
	auto pctx = reinterpret_cast<thread_helper_context*>(pv);
	auto comm = pctx->that;
	auto which = pctx->which;

	delete pctx;

	switch (which)
	{
	case thread_helper_context::e_which::kEvent:
		return comm->thread_event();
	case thread_helper_context::e_which::kRead:
		return comm->thread_read();
	case thread_helper_context::e_which::kWrite:
		return comm->thread_write();
	default:
		return 1;
	}
}

unsigned int ComTaskModel::thread_event()
{
	BOOL bRet;
	DWORD dw, dw2;

_wait_for_work:
	debug_out(("COM-[事件线程] 就绪!\n"));
	dw = ::WaitForSingleObject(_thread_event.hEventToBegin, INFINITE);

	debug_out(("COM-[事件线程] 开始工作...\n"));
	if (!IsDeviceOpen())
	{
		debug_out(("COM-[事件线程] 没有工作, 退出中...\n"));
		::SetEvent(_thread_event.hEventToExit);
		return 0;
	}

	c_overlapped o(true, false);

_wait_again:
	DWORD dwEvent = 0;
	::ResetEvent(o.hEvent);
	dw = ::WaitCommEvent(_hComPort, &dwEvent, &o);
	if (dw != FALSE)
	{
		_event_listener.call_listeners(dwEvent);
		goto _wait_again;
	}
	else
	{ // I/O is pending
		if (::GetLastError() == ERROR_IO_PENDING)
		{
			HANDLE handles[2];
			handles[0] = _thread_event.hEventToExit;
			handles[1] = o.hEvent;

			switch (::WaitForMultipleObjects(_countof(handles), handles, FALSE, INFINITE))
			{
			case WAIT_FAILED:
				//_notifier->msgerr("[事件线程::Wait失败]");
				goto _restart;
				break;
			case WAIT_OBJECT_0 + 0:
				debug_out(("COM-[事件线程] 收到退出事件!\n"));
				goto _restart;
				break;
			case WAIT_OBJECT_0 + 1:
				bRet = ::GetOverlappedResult(_hComPort, &o, &dw2, FALSE);
				if (bRet == FALSE)
				{
					//_notifier->msgerr("[事件线程::Wait失败]");
					goto _restart;
				}
				else
				{
					_event_listener.call_listeners(dwEvent); // uses dwEvent, not dw2
					goto _wait_again;
				}
				break;
			}
		}
		else
		{
			//_notifier->msgerr("[事件线程]::GetLastError() != ERROR_IO_PENDING\n\n");
		}
	}

_restart:
	if (!::CancelIo(_hComPort))
	{

	}

	::WaitForSingleObject(_thread_event.hEventToExit, INFINITE);
	::ResetEvent(_thread_event.hEventToExit);

	goto _wait_for_work;
}

unsigned int ComTaskModel::thread_write()
{
#if 1
	BOOL bRet;
	DWORD dw;

	c_event_event_listener listener;

#define SEND_BUFF_LIMITES 1024
	BYTE * sendBuff = (BYTE *)malloc(sizeof(BYTE)* SEND_BUFF_LIMITES);
	DWORD nWrites = 0;	// 需要发送字节数长度
	DWORD nWritten = 0; // 实际发送字节数长度
	//static DWORD nWrittenData = 0; // 统计一共发送字节数目

_wait_for_work:
	debug_out(("COM-[写线程] 开始工作...\n"));
	debug_out(("COM-[写线程] 就绪\n"));
	dw = ::WaitForSingleObject(_thread_write.hEventToBegin, INFINITE);

	c_overlapped overlap(false, false);
	_event_listener.add_listener(listener, EV_TXEMPTY);

	std::list<std::list<BYTE>>::iterator pack;

_get_packet:
	debug_out(("COM-[写线程] 取数据包中...\n"));
	::WaitForSingleObject(_thread_write_tx_event, INFINITE); // 等待是否接收到发送数据。	

	if (!IsDeviceOpen())
	{
		debug_out(("COM-[写线程] 没有工作, 退出中...\n"));
		::SetEvent(_thread_write.hEventToExit);
		free(sendBuff);
		return 0;
	}
	
	ResetEvent(_thread_write_tx_event);

#if 1
	if (m_SendDataList.size() == 0)
	{
		Sleep(5);
		goto _get_packet;
	}
#endif

	nWrites = 0;
	_snd_lock.lock(); 	
	for (pack = m_SendDataList.begin(); pack != m_SendDataList.end(); )
	{
		if (SEND_BUFF_LIMITES - nWrites < pack->size())
			break;
		for (std::list<BYTE>::iterator it = pack->begin(); it != pack->end(); ++it)
		{
			sendBuff[nWrites++] = *it;
		}
		pack = m_SendDataList.erase(pack);
	}
	_snd_lock.unlock();

	debug_out(("COM-[写线程] 取得一个发送数据包, 长度为 %d 字节\n", nWrites));

	bRet = ::WriteFile(_hComPort, sendBuff, nWrites, NULL, &overlap);
	if (bRet != FALSE)
	{ // I/O is completed
		bRet = ::GetOverlappedResult(_hComPort, &overlap, &nWritten, FALSE);
		if (bRet)
		{
			debug_out(("COM-[写线程] I/O completed immediately, bytes : %d\n", nWritten));
		}
		else
		{
			//_notifier->msgerr("[写线程] GetOverlappedResult失败(I/O completed)!\n");
			goto _restart;
		}
	}
	else
	{ // I/O is pending						
		if (::GetLastError() == ERROR_IO_PENDING)
		{
			HANDLE handles[2];
			handles[0] = _thread_write.hEventToExit;
			handles[1] = listener.hEvent;

			switch (::WaitForMultipleObjects(_countof(handles), &handles[0], FALSE, INFINITE))
			{
			case WAIT_FAILED:
				//_notifier->msgerr("[写线程] Wait失败!\n");
				goto _restart;
				break;
			case WAIT_OBJECT_0 + 0: // now we exit
				debug_out(("COM-[写线程] 收到退出事件!\n"));
				goto _restart;
				break;
			case WAIT_OBJECT_0 + 1: // the I/O operation is now completed
				bRet = ::GetOverlappedResult(_hComPort, &overlap, &nWritten, FALSE);
				if (bRet)
				{
					debug_out(("COM-[写线程] 写入 %d 个字节!\n", nWritten));
				}
				else
				{
					//_notifier->msgerr("[写线程] GetOverlappedResult失败(I/O pending)!\n");
					goto _restart;
				}
				break;
			}
		}
		else
		{
			//_notifier->msgerr("[写线程] ::GetLastError() != ERROR_IO_PENDING");
			goto _restart;
		}
	}
	//nWrittenData += nWritten;
	goto _get_packet;
_restart:
	if (!::CancelIo(_hComPort))
	{

	}

	_event_listener.remove_listener(listener);
	listener.reset();

	// Do just like the thread_read do.
	::WaitForSingleObject(_thread_write.hEventToExit, INFINITE);
	::ResetEvent(_thread_write.hEventToExit);

	goto _wait_for_work;

#endif
	return 0;
}

unsigned int ComTaskModel::thread_read()
{
	BOOL bRet;
	DWORD dw;

	c_event_event_listener listener;

	const int kReadBufSize = 1 << 20;
	unsigned char* block_data = NULL;
	block_data = new unsigned char[kReadBufSize];

_wait_for_work:
	debug_out(("COM-[读线程] 就绪\n"));
	dw = ::WaitForSingleObject(_thread_read.hEventToBegin, INFINITE);

	debug_out(("COM-[读线程] 开始工作...\n"));
	if (!IsDeviceOpen())
	{
		debug_out(("COM-[读线程] 没有工作, 退出中...\n"));
		delete[] block_data;
		::SetEvent(_thread_read.hEventToExit);
		return 0;
	}

	c_overlapped overlap(false, false);

	_event_listener.add_listener(listener, EV_RXCHAR);


	HANDLE handles[3];
	handles[0] = _thread_read.hEventToExit;
	handles[1] = listener.hEvent;
	handles[2] = _thread_read.hEventToBegin;

_get_packet:
	switch (::WaitForMultipleObjects(_countof(handles), handles, FALSE, INFINITE))
	{
	case WAIT_FAILED:
		//_notifier->msgerr("[读线程] Wait失败!\n");
		goto _restart;
	case WAIT_OBJECT_0 + 0:
		debug_out(("COM-[读线程] 收到退出事件!\n"));
		goto _restart;
	case WAIT_OBJECT_0 + 1:
		break;
	case WAIT_OBJECT_0 + 2:
		if (!IsDeviceOpen())
		{
			debug_out(("COM-[读线程] 没有工作, 退出中...\n"));
			delete[] block_data;
			::SetEvent(_thread_read.hEventToExit);
			return 0;
		}
		break;
	}

	DWORD nBytesToRead, nRead, nTotalRead;
	DWORD	comerr;
	COMSTAT	comsta;
	// for some reasons, such as comport has been removed
	if (!::ClearCommError(_hComPort, &comerr, &comsta))
	{
		//_notifier->msgerr("ClearCommError()");
		goto _restart;
	}

	nBytesToRead = comsta.cbInQue;
	if (nBytesToRead == 0)
		nBytesToRead++; // would never happen

	if (nBytesToRead > kReadBufSize)
		nBytesToRead = kReadBufSize;

	for (nTotalRead = 0; nTotalRead < nBytesToRead;)
	{
		bRet = ::ReadFile(_hComPort, block_data + nTotalRead, nBytesToRead - nTotalRead, &nRead, &overlap);
		if (bRet != FALSE)
		{
			bRet = ::GetOverlappedResult(_hComPort, &overlap, &nRead, FALSE);
			if (bRet)
			{
				debug_out(("COM-[读线程] 读取 %d 字节, bRet==TRUE, nBytesToRead: %d\n", nRead, nBytesToRead));
			}
			else
			{
				//_notifier->msgerr("[写线程] GetOverlappedResult失败!\n");
				goto _restart;
			}
		}
		else
		{
			if (::GetLastError() == ERROR_IO_PENDING)
			{
				HANDLE handles[2];
				handles[0] = _thread_read.hEventToExit;
				handles[1] = overlap.hEvent;

				switch (::WaitForMultipleObjects(_countof(handles), &handles[0], FALSE, INFINITE))
				{
				case WAIT_FAILED:
					debug_out(("COM-[读线程] 等待失败!\n"));
					goto _restart;
				case WAIT_OBJECT_0 + 0:
					debug_out(("COM-[读线程] 收到退出事件!\n"));
					goto _restart;
				case WAIT_OBJECT_0 + 1:
					bRet = ::GetOverlappedResult(_hComPort, &overlap, &nRead, FALSE);
					if (bRet)
					{
						debug_out(("COM-[读线程] 读取 %d 字节, bRet==FALSE\n", nRead));
					}
					else
					{
						//_notifier->msgerr("[读线程] GetOverlappedResult失败!\n");
						goto _restart;
					}
					break;
				}
			}
			else
			{
				//_notifier->msgerr("[读线程] ::GetLastError() != ERROR_IO_PENDING");
				goto _restart;
			}
		}

		if (nRead > 0)
		{
			nTotalRead += nRead;
		}
		else
		{
			nBytesToRead--;
		}
	}
	call_data_receivers(block_data, nBytesToRead);
	goto _get_packet;

_restart:
	if (!::CancelIo(_hComPort))
	{

	}

	// Sometimes we got here not because of we've got a exit signal
	// Maybe something wrong
	// And if something wrong, the following handle is still non-signal.
	// The main thread notify this thread to exit by signaling the event and then wait
	// this thread Reset it, since the event is a Manual reset event handle.
	// So, let's wait whatever the current signal-state the event is, just before the
	// main thread  really want we do that.
	::WaitForSingleObject(_thread_read.hEventToExit, INFINITE);
	::ResetEvent(_thread_read.hEventToExit);

	goto _wait_for_work;
}
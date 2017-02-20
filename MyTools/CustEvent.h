#ifndef __CUST_EVENT__
#define __CUST_EVENT__
#include <vector>
#include <windef.h>
#include <WinBase.h>

#include "CustUtils.h"
// 串口事件监听器接口
class i_com_event_listener
{
public:
	virtual void do_event(DWORD evt) = 0;
};

// do_event的过程一定不要长时间不返回, 所以写了个基于事件的监听器
class c_event_event_listener : public i_com_event_listener
{
public:
	operator i_com_event_listener*()
	{
		return this;
	}
	virtual void do_event(DWORD evt) override
	{
		event = evt;
		::SetEvent(hEvent);
	}

public:
	c_event_event_listener()
	{
		hEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}
	~c_event_event_listener()
	{
		::CloseHandle(hEvent);
	}

	void reset()
	{
		::ResetEvent(hEvent);
	}


public:
	DWORD	event;
	HANDLE	hEvent;
};

// 串口事件监听器接口 管理器
class c_com_event_listener
{
	struct item
	{
		item(i_com_event_listener* p, DWORD _mask)
		: listener(p)
		, mask(_mask)
		{
		}

		i_com_event_listener* listener;
		DWORD mask;
	};
public:
	void call_listeners(DWORD dwEvt)
	{
		_lock.lock();
		for (auto& item : _listeners)
		{
			if (dwEvt & item.mask)
			{
				item.listener->do_event(dwEvt);
			}
		}
		_lock.unlock();
	}
	void add_listener(i_com_event_listener* pcel, DWORD mask)
	{
		_lock.lock();
		_listeners.push_back(item(pcel, mask));
		_lock.unlock();
	}
	void remove_listener(i_com_event_listener* pcel)
	{
		_lock.lock();
		for (auto it = _listeners.begin(); it != _listeners.end(); it++)
		{
			if (it->listener == pcel)
			{
				_listeners.erase(it);
				break;
			}
		}
		_lock.unlock();
	}

protected:
	c_critical_locker	_lock;
	std::vector<item>	_listeners;
};


struct thread_state
{
	HANDLE hThread;
	HANDLE hEventToBegin;
	HANDLE hEventToExit;
};

#endif
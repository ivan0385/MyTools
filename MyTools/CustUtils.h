#pragma once
#include <WinBase.h>
#include <functional>
template<class T>

#ifdef _DEBUG
#else
#define assert(msg)
#endif
class c_ptr_array
{
public:
	c_ptr_array()
		: m_pp(0)
		, m_cnt(0)
		, m_allocated(0)
	{

	}
	virtual ~c_ptr_array()
	{
		empty();
	}

public:
	void empty()
	{
		if (m_pp)
		{
			::free(m_pp);
		}
		m_pp = 0;
		m_cnt = 0;
		m_allocated = 0;
	}

	int find(T* pv) const
	{
		for (int i = 0; i<m_cnt; i++)
		{
			if (m_pp[i] == pv)
			{
				return i;
			}
		}
		return -1;
	}

	bool add(T* pv)
	{
		assert(find(pv) == -1);
		if (++m_cnt > m_allocated)
		{
			int n = m_allocated * 2;
			if (!n) n = 1;

			T** pp = static_cast<T**>(::realloc(m_pp, n * sizeof(void*)));
			if (pp)
			{
				m_allocated = n;
				m_pp = pp;
			}
			else
			{
				--m_cnt;
				return false;
			}
		}
		m_pp[m_cnt - 1] = pv;
		return true;
	}

	bool remove(T* pv)
	{
		int i = find(pv);
		if (i == -1)
		{
			assert(0);
			return false;
		}
		else
		{
			--m_cnt;
			::memmove(m_pp + i, m_pp + i + 1, (m_cnt - i)*sizeof(void*));
			return true;
		}
	}

	int size() const
	{
		return m_cnt;
	}

	T* getat(int i) const
	{
		assert(i >= 0 && i<m_cnt);
		return m_pp[i];
	}

	T* operator[](int i) const
	{
		return getat(i);
	}

protected:
	T** m_pp;
	int m_cnt;
	int m_allocated;
};

template <int pre_size, int granularity>
class c_byte_array
{
public:
	c_byte_array()
		: _data(0)
		, _pos(0)
	{
		assert(pre_size >= 0);
		assert(granularity > 0);
		_size = pre_size;
		_granularity = granularity;

		if (_size > 0)
		{
			_data = new unsigned char[_size];
		}
	}
	~c_byte_array()
	{
		empty();
	}

	void empty()
	{
		if (_data)
		{
			delete[] _data;
			_data = NULL;
		}
		_pos = 0;
		_size = 0;
	}

	void* get_data() const
	{
		return _data;
	}

	int get_size() const
	{
		return _pos;
	}

	void append(const unsigned char* ba, int cb)
	{
		if (cb > get_space_left())
			inc_data_space(cb);
		memcpy(_data + get_space_used(), ba, cb);
		_pos += cb;
	}

	void append_char(const unsigned char c)
	{
		return append(&c, 1);
	}

protected:
	int get_space_left()
	{
		return _size - 1 - _pos + 1;
	}

	int get_space_used()
	{
		return _pos;
	}

	bool inc_data_space(int addition)
	{
		int left = addition - get_space_left();
		int n_blocks = left / _granularity;
		int n_remain = left - n_blocks * _granularity;

		if (n_remain) ++n_blocks;

		int new_size = _size + n_blocks * _granularity;
		unsigned char* p = new unsigned char[new_size];

		memcpy(p, _data, get_space_used());

		if (_data) delete[] _data;
		_data = p;
		_size = new_size;

		return true;
	}

protected:
	unsigned char* _data;
	int _size;
	int _pos;
	int _granularity;
};

// 观察者模式
class i_observer
{
public:
	virtual bool do_event() = 0;
};

class i_observable
{
public:
	virtual operator i_observable*() = 0;
	virtual void empty() = 0;
	virtual int find(i_observer* ob) = 0;
	virtual bool add(i_observer* ob) = 0;
	virtual bool add(std::function<bool()> doevt) = 0;
	virtual bool remove(i_observer* ob) = 0;
	virtual int size() = 0;
	virtual i_observer* getat(int i) const = 0;
	virtual i_observer* operator[](int i) const = 0;
};

class c_observable : public i_observable
{
public:
	virtual operator i_observable*()
	{
		return static_cast<i_observable*>(this);
	}
	virtual void empty()
	{
		_obs.empty();
	}
	virtual int find(i_observer* ob)
	{
		return _obs.find(ob);
	}
	virtual bool add(i_observer* ob)
	{
		return _obs.add(ob);
	}
	virtual bool add(std::function<bool()> doevt)
	{
		_fobs.push_back(doevt);
		return true;
	}
	virtual bool remove(i_observer* ob)
	{
		return _obs.remove(ob);
	}
	virtual int size()
	{
		return _obs.size();
	}
	virtual i_observer* getat(int i) const
	{
		return _obs.getat(i);
	}
	virtual i_observer* operator[](int i) const
	{
		return _obs[i];
	}

public:
	bool call_observers()
	{
		for (int i = 0; i < size(); i++)
		{
			if (getat(i)->do_event())
			{
				return false;
			}
		}
		for (auto& ob : _fobs)
		{
			if (ob())
			{
				return false;
			}
		}
		return true;
	}

protected:
	c_ptr_array<i_observer> _obs;
	std::vector<std::function<bool()>> _fobs;
};

class c_critical_locker
{
public:
	c_critical_locker()
	{
		::InitializeCriticalSection(&_cs);
	}
	~c_critical_locker()
	{
		::DeleteCriticalSection(&_cs);
	}

	void lock()
	{
		::EnterCriticalSection(&_cs);
	}

	void unlock()
	{
		::LeaveCriticalSection(&_cs);
	}

	bool try_lock()
	{
		return !!::TryEnterCriticalSection(&_cs);
	}

private:
	CRITICAL_SECTION _cs;
};


#define __ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#ifndef _SINGLETON_H__
#define _SINGLETON_H__


template <class T>
class MySingleton
{
public:
	static T* Get_Instance()
	{
		if (ms_pSingleton == NULL)
			ms_pSingleton = new T();
		return ms_pSingleton;
	}

	static void Destroy_Instance()
	{
		if (ms_pSingleton != NULL)
			delete ms_pSingleton;

		ms_pSingleton = nullptr;
	}

private:
	MySingleton() {};
	virtual ~MySingleton() {};

private:
	static T* ms_pSingleton;
};

template <class T>
T* MySingleton<T>::ms_pSingleton = NULL;

#define MY_SINGLETON_CLASS(T) friend class MySingleton<T>

#endif //_SINGLETON_H__

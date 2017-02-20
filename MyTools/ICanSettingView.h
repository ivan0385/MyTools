#pragma once
#include "CanBus.h"
class ICanSettingView
{
public:

	ICanSettingView()
	{
	}

	virtual ~ICanSettingView()
	{
	}

	virtual CanBus GetCanBus() = 0;
	virtual bool SetCanBus(CanBus info) = 0;
	virtual bool CanDeivceIsOpened() = 0;
	virtual bool CanDeivceIsClosed() = 0;
	virtual bool CanDeivceIsError() = 0;

};


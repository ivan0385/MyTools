#pragma once
class ICanButtonsSetupView
{
public:

	ICanButtonsSetupView()
	{
	}

	virtual ~ICanButtonsSetupView()
	{
	}

	virtual void SetCaptionsData(const CString * data) = 0;
};


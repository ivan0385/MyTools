#pragma once

#include <windef.h>
#include "ControlCAN.h"

class i_data_receiver
{
public:
	// 数据接收函数, 读线程接收到数据时调用此函数
	// ba指向的内容不应该被更改!
	virtual void receive(const BYTE* ba, int size)=0;
};


class i_data_receiver_can
{
public:
	// 数据接收函数, 读线程接收到数据时调用此函数
	// ba指向的内容不应该被更改!
	virtual void receive(const VCI_CAN_OBJ* ba, int size) = 0;
	virtual void send(const VCI_CAN_OBJ * ba, int size) = 0;
};

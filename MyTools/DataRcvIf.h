#pragma once

#include <windef.h>
#include "ControlCAN.h"

class i_data_receiver
{
public:
	// ���ݽ��պ���, ���߳̽��յ�����ʱ���ô˺���
	// baָ������ݲ�Ӧ�ñ�����!
	virtual void receive(const BYTE* ba, int size)=0;
};


class i_data_receiver_can
{
public:
	// ���ݽ��պ���, ���߳̽��յ�����ʱ���ô˺���
	// baָ������ݲ�Ӧ�ñ�����!
	virtual void receive(const VCI_CAN_OBJ* ba, int size) = 0;
	virtual void send(const VCI_CAN_OBJ * ba, int size) = 0;
};

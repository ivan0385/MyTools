#pragma once
#include <vector>
#include <map>

typedef struct
{
	bool enable;
	int tmInterval; //ms
	int id;
	//char data[8];
	std::vector<BYTE> data;
}ST_PACKET;

typedef struct
{
	std::string sgnlName;
	unsigned int sgnStartBit;
	unsigned int sgnBitSize;
	unsigned int sgnByteOrder;	// (0=little endian, 1=big endian)
	std::string sgnValueType;	// (+=unsigned, -=signed)
	double sgnFator;
	double sgnOffset;			// physical_value = raw_value * factor + offset
								// raw_value = (physical_value – offset) / factor
	unsigned int sgnMin;		// The minimum and maximum define the range of valid physical values of the signal.
	unsigned int sgnMax;
	std::string sgnUnit;
	std::string sgnReceiver;

	// 属性
	unsigned int sgnSendType;//such as:"Cyclic","OnWrite","OnWriteWithRepetition","OnChange","OnChangeWithRepetition","IfActive","IfActiveWithRepetition","NoSigSendType";

	// add by user
	unsigned int sgnDefValue;
	unsigned int sgnUserValue;
}CAN_SIGNAL;

typedef struct
{
	unsigned int msgId;	//如果最高bit为1，则为扩展帧ID， 否则为标准帧。
	std::string msgName;
	unsigned int msgLength;//bytes
	std::string msgTranmistter;
	std::vector<CAN_SIGNAL> signalList;	//'SG_' signal_name  
										//':' start_bit '|'	signal_size '@' byte_order value_type '(' factor ',' offset ')'
										//'[' minimum '|' maximum ']' unit receiver{ ',' receiver }
	// 属性
	unsigned int msgSendType; // "Cyclic","not_used","not_used","not_used","not_used","not_used","not_used","IfActive","NoMsgSendType";
	unsigned int msgCycleTime; // ms

	// add by user
	bool msgIsEnable;
}CAN_MSG;

typedef struct
{
	std::string name; // project name
	std::string nodeName; // BU_: {node name}
	std::map < std::string, std::vector<std::string>> defAtrList; // User Defined Attribute Definitions
	std::vector<CAN_MSG> msgList; // BO_ message_id message_name : 报文字节数 transmitter { signal }
}CAN_MATRIX;


typedef struct
{
	CAN_MSG * msg;
	std::vector<BYTE> msgData;
}CAN_MSG_DATA;




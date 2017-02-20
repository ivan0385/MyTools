#pragma once
#include "CanMatrixDef.h"
#include "MyString.h"
#include "MyUtility.h"
#include "AccessDbDriver.h"
class CanDbc
{
public:

	CanDbc()
		: m_dbAddr(_T(""))
	{
		m_CanMatrix.defAtrList.clear();
		m_CanMatrix.msgList.clear();

		m_dbAddr = CMyUtility::GetModuleFolder() + L"MyCanDb.accdb";
	}

	~CanDbc()
	{
	}
private:
	CAN_MATRIX m_CanMatrix;

	void _empty_msg(CAN_MSG *rdMsg)
	{
		rdMsg->msgId = 0;
		rdMsg->msgName = "";
		rdMsg->msgLength = 0;
		rdMsg->msgTranmistter = "";
		rdMsg->msgSendType = 0;
		rdMsg->msgIsEnable = false;
		rdMsg->msgCycleTime = 0;
		rdMsg->signalList.clear();
	}

	void _empty_sig(CAN_SIGNAL *rdSgn)
	{		
		rdSgn->sgnlName="";
		rdSgn->sgnStartBit=0;
		rdSgn->sgnBitSize=0;
		rdSgn->sgnByteOrder=0;	// (0=little endian, 1=big endian)
		rdSgn->sgnValueType = "+";	// (+=unsigned, -=signed)
		rdSgn->sgnFator=0.0;
		rdSgn->sgnOffset=0.0;			// physical_value = raw_value * factor + offset
		// raw_value = (physical_value – offset) / factor
		rdSgn->sgnMin = 0;		// The minimum and maximum define the range of valid physical values of the signal.
		rdSgn->sgnMax = 0;
		rdSgn->sgnUnit = "";
		rdSgn->sgnReceiver = "";

		// 属性
		rdSgn->sgnSendType = 0;//such as:"Cyclic","OnWrite","OnWriteWithRepetition","OnChange","OnChangeWithRepetition","IfActive","IfActiveWithRepetition","NoSigSendType";

		// add by user
		rdSgn->sgnDefValue = 0;
		rdSgn->sgnUserValue = 0;
	}

	void _parse_bu(std::string line)
	{
		m_CanMatrix.nodeName = CMyString::trim(line);
	}

	void _parse_bo(std::string line, CAN_MSG *rdMsg)
	{
		std::vector<std::string> seps;
		std::string tag;
		seps.push_back(" ");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdMsg->msgId = CMyString::stringToNum<unsigned int>(CMyString::trim(tag));

		seps.clear();
		seps.push_back(":");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdMsg->msgName = CMyString::trim(tag);

		line = CMyString::trim(line);
		seps.clear();
		seps.push_back(" ");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdMsg->msgLength = CMyString::stringToNum<unsigned int>(CMyString::trim(tag));

		rdMsg->msgTranmistter = line;
	}

	void _parse_sg(std::string line, CAN_MSG *rdMsg)
	{
		CAN_SIGNAL rdSgn;
		_empty_sig(&rdSgn);
		std::vector<std::string> seps;
		std::string tag;
		seps.push_back(":");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnlName = CMyString::trim(tag);

		seps.clear();
		seps.push_back("|");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnStartBit = CMyString::stringToNum<unsigned int>(CMyString::trim(tag));

		seps.clear();
		seps.push_back("@");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnBitSize = CMyString::stringToNum<unsigned int>(CMyString::trim(tag));

		seps.clear();
		seps.push_back("(");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnByteOrder = tag[0] == '0' ? 0 : 1;
		rdSgn.sgnValueType = tag[1] == '+' ? "+" : "-";

		seps.clear();
		seps.push_back(",");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnFator = CMyString::stringToNum<double>(CMyString::trim(tag));

		seps.clear();
		seps.push_back(")");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnOffset = CMyString::stringToNum<double>(CMyString::trim(tag));

		seps.clear();
		seps.push_back("[");
		if (!CMyString::split_first_of(line, seps, tag))
			return;

		seps.clear();
		seps.push_back("|");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnMin = CMyString::stringToNum<unsigned int>(CMyString::trim(tag));

		seps.clear();
		seps.push_back("]");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnMax = CMyString::stringToNum<unsigned int>(CMyString::trim(tag));

		seps.clear();
		seps.push_back(" ");
		if (!CMyString::split_first_of(line, seps, tag))
			return;
		rdSgn.sgnReceiver = CMyString::trim(line);

		rdMsg->signalList.push_back(rdSgn);
	}

	void _parse_ba_def(std::string line)
	{
		std::string backup(line);
		std::vector<std::string> seps;
		std::string tag;
		seps.push_back("BO_  \"GenMsgSendType\" ENUM");
		if (CMyString::split_first_of(line, seps, tag))
		{
			std::vector<std::string>list;
			std::string sep(",");
			CMyString::split(line, sep, list);
			m_CanMatrix.defAtrList.insert(std::make_pair("BO_GenMsgSendType", list));
		}
		else
		{
			line = backup;
			seps.clear();
			seps.push_back("SG_  \"GenSigSendType\" ENUM");
			if (!CMyString::split_first_of(line, seps, tag))
				return;
			std::vector<std::string>list;
			std::string sep(",");
			CMyString::split(line, sep, list);
			m_CanMatrix.defAtrList.insert(std::make_pair("SG_GenSigSendType", list));
		}
		
	}

	void _parse_ba_def_def(std::string line)
	{
		std::string tag;
		if (!CMyString::split_first_of(line, " ", tag))
			return;
		if (tag.find("\"GenMsgCycleTime\"") != std::string::npos)
		{
			if (!CMyString::split_first_of(line, ";", tag))
				return;
			int value = CMyString::stringToNum<int>(CMyString::trim(tag));

			for (unsigned int i = 0; i < m_CanMatrix.msgList.size(); i++)
			{
				m_CanMatrix.msgList[i].msgCycleTime = value;
			}
		}
	}

	void _parse_ba(std::string line)
	{
		std::string tag;
		if (!CMyString::split_first_of(line, " ", tag))
			return;
		if (tag.find("\"GenMsgCycleTime\"") != std::string::npos)
		{
			if (!CMyString::split_first_of(line, " ", tag))
				return;
			if (!CMyString::split_first_of(line, " ", tag))
				return;
			int msgid = CMyString::stringToNum<int>(CMyString::trim(tag));
			if (!CMyString::split_first_of(line, ";", tag))
				return;
			int value = CMyString::stringToNum<int>(CMyString::trim(tag));

			for (unsigned int i = 0; i < m_CanMatrix.msgList.size(); i++)
			{
				if (m_CanMatrix.msgList[i].msgId == msgid)
				{
					m_CanMatrix.msgList[i].msgCycleTime = value;
				}
			}
		}
		else if (tag.find("\"GenMsgSendType\"") != std::string::npos)
		{
			if (!CMyString::split_first_of(line, " ", tag))
				return;
			if (!CMyString::split_first_of(line, " ", tag))
				return;
			int msgid = CMyString::stringToNum<int>(CMyString::trim(tag));
			if (!CMyString::split_first_of(line, ";", tag))
				return;
			int value = CMyString::stringToNum<int>(CMyString::trim(tag));

			for (unsigned int i = 0; i < m_CanMatrix.msgList.size(); i++)
			{
				if (m_CanMatrix.msgList[i].msgId == msgid)
				{
					m_CanMatrix.msgList[i].msgSendType = value;
				}
			}
		}
		else if (tag.find("\"GenSigSendType\"") != std::string::npos)
		{
			if (!CMyString::split_first_of(line, " ", tag))
				return;
			if (!CMyString::split_first_of(line, " ", tag))
				return;
			int msgid = CMyString::stringToNum<int>(CMyString::trim(tag));
			if (!CMyString::split_first_of(line, " ", tag))
				return;
			std::string sigName = CMyString::trim(tag);
			if (!CMyString::split_first_of(line, ";", tag))
				return;
			int value = CMyString::stringToNum<int>(CMyString::trim(tag));

			for (unsigned int i = 0; i < m_CanMatrix.msgList.size(); i++)
			{
				if (m_CanMatrix.msgList[i].msgId == msgid)
				{
					for (unsigned int j = 0; j < m_CanMatrix.msgList[i].signalList.size(); j++)
					{
						if (m_CanMatrix.msgList[i].signalList[j].sgnlName == sigName)
						{
							m_CanMatrix.msgList[i].signalList[j].sgnSendType = value;
						}
					}
				}
			}
		}		
	}


	unsigned long long _updateBits(unsigned long long src, unsigned long long value, int end, int start)
	{
		unsigned long long allOnes = ~0;
		unsigned long long left = allOnes << (end + 1);
		unsigned long long right = ((unsigned long long)1 << start) - 1;
		unsigned long long mask = left | right;
		unsigned long long n_cleared = src & mask;
		unsigned long long m_shifted = value << start;
		return n_cleared | m_shifted;
	}

	unsigned long long _getBitsValue(unsigned long long src, int end, int start)
	{
		unsigned long long allOnes = ~0;
		unsigned long long left = allOnes << (end + 1);
		unsigned long long right = ((unsigned long long)1 << start) - 1;
		unsigned long long mask = left | right;
		mask = ~mask;
		unsigned long long n_cleared = src & mask;
		return n_cleared>>start;
	}


	void _setSgnValue(CAN_MSG & msg, std::string sgnName, int value)
	{
		std::vector<CAN_SIGNAL>::iterator d_itr;
		for (d_itr = msg.signalList.begin(); d_itr != msg.signalList.end(); d_itr++)
		{
			if (d_itr->sgnlName==sgnName)
			{
				if (abs(d_itr->sgnFator) > 1e-15)
					d_itr->sgnUserValue =(unsigned int) (value / d_itr->sgnFator);
				else
					d_itr->sgnUserValue = value;
			}
		}
	}

	void _ResetSgnUserValue(CAN_MSG & msg)
	{
		std::vector<CAN_SIGNAL>::iterator d_itr;
		for (d_itr = msg.signalList.begin(); d_itr != msg.signalList.end(); d_itr++)
		{
			if (d_itr->sgnSendType == 4)
			{
				d_itr->sgnUserValue = d_itr->sgnDefValue;
			}
		}
	}

	void _buildMsgData(CAN_MSG & msg, std::vector<BYTE> & buff)
	{
		int bitStart = 0, bitSize = 0;
		unsigned long long value = 0;
		unsigned long long dest = 0;
		unsigned int size = buff.size();
		std::vector<CAN_SIGNAL>::iterator d_itr;
		for (d_itr = msg.signalList.begin(); d_itr != msg.signalList.end(); d_itr++)
		{
			bitStart = d_itr->sgnStartBit;
			bitSize = d_itr->sgnBitSize;
			value = d_itr->sgnUserValue;


			if (d_itr->sgnByteOrder == 0)
			{
				static unsigned char m[64] = { 56,57,58,59,60,61,62,63,48,49,50,51,52,53,54,55,40,41,42,43,44,45,46,47
					,32,33,34,35,36,37,38,39,24,25,26,27,28,29,30,31,16,17,18,19,20,21,22,23,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};

				dest |= _updateBits(dest, value, m[bitStart], m[bitStart]-bitSize+1);
			}
		}
		buff.clear();
		for (unsigned int i = 0; i < size; i++)
		{
			buff.push_back((unsigned char)(dest >> ((8-i-1)*8)));
		}
	}

	void _updateMsgData(CAN_MSG & msg, unsigned long long value)
	{
		int bitStart = 0, bitSize = 0;
		unsigned long long dest = 0;
		std::vector<CAN_SIGNAL>::iterator d_itr;
		for (d_itr = msg.signalList.begin(); d_itr != msg.signalList.end(); d_itr++)
		{
			bitStart = d_itr->sgnStartBit;
			bitSize = d_itr->sgnBitSize;

			if (d_itr->sgnByteOrder == 0)
			{
				static unsigned char m[64] = { 56, 57, 58, 59, 60, 61, 62, 63, 48, 49, 50, 51, 52, 53, 54, 55, 40, 41, 42, 43, 44, 45, 46, 47
					, 32, 33, 34, 35, 36, 37, 38, 39, 24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7 };

				d_itr->sgnUserValue = (unsigned int)_getBitsValue(value, m[bitStart], m[bitStart] - bitSize + 1);
			}
		}
	}

public:
	bool ParseDbcFile(CString filePath)
	{
		std::vector<BYTE> file_data;
		bool ret = CMyUtility::FileRead(filePath, file_data);
		if (ret)
		{
			m_CanMatrix.defAtrList.clear();
			m_CanMatrix.name = "";
			m_CanMatrix.nodeName = "";
			m_CanMatrix.msgList.clear();

			std::vector<BYTE>::iterator itr;
			std::string src;
			for (itr = file_data.begin(); itr != file_data.end(); itr++)
				src += *itr;

			std::vector<std::string> dest;
			CMyString::split(src, "\r\n", dest);

			std::vector<std::string>::iterator d_itr;
			std::string tag;
			std::vector<std::string> seps;
			seps.push_back(":");
			seps.push_back(" ");
			CAN_MSG *msg=NULL;
			for (d_itr = dest.begin(); d_itr != dest.end(); d_itr++)
			{
				if (CMyString::split_first_of(*d_itr, seps, tag))
				{
					if (tag == "BU_")
					{
						_parse_bu(*d_itr);
					}
					else if (tag == "BO_")
					{
						CAN_MSG rdMsg;
						_empty_msg(&rdMsg);
						_parse_bo(*d_itr, &rdMsg);
						m_CanMatrix.msgList.push_back(rdMsg);
						for (unsigned int i = 0; i < m_CanMatrix.msgList.size(); i++)
						{
							if (m_CanMatrix.msgList[i].msgId == rdMsg.msgId)
							{
								msg = &(m_CanMatrix.msgList[i]);
								break;
							}
						}
					}
					else if (tag == "SG_")
					{
						_parse_sg(*d_itr, msg);
					}
					else if (tag == "BA_DEF_")
					{
						_parse_ba_def(*d_itr);
					}
					else if (tag == "BA_DEF_DEF_")
					{
						_parse_ba_def_def(*d_itr);
					}
					else if (tag == "BA_")
					{
						_parse_ba(*d_itr);
					}
				}
			}

		} 

		CAN_MSG_DATA data;
		SetSignalValue(0x180, "BCM_IgnKeyPOS", 2, data);
		return ret;
	}
	
	bool GetSignalValue(unsigned int msgId, std::string sigName,unsigned int &value)
	{
		CAN_MSG *msg = NULL;
		for (unsigned int i = 0; i < m_CanMatrix.msgList.size(); i++)
		{
			if (m_CanMatrix.msgList[i].msgId == msgId)
			{
				msg = &(m_CanMatrix.msgList[i]);
				break;
			}
		}
		if (msg != NULL)
		{
			for (unsigned int i = 0; i < msg->signalList.size(); i++)
			{
				if (msg->signalList[i].sgnlName == sigName)
				{
					value = msg->signalList[i].sgnUserValue;
					return true;
				}
			}
		}
		return false;
	}

	bool SetSignalValue(unsigned int msgId, std::string sigName, int sigValue, CAN_MSG_DATA & output)
	{
		CAN_MSG *msg = NULL;
		for (unsigned int i = 0; i < m_CanMatrix.msgList.size(); i++)
		{
			if (m_CanMatrix.msgList[i].msgId == msgId)
			{
				msg = &(m_CanMatrix.msgList[i]);
				break;
			}
		}
		if (msg != NULL)
		{
			_setSgnValue(*msg, sigName, sigValue);
			output.msgData.resize(msg->msgLength);
			_buildMsgData(*msg, output.msgData);
			_ResetSgnUserValue(*msg);
			output.msg = msg;
			return true;
		}
		return false;
	}

	void UpdateMsgByRcv(unsigned int msgId, const unsigned char *data, unsigned int size)
	{
		CAN_MSG *msg = NULL;
		unsigned long long value = 0;
		for (unsigned int i = 0; i < m_CanMatrix.msgList.size(); i++)
		{
			if (m_CanMatrix.msgList[i].msgId == msgId)
			{
				msg = &(m_CanMatrix.msgList[i]);
				break;
			}
		}
		if (msg == NULL)
			return;
		for (unsigned int i = 0; i < size; i++)
		{
			value |= ((unsigned long long)data[i] << ((size - 1 - i) * 8));
		}
		_updateMsgData(*msg, value);
	}

	CAN_MATRIX & GetCanMatrix()
	{
		return  m_CanMatrix;
	}

	bool _DbInsertMatrix(AccessDbDriver &adb,CAN_MATRIX & matrix)
	{
		CString sql = L"insert into CAN_MATRIX(name,nodeName) values('";
		sql.Append(CString(matrix.name.c_str()));
		sql.Append(L"','");
		sql.Append(CString(matrix.nodeName.c_str()));
		sql.Append(L"')");
		return adb.Insertdb(sql,L"CAN_MATRIX",L"name",CString(matrix.name.c_str()));
	}

	int _DbFindMatrixIdByName(AccessDbDriver &adb, std::string name)
	{
		CString sql = L"select ID from CAN_MATRIX where name like '";
		sql += CString(name.c_str()) + L"'";

		std::vector<std::vector<CString> > rec;
		if (adb.ReadDb(sql, rec))
		{
			if (rec.size() > 0)
				return _ttoi(rec[0][0]);
		}

		return -1;
	}

	bool _SetActiveCanMatrix(AccessDbDriver &adb, CString name)
	{
		CString sql;
		sql.Format(L"delete from ACTIVE_CAN_MATRIX");
		adb.ExcuteSql(sql);
		sql.Format(L"insert into ACTIVE_CAN_MATRIX(activeName) values('%s')",name);
		return adb.Insertdb(sql, L"ACTIVE_CAN_MATRIX", L"activeName", name);
	}

	bool _DbInsertMsg(AccessDbDriver &adb, CAN_MSG & msg, int matrixID)
	{
		CString tmp;
		CString sql = L"insert into CAN_MSG(msgName,msgId,msgLength,msgTranmistter,msgSendType,msgCycleTime,msgIsEnable,matrixID) values('";
		
		sql.Append(CString(msg.msgName.c_str()));
		sql.Append(L"',");

		tmp.Format(L"%d", msg.msgId);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", msg.msgLength);
		sql.Append(tmp);
		sql.Append(L",'");

		sql.Append(CString(msg.msgTranmistter.c_str()));
		sql.Append(L"',");

		tmp.Format(L"%d", msg.msgSendType);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", msg.msgCycleTime);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", msg.msgIsEnable);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", matrixID);
		sql.Append(tmp);
		sql.Append(L")");
		return adb.Insertdb(sql, L"CAN_MSG", L"msgId", msg.msgId, L"matrixID", matrixID);
	}

	int _DbFindMsgIdByName(AccessDbDriver &adb, std::string msgName, int matrixid)
	{
		CString sql;
		sql.Format(L"select ID from CAN_MSG where msgName like '%s' and matrixID = %d", CString(msgName.c_str()),matrixid);

		std::vector<std::vector<CString>> rec;
		if (adb.ReadDb(sql, rec))
		{
			if (rec.size() > 0)
				return _ttoi(rec[0][0]);
		}

		return -1;
	}

	bool _DbInsertSig(AccessDbDriver &adb,CAN_SIGNAL & sig, unsigned int msgid)
	{
		CString tmp;
		CString sql = L"insert into CAN_SIGNAL(sgnName,sgnStartBit,sgnBitSize,sgnByteOrder,sgnValueType,sgnFator,sgnOffset,sgnMin,sgnMax,msgId,";
			
		sql += "sgnUnit,sgnReceiver,sgnSendType,sgnDefValue,sgnUserValue) values('";

		sql.Append(CString(sig.sgnlName.c_str()));
		sql.Append(L"',");

		tmp.Format(L"%d", sig.sgnStartBit);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", sig.sgnBitSize);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", sig.sgnByteOrder);
		sql.Append(tmp);
		sql.Append(L",'");

		sql.Append(CString(sig.sgnValueType.c_str()));
		sql.Append(L"',");

		tmp.Format(L"%f", sig.sgnFator);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%f", sig.sgnOffset);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", sig.sgnMin);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", sig.sgnMax);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", msgid);
		sql.Append(tmp);
		sql.Append(L",'");
		
		sql.Append(CString(sig.sgnUnit.c_str()));
		sql.Append(L"','");

		sql.Append(CString(sig.sgnReceiver.c_str()));
		sql.Append(L"',");
		
		tmp.Format(L"%d", sig.sgnSendType);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", sig.sgnDefValue);
		sql.Append(tmp);
		sql.Append(L",");

		tmp.Format(L"%d", sig.sgnUserValue);
		sql.Append(tmp);

		sql.Append(L")");
		return adb.Insertdb(sql, L"CAN_SIGNAL", L"sgnName", CString(sig.sgnlName.c_str()), L"msgId", msgid);
	}
	bool SaveMatrixToDb()
	{
		AccessDbDriver adb;
		adb.OpenDb(m_dbAddr);

		_SetActiveCanMatrix(adb, CMyString::toCString(m_CanMatrix.name.c_str()));
		// Can_Matrix 
		if (!_DbInsertMatrix(adb, m_CanMatrix))
		{
			AfxMessageBox(L"写入CAN_MATRIX 失败！");
			return false;
		}
		int matrixId = _DbFindMatrixIdByName(adb,m_CanMatrix.name);
		int msgid;
		std::vector<CAN_MSG> msgList = m_CanMatrix.msgList;
		for (unsigned int i = 0; i < msgList.size(); i++)
		{
			if(!_DbInsertMsg(adb, msgList[i], matrixId)) // CAN_MSG
			{
				AfxMessageBox(L"写入CAN_MSG 失败！");
				return false;
			}
			msgid = _DbFindMsgIdByName(adb, msgList[i].msgName,matrixId);
			if (msgid < 0)
				break;
			for (unsigned int j = 0; j < msgList[i].signalList.size(); j++)
			{
				if(!_DbInsertSig(adb, msgList[i].signalList[j], msgid))//CAN_SIGNAL
				{
					AfxMessageBox(L"写入CAN_SIGNAL 失败！");
					return false;
				}
			}
		}

		adb.CloseDb();

		AfxMessageBox(L"写入数据库成功！");
		return true;
	}


	bool GetSaveCanMatrixFromDb()
	{
		bool ret = false;
		AccessDbDriver adb;
		adb.OpenDb(m_dbAddr);

		std::vector<std::vector<CString > > readlist;
		std::vector<std::vector<CString > > siglist;
		int id = 0;
		CString sql;
		sql.Format(L"select activeName from ACTIVE_CAN_MATRIX");
		adb.ReadDb(sql, readlist);
		if (readlist.size() < 1)
			ret = false;
		else
		{
			m_CanMatrix.name = CMyString::toString(readlist[0][0]);
			sql.Format(L"select ID,nodeName from CAN_MATRIX where name like '%s'", readlist[0][0]);
			///AfxMessageBox(sql);
			adb.ReadDb(sql,readlist);
			if (readlist.size()>0)
			{
				m_CanMatrix.nodeName = CMyString::toString(readlist[0][1]);
				id = CMyString::toInt(readlist[0][0]);
				m_CanMatrix.msgList.clear();			

				sql.Format(L"select ID,msgName,msgId,msgLength,msgTranmistter,msgSendType,msgCycleTime,msgIsEnable from CAN_MSG where matrixId=%d", id);
				//AfxMessageBox(sql);
				adb.ReadDb(sql, readlist);

				int col = 0;
				int sigcol = 0;
				for (unsigned int i = 0; i < readlist.size(); i++)
				{
					CAN_MSG canMsg;
					_empty_msg(&canMsg);

					col = 0;

					id = CMyString::toInt(readlist[i][col++]);
					canMsg.msgName = CMyString::toString(readlist[i][col++]);
					canMsg.msgId = CMyString::toInt(readlist[i][col++]);
					canMsg.msgLength = CMyString::toInt(readlist[i][col++]);
					canMsg.msgTranmistter = CMyString::toString(readlist[i][col++]);
					canMsg.msgSendType = CMyString::toInt(readlist[i][col++]);
					canMsg.msgCycleTime = CMyString::toInt(readlist[i][col++]);
					canMsg.msgIsEnable = !!(CMyString::toInt(readlist[i][col++]));

					sql.Format(L"select sgnName,sgnStartBit,sgnBitSize,sgnByteOrder,sgnValueType,sgnFator,sgnOffset,sgnMin,sgnMax,sgnUnit,sgnReceiver,sgnSendType,sgnDefValue,sgnUserValue from CAN_SIGNAL where msgId=%d", id);
					//AfxMessageBox(sql);
					adb.ReadDb(sql, siglist);

					for (unsigned int j = 0; j < siglist.size(); j++)
					{
						CAN_SIGNAL sig;
						_empty_sig(&sig);

						sigcol = 0; 

						sig.sgnlName = CMyString::toString(siglist[j][sigcol++]);
						sig.sgnStartBit = CMyString::toInt(siglist[j][sigcol++]);
						sig.sgnBitSize = CMyString::toInt(siglist[j][sigcol++]);
						sig.sgnByteOrder = CMyString::toInt(siglist[j][sigcol++]);
						sig.sgnValueType = CMyString::toString(siglist[j][sigcol++]);
						sig.sgnFator = CMyString::toFloat(siglist[j][sigcol++]);
						sig.sgnOffset = CMyString::toFloat(siglist[j][sigcol++]);
						sig.sgnMin = CMyString::toInt(siglist[j][sigcol++]);
						sig.sgnMax = CMyString::toInt(siglist[j][sigcol++]);
						sig.sgnUnit = CMyString::toString(siglist[j][sigcol++]);
						sig.sgnReceiver = CMyString::toString(siglist[j][sigcol++]);
						sig.sgnSendType = CMyString::toInt(siglist[j][sigcol++]);
						sig.sgnDefValue = CMyString::toInt(siglist[j][sigcol++]);
						sig.sgnUserValue = CMyString::toInt(siglist[j][sigcol++]);

						canMsg.signalList.push_back(sig);
					}

					m_CanMatrix.msgList.push_back(canMsg);
				}
			}
		}

		adb.CloseDb();
		return ret;
	}

	bool GetModelListFromDb(std::vector<CString> &list)
	{
		bool ret = false;
		AccessDbDriver adb;
		adb.OpenDb(m_dbAddr);

		CString sql;
		std::vector<std::vector<CString > > reclist;
		sql.Format(L"select name from CAN_MATRIX");
		if (adb.ReadDb(sql, reclist))
		{
			for (unsigned int i = 0; i < reclist.size(); i++)
			{
				list.push_back(reclist[i][0]);
			}
			ret = true;
		}		

		adb.CloseDb();
		return ret;
	}
	CString m_dbAddr;

	bool ReLoadCanMatrixFromDb(CString avtivePrj)
	{
		AccessDbDriver adb;
		adb.OpenDb(m_dbAddr);
		_SetActiveCanMatrix(adb, avtivePrj);
		adb.CloseDb();
		return GetSaveCanMatrixFromDb();
	}


};


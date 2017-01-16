//********************************************************************************
//  MY_MODBUS v0.4
//
//
//********************************************************************************


#ifndef my_MBUS_MASTER_h
#define my_MBUS_MASTER_h
#if defined(USE_MBUS) 
#define __PROG_TYPES_COMPAT__
//#include <avr/pgmspace.h>
#include "MY_define.h"
#include "MY_MODBUS_DEF.h"
#include "MY_MODBUS_BASE.h"
#include <ESP8266TrueRandom.h>


#define MB_MASTER_TIMEOUT 500
//#define MEGUNO_MBMASTER_DEBUG
//#define MB_MASTER_DEBUG_RESPONSE

//#ifndef MEGUNO
//#undef MEGUNO_MBMASTER_DEBUG
//#endif
//#if defined(MB_MASTER_DEBUG_SEND)|| defined (MB_MASTER_DEBUG_RESPONSE)
//#define MB_MASTER_DEBUG
//#endif
//#define DefSerialExt HardwareSerial 

class MODBUS_MASTER : public MODBUS_BASE
{
public:

	//enum CHANGE_PARAM_CODE
	//{
	//	OM_310 = RAR_OM_PARAM
	//};
	enum MB_MASTER_status
	{
		MB_MASTER_get_from_device = 0,
		//		MB_MASTER_get_single,
		//		MB_MASTER_send_to_device,
		MB_MASTER_response_ERR,
		MB_MASTER_response_TIME,
		MB_MASTER_response_LEN,
		MB_MASTER_raw_to_int
	};
	enum MB_MASTER_Param
	{
		MB_MASTER_param_MERCURY = 1,
		MB_MASTER_param_Disable_MERC = 2,
		MB_MASTER_param_Disable_EXT = 3,
		MB_MASTER_param_Disable_OM310 = 4,
		MB_MASTER_param_InfoMail
	};

	enum Device_names
	{
#if defined(USE_MERCURY)
		DEV_MERC,
#endif
#if defined(USE_OM310)
		DEV_OM,
#endif
#if defined (USE_EXT_ETHERNET)
		DEV_EXT,
#endif
		DEV_COUNT
	};

	uint16_t _status_master;
	uint8_t _param;
//	CHANGE_PARAM_STRUCT _CHANGE_PARAM;	
	//void EXT_DEBUG();
	void get_from_device(uint16_t timeout);
//	void get_from_device(const uint8_t* cmd, uint16_t timeout);
	void get_from_device(uint8_t adr, uint16_t reg, uint16_t reg_count);

	uint16_t get_single(uint8_t adr, uint16_t reg);

	void response(uint16_t timeout);
//#if defined(MEGUNO_MBMASTER_DEBUG) && defined(DEBUG_OUT)
//	void  DumpMBMStatus();
//	void  DumpMBMparam();
//#endif
	inline void StatusMSet(MB_MASTER_status Nbit) { bitSet(_status_master, Nbit); }
	inline uint8_t StatusMChk(MB_MASTER_status Nbit) { return bitRead(_status_master, Nbit); };
	inline 	void StatusMClear(MB_MASTER_status Nbit) { bitClear(_status_master, Nbit); }

	inline void StatusPSet(MB_MASTER_Param Nbit) { bitSet(_param, Nbit); };
	inline uint8_t StatusPChk(MB_MASTER_Param Nbit) { return bitRead(_param, Nbit); };
	inline 	void StatusPClear(MB_MASTER_Param Nbit) { bitClear(_param, Nbit); };

	inline void Disable_MERC()  { bitSet(_param, MB_MASTER_param_Disable_MERC); };
	inline void Disable_OM310() { bitSet(_param, MB_MASTER_param_Disable_OM310); };
	//inline void Disable_EXT()   { bitSet(_param, MB_MASTER_param_Disable_EXT); };

	inline uint8_t DevCHK_MERC()  { return bitRead(_param, MB_MASTER_param_Disable_MERC); };
	inline uint8_t DevCHK_OM310() { return bitRead(_param, MB_MASTER_param_Disable_OM310); };
//	inline uint8_t DevCHK_EXT()   { return bitRead(_param, MB_MASTER_param_Disable_EXT); };

	inline 	void Enable_MERC() { bitClear(_param, MB_MASTER_param_Disable_MERC); };
	inline 	void Enable_OM310() { bitClear(_param, MB_MASTER_param_Disable_OM310); };
//	inline 	void Enable_EXT() { bitClear(_param, MB_MASTER_param_Disable_EXT); };
	uint16_t Device_CHK(uint8_t Device_adr);
};

//void MODBUS_MASTER::EXT_DEBUG()
//{
//		uint16_t diag_code = 0;
//		_buf_send = _buf;
//		uint16_t return_bad = 0xFFFF;
//		while (1)
//	{
//
//	labc:	insert_cmd(RAR_ETHERNET, MB_CMD_DIAG, (uint16_t)diag_code);
//			uint16_t diag_data = random(300);
//	insert(diag_data);
//		SET_NORESP(NULL);
//		set_crc();
//		//_buf_i_send = 0;
//		//_buf_send[_buf_i_send++] = 0x00;
//		//_buf_send[_buf_i_send++] = 0x01;
//		//_buf_send[_buf_i_send++] = 0xFF;
//		//write();
//		//	dump_buf_send();
//		response(200);
//
//		delay(3000);
//	}
//}
//******************************************************************
uint16_t MODBUS_MASTER::Device_CHK(uint8_t Device_adr)
//******************************************************************
{
//	_Device_num = Device_num;
	uint8_t repeat = 0;
	uint16_t diag_code = 0;
	uint16_t diag_data = ESP8266TrueRandom.randomByte();
	_buf_send = _buf;
	uint16_t return_bad = 0xFFFF;
labc:	insert_cmd(Device_adr, MB_CMD_DIAG, (uint16_t)diag_code);
	insert(diag_data);
//	SET_NORESP(NULL);
//	Mbus.SET_ERR(_EVENTS[_DEVICES[Device_num]._Event_ERR]._Func_Begin);
//	Mbus.SET_BADCRC(_DEVICES[Device_num]._Func_BADCRC);
	set_crc();

		dump_buf_send();

	response(200);
			dump_buf();

	if (StatusChk(MODBUS_BASE::MB_ERROR_TIMEOUT) == 1)
	{
		repeat++;
		if (repeat < _repeat)
		{
			goto labc;
		}
		return return_bad;
	}
	else
		if (StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
		{
//#if defined(USE_DEV_DEBUG)
//			Device_DEBUG_MSG();
//			DEBUG_OUT << F("CHK: response_ERR ADD_ERR") << endol;
//#endif
			return return_bad;
		}
	uint16_t val;
	raw_to_int(&val, 1, 4);
	if (val == diag_data)
	{
		//Device_RESET(Device_num);
		//		DEBUG_OUT << "CHK OK! \n";
		return 0;
	}
	else
	{
//#if defined(USE_DEV_DEBUG)
//		Device_DEBUG_MSG();
//		DEBUG_OUT << F("CHK: resp(0x") << _HEX(val) << F(")<>req(0x") << _HEX(diag_data) << ')' << endol;
//#endif
		return return_bad;
	}
	return return_bad;
}

////************************************************************************************
//#if defined(MEGUNO_MBMASTER_DEBUG) && defined(DEBUG_OUT)
////*************************************************************************************
//void  MODBUS_MASTER::DumpMBMStatus()
////*************************************************************
//{
//	for (uint16_t i = 0; i < 5; i++)
//	{
//		if (bitRead(_status_master, i))
//			DEBUG_OUT << F("{UI:")  << F("|SET|MBMS") << i << F(".Visible=True}\n");
//		else
//			DEBUG_OUT << F("{UI:")  << F("|SET|MBMS") << i << F(".Visible=False}\n");
//	}
//}
////*******************************************************************************
//void  MODBUS_MASTER::DumpMBMparam()
////******************************************************
//{
//	for (uint16_t i = 1; i < 6; i++)
//	{
//		if (bitRead(_param, i))
//			DEBUG_OUT << F("{UI:") << F("|SET|MBMP") << i << F(".Visible=True}\n");
//		else
//			DEBUG_OUT << F("{UI:")  << F("|SET|MBMP") << i << F(".Visible=False}\n");
//	}
//}
//#endif
//
////*********************************************************************************
//void MODBUS_MASTER::get_from_device(const uint8_t* cmd, uint16_t timeout)
////*********************************************************************************
//{
//	//	_buf_send = (uint8_t*)&cmd[1];
//
//	_buf_i_send = pgm_read_byte_near(cmd + 1);
//	for (uint8_t i = 0; i < _buf_i_send; i++)
//	{
//		_buf_send[i] = pgm_read_byte(cmd + i);
//	}
//	_buf_i_send = cmd[0];
//	uint16_t savt = _tmout;
//	_tmout = timeout;
//	//	write();
//	response(timeout);
//
//	_tmout = savt;
//}
//*********************************************************************************
void MODBUS_MASTER::get_from_device(uint16_t timeout)
//*********************************************************************************
{
	_status_master = 0;
	//	_status_base = 0;
	//	_buf_send = _buf;
	_dev_adr = _buf_send[0];
	//	send();

	response(timeout);
}
//*********************************************************************************
void MODBUS_MASTER::get_from_device(uint8_t adr, uint16_t reg, uint16_t reg_count)
//*********************************************************************************
{
	_status_master = 0;
	//	_status_base=0;
	_dev_adr = adr;
	//	DEBUG_OUT<<"MODBUS::get_from_device: begin\n";
//#if defined(MB_MASTER_DEBUG_SEND)
//	char p1[] = "MODBUS::get_from_device: ";
//	DEBUG_OUT << p1 << "register:" << reg << "(";
//	my_printHEX(reg >> 8); DEBUG_OUT.print(";");
//	my_printHEX(reg & 0x00ff); DEBUG_OUT << ") reg_count=" << reg_count;
//	DEBUG_OUT << " responce_len=" << responce_len << "\n";
//#endif
	//	uint16_t sav_buf_i = Mbus._buf_i;
	uint8_t buf_send[8];
	_buf_send = buf_send;
	insert_cmd(adr, MB_CMD_READ, reg);					//����� ����������
	insert(reg_count);									//���-�� ���������
	set_crc();
	response(200);
}

//*********************************************************************************
uint16_t MODBUS_MASTER::get_single(uint8_t adr, uint16_t reg)
//*********************************************************************************
{
	//	DEBUG_OUT << "GET SINGLE::"<< endol;
	get_from_device(adr, reg, 1);
	if (StatusMChk(MB_MASTER_get_from_device) == 0)
	{
		uint16_t val;
		raw_to_int(&val, 1, 2);
		return val;
	}
	else
		return 0xFFFF;
}


//*********************************************************************************
void MODBUS_MASTER::response(uint16_t timeout)
//*********************************************************************************
{
//#if defined(MB_MASTER_DEBUG_SEND)
//	DEBUG_OUT << F("**** MB MASTER: response") << F(" BEGIN ****") << "\n";
//#endif
lab1:	StatusMClear(MB_MASTER_response_ERR);
//	DEBUG_OUT << F("Point1") << "\n";
	write();
//	DEBUG_OUT << F("Point2") << "\n";
	read(timeout);
//	DEBUG_OUT << F("Point3") << "\n";
	//	dump_buf();
	//если был тайм-аут - повторить еще раз
	if ((StatusChk(MB_ERROR_TIMEOUT) == 1))
	{
		if (_repeat == 1)
		{
			_repeat++;
//#if defined (MB_MASTER_DEBUG_SEND)
//
//
//			DEBUG_OUT << F("Point4") << "\n";
//#endif
//			DEBUG_OUT << F("\nIN Response: repeat\n");	
			delay(100);
			goto lab1;
		}
		else
		{
//			DEBUG_OUT << F("Point5") << "\n";
			StatusMSet(MB_MASTER_response_TIME);
			//if (_Func_NORESP != NULL)
			//{
			//	DEBUG_OUT << F("Point6") << "\n";
			////	delay(20);
			//	//******************************************
			//	// тайм-аут - нет ответа
			//	//******************************************
			//	(*_Func_NORESP)();
			//	_Func_NORESP = NULL;
			//}
			//DEBUG_OUT << F("**** MB MASTER: response") << F(" RETURN") << "\n";
			return;
		}
	}
	//DEBUG_OUT << F("Point8") << "\n";
	//****************************************************************************
	// ошибка crc
	//****************************************************************************
	if (StatusChk(MB_ERROR_CRC) == 1)
	{
//		DEBUG_OUT << F("\n*****BAD CRC*****") << "\n";
		dump_buf();
		StatusMSet(MB_MASTER_response_ERR);
		//if (_Func_BADCRC != NULL)
		//{
		//	//			DEBUG_OUT << F("PointA") << "\n";
		//	(*_Func_BADCRC)();
		//	_Func_BADCRC = NULL;
	
		//}	
		//DEBUG_OUT << F("**** MB MASTER: response") << F(" RETURN") << "\n";

		return;
	}
	//***************************************************************************
	// ERR
	//***************************************************************************
	if ((_buf[1] != _cmd) && (StatusPChk(MB_MASTER_param_MERCURY) == 0))
	{
//		DEBUG_OUT << F("Point10") << "\n";
		StatusMSet(MB_MASTER_response_ERR);
		StatusSet(MB_ERROR_CMD);
//#//if defined (MB_MASTER_DEBUG_SEND)
//		DEBUG_OUT << F("\nRESPONSE:SET MB_ERROR_CMD  sav cmd=") << _HEX(_cmd) << F(" return cmd=") << _HEX(_buf[1]) << endol;
////#endif

		dump_buf();
	//	STOP
//			if (_Func_ERR != NULL)
//			{
////				DEBUG_OUT << F("Point11") << "\n";
//				//******************************************
//				//******************************************
//				(*_Func_ERR)();
//				_Func_ERR = NULL;
//			}
	}
//#if defined(MB_MASTER_DEBUG_SEND)
//	DEBUG_OUT << F("**** MB MASTER: response") << F(" END") << "\n";
//#endif
}


#endif

#endif
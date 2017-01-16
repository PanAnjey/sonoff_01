//********************************************************************************
//  Mercury v0.1
// библиотека для работы со счетчиком Меркурий 230
//
//********************************************************************************



#ifndef my_MR_h
#define my_MR_h
#define __PROG_TYPES_COMPAT__
#include <avr\pgmspace.h>
#include "MY_MODBUS_MASTER.h"
#include "MY_EVENT_ARRAY.h"
#include "MY_MERCURY_DEF.h"
#include <Streaming.h>
//#include <MY_SERIAL_DEBUG2.h>
//#include <EEPROM.h>

#include <MY_Time2.h>
#include <Flash.h>


//#define MR_DEBUG_TRACE
#ifndef MEGUNO
#undef MR_DEBUG_TRACE
#endif
//#define MR_DEBUG_RESPONSE
//#define MR_DEBUG_LOG
#define MR_TIMEOUT 60  //кол-во милисекунд до таймаута чтения данных
#define MR_CHAN_TIMEOUT 15 //кол-во секунд откытия канала.


//#define MR_DEBUG_PARAM

//#define MAX_RESPONSE_LENGTH 256

EXT_ENERGY EXT_EN;



FLASH_ARRAY(uint8_t, MR_CHANCHK, MERCURY_addr, MRCMD_CHAN_CHK, 0x1F, 0x40);
//const uint8_t PROGMEM   MR_CHANOPEN[] = { 11, MERCURY_addr, MRCMD_CHAN_OPEN, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x5A, 0xF6 };
FLASH_ARRAY(uint8_t, MR_CHANOPEN, MERCURY_addr, MRCMD_CHAN_OPEN, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x5A, 0xF6);
FLASH_ARRAY(uint8_t, MR_GETCURTIME, MERCURY_addr, MRCMD_READ_TIMES, MR_CUR_TIME, 0x03, 0x08);//текущее время

//const prog_char MR_GETEN_TOTAL[] PROGMEM = { MERCURY_addr, MRCMD_GET_ENERGY, MR_EN, 0x00, 0x19, 0xC1 };			//полная энергия по сумме тарифов от сброса...
FLASH_ARRAY(uint8_t, MR_GETEN_YA, MERCURY_addr, MRCMD_GET_ENERGY, MR_EN_YA, 0x00, 0x14, 0x01);//полная энергия по сумме тарифов за текущий год...
FLASH_ARRAY(uint8_t, MR_GETEN_LYA, MERCURY_addr, MRCMD_GET_ENERGY, MR_EN_LYA, 0x00, 0x00, 0x01);//полная энергия по сумме тарифов за предыдущий год...
//prog_char MR_GETEN_ME[]={MERCURY_addr,MRCMD_GET_ENERGY,MR_EN_ME,0x00,0x14,0x01};							//полная энергия по сумме тарифов за месяц...		
FLASH_ARRAY(uint8_t, MR_GETEN_ED, MERCURY_addr, MRCMD_GET_ENERGY, MR_EN_24, 0x00, 0x28, 0x01);//полная энергия по сумме тарифов за текущие сутки...
FLASH_ARRAY(uint8_t, MR_GETEN_LD, MERCURY_addr, MRCMD_GET_ENERGY, MR_EN_L24, 0x00, 0x25, 0xC1);//полная энергия по сумме тарифов за предыдущие сутки...
FLASH_ARRAY(uint8_t, MR_GETEN_ABC, MERCURY_addr, MRCMD_GET_ENERGY, MR_ENP_ABC, 0x00, 0x31, 0xC1);//Чтение пофазных значений накопленной активной энергии прямого направления


/*enum _MR_CMD_
{
	MR_CMD_CHANCHK,
	MR_CMD_CHANOPEN,
	MR_CMD_GETCURTIME,
	MR_CMD_GETEN_YA,
	MR_CMD_GETEN_LYA,
	MR_CMD_GETEN_24,
	MR_CMD_GETEN_L24,
	MR_CMD_GETEN_ABC
};*/

/*const   uint8_t* const MERC_CMD[] PROGMEM =
{
	MR_CHANCHK,
	MR_CHANOPEN,
	MR_GETCURTIME,
	MR_GETEN_YA,
	MR_GETEN_LYA,
	MR_GETEN_24,
	MR_GETEN_L24,
	MR_GETEN_ABC
	//	MR_GETEN_TOTAL
};*/


//const uint8_t PROGMEM MR_VAL_HEADER[3] = { MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2 };
FLASH_ARRAY(uint8_t, MR_VAL_HEADER, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2);

FLASH_ARRAY(uint8_t, MR_VAL_POWERP, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2, MR_VALUES_PWRP, 0x86, 0x62);
FLASH_ARRAY(uint8_t, MR_VAL_POWERQ, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2, MR_VALUES_PWRQ, 0x87, 0xA1);
FLASH_ARRAY(uint8_t, MR_VAL_POWERS, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2, MR_VALUES_PWRS, 0x87, 0xA4);
FLASH_ARRAY(uint8_t, MR_VAL_COSF, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2, MR_VALUES_COSF, 0x86, 0x76);

FLASH_ARRAY(uint8_t, MR_VALV, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2, MR_VALUES_V, 0x46, 0x6E);
FLASH_ARRAY(uint8_t, MR_VALI, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2, MR_VALUES_I, 0x46, 0x7A);
FLASH_ARRAY(uint8_t, MR_VALA, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2, MR_VALUES_A12, 0x47, 0x9E);
FLASH_ARRAY(uint8_t, MR_VALF, MERCURY_addr, MRCMD_GET_PARAM, MRCMD_SUB_P2, MR_VALUES_F, 0x87, 0x92);

const password MR_pasw = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };

//*********************************************************************************************
class Mercury
	//*********************************************************************************************
{
public:
	enum MR_enum
	{
		MR_OK,
		MR_ERROR_CHAN_CHK,
		MR_ERROR_CHAN_OPEN,
		MR_ERROR_LOG,
		MR_ERROR_VALUES,
		MR_ERROR_REG_POW,
		MR_ERROR_READ
	};


	uint8_t _LOG_TYP;
	uint8_t _loglen1;
	uint8_t _len2;
	uint8_t _len;
	uint8_t _log_ON_and_OFF;
	time_t _chan_open_Last_time;
	typedef void(*RunExternalProc)();
	RunExternalProc _Func_NORESP;			// процедура запускаемая при наступлении таймаута
	RunExternalProc _Func_BADCRC;			// процедура запускаемая при несовпадении CRC
	RunExternalProc _Func_ERR;				// процедура запускаемая при других ошибках	

//	void begin(uint8_t counter_adr, MODBUS_MASTER * Mbus);
	void begin();
	void SetHandler();
	void get_from_device(uint8_t addr, uint8_t cmd, uint8_t reg, uint8_t param);
	void response(_FLASH_ARRAY<uint8_t> dat);
	void response();
	inline void  SET_NORESP(void(*f_func)()) { _Func_NORESP = f_func; };
	inline void  SET_BADCRC(void(*f_func)()) { _Func_BADCRC = f_func; };
	inline void  SET_ERR(void(*f_func)()) { _Func_ERR = f_func; };
	inline void SET_FUNC(void(*f_nfunc)(), void(*f_cfunc)(), void(*f_efunc)())
	{
		_Func_NORESP = f_nfunc; _Func_BADCRC = f_cfunc; _Func_ERR = f_efunc;
	}
	void chan_open();

	time_t get_cur_time();
	//	int get_FromOM(int reg, int len, int* data);
	void Set_log_param(uint8_t LOG_TYP);
	void get_log_1(uint8_t rec_no, LOG_REC *logs_data);
	void get_log_N(uint8_t event_log, LOG_REC *logs_data);
	uint8_t get_log_last_rec();

	void decode_log_rec(LOG_REC *logs_data);

	MR_current_value get_values();
	uint8_t get_values(MR_current_value* __values);

	void time_correct(time_t time_c);
	void time_set(time_t time_c);
	//	void req_resp_mk(uint16_t reg, uint8_t cnt, uint8_t *val, uint8_t size);
	void MR_current_value_test(MR_current_value* t);

	MR_ENERGY_STRUCT get_values_44(const uint8_t *p);
private:

	uint32_t copy3bytes(int8_t b1, int8_t b2, int8_t b3);
	uint32_t copy3bytes2(uint8_t n);
	uint32_t copy4bytes(int8_t b1);
	uint8_t to10(uint8_t b);
	uint8_t to2_10(uint8_t c);
	//	void set_buf(uint8_t subcmd);
	MR_VAL3 get_values_33(_FLASH_ARRAY<uint8_t> dat);
	MR_VAL4 get_values_43(_FLASH_ARRAY<uint8_t> dat);

};
//**************************************************************************
void Mercury::MR_current_value_test(MR_current_value* t)
//**************************************************************************
{
	DEBUG_OUT << "POWER P A:" << t->_POW.A << " B:" << t->_POW.B << " C:" << t->_POW.C << " S:" << t->_POW.S << endol;
	DEBUG_OUT << "POWER Q A:" << t->_Q.A << " B:" << t->_Q.B << " C:" << t->_Q.C << " S:" << t->_Q.S << endol;
	DEBUG_OUT << "POWER S A:" << t->_SPOW.A << " B:" << t->_SPOW.B << " C:" << t->_SPOW.C << " S:" << t->_SPOW.S << endol;
	DEBUG_OUT << "COSF   A:" << t->_COSF.A << " B:" << t->_COSF.B << " C:" << t->_COSF.C << " S:" << t->_COSF.S << endol;
	DEBUG_OUT << "VOLT   A:" << t->_V.A << " B:" << t->_V.B << " C:" << t->_V.C << endol;
	DEBUG_OUT << "AMPER  A:" << t->_I.A << " B:" << t->_I.B << " C:" << t->_I.C << endol;
	//	DEBUG_OUT << "ANGL   A:" << t->_A.A << " B:" << t->_A.B << " C:" << t->_A.C << endol;
	DEBUG_OUT << "FREQ   F:" << t->_F << endol;
	DEBUG_OUT << "Timestamp:" << t->_TimeStamp << endol;

}

//*********************************************************************************
void Mercury::begin()
//*********************************************************************************
{
	//	_statusE = MR_OK;
	//	_addr = counter_adr;
	//	_Mbus = Mbus;
	_chan_open_Last_time = 0;
	//	_repeat=2;

}
//********************************************************************
void Mercury::SetHandler()
//********************************************************************
{//устанавливаем обработчики событий
	Mbus.SET_NORESP(_Func_NORESP);
	Mbus.SET_ERR(_Func_ERR);
	Mbus.SET_BADCRC(_Func_BADCRC);
//	DEBUG_OUT << "SET HANDLER\n";
}
//***********************************************************************************
void Mercury::response()
//***********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC response"));
#endif
	Mbus._status_master = 0;
	Mbus.StatusPSet(MODBUS_MASTER::MB_MASTER_param_MERCURY);
	//устанавливаем обработчики событий	
	SetHandler();
	Mbus.get_from_device(MR_TIMEOUT);
//	STOP
	Mbus.StatusPClear(MODBUS_MASTER::MB_MASTER_param_MERCURY);
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceEnd);
#endif
}

//*********************************************************************************
void Mercury::response(_FLASH_ARRAY<uint8_t> dat)
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC response FLASH"));
#endif
	Mbus._buf_send = Mbus._buf;
	Mbus.set_buf_send_fromFLASH(dat);
	response();
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceEnd();
#endif
}

//*********************************************************************************
void  Mercury::chan_open()
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC CHAN OPEN"));
#endif
	response(MR_CHANOPEN);
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceEnd();
#endif
}

//*********************************************************************************
void  Mercury::time_correct(time_t time_c)
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC time_correct"));
	DEBUG_TRACE << "time correct: second=" << second(time_c) << "\n";
#endif
	Mbus.insert_cmd(MERCURY_addr, MRCMD_WRITE_PARAM, (uint8_t)TimeCORRECT);
	Mbus.insert(to2_10(second(time_c)));
	Mbus.insert(to2_10(minute(time_c)));
	Mbus.insert(to2_10(hour(time_c)));
	//	Mbus.dump_buf_send();
	//	STOP
	Mbus.set_crc();
	SetHandler();
	Mbus.get_from_device(2000);
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceEnd();
#endif
}

//*********************************************************************************
void Mercury::time_set(time_t time_c)
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC time_set"));
#endif
	tmElements_t t;
	breakTime(time_c, t);
	//	Mbus.dump_buf("time_set", " start", Mbus._buf);
	Mbus.insert_cmd(MERCURY_addr, MRCMD_WRITE_PARAM, (uint8_t)TimeSET);
	Mbus.insert(to2_10(t.Second));
	Mbus.insert(to2_10(t.Minute));
	Mbus.insert(to2_10(t.Hour));
	Mbus.insert(to2_10(t.Day));
	Mbus.insert(to2_10(t.Month));
	Mbus.insert(to2_10(t.Year));
	Mbus.insert((uint8_t)0);
	Mbus.get_from_device(MR_TIMEOUT);
	if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 0)
	{
		//		DEBUG_OUT << "*Mercury* time_set" << endol;
		//		Mbus.dump_buf( Mbus._buf);

		if (time_c != get_cur_time())
		{
			DEBUG_OUT << "TIME NOT SET:" << endol;
			//			_statusE = MR_ERROR_VALUES;
		}
	}
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceEnd();
#endif
}

//*********************************************************************************
uint8_t Mercury::to2_10(uint8_t c)
//*********************************************************************************
{
	uint8_t ed = c % 10;
	uint8_t des = (c - ed) / 10;

	//	DEBUG_OUT << F("to2_10 c=") << c << F(" res=") << _HEX((des << 4) | ed)<<endol;
	return (des << 4) | ed;
}
/*
//*********************************************************************************
void  Mercury::get_from_device( uint16_t timeout)
//*********************************************************************************
{
	Mbus.StatusPSet(MODBUS_MASTER::MB_MASTER_param_MERCURY);
	Mbus.get_from_device( timeout);
	Mbus.StatusPClear(MODBUS_MASTER::MB_MASTER_param_MERCURY);
}
//*********************************************************************************
void  Mercury::get_from_device(const char* const cmd,  uint16_t timeout)
//*********************************************************************************
{
	Mbus.StatusPSet(MODBUS_MASTER::MB_MASTER_param_MERCURY);
	Mbus.get_from_device(cmd, timeout);
	Mbus.StatusPClear(MODBUS_MASTER::MB_MASTER_param_MERCURY);
}*/


/**/
//*********************************************************************************
void Mercury::get_from_device(uint8_t addr, uint8_t cmd, uint8_t reg, uint8_t param)
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC get_from_device"));
#endif
	//_statusE = MR_OK;
	uint8_t buf_send[6];
	Mbus._buf_send = buf_send;
	Mbus.insert_cmd(addr, cmd, reg, param);
	Mbus.StatusPSet(MODBUS_MASTER::MB_MASTER_param_MERCURY);
	Mbus.set_crc();
	SetHandler();
	uint8_t savrep = Mbus._repeat;
	Mbus._repeat = 1;
	Mbus.response(MB_MASTER_TIMEOUT);
	Mbus._repeat = savrep;

	Mbus.StatusPClear(MODBUS_MASTER::MB_MASTER_param_MERCURY);
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceEnd();
#endif
}
/**/
////*********************************************************************************
//void Mercury::send_cmd4(uint8_t cmd)
////*********************************************************************************
//{
//	Mbus.insert_cmd(MERCURY_addr, cmd);
//	Mbus.get_from_device(300);				//������ ������
//}
//*********************************************************************************
uint8_t Mercury::to10(uint8_t b)
//*********************************************************************************
{
	uint8_t dig, dig2;
	dig = b;
	dig &= 0xF0;
	dig = dig >> 4;
	dig = dig * 10;
	dig2 = b;
	dig2 &= 0x0F;
	return dig = dig + dig2;
}

//*********************************************************************************
time_t Mercury::get_cur_time()
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC get_cur_time"));

#endif
	//	Mbus.get_from_device(MR_GETCURTIME, MR_TIMEOUT);
	response(MR_GETCURTIME);
	if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 0)
	{

		//#if defined(MR_DEBUG) && defined (MR_DEBUG_RESPONSE)
		//	Mbus.dump_buf("Get time: ","", Mbus._buf);
		/*00-0;
		50-1-сек;
		52-2-мин;
		23-3-час;
		04-4-день;
		26-5-число;
		12-6-мес;
		13-7-год;
		01-8- зима/лето;
		ED;27- 9,10 - crc;
		*/
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
		MG_TraceEnd();
#endif
		return makeTime(to10(Mbus._buf[3]), to10(Mbus._buf[2]), to10(Mbus._buf[1]), \
			to10(Mbus._buf[5]), to10(Mbus._buf[6]), to10(Mbus._buf[7]));
	}
	else
	{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
		MG_TraceEnd();
		return 0;
#endif
	}
}

//*********************************************************************************
void Mercury::decode_log_rec(LOG_REC *rec)
//*********************************************************************************
{
	//	time_t t;
	//	Mbus.dump_buf(Mbus._buf);
	int j = 0;

	for (int i = 0; i < _loglen1; i++)
	{
		rec[i].ON = 0;
		if (Mbus._buf[6 + j] != 0)
			rec[i].ON = makeTime(to10(Mbus._buf[3 + j]), to10(Mbus._buf[2 + j]), to10(Mbus._buf[1 + j]), \
				to10(Mbus._buf[4 + j]), to10(Mbus._buf[5 + j]), to10(Mbus._buf[6 + j]));
		//#if defined(MR_DEBUG_LOG)
				//DEBUG_OUT << "\n Rec #:" << i << " ON: " << _HEX(Mbus._buf[6 + j]) << "-" << _HEX(Mbus._buf[5 + j]) << "-"\
				//	<< _HEX(Mbus._buf[4 + j]) << " " << _HEX(Mbus._buf[3 + j]) << ":" << _HEX(Mbus._buf[2 + j]) << ":" << _HEX(Mbus._buf[1 + j]) << "";
				//	DEBUG_OUT << "time:";
				//	printTime(t);
				//	DEBUG_OUT << endol;
		//#endif
		j = j + 6;

		if (_log_ON_and_OFF)
		{
			rec[i].OFF = 0;
			//		DEBUG_OUT << "12 + j=" << 12 + j << " buf=" << Mbus._buf[12 + j] << endol;
			if (Mbus._buf[6 + j] != 0)
			{
				rec[i].OFF = makeTime(to10(Mbus._buf[3 + j]), to10(Mbus._buf[2 + j]), to10(Mbus._buf[1 + j]), \
					to10(Mbus._buf[4 + j]), to10(Mbus._buf[5 + j]), to10(Mbus._buf[6 + j]));
				//#if defined(MR_DEBUG_LOG)
								//DEBUG_OUT << "  OFF: " << _HEX(Mbus._buf[6 + j]) << "-" << _HEX(Mbus._buf[5 + j]) << "-"\
								//	<< _HEX(Mbus._buf[4 + j]) << " " << _HEX(Mbus._buf[3 + j]) << ":" << _HEX(Mbus._buf[2 + j]) << ":" << _HEX(Mbus._buf[1 + j]) << "\n";

								//			DEBUG_OUT << "time:";
								//			printTime(t);
								//			DEBUG_OUT << endol;
				//#endif
			}
			j = j + 6;

		}
	}
}

/*********************************************************************************
LOG_REC Mercury::get_log_rec()
//возвращает декодированную запись лога из входного буфера
//*********************************************************************************
{
	LOG_REC rec;
	time_t t;
	if (Mbus._buf[6] == 0)
		t = 0;
	else
		t = makeTime(to10(Mbus._buf[3]), to10(Mbus._buf[2]), to10(Mbus._buf[1]), \
		to10(Mbus._buf[4]), to10(Mbus._buf[5]), to10(Mbus._buf[6]));
#if defined(MR_DEBUG_LOG)
	DEBUG_OUT << "---------\n Rec #:" << _rec_no << " ON: " << _HEX(Mbus._buf[6]) << "-" << _HEX(Mbus._buf[5]) << "-"\
		<< _HEX(Mbus._buf[4]) << " " << _HEX(Mbus._buf[3]) << ":" << _HEX(Mbus._buf[2]) << ":" << _HEX(Mbus._buf[1]) << "\n";
	DEBUG_OUT << ">>>time:" << t << "=";
	printTime(t);
	DEBUG_OUT << endol;
#endif
	rec.ON = t;
	if (_log_ON_and_OFF)
	{
		if (Mbus._buf[12] == 0)
			t = 0;
		else
			t = makeTime(to10(Mbus._buf[9]), to10(Mbus._buf[8]), to10(Mbus._buf[7]), \
			to10(Mbus._buf[10]), to10(Mbus._buf[11]), to10(Mbus._buf[12]));
#if defined(MR_DEBUG_LOG)
		DEBUG_OUT << "  OFF: " << _HEX(Mbus._buf[12]) << "-" << _HEX(Mbus._buf[11]) << "-" << _HEX(Mbus._buf[10]) << " "\
			<< _HEX(Mbus._buf[9]) << ":" << _HEX(Mbus._buf[8]) << ":" << _HEX(Mbus._buf[7]) << "\n";
		DEBUG_OUT << ">>>time:" << t << "=";
		printTime(t);
		DEBUG_OUT << endol;
#endif
		rec.OFF = t;
	}
	return rec;
}
*/
//*********************************************************************************
void Mercury::get_log_1(uint8_t rec_no, LOG_REC *logs_data)
//считывает запись из указанного Лога 

//rec_no - номер считываемой записи
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC get_log_1"));

#endif
	Mbus._status_master = 0;
	//	Mbus._status_base = Mbus.MB_OK;
	// скобочки оставить - для совместимости
	uint8_t buf_send[6];
	Mbus._buf_send = buf_send;
	Mbus.insert_cmd(MERCURY_addr, MRCMD_READ_TIMES, _LOG_TYP, rec_no);	//формирование запроса
	Mbus.StatusPSet(MODBUS_MASTER::MB_MASTER_param_MERCURY);
	Mbus.get_from_device(100);											//чтение ответа
	if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 0)	//если удачно считалось
	{
		decode_log_rec(logs_data);										//декодируем и возвращаем результат
	}
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceEnd();
#endif
}
//*********************************************************************************
void Mercury::Set_log_param(uint8_t LOG_TYP)
//*********************************************************************************
{
	_LOG_TYP = LOG_TYP;
	if ((_LOG_TYP >= RAR_MERC_LOG_COR_SCHED) && (_LOG_TYP <= RAR_MERC_LOG_EN_LIM_4))
	{
		_len = 63;
		_len2 = 10;
		_log_ON_and_OFF = 0;
		_loglen1 = 10;
	}
	else
		//***********************************************************************************	*/
	{
		_len2 = 16;
		_log_ON_and_OFF = 1;
		if (_LOG_TYP >= RAR_MERC_LOG_V_MIN_A)
		{
			_len = 243;
			_loglen1 = 20;
		}
		else
		{
			_len = 123;
			_loglen1 = 10;
		}
	}
	//	DEBUG_OUT << "PARAM LOG:" << _LOG_TYP<<" _len = " << _len << " _len2 = " << _len2 << " _log_ON_and_OFF = " << _log_ON_and_OFF << " _loglen1 = " << _loglen1 << endol;
}
//*********************************************************************************
void Mercury::get_log_N(uint8_t LOG_TYP, LOG_REC *logs_data)
//считывает записи из указанного Лога в ускоренном режиме
//event_log - код лога
//logs_data - указатель на структуру для размещения считанных данных
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	DEBUG_TRACE << F("MERC ") << F("get_log_N") << F(" BEGIN\n");
	MG_TraceBegin(F("MERC get_log_N"));
#endif
	Mbus._status_master = 0;
	Set_log_param(LOG_TYP);
	get_from_device(MERCURY_addr, MRCMD_READ_TIMES, LOG_TYP, 0xFE);	//формирование запроса для ускоренного режима чтения
	//для логов с числом записей больше 10 в первой	пачке вернутся 
	//записи 0-19
	//Mbus.dump_buf(Mbus._buf);
	if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
	{
		DEBUG_OUT << "get_log_N::RESPONSE ERROR\n";
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
		MG_TraceEnd();
#endif
		return;
	}
	decode_log_rec(&logs_data[0]);
	//**********************************************
	//считывание записей лога если их больше 10
	//**********************************************
	if (_LOG_TYP >= RAR_MERC_LOG_V_MIN_A)
	{
		get_from_device(MERCURY_addr, MRCMD_READ_TIMES, _LOG_TYP, 0xFD);	//запрос записей 20-39
		if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
		{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
			MG_TraceEnd();
#endif
			return;
		}
		decode_log_rec(&logs_data[20]);

		get_from_device(MERCURY_addr, MRCMD_READ_TIMES, _LOG_TYP, 0xFC);	//запрос записей 40-59
		if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
		{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
			MG_TraceEnd();
#endif
			return;
		}
		decode_log_rec(&logs_data[40]);

		get_from_device(MERCURY_addr, MRCMD_READ_TIMES, _LOG_TYP, 0xFB);	//запрос записей 60-79
		if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
		{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
			MG_TraceEnd();
#endif
			return;
		}
		decode_log_rec(&logs_data[60]);

		get_from_device(MERCURY_addr, MRCMD_READ_TIMES, _LOG_TYP, 0xFA);	//запрос записей 80-99
		if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
		{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
			MG_TraceEnd();
#endif
			return;
		}
		decode_log_rec(&logs_data[80]);
	}
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceEnd();
#endif
}

//*********************************************************************************
uint8_t Mercury::get_log_last_rec()
//возвращает номер последней записи указанного лога
//*********************************************************************************
{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
	MG_TraceBegin(F("MERC get_log_last_rec"));
#endif
	Mbus._status_master = 0;
	//	Mbus._status_base = Mbus.MB_OK;
	uint8_t buf_send[6];
	Mbus._buf_send = buf_send;
	Mbus.insert_cmd(MERCURY_addr, MRCMD_READ_TIMES, _LOG_TYP, 0xFF);
	Mbus.set_crc();
	//	DEBUG_OUT << F("BEGIN get_log_last_rec...\n");
	//		Mbus.StatusSet(MODBUS_BASE::MB_DEBUG);
	response();
	//	Mbus.dump_buf_send();
	//	Mbus.dump_buf();
	//	Mbus.StatusClear(MODBUS_BASE::MB_DEBUG);
	if (Mbus._status_master == 0)
	{
		if (_log_ON_and_OFF)
		{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
			MG_TraceEnd();
#endif
			return Mbus._buf[13];
		}
		else
		{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
			MG_TraceEnd();
#endif
			return Mbus._buf[7];
		}
	}
	else
	{
#if defined DEBUG_TRACE && defined(MR_DEBUG_TRACE)
		MG_TraceEnd();
#endif
		return 0xFF;
	}
}
//*********************************************************************************
uint32_t Mercury::copy3bytes(int8_t b1, int8_t b2, int8_t b3)
//*********************************************************************************
{
	uint32_t val;
	uint8_t *pp;
	pp = (uint8_t*)&val;
	//	DEBUG_OUT<<"ORDER:"<<b1<<", "<<b2<<", "<<b3<<"\n";
	pp[0] = Mbus._buf[b1];
	pp[1] = Mbus._buf[b2];
	pp[2] = Mbus._buf[b3];
	if ((pp[0] == pp[1]) && (pp[0] == pp[2]) && (pp[0] == 0xFF))
		pp[3] = 0xFF;
	else
		pp[3] = 0;
	//	DEBUG_OUT<<"DATA:"<<_HEX(pp[0])<<", "<<_HEX(pp[1])<<", "<<_HEX(pp[2])<<"\n->"<<val<<"("<<_HEX(val)<<")\n";
	return val;
}
//*********************************************************************************
uint32_t Mercury::copy3bytes2(uint8_t n)
//*********************************************************************************
{
	uint32_t val = 0;
	//	uint32_t val_t = 200;
	uint8_t *pp;
	pp = (uint8_t*)&val;
	//	DEBUG_OUT << "ORDER:" << b1 << ", " << b2 << ", " << b3 << " test=" << val_t << "(" << _HEX(val_t) << endol;
	pp[2] = Mbus._buf[n];
	pp[1] = Mbus._buf[n + 2];
	pp[0] = Mbus._buf[n + 1];
	if ((pp[0] == pp[1]) && (pp[0] == pp[2]) && (pp[0] == 0xFF))
		pp[3] = 0xFF;
	else
		pp[3] = 0;
	//DEBUG_OUT << "DATA:" << _HEX(pp[0]) << ", " << _HEX(pp[1]) << ", " << _HEX(pp[2]) << "\n->" << val << "(" << _HEX(val) << ")" << endol;
	return val;
}
//*********************************************************************************
uint32_t Mercury::copy4bytes(int8_t b1)
//*********************************************************************************
{
	uint32_t val;
	uint8_t *pp;
	pp = (uint8_t*)&val;
	pp[2] = Mbus._buf[b1];
	//	DEBUG_OUT<<"b1="<<b1<<" buf="<<_HEX(Mbus._buf[b1])<<endol;
	pp[3] = Mbus._buf[++b1];
	//	DEBUG_OUT<<"b1+1="<<b1<<" buf="<<_HEX(Mbus._buf[b1])<<endol;
	pp[0] = Mbus._buf[++b1];
	//	DEBUG_OUT<<"b1+2="<<b1<<" buf="<<_HEX(Mbus._buf[b1])<<endol;
	pp[1] = Mbus._buf[++b1];
	//	DEBUG_OUT<<"b1+3="<<b1<<" buf="<<_HEX(Mbus._buf[b1])<<endol;
	//    DEBUG_OUT<<"val="<<val<<"("<<_HEX(val)<<")\n\n";
	return val;
}
/*
//*********************************************************************************
void Mercury::set_buf(uint8_t subcmd)
//*********************************************************************************
{
Mbus.insert_cmd(_addr, MRCMD_GET_PARAM, (uint8_t)MRCMD_SUB_P2, subcmd);
}
*/
//*********************************************************************************
MR_VAL3 Mercury::get_values_33(_FLASH_ARRAY<uint8_t> dat)
//*********************************************************************************
{
	response(dat);
	MR_VAL3 val;
	if (Mbus._status_master == 0)
	{
		val.A = copy3bytes(2, 3, 1);
		val.B = copy3bytes(5, 6, 4);
		val.C = copy3bytes(8, 9, 7);
//		Mbus.StatusMClear(MODBUS_MASTER::MB_MASTER_raw_to_int);
		DEBUG_OUT << F(">> raw_to_int clear\n");
#if defined(MEGUNO) && defined(DEBUG_OUT)
		MG_HideControl(F("values_33"));
#endif
	}
	else
	{
		val.A = 0xFFFFFFFF;
		val.B = 0xFFFFFFFF;
		val.C = 0xFFFFFFFF;
		Mbus.StatusMSet(MODBUS_MASTER::MB_MASTER_raw_to_int);
		DEBUG_OUT << F(">> raw_to_int set\n");

#if defined(MEGUNO) && defined(DEBUG_OUT)
		MG_ShowControl(F("values_33"));
//		Mbus.DumpMBMStatus();
#endif
	}
	return val;
}
//*********************************************************************************
MR_VAL4 Mercury::get_values_43(_FLASH_ARRAY<uint8_t> dat)
//*********************************************************************************
{

	response(dat);
	MR_VAL4 val;

	if (Mbus._status_master == 0)
	{
		bitClear(Mbus._buf[1], 7);
		bitClear(Mbus._buf[1], 6);
		bitClear(Mbus._buf[4], 7);
		bitClear(Mbus._buf[4], 6);
		bitClear(Mbus._buf[7], 7);
		bitClear(Mbus._buf[7], 6);
		bitClear(Mbus._buf[10], 7);
		bitClear(Mbus._buf[10], 6);
		val.S = copy3bytes(2, 3, 1);
		val.A = copy3bytes(5, 6, 4);
		val.B = copy3bytes(8, 9, 7);
		val.C = copy3bytes(11, 12, 10);
//		Mbus.StatusMClear(MODBUS_MASTER::MB_MASTER_raw_to_int);
#if defined(MEGUNO) && defined(DEBUG_OUT)
		MG_HideControl(F("values_43"));
#endif
	}
	else
	{
		val.A = 0xFFFFFFFF;
		val.B = 0xFFFFFFFF;
		val.C = 0xFFFFFFFF;
		val.S = 0xFFFFFFFF;
		Mbus.StatusMSet(MODBUS_MASTER::MB_MASTER_raw_to_int);
		DEBUG_OUT << F(">> raw_to_int set\n");
#if defined(MEGUNO) && defined(DEBUG_OUT)
		Mbus.DumpMBMStatus();
		MG_ShowControl(F("values_43"));
#endif
	}
	return val;
}

//*********************************************************************************
MR_ENERGY_STRUCT Mercury::get_values_44(const uint8_t *p)
//*********************************************************************************
{
#if defined (DEBUG_TRACE) && defined(MR_DEBUG_TRACE)
	MG_TraceBegin( F("get_values_44"));
#endif
	Mbus._buf_send = (uint8_t*)p;
	//для разных вариантов запросов энергии - разые длины ответов
	uint8_t len = 19;
	if (Mbus._buf_send[2] == MR_ENP_ABC)
	{
		len = 15;
	}
	Mbus._buf_i_send = 6;	//указатель - на последний байт в командной последовательности
	//	Mbus.dump_buf_send();
//	Mbus.write();
//	Mbus.StatusPSet(Mbus.MB_MASTER_param_MERCURY);
//	SetHandler();
//	Mbus.response(MR_TIMEOUT);
	response();
	//	Mbus.dump_buf();
	Mbus.StatusPClear(Mbus.MB_MASTER_param_MERCURY);
//#if defined(MEGUNO_MBUS_DEBUG) && defined(DEBUG_OUT)
//	Mbus.DumpMBMStatus();
//	Mbus.DumpMBMparam();
//#endif
	MR_ENERGY_STRUCT val;
	if (Mbus._status_master == 0)
	{
		val.APLUS = copy4bytes(1);
		val.AMINUS = copy4bytes(5);
		val.RPLUS = copy4bytes(9);
		if (Mbus._buf_send[2] != MR_ENP_ABC)
			val.RMINUS = copy4bytes(13);
		else
			//	val.RMINUS=0xFFFFFFFF;
			val.RMINUS = 0;
		val._TimeStamp = now();
	//	Mbus.StatusMClear(MODBUS_MASTER::MB_MASTER_raw_to_int);
#if defined(MEGUNO) && defined(DEBUG_OUT)
		MG_HideControl(F("values_44"));
#endif
	}
	else
	{
		//		DEBUG_OUT << F("**get_values_44 ERROR\n");
#if defined(MEGUNO) && defined(DEBUG_OUT)
		MG_ShowControl(F("values_44"));
#endif
		//		STOP
		val.APLUS = 0xFFFFFFFF;
		val.AMINUS = 0xFFFFFFFF;
		val.RPLUS = 0xFFFFFFFF;
		val.RMINUS = 0xFFFFFFFF;
		Mbus.StatusMSet(MODBUS_MASTER::MB_MASTER_raw_to_int);
		DEBUG_OUT << F(">> raw_to_int set\n");

//#if defined(MEGUNO) && defined(DEBUG_OUT)
//		Mbus.DumpMBMStatus();
//#endif
	}
#if defined (DEBUG_TRACE) && defined(MR_DEBUG_TRACE)
	MG_TraceEnd();
#endif
	return val;
}



//*********************************************************************************
MR_current_value Mercury::get_values()
//*********************************************************************************
{
	MR_current_value __values;
	uint8_t buf_send[6];
	Mbus._buf_send = buf_send;
	memset((void*)&__values, 0xFF, sizeof(MR_current_value));

	__values._V = get_values_33(MR_VALV);
	if (Mbus._status_master != 0) goto lab_exit;
	__values._I = get_values_33(MR_VALI);
	if (Mbus._status_master != 0) goto lab_exit;
	__values._POW = get_values_43(MR_VAL_POWERP);
	if (Mbus._status_master != 0) goto lab_exit;
	__values._Q = get_values_43(MR_VAL_POWERQ);
	if (Mbus._status_master != 0) goto lab_exit;
	__values._SPOW = get_values_43(MR_VAL_POWERS);
	if (Mbus._status_master != 0) goto lab_exit;
	__values._COSF = get_values_43(MR_VAL_COSF);
	if (Mbus._status_master != 0) goto lab_exit;
	response(MR_VALF);
	if (Mbus._status_master != 0) goto lab_exit;
	__values._F = copy3bytes(2, 3, 1);
	__values._TimeStamp = now();
lab_exit:	return __values;

}

//*********************************************************************************
uint8_t  Mercury::get_values(MR_current_value* __values)
//*********************************************************************************
{
//	MR_current_value __values;
	uint8_t buf_send[6];
	Mbus._buf_send = buf_send;
	memset((void*)&__values, 0xFF, sizeof(MR_current_value));

	__values->_V = get_values_33(MR_VALV);
	__values->_I = get_values_33(MR_VALI);
	__values->_POW = get_values_43(MR_VAL_POWERP);
	__values->_Q = get_values_43(MR_VAL_POWERQ);
	__values->_SPOW = get_values_43(MR_VAL_POWERS);
	__values->_COSF = get_values_43(MR_VAL_COSF);
	response(MR_VALF);
	__values->_F = copy3bytes(2, 3, 1);
	__values->_TimeStamp = now();
	return 0;

}

#endif

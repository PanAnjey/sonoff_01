//***************************************************************
// Функции работы с MODBUS
//***************************************************************
#pragma once
#ifndef my_MBUS_BASE_h
#define my_MBUS_BASE_h
//#include <Streaming.h>
//#ifndef __ESP8266_ESp8266__
//#include <Flash.h>
//#endif
//#include <MY_MODBUS_DEF.h>
//#ifndef DEBUG_OUT
//#define DEBUG_OUT Serial
//#endif
//#define MEGUNO_MBUS_DEBUG
//#ifndef MEGUNO
//#undef MEGUNO_MBUS_DEBUG
//#endif
//
//#if defined(__AVR_ATmega328p__)
//#include <NewSoftSerial.h>
//#define DefSerial NewSoftSerial 
//#else
//#define DefSerial HardwareSerial 
//
//#endif

//#define MB_BASE_ECHO		//���� ���� "���"

//#define MBASE_DEBUG	

#define MAX_RESPONSE_LENGTH 250
#define MB_TIMEOUT 30
#define MB_BASE_status_Mask 0xFF7F

class MODBUS_BASE
{

public:
	enum MB_enum 
	{						//���� ������
		RX_NOT_FINISHED = 0,      // not finished yet
		RX_FINISHED,
		RX_TMOUT_ERR
	};
	enum MB_BASE_status
	{
		//	MB_OK,
		MB_ERROR_TO_BUF,
		MB_ERROR_GET_FROM_BUF,
		MB_ERROR_TIMEOUT,
		MB_ERROR_BIG,
		MB_ERROR_LEN,
		MB_ERROR_CRC,
		//	MB_ERROR_CRC2,
		MB_ERROR_ADR,
		//		MB_DEBUG,
		MB_ERROR_CMD
	};

	enum MB_BASE_inout
	{
		MB_IN = 0,
		MB_OUT
	};

	uint8_t _dev_adr;				//адрес устройства на шине
	uint8_t _cmd;					//код команды
	uint16_t _size;					//размер буфера
	uint8_t * _buf;					//указатель на буфер приема
	uint8_t *_buf_send;				//указатель на буфер отправки
	uint16_t _buf_i;				//указатель положения бувера	
	uint16_t _buf_i_send;
	uint16_t _status_base;			//слово состояния


	uint16_t _crc;					//хранит вычисленную crc
	uint32_t _tmout;				//интервал ожидания ответа
	//	uint8_t _count;
	uint8_t _repeat;				//число повторений при неудаче
//	uint8_t _INTPin;
	uint8_t _REPin;
//#ifndef ARDUINO_ESP8266_NODEMCU
//	uint8_t _DEPin;
//
//	typedef void(*RunExternalProc)();
//	RunExternalProc _Func_NORESP;			// процедура запускаемая при наступлении таймаута
//	RunExternalProc _Func_BADCRC;			// процедура запускаемая при несовпадении CRC
//	RunExternalProc _Func_ERR;				// процедура запускаемая при других ошибках	
//#endif
	void begin(uint8_t remote_adr, uint8_t len, uint8_t *buf, uint8_t RE_Pin);
	//	void begin();
	void raw_to_int(uint16_t *dest, uint8_t dest_size, uint8_t start_pos);							//��������� �� ������ ������ ������������ ��������
	uint8_t req_get_reg_hi() { return _buf[2]; };
	uint8_t req_get_reg_lo() { return _buf[3]; };
	uint16_t req_get_reg();
	void crc_chk();
	void crc_calc(uint8_t len);		//вычисляет CRC
	uint16_t  crc_calc(uint8_t *_ptr, uint8_t len);
	inline void	crc_inject() { _buf_send[_buf_i_send++] = _crc >> 8; _buf_send[_buf_i_send++] = _crc & 0x00ff; }
	inline void	crc_inject(uint16_t _CRC_) { _buf_send[_buf_i_send++] = _CRC_ >> 8; _buf_send[_buf_i_send++] = _CRC_ & 0x00ff; }
	void set_crc();					//вставка crc
//#ifndef ARDUINO_ESP8266_NODEMCU
//	inline void  SET_NORESP(void(*f_func)()) { _Func_NORESP = f_func; };
//	inline void  SET_BADCRC(void(*f_func)()) { _Func_BADCRC = f_func; };
//	inline void  SET_ERR(void(*f_func)()) { _Func_ERR = f_func; };
//	void set_buf_send_fromFLASH(_FLASH_ARRAY<uint8_t> dat);
//#endif
	//	void read(uint8_t len, uint16_t timeout);
	void read(uint16_t timeout);

	//	uint8_t read(uint16_t start_comm_tmout, uint16_t max_interchar_tmout);
	//	uint8_t IsRxFinished(void);
	void write();					//вывод в линию
	void write_ext();
	void init_buf(uint8_t len);		//забой буфера
	void enable();					//разрешение записи в линию
	void disable();
	void get_from_buf(uint8_t *val, uint8_t dest_size);		
	void insert(uint8_t *val, uint8_t dest_size);	
	inline void insert(uint8_t val) { _buf_send[_buf_i_send++] = val; }
	//		void insert(uint16_t val);
	inline void insert(uint16_t val) { _buf_send[_buf_i_send++] = val >> 8; _buf_send[_buf_i_send++] = val & 0x00ff; }
	//	void insert_cmd_read(uint8_t dev_adr, uint16_t reg, uint16_t col_reg);
	void insert_cmd(uint8_t dev_adr, uint8_t cmd);
	void insert_cmd(uint8_t dev_adr, uint8_t cmd, uint8_t param);
	void insert_cmd(uint8_t dev_adr, uint8_t cmd, uint16_t param);
	void insert_cmd(uint8_t dev_adr, uint8_t cmd, uint8_t param1, uint8_t param2);
	void dump_buf();
	void dump_buf_send();
	void swap16(uint16_t* val);
//#if defined(MEGUNO_MBUS_DEBUG) && defined(DEBUG_OUT)
//	void  DumpMBStatus();
//#endif
	inline void StatusSet(MB_BASE_status Nbit) { bitSet(_status_base, Nbit); };
	inline uint8_t StatusChk(MB_BASE_status Nbit) { return bitRead(_status_base, Nbit); };
	inline 	void StatusClear(MB_BASE_status Nbit) { bitClear(_status_base, Nbit); };
};

////*********************************************************************************
//#if defined(MEGUNO_MBUS_DEBUG) && defined(DEBUG_OUT)
//void  MODBUS_BASE::DumpMBStatus()
////*********************************************************************************
//{
//	for (uint16_t i = 0; i < 9; i++)
//	{
//		if (bitRead(_status_base, i))
//			DEBUG_OUT << F("{UI:") << F("|SET|MBS") << i << F(".Visible=True}\n");
//		else
//			DEBUG_OUT << F("{UI:") << F("|SET|MBS") << i << F(".Visible=False}\n");
//	}
//}
//#endif

//*********************************************************************************
void MODBUS_BASE::raw_to_int(uint16_t *dest, uint8_t count, uint8_t start_pos)
//*********************************************************************************
{
	uint16_t temp, i;
	uint8_t j = start_pos;
	for (i = 0; i < count; i++)
	{
		temp = _buf[j++] << 8;
		//		DEBUG_OUT << "temp:" << _HEX(temp);
		temp = temp | _buf[j++];
		//		DEBUG_OUT << "temp:" << _HEX(temp);
		//	swap16(&temp);
		dest[i] = temp;
		//	DEBUG_OUT<<" i="<<i<< " buf["<<start_pos + i * 2<<"]="<<_HEX(_buf[start_pos + i * 2])\
										<<" buf["<<start_pos +1+ i * 2<<"]="<<_HEX(_buf[start_pos+1 + i * 2])<<" data="<<dest[i] <<"\n";

	}
}
//*********************************************************************************
void MODBUS_BASE::swap16(uint16_t* val)
//*********************************************************************************
{
	uint16_t l = *val >> 8;
	*val = (*val << 8) | l;
	*val &= 0xFFFF;
}
//*********************************************************************************
void MODBUS_BASE::enable()
//*********************************************************************************
{
	digitalWrite(_REPin, HIGH);
	digitalWrite(LED_BUILTIN, LOW);
//#ifndef ARDUINO_ESP8266_NODEMCU
//	digitalWrite(_DEPin, HIGH);
//#endif
	delay(20);
}

//*********************************************************************************
void MODBUS_BASE::disable()
//*********************************************************************************
{

	digitalWrite(_REPin, LOW);
	digitalWrite(LED_BUILTIN, HIGH);
//#ifndef ARDUINO_ESP8266_NODEMCU
//	digitalWrite(_DEPin, LOW);
//#endif
	delay(20);
}

//*********************************************************************************
void MODBUS_BASE::begin(uint8_t counter_adr, uint8_t len, uint8_t *buf, uint8_t RE_Pin)
//*********************************************************************************
{
	//	DEBUG_OUT<<" MODBUS_BASE::begin\n";
	_dev_adr = counter_adr;
	_buf = buf;
	_buf_send = buf;
	_size = len;
	_buf_i = 0;
	_buf_i_send = 0;
	_status_base = 0;
	_repeat = 3;
//	_INTPin = 0xFF;
	_REPin = RE_Pin;
	pinMode(_REPin, OUTPUT);
//#ifndef ARDUINO_ESP8266_NODEMCU
//	_DEPin = DE_Pin;
//	pinMode(_DEPin, OUTPUT);
//	_Func_NORESP = NULL;
//	_Func_ERR = NULL;
//	_Func_BADCRC = NULL;
//#endif
	_tmout = MB_TIMEOUT;
	disable();


}

//*********************************************************************************
void MODBUS_BASE::init_buf(uint8_t len)
//*********************************************************************************
{
	for (int i = 0; i < len; i++)
	{
		_buf[i] = 0x00;
	}
	_buf_i = 0;
}

//****************************************************************************
//  CRC Modbus polinom
void  MODBUS_BASE::crc_calc(uint8_t len)
//****************************************************************************
{
	uint16_t i, j;
	uint16_t k, l;
	k = 0xFFFF;
	for (i = 0; i < len; i++)
	{
		k = k ^ _buf[i];
		for (j = 1; j <= 8; j++)
		{
			l = k & 0x0001;
			k = k >> 1;
			if (l)
				k = k ^ 0xA001;
		}
	}
	l = k >> 8;
	k = (k << 8) | l;
	k &= 0xFFFF;
	_crc = k;
}
//****************************************************************************
//  CRC Modbus polinom
uint16_t  MODBUS_BASE::crc_calc(uint8_t *_ptr, uint8_t len)
//****************************************************************************
{
	uint16_t i, j;
	uint16_t k, l;
	k = 0xFFFF;
	for (i = 0; i < len; i++)
	{
		k = k ^ _ptr[i];
		for (j = 1; j <= 8; j++) {
			l = k & 0x0001;
			k = k >> 1;
			if (l)
				k = k ^ 0xA001;
		}
	}
	l = k >> 8;
	k = (k << 8) | l;
	k &= 0xFFFF;
	return k;
}
//#ifndef ARDUINO_ESP8266_NODEMCU
////***********************************************************************************************
//void MODBUS_BASE::set_buf_send_fromFLASH(_FLASH_ARRAY<uint8_t> dat)
////заполнение исходящего буфера данными из FLASH
////***********************************************************************************************
//{
//	_buf_i_send = dat.count();
//	for (uint8_t i = 0; i < _buf_i_send; i++)
//	{
//		_buf_send[i] = dat[i];
//	}
//}
//#endif
//*********************************************************************************
void MODBUS_BASE::read(uint16_t timeout)
//*********************************************************************************
{
	uint8_t	num_of_bytes = 0;
	_status_base &= 0x80;
	_buf_i = 0;
	uint8_t _count = 0;
	uint8_t inByte;
	uint32_t _tmout;
lab1:
	if (num_of_bytes == 0)
	{
		_tmout = millis() + timeout;
	}
	else
	{
		_tmout = millis() + 30;
	}
	while (millis() < _tmout)
	{
		if (RS485.available())goto lab2;
	}
	if (_buf_i == 0)
	{
#ifdef USE_DEBUG_MBUS
		DEBUG_OUT.print(F("Mbus READ TIMEOUT. NO DATA\n"));
#endif
		StatusSet(MB_ERROR_TIMEOUT);
		return;
	}
	else
	{
		crc_chk();
#ifdef USE_DEBUG_MBUS
		DEBUG_OUT.print(F("\n>> READ p1\n"));
		DEBUG_OUT.print(("READ:: _size="));
		DEBUG_OUT.print(_size);
		DEBUG_OUT.print(F(" _buf_i="));
		DEBUG_OUT.println(_buf_i);
		DEBUG_OUT.print(F("num_of_bytes="));
		DEBUG_OUT.print(num_of_bytes);
		DEBUG_OUT.print(F("inByte="));
		DEBUG_OUT.print(inByte, HEX);
		DEBUG_OUT.print(F("\ncount:"));
		DEBUG_OUT.println(_count);
#endif
		return;
	}

lab2:
	inByte = RS485.read();
	_count++;
	if (num_of_bytes == 0)
	{
		if (inByte == _dev_adr)
		{
			num_of_bytes = 1;
			_buf[_buf_i] = inByte;
			_buf_i++;
		}
		goto lab1;
	}
	else
	{
		if ((_buf_i < _size))					//���� ���� �����
		{
			_buf[_buf_i] = inByte;
			_buf_i++;
		}
		goto lab1;
	}

	//	DEBUG_OUT << F("READ:: _size=") << _size << F(" _buf_i=") << _buf_i;
	//	DEBUG_OUT << F("num_of_bytes=") << num_of_bytes << F("inByte=") << _HEX(inByte) << F("\ncount:")<< _count<<endol;

}
//*********************************************************************************
void MODBUS_BASE::write_ext()
//*********************************************************************************
{
	_dev_adr = _buf_send[0];
	_cmd = _buf_send[1];
	//while (RS485.available())
	//{
	//	int inByte = RS485.read();
	//	//		DEBUG_OUT << F("in write Read2:") << _HEX(inByte) << F("_Buf_i=") << _buf_i <<  "\n";
	//}

	enable();
	//if (_dev_adr == RAR_ETHERNET)
	//{
	//	RS485.write('*');
	//	RS485.flush();
	//	delay(30);
	//}

	for (int i = 0; i < _buf_i_send; i++)
	{
		RS485.write(_buf_send[i]);
	}
	//	DEBUG_OUT << endol;
		delay(20);
		RS485.flush();
	disable();
}

//*********************************************************************************
void MODBUS_BASE::write()
//*********************************************************************************
{
	//
	//#if defined(MBASE_DEBUG)
	//	if (StatusChk(MB_DEBUG) == 1)
	//	{
	//	DEBUG_OUT << "WRITE::";
	//	dump_buf_send();
	//StatusClear(MB_DEBUG);
	//	}
	//#endif
	/*		while (RS485.available())
	{
	int inByte = RS485.read();
	//			DEBUG_OUT << F("in write Read1:") << _HEX(inByte) << F("_Buf_i=") << _buf_i << "\n";
	}*/
	_dev_adr = _buf_send[0];
	_cmd = _buf_send[1];
	while (RS485.available())
	{
		int inByte = RS485.read();
		//		DEBUG_OUT << F("in write Read2:") << _HEX(inByte) << F("_Buf_i=") << _buf_i <<  "\n";
	}
	enable();
	//if (_dev_adr == RAR_ETHERNET)
	//{
	//	RS485.write('*');
	//	RS485.flush();
	//	delay(30);
	//}
	for (int i = 0; i < _buf_i_send; i++)
	{
		RS485.write(_buf_send[i]);
	}
	//	DEBUG_OUT << endol;
	RS485.flush();
	//	delay(20);
	disable();
}

//*********************************************************************************
void MODBUS_BASE::set_crc()
//*********************************************************************************
{
	_crc = crc_calc(_buf_send, _buf_i_send);
	crc_inject();
	//	write();	//перенесено в respnce
}

//*********************************************************************************
void MODBUS_BASE::get_from_buf(uint8_t *val, uint8_t dest_size)
//*********************************************************************************
{
	//	_buf_i=3;
	if ((_buf_i + dest_size) > (_size - 2))
	{
		StatusSet(MB_ERROR_TO_BUF);
		//	_status=MB_ERROR_GET_FROM_BUF;
		return;
	}

	for (int i = 0; i < dest_size; i++)
	{
		val[i] = _buf[_buf_i++];
	}
}

//*********************************************************************************
void MODBUS_BASE::insert(uint8_t *val, uint8_t dest_size)
//*********************************************************************************
{
	//	DEBUG_OUT<<"insert dest_size="<<dest_size<<"\n";
	if ((_buf_i_send + dest_size) > (MAX_RESPONSE_LENGTH - 2))
	{
		StatusSet(MB_ERROR_TO_BUF);
		return;
	}
	for (int i = 0; i < dest_size; i++)
	{
		_buf_send[_buf_i_send++] = val[i];
	}
}
//*********************************************************************************
void MODBUS_BASE::insert_cmd(uint8_t dev_adr, uint8_t cmd)
//*********************************************************************************
{
	_buf_i_send = 0;
	insert(dev_adr);
	insert(cmd);
}
//*********************************************************************************
void MODBUS_BASE::insert_cmd(uint8_t dev_adr, uint8_t cmd, uint8_t param)
//*********************************************************************************
{
	insert_cmd(dev_adr, cmd);
	insert(param);
}
//*********************************************************************************
void MODBUS_BASE::insert_cmd(uint8_t dev_adr, uint8_t cmd, uint16_t param)
//*********************************************************************************
{
	insert_cmd(dev_adr, cmd);
	insert(param);
}
//*********************************************************************************
void MODBUS_BASE::insert_cmd(uint8_t dev_adr, uint8_t cmd, uint8_t param1, uint8_t param2)
//*********************************************************************************
{
	insert_cmd(dev_adr, cmd, param1);
	insert(param2);
}

//*********************************************************************************
void MODBUS_BASE::dump_buf()
//*********************************************************************************
{
#ifdef USE_DEBUG_MBUS
	DEBUG_OUT.println("BUF:");
	DEBUG_OUT.print(F(" _dev_adr = "));
	DEBUG_OUT.print(_buf[0], HEX);
	DEBUG_OUT.print(F(" _cmd ="));
	DEBUG_OUT.print(_buf[1], HEX);
	DEBUG_OUT.print("_buf_i=");
	DEBUG_OUT.println(_buf_i);
	for (int i = 0; i < _buf_i; i++)
	{
		DEBUG_OUT.print("i=");
		DEBUG_OUT.print(i);
		DEBUG_OUT.print(' ');
		if (_buf[i] < 16)
			DEBUG_OUT.print('0');
		DEBUG_OUT.print(_buf[i], HEX);
		DEBUG_OUT.print(':');
		DEBUG_OUT.println(_buf[i]);
	}
	DEBUG_OUT.println();
#endif
}
//*********************************************************************************
void MODBUS_BASE::dump_buf_send()
//*********************************************************************************
{
#ifdef USE_DEBUG_MBUS
	DEBUG_OUT.println("Buf SEND:");
	DEBUG_OUT.print(F(" _dev_adr = "));
	DEBUG_OUT.print(_buf_send[0], HEX);
	DEBUG_OUT.print(F(" _cmd ="));
	DEBUG_OUT.print(_buf_send[1], HEX);
	DEBUG_OUT.print("_buf_i=");
	DEBUG_OUT.println(_buf_i_send);
	for (int i = 0; i < _buf_i_send; i++)
	{
		DEBUG_OUT.print("i=");
		DEBUG_OUT.print(i);
		DEBUG_OUT.print(' ');
		if (_buf_send[i] < 16)
			DEBUG_OUT.print('0');
		DEBUG_OUT.print(_buf_send[i], HEX);
		DEBUG_OUT.print(':');
		DEBUG_OUT.println(_buf_send[i]);
	}
	DEBUG_OUT.println();
#endif
}

//*********************************************************************************
void MODBUS_BASE::crc_chk()
{
	StatusClear(MB_ERROR_CRC);
	crc_calc(_buf_i - 2);
	uint16_t old_crc = _buf[_buf_i - 2];
	old_crc = old_crc << 8;
	old_crc = old_crc | _buf[_buf_i - 1];
	if (old_crc != _crc)
	{
//#ifndef ARDUINO_ESP8266_NODEMCU
//		if (_Func_BADCRC != NULL)
//		{
//			(*_Func_BADCRC)();
//			_Func_BADCRC = NULL;
//		}
//		else
//#endif
			StatusSet(MB_ERROR_CRC);
	}
}

//*********************************************************************************
uint16_t MODBUS_BASE::req_get_reg()

//*********************************************************************************
{
	uint16_t temp;
	temp = _buf[2] << 8;
	temp = temp | _buf[3];
	//	highByte(temp);
	return temp;
}

#endif

#ifndef my_MBUS_def_h
#define my_MBUS_def_h

//************************   MODBUS*****************************************
const uint8_t  MB_CMD_READ =0x03;		//������ ���������
const uint8_t  MB_CMD_WRITE= 0x06;		//������ ������ ��������
const uint8_t  MB_CMD_DIAG = 0x08;		//�����������
const uint8_t  MB_CMD_MWRITE = 0x10;	//������ ���������� ���������

#define MAX_RESPONSE_LENGTH 250
#define MB_TIMEOUT 30

#define MB_BASE_status_Mask 0xFF7F

//исп. для изменения параметров OM-310
//struct CHANGE_PARAM_STRUCT
//{
//	uint8_t		_PREV;		//признак предыдущей операции 
//							//=0 - изменений OM-310 не требуется
//							//0xFF - предудущая операция с Серверм закончилась ошибкой
//	uint16_t	_param_adr;	//адрес изменяемого параметра
//	uint16_t	_param_value;//новое значение
//	uint16_t	_ID;		//идентификатор события на сервере. 
//};

//struct EXT_RESPONSE_STRUCT
//{
//	uint8_t		_dev_adr;	//адрес устройства
//	uint8_t		_cmd;		//команда
//	uint8_t		_STATUS;	//0x55 - все хорошо
//	CHANGE_PARAM_STRUCT _change_param;	//в случае если от сервера идет команда на изменение параметров OM-310
//	uint16_t	_CRC;		//контрольная сумма
//};


//struct DEVICE_DATA_SEND_STRUCT
//{
//	uint8_t		_dev_adr;	//0-�����
//	uint8_t		_cmd;		//1-��� �������		=MB_CMD_MWRITE
//	uint16_t	_reg;		//2,3-����� ��������	=RAR_DA
//	uint16_t	_dev_cnt;	//4,5-����� ���������	=���-�� ��������
//	uint8_t		_len;		//6-����� ����
//	unsigned long _timestamp;
//	uint8_t		_val;		//7-��������   �� ��� ����� ������ ������ � ��������
//};
//
//struct DEVICE_REMOTE_DATA_SEND_STRUCT
//{
//	uint8_t	_echoadr;
//	uint8_t	_echocmd;
//	uint8_t	_echolen;
//	DEVICE_DATA_SEND_STRUCT _SEND_DATA;
//};

enum RAR_REG_R
{
	RAR_STATUS = 0x10,	//���������
	RAR_DA_STAT = 0x20,	//����� �������� Dallas
	RAR_DA = 0x21,		//������� ��������� Dallas
	RAR_SHT_STAT = 0x30,	//����� �������� ���������
	RAR_SHT = 0x31,		//������� ��������� ���������
	//	RAR_SHT_T,			//������� �����������
	//	RAR_SHT_P,			//���
	RAR_TIME = 0x40,
	RAR_MERC_VAL = 0x50,
	RAR_MERC_EN = 0x60,
	RAR_MERC_YA = 0x61,
	RAR_MERC_LYA = 0x62,
	RAR_MERC_ED = 0x63,
	RAR_MERC_LD = 0x64,
	RAR_MERC_ABC = 0x65,
	RAR_MERC_LOG = 0x52,
	RAR_MERC_LOG_LAST = 0x53,
	RAR_OM_PARAM = 0x70
};

struct BUF_CMD_SEND_03
{
	uint8_t		_dev_adr;	//�����
	uint8_t		_cmd;		//��� �������
	uint16_t	_reg;		//����� ��������
	uint16_t	_reg_cnt;	//����� ���������
	uint16_t	_crc;
};

struct BUF_CMD_READ_SINGLE
{
	uint8_t		_dev_adr;	//�����
	uint8_t		_cmd;		//��� �������;
	uint8_t		_len;		//����� ����	������ ���� ����� 2
	uint16_t	_val;		//��������
	uint16_t	_crc;		//����������� �����
};

struct BUF_CMD_SEND_COUNTER
{
	uint8_t		_dev_adr;	//�����
	uint8_t		_cmd;		//��� �������
	uint16_t	_reg;		//����� ��������
	uint16_t	_reg_cnt;	//����� ���������
	uint8_t		_len;		//����� ����
	uint16_t	_value;		//��������
	uint16_t	_crc;		//����������� �����
};


#endif


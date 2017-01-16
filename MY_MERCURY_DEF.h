#ifndef _MY_MERCURY_DEF_
#define _MY_MERCURY_DEF_

#define MERCURY_addr 43

//#include <Arduino.h>


#define RAR_MERC_LOG_RECORDS 100
struct LOG_REC
{
	unsigned long ON;
	unsigned long OFF;
};

struct LOG_REC1
{
	unsigned long ON;
};
struct send_data
{
	uint8_t len;
	uint8_t data;
};

//typedef LOG_REC LOG_REC20[20];

typedef uint8_t password[6];

typedef unsigned long val4[4];
typedef unsigned long val3[3];
//#pragma push() // запоминаем текущее выравнивание
//#pragma pack(1)

struct MR_ENERGY_STRUCT
{
	uint8_t EN_TYPE;
	unsigned long APLUS;
	unsigned long AMINUS;
	unsigned long RPLUS;
	unsigned long RMINUS;
	unsigned long _TimeStamp;
	//	uint32_t AC;*/
}__attribute__((__packed__));



struct MR_VAL4
{

	unsigned long A;
	unsigned long B;
	unsigned long C;
	unsigned long S;
};
struct MR_VAL3
{

	unsigned long A;
	unsigned long B;
	unsigned long C;
};
struct MR_VAL1
{

	unsigned long S;

};

struct MR_current_value
{
	MR_VAL4 _POW;//мощность
	MR_VAL4 _Q;
	MR_VAL4 _SPOW;
	MR_VAL4 _COSF;		//коэф.мощности
	MR_VAL3 _V;			//напряжение
	MR_VAL3 _I;			//ток
	//	MR_VAL3 _A;			//углы
	unsigned long _F;			//частота
	unsigned long _TimeStamp;
	//	uint8_t chk;		//для контроля
}__attribute__((__packed__));

struct EXT_VALUES
{
	uint8_t		_dev_adr;	//0-адрес
	uint8_t		_cmd;		//1-код команды		=MB_CMD_MWRITE
	uint16_t	_reg;		//2,3-номер регистра	=RAR_DA
	uint16_t	_dev_cnt;	//4,5-число регистров	=кол-во датчиков
	uint8_t		_len;		//6-число байт
	MR_current_value VALUES;
	uint16_t _crc;
}__attribute__((__packed__));
struct EXT_ENERGY
{
	uint8_t		_dev_adr;	//0-адрес
	uint8_t		_cmd;		//1-код команды		=MB_CMD_MWRITE
	uint16_t	_reg;		//2,3-номер регистра	=RAR_DA
	uint16_t	_dev_cnt;	//4,5-число регистров	=кол-во датчиков
	uint8_t		_len;		//6-число байт
	MR_ENERGY_STRUCT ED2;	//данные по энергии
	uint16_t _crc;
}__attribute__((__packed__));

struct EXT_LOG_10_1
{
	uint8_t		_dev_adr;	//0-адрес
	uint8_t		_cmd;		//1-код команды		=MB_CMD_MWRITE
	uint16_t	_reg;		//2,3-номер регистра	=RAR_DA
	uint16_t	_dev_cnt;	//4,5-число регистров	=кол-во датчиков
	uint8_t		_len;		//6-число байт
	uint8_t		_LOG_TYP;
	uint8_t		_LAST_REC;
	LOG_REC1	_LOG[10];
	uint16_t	_crc;
}__attribute__((__packed__));
struct EXT_LOG_10_2
{
	uint8_t		_dev_adr;	//0-адрес
	uint8_t		_cmd;		//1-код команды		=MB_CMD_MWRITE
	uint16_t	_reg;		//2,3-номер регистра	=RAR_DA
	uint16_t	_dev_cnt;	//4,5-число регистров	=кол-во датчиков
	uint8_t		_len;		//6-число байт
	uint8_t		_LOG_TYP;
	uint8_t		_LAST_REC;
	LOG_REC		_LOG[10];
	uint16_t	_crc;
}__attribute__((__packed__));

struct EXT_LOG_20
{
	uint8_t		_dev_adr;	//0-адрес
	uint8_t		_cmd;		//1-код команды		=MB_CMD_MWRITE
	uint16_t	_reg;		//2,3-номер регистра	=RAR_DA
	uint16_t	_dev_cnt;	//4,5-число регистров	=кол-во датчиков
	uint8_t		_len;		//6-число байт
	uint8_t		_LOG_TYP;
	uint8_t		_LAST_REC;
	LOG_REC		_LOG[20];
	uint16_t	_crc;
}__attribute__((__packed__));

struct LOG_LASTREC
{
	uint8_t _LOG_TYP;
	uint8_t _Event_ID;
};
//#pragma pop()
/*
struct MR_EN_value
{
MR_ENERGY_STRUCT 	TOT;							//Чтение накопленной энергии от сброса.
MR_ENERGY_STRUCT 	YA;								//Чтение накопленной энергии за год.
MR_ENERGY_STRUCT 	LYA;							//Чтение накопленной энергии за предыдущий год.
MR_ENERGY_STRUCT 	ME;								//Чтение накопленной энергии за месяц.
MR_ENERGY_STRUCT 	ED;								//Чтение накопленной энергии за текущие сутки.
MR_ENERGY_STRUCT 	LD;								//Чтение накопленной энергии за предыдущие сутки.
MR_ENERGY_STRUCT 	ABC;							//Чтение пофазных значений накопленной активной энергии прямого направления
//		unsigned long _TimeStamp;

};
*/
enum MR_CMD
{
	MRCMD_CHAN_CHK = 0x00,
	MRCMD_CHAN_OPEN = 0x01,
	MRCMD_CHAN_CLOSE = 0x02,
	MRCMD_WRITE_PARAM = 0x03,
	MRCMD_READ_TIMES = 0x04,
	MRCMD_GET_ENERGY = 0x05,
	MRCMD_GET_PARAM = 0x08,
	MRCMD_SUB_P2 = 0x16
};

/*
const PROGMEM uint8_t	MRCMD_CHAN_CHK=0x00;
const PROGMEM uint8_t	MRCMD_CHAN_OPEN=0x01;
const PROGMEM uint8_t	MRCMD_CHAN_CLOSE=0x02;
const PROGMEM uint8_t   MRCMD_WRITE_PARAM=0x03;

const PROGMEM uint8_t	MRCMD_READ_TIMES=0x04;
const PROGMEM uint8_t	MRCMD_GET_ENERGY=0x05;
const PROGMEM uint8_t	MRCMD_GET_PARAM=0x08;

*/
//Чтение вспомогательных параметров: мгновенной активной, реактивной, полной мощности, напряжения тока, коэффициента мощности и частоты 
//const PROGMEM uint8_t MR_WRPARAM_TCORRECT=0x0D;

enum MR_WRPARAM
{
	TimeSET = 0x0C,
	TimeCORRECT = 0x0D,
	EN_RESET = 0x20
};


enum RAR_MERC_LOG
{
	MR_CUR_TIME = 0,
	RAR_MERC_LOG_ONOFF = 0x01,				//Чтение времени включения/выключения прибора 
	//	RAR_MERC_LOG_COR_TIME=	0x02,				//Чтение времени коррекции часов прибора 
	RAR_MERC_LOG_ONOFF_A = 0x03,				//Чтение времени включения/выключения фазы 1 прибора 
	RAR_MERC_LOG_ONOFF_B = 0x04,				//Чтение времени включения/выключения фазы 2 прибора 
	RAR_MERC_LOG_ONOFF_C = 0x05,				//Чтение времени включения/выключения фазы 3 прибора 
	RAR_MERC_LOG_PWR_LIM = 0x06,				//Чтение времени начала/окончания превышения лимита мощности прибора 	

	RAR_MERC_LOG_COR_SCHED = 0x07,				//Чтение времени коррекции тарифного расписания
	//	RAR_MERC_LOG_COR_HOLIDAY= 0x08,				//Чтение времени коррекции расписания праздничных дней
	//	RAR_MERC_LOG_EN_RESET=	0x09,				//Чтение времени сброса регистров накопленной энергии
	//	RAR_MERC_LOG_AVRPOW_RESET=0x0A,				//Чтение времени инициализации массива средних мощностей
	//	RAR_MERC_LOG_EN_LIM_1=	0x0B,				//Чтение времени превышения лимита энергии по тарифу 1
	//	RAR_MERC_LOG_EN_LIM_2=	0x0C,				//Чтение времени превышения лимита энергии по тарифу 2
	//	RAR_MERC_LOG_EN_LIM_3=	0x0D,				//Чтение времени превышения лимита энергии по тарифу 3
	RAR_MERC_LOG_EN_LIM_4 = 0x0E,				//Чтение времени превышения лимита энергии по тарифу 4

	//	RAR_MERC_LOG_COR_POW_LIM=0xF,					//Чтение времени коррекции параметров контроля за превышением лимита мощности
	//	RAR_MERC_LOG_COR_EN_LIM=	0x10,					//Чтение времени коррекции параметров контроля за превышением лимита энергии 
	//	RAR_MERC_LOG_COR_LOSS=	0x11,					//Чтение времени коррекции параметров учета технических потерь

	//	RAR_MERC_LOG_OPENCLOSE=	0x12,					//Чтение времени вскрытия/закрытия прибора

	//	RAR_MERC_LOG_PROG=		0x13,					//Чтение времени и кода перепрограммирования прибора 
	//	RAR_MERC_LOG_STATUS=		0x14,					//Чтение времени и кода словосостояния прибора 

	//	RAR_MERC_LOG_COR_SHED_MIMA= 0x15,				//Чтение времени коррекции расписания утренних и вечерних максимумов мощности
	//	RAR_MERC_LOG_PWR_MAX_RESET= 0x16,				//Чтение времени сброса массива значений максимумов мощности

	RAR_MERC_LOG_V_MIN_A = 0x20,					//Чтение времени выхода/возврата за мин. предельно допустимое значение напряжения в фазе 1. 
	RAR_MERC_LOG_V_N_MIN_A = 0x21,					//Чтение времени выхода/возврата за мин. нормально допустимое значение напряжения в фазе 1. 
	RAR_MERC_LOG_V_N_MAX_A = 0x22,					//Чтение времени выхода/возврата за макс. нормально допустимое значение напряжения в фазе 1. 
	RAR_MERC_LOG_V_MAX_A = 0x23,					//Чтение времени выхода/возврата за макс. предельно допустимое значение напряжения в фазе 1. 
	RAR_MERC_LOG_V_MIN_B = 0x24,					//Чтение времени выхода/возврата за мин. предельно допустимое значение напряжения в фазе 2. 
	RAR_MERC_LOG_V_N_MIN_B = 0x25,					//Чтение времени выхода/возврата за мин. нормально допустимое значение напряжения в фазе 2. 
	RAR_MERC_LOG_V_N_MAX_B = 0x26,					//Чтение времени выхода/возврата за макс. нормально допустимое значение напряжения в фазе 2. 
	RAR_MERC_LOG_V_MAX_B = 0x27,					//Чтение времени выхода/возврата за макс. предельно допустимое значение напряжения в фазе 2. 
	RAR_MERC_LOG_V_MIN_C = 0x28,					//Чтение времени выхода/возврата за мин. предельно допустимое значение напряжения в фазе 3. 
	RAR_MERC_LOG_V_N_MIN_C = 0x29,					//Чтение времени выхода/возврата за мин. нормально допустимое значение напряжения в фазе 3. 
	RAR_MERC_LOG_V_N_MAX_C = 0x2A,					//Чтение времени выхода/возврата за макс. нормально допустимое значение напряжения в фазе 3. 
	RAR_MERC_LOG_V_MAX_C = 0x2B,					//Чтение времени выхода/возврата за макс. предельно допустимое значение напряжения в фазе 3. 
	RAR_MERC_LOG_F_MIN = 0x2C,					//Чтение времени выхода/возврата за мин. предельно допустимое значение частоты сети. 
	RAR_MERC_LOG_F_N_MIN = 0x2D,					//Чтение времени выхода/возврата за мин. нормально допустимое значение частоты сети. 
	RAR_MERC_LOG_F_N_MAX = 0x2E,					//Чтение времени выхода/возврата за макс. нормально допустимое значение частоты сети. 
	RAR_MERC_LOG_F_MAX = 0x2F					//Чтение времени выхода/возврата за макс. предельно допустимое значение частоты сети. 
};


enum MRREG_VALUES
{
	MR_VALUES_PWRP = 0x00,						//Чтение мгновенных значений активной мощности P(Вт) по сумме фаз
	MR_VALUES_PWRQ = 0x04,						//Чтение мгновенных значений реактивной мощности Q(Вар) по сумме фаз
	MR_VALUES_PWRS = 0x08,						//Чтение мгновенных значений полной мощности S(Ва) по сумме фаз
	MR_VALUES_V = 0x11,						//Чтение значения напряжения по фазе1
	MR_VALUES_I = 0x21,						//Чтение значения тока по фазе1
	MR_VALUES_COSF = 0x30,						//Чтение мгновенных значений cos fi  по сумме фаз
	MR_VALUES_F = 0x40,						//Чтение значения частоты
	MR_VALUES_A12 = 0x51,						//Угол
	MR_VALUES_A13 = 0x52,						//Угол
	MR_VALUES_A23 = 0x53						//Угол

};

enum MRREG_ENERGY
{
	MR_EN = 0x00,								//Чтение накопленной энергии от сброса.
	MR_EN_YA = 0x10,								//Чтение накопленной энергии за год.
	MR_EN_LYA = 0x20,							//Чтение накопленной энергии за предыдущий год.
	MR_EN_ME = 0x30,								//Чтение накопленной энергии за месяц.
	MR_EN_24 = 0x40,								//Чтение накопленной энергии за текущие сутки.
	MR_EN_L24 = 0x50,							//Чтение накопленной энергии за предыдущие сутки.
	MR_ENP_ABC = 0x60							//Чтение пофазных значений накопленной активной энергии прямого направления
};

#define MR_SERIAL 0x00						//Чтение серийного номера счетчика и даты выпуска
#define MR_KNKT 0x02						//Чтение коэффициента трансформации.
#define MR_VER 0x03							//Чтение версии ПО.
#define MR_ADR 0x05							//Чтение сетевого адреса.
#define MR_SUMMER 0x07						//Чтение значений времен перехода на летнее и зимнее время 
#define MR_VALUES_FLAG	 0x09				//Чтение программируемых флагов
#define MR_ST 0x0A							//Чтение байт состояния
#define MR_POS 0x0B 						//Чтение местоположения прибора.
#define MR_SHED_PWR_MIMA 0x0C 				//Чтение расписания утренних и вечерних максимумов мощности 
#define MR_PWR_MIMA 0x0D  					//Чтение значений утренних и вечерних максимумов мощности 

#define MR_P 0x11  							//Чтение вспомогательных параметров: мгновенной активной, реактивной, полной мощности, напряжения тока, коэффициента мощности и частоты 

#define MR_V_B 0x12							//Чтение значения напряжения по фазе2
#define MR_V_C 0x13							//Чтение значения напряжения по фазе3



#define MR_I_B 0x22						//Чтение значения тока по фазе2
#define MR_I_C 0x23						//Чтение значения тока по фазе3
//******************


#define MR_PWRP_A 0x01					//Чтение мгновенных значений активной мощности P по  фазе1
#define MR_PWRP_B 0x02					//Чтение мгновенных значений активной мощности P по  фазе2
#define MR_PWRP_C 0x03					//Чтение мгновенных значений активной мощности P по  фазе3
#define MR_PWRQ_A 0x05					//Чтение мгновенных значений реактивной мощности Q по  фазе1
#define MR_PWRQ_B 0x06					//Чтение мгновенных значений реактивной мощности Q по  фазе2
#define MR_PWRQ_C 0x07					//Чтение мгновенных значений реактивной  мощности Q по  фазе3
#define MR_PWRS_A 0x09					//Чтение мгновенных значений полной мощности S по  фазе1
#define MR_PWRS_B 0x0A					//Чтение мгновенных значений полной мощности S по  фазе2
#define MR_PWRS_C 0x0B					//Чтение мгновенных значений полной мощности S по  фазе3
#define MR_cosf_A 0x31					//Чтение мгновенных значений cos fi  по  фазе1
#define MR_cosf_B 0x32					//Чтение мгновенных значений cos fi  по  фазе2
#define MR_cosf_C 0x33					//Чтение мгновенных значений cos fi  по  фазе3

#define MR_VAR 0x12  						//Чтение варианта исполнения
#define MR_AVR_PWR 0x13  					//Чтение параметров последней записи средних мощностей 
#define MR_FIX 0x14  						//Чтение зафиксированных данных
#define MR_ST_TARIF 0x17  					//Чтение байта состояния тарификатора
#define MR_WORKLOAD 0x18 					//Чтение слова состояния управления нагрузкой
#define MR_PWR_LIM 0x19  					//Чтение лимита мощности
#define MR_EN_LIM 01A  						//Чтение лимита энергии по тарифу 1-4 
#define MR_IND1 0x1B  						//Чтение параметров индикации счетчика (по индицируемым тарифам) 
#define MR_IND2 0x1C  						//Чтение параметров индикации счетчика (по периодам индикации) 
#define MR_TIMEOUT 0x1D  					//Чтение множителя тайм-аута.
#define MR_LOS_PARAM  0x1E					//Чтение параметров режима учета технических потерь 
#define MR_LOS 0x1F  						//Чтение мощностей технических потерь 
#define MR_VALID 0x20 						//Чтение допустимых значений 
#define MR_AVR 0x21  						//Чтение значений времен усреднения










#endif
//********************************************************************************
//  OM-310 v0.1
//
//********************************************************************************
//  ��������� � Serial1

#ifndef my_OM310_DEV_h
#define my_OM310_DEV_h
//#include <Streaming.h>
#include "MY_MODBUS_DEF.h"
//#include "MY_EVENT_ARRAY.h"
//#include "DEVICE_ARRAY.h"

#include "MY_MODBUS_MASTER.h"
#include "MY_OM310_DEF.h"
//#include <MY_EVENT_EXT.h>
//#include "MY_EVENT_DEV_EXT.h"
//#ifndef STOP
//#define STOP 	DEBUG_OUT<<"STOP";while(1);
//#endif
//#ifndef  DEBUG_OUT
//#define DEBUG_OUT Serial
//#endif

#define OM_DEBUG
#define OM_DEBUG_TRCE

//uint16_t OM_Get_param1(uint16_t ADR);
void OM_SendtoEXT_param1();
void OM_Send_and_Get(uint16_t ADR, uint16_t VALUE);
void OM_Send_ALARM();
void OM_GET_ALARM();
void OM_Get_Param();
void OM_CHK_ALARM();
//void OM_GET_PWRS_S();

//void OM_SetHandler();			//устанавливаем обработчики ошибок
boolean OM_CHK();			//проверка доступности
//void NORESP_OM();		//  Обработчик события тайм-аута
//void CRC_OM();
//void ERR_OM();

OM_DATA OM310_Alarm_Value;	//хранит текущие значения считанные с ОМ
//******************************************************************************
//******************************************************************************
//void OM_SetHandler()
////******************************************************************************
//{//устанавливаем обработчики ошибок
//	Mbus.SET_NORESP(NORESP_OM);		//тайм-аут
//	Mbus.SET_BADCRC(CRC_OM);			//bad crc
//	Mbus.SET_ERR(ERR_OM);			//прочие ошибки
//}

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void CRC_OM()
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//{
//}
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void NORESP_OM()
////  Обработчик события тайм-аута
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//{
//	EVENTS_RESTART(OM_ERR, (time_t)15);
//	EVENTS_ADD(OM_CHK);			//запустить периодическую проверку
//}
//
//
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void ERR_OM()
////  обработчик события Ошибка устройства
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//{
//	Mbus.Disable_OM310();					//уст. признак запрета устройства
//	EVENTS_RESTART(OM_CHK, (time_t)15);		//перезапуск проверки
//}


//******************************************************************************
boolean OM_CHK()
//******************************************************************************
{
	if (Mbus.Device_CHK(OM_addr) != 0)	//проверка устройства
	{//если устройство откликнулось правильно
		//сбрасываем события ошибок
		//EVENTS_RESET(OM_NORESP);
		//EVENTS_RESET(OM_ERR);
		//EVENTS_RESET(OM_CHK);
		Mbus.Disable_OM310();
		return false;
	}
	else
		return true;
}

/**/
////****************************************************************************************
//void OM_SendtoEXT_param1()
////отправляет на сервер значение параметра om-310
////адрес и значение - должны находиться в  Mbus._CHANGE_PARAM
////****************************************************************************************
//{
//	//формируем и заполняем структуру
//	EXT_OM_PARAM OM_param;
//	OM_param._reg = RAR_OM_PARAM;
//	OM_param._dev_cnt = 1;
//	OM_param._TYP = Mbus._CHANGE_PARAM._param_adr;
//	OM_param._len = sizeof(Mbus._CHANGE_PARAM._param_value);
//	OM_param.VALUES[0] = Mbus._CHANGE_PARAM._param_value;
//	//отправляем
//#ifdef USE_EXT_ETHERNET
//	EXT_SendData((uint8_t*)&OM_param, sizeof(OM_param) - 2);
//#endif // USE_EXT_ETHERNET
//
//}
///**/
////****************************************************************************************
//void OM_Send_and_Get()
////присваивает в OM-310 указанному параметру новое значение
////и считывает его обратно
////****************************************************************************************
//{
//	if (Mbus.DevCHK_OM310())	//проверка запрета устройства
//	{
//		EVENTS_RESTART(_Event_num, (time_t)20);//рестарт текущего события
//#ifdef OM_DEBUG
//		DEBUG_OUT << F("DISABLE OM// restart\n");
//#endif // OM_DEBUG
//		return;
//	}
//	//формируем командную последовательность
//	uint8_t buf[8];
//	Mbus._buf_send = buf;
//	Mbus.insert_cmd(OM_addr, MB_CMD_WRITE);
//	Mbus.insert(Mbus._CHANGE_PARAM._param_adr);
//	Mbus.insert(Mbus._CHANGE_PARAM._param_value);
//#ifdef OM_DEBUG
//	DEBUG_OUT << F("GET SEND PARAM ADR=") << Mbus._CHANGE_PARAM._param_adr << F(" VALUE=") << Mbus._CHANGE_PARAM._param_value;
//#endif
//	//	Device_Set_Mbus_Events(OM);
//	Mbus.set_crc();
//	//OM_SetHandler();	//уст. обработчики
//	Mbus.response(200);
//	//если была ошибка
//	if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
//	{
//		EVENTS_RESTART(_Event_num, (time_t)20);//рестарт текущего события
//		return;
//	}
//	//проверяем что записали
//	uint16_t NEW_VALUE;
//	NEW_VALUE = Mbus.get_single(OM_addr, Mbus._CHANGE_PARAM._param_adr);
//	//если не совпали - ошибка
//	if (Mbus._CHANGE_PARAM._param_value != NEW_VALUE)
//	{
//		ERR_OM();
//		return;
//	}
//	OM_SendtoEXT_param1();//отправить на сервер для подтверждения 
//}
//
////****************************************************************************************
//void OM_Get_Param()
////чтение пареметров om-310
////****************************************************************************************
//{
//	if (!Mbus.DevCHK_OM310())	//проверка запрета устройства
//	{
//		//		goto rest;
//
//
//#if defined DEBUG_TRACE && defined(OM_DEBUG_TRCE)
//		DEBUG_TRACE << F("OM ") << F("GETPARAM") << F(" BEGIN\n");
//#endif
//		uint8_t buf[8];
//		EXT_OM_PARAM OM_param;
//		uint16_t i = OMP_TT;
//		//	OM_param._dev_adr = RAR_ETHERNET;
//		//	OM_param._cmd = MB_CMD_MWRITE;
//		OM_param._reg = RAR_OM_PARAM;
//		OM_param._dev_cnt = 12;
//		OM_param._len = sizeof(OM_param.VALUES);
//		//считываем по 12 парметров за раз
//		while (i < 198)
//		{
//			OM_param._TYP = i;
//			Mbus._buf_send = buf;
//			Mbus.insert_cmd(OM_addr, MB_CMD_READ);
//			Mbus.insert((uint16_t)i);
//			Mbus.insert((uint16_t)OM_param._dev_cnt);
//			Mbus.set_crc();
//			//OM_SetHandler();	//уст. обработчики
//			Mbus.response(200);	//получаем ответ
//			if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
//			{
//				goto rest;
//			}
//			Mbus.raw_to_int((uint16_t *)&OM_param.VALUES, OM_param._dev_cnt, 3);
//			i += 12;
//			//		DEBUG_OUT << "Call EXT_SendData " << endol;
//#if defined(USE_EXT_ETHERNET)
//			if (EXT_SendData((uint8_t*)&OM_param, sizeof(OM_param) - 2))
//			{
//				goto rest;
//			}
//#endif
//		}
//		//оставшиеся 6
//		OM_param._dev_cnt = 6;
//		OM_param._TYP = i;
//		Mbus._buf_send = buf;
//		Mbus.insert_cmd(OM_addr, MB_CMD_READ);
//		Mbus.insert((uint16_t)i);
//		Mbus.insert((uint16_t)OM_param._dev_cnt);
//		Mbus.set_crc();
////		OM_SetHandler();	//уст. обработчики
//		Mbus.response(200);
//		if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)	//���� ����������� ���� ������
//		{
//			goto rest;
//		}
//		Mbus.raw_to_int((uint16_t *)&OM_param.VALUES, OM_param._dev_cnt, 3);
//#if defined(USE_EXT_ETHERNET)
//		if (EXT_SendData((uint8_t*)&OM_param, sizeof(OM_param) - 2))
//		{
//			goto rest;
//		}
//#endif
//#if defined DEBUG_TRACE && defined(OM_DEBUG_TRCE)
//		DEBUG_TRACE << F("OM ") << F("GETPARAM") << F(" END\n");
//#endif
//		return;
//	}
//rest:
////	EVENTS_RESTART(_Event_num, (time_t)20);//рестарт текущего события
//#ifdef OM_DEBUG
////	DEBUG_OUT << F("DISABLE OM// restart\n");
//#endif // OM_DEBUG
//	return;
//}
//////****************************************************************************************
////void OM_GET_PWRS_S()
//////****************************************************************************************
////{
////#if defined DEBUG_TRACE && defined(OM_DEBUG_TRCE)
////	DEBUG_TRACE << F("OM ") << F("GET_PWRS_S") << F(" BEGIN\n");
////#endif
////	if (Device_IsDisable(OM))
////	{
////		EVENTS_RESTART(_Event_num, (time_t)15);
////		return;
////	}
////	uint8_t buf[8];
////	Mbus._buf_send = buf;
////	Mbus.insert_cmd(_DEVICES[OM]._dev_adr, MB_CMD_READ);
////	Mbus.insert((uint16_t)OM_PWRS_S);
////	Mbus.insert((uint16_t)2);
////	Device_Set_Mbus_Events(OM);
////	Mbus.set_crc();
////	Mbus.response(400);
////	if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)	
////	{
////		EVENTS_RESTART(_Event_num, (time_t)15);
////		return;
////	}
////#if defined DEBUG_TRACE && defined(OM_DEBUG_TRCE)
////	DEBUG_TRACE << F("OM ") << F("GET_PWRS_S") << F(" END\n");
////#endif
////}
//
////****************************************************************************************
//void OM_CHK_ALARM()
////обработчик события периодической проверки сигналов аварии OM-310
////****************************************************************************************
//{
//	if (Mbus.DevCHK_OM310())	//проверка запрета устройства
//	{
////		EVENTS_RESTART(_Event_num, (time_t)20);//рестарт текущего события
//#ifdef OM_DEBUG
////		DEBUG_OUT << F("DISABLE OM// restart\n");
//#endif // OM_DEBUG
//		return;
//	}
//	uint8_t buf[8];
//	Mbus._buf_send = buf;
//	Mbus.insert_cmd(OM_addr, MB_CMD_READ);
//	Mbus.insert((uint16_t)OM_STATUS);
//	Mbus.insert((uint16_t)2);
//	Mbus.set_crc();
////	OM_SetHandler();	//уст. обработчики
//	Mbus.response(400);
//	if (Mbus.StatusMChk(MODBUS_MASTER::MB_MASTER_response_ERR) == 1)
//	{
//		DEBUG_OUT << F("RESP ERR - RETURN\n");
//		return;
//	}
//	//	DEBUG_OUT << "PP2" << endol;
//	OM_ALARM_STRUCT cur_status;
//	Mbus.raw_to_int((uint16_t *)&cur_status, 2, 3);
//	//	OM31_Alarm_Value = P_OM_Get_param1(OM_STATUS);
//	if ((bitRead(OM310_Alarm_Value._Status, 0) == 1) && (bitRead(cur_status._Status, 0) == 0))
//	{
////		digitalWrite(PIN_ERROR_OM310_ALARM, LOW);
//		OM310_Alarm_Value._ALARM = 0;
//		OM_Send_ALARM();
//		DEBUG_OUT << F("*********NO OM-310 ALARM\n");
//		EVENTS_RESET(OM_GET_ALARM);
//	}
//	else
//		if ((bitRead(OM310_Alarm_Value._Status, 0) == 0) && (bitRead(cur_status._Status, 0) == 1))
//
//		{
//			digitalWrite(PIN_ERROR_OM310_ALARM, HIGH);
//			EVENTS_ADD(OM_GET_ALARM);
//			DEBUG_OUT << F("******OM-310 ALARM!\n");
//		}
//	OM310_Alarm_Value._Status = cur_status._Status;
//	OM310_Alarm_Value._ALARM = cur_status._ALARM;
//	//	digitalWrite(PIN_DEBUG_1, LOW);
////	DEBUG_OUT << ">END" << endol;
//}
//
////****************************************************************************************
//void OM_Send_ALARM()
////****************************************************************************************
//{
//	EXT_OM_PARAM OM_param;
//	//	OM_param._dev_adr = RAR_ETHERNET;
//	//	OM_param._cmd = MB_CMD_MWRITE;
//	OM_param._reg = RAR_OM_PARAM;
//	OM_param._dev_cnt = 2;
//	OM_param._TYP = OM_STATUS;
//	OM_param._len = sizeof(OM_param.VALUES);
//	OM_param.VALUES[0] = OM310_Alarm_Value._Status;
//	OM_param.VALUES[1] = OM310_Alarm_Value._ALARM;
//#ifdef USE_EXT_ETHERNET
//	if (EXT_SendData((uint8_t*)&OM_param, sizeof(OM_param) - 2))
//	{
//		EVENTS_RESTART(_Event_num, (time_t)10);
//		return;
//	}
//#endif
//}
////****************************************************************************************
//void OM_CRC_ERR()
////****************************************************************************************
//{
//	if (Mbus._buf[Mbus._buf_i - 1] == 0)
//	{
//		//		DEBUG_OUT << "***/*/OM CRC ERR!" << endol;
//	}
//	else
//	{
//		Mbus.StatusSet(MODBUS_BASE::MB_ERROR_CRC);
//	}
//}
////****************************************************************************************
//void OM_GET_ALARM()
//
////****************************************************************************************
//{
//	DEBUG_OUT << F("OM_GET_ALARM\n");
//	EXT_OM_PARAM OM_param;
//	OM_param._reg = RAR_OM_PARAM;
//	OM_param._dev_cnt = 9;
//	OM_param._TYP = OM_ALARM_VALUE;
//	OM_param._len = sizeof(OM_param.VALUES);
//	for (size_t i = 0; i < 9; i++)
//	{
//		if (bit_is_set(OM310_Alarm_Value._ALARM, i))
//		{
//			OM_param.VALUES[i] = Mbus.get_single(OM_addr, OM_ALARM_VALUE + i);
//			if (Mbus._status_base & MB_BASE_status_Mask != 0)
//			{
//				EVENTS_RESTART(_Event_num, (time_t)15);
//				return;
//			}
//		}
//		else
//			OM_param.VALUES[i] = 0xFFFF;
//	}
//#ifdef USE_EXT_ETHERNET
//	if (EXT_SendData((uint8_t*)&OM_param, sizeof(OM_param) - 2))
//	{
//		return;
//	}
//#endif
//}
#endif
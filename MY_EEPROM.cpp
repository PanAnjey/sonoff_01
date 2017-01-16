#include "MY_EEPROM.h"
#include <TimeLib.h>
#include "MY_MERCURY_DEF.h"
#include "MY_OM310_DEF.h"
struct Config MY_Config;
myip def_timeserv[]=
{
	{ 88,147,254,232 },
	{ 88,147,254,235 },
	{ 91,226,136,155 },
	{ 88,147,254,234 }
};
//************************************************************************
// Инициализация EEPROM
void EC_begin(void)
//************************************************************************
{
	size_t sz1 = sizeof(MY_Config);
	EEPROM.begin(sz1);
#ifdef USE_DEBUG
	DEBUG_OUT.printf("EEPROM init. Size = %d\n", (int)sz1);
#endif
}
#if defined(USE_DEBUG) 
//*******************************************************
void EC_DEBUG()
//*********************************************************
{

#if defined(USE_WIFI_CLIENT)
	DEBUG_OUT.printf("STA_SSID:`%s`\nSTA_PASS:`%s`\n", MY_Config.STA_SSID, MY_Config.STA_PASS);
	DEBUG_OUT.printf("AP_SSID:`%s`\nAP_PASS:`%s`\n", MY_Config.AP_SSID, MY_Config.AP_PASS);
#ifdef USE_WEBSERVER
	DEBUG_OUT.printf("HTTP_PASS:`%s`\n", MY_Config.HTTP_PASS);
#endif
	DEBUG_OUT.printf("MAC:0x%X:0x%X:0x%X:0x%X:0x%X:0x%X\n", MY_Config.MAC[0], MY_Config.MAC[1], MY_Config.MAC[2], MY_Config.MAC[3], MY_Config.MAC[4], MY_Config.MAC[5]);
#endif
#ifdef USE_NTP
	DEBUG_OUT.println("NTP_SERVERS:");
	uint8_t *p;
	p = (uint8_t*)&MY_Config.timeServer;
	for (uint8_t i = 0; i < sizeof(MY_Config.timeServer); i++)
	{
		DEBUG_OUT.printf("i=%d %d\n", i, p[i]);
	}
#endif
#ifdef USE_OM310
	DEBUG_OUT.printf("OM-310 M-bus adr:%u", MY_Config.OM310_adr);
#endif
#ifdef USE_MERCURY
	DEBUG_OUT.printf("MERCURY M-bus adr:%u", MY_Config.MERCURY_adr);
#endif
#ifdef USE_MQTT
	DEBUG_OUT.printf("MQTT_Sever:`%s`\n", MY_Config.MQTT_Sever);
	DEBUG_OUT.printf("MQTT_Port:%d\n", MY_Config.MQTT_Port);
	DEBUG_OUT.printf("MQTT_User:`%s`\n", MY_Config.MQTT_User);
	DEBUG_OUT.printf("MQTT_Pasw:`%s`\n", MY_Config.MQTT_Pasw);
#endif
#ifdef USE_DHT
	DEBUG_OUT.print("hum_on:"); DEBUG_OUT.println(MY_Config.VENT.hum_on);
	DEBUG_OUT.print("hum_off:"); DEBUG_OUT.println(MY_Config.VENT.hum_off);
	DEBUG_OUT.print("delay_onLight:"); DEBUG_OUT.println(MY_Config.VENT.delay_onLight);
	DEBUG_OUT.print("delay_offLight:"); DEBUG_OUT.println(MY_Config.VENT.delay_offLight);
	DEBUG_OUT.print("max_on:"); DEBUG_OUT.println(MY_Config.VENT.max_on);
#endif
	DEBUG_OUT.print("CRC:0x"); DEBUG_OUT.println(MY_Config.CRC, HEX);

}
#endif
//************************************************************************
// Читаем конфигурацию из EEPROM в память
void EC_read(void)
//************************************************************************
{
	//	DEBUG_OUT.println("***************\nREAD EEPROM:");
	size_t sz1 = sizeof(MY_Config);
	//   DEBUG_OUT.printf("size of MY_Config=%d\n", sz1);
	uint8_t *pp;
	pp = (uint8_t*)&MY_Config;
	for (int i = 0; i < sz1; i++)
	{
		uint8_t c = EEPROM.read(i);
		pp[i] = c;
		//	   DEBUG_OUT.printf("i=%d `%c` 0x%X\n", i, pp[i], pp[i]);
	}
	uint16_t crc = EC_CRC();
	//	DEBUG_OUT.printf("read CRC=%X calc CRC=%X\n", MY_Config.CRC, crc);
	if (MY_Config.CRC == crc)
	{
#ifdef USE_DEBUG
		DEBUG_OUT.printf("EEPROM Config is correct\n");
#endif
	}
	else
	{
#ifdef USE_DEBUG
		DEBUG_OUT.printf("EEPROM CRC is not valid: %X %X\n", crc, MY_Config.CRC);
#endif
		EC_default();
		EC_save();
	}
}

//************************************************************************
// Устанавливаем значения конфигурации по-умолчанию
void EC_default(void)
//************************************************************************
{
	//	DEBUG_OUT.println("************\nEC_default");
	size_t sz1 = sizeof(MY_Config);
	memset(&MY_Config, '\0', sz1);
	//   for( int i=0, byte *p = (byte *)&MY_Config; i<sz1; i++, p++) 
	//       *p = 0;   
#ifdef USE_WIFI_CLIENT
	strcpy(MY_Config.STA_SSID, SSID_NAME);
	strcpy(MY_Config.STA_PASS, SSID_PASS);
	strcpy(MY_Config.AP_SSID, AP_SSID_NAME);
	strcpy(MY_Config.AP_PASS, AP_SSID_PASS);
#ifdef USE_WEBSERVER
	strcpy(MY_Config.HTTP_PASS, PASW);
#endif
	MY_Config.MAC[0] = 0x60;
	MY_Config.MAC[1] = 0x01;
	MY_Config.MAC[2] = 0x94;
	MY_Config.MAC[3] = 0x07;
	MY_Config.MAC[4] = 0xc3;
#if MODUL_TYP == A108
	MY_Config.MAC[5] = 0xb1;
#endif
#if MODUL_TYP == MODBUS
	MY_Config.MAC[5] = 0xb2;
#ifdef USE_OM310
	MY_Config.OM310_adr = OM_addr;
#endif
#ifdef USE_MERCURY
	MY_Config.MERCURY_adr = MERCURY_addr;
#endif
#endif
#ifdef USE_NTP
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
	MY_Config.timeServer[i][j] = def_timeserv[i][j];
		}
	}
MY_Config.TZ = 3;
#endif
#ifdef USE_MQTT
strcpy(MY_Config.MQTT_Sever, MQTT_SERV_NAME);
MY_Config.MQTT_Port = MQTT_SERV_PORT;
strcpy(MY_Config.MQTT_User, MQTT_USER);
strcpy(MY_Config.MQTT_Pasw, MQTT_PASW);
#endif
// strcpy(MY_Config.HTTP_SERVER,"service.samopal.pro");

#endif
#ifdef USE_DHT
MY_Config.VENT.delay_offLight = 0;	//задержки включения-выключения при управлении светом
MY_Config.VENT.delay_onLight = 0;
MY_Config.VENT.hum_on = 60.0;		//влажность включения-выключения
MY_Config.VENT.hum_off = 40.0;
MY_Config.VENT.max_on = SECS_PER_HOUR;
#endif
//   strcpy(MY_Config.HTTP_REQUEST,"/save3?id=%s&h=%d&t=%d&a=%d&tm1=%d&tm2=%d&uptime=%ld");  
}

//************************************************************************
// Сохраняем значение конфигурации в EEPROM

void EC_save(void)
//************************************************************************
{
	MY_Config.CRC = EC_CRC();
	size_t sz1 = sizeof(MY_Config);
	uint8_t *pp;
	pp = (uint8_t*)&MY_Config;
	for (int i = 0; i < sz1; i++)
	{
		EEPROM.write(i, pp[i]);
		//   DEBUG_OUT.printf("i=%d `%c` 0x%X\n", i, pp[i], pp[i]);
	}
	EEPROM.commit();
#ifdef USE_DEBUG
	DEBUG_OUT.printf("Save Config to EEPROM. CRC=%d\n", MY_Config.CRC);
#endif
}

//************************************************************************
// Вычисляем CRC
uint16_t EC_CRC(void)
//************************************************************************
{
	//  uint16_t crc = 0;
   //	DEBUG_OUT.println("*******************\nCRC calc:");
	size_t len = sizeof(MY_Config) - 2;
	//   uint16_t crc_save = MY_Config.CRC;
	//   MY_Config.CRC = 0;
	uint16_t i, j;
	uint16_t k, l;
	uint8_t *_ptr;
	_ptr = (uint8_t*)&MY_Config;
	k = 0xFFFF;
	//			DEBUG_OUT<<"***\n";
	for (i = 0; i < len; i++)
	{
		k = k ^ _ptr[i];
		//   DEBUG_OUT.printf("i=%d `%c` 0x%X\n",i, _ptr[i],_ptr[i]);
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
#ifdef USE_DEBUG
	DEBUG_OUT.printf("CRC calc =%d (0x%X) \n", k, k);
#endif
	//   MY_Config.CRC = crc_save;
	return k;
}


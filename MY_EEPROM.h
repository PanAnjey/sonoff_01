//************************************************************
// сохранение в EEPROM пареметров конфигурации
//***********************************************************


#ifndef MY_EEPROM_h
#define MY_EEPROM_h
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <EEPROM.h>
#include "MY_define.h"
extern struct Config MY_Config;
extern uint32_t tm;
extern bool flag_light;
extern bool flag_hum;
extern bool flag_btn;
extern uint16_t timer;
//#pragma pack(push, 1)
struct Config
{
#ifdef USE_WIFI_CLIENT
// Наименование в режиме точки доступа  
   char STA_SSID[10];
   char STA_PASS[20];
// Параметры подключения в режиме клиента
   char AP_SSID[10];
   char AP_PASS[20];
// Пароль доступа к контроллеру
#ifdef USE_WEBSERVER
   char HTTP_PASS[20];
#endif
   uint8_t MAC[6];
 //  IPAddress IP;
   //IPAddress MASK;
   //IPAddress GW;
// Параметры NTP сервера
#ifdef USE_NTP
   myip timeServer[4];
   short int  TZ;
#endif
   
// Сервер MQTT
#ifdef USE_MQTT
   char MQTT_Sever[50];
   uint16_t MQTT_Port;
   char MQTT_User[10];
   char MQTT_Pasw[20];
#endif
 //  char     HTTP_SERVER[48]; 
#endif
//**************************
#ifdef USE_OM310
   uint8_t OM310_adr;
#endif
#ifdef USE_MERCURY
   uint8_t MERCURY_adr;
#endif

// влажность включения и выключения вентилятора
#ifdef USE_DHT
   VENT_PARAM VENT;
#endif
   // Строка отправки параметров на сервер
//   char     HTTP_REQUEST[128];   
// Контрольная сумма   
   uint16_t CRC;   
}__attribute__((__packed__));
//#pragma pack(pop)

void     EC_begin(void);
void     EC_read(void);
void     EC_save(void);
uint16_t EC_CRC(void);
void     EC_default(void);
#ifdef USE_DEBUG
void EC_DEBUG();
#endif




#endif


#ifndef _my_defin_
#define _my_defin_

//*******************************************************************************
// используемые модули
//*******************************************************************************
//#define MODUL_TYP A108
#define MODUL_TYP MODBUS

#if MODUL_TYP == A108
#undef USE_MERCURY
#undef USE_OM310
#undef USE_MBUS
#define USE_DHT
#endif

#if MODUL_TYP == MODBUS
//#define USE_MERCURY			//Мкеркурий 230
#define USE_OM310			//ОМ-310
#define USE_MBUS
#undef USE_DHT
#endif




#define USE_WIFI_CLIENT
#define USE_WEBSERVER


//#define USE_MQTT
#define USE_NTP
#define NTP_interval (time_t)SECS_PER_HOUR
//

#if defined(USE_MBUS)

#define RS485 Serial
#define bRecive_speed 9600
#endif

#define USE_DEBUG
#if defined(USE_MBUS)
#define DEBUG_OUT Serial1
#define USE_DEBUG_MBUS
#else
#define DEBUG_OUT Serial
#endif




#define LEDgreen LED_BUILTIN	
#define PIN_LED2 13
#define PIN_RELAY 12


typedef uint8_t myip[4];

#ifdef USE_DHT
#define PIN_DHT 14
struct DHT_DATA
{
	float temp;	//темперетура
	float hum;	//влажность
};

struct VENT_PARAM	//параметры включения/выключения вентилятора по показанию влажности
{
	float hum_on;	//при этом значении влажности включаем вентилятор
	float hum_off;	//при этом - выключаем
	time_t delay_onLight;	//задержка включения вентилятора после включения света
	time_t delay_offLight;	//задержка после выключения света
	time_t max_on;			//макс. время непрерывной работы вентилятора
};
#endif
#ifdef USE_WEBSERVER
#define WD 15 //ширина поля ввода
#endif
#if defined (USE_WEBSERVER) && defined (USE_WIFI_CLIENT)
#define USER "admin"
#define PASW "admin"
#endif

#ifdef  USE_WIFI_CLIENT
#define SSID_NAME "sve3com2"
#define SSID_PASS "serayakrysa89"
//#define SSID_NAME "Z403"
//#define SSID_PASS "4953617940"
#if MODUL_TYP == A108
#define AP_SSID_NAME "A108"
#define AP_SSID_PASS "serayakrysa89"
#endif
#if MODUL_TYP == MODBUS
#define AP_SSID_NAME "Mercury01"
#define AP_SSID_PASS "serayakrysa89"
#endif
#endif

#ifdef USE_MQTT
#define MQTT_SERV_NAME "192.168.1.5"
#define MQTT_SERV_PORT 1883
#define MQTT_USER "A108"
#define MQTT_PASW "A108pasw"
#endif

#ifndef USE_WIFI_CLIENT
#undef USE_MQTT
#undef USE_NTP
#undef USE_WEBSERVER
#endif

#ifndef USE_DEBUG
#undef DEBUG_OUT
#endif
////**********************************************************************************
////								пины MBUS
////**********************************************************************************
#if defined(USE_MBUS)
#define RE_PIN D6
#endif


//#define buttonPin  A2		 

#define MEANS_INTERV (time_t)15		//интервал измерений
#define CHK_WIFI_INTERV (time_t)SECS_PER_MIN
#define ERROR_VALUE 2147483647
		
//#ifdef USE_DEBUG
//#define STOP 	while(1);
//#endif



#endif

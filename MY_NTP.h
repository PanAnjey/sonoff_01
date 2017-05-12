/**********************************************************
* работа с NTP серверами
**********************************************************/

#ifndef MY_NTP_h
#define MY_NTP_h
#ifdef USE_NTP
#include <ESP8266WiFi.h>
//#include <WiFiUdp.h>

extern time_t  next_ntp;
//extern time_t ntp_last;
extern time_t uptime_tm;
//extern uint8_t ntp_serial;
//extern char   ntp_host[];
extern bool send_NTP;
extern uint8_t NTP_INDEX;
extern time_t ntp_time;
void NTPBegin(void);
time_t NTPGet(void);
void NTPSend();

#endif
#endif

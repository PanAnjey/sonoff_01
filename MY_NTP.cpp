#ifdef USE_NTP
#include "MY_NTP.h"
#include "MY_EEPROM.h"
#include <TimeLib.h>
#include <WiFiUdp.h>
//#include <ESP8266Ping.h>
WiFiUDP udp;
#define  NTP_PACKET_SIZE  48

byte packetBuffer[NTP_PACKET_SIZE];
//uint8_t ntp_serial      = 1;
time_t next_ntp;
time_t ntp_time = 0;
time_t uptime_tm        = 0;
//char   ntp_host[32];
bool send_NTP; 

uint8_t NTP_INDEX;

unsigned int  localPort = 2390;      // local port to listen for UDP packets

//********************************************************************
void NTPBegin(void)
//********************************************************************
{

	// Инициализация UDP соединения с NTP сервером
#ifdef USE_DEBUG
	DEBUG_OUT.println("Starting UDP");
#endif
	udp.begin(localPort);
#ifdef USE_DEBUG
	DEBUG_OUT.print("Local port: ");
	DEBUG_OUT.println(udp.localPort());
#endif
	send_NTP = false;
	for (uint8_t i = 0; i < 4; i++)
	{
		NTP_INDEX = i;
	//	bool echoReply;

	//	echoReply= Ping.ping(MY_Config.timeServer[NTP_INDEX]);
	//	if (echoReply)
	//	{
#ifdef USE_DEBUG
			DEBUG_OUT.printf("NTP INDEX: %d\n", NTP_INDEX);
#endif
		NTPSend();
			delay(1000);
			ntp_time = NTPGet();
			if (ntp_time != 0)
			{
				setTime(ntp_time);
#ifdef USE_DEBUG
				tmElements_t tt;
				breakTime(now(), tt);
				DEBUG_OUT.printf("SET system time: %d.%d.%d %d:%d:%d\n",tt.Day,tt.Month,tt.Year,tt.Hour,tt.Minute,tt.Second);
#endif
				next_ntp = now() + NTP_interval;
				return;	//сервер пингуется
			}
	//	}
	}
//	если пришли сюда - ни один сервер не пингуется
//#ifdef USE_DEBUG
//	DEBUG_OUT.println("NO NTP PING");
//#endif
	
}


//********************************************************************
/**
 * Посылаем и парсим запрос к NTP серверу
 */
 //********************************************************************
time_t NTPGet(void)
//********************************************************************
{
	// IPAddress ntpIP;
	 time_t tm = 0;


   //// Ротация NTP сервров из пула
   //  //switch(ntp_serial)
   //  //{
   //  //   case 0:
   //  //      strcpy(ntp_host,MY_Config.NTP_SERVER1);
   //  //      ntp_serial = 1;
   //  //      break;
   //  //   case 1:
   //  //      strcpy(ntp_host,MY_Config.NTP_SERVER2);
   //  //      ntp_serial = 2;
   //  //      break;
   //  //   default:        
   //  //      strcpy(ntp_host,MY_Config.NTP_SERVER3);
   //  //      ntp_serial = 0;
   //  //      break;
   //  // }
   //  //WiFi.hostByName(ntp_host, ntpIP); 
   //#ifdef USE_DEBUG
   //  DEBUG_OUT.printf("Send NTP server %s ",ntp_host);
   //  DEBUG_OUT.println(ntpIP);
   //#endif
   //  sendNTPpacket(ntpIP); 
   //  delay(1000);
   //  
   int cb = udp.parsePacket();
     if (!cb) 
    {
 /*  #ifdef USE_DEBUG
      Serial.println("NTP server error");
   #endif*/
       return tm;
   }
     else
     {
   //    DEBUG_OUT.printf("NTP received, length=%d\n",cb);
   // Читаем пакет в буфер    
       udp.read(packetBuffer, NTP_PACKET_SIZE); 
   // 4 байта начиная с 40-го сождержат таймстамп времени - число секунд 
   // от 01.01.1900   
       unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
       unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
   // Конвертируем два слова в переменную long
       unsigned long secsSince1900 = highWord << 16 | lowWord;
   // Конвертируем в UNIX-таймстамп (число секунд от 01.01.1970
       const unsigned long seventyYears = 2208988800UL;
       unsigned long epoch = secsSince1900 - seventyYears;
   // Делаем поправку на местную тайм-зону
       tm = epoch + MY_Config.TZ*3600; 
 /*  #ifdef USE_DEBUG
       DEBUG_OUT.printf("NTP  time = %ld\n",tm);
   #endif*/
     //  ntp_last = tm;
     }
	return tm;
}

//********************************************************************
/**
 * Посылаем запрос NTP серверу на заданный адрес
 */
 //********************************************************************
void NTPSend()
//********************************************************************
{

//	Serial.println("Sending NTP packet...");
	// Очистка буфера в 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Формируем строку запроса NTP сервера
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// Посылаем запрос на NTP сервер (123 порт)
	udp.beginPacket(MY_Config.timeServer[NTP_INDEX], 123);
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();

}

#endif
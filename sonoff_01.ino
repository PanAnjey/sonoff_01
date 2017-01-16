//**************************************************
// модуль управления вентилятором в пом. 108
#include <arduino.h>
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif
//**************************************************************************
#include <TimeLib.h>
time_t time_next;

#include "MY_define.h"
#include <EEPROM.h>
#include "MY_EEPROM.h"
//#include <MY_MODBUS\MY_MODBUS_BASE.h>



//для управления миганием светодиодом
uint8_t  blink_loop = 0;
uint8_t  modes_count = 0;
uint8_t  blink_mode = 0B00000101;
uint32_t ms0 = 0;
uint32_t ms1 = 0;
uint32_t ms2 = 0;
uint32_t ms3 = 0;
uint32_t ms4 = 0;
uint32_t ms5 = 0;
bool relay_on = false;		//состояние реле
//****************************************************************
#if defined (USE_DHT)
#include <Adafruit_Sensor.h>
// DHT от Adafruit 
// https://github.com/adafruit/DHT-sensor-library
#include "DHT.h" 
//bool ds_enable = false;
bool dht_enable = false;	//признак досупности датчика температуры/влажности


DHT dht(PIN_DHT, AM2301);
DHT_DATA _dht;
//VENT_PARAM _vent;
#endif
//**********************************************************
// MBUS
//***********************************************************

#ifdef USE_MBUS
//#include "MY_MODBUS_BASE.h"
#include "MY_MODBUS_MASTER.h"
#include <ESP8266TrueRandom\ESP8266TrueRandom.h>
volatile uint8_t buf[MAX_RESPONSE_LENGTH];
MODBUS_MASTER Mbus;
#ifdef USE_OM310
#include "MY_OM310_DEF.h"
#include "MY_OM310.h"
#endif
#endif

//***************************************************************

#ifdef USE_WIFI_CLIENT
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266Ping.h>
WiFiClient client;
#ifdef USE_WEBSERVER 

//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "MY_HTTP.h"


//const char* host = "192.168.1.5";
IPAddress Server(192, 168, 1, 5);
ESP8266WebServer server(80);
#endif
#ifdef USE_MQTT
#include <PubSubClient.h>
PubSubClient MQTT_client(espClient);
#endif // USE_MQTT
#ifdef USE_NTP
#include "MY_NTP.h"
#endif
//uint8_t mac[6] = { 0x60, 0x01, 0x94, 0x07, 0xc3, 0xb1 };
//const char* ssid = "sve3com2";
//const char* password = "serayakrysa89";
bool wifi_enable = false;		// признак доступности wi-fi
#define WIFI_CHK_INTERV (time_t)SECS_PER_MIN * 10		//интервал проверки доступности wifi
#define WIFI_CHK_CNT_MAX 5		//число проверок
time_t wifi_time_next_chk = 0;	//хранит время след. проверки wifi
uint8_t wifi_chk_cnt = 0;		//счетчик проверок
IPAddress aa;
#endif
#ifdef USE_WEBSERVER
//***************************************************************
//проверка доступа
bool is_authentified()
//****************************************************************
{
	//	DEBUG_OUT.println("Enter is_authentified");
	if (server.hasHeader("Cookie"))
	{
		//		DEBUG_OUT.print("Found cookie: ");
		String cookie = server.header("Cookie");
		//		DEBUG_OUT.println(cookie);
		if (cookie.indexOf("ESPSESSIONID=1") != -1)
		{
			//			DEBUG_OUT.println("Authentification Successful");
			return true;
		}
	}
	//	DEBUG_OUT.println("Authentification Failed");
	return false;
}
//************************************************************************************************************************
//       вывод заголовка html
void HTTP_printHeader(String &out, const char *title, const char *refresh)
//************************************************************************************************************************
{
	String ff;
	if (server.hasHeader("User-Agent"))
	{
		String Agent = server.header("User-Agent");
		//		DEBUG_OUT.println(cookie);
		if (Agent.indexOf("C6503") != -1)
			ff = "font-size: 170%;";
		else
			ff = "";
	}

	out += "<html><head><meta charset=\"utf-8\" />\n";
	if (refresh != "")		// если определен параметр refresh
	{
		out += "<meta http-equiv='refresh' content='";

		out += refresh;
		out += "'/>\n";
	}
	out += "<title>";
	out += title;
	out += "</title>\n";
	out += "<style>body {width: 100%; height: 100%; background-color: #cccccc;\
     font-family: Arial, Helvetica, Sans-Serif;";
	out += ff;
	out += "Color: #000088;}";
	out += " td {vertical-align: baseline;" + ff + "}";
	out += " h3 {" + ff + "}";
	out += " a {" + ff + "}";
	out += " input {" + ff + "}";
		out+="html{ width: 100%; height: 100%}</style></head>\n";

		out += "<body><h1>";
	out += title;
	out += "</h1>\n";
}

//**************************************************************
// страница логина
void handleLogin()
//**************************************************************
{
	String msg;
	String h1 = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=";
	String h2 = "\r\nLocation: ";
	String h3 = "\r\nCache-Control: no-cache\r\n\r\n";
	if (server.hasHeader("Cookie"))
	{
#ifdef USE_DEBUG
		DEBUG_OUT.print("Found cookie: ");
#endif
		String cookie = server.header("Cookie");
#ifdef USE_DEBUG
		DEBUG_OUT.println(cookie);
#endif
	}
	if (server.hasArg("DISCONNECT"))
	{
#ifdef USE_DEBUG
		DEBUG_OUT.println("Disconnection");
#endif
		String header = h1 + "0" + h2 = "/login" + h3;
		server.sendContent(header);
		return;
	}
	if (server.hasArg("USERNAME") && server.hasArg("PASSWORD"))
	{
		if (server.arg("USERNAME") == USER &&  server.arg("PASSWORD") == PASW)
		{
			String header = h1 + "1" + h2 + "/" + h3;
			server.sendContent(header);
#ifdef USE_DEBUG
			DEBUG_OUT.println("Log in Successful");
#endif
			msg = "";
			return;
		}
		msg = "Wrong username/password! try again.";
#ifdef USE_DEBUG
		DEBUG_OUT.println("Log in Failed");
#endif
	}
	String content;
	HTTP_printHeader(content, "Login", "");

	content += "<form action='/login' method='POST'>\
    <table border=0 width=80%>";
	HTTP_printInput(content, "Введите Логин:", "USERNAME", "user name", 32, 0);
	HTTP_printInput(content, "Введите Пароль:", "PASSWORD", "password", 32, 1);
	content += "</table><br><input type = 'submit' name = 'SUBMIT' value = 'Ввод'></form><b>";
	content += msg;
	content += "</b>";
	HTTP_printTail(content);
	server.send(200, "text/html", content);
}


//******************************************************************************
//  установка параметров включения/выключения вентилятора
void handleParam()
//******************************************************************************
{

#ifdef USE_DEBUG
	DEBUG_OUT.println("Enter handleparam");
#endif
	String content;
	if (!is_authentified())	//если не авторизованы - отправляемся на login
	{
		String content = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(content);
		return;
	}
	if (server.hasArg("STA_SSID") && server.hasArg("STA_PASS"))
	{
		if (server.hasArg("STA_SSID"))
		{
			//			DEBUG_OUT.print("Old param 'STA_SSID'="); DEBUG_OUT.println(MY_Config.STA_SSID);
			strcpy(MY_Config.STA_SSID, server.arg("STA_SSID").c_str());
			//			DEBUG_OUT.print("New param 'STA_SSID'="); DEBUG_OUT.println(MY_Config.STA_SSID);
		}
		if (server.hasArg("STA_PASS"))
		{
			//			DEBUG_OUT.print("Old param 'STA_PASS'="); DEBUG_OUT.println(MY_Config.STA_PASS);
			strcpy(MY_Config.STA_PASS, server.arg("STA_PASS").c_str());
			//			DEBUG_OUT.print("New param 'STA_PASS'="); DEBUG_OUT.println(MY_Config.STA_PASS);

		}
		if (server.hasArg("AP_SSID"))
		{
			//			DEBUG_OUT.print("Old param 'AP_SSID'="); DEBUG_OUT.println(MY_Config.AP_SSID);
			strcpy(MY_Config.AP_SSID, server.arg("AP_SSID").c_str());
			//			DEBUG_OUT.print("New param 'AP_SSID'="); DEBUG_OUT.println(MY_Config.AP_SSID);
		}
		if (server.hasArg("AP_PASS"))
		{
			//		DEBUG_OUT.print("Old param 'AP_PASS'="); DEBUG_OUT.println(MY_Config.AP_PASS);
			strcpy(MY_Config.AP_PASS, server.arg("AP_PASS").c_str());
			//		DEBUG_OUT.print("New param 'AP_PASS'="); DEBUG_OUT.println(MY_Config.AP_PASS);
		}
		if (server.hasArg("HTTP_PASS"))
		{
			//		DEBUG_OUT.print("Old param 'HTTP_PASS'="); DEBUG_OUT.println(MY_Config.AP_PASS);
			strcpy(MY_Config.HTTP_PASS, server.arg("HTTP_PASS").c_str());
			//		DEBUG_OUT.print("New param 'HTTP_PASS'="); DEBUG_OUT.println(MY_Config.AP_PASS);
		}
		//	if (server.hasArg("AP_SSID"))strcpy(MY_Config.AP_SSID, server.arg("AP_SSID").c_str());
		//	if (server.hasArg("AP_SSID"))strcpy(MY_Config.AP_SSID, server.arg("AP_SSID").c_str());
#ifdef USE_DA
		if (server.hasArg("HUM_ON")) MY_Config.VENT.hum_on = atoi(server.arg("HUM_ON").c_str());
		if (server.hasArg("HUM_OFF")) MY_Config.VENT.hum_off = atoi(server.arg("HUM_OFF").c_str());
#endif // USE_DA

		if (server.hasArg("TZ")) MY_Config.TZ = atoi(server.arg("TZ").c_str());
#ifdef USE_OM310
		if (server.hasArg("OM310_adr")) MY_Config.OM310_adr = atoi(server.arg("OM310_adr").c_str());
#endif // USE_OM310
#ifdef USE_MERCURY
		if (server.hasArg("MERCURY_adr")) MY_Config.MERCURY_adr = atoi(server.arg("MERCURY_adr").c_str());
#endif // USE_MERCURY
		EC_save();
		String header = "HTTP/1.1 301 OK\r\nLocation: /config\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;

		//	ESP.reset();

	}
	content = "";
	HTTP_printHeader(content, "Конфигурация контроллера MERC", "");
	content += "<ul>\
     <li><a href=\"/\">Главная</a>\
     <li><a href=\"/reboot\">Перезагрузка</a>\
     </ul>\n";
	content += "<form action='/setparam' method='POST'>\
    <table border=0 width=100%>";

	HTTP_printInput(content, "PSK точки доступа", "AP_SSID", MY_Config.AP_SSID, WD, 0);
	HTTP_printInput(content, "SSID точки доступа", "AP_PASS", MY_Config.AP_PASS, WD, 0);
	HTTP_printInput(content, "SSID", "STA_SSID", MY_Config.STA_SSID, WD, 0);
	HTTP_printInput(content, "WPA-PSK", "STA_PASS", MY_Config.STA_PASS, WD, 0);
	//	HTTP_printInput(content, "SSID точки доступа", "AP_SSID", MY_Config.AP_SSID, WD, 0);
	HTTP_printInput(content, "Пароль доступа HTTP", "HTTP_PASS", MY_Config.HTTP_PASS, WD, 0);
	//content += "</table><table border=0><tr>";
	//HTTP_printInput_mac(content, "MAC adr.:", "MAC0", MY_Config.MAC[0], 3, 0);
	//HTTP_printInput_mac(content, ".", "MAC1", MY_Config.MAC[1], 3, 0);
	//HTTP_printInput_mac(content, ".", "MAC2", MY_Config.MAC[2], 3, 0);
	//HTTP_printInput_mac(content, ".", "MAC3", MY_Config.MAC[3], 3, 0);
	//HTTP_printInput_mac(content, ".", "MAC4", MY_Config.MAC[4], 3, 0);
	//HTTP_printInput_mac(content, ".", "MAC5", MY_Config.MAC[5], 3, 0);
	HTTP_printInput(content, "Временная зона", "TZ", MY_Config.TZ, WD, 0);
#ifdef USE_DA
	HTTP_printInput(content, "Вент. % включение", "HUM_ON", MY_Config.VENT.hum_on, WD, 0);
	HTTP_printInput(content, "Вент. % выключение", "HUM_OFF", MY_Config.VENT.hum_off, WD, 0);
#endif
#ifdef USE_OM310
	HTTP_printInput(content, "ОМ-310 - адрес на шине", "OM310_adr", MY_Config.OM310_adr, WD, 0);
#endif // USE_OM310
#ifdef USE_MERCURY
	HTTP_printInput(content, "Меркурий - адрес на шине", "MERCURY_adr", MY_Config.MERCURY_adr, WD, 0);
#endif
	content += "</table>\n";
	//#ifdef USE_NTP
	//	content += "<table border = 0 width=100%>";
	//	//HTTP_printInput(content, "NTP_SERVER1", "NTP_SERVER1", MY_Config.NTP_SERVER1, 32, 0);
	//	//HTTP_printInput(content, "NTP_SERVER2", "NTP_SERVER2", MY_Config.NTP_SERVER1, 32, 0);
	//	//HTTP_printInput(content, "NTP_SERVER3", "NTP_SERVER3", MY_Config.NTP_SERVER1, 32, 0);
	//	content += "</tr></table>\n";
	//#endif
	content += "<input type = 'submit' name = 'SUBMIT' value = 'Сохранить'>";
	content += "</form>\n";
	HTTP_printTail(content);
	server.send(200, "text/html", content);
}



//**************************************************************************
void HTTP_handleReboot(void) 
//**************************************************************************
{
	String out = "";
	HTTP_printHeader(out, "Перезагрузка контроллера", "30;URL=/");
    out+= "<p><a href=\"/\">Через 30 сек будет переадресация на главную</a></body></html>";
	server.send(200, "text/html", out);
	//  ((void(*)(void))0x40000080)();
	  ESP.reset();  
}
//********************************************************************************
//Основная страница
void handleRoot()
//********************************************************************************
{
//#ifdef USE_DEBUG
//	DEBUG_OUT.println("Enter handleRoot");
//#endif
	String header;
	if (!is_authentified())	//если не авторизованы - отправляемся на login
	{
		String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
	String content;
	//= "<html><head><meta charset=\"utf-8\"><meta http-equiv=\"refresh\" content=\"5\"></head> <body><H2>hello, you successfully connected to SONOFF !</H2><br>";
#ifdef USE_MBUS
#ifdef USE_OM310
	HTTP_printHeader(content, "Текущие данные ОМ-310", "5");
#endif
#ifdef USE_MERCURY
	HTTP_printHeader(content, "Текущие данные МЕРКУРИЙ", "5");
#endif
#else
	HTTP_printHeader(content, "Текущие данные", "5");
#endif
	char str_data[50];
	//sprintf(str_data, "temp: %f<br>hum: %f<br>", _dht.temp, _dht.hum);
	//DEBUG_OUT.println(str_data);
#ifdef USE_OM310
	String thead0,thead1;
	thead0 = "<table border = 0 width = '100%'><tr>\
<td colspan=3>";
	thead1 = "</td></tr>\
<tr><td width=33%>А</td><td width=33%>B</td><td width=33%>C</td></tr><tr>";
	content += thead0;
	content += "Значения напряжения по фазам";
	content += thead1;
//	dtostrf(_dht.temp, 3, 2, str_data);
	for (uint8_t i = 0; i < 3; i++)
	{
		sprintf(str_data, "", OM310_Alarm_Value.V[i]);
		content += "<td width=33%>";
		content += str_data;
		content += "</td>";
	}
	content += "</table>";
#endif
#ifdef USE_DHT
	dtostrf(_dht.temp, 3, 2, str_data);
	content += "<table border = 0 width = '100%'>\
		<tbody><tr><td align='right' width='50%'><h3>Температура(C):</h3></td>\n<td><h3>";
	content += str_data;
	content += "</h3></td></tr>\
		<tr><td align = 'right' width = '50%'><h3>Влажность(%):</h3></td>\
		<td><h3><strong>";
	dtostrf(_dht.hum, 3, 0, str_data);
	if (relay_on)
	{
		content += "<blink><font color='red'>";
		content += str_data;
		content += "</font></blink>";
	}
	else
	{
		content += "<font color='green'>";
		content += str_data;
		content += "</font>";
	}
	content += "</strong></h3></td></tr></tbody></table>";
#endif
	tmElements_t tt;
	breakTime(now(), tt);

	sprintf(str_data, "<h3>Дата:%02u.%02u.%04u %02u:%02u:%02u</h3>", \
		tt.Day, tt.Month, tmYearToCalendar(tt.Year), tt.Hour, tt.Minute, tt.Second);
	content += str_data;
	//server.sendContent(content);
//	server.sendContent(content);

//	if (server.hasHeader("User-Agent")) 
//	{
//		content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
//#ifdef USE_DEBUG
//		DEBUG_OUT.print("User-Agent:");
//		DEBUG_OUT.println(server.header("User-Agent"));
//
//
//#endif
//	}
	//int pn = server.headers();
	//for (size_t i = 0; i < pn; i++)
	//{
	//	content += "<br> Header name:" + server.headerName(i);
	//	content += ":&nbsp" + server.header(i);
	//}
	//content += "<br> Host header:" + server.hostHeader();
	content += "<br><a href=\"/setparam\">Настройка параметров</a></body></html>";
	server.send(200, "text/html", content);
}

//**************************************************************************
void handleNotFound()
//**************************************************************************
{
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
	for (uint8_t i = 0; i < server.args(); i++)
	{
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}
	server.send(404, "text/plain", message);
}
#endif

//***************************************************************************
void setup()
//***************************************************************************
{
	long ret;
	//************************ реле *********************************************
	pinMode(PIN_RELAY, OUTPUT);
	pinMode(PIN_LED2, OUTPUT);

	//relay_on = !relay_on;
	digitalWrite(PIN_RELAY, relay_on);
	digitalWrite(PIN_LED2, wifi_enable);
	delay(5000);
#ifdef USE_DEBUG
	DEBUG_OUT.begin(115200);// Последовательный порт для отладки
#endif
	//***************************************************************************
	// Инициализация MBUS
	//***************************************************************************
#if defined(USE_MBUS)
	//randomSeed(analogRead(0));
	//	prog_char ini_mbus[] = "MODBUS..";
	Mbus.MODBUS_BASE::begin(0, MAX_RESPONSE_LENGTH, (uint8_t*)buf, RE_PIN);
	Mbus.init_buf(MAX_RESPONSE_LENGTH);
	RS485.begin(9600);
	//	DEBUG_OUT << F("MODBUS init OK\n");
	Serial.swap();
#endif
	//************************************************
	//инициализация конфигурации
	//************************************************
	EC_begin();		
	EC_read();
#ifdef USE_DEBUG
	EC_DEBUG();
#endif
	//************************************************
	// Инициализация wi-fi клиента
	//***********************************************
#ifdef USE_WIFI_CLIENT
	wifi_set_macaddr(STATION_IF, MY_Config.MAC);
#ifdef USE_DEBUG
	DEBUG_OUT.printf("\nConnecting to: %s/%s\n", MY_Config.STA_SSID, MY_Config.STA_PASS);
	DEBUG_OUT.print("MAC: ");	DEBUG_OUT.println(WiFi.macAddress());
#endif	
	//**************************************************************************************************************
	WiFi.mode(WIFI_STA);
	// Пытаемся соединиться с точкой доступа
	WiFi.begin(MY_Config.STA_SSID, MY_Config.STA_PASS);
	delay(1000);

	// Максиммум N раз проверка соединения (12 секунд)
	for (int j = 0; j < 15; j++)
	{
		if (WiFi.status() == WL_CONNECTED)
		{
			wifi_enable = true;
			blink_mode = 0B11111111;
#ifdef USE_DEBUG
			DEBUG_OUT.print("\nWiFi connect: ");
			DEBUG_OUT.print(WiFi.localIP());
			DEBUG_OUT.print("/");
			DEBUG_OUT.print(WiFi.subnetMask());
			DEBUG_OUT.print("/");
			DEBUG_OUT.println(WiFi.gatewayIP());
#endif
#ifdef USE_NTP
			NTPBegin();
#endif
#ifdef USE_MQTT
			MQTT_client.setServer(MY_Config.MQTT_Sever, MY_Config.MQTT_Port);
			if (!MQTT_client.connected())
			{//пытаемся коннектится
#ifdef USE_DEBUG
				DEBUG_OUT.println("MQTT connected...");
#endif		
				if (MQTT_client.connect("ESP8266Client"))
				{
#ifdef USE_DEBUG
					DEBUG_OUT.println("OK.");
#endif
					// Once connected, publish an announcement...
					MQTT_client.publish("A108/DEBUG", "START SETUP...");
					// ... and resubscribe
					//client.subscribe("inTopic");
				}
				else
				{
#ifdef USE_DEBUG
					DEBUG_OUT.print("failed, rc=");
					DEBUG_OUT.print(MQTT_client.state());
					//			Serial.println(" try again in 5 seconds");
								// Wait 5 seconds before retrying
					//			delay(5000);
				}
			}
#endif
#endif
			goto lab1;
		}
		delay(1000);
#ifdef USE_DEBUG
		DEBUG_OUT.print(".");
#endif
	}
#ifdef USE_DEBUG
	DEBUG_OUT.printf("\nConnect WiFi failed ...\n");
#endif
	WiFi.mode(WIFI_AP);
	WiFi.softAP(MY_Config.AP_SSID, MY_Config.AP_PASS);
	delay(1000);
	blink_mode = 0B00000101;
#ifdef USE_DEBUG
	DEBUG_OUT.println(WiFi.softAPIP());
#endif
	wifi_enable = false;
#endif
lab1:
#ifdef USE_WEBSERVER
	server.on("/", handleRoot);
	server.on("/login", handleLogin);
	server.on("/setparam", handleParam);
	server.on("/reboot", HTTP_handleReboot);
	//{
	//	server.send(200, "text/plain", "this works without need of authentification");
	//}
	//);
	//server.onNotFound(handleNotFound);
	//here the list of headers to be recorded
	const char * headerkeys[] = { "User-Agent","Cookie"};
	size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
	//ask server to track these headers
	server.collectHeaders(headerkeys, headerkeyssize);
	server.begin();
#endif
#ifdef USE_DEBUG
	DEBUG_OUT.println("HTTP server started");
#endif
	//*********************************** DHT ***********************************************************
#ifdef USE_DHT
	// Инициализация DHT
#ifdef USE_DEBUG
	DEBUG_OUT.printf("Init DHT21 on %d ... ", PIN_DHT);
#endif
	//lab2:
	dht.begin();
	delay(500);
	ret = (long)dht.readTemperature();
	//   DEBUG_OUT.printf(" %ld ",ret);
	if (ret != ERROR_VALUE)
	{
		dht_enable = true;
#ifdef USE_DEBUG
		DEBUG_OUT.println("ok");
#endif
	}
	else
	{
#ifdef USE_DEBUG
		DEBUG_OUT.println("fail");
#endif
	}
	if (dht_enable)
	{
		_dht.temp = dht.readTemperature();
		_dht.hum = dht.readHumidity();
	}
#endif
	//***************************************************************

	time_next = now();									//определяем время следующего измерения
#ifdef USE_WIFI_CLIENT
	wifi_time_next_chk = now() + WIFI_CHK_INTERV;		//определяем время след.проверки wi-fi
#endif
	//********************************************
	// проверка доступности ОМ-310
	//********************************************
#ifdef USE_OM310
#ifdef USE_DEBUG
	DEBUG_OUT.print("TEST OM310...");
#endif
	OM_CHK();

#endif
	//***********************************************
	//**********************************************
#ifdef USE_DEBUG
	delay(5000);
	DEBUG_OUT.printf("\n\nFree memory %d\n", ESP.getFreeHeap());
	DEBUG_OUT.println("");
#endif

}

//*********************************************************************************
void pIP_send()
//*********************************************************************************
{
	while (client.available())
	{
		char c = client.read();
	}
	client.stop();
	delay(100);
	phttpRequest();
}
//********************************************
void phttpRequest()
//***************************************
{
	char str_data[200];

	if (client.connect(Server, 80))
	{
		//#if defined( USE_IP_DEBUG) && defined(DEBUG_OUT) //&& defined ( USE_DEBUG_OUT)
		//		DEBUG_OUT << F(" connected") << endol;
		//#endif
		// Make a HTTP request:arduino/accept.php?
		// client.println("GET /search?q=arduino HTTP/1.0");
		//client.println("GET /arduino/accept.php?temp=1111&pres=2222 HTTP/1.0");
		//client.println();
		//http://192.168.1.60/accept.php?DT=12&DA_temp=45&BM_temp=128&SHT_temp=125&BM=11.1&SHT=12.2
		//  client<<"GET /accept.php?DD="<< now();
//		DEBUG_OUT.println("***************\nSEND TO SRV01");
		tmElements_t tt;
		breakTime(now(), tt);
		client.print("GET /arduino/accept_sonoff.php?DT=");
		client.print(tmYearToCalendar(tt.Year));
		client.print('-');
		client.print(tt.Month);
		client.print('-');
		client.print(tt.Day);
		client.print("%20");
		client.print(tt.Hour);
		client.print(':');
		client.print(tt.Minute);
		client.print(':');
		client.print(tt.Second);
#ifdef USE_DHT
		client.print("&T=");
		client.print(_dht.temp);
		client.print("&SHT=");
		client.print(_dht.hum);
#endif
	/*	DEBUG_OUT.println(now());
		DEBUG_OUT.print("GET /arduino/accept_sonoff.php?DT=");
		DEBUG_OUT.print(tmYearToCalendar(tt.Year));
		DEBUG_OUT.print('-');
		DEBUG_OUT.print(tt.Month);
		DEBUG_OUT.print('-');
		DEBUG_OUT.print(tt.Day);
		DEBUG_OUT.print("%20");
		DEBUG_OUT.print(tt.Hour);
		DEBUG_OUT.print(':');
		DEBUG_OUT.print(tt.Minute);
		DEBUG_OUT.print(':');
		DEBUG_OUT.print(tt.Second);
		DEBUG_OUT.print("&T=");
		DEBUG_OUT.print(_dht.temp);
		DEBUG_OUT.print("&SHT=");
		DEBUG_OUT.print(_dht.hum);*/

		//sprintf(str_data, "GET /arduino/accept_sonoff.php?DT=%4u-%2d-%2d%%20%2d:%2d:%2d&T=", tt.Year, tt.Month, tt.Day, tt.Hour, tt.Minute, tt.Second);
		//client.print(str_data);
		//DEBUG_OUT.print(str_data);
		///*dtostrf(_dht.temp, 3, 2, str_data);
		//client.print(str_data);
		//DEBUG_OUT.print(str_data);
		//client.print("&SHT=");
		//DEBUG_OUT.print("&SHT=");
		//dtostrf(_dht.hum, 3, 0, str_data);
		//client.print(str_data);
		//DEBUG_OUT.println(str_data);*/

		client.println(F(" HTTP/1.1"));
				client.println(F("Host: srv01.myhome.local"));
		client.println(F("User-Agent: arduino-ethernet"));
		client.println(F("Connection: close"));
		client.println();
	}
	// если не приконнектились to the server:
//	else
//	{
//#ifdef USE_LCD
//		lcd.setCursor(18, 0);
//		lcd.print("\xa0");
//#endif
//		//#if defined( USE_IP_DEBUG) // && defined ( USE_DEBUG_OUT)
//		//		DEBUG_OUT << F("connection failed.\n");
//		//#endif		
//	}
//#if defined( USE_IP_DEBUG)
//	DEBUG_OUT << F("close\n");
//#endif
//	//	client.stop();//читаем ответ сервера
}



//************************************************************
void loop()
//************************************************************
{

	time_t ntime = now();	//смотрим текущее время
	uint32_t ms = millis();
	// Событие срабатывающее каждые 125 мс   
	//************************************************ кусок темного кода *************
	if ((ms - ms1) > 125 || ms < ms1)
	{
		ms1 = ms;
		// Режим светодиода ищем по битовой маске       
		//if (blink_mode & 1 << (blink_loop & 0x07)) digitalWrite(PIN_LED2, LOW);
		//else  digitalWrite(PIN_LED2, HIGH);
		if (blink_mode & 1 << (blink_loop & 0x07)) digitalWrite(PIN_LED2, LOW);
		else  digitalWrite(PIN_LED2, HIGH);
		blink_loop++;
	}
	if (ms < ms2 || (ms - ms2) > 500)
	{
		ms2 = ms;
		if (WiFi.status() == WL_CONNECTED)blink_mode = 0B11111111;
		else blink_mode = 0B00000101;
	}
	//************************************************ кнец темного кода ****************
//********************** интервал измерения ***********************************
	if (ntime >= time_next)
	{
#ifdef USE_DHT
		_dht.temp = dht.readTemperature();
		//	int16_t temp_i = _dht.temp * 100;
		_dht.hum = dht.readHumidity();
		if (_dht.temp != ERROR_VALUE)
		{
			// проверка на достижение влажности границы включения вентилятора
			if ((relay_on == false) && (_dht.hum >= MY_Config.VENT.hum_on))
			{
				// включить вентилятор
				relay_on = !relay_on;
				digitalWrite(PIN_RELAY, relay_on);
			}
			else
				if ((relay_on) && (_dht.hum <= MY_Config.VENT.hum_off))
				{
					//выключить вентилятор
					relay_on = !relay_on;
					digitalWrite(PIN_RELAY, relay_on);
				}
			//******************************************************
			//отправка на HTTP SERVER
			pIP_send();

			// здесь вставить отправку  mqtt
#ifdef USE_MQTT
//пытаемся коннектится
			if (MQTT_client.connect("ESP8266Client") && wifi_enable)
			{
				//		Serial.println("connected");
						// Once connected, publish an announcement...
				char str_data[20];
				//sprintf(str_data, "temp: %f<br>hum: %f<br>", _dht.temp, _dht.hum);
				//DEBUG_OUT.println(str_data);
				dtostrf(_dht.temp, 3, 2, str_data);
				MQTT_client.publish("A108/temp", str_data);
				dtostrf(_dht.hum, 3, 2, str_data);
				MQTT_client.publish("A108/hum", str_data);
				// ... and resubscribe
		//		client.subscribe("inTopic");
			}

#endif
#ifdef USE_DEBUG	
			DEBUG_OUT.print("Temperature=");
			DEBUG_OUT.print(dht.readTemperature(), 1);
			DEBUG_OUT.println(" C");
			DEBUG_OUT.print("Humidity=");
			DEBUG_OUT.print(dht.readHumidity(), 0);
			DEBUG_OUT.println("%");
#endif		
		}
		else
		{
#ifdef USE_DEBUG
			DEBUG_OUT.println("DHT ERROR");
#endif
		}
#endif
		time_next = ntime + MEANS_INTERV;
	}

	//***************************************************************************
	//     периодическая проверка подключения к wi-fi
	if ((now() >= wifi_time_next_chk) && (wifi_enable))//если пришло время проверить wi-fi
	{
		if ((WiFi.getMode() == WIFI_STA) && (WiFi.status() != WL_CONNECTED))	//если нет коннекта
		{
			wifi_enable = false;
			if (wifi_chk_cnt < 5)
			{
				WiFi.begin(MY_Config.STA_SSID, MY_Config.STA_PASS);	//пытаемся приконнектиться
	//				wifi_time_next_chk = now() + CHK_WIFI_INTERV;		//о
				wifi_chk_cnt++;										//счетчик попыток
			}
			else
			{
				// переход в режим точки доступа
				WiFi.mode(WIFI_AP);
				WiFi.softAP(MY_Config.AP_SSID, MY_Config.AP_PASS);
			}
		}
		else			//коннект есть
		{
			wifi_chk_cnt = 0;
		}
	//		digitalWrite(PIN_LED2, wifi_enable);
		wifi_time_next_chk = now() + CHK_WIFI_INTERV;		//следующее время проверки
	}
	//**********************************************************************************
	//     обработка запросов от браузера
#ifdef USE_WEBSERVER
	server.handleClient();
#endif
	//***********************************************************************
	//синхронизация времени
	//***********************************************************************
	if ((now() >= next_ntp) && !send_NTP)// если не посылали пакет
	{

		NTPSend();
		send_NTP = true;
		next_ntp = now() + 1;
	}
	if (send_NTP)	//пакет отправляли	
	{
		ntp_time = NTPGet();//проверяем - приехал ли пакет
		if (ntp_time != 0)
		{
			setTime(ntp_time);
			next_ntp = now() + NTP_interval;
			send_NTP = false;
#ifdef USE_DEBUG
			tmElements_t tt;
			breakTime(next_ntp, tt);
			//				DEBUG_OUT.println("NEXT SET time");
			DEBUG_OUT.printf("NEXT SET system time: %d.%d.%d %d:%d:%d\n", tt.Day, tt.Month, tt.Year, tt.Hour, tt.Minute, tt.Second);
#endif

		}
		else
			if (now() >= next_ntp)	//если время ожидания истекло
			{	//переключаемся на другой сервер
				NTP_INDEX++;
				if (NTP_INDEX >= 4)
					NTP_INDEX = 0;
//				DEBUG_OUT.println("CHANGE NTP");
				NTPSend();
				send_NTP = true;
				next_ntp = now() + 1;
			}

	}


}

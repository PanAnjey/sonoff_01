#pragma once
#ifndef _MY_HTTP_
#define _MY_HTTP_
//void HTTP_printHeader(String &out, const char *title, uint16_t refresh);
void HTTP_printTail(String &out);

void HTTP_printInput(String &out, const char *label, const char *name, const char *value, int size,  bool is_pass);
void HTTP_printInput(String &out, const char *label, const char *name, const float value, const uint8_t width, const uint8_t prec, int size, bool is_pass);
void HTTP_printInput(String &out, const char *label, const char *name, const int8_t value,  int size, bool is_pass);
void HTTP_printInput_mac(String &out, const char *label, const char *name, const uint8_t value, int size, bool is_pass);

//***********************************************************************************************************************
void HTTP_printInput0(String &out, const char *label, const char *name)
//***********************************************************************************************************************
{
	out += "<tr><td width=50%><h3>";
	out += label;
	out += "</h3></td>";
	out += "<td width=50%><input name ='";
	out += name;
	out += "' value='";
}

//***********************************************************************************************************************
void HTTP_printInput1(String &out, int size, bool is_pass)
//***********************************************************************************************************************
{
	char str[10];
	out += "' size=";
	sprintf(str, "%d", size);
	out += str;
	if (is_pass)out += " type='password'";
	out += "></td></tr>\n";
}
//***********************************************************************************************************************
// Вывод в буфер одного поля формы
// out		- строка результат
// label	- отображаемый текст в таблице
// name		- имя поля формы
// value	- отображаемое значение поля
// size		- размер поля ввода
// len		- размер ячейки таблицы
// is_pass	- признак поля-пароля.
void HTTP_printInput(String &out, const char *label, const char *name, const char *value, int size, bool is_pass)
//************************************************************************************************************************
{
	HTTP_printInput0(out, label, name);
	out += value;
	HTTP_printInput1(out, size, is_pass);
}

void HTTP_printInput_mac(String &out, const char *label, const char *name, const uint8_t value, int size, bool is_pass)
//************************************************************************************************************************
{
	out += "<td>";
	out += label;
	out += "</td>";
	out += "<td><input name ='";
	out += name;
	out += "' value='";
	out += value;
	char str[10];
	out += "' size=";
	sprintf(str, "%d", size);
	out += str;
	out += "></td>\n";
}
//************************************************************************************************************************
void HTTP_printInput(String &out, const char *label, const char *name, const float value, const uint8_t width, const uint8_t prec, int size, bool is_pass)
//************************************************************************************************************************
{
	char str[10];
	dtostrf(value, width, prec, str);
	HTTP_printInput0(out, label, name);
	out += str;
	HTTP_printInput1(out, size, is_pass);
}

//************************************************************************************************************************
void HTTP_printInput(String &out, const char *label, const char *name, const int8_t value,  int size, bool is_pass)
//************************************************************************************************************************
{
	char str[10];
//	dtostrf(value, width, prec, str);
	HTTP_printInput0(out, label, name);
sprintf(str, "%d", value);
out += str;
	HTTP_printInput1(out, size, is_pass);
}



//*************************************************************************************************************************
// Выаод окончания файла HTML
void HTTP_printTail(String &out)
//************************************************************************************************************************* 	
{
	out += "</body></html>";
}
#endif // !_MY_HTTP_
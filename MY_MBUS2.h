//***********************************************************
//  для прямой работы с Меркурием
//***********************************************************

#pragma once
#ifndef _MBUS2_
#define _MBUS2_
#include <MY_MODBUS\MY_MODBUS_BASE.h>
#if defined(__AVR_ATmega2560__)
//#define MBUS Serial2
#define MBUS_SPEED 9600
//#define DEBUG_OUT Serial
#endif
class MBUS2
{
public:
	void  write();
	void write(uint8_t inByte);
	void  read();
	void read_from_comp();
};

//*********************************************************************************
void MBUS2::read_from_comp()
//*********************************************************************************
{
	long _tmout;
	Mbus._buf_i = 0;
lab1:
	if (Mbus._buf_i == 0)
	{
		Mbus._tmout = millis() + 50;
	}
	else
	{
		_tmout = millis() + 30;
	}
	while (millis() < _tmout)
	{
		if (COMP.available())
			goto lab2;
	}
	Mbus._buf_send = Mbus._buf;
	Mbus._buf_i_send = Mbus._buf_i;
	write();
	return;
lab2:
	Mbus._buf[Mbus._buf_i++] = COMP.read();
	goto lab1;
}



//*********************************************************************************
void MBUS2::write()
//*********************************************************************************
{
	while (MBUS.available())
	{
		int thisByte = MBUS.read();

	}
	Mbus.enable();
	for (int i = 0; i < Mbus._buf_i_send; i++)
	{
		MBUS.write(Mbus._buf_send[i]);
	}
	MBUS.flush();
	Mbus.disable();
}
//*********************************************************************************
void MBUS2::write(uint8_t inByte)
//*********************************************************************************
{
	Mbus.enable();
	MBUS.write(inByte);
//	MBUS.flush();
	Mbus.disable();
	delay(20);
}
//*********************************************************************************
void MBUS2::read()
//*********************************************************************************
{
	long _tmout;
	Mbus._buf_i = 0;
lab1:
	if (Mbus._buf_i == 0)
	{
	   _tmout = millis() + 50;
	}
	else
	{
		_tmout = millis() + 30;
	}
	while (millis() < _tmout)
	{
		if (MBUS.available())
			goto lab2;
	}

	for (size_t i = 0; i < Mbus._buf_i; i++)
	{
		COMP.write(Mbus._buf[i]);
	}
#if defined (DEBUG_OUT)
	DEBUG_OUT.print("MBUS>");
	for (size_t i = 0; i < Mbus._buf_i; i++)
	{
		if (Mbus._buf[i] < 16)
		{
			DEBUG_OUT.print("0");
		}
		DEBUG_OUT.print(Mbus._buf[i], HEX);
		DEBUG_OUT.print("; ");
	}
	DEBUG_OUT.println();
#endif

	return;
lab2:
	Mbus._buf[Mbus._buf_i++] = MBUS.read();
	goto lab1;

}

#endif

/*************************************************************************
 * RTD shield libary - Version 1
 * by Christodoulos P. Lekkos <tolis81@gmail.com> , August 10, 2012.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * published by the Free Software Foundation.
*************************************************************************/

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
 
#include <RTD_v1.h>
#include <../SPI/SPI.h>

#define WIRE_COMP_OFFSET -0.7 // mV
#define VREF 2.31 // reference voltage (V)
#define IEX 0.9537 // excitation current (mA)
#define EOCPIN 12 // !EOC pin for conversion monitoring

const int _muxPin[] = {5, 4, 3}; // pins used for multiplexing
const int _csPin = 10; // ADC cs pin
int _muxPinState[] = {0, 0, 0}; // array to store current channel configuration

RTD::RTD(int nChannel)
{
	_nChannel = nChannel;
	for (int i = 0; i < 3; i++) pinMode(_muxPin[i], OUTPUT); // configure the multiplexer pins
}
 
void RTD::ADC_INIT() // function to initialize a measurement after the appropriate channel has been choosen
{
	digitalWrite(_csPin, LOW);
	byte initByte = SPI.transfer(0x00);
	digitalWrite(_csPin, HIGH);
}
 
void RTD::MUX_SELECT(int _nChannel) // function for channel selection
{
	_muxPinState[0] = bitRead(_nChannel,0);
	_muxPinState[1] = bitRead(_nChannel,1);  
	_muxPinState[2] = bitRead(_nChannel,2);
	digitalWrite(_muxPin[0], _muxPinState[0]);
	digitalWrite(_muxPin[1], _muxPinState[1]);
	digitalWrite(_muxPin[2], _muxPinState[2]);
	//Serial.print("Channel Selected: "); // uncomment for debugging
	//Serial.println(_nChannel); // uncomment for debugging
}
 
long RTD::ADC_READ() // function to read the ADC
{
	digitalWrite(_csPin, LOW);
	if (digitalRead(EOCPIN))
	{
		//Serial.println("!EOC"); // uncomment for debugging
		digitalWrite(_csPin, HIGH);
		ADC_READ();
	}
	else
	{
		byte adcByte = SPI.transfer(0x00);
		//Serial.println(adcByte, BIN); // uncomment for debugging
		adcByte &= 0x0F;
		//Serial.println(adcByte, BIN); // uncomment for debugging
		long adcValue = adcByte << 8;
		adcByte = SPI.transfer(0x00);
		//Serial.println(adcByte, BIN); // uncomment for debugging
		adcValue |= adcByte;
		adcValue <<= 8;
		adcByte = SPI.transfer(0x00);
		//Serial.println(adcByte, BIN); // uncomment for debugging
		adcValue |= adcByte;
		digitalWrite(_csPin, HIGH);
		return adcValue;
	}
}

double RTD::ADC_2_mV(long adcRaw) // convert raw ADC data to mV
{
	double mV =  (double)adcRaw*0.00000095367431640625*(double)VREF*1000 + (double)WIRE_COMP_OFFSET;
	//Serial.println(mV); // uncomment for debugging
	return mV;
}

double RTD::OHM2CELSIUS(double mVadc) // calculate temperature from mV (degree Celcius)
{
	double ohm = mVadc / (double)IEX;
	double celsius = (0.000000003*ohm*ohm*ohm*ohm - 0.000001*ohm*ohm*ohm + 0.0013*ohm*ohm + 2.3141*ohm - 243.85);
	//double celsius = 3*pow(10, -9)*pow(ohm, 4)-1*pow(10, -6)*pow(ohm, 3)+0.0013*pow(ohm, 2)+2.3141*ohm - 243.85;
	//Serial.println(ohm); // uncomment for debugging
	return celsius;
}

double RTD::tempC() // main function to read the RTD temperature in degree Celsius
{
	MUX_SELECT(_nChannel);
	ADC_INIT();
	long adcRaw = ADC_READ();
	double rtdTempC = OHM2CELSIUS(ADC_2_mV(adcRaw));
	return rtdTempC;
}

double RTD::tempK() // convert to Kelvin
{
	double rtdTempK = tempC();
	rtdTempK += 273.0;
	return rtdTempK;
}

double RTD::tempF() // convert to degree Fahrenheit
{
	double rtdTempF = tempC();
	rtdTempF = (rtdTempF*9/5)+32;
	return rtdTempF;
}
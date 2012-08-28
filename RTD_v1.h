/*************************************************************************
 * RTD shield libary - Version 1
 * by Christodoulos P. Lekkos <tolis81@gmail.com> , August 10, 2012.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * published by the Free Software Foundation.
*************************************************************************/

#ifndef RTD_v1_h
#define RTD_v1_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class RTD
{
	public:
		RTD(int nChannel);
		double tempC();
		double tempK();
		double tempF();
	private:
		int _nChannel;
		void MUX_SELECT(int);
		void ADC_INIT();
		double ADC_2_mV(long);
		double OHM2CELSIUS(double);
		long ADC_READ();
		long _adcRaw;
};
 
 #endif 

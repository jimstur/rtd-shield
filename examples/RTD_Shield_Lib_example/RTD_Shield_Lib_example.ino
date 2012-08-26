/***************************************************************************************
 * RTD Shield Example
 * This example creates and reads 4 RTD instances
 * The temperature output is displayed in different scales
 * 
 * - create instances of RTD class like: RTD instanceName(channel) - channel[0-3]
 * - display temperature in 
 *   e.g degree Celsius by calling the appropriate function like: instanceName.tempC();
 *
 * created 26 Aug 2012 
 * by Christodoulos P. Lekkos
 ***************************************************************************************/
 
#include <SPI.h> // include the SPI library for the LTC2420 ADC communication
#include <RTD_v1.h> // include the RTD library

RTD furnaceRTD(0);
RTD ambientRTD(1);
RTD waterTankRTD(2);
RTD nitrogenGasFlowRTD(3);

double rtdTemp[] = {0, 0, 0, 0}; // array to store the temperature measurements

void setup() {
  SPI.begin();  // initialize SPI interface
  Serial.begin(9600); /// initialize serial communication
}

void loop() {
  rtdTemp[0] = furnaceRTD.tempC(); // compute temperature in degree Celsius
  rtdTemp[1] = ambientRTD.tempF(); // compute temperature in degree Fahrenheit
  rtdTemp[2] = waterTankRTD.tempC(); // compute temperature in degree Celsius
  rtdTemp[3] = nitrogenGasFlowRTD.tempK(); // // compute temperature in Kelvin
  
  for (int i= 0; i < 4; i++) { // Print the temperatures
    Serial.println(rtdTemp[i]);
  }
  delay(1000); // update values every 1 second
}
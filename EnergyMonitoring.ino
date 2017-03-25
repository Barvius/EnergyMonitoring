#include "EmonLib.h"                   // Include Emon Library
#include "OneWireHub.h"
#include "DS18B20.h"  // Digital Thermometer, 12bit

EnergyMonitor emon1;                   // Create an instance
constexpr uint8_t pin_onewire   { 8 };

float I_value=0;
float U_value=0;
long previousMillis = 0;
long interval = 1000;

auto hub    = OneWireHub(pin_onewire);

auto I = DS18B20(DS18B20::family_code, 0x00, 0x00, 0xB2, 0x18, 0xDA, 0x00);
auto U = DS18B20(DS18B20::family_code, 0x00, 0x00, 0xB2, 0x18, 0xDA, 0x01);

void setup(){  
  Serial.begin(9600);
  emon1.current(3, 26); // Current: input pin, calibration.
  emon1.voltage(2, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  hub.attach(I);
  hub.attach(U);
}


void loop(){
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor) 
    I_value=emon1.Irms; 
    U_value=emon1.Vrms;    
  } 
  hub.poll();
  I.setTemperature(I_value);
  U.setTemperature(U_value/10.0f);
}
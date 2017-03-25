#include <MsTimer2.h>
#include "EmonLib.h"                   // Include Emon Library
#include "OneWireHub.h"
#include "DS18B20.h"  // Digital Thermometer, 12bit

EnergyMonitor emon1;                   // Create an instance
constexpr uint8_t pin_onewire   { 8 };
float val=0;

auto hub    = OneWireHub(pin_onewire);

auto I = DS18B20(DS18B20::family_code, 0x00, 0x00, 0xB2, 0x18, 0xDA, 0x00); // DS18B20: 9-12bit, -55 -  +85 degC

void setup(){  
  Serial.begin(9600);
  emon1.current(3, 26); // Current: input pin, calibration.
  hub.attach(I);
  I.setTemperature(int8_t(0));
  MsTimer2::set(1000, timerInterupt);
  MsTimer2::start();
}

void  timerInterupt() {
  val= emon1.calcIrms(1480);   
}

void loop(){
  hub.poll();
  I.setTemperature(val);
}

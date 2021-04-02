#include "sensors.h"
#include <Adafruit_MCP3008.h>

Adafruit_MCP3008 adc;
int count = 0;

Sensors::Sensors(byte cs_pin) {
  this->cs_pin = cs_pin;
}
void Sensors::init() {
  adc.begin(13, 11, 12, cs_pin);
}

int Sensors::record(int rate, int num_channels) {
  for (int chan=0; chan<num_channels; chan++) {
    Serial.print(adc.readADC(chan)); Serial.print("\t");
  }

  Serial.print("["); Serial.print(count); Serial.println("]");
  count++;
  
  delay(rate);

  return count;
}

void Sensors::off() {
  
}

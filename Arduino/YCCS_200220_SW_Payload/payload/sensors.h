#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

class Sensors {
    private:
      byte cs_pin;
      
    public:
      Sensors (byte cs_pin);

      void init();
      void off();
      int record(int rate, int num_channels);
      
      
};

#endif

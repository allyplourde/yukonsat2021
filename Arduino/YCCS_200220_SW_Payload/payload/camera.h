#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <ArduCAM.h>

class Camera {
    private:
      byte cs_pin;
      byte sd_pin;
      ArduCAM myCAM;
    public:
      Camera (byte cs_pin, byte sd_pin);
      void init();
      int capture();
};

#endif

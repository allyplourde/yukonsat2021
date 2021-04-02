#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

class Storage {
    private:
      byte cs_pin;
      
    public:
      Storage (byte cs_pin);

      void init();
      byte storeDataString(String data, String file_name);
      void uploadImage();
      
      
};

#endif

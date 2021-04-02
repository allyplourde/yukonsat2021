#ifndef SCREEN_H
#define SCREEN_H
#include <NHD_SEPS525.h>
#include <SD.h>

#include <Arduino.h>

class Screen {
    private:
      byte OLED_CS;
      byte SD_CS;

      uint16_t read16(File f);
      uint32_t read32(File f);
      void Screen::displaySend(uint8_t sendType, uint32_t v, bool relCS = true, uint8_t numBits = 8);
      
    public:
      Screen (byte oled_cs, byte sd_cs);

      void init();
      void off();
      //int healthcheck(int rate, int num_channels);
      void loadImage(char *filename="profile.bmp", uint8_t x=0, uint8_t y=0);
      void reset();
      
      
};

#endif

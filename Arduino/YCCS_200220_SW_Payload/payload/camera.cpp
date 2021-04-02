#include "camera.h"

#include <ArduCAM.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define OV2640_MINI_2MP_PLUS
#define   FRAMES_NUM    0x06
bool is_header = false;
int total_time = 0;
uint8_t read_fifo_burst(ArduCAM myCAM);
uint8_t vid, pid;
uint8_t temp;

Camera::Camera(byte cs_pin, byte sd_pin) {
  ArduCAM myCAM( OV2640, cs_pin );
  this->cs_pin = cs_pin;
  this->sd_pin = sd_pin;
  this->myCAM = myCAM;
  init();
}
void Camera::init() {
  
  Wire.begin();

  Serial.println(F("ArduCAM Start!"));
  // set the CS as an output:
  pinMode(cs_pin, OUTPUT);
  digitalWrite(cs_pin, HIGH);
  // initialize SPI:
  SPI.begin();    

  myCAM.set_format(JPEG);
  //myCAM.InitCAM();  
  //myCAM.OV2640_set_JPEG_size(OV2640_320x240);
  //myCAM.clear_fifo_flag();
  //myCAM.write_reg(ARDUCHIP_FRAMES,0x00);

}//end init

int Camera::capture() {

  while (!SD.begin(sd_pin)) {
    Serial.println(F("SD Card Error!")); delay(1000);
  }

  return 1;
}

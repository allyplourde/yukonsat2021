#include <Adafruit_GFX.h>
#include <NHD_SEPS525.h>
#include <SD.h>
#include <SPI.h>

#include "screen.h"

// If we are using the hardware SPI interface, these are the pins (for future ref)
// may not need all definitions, should come from payload instead
#define SCK 13
#define MOSI 11
//#define cs   5
#define RES_PIN  6
#define DC_PIN 4

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF
#define PURPLE          0x780F

// to draw images from the SD card, we will share the hardware SPI interface
SEPS525 oled_driver;

// the file itself
File bmpFile;

// information we extract about the bitmap file
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;


Screen::Screen(byte oled_cs, byte sd_cs) {
  this->OLED_CS = oled_cs;
  this->SD_CS = sd_cs;
}
void Screen::init() {
  // initialize the OLED
  oled_driver.begin();  
  oled_driver.fillScreen(PURPLE);

  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
  pinMode(RES_PIN, OUTPUT);
  pinMode(OLED_CS, OUTPUT);

  reset();

}

void Screen::off() {
  oled_driver.fillScreen(BLACK); //needs to be changed to reset -> low power mode
}

#define MODE_4WIRE    2   // PS=0, needs the RS (D/C) signal; SLOOOOOOW...
#define MODE_SPI4W    3   // PS=0, needs the RS (D/C) signal; FAST HW SPI!

#define SEND_CMD      1   // 4-wire - Display instruction (command)
#define SEND_DAT      2   // 4-wire - Display instruction (data)
int SIG_MODE = MODE_SPI4W;  // can only do 16 bits color in 8-bit SPI hardware

void Screen::displaySend(uint8_t sendType, uint32_t v, bool relCS = true, uint8_t numBits = 8)
{
  unsigned char i;
  uint32_t mask = 1<<(numBits-1);

  digitalWrite(OLED_CS, LOW);

  if (sendType == SEND_CMD)
  { // Send a command value
      digitalWrite(DC_PIN, LOW);
  }
  else if (sendType == SEND_DAT)
  { // Send a data value
      digitalWrite(DC_PIN, HIGH);
  }

  if (SIG_MODE == MODE_4WIRE)
  {
    for(i=numBits;i>0;i--)
    { // Decrementing is faster
      digitalWrite(SCK, LOW);
      if((v&mask)>>(numBits-1)==1)
      {
        digitalWrite(MOSI, HIGH);
      }
      else
      {
        digitalWrite(MOSI, LOW);
      }
      v=v<<1;
      digitalWrite(SCK, HIGH);
    }
  }
  else if (SIG_MODE == MODE_SPI4W) {
    SPI.transfer(v);
  }
  
  if (relCS) {
    digitalWrite(OLED_CS, HIGH);
  }
}

void Screen::reset()
{
  digitalWrite(RES_PIN, HIGH);
  delay(100);

  // Directly from the specs
  displaySend(SEND_CMD, 0x04); // 
  displaySend(SEND_DAT, 0x03); // = 
  delay(2);

  displaySend(SEND_CMD, 0x04); // 
  displaySend(SEND_DAT, 0x04); // = orig = 0x00 (0x04 saves power)
  delay(2);

  displaySend(SEND_CMD, 0x3B); // 
  displaySend(SEND_DAT, 0x00); // = 

  displaySend(SEND_CMD, 0x02); // 
  displaySend(SEND_DAT, 0x01); // = 

  displaySend(SEND_CMD, 0x03); // 
  displaySend(SEND_DAT, 0x90); // = 

  displaySend(SEND_CMD, 0x80); // 
  displaySend(SEND_DAT, 0x01); // = 

  displaySend(SEND_CMD, 0x08); // 
  displaySend(SEND_DAT, 0x04); // = 
  displaySend(SEND_CMD, 0x09); // 
  displaySend(SEND_DAT, 0x05); // = 
  displaySend(SEND_CMD, 0x0A); // 
  displaySend(SEND_DAT, 0x05); // = 

  displaySend(SEND_CMD, 0x0B); // 
  displaySend(SEND_DAT, 0x9D); // = 
  displaySend(SEND_CMD, 0x0C); // 
  displaySend(SEND_DAT, 0x8C); // = 
  displaySend(SEND_CMD, 0x0D); // 
  displaySend(SEND_DAT, 0x57); // = 

  displaySend(SEND_CMD, 0x10); // 
  displaySend(SEND_DAT, 0x56); // = 
  displaySend(SEND_CMD, 0x11); // 
  displaySend(SEND_DAT, 0x4D); // = 
  displaySend(SEND_CMD, 0x12); // 
  displaySend(SEND_DAT, 0x46); // = 

  displaySend(SEND_CMD, 0x13); // 
  displaySend(SEND_DAT, 0x00); // = orig 0x0A - BIG difference!

  displaySend(SEND_CMD, 0x14); // 
  displaySend(SEND_DAT, 0x01); // = orig 0x01 - appears irrelevant in 4-wire mode
  displaySend(SEND_CMD, 0x16); // 
  displaySend(SEND_DAT, 0x66); // = orig 0x76 = 3-bytes 262K colors, 0x66 = 2-bytes (65K colors)

  displaySend(SEND_CMD, 0x20); // 
  displaySend(SEND_DAT, 0x00); // = 
  displaySend(SEND_CMD, 0x21); // 
  displaySend(SEND_DAT, 0x00); // = 

  displaySend(SEND_CMD, 0x28); // 
  displaySend(SEND_DAT, 0x7F); // = 

  displaySend(SEND_CMD, 0x29); // 
  displaySend(SEND_DAT, 0x00); // = 

  displaySend(SEND_CMD, 0x06); // 
  displaySend(SEND_DAT, 0x01); // = 

  displaySend(SEND_CMD, 0x05); // 
  displaySend(SEND_DAT, 0x00); // = 

  displaySend(SEND_CMD, 0x15); // 
  displaySend(SEND_DAT, 0x00); // = 

  delay(10);
}








// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void Screen::loadImage(char *filename, uint8_t x, uint8_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, _row, _col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= col) || (y >= row)) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= col)  w = col  - x;
        if((y+h-1) >= row) h = row - y;

        for (_row=0; _row<h; _row++) { // For each scanline...
          //oled_driver.goTo(x, y+_row);

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - _row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + _row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          // optimize by setting pins now
          for (_col=0; _col<w; _col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];

            oled_driver.drawPixel(x+_col, y+_row, oled_driver.color565(r,g,b));
            // optimized!
            //tft.pushColor(oled_driver.color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t Screen::read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t Screen::read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

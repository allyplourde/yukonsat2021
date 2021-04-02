/*
 * CubeSat slave Arduino UNO encoder 
 * Reads signals from motor's encoder and determines position and direction of rotor movement
 * Transmits encoder information to Master device upon request (MotorOps_Master_Predriver)
 */

#include <Wire.h>

#define PINA 2
#define PINB 4
#define PINI 3
#define INTERRUPTA 0  // pin 2 on Arduino UNO
#define INTERRUPTI 1  // pin 3 on Arduino UNO

// Encoder count variables
volatile boolean fired;
volatile boolean up;
volatile boolean rst;
volatile long encoderValue = 0; 
int encoderValueOut = 0;        // Encoder value to send to Master devices

// I²C variables 
const byte SlaveDeviceId = 1;

void setup() {
  // Start I²C bus as a slave
  Wire.begin(SlaveDeviceId);

  // Set the callback to call when data is requested.
  Wire.onRequest(requestCallback);

  // Pin declarations turn on inner pullups so that I²C is read properly
  pinMode(PINA, INPUT_PULLUP);
  pinMode(PINB, INPUT_PULLUP);
  pinMode(PINI, INPUT_PULLUP);

  // Declare interrupt functions which trigger on voltage change one with the A phase and one with the Index phase
  attachInterrupt (INTERRUPTA, updateEncoder, CHANGE);   // interrupt 0 is pin 2, interrupt 1 is pin 3
  attachInterrupt (INTERRUPTI, homeEncoder, CHANGE); 
 
 Serial.begin (115200);
}

void loop() {
  // Changes encoder value if a change is detected
  if(fired){
    // Increments od decrements encoder value
    if(up){
      encoderValue++;
    } else {
      encoderValue--;
    }
    fired = false; 

    // Maps encoder values to create all positive values creating an absolute position from 0-1024
    if(encoderValue <= 0){
      encoderValueOut = map(encoderValue,0,-1024,1024,0);
    } else {
      encoderValueOut = encoderValue;
    }

    //Debugging
    Serial.print ("Count = ");  
    Serial.print (encoderValue);
    Serial.print(",");
    Serial.println(encoderValueOut);
  } 

}

void updateEncoder(){
  // Signals a position change and determines which direction
  if(digitalRead(PINA)){
    up = digitalRead (PINB); 
  }else{
    up = !digitalRead (PINB);
  }
  fired = true;
}

void homeEncoder(){
  // The home position is considered as 0
  encoderValue = 0;
}

void requestCallback()  {
  // Send two bytes to master
  int input = encoderValueOut;
  uint8_t buffer[2];
  buffer[0] = input >> 8;
  buffer[1] = input & 0xff;  
  Wire.write(buffer, 2);
}

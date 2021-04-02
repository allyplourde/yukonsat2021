/*
 * CubeSat master Arduino UNO Controller and Pre-driver
 * Requests encoder information from slave device (MotorOps_Slave_Encoder)
 * Moves motor at constant speed to certain input position from serial monitor
 */

#include <Wire.h>

// I²C variables 
const byte SlaveDeviceId = 1;
int receivedValue;

// Serial Variables
String inString = "";

// PINS
const int motorPin1 = 9;
const int motorPin2 = 10;
const int motorPin3 = 11;
const int setpointPin = A1;

// Motor movement variables
int pos = 0;          // position of motor
boolean direct = 0;   // Direction of motor
int setpoint = 0;
int error = 0;
int increment = 0;
int currentStepA;
int currentStepB;
int currentStepC;
int sineArraySize;

// Timing Delay Variables 
unsigned long currentMillis;
unsigned long previousMillis = 0;        
const long interval = 50;

// SPWM (Sine Wave)
//const int pwmSin[] = {127, 138, 149, 160, 170, 181, 191, 200, 209, 217, 224, 231, 237, 242, 246, 250, 252, 254, 254, 254, 252, 250, 246, 242, 237, 231, 224, 217, 209, 200, 191, 181, 170, 160, 149, 138, 127, 116, 105, 94, 84, 73, 64, 54, 45, 37, 30, 23, 17, 12, 8, 4, 2, 0, 0, 0, 2, 4, 8, 12, 17, 23, 30, 37, 45, 54, 64, 73, 84, 94, 105, 116 };
// Every angle
const int pwmSin[] = {127, 129, 131, 134, 136, 138, 140, 143, 145, 147, 149, 151, 154, 156, 158, 160, 162, 164, 166, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187, 189, 191, 193, 195, 196, 198, 200, 202, 204, 205, 207, 209, 211, 212, 214, 216, 217, 219, 220, 222, 223, 225, 226, 227, 229, 230, 231, 233, 234, 235, 236, 237, 239, 240, 241, 242, 243, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 251, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 255, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 252, 252, 251, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 243, 242, 241, 240, 239, 237, 236, 235, 234, 233, 231, 230, 229, 227, 226, 225, 223, 222, 220, 219, 217, 216, 214, 212, 211, 209, 207, 205, 204, 202, 200, 198, 196, 195, 193, 191, 189, 187, 185, 183, 181, 179, 177, 175, 173, 171, 169, 166, 164, 162, 160, 158, 156, 154, 151, 149, 147, 145, 143, 140, 138, 136, 134, 131, 129, 127, 125, 123, 120, 118, 116, 114, 111, 109, 107, 105, 103, 100, 98, 96, 94, 92, 90, 88, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 63, 61, 59, 58, 56, 54, 52, 50, 49, 47, 45, 43, 42, 40, 38, 37, 35, 34, 32, 31, 29, 28, 27, 25, 24, 23, 21, 20, 19, 18, 17, 15, 14, 13, 12, 11, 11, 10, 9, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,0 ,0 ,0 ,0 , 0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 10, 11, 11, 12, 13, 14, 15, 17, 18, 19, 20, 21, 23, 24, 25, 27, 28, 29, 31, 32, 34, 35, 37, 38, 40, 42, 43, 45, 47, 49, 50, 52, 54, 56, 58, 59, 61, 63, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 88, 90, 92, 94, 96, 98, 100, 103, 105, 107, 109, 111, 114, 116, 118, 120, 123, 125, 127};
// Every half angle
//const int pwmSin[] = {127,128,129,130,131,133,134,135,136,137,138,139,140,141,143,144,145,146,147,148,149,150,151,152,154,155,156,157,158,159,160,161,162,163,164,165,166,167,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,196,197,198,199,200,201,202,203,204,205,205,206,207,208,209,210,211,211,212,213,214,215,216,216,217,218,219,219,220,221,222,222,223,224,225,225,226,227,227,228,229,229,230,231,231,232,233,233,234,235,235,236,236,237,237,238,239,239,240,240,241,241,242,242,243,243,243,244,244,245,245,246,246,246,247,247,248,248,248,249,249,249,250,250,250,250,251,251,251,251,252,252,252,252,253,253,253,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,254,254,255,254,254,254,254,254,254,254,254,254,254,254,254,254,254,253,253,253,253,253,253,252,252,252,252,251,251,251,251,250,250,250,250,249,249,249,248,248,248,247,247,246,246,246,245,245,244,244,243,243,243,242,242,241,241,240,240,239,239,238,237,237,236,236,235,235,234,233,233,232,231,231,230,229,229,228,227,227,226,225,225,224,223,222,222,221,220,219,219,218,217,216,216,215,214,213,212,211,211,210,209,208,207,206,205,205,204,203,202,201,200,199,198,197,196,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,167,166,165,164,163,162,161,160,159,158,157,156,155,154,152,151,150,149,148,147,146,145,144,143,141,140,139,138,137,136,135,134,133,131,130,129,128,127,126,125,124,123,121,120,119,118,117,116,115,114,113,111,110,109,108,107,106,105,104,103,102,100,99,98,97,96,95,94,93,92,91,90,89,88,87,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,58,57,56,55,54,53,52,51,50,49,49,48,47,46,45,44,43,43,42,41,40,39,38,38,37,36,35,35,34,33,32,32,31,30,29,29,28,27,27,26,25,25,24,23,23,22,21,21,20,19,19,18,18,17,17,16,15,15,14,14,13,13,12,12,11,11,11,10,10,9,9,8,8,8,7,7,6,6,6,5,5,5,4,4,4,4,3,3,3,3,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,6,6,6,7,7,8,8,8,9,9,10,10,11,11,11,12,12,13,13,14,14,15,15,16,17,17,18,18,19,19,20,21,21,22,23,23,24,25,25,26,27,27,28,29,29,30,31,32,32,33,34,35,35,36,37,38,38,39,40,41,42,43,43,44,45,46,47,48,49,49,50,51,52,53,54,55,56,57,58,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,87,88,89,90,91,92,93,94,95,96,97,98,99,100,102,103,104,105,106,107,108,109,110,111,113,114,115,116,117,118,119,120,121,123,124,125,126,127};
 
/// SVPWM (Space Vector Wave)
//const int pwmSin[] = {128, 147, 166, 185, 203, 221, 238, 243, 248, 251, 253, 255, 255, 255, 253, 251, 248, 243, 238, 243, 248, 251, 253, 255, 255, 255, 253, 251, 248, 243, 238, 221, 203, 185, 166, 147, 128, 109, 90, 71, 53, 35, 18, 13, 8, 5, 3, 1, 1, 1, 3, 5, 8, 13, 18, 13, 8, 5, 3, 1, 1, 1, 3, 5, 8, 13, 18, 35, 53, 71, 90, 109};
//const int pwmSin[] = {128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170, 174, 178, 181, 185, 189, 192, 196, 200, 203, 207, 211, 214, 218, 221, 225, 228, 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221, 218, 214, 211, 207, 203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162, 159, 155, 151, 147, 143, 140, 136, 132, 128, 124, 120, 116, 113, 109, 105, 101, 97, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 49, 45, 42, 38, 35, 31, 28, 24, 21, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 24, 28, 31, 35, 38, 42, 45, 49, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 97, 101, 105, 109, 113, 116, 120, 124};
 
void setup() {
  Wire.begin();

  // Change Frequency of pins to above audible range (32kHz). Only for Arduino UNO
  setPwmFrequency(motorPin1);
  setPwmFrequency(motorPin2);
  setPwmFrequency(motorPin3);

  // Pin initializations
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);

  // Shifts each output variable to match whichever sine lookup table is being used. This ensures that the outputs are always 120 degrees apart.
  sineArraySize = sizeof(pwmSin)/sizeof(int); // Find lookup table size
  int phaseShift = sineArraySize/3;           // Find phase shift and initial A, B C phase values
  currentStepA = 0;
  currentStepB = currentStepA + phaseShift;
  currentStepC = currentStepB + phaseShift;
  
  sineArraySize--; // Convert from array Size to last PWM array number

  Serial.begin(115200);
}

void loop() {
  // Read position information from encoder
  readEncoder();
  // Read setpoint from Serial Monitor
  readSetpoint();

  // Take the input value from the encoder and translate it into 360 degrees 
  pos=receivedValue;
  pos=map(receivedValue,0,1024,0,360);
  pos=constrain(pos,0,360);

  error=pos-setpoint;   // Calculate the error

  // Only change the position in the lookup table after a certain time. Changing "interval" variable changes the speed which the motor turns
  currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;

    //Move the motor in certain direction based on if the error is above a certain threshold
    if(error < -2){
      direct=false;
      moveMotor();
    } else if(error > 2){
      direct=true;
      moveMotor();
    }
  }
  
}

void moveMotor(){
  // moves through the lookup table based on previously declared direction
  if(direct){
    increment = -1;
  } else {
    increment = 1;
  }

  // Change Duty Cycle value in the lookup table
  currentStepA = currentStepA + increment;
  currentStepB = currentStepB + increment;
  currentStepC = currentStepC + increment;
  
  //Check for lookup table overflow and return to opposite end if necessary
  if(currentStepA > sineArraySize)  currentStepA = 0;
  if(currentStepA < 0)  currentStepA = sineArraySize;
 
  if(currentStepB > sineArraySize)  currentStepB = 0;
  if(currentStepB < 0)  currentStepB = sineArraySize;
 
  if(currentStepC > sineArraySize)  currentStepC = 0;
  if(currentStepC < 0) currentStepC = sineArraySize;
  
  // PWM output on motor pins with duty cycle determined by lookup table 
  analogWrite(motorPin1,pwmSin[currentStepA]);
  analogWrite(motorPin2,pwmSin[currentStepB]);
  analogWrite(motorPin3,pwmSin[currentStepC]);

  // Debugging
  /*
  Serial.print(pwmSin[currentStepA]);
  Serial.print(",");
  Serial.print(pwmSin[currentStepB]);
  Serial.print(",");
  Serial.print(pwmSin[currentStepC]);
  Serial.print(",");
  //*/
  /*
  Serial.print(pos);
  Serial.print(",");
  Serial.print(setpoint);
  Serial.print(",");
  Serial.println(error);
  //*/
}

void readSetpoint(){
  while (Serial.available() > 0) {
  int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      Serial.print("Value:");
      Serial.println(inString.toInt());
      setpoint = inString.toInt();
      Serial.print("String: ");
      Serial.println(inString);
      // clear the string for new input:
      inString = "";
    }
  }
}

void readEncoder(){
  //Read two bytes from the slave device
  Wire.beginTransmission(SlaveDeviceId);
  int available = Wire.requestFrom(SlaveDeviceId, (uint8_t)2);

  if(available == 2){
    receivedValue = Wire.read() << 8 | Wire.read();
    //Serial.print(receivedValue);
  } else {
    Serial.print("Unexpected number of bytes received: ");
    Serial.println(available);
  }

  int result = Wire.endTransmission();
  if(result){
    Serial.print("Unexpected endTransmission result: ");
    Serial.println(result);
  }
  
}

void setPwmFrequency(int pin) {
  //Write to Arduino UNO registers to set the output frequency of the PWM pins used
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | 0x01;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | 0x01;
    }
  }
  else if(pin == 3 || pin == 11) {
    TCCR2B = TCCR2B & 0b11111000 | 0x01;
  }
}

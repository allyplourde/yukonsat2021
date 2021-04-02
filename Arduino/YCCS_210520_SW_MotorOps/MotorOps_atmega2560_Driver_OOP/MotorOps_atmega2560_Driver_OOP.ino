/*
 * Position Control of BLDC for CubeSat
 * Pins demarcations for Arduino Mega
 */
 
#include "MotorDriver.h"

#define PINA 21
#define PINB 20
#define PINI 18
#define INTERRUPTA 2
#define INTERRUPTB 3
#define INTERRUPTI 5
#define MOTOR_PIN1 8
#define MOTOR_PIN2 7
#define MOTOR_PIN3 6
#define ENABLE12_PIN 10
#define ENABLE34_PIN 9

// Encoder count variables
volatile int lastEncoded = 0; // Here updated value of encoder store.
volatile int encoderValue = 0; // Raw encoder value

// Motor movement variables
int pos = 0;          // position of motor
boolean direct = 0;   // Direction of motor
int setpoint = 0;
int error = 0;
int interval = 50;

MotorDriver motor1(interval, MOTOR_PIN1, MOTOR_PIN2, MOTOR_PIN3, ENABLE12_PIN, ENABLE34_PIN);

void setup() {
  // Change Frequency of pins to above audible range (32kHz)
  TCCR4B = TCCR4B & B11111000 | B00000001;
  TCCR5B = TCCR5B & B11111000 | B00000001; 

  // Declare interrupt functions which trigger on voltage change one with the A phase and one with the Index phase
  attachInterrupt (INTERRUPTA, updateEncoder, CHANGE);   // interrupt 0 is pin 2, interrupt 1 is pin 3
  attachInterrupt (INTERRUPTB, updateEncoder, CHANGE);
  attachInterrupt (INTERRUPTI, homeEncoder, CHANGE); 
  
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  // Read setpoint from Serial Monitor
  readSetpoint();

  // Maps encoder values to create all positive values creating an absolute position from 0-1024
  if(encoderValue <= 0){
    pos = map(encoderValue,-1,-2048,2048,1);
  } else {
    pos = encoderValue;
  }

  // Take the input value from the encoder and translate it into 360 degrees 
  pos=map(pos,0,2048,0,360);
  pos=constrain(pos,0,360);
  error=setpoint-pos;   // Calculate the error

  //Debugging
  Serial.print(setpoint);
  Serial.print(" , ");
  Serial.print(pos);
  Serial.print(" , ");
    
  motor1.updateError(error);
  motor1.moveMotor();
}


void updateEncoder(){
  int MSB = digitalRead(PINA); //MSB = most significant bit
  int LSB = digitalRead(PINB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}

void homeEncoder(){
  // The home position is considered as 0
  encoderValue = 0;
}

void readSetpoint(){
  if(Serial.available() > 0) {
    setpoint = Serial.parseInt();
  }
}

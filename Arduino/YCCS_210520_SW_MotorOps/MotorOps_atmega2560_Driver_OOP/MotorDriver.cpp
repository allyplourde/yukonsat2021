#include "MotorDriver.h"

MotorDriver::MotorDriver(int interval, byte MOTOR_PIN1, byte MOTOR_PIN2, byte MOTOR_PIN3, byte ENABLE12_PIN, byte ENABLE34_PIN) {
  this->interval = interval;
  this->MOTOR_PIN1 = MOTOR_PIN1;
  this->MOTOR_PIN2 = MOTOR_PIN2;
  this->MOTOR_PIN3 = MOTOR_PIN3;
  this->ENABLE12_PIN = ENABLE12_PIN;
  this->ENABLE34_PIN = ENABLE34_PIN;

  init();
}

void MotorDriver::init(){
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_PIN3, OUTPUT);
  pinMode(ENABLE12_PIN, OUTPUT);
  pinMode(ENABLE34_PIN, OUTPUT); 
  
  digitalWrite(ENABLE12_PIN, HIGH);
  digitalWrite(ENABLE34_PIN, HIGH);
  
  // Shifts each output variable to match whichever sine lookup table is being used. This ensures that the outputs are always 120 degrees apart.
  sineArraySize = sizeof(pwmSin)/sizeof(int); // Find lookup table size
  int phaseShift = sineArraySize/3;           // Find phase shift and initial A, B C phase values
  currentStepA = 0;
  currentStepB = currentStepA + phaseShift;
  currentStepC = currentStepB + phaseShift;
  
  sineArraySize--; // Convert from array Size to last PWM array number
  
}

void MotorDriver::updateError(int error){
  this->error = error;
  //Serial.println(error);
}

void MotorDriver::moveMotor(){
  currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    
    if(error < -2){
      increment=1;
    } else if(error > 2){
      increment=-1;
    }

    //Serial.print(increment);
    //Move the motor in certain direction based on if the error is above a certain threshold
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
    analogWrite(MOTOR_PIN1,pwmSin[currentStepA]);
    analogWrite(MOTOR_PIN2,pwmSin[currentStepB]);
    analogWrite(MOTOR_PIN3,pwmSin[currentStepC]);

    //debugging
    Serial.print(currentStepA);
    Serial.print(","),
    Serial.print(currentStepB);
    Serial.print(","),
    Serial.println(currentStepC);
        
  }  
}

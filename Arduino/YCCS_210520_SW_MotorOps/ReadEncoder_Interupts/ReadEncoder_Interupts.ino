/*
 * Simple Encoder Reader for BLDC Maxon rotary encoder using interupts 
 * Only looks at A and B phases 
 * Pins set for arduino UNO
 */

const int encoderPinOUT = A3;
const int encoderPin1 = 2; //Encoder Output 'A' must connected with intreput pin of arduino.
const int encoderPin2 = 3; //Encoder Otput 'B' must connected with intreput pin of arduino.
const int indexPin = 4;
volatile int lastEncoded = 0; // Here updated value of encoder store.
volatile long encoderValue = 0; // Raw encoder value
int lastMSB = 0;
int lastLSB = 0;
int encoderOUT;

void setup() {
  Serial.begin(9600); //initialize serial comunication

  pinMode(encoderPin1, INPUT_PULLUP); 
  pinMode(encoderPin2, INPUT_PULLUP);
  pinMode(indexPin, INPUT);
  pinMode(encoderPinOUT, OUTPUT);
  
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
}

void loop() {
  encoderOUT = map(encoderValue,0,2048,0,1023);
  analogWrite(encoderPinOUT,encoderValue);
  Serial.print("encoderValue - ");
  Serial.print(encoderValue);
  Serial.print(", ");
  Serial.println(encoderOUT);
}

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  if(digitalRead(indexPin)==1) encoderValue = 0;
  
  lastEncoded = encoded; //store this value for next time
}

const int INUL = 6;
const int INUH = 10;
const int INVL = 7;
const int INVH = 11;
const int INWL = 5;
const int INWH = 9;

const int STBY = 4;

//const int pwmSin[] = {127, 138, 149, 160, 170, 181, 191, 200, 209, 217, 224, 231, 237, 242, 246, 250, 252, 254, 254, 254, 252, 250, 246, 242, 237, 231, 224, 217, 209, 200, 191, 181, 170, 160, 149, 138, 127, 116, 105, 94, 84, 73, 64, 54, 45, 37, 30, 23, 17, 12, 8, 4, 2, 0, 0, 0, 2, 4, 8, 12, 17, 23, 30, 37, 45, 54, 64, 73, 84, 94, 105, 116 };
//const int pwmSin[] = {128, 147, 166, 185, 203, 221, 238, 243, 248, 251, 253, 255, 255, 255, 253, 251, 248, 243, 238, 243, 248, 251, 253, 255, 255, 255, 253, 251, 248, 243, 238, 221, 203, 185, 166, 147, 128, 109, 90, 71, 53, 35, 18, 13, 8, 5, 3, 1, 1, 1, 3, 5, 8, 13, 18, 13, 8, 5, 3, 1, 1, 1, 3, 5, 8, 13, 18, 35, 53, 71, 90, 109};
const int pwmSin[] = {128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170, 174, 178, 181, 185, 189, 192, 196, 200, 203, 207, 211, 214, 218, 221, 225, 228, 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221, 218, 214, 211, 207, 203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162, 159, 155, 151, 147, 143, 140, 136, 132, 128, 124, 120, 116, 113, 109, 105, 101, 97, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 49, 45, 42, 38, 35, 31, 28, 24, 21, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 24, 28, 31, 35, 38, 42, 45, 49, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 97, 101, 105, 109, 113, 116, 120, 124};
//const int pwmSin[] = {255, 255, 255, 255, 255, 255, 255,

int currentStepU;
int currentStepV;
int currentStepW;
int sineArraySize;
int increment = 0;
boolean direct = 1; // direction true=forward, false=backward

void setup() {
  setPwmFrequency(INUH); // Increase PWM frequency to 32 kHz  (make unaudible)
  setPwmFrequency(INVH);
  setPwmFrequency(INWH);
  
  pinMode(INUL, OUTPUT);
  pinMode(INUH, OUTPUT);
  pinMode(INVL, OUTPUT);
  pinMode(INVH, OUTPUT);
  pinMode(INWL, OUTPUT);
  pinMode(INWH, OUTPUT);

  Serial.begin(9600);

  sineArraySize = sizeof(pwmSin)/sizeof(int); // Find lookup table size
  int phaseShift = sineArraySize / 3;         // Find phase shift and initial A, B C phase values
  currentStepU = 0;
  currentStepV = currentStepU + phaseShift;
  currentStepW = currentStepV + phaseShift;
 
  sineArraySize--; // Convert from array Size to last PWM array number
}

void loop() {
  /*
  digitalWrite(INUL,HIGH);
  digitalWrite(INUH,HIGH);
  */

  //analogWrite(INUH, 55);
  digitalWrite(INUL,LOW);
  digitalWrite(INVL,LOW);
  digitalWrite(INWL,LOW);

  analogWrite(INUH, pwmSin[currentStepU]);
  analogWrite(INVH, pwmSin[currentStepV]);
  analogWrite(INWH, pwmSin[currentStepW]);  
  
  if (direct==true) increment = 1;
  else increment = -1;     
   
  currentStepU = currentStepU + increment;
  currentStepV = currentStepV + increment;
  currentStepW = currentStepW + increment;
  
  //Check for lookup table overflow and return to opposite end if necessary
  if(currentStepU > sineArraySize)  currentStepU = 0;
  if(currentStepU < 0)  currentStepU = sineArraySize;
  
  if(currentStepV > sineArraySize)  currentStepV = 0;
  if(currentStepV < 0)  currentStepV = sineArraySize;
 
  if(currentStepW > sineArraySize)  currentStepW = 0;
  if(currentStepW < 0) currentStepW = sineArraySize; 
  /*
  Serial.print(analogRead(W));
  Serial.print("  ");
  Serial.print(analogRead(V));
  Serial.print("  ");
  Serial.println(analogRead(U));
  */
  /// Control speed by this delay
  delay(10);
}

void setPwmFrequency(int pin) {
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

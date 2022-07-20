const byte ButtonSize = 6;
const byte INPins[] = { 2, 3, 4, 5, 6, 7 };
const byte LEDPins[] = { 8, 9, 10, 11, 12, A1 };

void setup() {
  for(int i = 0; i < ButtonSize; i++) {
    pinMode(INPins[i], INPUT);
  }
  for(int i = 0; i < ButtonSize; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }
  
  Serial.begin(250000);
  
  while (!Serial) {
    delay(10);
  }
  Serial.println("0");
}

 
void loop() {
  for(int i = 0; i < ButtonSize; i++) {
    if (digitalRead(INPins[i]) == LOW){
      digitalWrite(LEDPins[i], HIGH);
      Serial.print(1);
    }
    else {
      digitalWrite(LEDPins[i], LOW);
      Serial.print(0);
    }
  }
  Serial.print(' ');
  delay(2);
}

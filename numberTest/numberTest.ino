int clicks=256;
boolean bitToPush=true;
boolean pushedOld= false;
#define pushButton 3
#define latchPin 8
#define clockPin 12
#define dataPin 11

void setup() {
  Serial.begin(9600);
  pinMode(pushButton,INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
 
}

void loop() {
pushBit();
delay(10);
}

void pushBit(){
  int byteHigh=(clicks & 65280)/256;
  int byteLow=clicks & 255;
digitalWrite(latchPin,LOW);
 shiftOut(dataPin, clockPin, MSBFIRST, byteHigh);
 shiftOut(dataPin, clockPin, MSBFIRST, byteLow);
digitalWrite(latchPin,HIGH);
delay(10);
}


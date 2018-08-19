#include <EEPROM.h>

int clicks;
boolean bitToPush=true;
boolean pushedOld= false;
int powerOn=1;
#define pushButton 3
#define latchPin 8
#define clockPin 12
#define dataPin 11

//value for the read in battery runoff with voltage less than 5.5V
//when using a 1:1 tension divider
int readPowerOff=620;

void setup() {
  //uncomment for debug
  //Serial.begin(9600);
  
  //read clicks from EEPROM, bitHigh must be shifted 8
  clicks=(EEPROM.read(0)<<8)+ EEPROM.read(1);
  
  //define pin modes
  pinMode(pushButton,INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
 
}

void loop() {
  //uncomment for debug
  //Serial.println(analogRead(A0));
  
  //protection for low battery or power out routine
  if (analogRead(A0) < readPowerOff && powerOn){
    powerOn=0;
    saveClicks();
    turnOffLeds();
    }
  
  //detection of press and click count
  boolean pushed=digitalRead(pushButton);
  if (pushed!=pushedOld && powerOn) {
    if (pushed){
      clicks++;
      pushClicks();
      Serial.println(clicks,BIN);
    }
    pushedOld=pushed;
   }
  delay(10);
}

//pushing clicks to the registers
void pushClicks(){
  
  //decomposing clicks into to bytes
  int byteHigh=(clicks & 65280)/256;
  int byteLow=clicks & 255;
  
  //writing in register
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, byteHigh);
  shiftOut(dataPin, clockPin, MSBFIRST, byteLow);
  digitalWrite(latchPin,HIGH);
  delay(10);
}

//saving clicks on EEPROM whenever power is low or off
void saveClicks(){
  int byteHigh=(clicks & 65280)/256;
  int byteLow=clicks & 255;
  EEPROM.write(0,byteHigh);
  EEPROM.write(1,byteLow);
}

//turning off leds
void turnOffLeds(){
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin,HIGH);
  delay(10);
}



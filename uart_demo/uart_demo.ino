#include <SoftwareSerial.h>

int LED = 8;
int SEN = A0;
int data = 0;
char cmd;

//SoftwareSerial ser1(2,3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void cmdSwitch(char cmd){
  switch(cmd){
    case 'a': 
      digitalWrite(LED, LOW);
      break;
    case 'b': 
      digitalWrite(LED, HIGH);
      break;
    case 'c':
      data = analogRead(SEN);
      Serial.println(data);
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    cmd = Serial.read();
    cmdSwitch(cmd);
  }
  delay(250);
}

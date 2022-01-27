#include <SoftwareSerial.h>

int rx = 2;
int tx = 3;
int val = 0;
int LED = 8;

SoftwareSerial Ser2 (rx, tx);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Ser2.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = Ser2.read();
  Serial.write(val);

  /*if (val > 512){
    digitalWrite(LED,LOW);
  } else {
    digitalWrite(LED, HIGH);
  }*/
}

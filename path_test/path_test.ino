int val = 0;
int LED = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = Serial.read()

  if (val > 512){
    digitalWrite(LED,LOW);
  } else {
    digitalWrite(LED, HIGH);
  }
}

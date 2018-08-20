#define Pin 5 

void setup() {
  Serial.begin(9600);
}

void loop() {

  //thing.handle();
  int value = analogRead(Pin);
  Serial.println(value);

  delay(1000);
}

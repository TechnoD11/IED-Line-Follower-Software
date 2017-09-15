
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Right: ");
  Serial.println(analogRead(0));
  Serial.print("Left: ");
  Serial.println(analogRead(1));
  Serial.print("Center: ");
  Serial.println(analogRead(2));
  Serial.println();
  delay(750);
}


const int PWM = 6;
const int PWM2 = 5;
const int digPin = 10;
const int digPin2 = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(PWM, OUTPUT);
  pinMode(digPin, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(digPin2, OUTPUT);
}

void loop() {

  digitalWrite(digPin, HIGH);
  digitalWrite(digPin2, HIGH);
  for(int i = 0; i < 255; i++){
    analogWrite(PWM, i);
    analogWrite(PWM2, i);
    delay(1);  
  }
  for(int i = 255; i > 0; i--){
    analogWrite(PWM, i);
    analogWrite(PWM2, i);
    delay(1);  
  }
  digitalWrite(digPin, LOW);
  digitalWrite(digPin2, LOW);
  for(int i = 0; i < 255; i++){
    analogWrite(PWM, i);
    analogWrite(PWM2, i);
    delay(1);  
  }
  for(int i = 255; i > 0; i--){
    analogWrite(PWM, i);
    analogWrite(PWM2, i);
    delay(1);  
  }
  
  
  
  // put your main code here, to run repeatedly:
	
	
}

/*Filename lineFollowerCode
 * Written by Steven Beninati and Derek Wider
 * Written on 9/12/17
 * 
 * This code is designed for the IED line follower project. The robot follows a line using 3 phototransistors
 */

bool is_running = false; 

//Digital Pin Definitions
const int GAIN_LED_0 = 2;
const int GAIN_LED_1 = 3;
const int PWR_LED = 4;
const int MTR_L = 5;
const int MTR_R = 6;
const int IN_1_2 = 7;
const int GAIN_PB = 8;
const int START_PB = 9;
const int IN_3_4 = 10;

//Analog Pin Definitions
const int SENSE_RIGHT = 0;
const int SENSE_LEFT = 1;
const int SENSE_CENTER = 2;

int last_dir = CENTER;

//Analog Threshhold Values
const int RIGHT_THRESH = 200;
const int LEFT_THRESH = 200;
const int CENT_THRESH = 200;

unsigned long last_detect = 0; //time since last detection

int gain = 1;

void setup() {
  // configure the outputs on the Arduino
  pinMode(GAIN_LED_0, OUTPUT);  //Gain mode LED (1)
  pinMode(GAIN_LED_1, OUTPUT);  //Gain mode LED (0)
  pinMode(PWR_LED,OUTPUT);  //PWR/status LED
  pinMode(MTR_L,OUTPUT);  //Mtr 2 (L) PWM
  pinMode(MTR_R,OUTPUT);  //Mtr 1 (R) PWM
  pinMode(IN_1_2, OUTPUT);	//Direction Switching for MTR_L
  pinMode(IN_3_4, OUTPUT);	//Direction Switching for MTR_R
  pinMode(GAIN_PB,INPUT);   //PB1 (Gain)
  pinMode(START_PB,INPUT);   //PB2 (Start) 

}

void loop() {
  //if the start button is pressed
  if(!digitalRead(Start_PB)){ 
    //wait 1ms for debounce clear, then check again
    delay(1);
    if(!digitalRead(Start_PB)){
      if(is_running){
        //enable the running LED
        is_running = true;
        digitalWrite(PWR_LED, HIGH);
      }
      else{            //if running, stop and turn off LED
        is_running = false;
        digitalWrite(PWR_LED, LOW);
        stop_motors();
      }
  }
  if(!digitalRead(Gain_PB)){ //if gain pressed, cycle gain
    //wait 1ms for debounce clear
    delay(1);
    if(!digitalRead(Gain_PB)){
      if(gain == 3){
        gain = 1;
      }
      else gain++;
      if(gain == 1){
        digitalWrite(Gain_LED_1, HIGH);
        digitalWrite(Gain_LED_2, LOW);
      }
      else if(gain == 2){
        digitalWrite(Gain_LED_1, LOW);
        digitalWrite(Gain_LED_2, HIGH);
      }
      else{
        digitalWrite(Gain_LED_1, HIGH);
        digitalWrite(Gain_LED_2, HIGH);
      }
    }
  }
  if(is_running) 
  {
    if(isCentTrig()) //if the center sensor detects the line, drive straight
    {
      digitalWrite(MTR_L_In,HIGH);
      digitalWrite(MTR_R_In,HIGH);
      last_detect = millis();
    }
    else if(isLeftTrig()) //if the left sensor detects the line, turn left
    {
      digitalWrite(MTR_L_In,HIGH);
      digitalWrite(MTR_R_In,LOW);
      last_detect = millis();
    }
    else if(isRightTrig()) //if the right sensor detects the line, turn right
    {
      digitalWrite(MTR_L_In,LOW);
      digitalWrite(MTR_R_In,HIGH);
    }
    else //if no sensors detect line
    {
      if(millis()-last_detect > 2000) //if more than 2 seconds have passed since last detection, stop
      {
        stop_motors();
        while(digitalRead(Start_PB))
        {
          digitalWrite(PWR_LED,1);
          delay(250);
          digitalWrite(PWR_LED,0);
          delay(250);
        }
      }
      last_detect = 0;
    }
    //scale PWM as necessary - based on gain variable
  }
}

//funtion to turn off the motors
void stop_motors() 
{
   digitalWrite(MTR_L,LOW);
   digitalWrite(MTR_R,LOW);
}
//return true if the right sensor sees the line
bool isRightTrig(){
	if(SENSE_RIGHT > RIGHT_THRESH)
		return true;
	else return false;
}
//return true if the left sensor sees the line
bool isLeftTrig(){
	if(SENSE_LEFT > LEFT_THRESH)
		return true;
	else return false;
}
//return true if the center sensor sees the line
bool isCentTrig(){
	if(SENSE_CENTER > CENT_THRESH)
		return true;
	else return false;
}


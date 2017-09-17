/*Filename lineFollowerCode
 * Written by Steven Beninati and Derek Wider
 * Written on 9/12/17
 * 
 * This code is designed for the IED line follower project. The robot follows a line using 3 phototransistors
 */

bool is_running = false; 
bool isTimeUp = false;

//Digital Pin Definitions
const int MODE_LED_0 = 2;
const int MODE_LED_1 = 3;
const int PWR_LED = 4;
const int MTR_L = 5;
const int MTR_R = 6;
const int IN_1_2 = 7;
const int MODE_PB = 8;
const int START_PB = 9;
const int IN_3_4 = 10;

//Analog Pin Definitions
const int SENSE_RIGHT = 0;
const int SENSE_LEFT = 1;
const int SENSE_CENTER = 2;

//Analog Threshhold Values
const int RIGHT_THRESH = 200;
const int LEFT_THRESH = 200;
const int CENT_THRESH = 150;

unsigned long last_detect = 0; //time since last detection

int mode = 1;

void setup() {
	Serial.begin(9600);
	// configure the outputs on the Arduino
	pinMode(MODE_LED_0, OUTPUT);  //mode LED (1)
	pinMode(MODE_LED_1, OUTPUT);  //mode LED (0)
	pinMode(PWR_LED,OUTPUT);  //PWR/status LED
	pinMode(MTR_L,OUTPUT);  //Mtr 2 (L) PWM
	pinMode(MTR_R,OUTPUT);  //Mtr 1 (R) PWM
	pinMode(IN_1_2, OUTPUT);	//Direction Switching for MTR_L
	pinMode(IN_3_4, OUTPUT);	//Direction Switching for MTR_R
	pinMode(MODE_PB,INPUT);   //PB1 (mode)
	pinMode(START_PB,INPUT);   //PB2 (Start) 
	
	//Display default Mode (1):
	digitalWrite(MODE_LED_0, HIGH);
	digitalWrite(MODE_LED_1, LOW);

}

void loop() {
	//if the start button is pressed
	if(!digitalRead(START_PB)){ 
		//wait 1ms for debounce clear, then check amode
		delay(1);
		if(!digitalRead(START_PB)){
			if(!is_running){
				is_running = true;
				digitalWrite(PWR_LED, HIGH);
			}
			//if running, stop and turn off LED
			else{            
			is_running = false;
			digitalWrite(PWR_LED, LOW);
			stop_motors();
			}
		}
		delay(500);
	}
	if(!digitalRead(MODE_PB) && !is_running){ //if mode pressed, cycle mode
		//wait 1ms for debounce clear
		delay(1);
		if(!digitalRead(MODE_PB)){
			if(mode == 3){
				mode = 1;
			}
			else mode++;
			switch (mode){
				case 1: 
					digitalWrite(MODE_LED_0, HIGH);
					digitalWrite(MODE_LED_1, LOW);
					break;
				case 2: 
					digitalWrite(MODE_LED_0, LOW);
					digitalWrite(MODE_LED_1, HIGH);
					break;
				case 3:
					digitalWrite(MODE_LED_0, HIGH);
					digitalWrite(MODE_LED_1, HIGH);
					break;
			}
		}
		delay(500);
	}
	if(is_running){
		switch (mode){
			case 1:
				if(isLeftTrig() && isRightTrig()){
				
				}
				else if(isCentTrig()){
					goForwardFast();
				}
				else if(isLeftTrig()){
					//motor status: Both Forward
					digitalWrite(IN_1_2, HIGH);
					digitalWrite(IN_3_4, HIGH);
					//motor speed: Right higher than left
					analogWrite(MTR_L, 75);
					analogWrite(MTR_R, 110);
				}
				else if(isRightTrig()){
					//motor status: Both Forward
					digitalWrite(IN_1_2, HIGH);
					digitalWrite(IN_3_4, HIGH);
					//motor speed: Left higher than Right
					analogWrite(MTR_L, 110);
					analogWrite(MTR_R, 75);
				}
				//If no sensor sees the line
				else{
					noLineProcess();
				}
				break;
			case 2:
				if(isLeftTrig() && isRightTrig()){
				
				}
				else if(isCentTrig()){
					goForwardFast();
				}
				else if(isLeftTrig()){
					//motor status: Right Forward, Left Backward (off)
					digitalWrite(IN_1_2, LOW);
					digitalWrite(IN_3_4, HIGH);
					//motor speed: Left off, Right forward
					analogWrite(MTR_L, 40);
					analogWrite(MTR_R, 170);
				}
				else if(isRightTrig()){
					//motor status: Left Forward, Right Backward (off)
					digitalWrite(IN_1_2, HIGH);
					digitalWrite(IN_3_4, LOW);
					//motor speed: Left higher than Right
					analogWrite(MTR_L, 170);
					analogWrite(MTR_R, 40);
				}
				//If no sensor sees the line
				else{
					noLineProcess();
				}
				break;
			case 3:
				if(isLeftTrig() && isRightTrig() && isCentTrig()){
					offTrack();
				}
				else if(isLeftTrig() && isRightTrig()){
					goForwardFast();
				}
				else if(isCentTrig()){
					goForwardFast();
				}
				else if(isLeftTrig()){
					//motor status: Both Reverse
					digitalWrite(IN_1_2, LOW);
					digitalWrite(IN_3_4, HIGH);
					//motor speed: Left Reverse, Right forward
					analogWrite(MTR_L, 220);
					analogWrite(MTR_R, 150);
				}
				else if(isRightTrig()){
					//motor status: Both Reverse
					digitalWrite(IN_1_2, HIGH);
					digitalWrite(IN_3_4, LOW);
					//motor speed: Right Reverse, Left forward
					analogWrite(MTR_L, 150);
					analogWrite(MTR_R, 220);
				}
				//If no sensor sees the line
				else{
					noLineProcess();
				}
				break;
		}
	}
}
void goForwardFast(){
	//set both motors to forward
	digitalWrite(IN_1_2, HIGH);
	digitalWrite(IN_3_4, HIGH);
	//set motors to same speed
	analogWrite(MTR_L, 140);
	analogWrite(MTR_R, 140);
}
//call this is no of the line sensors see the line in any mode
void noLineProcess(){
	last_detect = millis();
	digitalWrite(IN_1_2, HIGH);
	digitalWrite(IN_3_4, HIGH);
	while(!isRightTrig() && !isLeftTrig() && !isTimeUp){	
		Serial.println(millis() - last_detect);
		if((millis() - last_detect) > 2000){
			isTimeUp = true;
		}
		analogWrite(MTR_L, 140);
		analogWrite(MTR_R, 140);
	}
	isTimeUp = false;
	if(millis() - last_detect >= 2000){
		is_running = false;
		last_detect = 0;
		stop_motors();
		while(digitalRead(START_PB)){
          digitalWrite(PWR_LED, HIGH);
          delay(250);
          digitalWrite(PWR_LED, LOW);
          delay(250);
        }
	}
}

void offTrack(){
	last_detect = millis();
	digitalWrite(IN_1_2, HIGH);
	digitalWrite(IN_3_4, HIGH);
	while(isRightTrig() && isLeftTrig() && !isTimeUp){	
		Serial.println(millis() - last_detect);
		if((millis() - last_detect) > 2000){
			isTimeUp = true;
		}
		analogWrite(MTR_L, 140);
		analogWrite(MTR_R, 140);
	}
	isTimeUp = false;
	if(millis() - last_detect >= 2000){
		is_running = false;
		last_detect = 0;
		stop_motors();
		while(digitalRead(START_PB)){
          digitalWrite(PWR_LED, HIGH);
          delay(250);
          digitalWrite(PWR_LED, LOW);
          delay(250);
        }
	}
}
//funtion to turn off the motors
void stop_motors(){
   digitalWrite(MTR_L,LOW);
   digitalWrite(MTR_R,LOW);
}
//return true if the right sensor sees the line
bool isRightTrig(){
	Serial.println("in right trig");
	if(analogRead(SENSE_RIGHT) > RIGHT_THRESH){
		return true;
	}
	else return false;
}
//return true if the left sensor sees the line
bool isLeftTrig(){
	if(analogRead(SENSE_LEFT) > LEFT_THRESH){
		return true;
	}
	else return false;
}
//return true if the center sensor sees the line
bool isCentTrig(){
	if(analogRead(SENSE_CENTER) > CENT_THRESH){
		return true;
	}
	else return false;
}


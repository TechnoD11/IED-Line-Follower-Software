/*Filename lineFollowerCode
 * Written by Steven Beninati and Derek Wider
 * Written on 9/12/17
 * 
 * This code is designed for the IED line follower project. The robot follows a line using 3 phototransistors
 */
const int LEFT = 0;
const int RIGHT = 1;
const int CENTER = 2;

bool is_running = false; //is the robot running 

//pin outputs
int gain_state = 1;
const int Gain_LED_1 = 2;
const int Gain_LED_0 = 3;
const int PWR_LED = 4;
const int MTR_L = 5;
const int MTR_R = 6;
const int IN_1_2 = 7;
const int Gain_PB = 8;
const int Start_PB = 9;
const int IN_3_4 = 10;
const int Sense_Left = 11;
const int Sense_Right = 12;
const int Sense_Center = 13;

int last_dir = CENTER;

unsigned long last_detect = 0; //time since last detection

void setup() {
  // configure the outputs on the Arduino
  pinMode(Gain_LED_1, OUTPUT);  //Gain mode LED (1)
  pinMode(Gain_LED_2, OUTPUT);  //Gain mode LED (0)
  pinMode(PWR_LED,OUTPUT);  //PWR/status LED
  pinMode(MTR_L,OUTPUT);  //Mtr 2 (L) PWM
  pinMode(MTR_R,OUTPUT);  //Mtr 1 (R) PWM
  pinMode(MTR_R_In,OUTPUT);  //IN 1/2 (R)
  pinMode(Gain_PB,INPUT);   //PB1 (Gain)
  pinMode(Start_PB,INPUT);   //PB2 (Start)
  pinMode(MTR_L_In,OUTPUT);  //IN 3/4 (L) 
  pinMode(Sense_Left,INPUT);   //Left sensor
  pinMode(Sense_Right,INPUT);   //Right sensor
  pinMode(Sense_Center,INPUT);  //Center sensor

}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(Start_PB)) //if the start button is pressed
  {
      if(!is_running) //if not running, start and turn on LED
      {
         is_running = true;
         digitalWrite(PWR_LED,LOW);
      }

      else            //if running, stop and turn off LED
      {
          is_running = false;
          digitalWrite(PWR_LED,HIGH);
          stop_motors();
      }
  }
  if(!digitalRead(Gain_PB)) //if gain pressed, cycle gain
  {
     if (gain == 3)
     {
         gain = 1;
     }
     else gain++;
  }
  if(is_running) //if the robot is running
  {
    if(digitalRead(Sense_Center)) //if the center sensor detects the line, drive straight
    {
      digitalWrite(MTR_L_In,HIGH);
      digitalWrite(MTR_R_In,HIGH);
      last_detect = millis();
    }
    else if(digitalRead(Sense_Left)) //if the left sensor detects the line, turn left
    {
      digitalWrite(MTR_L_In,HIGH);
      digitalWrite(MTR_R_In,LOW);
      last_detect = millis();
    }
    else if(digitalRead(Sense_Right)) //if the right sensor detects the line, turn right
    {
      digitalWrite(MTR_L_In,LOW);
      digitalWrite(MTR_R_In,HIGH);
    }
    else //if no sensors detect line
    {
      if(millis()-last_detect > 2000) //if more than 2 seconds have passed since last detection, stop
      {
        stop_motors();
        while(!digitalWrite(Start_PB))
        {
          digitalWrite(PWR_LED,1);
          delay(250);
          digitalWrite(PWR_LED,0);
          delay(250);
        }
      }
      last-detect = 0;
    }
    //scale PWM as necessary - based on gain variable
  }
}

void stop_motors() //funtion to turn off the motors
{
   digitalWrite(MTR_L,LOW);
   digitalWrite(MTR_R,LOW);
}


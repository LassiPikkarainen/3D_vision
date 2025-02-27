// Include Particle Device OS APIs
#include "Particle.h"
#include <cmath>


int deadtime = 4000; // time between closing one lens and opening another when changing frames (microseconds)

int left_eye_bit = 0b00000000000000000100000000000000; //D6 -> GPIOA 13
int right_eye_bit =  0b00000000000000000000000000001000; // D4 -> GPIOB 3

//operating mode
//set one of these to true
bool independent_mode = false; //independent, switches left and right based on a timer
bool software_mode = false;  //switches sides based on incoming bytes through serial port
bool corrected_mode = true; //attempts to keep the period of the glasses as close to a predefined value as possible

//init stuff
int incomingByte = 0;
bool showing_right = true;
bool showing_left = false;

//used by correcting mode to define the delay times
int additional_delay_us = 1; 
int additional_delay_amount = 0;
int target_micro_s = 8333; //correcting mode target time 8333 = 120Hz, 10000 = 100Hz, 10005 = 99,95Hz
int measure_interval = 1000;

SYSTEM_THREAD(ENABLED);

// The setup() method is called once when the device boots.
void setup()
{
  pinMode(D6, OUTPUT);
  pinMode(D4, OUTPUT);;

  WiFi.off();
  Serial.begin(9600);

  waitFor(Serial.isConnected, 30000);
  Serial.printlnf("connected");
  
}


//functions for sending commands to glasses
void close_left()
{

  //LEFT EYE
     GPIOA->ODR = left_eye_bit;

     //turn off
    delayMicroseconds(23);  

    GPIOA->ODR = 0x0000;
    delayMicroseconds(21);

    GPIOA->ODR = left_eye_bit;

    delayMicroseconds(24); 

    GPIOA->ODR = 0x0000;


}

void open_left() 
{
  //turn on
    GPIOA->ODR = left_eye_bit;

    delayMicroseconds(43);  

    GPIOA->ODR = 0x0000;
}

void close_right() 
{
    //Right eye off
    GPIOB->ODR = right_eye_bit;
    delayMicroseconds(23);  

    GPIOB->ODR = 0x0000;
    delayMicroseconds(78);

    GPIOB->ODR = right_eye_bit;

    delayMicroseconds(40); 

    GPIOB->ODR = 0x0000;

}


void open_right()
{
  //Right eye on
    GPIOB->ODR = right_eye_bit;

    delayMicroseconds(23);  

    GPIOB->ODR = 0x0000;
    delayMicroseconds(46);

    GPIOB->ODR = right_eye_bit;

    delayMicroseconds(31); 

    GPIOB->ODR = 0x0000;

}



void loop()
{

 if (independent_mode) 
 {
  while (true)
  {
    
    close_right();//disable right eye

    delayMicroseconds(deadtime); //wait, can be used to set duty cycle for glasses

    open_left(); //enable left eye, start of left eye frame

    delayMicroseconds(target_micro_s - deadtime); //duration of the frame, lens was opened a bit late, and is closed early -> smaller duration 

    close_left(); //disable left eye a bit before frame is over

    delayMicroseconds(deadtime); //wait

    open_right();
    
    delayMicroseconds(target_micro_s - deadtime);
  }
 }

 if (corrected_mode) 
 {
  int i = 1;
  unsigned long start_time_micro_s = micros();
  while (true)
  {
    
    close_right();//disable right eye

    delayMicroseconds(deadtime); //wait, can be used to set duty cycle for glasses

    open_left(); //enable left eye, start of left eye frame

    delayMicroseconds(target_micro_s - deadtime - 168 + additional_delay_us * additional_delay_amount); //duration of the frame, lens was opened a bit late, and is closed early -> smaller duration 

    close_left(); //disable left eye a bit before frame is over

    delayMicroseconds(deadtime); //wait

    open_right();
    
    delayMicroseconds(target_micro_s - deadtime - 168 + additional_delay_us * additional_delay_amount);


    if (Serial.available() > 0) {
      //switch eyes when something is received
      char c = Serial.read();
      if (c == 'S')
      {
        delayMicroseconds(300);
      }
      else if (c == 'M') 
      {
        additional_delay_amount ++;
        Serial.println("add delay");
      }
      else if (c == 'N') 
      {
        additional_delay_amount ++;
        Serial.println("remove delay");
      }
    }

    long t = micros() - start_time_micro_s;
    start_time_micro_s = micros();
    //Serial.println(ave);
    float diff = (t - target_micro_s * 2)/2;

    float ave_diff = (int)diff; //average diff to target on a frame

    if (ave_diff > 1 || ave_diff < -1)
      additional_delay_amount -= ave_diff;

  }

  

 }


 if (software_mode){
  while (true) 
  {
    if (Serial.available() > 0) {
      //switch eyes when something is received
      char c = Serial.read();
      if (c == 'L') 
      {
        close_right();//disable right eye

        delayMicroseconds(deadtime); //wait, can be used to set duty cycle for glasses

        open_left(); //enable left eye, start of left eye frame
        showing_left = false;
        showing_right = true;
      }
      else if (c == 'R')
      {
        close_left(); //disable left eye a bit before frame is over

        delayMicroseconds(deadtime); //wait

        open_right();
        showing_right = false;
        showing_left = true;
      }
      else if (c == 'S')
      {
        delayMicroseconds(100);
      }


    }
  }

 }

	
}



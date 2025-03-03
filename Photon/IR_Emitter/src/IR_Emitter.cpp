// Include Particle Device OS APIs
#include "Particle.h"
#include <cmath>
#include <string>


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

const int READ_BUF_LEN = 64; //should not have longer lines..
char readBuffer[READ_BUF_LEN];
int readBufferOffset = 0;

//setup by driver
bool setupmode = true;

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

//read entire line to a buffer
void readLine()
{
  Serial.printlnf("reading line");
  while(Serial.available()) {
		if (readBufferOffset < READ_BUF_LEN) {
			char c = Serial.read();
			if (c != '\n') {
				// Add character to buffer
				readBuffer[readBufferOffset++] = c;
			}
			else {
				// End of line character found, process line
				readBuffer[readBufferOffset] = 0;
        Serial.printf("received %s", readBuffer);
				readBufferOffset = 0;
			}
		}
		else {
			Serial.println("readBuffer overflow, emptying buffer");
			readBufferOffset = 0;
		}
	}

}

void loop()
{

  //go to here by default, if IR driver software is used
  if (setupmode) 
  {
    if (Serial.available() > 0) {
          char c = Serial.read();
          if (c == 'S')
          {
            int numbers = 0;
            Serial.printlnf("set frametime");
            while(Serial.available() > 0) {
              char d = Serial.read();
              if (readBufferOffset < READ_BUF_LEN) {
                
                if (d != '\n') {
                  // Add character to buffer
                  readBuffer[readBufferOffset++] = d;
                  numbers++;
                }
                else {
                  // End of line character found, process line
                  readBuffer[readBufferOffset] = 0;
                  readBufferOffset = 0;
                }
              }
              else {
                Serial.printlnf("readBuffer overflow, emptying buffer");
                readBufferOffset = 0;
              }
            }
            Serial.read(); //read the endline
            readBufferOffset = 0;
            std::string str_command = std::string(readBuffer, readBuffer+numbers);
            target_micro_s = std::stoi(str_command);
            Serial.printlnf("Period: %i",target_micro_s);

          }
          else if (c == 'D')
          {
            int numbers = 0;
            Serial.printlnf("set deadtime");
            while(Serial.available() > 0) {
              char d = Serial.read();
              if (readBufferOffset < READ_BUF_LEN) {
                
                if (d != '\n') {
                  // Add character to buffer
                  readBuffer[readBufferOffset++] = d;
                  numbers++;
                }
                else {
                  // End of line character found, process line
                  readBuffer[readBufferOffset] = 0;
                  readBufferOffset = 0;
                }
              }
              else {
                Serial.printlnf("readBuffer overflow, emptying buffer");
                readBufferOffset = 0;
              }
            }
            Serial.read(); //read the endline
            readBufferOffset = 0;
            std::string str_command = std::string(readBuffer, readBuffer+numbers);
            deadtime = std::stoi(str_command);
            Serial.printlnf("Deadtime: %i", deadtime);

          }

          //setup done
          else if (c == 'R') 
          {
            Serial.printlnf("run");
            setupmode = false;
          }
        }

  }

  else 
  {

    if (corrected_mode) 
    {
      Serial.printlnf("Run loop");
      Serial.printlnf("Period: %i",target_micro_s);
      Serial.printlnf("Deadtime: %i", deadtime);
      unsigned long start_time_micro_s = micros();
      while (true)
      {
        wait3:

        delayMicroseconds(500);

        wait2:

        delayMicroseconds(400);

        wait1:
        
        delayMicroseconds(100);

        start:

        //unsigned long start_time_micro_s = micros();

        close_right();//disable right eye

        delayMicroseconds(deadtime); //wait, can be used to set duty cycle for glasses

        open_left(); //enable left eye, start of left eye frame

        delayMicroseconds(target_micro_s - deadtime - 168 + additional_delay_us * additional_delay_amount); //duration of the frame, lens was opened a bit late, and is closed early -> smaller duration 

        swap:

        close_left(); //disable left eye a bit before frame is over

        delayMicroseconds(deadtime); //wait

        open_right();
        
        delayMicroseconds(target_micro_s - deadtime - 168 + additional_delay_us * additional_delay_amount);

        long t = micros() - start_time_micro_s;

        start_time_micro_s = micros();
        //Serial.println(ave);
        float diff = (t - target_micro_s * 2)/2;

        float ave_diff = (int)diff; //average diff to target on a frame

        if (ave_diff > 1 || ave_diff < -1)
          additional_delay_amount -= ave_diff;
        
        if (!Serial.available() > 0) {
          goto start;
        }
        //runtime commands
        else {
          t = micros() - start_time_micro_s;
          //switch eyes when something is received
          char c = Serial.read();
          if (c == 'W')
          {
            delayMicroseconds(50);
            start_time_micro_s = micros() + target_micro_s;
            goto swap;
          }

          else if (c == 'I')
          {
            start_time_micro_s += 100;
            goto wait1;
          }
          else if (c == 'J')
          {
            start_time_micro_s += 500;
           goto wait2;
          }
          else if (c == 'K')
          {
            start_time_micro_s += 1000;
            goto wait3;
          }
          else if (c == 'E')  //go to setup
          {
            setupmode = true;
            Serial.printlnf("Setupmode");
            break; 
          }
        }

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
  }
 

	
}



// Include Particle Device OS APIs
#include "Particle.h"



int deadtime = 1000; // time between closing one lens and opening another when changing frames (microseconds)

int left_eye_bit = 0b00000000000000000100000000000000; //D6 -> GPIOA 13
int right_eye_bit =  0b00000000000000000000000000001000; // D4 -> GPIOB 3

//operating mode
//set one to true
bool independent_mode = true; //independent, switches left and right based on a timer
bool software_mode = false;  //switches sides based on incoming bytes through serial port

//init stuff
int incomingByte = 0;
bool showing_right = false;
bool showing_left = true;


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

    delayMicroseconds(8333 - deadtime*2); //duration of the frame, lens was opened a bit late, and is closed early -> smaller duration 

    close_left(); //disable left eye a bit before frame is over

    delayMicroseconds(deadtime); //wait

    open_right();
    
    delayMicroseconds(8333 - deadtime*2);
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
        delayMicroseconds(5000); // testing
      }
      else if (c == 'R')
      {
        close_left(); //disable left eye a bit before frame is over

        delayMicroseconds(deadtime); //wait

        open_right();
        showing_right = false;
        showing_left = true;
        delayMicroseconds(5000); // testing
      }

    }
  }

 }

	
}



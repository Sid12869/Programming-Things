

#include <ZumoMotors.h>
#include <NewPing.h>

#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

ZumoMotors motors;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);


void setup() 
{
  // initialize serial communication:
  Serial.begin(9600); 
}

void loop() 
{
  char keypress;
  bool keyIn = false;
  delay(30);
  if (Serial.available() > 0) 
  {
    keypress = Serial.read();
    keyIn = true; 
  }
  if(keyIn == true)
  {
      movement(keypress);
      keyIn = false;
  }
  
}


void movement(char keypress)
{
 switch (keypress)
    {
      case 'W':forward();
               break;
      case 'D':right();
               break;
      case 'A':left();
               break;
      case 'S':backward();
               break;        
    }
}



void forward(){
   for (int speed = 0; speed <= 250; speed++)
   {
    if (sonar.ping_cm() > 10 and sonar.ping_cm() < 200  ) 
    {
     motors.setLeftSpeed(speed), motors.setRightSpeed(speed);
     delay(30);
    }
    else
    {
      Serial.println("There is a wall");
    }
   }
    motors.setLeftSpeed(0), motors.setRightSpeed(0);
  
}

void right(){
  for (int speed = 0; speed <= 250; speed++)
  {
    motors.setRightSpeed(speed);
    delay(10);
  }

  motors.setRightSpeed(0);
}

void left(){
  for (int speed = 0; speed <= 250; speed++)
  {
    motors.setLeftSpeed(speed);
    delay(10);
  }

  motors.setLeftSpeed(0);
}

void backward(){
  for (int speed = 0; speed >= -250; speed--)
  {
    motors.setLeftSpeed(speed),motors.setRightSpeed(speed);
    delay(10);
  }

  motors.setLeftSpeed(0),motors.setRightSpeed(0);
}


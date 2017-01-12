#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
 
#define LED 13
 
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  400 // microseconds
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     100 // 0 is stopped, 400 is full speed
#define TURN_SPEED        100
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

ZumoMotors motors;
 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
bool forwardBehaviour = true;
bool stopMotor = false; 

 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

bool lightSensorCheck(int c)
{
  bool check;
  sensors.read(sensor_values);
  for(int i=0; i < NUM_SENSORS; i++)
  {
    if(sensor_values[i] > QTR_THRESHOLD)
    {
      motors.setLeftSpeed(0),motors.setRightSpeed(0);
      check = false;    
      if(c == 1)
      {
        Serial.println("You hit a wall");
        c++;
      }
       return check;
    }
    else
    {
      check = true;
      return check;
    }
  }
 return true;
}

bool forwardSensorCheck()
{
  bool check;
  sensors.read(sensor_values);
  if ( sensor_values[1] > QTR_THRESHOLD || sensor_values[2] > QTR_THRESHOLD || sensor_values[3] > QTR_THRESHOLD || sensor_values[4] > QTR_THRESHOLD)
  {
    motors.setLeftSpeed(0),motors.setRightSpeed(0);
    check = false;    
    return check;
  }
  else if (sensor_values[5] > QTR_THRESHOLD)
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    check = true;
    return check;
  }
  else if (sensor_values[0] > QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    check = true;
    return check;
  }
  else
  {
    check = true;
    return check;
  }
}

bool edgeSensorCheck()
{
  bool check;
  sensors.read(sensor_values);
  if(sensor_values[0] > QTR_THRESHOLD)
  {
    motors.setLeftSpeed(100), motors.setRightSpeed(-100);
    delay(1000);
    check = false;
    return check;
  }
  else if(sensor_values[5] > QTR_THRESHOLD)
  {
    motors.setLeftSpeed(-100),motors.setRightSpeed(100);
    delay(1000);
    check = false;
    return check;    
    
  }
  else
   {
     check = true;
     return check;
   }
}
 
void setup()
{ 
 Serial.begin(9600);
}

void loop()
{
  sensors.read(sensor_values);
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
      case 'W':if(forwardBehaviour == false)
                {
                  intermitantforward();
                }
                else
                {
                  constantForward();
                }
               break;
      case 'D':right();
               break;
      case 'A':left();
               break;
      case 'S':backward();
               break;
      case ' ': stopMotor = true;
                break;
      case 'C': 
               if(forwardBehaviour == false)
               {
                 forwardBehaviour = true;
                 Serial.println("the ristrictions have been removed");
               }
               else
               {
                 forwardBehaviour = false;
                 Serial.println("the ristrictions have been added");
               }
               break;         
    }
}

void intermitantforward(){
  int c = 0;
   for (int speed = 0; speed <= 100; speed++)
   {
    c++;
     if (lightSensorCheck(c) == true)
     {
        motors.setLeftSpeed(speed), motors.setRightSpeed(speed);
        delay(30);
     }
   }
    motors.setLeftSpeed(0), motors.setRightSpeed(0);
  
}

void constantForward()
{
  while(forwardSensorCheck() == true && stopMotor == false)
  {
    motors.setLeftSpeed(100), motors.setRightSpeed(100);
  }
  if(stopMotor == false)
  {
   forwardBehaviour = false;
   Serial.println("You have reached the end of a corridor");
   Serial.println("auto matic forward has been cancelled please press C to turn on");
  }
}


void right(){
  for (int speed = 0; speed <= 150; speed++)
  {
    if (edgeSensorCheck() == true)
    {
      motors.setLeftSpeed(speed), motors.setRightSpeed(-speed);
      delay(10);
    }
  }
  
  motors.setLeftSpeed(0),motors.setRightSpeed(0);
}

void left(){
  for (int speed = 0; speed <= 150; speed++)
  {
    if (edgeSensorCheck() == true)
    {
      motors.setLeftSpeed(-speed), motors.setRightSpeed(speed);
      delay(10);
    }
  }
  motors.setLeftSpeed(0),motors.setRightSpeed(0);
}

void backward(){
  for (int speed = 0; speed >= -100; speed--)
  {
    motors.setLeftSpeed(speed), motors.setRightSpeed(speed);
    delay(30);
  }

  motors.setLeftSpeed(0),motors.setRightSpeed(0);
}



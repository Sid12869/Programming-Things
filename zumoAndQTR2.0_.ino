#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>

ZumoReflectanceSensorArray sensor(QTR_NO_EMITTER_PIN);
ZumoMotors motors;

// Define an array for holding sensor values.
#define NUM_SENSORS 5
#define QTR_THRESHOLD  2000
unsigned int sensorValues[NUM_SENSORS];

void setup() {
  Serial.begin(9600);
  delay(500);
}

void loop() {
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
   for (int speed = 0; speed <= 100; speed++)
   {
     if (edgeSensorCheck() == true)
     {
        motors.setLeftSpeed(speed), motors.setRightSpeed(speed);
        delay(30);
        
     }
   }
   motors.setLeftSpeed(0), motors.setRightSpeed(0);
   if (edgeSensorCheck() == false)
   {
     backward();
   }
  
}

void right(){
  for (int speed = 0; speed <= 100; speed++)
  {
    if (edgeSensorCheck() == true)
    {
      motors.setRightSpeed(speed);
      delay(10);
    }
//    else
//    {
//      left();
//    }
  }
  motors.setRightSpeed(0);
}

void left(){
  for (int speed = 0; speed <= 100; speed++)
  {
    if (edgeSensorCheck() == true)
    {
      motors.setLeftSpeed(speed);
      delay(10);
    }
//    else 
//    {
//      right();
//    }
  }

  motors.setLeftSpeed(0);
}

void backward(){
  for (int speed = 0; speed >= -100; speed--)
  {
    motors.setLeftSpeed(speed),motors.setRightSpeed(speed);
    delay(10);
  }

  motors.setLeftSpeed(0),motors.setRightSpeed(0);
}

bool lightSensorCheck()
{
  bool check = true;
  sensor.read(sensorValues);
  for(int i=0; i < NUM_SENSORS; i++)
  {
    if(sensorValues[i]< QTR_THRESHOLD)
    {
      
      check = false;
    }
  } 
 return check;
}

bool edgeSensorCheck()
{
  bool check = true;
  sensor.read(sensorValues);
  if(sensorValues[0] > QTR_THRESHOLD)
  {
    Serial.println("B");
    check = false;
  }
  else if (sensorValues[5] > QTR_THRESHOLD)
  {
    Serial.println("C");
    check = false;
  }
  return check;
}


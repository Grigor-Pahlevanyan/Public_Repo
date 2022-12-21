#include <AccelStepper.h>


//Defining pins
int Stepper1Pulse = 5;  // Pulse or step pin
int Stepper1Direction = 6;  // Direction pin

//defining terms

int Motor1speed = 0;
int speedmin = 0; //pulses per second
int speedmax = 2000;  //pulses per second change as u want

AccelStepper step1(1, Stepper1Pulse, Stepper1Direction);


void setup(){               
  step1.setMaxSpeed (speedmax);  
  step1.setSpeed(0);
  step1.setAcceleration(1000);
  pinMode(Stepper1Pulse, OUTPUT);
  pinMode(Stepper1Direction, OUTPUT);
  digitalWrite(Stepper1Pulse, LOW);
  digitalWrite(Stepper1Direction, LOW);
}

void loop() {
  //(analogRead() reads the value from the analog pin. Ex: If potentiometer is rotated then the analogRead will read its value and u can use it
  //Motor1speed = map(speedmax,0,1023,speedmin,speedmax); //replaced analogRead(speedpot) with max speed wince we dont have a potentiometer
  step1.setSpeed(Motor1speed);
  step1.runSpeed();
}

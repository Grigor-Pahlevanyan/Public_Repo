#include <Stepper.h>
#include <TFMPlus.h>  // Include TFMini Plus Library v1.5.0
TFMPlus tfmP;         // Create a TFMini Plus object
#include "printf.h"   // Modified to support Intel based Arduino


const int stepsPerRevolution = 800;  // change this to fit the number of steps per revolution

// initialize the stepper library on pins 8 and 9:
Stepper myStepper(stepsPerRevolution, 8, 9);

void setup(){

    myStepper.setSpeed(60);  //Sets the motor speed in rotations per minute (RPMs). This function doesn't make the motor turn,
                             //just sets the speed at which it will when you call step().           
    printf_begin();          // Initialize printf.                                 
    Serial2.begin(115200);                     
    delay(20);               // Give port time to initalize
    printf("\r\nTFMPlus Library Example - 10SEP2021\r\n");  // say 'hello'

    Serial2.begin(115200);   // Initialize TFMPLus device serial port.
    delay(20);               // Give port time to initalize
    tfmP.begin( &Serial2);   // Initialize device library object and...
                             // pass device serial port to the object.

    // Send some example commands to the TFMini-Plus
    // - - Perform a system reset - - - - - - - - - - -
    printf( "Soft reset: ");
    if( tfmP.sendCommand( SOFT_RESET, 0))
    {
        printf( "passed.\r\n");
    }
    else tfmP.printReply();
  
    delay(500);  // added to allow the System Rest enough time to complete

  // - - Display the firmware version - - - - - - - - -
    printf( "Firmware version: ");
    if( tfmP.sendCommand( GET_FIRMWARE_VERSION, 0))
    {
        printf( "%1u.", tfmP.version[ 0]); // print three single numbers
        printf( "%1u.", tfmP.version[ 1]); // each separated by a dot
        printf( "%1u\r\n", tfmP.version[ 2]);
    }
    else tfmP.printReply();
    // - - Set the data frame-rate to 20Hz - - - - - - - -
    printf( "Data-Frame rate: ");
    if( tfmP.sendCommand( SET_FRAME_RATE, FRAME_20))
    {
        printf( "%2uHz.\r\n", FRAME_20);
    }
    else tfmP.printReply();
    
    // - - - - - - - - - - - - - - - - - - - - - - - -

  delay(500);            // And wait for half a second.
}

// Initialize variables
int count = 2;
int counter = 0;
int i = 2;
int j = 0;
int reset = 0;
int16_t tfDist = 10000;    // Distance to object in centimeters
int16_t tfFlux = 0;    // Strength or quality of return signal
int16_t tfTemp = 0;    // Internal temperature of Lidar sensor chip
int16_t tfDist1 = 0;    // Distance to object in centimeters
int16_t tfDist2 = 0;    // Distance to object in centimeters

void loop(){
// The loop consists of 2 parts, the LiDAR and the Stepper Motor.
//_______________________________________________________________________________________________________________________________
// The code below computes your speed relative to the car in front of you. Basically the rate at which the distance is decreasing. 
// If this rate is too fast then check how much distance you have left with the car in front.
  while((tfDist1-tfDist2)/0.02 <= 1111.1){//(dist2 - dist1)/0.02 == 1111.1cm/s which is 40km/h
    delay(20);   // Loop delay to match the 20Hz data frame rate (change back to 50)
    tfmP.getData(tfDist1); // Get initial distance from the LiDAR.
    delay(20);   // Loop delay to match the 20Hz data frame rate (change back to 50), note delay(1000) == 1 second
    tfmP.getData(tfDist2); // Get final distance from the LiDAR.
    printf( "Dist:%d cm", tfDist2);   // display distance,
    printf( "\tFlux:%05i ",   tfFlux);   // display signal strength/quality,
    printf( "\tTemp:%2i%s",  tfTemp, "C");   // display temperature,
    printf( "\r\n");                   // end-of-line.
}

// The function below loops until the distance with the car in front is less than a certain threshold, in our case 1 meter.
// At this point the car is approaching very fast to the object in front, now if the distance is only 1m (or any threshold)
// then immidiately leave this loop and activate the stepper motor to sent the driver/passenger back.
  while(tfDist > 100){ // 
          tfmP.getData(tfDist); // Get data from the device.
          delay(50);   // Loop delay to match the 20Hz data frame rate (change back to 50)
                       // note delay(1000) == 1 second
                       // DECREASE THIS TIME SO ITS MORE ACCURATE
          printf( "Dist:%d cm", tfDist);   // display distance,
          printf( "\r\n");                   // end-of-line.
   }

 //______________________________________________________________________________________________________________________
 // Every code below this controls the stepper motor. I had to manually create the acceleration and velocity of the motor.

// TESTING BOUNDARIES
      myStepper.setSpeed(80); // Don't use 1000 speed for testing on 400 steps
      myStepper.step(-stepsPerRevolution);
        
//  while(count != 25){ //Accelerate          CHANGE THE != TO <
//      myStepper.setSpeed(60+count*40);
//      myStepper.step(stepsPerRevolution);
//      count++;
//  }
   
//  while(counter != 20){ //Decelerate
//      myStepper.step(stepsPerRevolution);
//      myStepper.setSpeed(700-30*(counter));
//      counter++;
//  }
//  delay(3000); //wait 10 seconds

// reseting the position of the seat after some time
   
//    while(i != 5){ // Accelerate reverse
//      myStepper.setSpeed(70+i*(30));
//      myStepper.step(-stepsPerRevolution);
//      i++;
//      //delay(1);
//  }
//
//    while(reset != 15){ // Constant verlocity reverse
//      myStepper.step(-stepsPerRevolution);
//      myStepper.setSpeed(190);
//      reset++;
//    }
//
//    while(j != 6){ // Decelerate
//      myStepper.step(-stepsPerRevolution);
//      myStepper.setSpeed(190-20*(j));
//      j++;
//      //delay(1);
//  }
}

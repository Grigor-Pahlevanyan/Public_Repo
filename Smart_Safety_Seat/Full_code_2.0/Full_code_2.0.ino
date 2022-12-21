#include <AccelStepper.h>
#include <TFMPlus.h>                          // Include TFMini Plus Library v1.5.0
TFMPlus tfmP;                                 // Create a TFMini Plus object

//Initialize Variables________________________________________
const int stepsPerRevolution = 400;           // change this to fit the number of steps per revolution
AccelStepper stepper(1,8,9);
int led1 = 5;                                 // Green LED
int led2 = 6;                                 // Red LED
bool toggle = true;                           // Led is on when true, off when false
enum condition{GOOD, EMERGENCY, FINISH};      // A new type instead of using integers
condition system_state = GOOD;                // Either scanning or triggered
int16_t tfDist = 10000;                       // Distance to object in centimeters
int16_t tfDist1 = 0;                          // Distance to object in centimeters
int16_t tfDist2 = 0;                          // Distance to object in centimeters

void setup(){
  
  //LIGHTS CONFIGURATION_______________________________________
  pinMode(led1, OUTPUT);                      // Green led setup
  pinMode(led2, OUTPUT);                      // Red Led setup

  //STEPPER CONFIGURATION_______________________________________
  stepper.setMaxSpeed(5000);        // Speed = Steps/Second
  stepper.setAcceleration(5000);    // Acceleration = Steps/(second^2)
  stepper.disableOutputs();         // Disable outputs -> motor won't get hot 
  stepper.setCurrentPosition(0);  

  //TIMER1 CONFIGURATION________________________________________
  TCCR1A = 0;                                 // Set entire TCCR1A register to 0
  TCCR1B = 0;                                 // Same for TCCR1B
  TCNT1  = 0;                                 // Initialize counter value to 0
  
  // Set The Output Compare Register
  OCR1A = 4999;                               // An iterupt will occur once the timer overflows on the OCR value
  
  // turn on CTC mode
  TCCR1B |= B00000100;                        // (1 << WGM12);//B00000100; 
   
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);        // 16*10^6)/(1024) - 1 = 15.624kHz instead of 16MHz
  
  // enable timer compare interrupt
  TIMSK1 |= B00000010;                        // (1 << OCIE1A); //B00000010; 

  //STEPPER CONFIGURATION_____________________________________________________
  //myStepper.setSpeed(60);                     // Sets the motor speed in rotations per minute (RPMs)
                                              // This function doesn't make the motor turn  

  //LIDAR CONFIGURATION________________________________________________________
  Serial2.begin(115200);                      // Initialize TFMPLus device serial port.
  delay(20);                                  // Give port time to initalize
  tfmP.begin( &Serial2);                      // Initialize device library object and...
                                              // pass device serial port to the object.
  delay(1000);                                 // And wait for half a second. (give lidar time to collect data)
 
  sei();                                      //allow interrupts
}

// Timer1 interupt for the signal LEDs_________________________________________
ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;                                  // This is an important piece of the interupt
  if(system_state == GOOD){                   // If system is in scanning mode, blink green LED
    toggle = !toggle;
    digitalWrite(led1, toggle);               // Toggle green LED
  }
  
  else if(system_state == EMERGENCY){         // Otherwise, in active/triggered mode blink red LED
    toggle = !toggle;
    digitalWrite(led1, LOW);                  // Turn green LED off
    digitalWrite(led2, toggle);               // Toggle red LED
  }
  else{
    digitalWrite(led1, LOW);                  // Turn green LED off
    digitalWrite(led2, LOW);                  // Turn red LED off
  }
}

void loop(){
// LIDAR SCANNING__________________________________________________________
  if(system_state == GOOD){
    tfmP.getData(tfDist1);       // Get initial distance from the LiDAR.
    delay(20);                   // Loop delay to match the 20Hz data frame rate (change back to 50), note delay(1000) == 1 second
    tfmP.getData(tfDist2);       // Get final distance from the LiDAR.
    delay(20);                   // Loop delay to match the 20Hz data frame rate (change back to 50)
  }

  // Speed relative to the car in front and relative distance
  if(system_state == GOOD && (tfDist1-tfDist2)/0.02 >= 400 && tfDist > 500){   //(dist2 - dist1)/0.02 == 1111.1cm/s which is 40km/h (5 meters now)
    system_state = EMERGENCY;
 }

 // Emergency Stepper______________________________________________________
  if(system_state == EMERGENCY){
    stepper.enableOutputs(); // Enables the pins
    stepper.runToNewPosition(9500);// 9500
    stepper.run(); // Move 1 step per clock cycle
    delay(3000);
    stepper.setMaxSpeed(2000);        // Speed = Steps/Second
    stepper.setAcceleration(200);    // Acceleration = Steps/(second^2)
    stepper.runToNewPosition(0);
    stepper.run(); // Move 1 step per clock cycle
    system_state = FINISH;
  }

  if(system_state == FINISH){
    stepper.disableOutputs();         // Disable outputs -> motor won't get hot 
  }
}

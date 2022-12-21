/* File Name: TFMP_example.ino
 * Developer: Bud Ryerson
 * Inception: 29JAN2019
 * Last work: 10SEP2021

 * Description: Arduino sketch to test the Benewake TFMini Plus
 * time-of-flight Lidar ranging sensor using the TFMPlus Library.

 * Default settings for the TFMini Plus are a 115200 serial baud rate
 * and a 100Hz measurement frame rate. The device will begin returning
 * measurement data right away:
 *   Distance in centimeters,
 *   Signal strength in arbitrary units,
 *   and an encoded number for Temperature in degrees centigrade.

 * Use the 'sendCommand()' to send commands and return a status code.
 * Commands are selected from the library's list of defined commands.
 * Parameters can be entered directly (115200, 250, etc) but for
 * safety, they should be chosen from the library's defined lists.
 */

#include <TFMPlus.h>  // Include TFMini Plus Library v1.5.0
TFMPlus tfmP;         // Create a TFMini Plus object

#include "printf.h"   // Modified to support Intel based Arduino
                      // devices such as the Galileo. Download from:
                      // https://github.com/spaniakos/AES/blob/master/printf.h

// The Software Serial library is an alternative for devices that
// have only one hardware serial port. Delete the comment slashes
// on lines 37 and 38 to invoke the library, and be sure to choose
// the correct RX and TX pins: pins 10 and 11 in this example. Then
// in the 'setup' section, change the name of the hardware 'Serial2'
// port to match the name of your software serial port, such as:
// 'mySerial.begin(115200); etc.

//#include <SoftwareSerial.h>       
//SoftwareSerial mySerial( 10, 11);   

// Initialize variables
int16_t count = 0;
int16_t tfDist = 0;    // Distance to object in centimeters
int16_t tfFlux = 0;    // Strength or quality of return signal
int16_t tfTemp = 0;    // Internal temperature of Lidar sensor chip
void ISR(TIMER1_COMPA_vect);
                                    
void setup()
{
    printf_begin();          // Initialize printf.
    Serial.begin(115200);    // Intialize terminal serial port
    delay(20);               // Give port time to initalize

    Serial2.begin( 115200);  // Initialize TFMPLus device serial port.
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

  delay(1000);            // And wait for a second. (Give the sensor enough time to read values)
  
    // Setup the timer at the end**
    cli();//stop interrupts
      //set timer1 interrupt at 1Hz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = 9999;// = (16*10^6) / (1*1024) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei();//allow interrupts
}

// Use the 'getData' function to pass back device data.
void loop()
{
  printf("Hey man \n");
}

void ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED){
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)

// The code below is the original code for reading the measurments of the LiDAR
    if(count == 0){
    //tfmP.getData(tfDist); // Get data from the device.
    printf( "Dist:%d cm", tfDist);   // display distance,
    //printf( "\tFlux:%05i ",   tfFlux);   // display signal strength/quality,
    //printf( "\tTemp:%2i%s",  tfTemp, "C");   // display temperature,
    printf( "\r\n");                   // end-of-line.
    //count = 1;
    }
//    else if(count == 1){
//      count = 2;
//    }
//    else{
//      count = 0;
//    }
}

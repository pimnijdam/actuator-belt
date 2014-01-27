#include <avr/power.h>
#include <avr/sleep.h>

#include <Wire.h> //I2C Arduino Library
#include<math.h>

#define address 0x1E //0011110b, I2C 7bit address of HMC5883
#define DEGREES_PER_RAD (180.0 / PI)

double rad2deg(double rad) {
  return rad * DEGREES_PER_RAD;
}


int ledpin = 13; // LED connected to pin 48 (on-board LED)

unsigned char actuators = 1;

int jiffies = 0;

void sleepNow()
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and 
     * wake up sources are available in which sleep modus.
     *
     * In the avr/sleep.h file, the call names of these sleep modus are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings 
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     *  the power reduction management <avr/power.h>  is described in 
     *  http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
     */  
     
  set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin 
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  
  
  sleep_mode();            // here the device is actually put to sleep!!
 
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();         // first thing after waking from sleep:
                            // disable sleep...

  power_all_enable();
   
}


void setupCompass() {
  //Setup HMC5883 for continuous measurement
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void setup() {

  //setup led
  pinMode(ledpin, OUTPUT);  // pin 48 (on-board LED) as OUTPUT
  //setup serial
  Serial.begin(9600);       // start serial communication at 9600bps
  //setup I2C with compass
  Wire.begin();
  setupCompass();
  
  //setup motors
  pinMode( 2, OUTPUT);
  digitalWrite(2,HIGH);
  pinMode( 3, OUTPUT);
  digitalWrite(3,HIGH);
  pinMode( 4, OUTPUT);
  digitalWrite(4,HIGH);
  pinMode( 5, OUTPUT);
  digitalWrite(5,HIGH);
  pinMode( 6, OUTPUT);
  digitalWrite(6,HIGH);
  pinMode( 7, OUTPUT);
  digitalWrite(7,HIGH);
  pinMode( 8, OUTPUT);
  digitalWrite(8,HIGH);
  pinMode( 9, OUTPUT);
  digitalWrite(9,HIGH);
  
  digitalWrite(2,LOW);
}

void write_pins(unsigned char values)
{
  unsigned char i;
  for (i = 0; i < 8; i++){
    pinMode( 2+i, (values & (1<<i)) ? HIGH : LOW);
  }
}

void disable_actuator(unsigned char actuator_id)
{
  actuators &= ~(1 << actuator_id);
}

void enable_actuator(unsigned char actuator_id)
{
  actuators |= (1 << actuator_id);
}

void enable_actuator_only(unsigned char actuator_id) {
  actuators = 1<<actuator_id;
}

void enable_all_actuators()
{
    actuators = 255;
}

void disable_all_actuators()
{
  actuators = 0;
}

int getCompassHeading() {
   int x,y,z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  

 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
  
  double headingRad = atan2(x,z);
  double headingDeg = round(rad2deg(headingRad));
  if (headingDeg < 0) {
    headingDeg += 360;
  }
  return round (headingDeg);
}


char getDirection() {
  int heading = getCompassHeading();
  //correct for different orientation of actuator and sensor
  heading = (heading + 90) % 360;
  //translate into eight areas
  const int nDirs = 8;
  int dir = ((int)round(heading * nDirs / 360.0))%nDirs;
  //this was the old serial protocol sending a character so do that.
  return 'A' + dir;
}

int mode = 1; //mode 0: serial input, mode 1: compass
void loop() {
  char pin                = 0;
  unsigned char mask      = 0;
  boolean changed = false;
  
  if (++jiffies >= 500) {
    //If there is a problem with the compass, e.g. short power outage it won't function.
    //So setup the compass every 5 seconds.
    setupCompass();
    jiffies = 0;
  }

  unsigned char actuators_prev = actuators;
  char val = -1;
  
  if (mode == 0 && Serial.available() )       // if data is available to read
  {
    val = Serial.read();         // read it and store it in 'val'
  } else if (mode == 1 && (jiffies%5) == 0) {
    //read from compass
    val = getDirection();
  }
  if (val >= 'a' && val <= 'h')
    disable_actuator(val-'a');
  else if (val >= 'A' && val <= 'H')
    enable_actuator_only(val-'A');
  else if (val == 'i')
    digitalWrite(ledpin,LOW);
  else if (val == 'I')
    digitalWrite(ledpin, HIGH);
  else if (val == 'J')
    enable_all_actuators();
  else if (val == 'j')
    disable_all_actuators();
  else if (val == 'k')
    mode = (mode + 1) % 2;
    
    
  changed = actuators != actuators_prev;
  
  if (jiffies == 0) {
    write_pins(actuators);
  } else if (jiffies == 10) {
    write_pins(0);
  } else if (jiffies == 20) {
    write_pins(actuators);
  } else if (jiffies == 40) {
    write_pins(0);
  } else if (changed) {
    write_pins(actuators);
    jiffies = 0;
  }
 
  delay(10);
} 

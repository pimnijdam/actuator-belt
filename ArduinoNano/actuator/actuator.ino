char val; // variable to receive data from the serial port
int ledpin = 13; // LED connected to pin 48 (on-board LED)

unsigned char actuators = 1;

unsigned char jiffies = 0;

void setup() {

  pinMode(ledpin, OUTPUT);  // pin 48 (on-board LED) as OUTPUT
  Serial.begin(9600);       // start serial communication at 9600bps
  
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

void enable_all_actuators()
{
  actuators = 255;
}

void disable_all_actuators()
{
  actuators = 0;
}

void loop() {
  char pin                = 0;
  unsigned char mask      = 0;
  
  if (++jiffies >= 100)
    jiffies = 0;
  
  if( Serial.available() )       // if data is available to read
  {
    val = Serial.read();         // read it and store it in 'val'
    
    if (val >= 'a' && val <= 'h')
      disable_actuator(val-'a');
    else if (val >= 'A' && val <= 'H')
      enable_actuator(val-'A');
    else if (val == 'i')
      digitalWrite(ledpin,LOW);
    else if (val == 'I')
      digitalWrite(ledpin, HIGH);
    else if (val == 'J')
      enable_all_actuators();
    else if (val == 'j')
      disable_all_actuators();
  }
  

  write_pins(actuators & (jiffies > 90 ? 255 : 0));
  //write_pins(1);
  delay(10);
} 

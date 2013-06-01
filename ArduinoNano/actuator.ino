char val; // variable to receive data from the serial port
int ledpin = 13; // LED connected to pin 48 (on-board LED)

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
}

void loop() {

  if( Serial.available() )       // if data is available to read
  {
    val = Serial.read();         // read it and store it in 'val'
  }
  
  
  char pin;
  
  if (val >= 'a' && val <= 'h')
    digitalWrite(val-'a'+2, HIGH);
  if (val >= 'A' && val <= 'H')
    digitalWrite(val-'A'+2, LOW);
  if (val == 'i')
    digitalWrite(ledpin,LOW);
  if (val == 'I')
    digitalWrite(ledpin, HIGH);
  
  
  
  delay(100);                    // wait 100ms for next reading
} 

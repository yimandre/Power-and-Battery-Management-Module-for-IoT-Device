#include <Wire.h>
// Include the required Wire library for I2C<br>#include <Wire.h>
int x=0;
int y=5;
void setup() {
  // Define the LED pin as Output
  pinMode (LED_BUILTIN, OUTPUT);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);  
  Serial.begin(9600);

  Wire.begin();
  delay(1000);
}
void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C

}
void loop() {  
  Serial.println(x);
    delay(500);
    if(x==5|| x==3){
    y = 6;
  Wire.beginTransmission(9);
  Wire.write(y);
  Wire.endTransmission();
  
  }
//  delay(500);
//  
//
//  y++;
// if(y>9) y=5;
// delay(3000);
 
}


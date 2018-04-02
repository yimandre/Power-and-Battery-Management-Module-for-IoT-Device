#include <Wire.h>
int x =0;
int y =0;
void setup() {
  // Start the I2C Bus as Master
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void receiveEvent(int bytes) {
  y = Wire.read();    // read one character from the I2C
  Serial.println(y);
}
void loop() {
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(x);              // sends x 
  Wire.endTransmission();    // stop transmitting
  x++; // Increment x
  if (x > 5) x = 0; // `reset x once it gets 6
  delay(1000);
}


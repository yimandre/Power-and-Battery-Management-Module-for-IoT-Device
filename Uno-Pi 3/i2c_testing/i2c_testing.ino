//
#include <Wire.h>

int address_Pi = 3;
int bytes;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Communication OK");
  Wire.begin();

}

void receiveFromPi(void) {

  Wire.requestFrom(address_Pi, 1);
  
  bytes = Wire.read();
  
}

void loop() {
  
  if (Wire.available() > 0){
    receiveFromPi();
  }

  Serial.println(bytes);

}


#include <Wire.h>
#include <LiFuelGauge.h>

// Creates a LiFuelGauge instance for the MAX17043 IC
LiFuelGauge gauge(MAX17043);

void setup()
{
    Serial.begin(9600); // Initialize serial port
    while ( !Serial ) ;
    
    gauge.reset();  // Resets MAX17043
    delay(1000);

    Serial.println(gauge.getSOC());
     
}

void loop()
{
  
}


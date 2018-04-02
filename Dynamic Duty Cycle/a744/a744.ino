/* Sysc4907 Team 11 - Power and Battery Management
 * Supervisor: Professor Ibnkahla
 * Co-Supervisor: Zied Bouida
 * Members: Jacob Sharkey, Shawn Khadem, June Looi, Andre Yim, Ashley Hooker
 * 
 * Purpose is to measure the state of charge (SOC) of 3 different lithium polymer batteries, turn on a MOSFET, send the measured data, then turn off the MOSFET for a set time calculated by the total mAh left in all 3 batteries combined; 
 * 
 */

#include <Wire.h>
#include <LiFuelGauge.h>

#define TCAADDR 0x70 //establishes TCA i2c mux address

LiFuelGauge gauge(MAX17043); //creates LiFuelGauge object

void tcaselect(uint8_t i) { //function used to select ports in i2c mux
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

double batt0 = 150, batt1 = 2000, batt2 = 150; // mAh of batteries connected to ports 0-2
double totalmAh = batt0 + batt1 + batt2;
double SOC0 = 0, SOC1 = 0, SOC2 = 0;
double minSOCtotal = 15;

int IoT1 = 7; // pin 7 connected to MOSFET that controls IoT device 1
int IoT2 = 8;
bool twoIoTDevs = true;
int timeOff = 0;
int bootupTime = 2;
int timeOn = 5;
int inMinutes = 1000;

int SerialBaudRate = 9600;

double getSOC0(){
  tcaselect(0);   
  delay(250);
  //gauge.reset();
  //delay(100);
  return gauge.getSOC();
}

double getSOC1(){
  tcaselect(1);   
  delay(250);
  return gauge.getSOC();
}

double getSOC2(){
  tcaselect(2);   
  delay(250);
  return gauge.getSOC();
}

void setup() {

  pinMode(IoT1, OUTPUT);
  pinMode(IoT2, OUTPUT);

}

void loop() {
  
  for(int t=0; t<3; t++){ //loop to reset all fuel gauges
    tcaselect(t);
    gauge.reset();
    delay(250);
  }
  
  //Get SOCs
  SOC0 = getSOC0();
  SOC1 = getSOC1(); 
  SOC2 = getSOC2();   
  
  //Total SOC
  double SOCtotal = (SOC0*batt0 + SOC1*batt1 + SOC2*batt2)/totalmAh;
  
  if (SOCtotal>minSOCtotal){ //check for low batteries -> turns on IoT device(s) if higher than minSOCSum
    
    //turn on IoT device(s)
    digitalWrite(7,HIGH); //1st IoT device on
    delay (bootupTime*inMinutes);
    
    //send initial SOCs
    Serial.begin(SerialBaudRate);
    while(!Serial);
    
    Serial.print(SOC0); Serial.print(",");
    Serial.print(SOC1); Serial.print(",");
    Serial.print(SOC2); Serial.print(",");
    Serial.println();
    Serial.print("SOCtotal: "); Serial.println(SOCtotal);
    
    delay(timeOn*inMinutes); //1st IoT device on
    
    digitalWrite(7,LOW); //1st IoT device off
    
    if (twoIoTDevs){
    
    digitalWrite(8,HIGH); //2nd IoT device on
    delay (bootupTime*inMinutes);   
    delay(timeOn*inMinutes);
    digitalWrite(8, LOW); //2nd IoT device off
    
    }

    //Acquire new SOCs
    SOC0 = getSOC0();
    SOC1 = getSOC1();
    SOC2 = getSOC2();
       
    //Total SOCs
    double SOCtotal = (SOC0*batt0 + SOC1*batt1 + SOC2*batt2)/totalmAh;
  
    //Make decision of timeOff duration based on new SOCs
    if (SOCtotal > 85){
      timeOff = 15;
    } 
    else if (85 < SOCtotal < 50){
      timeOff = 30;
    } 
    else if (25 < SOCtotal < 50){
      timeOff = 45;
    }
    else {
      timeOff = 60; //default case
    }
    
  } else { //default case if batteries are low
    timeOff = 60;
  }
  
 delay (timeOff*inMinutes);
 
}

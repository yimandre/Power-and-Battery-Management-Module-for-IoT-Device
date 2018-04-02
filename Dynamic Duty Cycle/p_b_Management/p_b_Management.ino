/* Sysc4907 Team 11 - Power and Battery Management
 * Supervisor: Professor Ibnkahla
 * Co-Supervisor: Zied Bouida
 * Members: Jacob Sharkey, Shawn Khadem, June Looi, Andre Yim, Ashley Hooker
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
double minSOCtotal = 15; //minimum percent of total state of charge

int IoT1 = 7; // pin 7 connected to MOSFET that controls 1st IoT device
int IoT2 = 8; // pin 8 connected to MOSFET that controls 2nd IoT device
bool twoIoTDevs = true;
int bootupTime1 = 2, timeOn1 = 5; //1st IoT device parameters
int bootupTime2 = 2, timeOn2 = 5; //2nd IoT device parameters
int timeOff = 0;
int inMinutes = 60000;

int SerialBaudRate = 9600;

double getSOC0(){ //retrieve SOC from port 0 of the mux
  tcaselect(0);   
  delay(250);
  //gauge.reset();
  //delay(100);
  return gauge.getSOC();
}

double getSOC1(){ //retrieve SOC from port 1 of the mux
  tcaselect(1);   
  delay(250);
  return gauge.getSOC();
}

double getSOC2(){ //retrieve SOC from port 1 of the mux
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
  
  //Retreive SOCs
  SOC0 = getSOC0();
  SOC1 = getSOC1(); 
  SOC2 = getSOC2();   
  
  //Total SOC as a percent of maximum capacity
  double SOCtotal = (SOC0*batt0 + SOC1*batt1 + SOC2*batt2)/totalmAh; 
  
  if (SOCtotal>minSOCtotal){ //check for low batteries -> turns on IoT device(s) if higher than minSOCSum
    
    //turn on IoT device(s)
    digitalWrite(7,HIGH); //1st IoT device on
    delay (bootupTime1*inMinutes);
    
    //send initial SOCs to 1st IoT
    Serial.begin(SerialBaudRate);
    while(!Serial);
    
    Serial.print(SOC0); Serial.print(",");
    Serial.print(SOC1); Serial.print(",");
    Serial.print(SOC2); Serial.print(",");
    Serial.println();
    Serial.print("SOCtotal: "); Serial.println(SOCtotal);
    
    delay(timeOn1*inMinutes); //1st IoT device on
    
    digitalWrite(7,LOW); //1st IoT device off
    delay(500);
    
    if (twoIoTDevs){
    
    digitalWrite(8,HIGH); //2nd IoT device on
    delay (bootupTime2*inMinutes);   
    delay(timeOn2*inMinutes);
    digitalWrite(8, LOW); //2nd IoT device off
    
    }

    //Acquire new SOCs
    SOC0 = getSOC0();
    SOC1 = getSOC1();
    SOC2 = getSOC2();
       
    //Total SOC as a percent of maximum capacity
    double SOCtotal = (SOC0*batt0 + SOC1*batt1 + SOC2*batt2)/totalmAh;
  
    //Set timeOff duration based on new SOCs (times expected to be user set differently based on situation) 
    if (SOCtotal > 85){
      timeOff = 15;
    } 
    else if (SOCtotal > 50){
      timeOff = 30;
    } 
    else if (SOCtotal > 25){
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

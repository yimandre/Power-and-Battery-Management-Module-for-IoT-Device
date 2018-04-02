/* Sysc4907 Team 11 - Power and Battery Management
 *  Professor Ibnkahla
 * Jacob Sharkey, Shawn Khadem, June Looi, Andre Yim, Ashley Hooker
 */

#include <Wire.h>
#include <LiFuelGauge.h>

#define TCAADDR 0x70 //establishes TCA i2c mux address

LiFuelGauge gauge(MAX17043); //creates LiFuelGauge object

void tcaselect(uint8_t i) { //function used to select ports in i2c mux
  if (i > 3) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

double SOC1, SOC2, SOC3;
double percent1 = 0.60;
double percent2 = 0.30;
double percent3 = 0.10;

int timeOff;
int bootupTime = 2;
int timeOn = 3;
int inMinutes = 60000;

bool twoIoTDevs = false;
double minSOCSum = 15; 

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

double getSOC3(){
  tcaselect(3);   
  delay(250);
  return gauge.getSOC();
}

void setup() {

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  
  Serial.begin(9600);
  while(!Serial);
  //Serial.println("Communication OK\n");

  //loop to reset all fuel gauges
  for(int t=1; t<4; t++){ //loop to reset all fuel gauges
    tcaselect(t);
    gauge.reset();
    delay(250);
  }
  //Serial.println("Gauges reset");

  //initial measurements
  //Serial.println("Initial measurements");
  SOC1 = getSOC1();
  //Serial.print("Port 1: "); Serial.print(SOC1); Serial.println("%");
  SOC2 = getSOC2();
  //Serial.print("Port 2: "); Serial.print(SOC2); Serial.println("%");
  SOC3 = getSOC3();
  //Serial.print("Port 3: "); Serial.print(SOC3); Serial.println("%");
  
}

void loop() {

  //check for low batteries -> do not turn on IoT device(s)
  
  //Get SOCs
  SOC1 = getSOC1();
  SOC2 = getSOC2(); 
  SOC3 = getSOC3();   

  //"Sum" SOC
  double SOCSum = SOC1*percent1 + SOC2*percent2 + SOC3*percent3;

  if (SOCSum>minSOCSum){ //check for low batteries -> does not turn on IoT device(s) if low
    
    //turn on IoT device(s)
    digitalWrite(7,HIGH); //1st IoT device on
    delay (bootupTime*inMinutes);
    
    //communication can be done here (weather)
    //send initial SOCs?
        
    delay(timeOn*inMinutes); //1st IoT device does its thing
    
    //Get new SOCs and send to IoT device
    SOC1 = getSOC1(); Serial.println(SOC1);
    SOC2 = getSOC2(); Serial.println(SOC2);
    SOC3 = getSOC3(); Serial.println(SOC3);
    
    digitalWrite(7,LOW); //1st IoT device off
    
    if (twoIoTDevs){
    
    digitalWrite(8,HIGH); //2nd IoT device on
    delay (bootupTime*inMinutes);   
    delay(timeOn*inMinutes); //2n IoT device does its thing
    digitalWrite(8, LOW); //2nd IoT device off
    
    }

    //Get new SOCs
    SOC1 = getSOC1();
    SOC2 = getSOC2();
    SOC3 = getSOC3();
       
    //"Sum" SOC
    double SOCSum = SOC1*percent1 + SOC2*percent2 + SOC3*percent3;
  
    //Make decision of timeOff based on SOCs
    if (SOCSum > 85){
      timeOff = 15;
    } 
    else if (85 < SOCSum < 50){
      timeOff = 30;
    } 
    else if (25 < SOCSum < 50){
      timeOff = 45;
    }
    else {
      timeOff = 60; //default case
    }
    
  } else {
    timeOff = 60;
  }

  //Serial.print("IoT device(s) off for "); Serial.print(timeOff); Serial.println(" minutes");  
  //arduino low power mode 
  delay (timeOff*inMinutes);
}

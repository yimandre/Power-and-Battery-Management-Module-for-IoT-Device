double SoC;

int timeOff;
int bootupTime = 2;
int timeOn = 3;

double maxSoC;
double medSoC;
double minSoC;

int weatherInfo;

int toMinutes = 60000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Good to go");
  
}

void lowWeather(){
  timeOff = 60*toMinutes;
  timeOn = 5*toMinutes;
}

void medWeather(){
  
  if (20<SoC<50){
        timeOff = 60*toMinutes;
        timeOn = 5*toMinutes;
  }

   else if (50<SoC<85){
        timeOff = 30*toMinutes;
        timeOn = 5*toMinutes;
   }
           
   else if (SoC>85) {
        timeOff = 15*toMinutes;
        timeOn = 5*toMinutes;
   }
   else {
        timeOff = 60*toMinutes;
        timeOn = 0;
  }
}

void highWeather(){
  if (20<SoC<50){
        timeOff = 40*toMinutes;
        timeOn = 5*toMinutes;
  }

   else if (50<SoC<85){
        timeOff = 20*toMinutes;
        timeOn = 5*toMinutes;
   }
           
   else if (SoC>85) {
        timeOff = 15*toMinutes;
        timeOn = 5*toMinutes;
   }
   else {
        timeOff = 60*toMinutes;
        timeOn = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  if (Serial.available()){
      
    digitalWrite(13,HIGH);
    
    delay (bootupTime);
    //communicate with Pi
    //weatherInfo = I2C weater info;
   
    delay(timeOn); //Pi does its thing

    SoC = Serial.read();

    //Get SoC
   
    if (weatherInfo == 2){
      medWeather();  
    }

    else if (weatherInfo == 3){
      highWeather();  
    }  

    else {
      lowWeather();
    }

    digitalWrite(13, LOW);

    //arduino low power mode?
    
    delay (timeOff);
       
  }
}

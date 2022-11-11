 //#define BLYNK_MAX_READBYTES 1024
//#define BLYNK_MSG_LIMIT 1024
#define BLYNK_HEARTBEAT 20
#define BLYNK_NO_BUILTIN 
//===========================================================

char server[]="192.168.31.207";
#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
//#include <BlynkSimpleEsp8266.h>//addedadittionaly for local server
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
////char auth[] = "npvrbt1VI77mPeEIxb3IA9hePibjx3kw";//blynk cloud server auth
char auth[] = "jJ2rKM6dWUaOzPZYPvAjfgiGUI9PpXPu";//local server auth
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "VADLAMUDI";
char pass[] = "9849692804";
// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1
// or Software Serial on Uno, Nano...
//#include <SoftwareSerial.h>
//SoftwareSerial EspSerial(2, 3); // RX, TX
// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600//changed to 9600 from 115200
ESP8266 wifi(&Serial);
//////////////////////////////////////////
BlynkTimer timer;

////////////////////////////////////////////////////
const int chargingpin =2;   
const int powerstatuspin =3;      
const int batvoltagepin =A0;   
const int batcurrentpin =A1;    
const int temperaturepin =A2;  

// Variables will change:
bool chargingstatus = LOW;  
bool powerstatus = LOW;  

int batcurrenttemp = 0;   
float batcurrent = 0.00; 
int batvolttemp =0;
float batvolt=0.00; 
float temperature=0.00;

float chargingcurrent=0.00;
float dischargingcurrent=0.00;



bool notificationstatus=1;
bool l=0;
bool m=0;
bool n=0;


unsigned long previousMillis = 0;  
unsigned long previousMillis1 = 0;  
unsigned long previousMillis2 = 0;
const long interval = 10;  

const int numReadings = 10;
int readings[numReadings];     
int readIndex = 0;              
int totalcurrent = 0;                 
int averagecurrent = 0;  

const int numReadings1 = 10;
int readings1[numReadings1];     
int readIndex1 = 0;              
int totalvoltage = 0;                 
int averagevoltage = 0; 

 const int numReadings2 = 10;
int readings2[numReadings2];     
int readIndex2 = 0;              
int totaltemperature = 0;                 
int averagetemperature = 0; 

/////////////////////////////////////////////////////////

BLYNK_CONNECTED() {
  // Request the latest state from the server
//  Blynk.syncVirtual(V0);
//  Blynk.syncVirtual(V1);
//  Blynk.syncVirtual(V2);
//  Blynk.syncVirtual(V4);
  // Blynk.syncAll();
  // Alternatively, you could override server state using:
  //Blynk.virtualWrite(V2, ledState);
}


BLYNK_WRITE(V5)
{
   notificationstatus = param.asInt(); // assigning incoming value from pin V1 to a variable

}





void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, batvolt);
   Blynk.virtualWrite(V2, batcurrent);
    Blynk.virtualWrite(V3, powerstatus);
    Blynk.virtualWrite(V4, temperature);
}


void notification() {
  if (notificationstatus==1){
    
  }
  
}
///////////////////////////////////////////////

void setup() {
//   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
//    readings[thisReading] = 0;
//  }
//   for (int thisReading1 = 0; thisReading1 < numReadings1; thisReading1++) {
//    readings1[thisReading1] = 0;
//  }
//  for (int thisReading2 = 0; thisReading2 < numReadings2; thisReading2++) {
//    readings2[thisReading2] = 0;
//  }
 analogReference(EXTERNAL);    
 pinMode(powerstatuspin, INPUT);
 pinMode(batcurrentpin, INPUT);
 pinMode(batvoltagepin, INPUT);
 pinMode(temperaturepin, INPUT);
 pinMode(chargingpin, OUTPUT);
  digitalWrite(chargingpin, LOW);
  
  //=======================================
   Serial.begin(9600);
  delay(10);
  // Set ESP8266 baud rate
  Serial.begin(ESP8266_BAUD);
  delay(10);
 // Blynk.begin(auth, wifi, ssid, pass);//for cloud server 
  Blynk.begin(auth, wifi, ssid, pass, server, 8080);//for local server
 timer.setInterval(1000L, myTimerEvent);
//  timer.setInterval(1549L, myTimerEvent2);
//  timer.setInterval(2003L, myTimerEvent3);



}

void loop() {
 
  
  notification();
  Blynk.run();
  timer.run();
  averagevalues();
  chargingstatusfunction();
  
}

void averagevalues(){
  // subtract the last reading:
  totalcurrent = totalcurrent - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(batcurrentpin);
   unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  do{
    unsigned long currentMillis = millis();
  }
  while(currentMillis - previousMillis >= interval);
   readings[readIndex] = analogRead(batcurrentpin);
   readings[readIndex] = analogRead(batcurrentpin);
  // add the reading to the total:
  totalcurrent = totalcurrent + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  averagecurrent = totalcurrent / numReadings;
    batcurrent=((averagecurrent*(4.95/1023))-2.425)*1000;
    if (batcurrent>0){
      chargingcurrent=batcurrent;
    }
    else if(batcurrent<0){
      dischargingcurrent=batcurrent;// send it to the computer as ASCII digits
    }
  //Serial.println(average);
 // delay(1);        
//=======================================================================
  totalvoltage = totalvoltage - readings1[readIndex1];
  // read from the sensor:
  readings1[readIndex1] = analogRead(batvoltagepin);
   unsigned long currentMillis1 = millis();
  previousMillis1 = currentMillis1;
  do{
    unsigned long currentMillis1 = millis();
  }
  while(currentMillis1 - previousMillis1 >= interval);
   readings1[readIndex1] = analogRead(batvoltagepin);
   readings1[readIndex1] = analogRead(batvoltagepin);
  // add the reading to the total:
  totalvoltage = totalvoltage + readings1[readIndex1];
  // advance to the next position in the array:
  readIndex1 = readIndex1 + 1;

  // if we're at the end of the array...
  if (readIndex1 >= numReadings1) {
    // ...wrap around to the beginning:
    readIndex1 = 0;
  }

  // calculate the average:
  averagevoltage = totalvoltage / numReadings1;
  batvolt=(averagevoltage*(4.95/1023))+8.91;
  //============================================================
  totaltemperature = totaltemperature - readings2[readIndex2];
  // read from the sensor:
  readings2[readIndex2] = analogRead(temperaturepin);
   unsigned long currentMillis2 = millis();
  previousMillis2 = currentMillis2;
  do{
    unsigned long currentMillis2 = millis();
  }
  while(currentMillis2 - previousMillis2 >= interval);
   readings2[readIndex2] = analogRead(temperaturepin);
   readings2[readIndex2] = analogRead(temperaturepin);
  // add the reading to the total:
  totaltemperature = totaltemperature + readings2[readIndex2];
  // advance to the next position in the array:
  readIndex2 = readIndex2 + 1;

  // if we're at the end of the array...
  if (readIndex2 >= numReadings2) {
    // ...wrap around to the beginning:
    readIndex2 = 0;
  }

  // calculate the average:
  averagetemperature = totaltemperature / numReadings2;
  temperature=(averagetemperature*(4.95/1023))*100;
}

void chargingstatusfunction(){
  powerstatus=digitalRead(powerstatuspin);
  if(batvolt<12.6 && powerstatus == HIGH){
    digitalWrite(chargingpin, HIGH);
  }
  
  if (batvolt>13.45 && chargingcurrent<50 ||powerstatus == LOW){
    digitalWrite(chargingpin, LOW);
    
  }
}

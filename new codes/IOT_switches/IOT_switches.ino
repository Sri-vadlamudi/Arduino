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
char auth[] = "eYo9t6ByIhgtRJrKUmF8d4svKIXTzQuP";//local server auth
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
#define ESP8266_BAUD 115200
ESP8266 wifi(&Serial);
//////////////////////////////////////////
BlynkTimer timer;

////////////////////////////////////////////////////
const int switch1 =2;    // the number of the pushbutton pin
const int switch2 =3;    
const int switch3 =4;  
const int relay1 =5;   
const int relay2 =6;    
const int relay3 =7;  

// Variables will change:
bool switch1status = LOW;   // the previous reading from the input pin
bool switch2status = LOW; 
bool switch3status = LOW; 

bool switch1status1 = LOW;   
bool switch2status1 = LOW; 
bool switch3status1 = LOW; 

bool read1temp=0;
bool read2temp=0;
bool read3temp=0;

int reading1=0;
int reading2=0;
int reading3=0;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
unsigned long debounceDelay1 = 50;  
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay2 = 50;  
unsigned long lastDebounceTime3 = 0;
unsigned long debounceDelay3 = 50;


unsigned long switch1millisec1=0;
unsigned long switch1millisec2=0;
unsigned long switch1millisec3=0;
unsigned long switch1millisec4=0;

unsigned long switch2millisec1=0;
unsigned long switch2millisec2=0;
unsigned long switch2millisec3=0;
unsigned long switch2millisec4=0;

unsigned long switch3millisec1=0;
unsigned long switch3millisec2=0;
unsigned long switch3millisec3=0;
unsigned long switch3millisec4=0;

/////////////////////////////////////////////////////////

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
 // Blynk.syncVirtual(V3);
  // Blynk.syncAll();
  // Alternatively, you could override server state using:
  //Blynk.virtualWrite(V2, ledState);
}


BLYNK_WRITE(V1)
{
   switch1status = param.asInt(); // assigning incoming value from pin V1 to a variable

}

BLYNK_WRITE(V2)
{
   switch2status = param.asInt(); // assigning incoming value from pin V1 to a variable

}
BLYNK_WRITE(V3)
{
   switch3status = param.asInt(); // assigning incoming value from pin V1 to a variable

}



void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, switch1status1);
   Blynk.virtualWrite(V2, switch2status1);
    Blynk.virtualWrite(V3, switch3status1);
}


void notification() {
  
}
///////////////////////////////////////////////

void setup() {
    
 pinMode(switch1, INPUT_PULLUP);
 pinMode(switch2, INPUT_PULLUP);
 pinMode(switch3, INPUT_PULLUP);
  pinMode(relay1, OUTPUT);
   pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
  
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
  switchstatusfunction();
   pushbuttonswitch();
  relay();
}
void switchstatusfunction(){
  
 
   
}
void pushbuttonswitch(){    
 
    reading1 = digitalRead(switch1);
    
 unsigned long switch1timer = millis();
  
if (reading1==0){
  switch1millisec1=switch1timer;
 switch1millisec4=switch1timer-switch1millisec3;
  
}
 else{
   
   switch1millisec2=switch1timer-switch1millisec1;
   switch1millisec3=switch1timer;
   read1temp=1;
   
  }
  switch1millisec2=switch1timer-switch1millisec1;
  switch1millisec4=switch1timer-switch1millisec3;
  
  if ( switch1millisec4> debounceDelay1&&read1temp==1) {
   
    if (switch1status==HIGH) {
      switch1status = LOW;
    }
     else {
      switch1status = HIGH;
    }
    read1temp=0;
   }
  
  

   reading2 = digitalRead(switch2);
 unsigned long switch2timer = millis();
if (reading1==0){
  switch2millisec1=switch2timer;
 switch2millisec4=switch2timer-switch2millisec3;
  
}
 else{
   
   switch2millisec2=switch2timer-switch2millisec1;
   switch2millisec3=switch2timer;
   read2temp=1;
   
  }
  switch1millisec2=switch2timer-switch2millisec1;
  switch2millisec4=switch2timer-switch2millisec3;
  
  if ( switch2millisec4> debounceDelay2&& read2temp==1) {
   
    if (switch2status==HIGH) {
      switch2status = LOW;
    }
     else {
      switch2status = HIGH;
    }
    read2temp=0;
    
   }
  
  
   reading3 = digitalRead(switch3);
unsigned long switch3timer = millis();
 
if (reading3==0){
  switch3millisec1=switch3timer;
 switch3millisec4=switch3timer-switch3millisec3;
  
}
else{
   
   switch3millisec2=switch3timer-switch3millisec1;
   switch3millisec3=switch3timer;
   read3temp=1;
  }
  switch3millisec2=switch3timer-switch3millisec1;
  switch3millisec4=switch3timer-switch3millisec3;
  
  if ( switch3millisec4> debounceDelay3&&read3temp==1) {
   
    if (switch3status==HIGH) {
      switch3status = LOW;
    }
    else {
      switch3status = HIGH;
    }
    read3temp=0;

   }
  
  
  


}
//=======================================
void relay(){
  if (switch1status==HIGH){
    digitalWrite(relay1, HIGH);
  }
  else{ 
    digitalWrite(relay1, LOW);    
  }
  if (switch2status==HIGH){
    digitalWrite(relay2, HIGH);
  }
  else{ 
    digitalWrite(relay2, LOW);    
  }
  if (switch3status==HIGH){
    digitalWrite(relay3, HIGH);
  }
  else{ 
    digitalWrite(relay3, LOW);    
  }
  switch1status1=switch1status;
  switch2status1=switch2status;
  switch3status1=switch3status;
  
}
//==========================================================

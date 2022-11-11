#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>

//=============================================================================
RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio
//=============================================================================
const uint16_t tank_node = 01;        // Address of our node in Octal format
const uint16_t master_node = 00;       // Address of the other node in Octal format
const uint16_t motor_node=02;

unsigned char outdata1[12];
unsigned char indata[3];
//================================================================================
int fan=A0;
int light=A1;
int relay3=A2;
int fanswitch=A5;
int ledswitch1=A6;
int ledswitch2=A7;
bool fanstatus=0;
bool lightstatus=0;
bool relay3status=0;
bool fanstatus1=0;
bool lightstatus1=0;
bool relay3status1=0;
bool fanstatus2=0;
bool lightstatus2=0;
bool relay3status2=0;
bool fanstatus3=0;
bool lightstatus3=0;
bool relay3status3=0;
bool fanstatus4=0;
bool lightstatus4=0;
bool relay3status4=0;
bool read1temp=0;
bool read2temp=0;
bool read3temp=0;
bool reading1=0;
int reading2=0;
int reading3=0;


int blueled=A3;
int trigpin1 = 7;
int echopin1= 8;
int trigpin2=6;
int echopin2=2;
int trigpin3=3;
int echopin3=4;
int trigpin4=5;
int echopin4=A4;
//======================================================================================

float duration1 = 0.00;
float duration2 = 0.00;
float duration3= 0.00;
float duration4 = 0.00;
float tank1 = 0.00;
float tank2= 0.00;
float tank3 = 0.00;
float tank4= 0.00;
float tankheight=170.0;

bool blueledstatus=LOW;

bool tank1full=0;
bool tank2full=0;
bool tank3full=0;
bool tank4full=0;

//============================================================================
unsigned long tank1millisec1=0;
unsigned long tank1millisec2=0;
unsigned long tank1millisec3=0;
unsigned long tank1millisec4=0;

unsigned long tank2millisec1=0;
unsigned long tank2millisec2=0;
unsigned long tank2millisec3=0;
unsigned long tank2millisec4=0;

unsigned long tank3millisec1=0;
unsigned long tank3millisec2=0;
unsigned long tank3millisec3=0;
unsigned long tank3millisec4=0;

unsigned long tank4millisec1=0;
unsigned long tank4millisec2=0;
unsigned long tank4millisec3=0;
unsigned long tank4millisec4=0;

unsigned long radiolastsent =0;
unsigned long radiosendinterval=450;
unsigned long previousmillis =0;
unsigned long interval1=1000;

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

unsigned long debounceDelay1 = 50;  
unsigned long debounceDelay2 = 50;  
unsigned long debounceDelay3 = 50;

//==================================================================================================
void setup(void)
{
  Serial.begin(9600);
  
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 25, /*node address*/ tank_node);
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_HIGH);
  bool radioconnected;
  radioconnected=radio.isChipConnected  (   ) ;
  Serial.print("radioconnected:");
  Serial.println(radioconnected);
  pinMode(trigpin1,OUTPUT); 
  pinMode(trigpin2,OUTPUT); 
  pinMode(trigpin3,OUTPUT); 
  pinMode(trigpin4,OUTPUT); 
  pinMode(echopin1,INPUT);
  pinMode(echopin2,INPUT);
  pinMode(echopin3,INPUT);
  pinMode(echopin4,INPUT);
  pinMode(fan,OUTPUT); 
  pinMode(light,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(fanswitch,INPUT_PULLUP);
 // pinMode(ledswitch1,INPUT);
 // pinMode(ledswitch2,INPUT);
}
//=======================================================================
void loop(void){
  ultrasonic();
  tank1fullstatus();
  tank2fullstatus();
  tank3fullstatus();
  tank4fullstatus();
  network.update();                  // Check the network regularly
  senddata(); 
  receivedata();
  pushbuttonswitch();
  relay();
  unsigned long currentmillis = millis();
 if (currentmillis - previousmillis >= interval1) {
     previousmillis = currentmillis;
     int pa;
     pa=radio.getPALevel();
     Serial.print("power level:");
     Serial.println(pa);
     bool radioconnected;
     radioconnected=radio.isChipConnected  (   ) ;
     Serial.print("radioconnected:");
     Serial.println(radioconnected);
  }

  }
//==========================================================================
  void ultrasonic(){
    delay(200);
     float duration1temp=0.0;
  digitalWrite(trigpin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigpin1, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigpin1, LOW);
  duration1temp = pulseIn(echopin1, HIGH);
  if ((duration1temp * 0.034 / 2)>0){
    duration1=duration1temp;    
  }
  tank1 = (tankheight-(duration1 * 0.034 / 2));
    Serial.print("tank1:");
     Serial.println(tank1);
  delay(10);
  float duration2temp=0.0;
   digitalWrite(trigpin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin2, LOW);
  duration2temp = pulseIn(echopin2, HIGH);
  if ((duration2temp * 0.034 / 2)>0){
    duration2=duration2temp;    
  }
  tank2 =(tankheight-(duration2 * 0.034 / 2));
   Serial.print("tank2:");
     Serial.println(tank2);
  delay(10);
  float duration3temp=0.0;
   digitalWrite(trigpin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin3, LOW);
  duration3temp = pulseIn(echopin3, HIGH); 
  if ((duration3temp * 0.034 / 2)>0){
    duration3=duration3temp;    
  } 
  tank3 = (tankheight-(duration3 * 0.034 / 2));
   Serial.print("tank3:");
   Serial.println(tank3);
  delay(10);
  float duration4temp=0;
  digitalWrite(trigpin4, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin4, LOW);
  duration4temp = pulseIn(echopin4, HIGH);
  if ((duration4temp * 0.034 / 2)>0){
    duration4=duration4temp;    
  }
  tank4 = (tankheight-(duration4 * 0.034 / 2));
   Serial.print("tank4:");
     Serial.println(tank4);
  
 }
 //============================================================================================
 void tank1fullstatus(){
  unsigned long tank1timer = millis();
  if(tank1>145.0){
   
   tank1millisec2=tank1timer-tank1millisec1;
   tank1millisec3=tank1timer;
   
  }
if (tank1<140.0){
  tank1millisec1=tank1timer;
 tank1millisec4=tank1timer-tank1millisec3;
  
}
  tank1millisec2=tank1timer-tank1millisec1;
  tank1millisec4=tank1timer-tank1millisec3;
  //========================================================
   if (tank1millisec2>10000){
 tank1full=1;  
 }  
   if (tank1millisec4>10000){  
   tank1full=0;    
  }
  
 }
 //================================================================================================
 void tank2fullstatus(){
  unsigned long tank2timer = millis();
  if(tank2>145.0){
   
   tank2millisec2=tank2timer-tank2millisec1;
   tank2millisec3=tank2timer;
   
  }
if (tank2<140.0){
  tank2millisec1=tank2timer;
 tank2millisec4=tank2timer-tank2millisec3;
  
}
  tank2millisec2=tank2timer-tank2millisec1;
  tank2millisec4=tank2timer-tank2millisec3;
  //========================================================
   if (tank2millisec2>10000){
 tank2full=1;  
 }  
   if (tank2millisec4>10000){  
   tank2full=0;    
  }
  
 }
 //================================================================================================
  void tank3fullstatus(){
  unsigned long tank3timer = millis();
  if(tank3>145.0){
   
   tank3millisec2=tank3timer-tank3millisec1;
   tank3millisec3=tank3timer;
   
  }
if (tank3<140.0){
  tank3millisec1=tank3timer;
 tank3millisec4=tank3timer-tank3millisec3;
  
}
  tank3millisec2=tank3timer-tank3millisec1;
  tank3millisec4=tank3timer-tank3millisec3;
  //========================================================
   if (tank3millisec2>10000){
 tank3full=1;  
 }  
   if (tank3millisec4>10000){  
   tank3full=0;    
  }
  
 }
 //===============================================================================================
 void tank4fullstatus(){
  unsigned long tank4timer = millis();
  if(tank4>145.0){
   
   tank4millisec2=tank4timer-tank4millisec1;
   tank4millisec3=tank4timer;
   
  }
if (tank4<140.0){
  tank4millisec1=tank4timer;
 tank4millisec4=tank4timer-tank4millisec3;
  
}
  tank4millisec2=tank4timer-tank4millisec1;
  tank4millisec4=tank4timer-tank4millisec3;
  //========================================================
   if (tank4millisec2>10000){
 tank4full=1;  
 }  
   if (tank4millisec4>10000){  
   tank4full=0;    
  }
  
 }
 //===============================================================================================  
 void senddata(){
     
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - radiolastsent >= radiosendinterval  )
  {
    radiolastsent = now;
     outdata1[0]=blueledstatus;
     outdata1[1]=tank1;
     outdata1[2]=tank2;
     outdata1[3]=tank3;
     outdata1[4]=tank4;
     outdata1[5]=tank1full;
     outdata1[6]=tank2full;
     outdata1[7]=tank3full;
     outdata1[8]=tank4full;
     outdata1[9]=fanstatus4;
     outdata1[10]=lightstatus4;
     outdata1[11]=relay3status4;
      

    Serial.print("Sending...");

    RF24NetworkHeader header(/*to node*/ master_node);
    bool ok = network.write(header,&outdata1,sizeof(outdata1));
    Serial.print("outdata1size:");
    Serial.println(sizeof(outdata1));
     if (ok){
      Serial.println("ok.");
      if (blueledstatus==LOW){
        blueledstatus=HIGH;
      }
      else{
        blueledstatus=LOW;
     }
   }
    else{
      Serial.println("failed.");
      blueledstatus=LOW;
    }
    digitalWrite(blueled, blueledstatus);
   }
  }
 
  //==================================================================================
 void receivedata(){

    while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    
    
    network.read(header,&indata,sizeof(indata));
    if (header.from_node == 00){
       fanstatus=indata[0];
       lightstatus=indata[1];
       relay3status=indata[2];       
        
    }
  }
}
//====================================================================================
void pushbuttonswitch(){    
 
    reading1 = digitalRead(fanswitch);
    
 unsigned long switch1timer = millis();
  
if (reading1==LOW){
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
  
  if ( switch1millisec4>debounceDelay1 && read1temp==1) {
   
    if (fanstatus1==HIGH) {
      fanstatus1 = LOW;
    }
     else {
      fanstatus1 = HIGH;
    }
    read1temp=0;
   }
  

   reading2 = analogRead(ledswitch1) <512 ? 0: 1;
 unsigned long switch2timer = millis();
if (reading2==0){
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
  
  if ( switch2millisec4>debounceDelay2 && read2temp==1) {
   
    if (lightstatus1==HIGH) {
      lightstatus1 = LOW;
    }
     else {
      lightstatus1 = HIGH;
    }
    read2temp=0;
    
   }
  
  
   reading3 = analogRead(ledswitch2)<512 ? 0 : 1;
   Serial.print("reading3:");
   Serial.println(reading3);
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
  
  if ( switch3millisec4>debounceDelay3&&read3temp==1) {
   
    if (relay3status1==HIGH) {
      relay3status1 = LOW;
    }
    else {
      relay3status1 = HIGH;
    }
    read3temp=0;

   }
 }
//========================================================================
void relay(){
 
  if(fanstatus2 != fanstatus1 || fanstatus3 != fanstatus){
    if(fanstatus3!=fanstatus){
      fanstatus4=fanstatus;
    }
    else if(fanstatus2!=fanstatus1){
      fanstatus4=fanstatus1;
    }
  }
  fanstatus2 = fanstatus1;
  fanstatus3 = fanstatus;
  
   if(lightstatus2!=lightstatus1 || lightstatus3!=lightstatus){
    if(lightstatus3!=lightstatus){
      lightstatus4=lightstatus;
    }
    else if(lightstatus2!=lightstatus1){
      lightstatus4=lightstatus1;
    }
  }
  lightstatus2 =lightstatus1;
  lightstatus3 = lightstatus;
  
   if(relay3status2!=relay3status1 || relay3status3!=relay3status){
    if(relay3status3!=relay3status){
      relay3status4=relay3status;
    }
    else if(relay3status2!=relay3status1){
      relay3status4=relay3status1;
    }
  }
  relay3status2 =relay3status1;
  relay3status3 = relay3status;
  
  digitalWrite(fan, fanstatus4);
  digitalWrite(light, lightstatus4);
  digitalWrite(relay3, relay3status4);

}

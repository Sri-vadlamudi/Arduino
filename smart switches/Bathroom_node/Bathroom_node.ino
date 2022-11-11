#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>

//=============================================================================
RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio
//=============================================================================
const uint16_t bathroom_node = 013;        // Address of our node in Octal format
const uint16_t master_node = 03;       // Address of the other node in Octal format

unsigned char outdata1[4];
unsigned char indata[3];
//================================================================================
int fan=3;
int light=4;
int trimmer=5;
int fanswitch=6;
int lightswitch=7;
int trimmerswitch=8;
bool fanstatus=0;
bool lightstatus=0;
bool trimmerstatus=0;
bool fanstatus1=0;
bool lightstatus1=0;
bool trimmerstatus1=0;
bool fanstatus2=0;
bool lightstatus2=0;
bool trimmerstatus2=0;
bool fanstatus3=0;
bool lightstatus3=0;
bool trimmerstatus3=0;
bool fanstatus4=0;
bool lightstatus4=0;
bool trimmerstatus4=0;
bool read1temp=0;
bool read2temp=0;
bool read3temp=0;
bool reading1=0;
int reading2=0;
int reading3=0;

bool blueledstatus=0;
//============================================================================

unsigned long radiolastsent =0;
unsigned long radiosendinterval=1050;
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

unsigned long debounceDelay1 = 150;  
unsigned long debounceDelay2 = 150;  
unsigned long debounceDelay3 = 150;

//==================================================================================================
void setup(void)
{
  Serial.begin(9600);
  
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 65, /*node address*/ bathroom_node);
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_LOW);
  bool radioconnected;
  radioconnected=radio.isChipConnected  (   ) ;
  Serial.print("radioconnected:");
  Serial.println(radioconnected);

  pinMode(fan,OUTPUT); 
  pinMode(light,OUTPUT);
  pinMode(trimmer,OUTPUT);
  pinMode(fanswitch,INPUT_PULLUP);
  pinMode(lightswitch,INPUT_PULLUP);
  pinMode(trimmerswitch,INPUT_PULLUP);
 // pinMode(ledswitch1,INPUT);
 // pinMode(ledswitch2,INPUT);
}
//=======================================================================
void loop(void){

  network.update();                  // Check the network regularly
  senddata(); 
  receivedata();
  pushbuttonswitch();
  relay();
  radiofunc();

  }
  
 //===============================================================================================  
 void senddata(){
     
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - radiolastsent >= radiosendinterval  )
  {
    radiolastsent = now;
     outdata1[0]=blueledstatus;
     outdata1[1]=fanstatus4;
     outdata1[2]=lightstatus4;
     outdata1[3]=trimmerstatus4;
      

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
    //digitalWrite(blueled, blueledstatus);
   }
  }
 
  //==================================================================================
 void receivedata(){

    while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    
    
    network.read(header,&indata,sizeof(indata));
    if (header.from_node == 03){
       fanstatus=indata[0];
       lightstatus=indata[1];
       trimmerstatus=indata[2];       
        
    }
    Serial.print("fanstatus:");
    Serial.println(fanstatus);
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
  

   reading2 = digitalRead(lightswitch);
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
  
  
   reading3 = digitalRead(trimmerswitch);
//   Serial.print("reading3:");
//   Serial.println(reading3);
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
   
    if (trimmerstatus1==HIGH) {
      trimmerstatus1 = LOW;
    }
    else {
      trimmerstatus1 = HIGH;
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
  
   if(trimmerstatus2!=trimmerstatus1 || trimmerstatus3!=trimmerstatus){
    if(trimmerstatus3!=trimmerstatus){
      trimmerstatus4=trimmerstatus;
    }
    else if(trimmerstatus2!=trimmerstatus1){
      trimmerstatus4=trimmerstatus1;
    }
  }
  trimmerstatus2 =trimmerstatus1;
  trimmerstatus3 = trimmerstatus;
  
  digitalWrite(fan, fanstatus4);
  digitalWrite(light, lightstatus4);
  digitalWrite(trimmer, trimmerstatus4);

}
//=============================================================================
void radiofunc(){
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
//==============================================================

char server[]="192.168.31.207";
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
/////////////////////////////////////////////////
#include <Wire.h> 

//////////////////////////////////////////////////////////////////////////
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
/////////////////////////////////////////////////
RF24 radio(2,15);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio
//========================================================
const uint16_t tank_node = 01;        // Address of our node in Octal format
const uint16_t master_node = 00;       // Address of the other node in Octal format
const uint16_t motor_node=02;

///////////////////////////////////////////////////
char auth[] = "zTzZP3TSDJCq6eYckdW5ypm-mPiEjQd9";
char ssid[] = "VADLAMUDI";
char pass[] = "9849692804";

///////////////////////////////////////////////////
uint8_t buzzer = D1; 
//unsigned char indata1[12];
//char indata2[2];
char outdata1[3];
unsigned char outdata2[1];

//tank node variables
bool relay3=0;
bool fan=0;
bool light=0;
bool fanstatus=0;
bool lightstatus=0;
bool relay3status=0;

//================
 bool f=0;
 bool i=0;
 bool j=0;
 bool k=0;
 bool l=0;
 bool m=0;
 bool n=0;
 bool o=0;
 bool p=0;
 bool q=0;
 bool r=0;
 bool s=0;
 bool t=0;

 //=====================================      
unsigned long tanknodemillisec1=0;
unsigned long tanknodemillisec2=0;
unsigned long tanknodemillisec3=0;
unsigned long tanknodemillisec4=0;

unsigned long motornodemillisec1=0;
unsigned long motornodemillisec2=0;
unsigned long motornodemillisec3=0;
unsigned long motornodemillisec4=0;

unsigned long previousMillis = 0;   
unsigned long previousMillis1 = 0;   
const long interval1 = 1100;
const unsigned long interval0 = 1000; //ms  // How often to send 'hello world to the other unit
const unsigned long interval = 1000; //ms  // How often to send 'hello world to the other unit
unsigned long last_sent; 
unsigned long last_sent1;     

//////////////////////////////////////////////////

char motorstatus=0;
int valvestatus=0;
char tank1=0;
char tank2=0;
char tank3=0;
char tank4=0;
char waterflow=0;
bool tank1full=0;
bool tank2full=0;
bool tank3full=0;
bool tank4full=0;
bool tanknode=0;
bool motornode=0;
bool tanknodestatus=0;
bool motornodestatus=0;
bool notificationstatus=1;

char tapposval=0;

/////////////////////////////////////////////
BlynkTimer timer;
//=============================================================================================================
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(V13);
//  Blynk.syncVirtual(V8);
//  Blynk.syncAll();
//  Blynk.syncVirtual(V5);
//   Blynk.syncVirtual(V4);
  
  }
//========================================================================================================
void myTimerEvent()
{


  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, tanknode);
  Blynk.virtualWrite(V1, tank1);
  Blynk.virtualWrite(V2, tank2);
  Blynk.virtualWrite(V3, tank3);
  Blynk.virtualWrite(V4, tank4);
  Blynk.virtualWrite(V33, fanstatus);
  Blynk.virtualWrite(V34, lightstatus);
  Blynk.virtualWrite(V35, relay3status);
  Blynk.virtualWrite(V36,notificationstatus);
//  Blynk.virtualWrite(V9, indata2[3]);
//  Blynk.virtualWrite(V10, indata2[4]);

    Blynk.virtualWrite(V5, waterflow*12);
    Blynk.virtualWrite(V6, motornode);
    Blynk.virtualWrite(V7, tapposval);
    Blynk.virtualWrite(V8, motorstatus);

}
//==========================================================================================================
BLYNK_WRITE(V11)
{
   valvestatus = param.asInt();
}

BLYNK_WRITE(V13)
{
   notificationstatus = param.asInt(); 
}
BLYNK_WRITE(V30)
{
   fan = param.asInt(); 

 
}
BLYNK_WRITE(V31)
{
  light = param.asInt(); 
}

BLYNK_WRITE(V32)
{
   relay3 = param.asInt(); 
}
//==============================================================================================================
void notification(){
  if(notificationstatus==1){
  if (f==1){
    if(i<1){
       Blynk.notify("Waterflow detected");
       i=1;
    }
  }
  if (f==0){
    i=0;
  }
 if (tank1full==1){
    if(j<1){
       Blynk.notify("Nagamani tank is full");
       j=1;
    }
    
  }
  if (tank1full==0){
    j=0;
  }



  if (tank2full==1){
    if(k<1){
       Blynk.notify("Pravalika tank is full");
       k=1;
    }
    
  }
  if (tank2full==0){
    k=0;
  }

  if (tank3full==1){
    if(l<1){
       Blynk.notify("Freshwater tank is full");
       l=1;
    }
   
  }
  if (tank3full==0){
    l=0;
  }

  if (tank4full==1){
    if(m<1){
       Blynk.notify("Groundwater tank is full");
       m=1;
    }
   
  }
  if (tank4full==0){
    m=0;
  }

if (tanknodestatus==1){
    if(n<1){
       Blynk.notify("Tank node is 'OFFLINE' ");
       n=1;
    }
   
  }
  if (tanknodestatus==0){
    n=0;
  }

    
if (motornodestatus==1){
    if(o<1){
       Blynk.notify("MOTOR node is 'OFFLINE' ");
       o=1;
    }
   
  }
  if (motornodestatus==0){
    o=0;
  }

  if (fanstatus==1){
    if(p<1){
       Blynk.notify("Terrace fan is 'ON' ");
       p=1;
    }
   
  }
  if (fanstatus==0){
    if(p==1){
    Blynk.notify("Terrace fan is 'OFF' ");
    p=0;
  }
  }
  if (lightstatus==1){
    if(q<1){
       Blynk.notify("Terrace light1 is 'ON' ");
       q=1;
    }
   
  }
   if (lightstatus==0){
    if(q==1){
    Blynk.notify("Terrace light is 'OFF' ");
    q=0;
  }
  }
  if (relay3status==1){
    if(r<1){
    Blynk.notify("Terrace light2 is 'ON' ");
    r=1;
    }
  }
  if (relay3status==0){
    if(r==1){
       Blynk.notify("Terrace light2 is 'OFF' ");
       r=0;
    }
   
  }
  if (motorstatus==7){
    if(s==1){
    Blynk.notify("Nagamani groundwater motor is 'ON' ");
    s=0;
    }
  }
   if (motorstatus!=7){
        s=0;
     }
  }
  if (motorstatus==8){
    if(t==1){
    Blynk.notify("pravalika groundwater motor is 'ON' ");
    t=0;
    }
  }
   if (motorstatus!=8){
        t=0;     
   }
  
 }
//=======================================================================================================
void setup(void)
{
  Serial.begin(115200); 
  pinMode(buzzer, OUTPUT);
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 25, /*node address*/ master_node);
   radio.setDataRate( RF24_250KBPS );
   radio.setPALevel(RF24_PA_HIGH);
 
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, ssid, pass, server, 8080);//for local server
  timer.setInterval(1500L, myTimerEvent);


}
//=====================================================================================================
void loop(void){
  
  network.update();                  
  senddata();
  receivedata();
 radiostatus();
 Blynk.run();
 timer.run(); 
 notification();
 tanknodestatusfunc();
 motornodestatusfunc();
 buzzerfunc();
 
  }
//=======================================================================================
void receivedata(){
unsigned char indata[12];
    while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    
    
    network.read(header,&indata,sizeof(indata));
    if (header.from_node == 01){
       tanknode=indata[0];
       tank1=indata[1];
       tank2=indata[2];
       tank3=indata[3];
       tank4=indata[4];
       tank1full=indata[5];
       tank2full=indata[6];
       tank3full=indata[7];
       tank4full=indata[8];
       fanstatus=indata[9];
       lightstatus=indata[10];
       relay3status=indata[11];    
//       Serial.print("RELAY3:");
//       Serial.println(relay3status);   
        
    }
     if (header.from_node == 02){
       motornode=indata[0];
       waterflow=indata[1];
       f=indata[2];
       motorstatus=indata[3];
       tapposval=indata[4];
      // indata2[2]=indata[2];
      // indata2[3]=indata[3];
      // indata2[4]=indata[4];
      // indata2[5]=indata[5];
      // f=indata2[1];
     }

  }
}
//==========================================================================================
void senddata(){
   
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;
    outdata1[0]=fan;
    outdata1[1]=light;
    outdata1[2]=relay3;
//    Serial.print("outdatasize:");
//    Serial.println(sizeof(outdata));
    
    RF24NetworkHeader header(/*to node*/ tank_node);
    bool ok = network.write(header,&outdata1,sizeof(outdata1));
    if (ok)
      Serial.println("ok1.");
    else
      Serial.println("failed1.");
  }

  unsigned long now1 = millis();              // If it's time to send a message, send it!
  if ( now1 - last_sent1 >= interval1  )
  {
    last_sent1 = now1;
    outdata2[0]=char(valvestatus);
    
//    Serial.print("outdatasize:");
//    Serial.println(sizeof(outdata));
    
    RF24NetworkHeader header(/*to node*/ motor_node);
    bool ok = network.write(header,&outdata2,sizeof(outdata2));
    if (ok){
      Serial.println("ok2.");
       Serial.print("outdata2size:");
       Serial.println(sizeof(outdata2));
      Serial.print("valvestatus:");
     Serial.println(outdata2[0]);
    }
    else
      Serial.println("failed2.");
  }
  }
//===================================================================================================
void tanknodestatusfunc(){
  unsigned long tanknodetimer = millis();
  if(tanknode==1){   
   tanknodemillisec2=tanknodetimer-tanknodemillisec1;
   tanknodemillisec3=tanknodetimer;
   
  }
if (tanknode==0){
  tanknodemillisec1=tanknodetimer;
  tanknodemillisec4=tanknodetimer-tanknodemillisec3;
  
}
  tanknodemillisec2=tanknodetimer-tanknodemillisec1;
  tanknodemillisec4=tanknodetimer-tanknodemillisec3;
  //========================================================
   if (tanknodemillisec2>45000 ||tanknodemillisec4>45000 ){
 tanknodestatus=1;  
 }  
 else{
   tanknodestatus=0; 
 }
    
}
//=========================================================================================
void motornodestatusfunc(){
  unsigned long motornodetimer = millis();
  if(motornode==1){   
   motornodemillisec2=motornodetimer-motornodemillisec1;
   motornodemillisec3=motornodetimer;
   
  }
if (motornode==0){
  motornodemillisec1=motornodetimer;
  motornodemillisec4=motornodetimer-motornodemillisec3;
  
}
  motornodemillisec2=motornodetimer-motornodemillisec1;
  motornodemillisec4=motornodetimer-motornodemillisec3;
  //========================================================
   if (motornodemillisec2>35000 ||motornodemillisec4>35000 ){
 motornodestatus=1;  
 }  
 else{
   motornodestatus=0; 
 }
    
}
//=================================================================================
void buzzerfunc(){
  if(f==1){
    if(motorstatus==0){
      digitalWrite(buzzer,HIGH);
    }
    else{
      digitalWrite(buzzer,LOW);
    }    
  }
  else{
    digitalWrite(buzzer,LOW);    
  }  
}
//==========================================================================================
void radiostatus(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval0) {
     previousMillis = currentMillis;
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

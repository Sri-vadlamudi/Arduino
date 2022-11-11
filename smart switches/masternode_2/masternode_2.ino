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
const uint16_t bathroom_node = 013;        // Address of our node in Octal format
const uint16_t master_node = 03;       // Address of the other node in Octal format
//const uint16_t motor_node=02;

///////////////////////////////////////////////////
char auth[] = "HHWRg7GaDQ_Ycu0hKKjnSG1YfQCQMqp_";
char ssid[] = "VADLAMUDI";
char pass[] = "9849692804";

///////////////////////////////////////////////////
//uint8_t buzzer = D1; 
//unsigned char indata1[12];
//char indata2[2];
unsigned char outdata1[3];
//unsigned char outdata2[1];

//bathroom node variables
bool trimmer=0;
bool fan=0;
bool light=0;
bool fanstatus=0;
bool lightstatus=0;
bool trimmerstatus=0;

//================
 bool f=0;
 bool i=0;
 bool j=0;
 bool k=0;
 //=====================================      
unsigned long bathroomnodemillisec1=0;
unsigned long bathroomnodemillisec2=0;
unsigned long bathroomnodemillisec3=0;
unsigned long bathroomnodemillisec4=0;


unsigned long previousMillis = 0;//radio status function millisec      
const unsigned long interval0 = 1000; //radio status function millisec  
const unsigned long interval = 1000; //send data interval
unsigned long last_sent;  //send data variable  

//////////////////////////////////////////////////
bool bathroomnode=0;
bool bathroomnodestatus=0;
bool notificationstatus=1;

/////////////////////////////////////////////
BlynkTimer timer;
//=============================================================================================================
BLYNK_CONNECTED() {
  // Request the latest state from the server
 // Blynk.syncVirtual(V13);
//  Blynk.syncAll();  
  }
//========================================================================================================
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, bathroomnode);
  Blynk.virtualWrite(V1, fanstatus);
  Blynk.virtualWrite(V2, lightstatus);
  Blynk.virtualWrite(V3, trimmerstatus);
  Blynk.virtualWrite(V4, notificationstatus);
}
//==========================================================================================================


BLYNK_WRITE(V11)
{
   notificationstatus = param.asInt(); 
}
BLYNK_WRITE(V12)
{
   fan = param.asInt(); 
 
}
BLYNK_WRITE(V13)
{
  light = param.asInt(); 
}

BLYNK_WRITE(V14)
{
   trimmer = param.asInt(); 
}
//==============================================================================================================
void notification(){
  if(notificationstatus==1){ 
  
if (bathroomnodestatus==1){
    if(f<1){
       Blynk.notify("Bathroom node is 'OFFLINE' ");
       f=1;
    }
   
  }
  if (bathroomnodestatus==0){
    f=0;
  }

  if (fanstatus==1){
    if(i<1){
       Blynk.notify("Fan switch is 'ON' ");
       i=1;
    }
   
  }
  if (fanstatus==0){
    i=0;
  }

   if (lightstatus==1){
    if(j<1){
       Blynk.notify("Tubelight switch is 'ON' ");
       j=1;
    }
   
  }
  if (lightstatus==0){
    j=0;
  }

   if (trimmerstatus==1){
    if(k<1){
       Blynk.notify("Trimmer switch is 'ON' ");
       k=1;
    }
   
  }
  if (trimmerstatus==0){
    k=0;
  }

  }
 }
//=======================================================================================================
void setup(void)
{
  Serial.begin(115200); 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 65, /*node address*/ master_node);
   radio.setDataRate( RF24_250KBPS );
   radio.setPALevel(RF24_PA_LOW);
 
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
  timer.setInterval(1000L, myTimerEvent);


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
 bathroomnodestatusfunc();
 
  }
//=======================================================================================
void receivedata(){
unsigned char indata[4];
    while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    
    
    network.read(header,&indata,sizeof(indata));
    if (header.from_node == 013){
       bathroomnode=indata[0];       
       fanstatus=indata[1];
       lightstatus=indata[2];
       trimmerstatus=indata[3];    
    } 
//    Serial.println("bathroom node:");
//    Serial.println(bathroomnode);
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
    outdata1[2]=trimmer;
        
    RF24NetworkHeader header(/*to node*/ bathroom_node);
    bool ok = network.write(header,&outdata1,sizeof(outdata1));
    if (ok)
      Serial.println("ok1.");
    else
      Serial.println("failed1.");
  }

  }
//===================================================================================================
void bathroomnodestatusfunc(){
  unsigned long bathroomnodetimer = millis();
  if(bathroomnode==1){   
   bathroomnodemillisec2=bathroomnodetimer-bathroomnodemillisec1;
   bathroomnodemillisec3=bathroomnodetimer;
   
  }
if (bathroomnode==0){
  bathroomnodemillisec1=bathroomnodetimer;
  bathroomnodemillisec4=bathroomnodetimer-bathroomnodemillisec3;
  
}
  bathroomnodemillisec2=bathroomnodetimer-bathroomnodemillisec1;
  bathroomnodemillisec4=bathroomnodetimer-bathroomnodemillisec3;
  //========================================================
   if (bathroomnodemillisec2>45000 ||bathroomnodemillisec4>45000 ){
 bathroomnodestatus=1;  
 }  
 else{
   bathroomnodestatus=0; 
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

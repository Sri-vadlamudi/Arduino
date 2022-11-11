//////////////////////////////////////////////////////////////////////////
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
/////////////////////////////////////////////////
RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio
/////////////////////////////////////////////////

//Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//========================================================
const uint16_t tank_node = 01;        // Address of our node in Octal format
const uint16_t master_node = 00;       // Address of the other node in Octal format
const uint16_t motor_node=02;

///////////////////////////////////////////////////
uint8_t buzzer = A0; 
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



//=======================================================================================================
void setup(void)
{
  Serial.begin(9600); 
  pinMode(buzzer, OUTPUT);
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 25, /*node address*/ master_node);
   radio.setDataRate( RF24_250KBPS );
   radio.setPALevel(RF24_PA_LOW);
 
//  Serial.println();
//  // initialize OLED display with address 0x3C for 128x64
//  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//    Serial.println(F("SSD1306 allocation failed"));
//    while (true);
//  }
//
//  delay(2000);         // wait for initializing
//  oled.clearDisplay(); // clear display
//
//  oled.setTextSize(1);          // text size
//  oled.setTextColor(WHITE);     // text color
//  oled.setCursor(0, 10);        // position to display
//  oled.println("Hello World!"); // text to display
//  oled.display();            
  


}
//=====================================================================================================
void loop(void){
                
  senddata();
  receivedata();
 radiostatus();

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
       Serial.print("RELAY3:");
       Serial.println(relay3status);   
        
    }
     if (header.from_node == 02){
       motornode=indata[0];
       waterflow=indata[1];
       f=indata[2];
       motorstatus=indata[3];
       tapposval=indata[4];
      // indata2[2]=indata[2];
//       indata2[3]=indata[3];
//       indata2[4]=indata[4];
//       indata2[5]=indata[5];
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
//========================================================================================

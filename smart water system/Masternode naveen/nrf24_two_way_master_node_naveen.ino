 
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
/////////////////////////////////////////////////
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
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

const unsigned long interval = 1000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?

///////////////////////////////////////////////////
char auth[] = "WyS1_JfO39ev2KRDbi8DFses26bo5W2V";
char ssid[] = "JVR MURTHY";
char pass[] = "jajula@1234";

///////////////////////////////////////////////////
float indata1[6]={0.0,0.0,0.0,0.0,0.0,0.0};
float indata2[6]={0.0,0.0,0.0,0.0,0.0,0.0};

bool f=0;
 bool i=0;
 bool j=0;
 bool k=0;
 bool l=0;
 bool m=0;
 bool n=0;
 bool q=0;
 bool r=0;
 bool t1=0;
 bool t2=0;
 

 int servopos=0;

 bool tankmodulestatus=0;
 bool tankmoduleonlinestatus=0;
 bool motormodulestatus=0;
 bool motormoduleonlinestatus=0;
 bool notificationstatus=1;
 float tank1=0.00;
 float tank2=0.00;

unsigned long tankmodulemillisec1=0;
unsigned long tankmodulemillisec2=0;
unsigned long tankmodulemillisec3=0;
unsigned long tankmodulemillisec4=0;

unsigned long motormodulemillisec1=0;
unsigned long motormodulemillisec2=0;
unsigned long motormodulemillisec3=0;
unsigned long motormodulemillisec4=0;
       
unsigned long millisec1=0;
unsigned long millisec2=0;
unsigned long millisec3=0;
unsigned long millisec4=0;

unsigned long previousMillis = 0;        // will store last time LED was updated


// constants won't change:
const long interval1 = 1000;   

//////////////////////////////////////////////////
int outdata1;
int motorstatus=0;
int mode1=0;
int lcdlight=0;
/////////////////////////////////////////////
BlynkTimer timer;
//=============================================================================================================
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(V10);
  Blynk.syncVirtual(V8);
  Blynk.syncAll();
  Blynk.syncVirtual(V5);
   Blynk.syncVirtual(V4);
    Blynk.syncVirtual(V22);
    Blynk.syncVirtual(V23);
  // Alternatively, you couBlynk.syncVirtual(V5);ld override server state using:
  //Blynk.virtualWrite(V2, ledState);
}
//========================================================================================================
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, indata1[0]);
  Blynk.virtualWrite(V1, indata1[1]);
  Blynk.virtualWrite(V2, indata1[2]);
  //Blynk.virtualWrite(V3, indata1[3]);
 // Blynk.virtualWrite(V4, indata1[4]);
 // Blynk.virtualWrite(V5, indata1[5]);
  Blynk.virtualWrite(V6, indata2[0]);
  Blynk.virtualWrite(V7, indata2[1]);
  Blynk.virtualWrite(V8, indata2[2]);
  Blynk.virtualWrite(V9, motormodulestatus);
  Blynk.virtualWrite(V23, notificationstatus);
}
//==========================================================================================================
//BLYNK_WRITE(V11)
//{
//   servopos = param.asInt(); // assigning incoming value from pin V1 to a variable
//
//  // process received value
//}
BLYNK_WRITE(V10)
{
   motorstatus = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}

BLYNK_WRITE(V5)
{
   mode1 = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}
BLYNK_WRITE(V4)
{
   lcdlight = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
} 
BLYNK_WRITE(V22)
{
   notificationstatus = param.asInt(); 
}
//==============================================================================================================
void notification(){
    if (notificationstatus==1){
  if (f==1){
    if(i==0){
       Blynk.notify("Waterflow detected");
       i=1;
    }
  }
  if (f==0){
    i=0;
  }
 if (t1==1){
    if(j==1){
       Blynk.notify("tank1 is full");
       j=1;
    }
    //threshold1=147;
  }
  if (t1==0){
    j=0;
  }



  if (t2==1){
    if(k==0){
       Blynk.notify("tank2 is full");
       k=1;
    }
    //threshold2=110;
  }
  if (t2==0){
    k=0;
  }

   if (tankmoduleonlinestatus==0){
    if(q==0){
       Blynk.notify("Tank module OFFLINE");
       q=1;
    }
   
  }
  if (tankmoduleonlinestatus==1){
    q=0;
  }
   if (motormoduleonlinestatus==0){
    if(r==0){
       Blynk.notify("Motor module OFFLINE");
       r=1;
    }
   
  }
  if (motormoduleonlinestatus==1){
    r=0;
  }

    
}

  }
//=======================================================================================================
void setup(void)
{
  Serial.begin(115200); 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 25, /*node address*/ master_node);
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

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);

lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
}
//=====================================================================================================
void loop(void){
  
  network.update();                  // Check the network regularly
  senddata();
receivedata();
unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval1) {
     previousMillis = currentMillis;
     int pa;
     pa=radio.getPALevel();
     Serial.print("power level:");
     Serial.println(pa);
  }

 Blynk.run();
 timer.run(); 
 notification();
tankmodulefunc();
motormodulefunc();
 lcddisplay();
  }
//=======================================================================================
void receivedata(){
 float indata[6]={0.0,0.0,0.0,0.0,0.0,0.0};
    while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    
    
    network.read(header,&indata,sizeof(indata));
    if (header.from_node == 01){
      indata1[0]=indata[0];//water level in feet.
       indata1[1]=indata[1];
        indata1[2]=indata[2];
        indata1[3]=indata[3];
        indata1[4]=indata[4];

        tank1=indata1[0];
     tank2=indata1[1];
    t1=indata1[3];
     t2=indata1[4];
      tankmodulestatus=indata1[2];
        
    }
     if (header.from_node == 02){
       indata2[0]=indata[0];
       indata2[1]=indata[1];
        indata2[2]=indata[2];
         indata2[3]=indata[3];
          indata2[4]=indata[4];
           indata2[5]=indata[5];
           f=indata2[1];
            motormodulestatus=indata2[3];
     }

    
    Serial.print("tank1:");
    Serial.println(indata1[0]);
    Serial.print("tank2:");
    Serial.println(indata1[1]);
    Serial.print("led status:");
    Serial.println(indata1[2]);
   Serial.print("f:");
   Serial.println(indata2[1]);


  }
}
//==========================================================================================
void senddata(){
   float outdata[3];
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;
    outdata[0]=motorstatus;
    outdata[1]=t2;//t2;changed from t2 to t1 due to damage of t2 ultrasonic sensor
    outdata[2]=mode1;
//    outdata[3]=servopos;
    
    Serial.print("Sending...");
    Serial.print("motorstatus:");
    Serial.println(outdata[0]);
    Serial.print("t2:");
    Serial.println(outdata[1]);
    Serial.print("mode1:");
    Serial.println(outdata[2]);
    
    RF24NetworkHeader header(/*to node*/ motor_node);
    bool ok = network.write(header,&outdata,sizeof(outdata));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
  }
//====================================================================================================================

void lcddisplay(){
  if(lcdlight==1){
    lcd.backlight();
  }
  else{
 lcd.noBacklight();
  }
  //lcd.clear();
lcd.setCursor(0,0);
lcd.print("T1:");
lcd.setCursor(3,0);
if (indata1[0]<30){
lcd.print("LOW");
}
else if (indata1[0]>150){
lcd.print("FULL");
}
else if(indata1[1]>32 &&indata1[1]<149){
 lcd.print("MED"); 
}
lcd.setCursor(8,0);
lcd.print("T2:");
lcd.setCursor(11,0);
if (indata1[1]<30){
lcd.print("LOW");
}
else if (indata1[1]>110){
lcd.print("FULL");
}
else if(indata1[1]>32 &&indata1[1]<109){
 lcd.print("MED"); 
}
lcd.setCursor(0,1);
lcd.print("MT:");
if (indata2[2]==1){
lcd.setCursor(3,1);
lcd.print("ON!");
}
if (indata2[2]==0){
lcd.setCursor(3,1);
lcd.print("OFF");
}
lcd.setCursor(8,1);
lcd.print("WF:");
lcd.setCursor(11,1);
if ((indata2[0])>1){
lcd.print(int(indata2[0]));
}
else{
lcd.print("0");
}
}
//===========================================================================
void tankmodulefunc(){
  
unsigned long tankmoduletimer = millis();  

  if(tankmodulestatus==0){
   
   tankmodulemillisec2= tankmoduletimer- tankmodulemillisec1;
    tankmodulemillisec3=tankmoduletimer;
  
  }
if (tankmodulestatus==1){
   tankmodulemillisec1= tankmoduletimer;
  tankmodulemillisec4= tankmoduletimer- tankmodulemillisec3;
  
}
    tankmodulemillisec2= tankmoduletimer-tankmodulemillisec1;
   tankmodulemillisec4= tankmoduletimer-tankmodulemillisec3;
  //===========
   if (tankmodulemillisec2>45000||tankmodulemillisec4>45000){

tankmoduleonlinestatus=0;

  
 }  
  else{
    tankmoduleonlinestatus=1;
  }
  
  
}
//==================================================================================
void motormodulefunc(){
  
unsigned long motormoduletimer = millis();  

  if(motormodulestatus==0){
   
   motormodulemillisec2= motormoduletimer- motormodulemillisec1;
    motormodulemillisec3=motormoduletimer;
  
  }
if (motormodulestatus==1){
   motormodulemillisec1= motormoduletimer;
  motormodulemillisec4= motormoduletimer- motormodulemillisec3;
  
}
    motormodulemillisec2= motormoduletimer-motormodulemillisec1;
   motormodulemillisec4= motormoduletimer-motormodulemillisec3;
  //===========
   if (motormodulemillisec2>45000||motormodulemillisec4>45000){

motormoduleonlinestatus=0;

  
 }  
  else{
    motormoduleonlinestatus=1;
  }
  
  
}

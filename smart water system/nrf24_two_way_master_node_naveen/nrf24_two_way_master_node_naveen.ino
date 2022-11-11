 
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
float indata1[3];
float indata2[6];

int f=0;
 int i=0;
 int j=0;
 int k=0;
 int l=0;
 int m=0;
 int n=0;
 int t1=0;
 int t2=0;
 int threshold1=152;
 int threshold2=115;
       
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
float tank2=0.00;
int lcdlight=0;
/////////////////////////////////////////////
BlynkTimer timer;
//=============================================================================================================
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(V10);
  Blynk.syncVirtual(V8);
 // Blynk.syncAll();
  Blynk.syncVirtual(V5);
   Blynk.syncVirtual(V4);
  // Alternatively, you could override server state using:
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
  Blynk.virtualWrite(V3, indata1[3]);
 // Blynk.virtualWrite(V4, indata1[4]);
 // Blynk.virtualWrite(V5, indata1[5]);
  Blynk.virtualWrite(V6, indata2[0]);
  Blynk.virtualWrite(V7, indata2[1]);
  Blynk.virtualWrite(V8, indata2[2]);
  Blynk.virtualWrite(V9, indata2[3]);
 // Blynk.virtualWrite(V10, indata2[4]);
}
//==========================================================================================================
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
//==============================================================================================================
void notification(){
  if (f==1){
    while(i<=1){
       Blynk.notify("Waterflow detected");
       i++;
    }
  }
  if (f==0){
    i=0;
  }
 if (t1==1){
    while(j<=1){
       Blynk.notify("tank1 is full");
       j++;
    }
    threshold1=147;
  }
  if (t1==0){
    j=0;
  }



//  if (t2==1){
//    while(k<=1){
//       Blynk.notify("tank2 is full");
//       k++;
//    }
//    threshold2=110;
//  }
//  if (t2==0){
//    k=0;
//  }

    if (indata2[4]>3.5){
    while(n<=1){
       Blynk.notify("water level detected");
       n++;
    }
  }
  if (indata2[4]<3.5){
    n=0;
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
tanklevel();
lcddisplay();
  }
//=======================================================================================
void receivedata(){
 float indata[6];
    while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    
    
    network.read(header,&indata,sizeof(indata));
    if (header.from_node == 01){
      indata1[0]=indata[0];//water level in feet.
       indata1[1]=indata[1];
        indata1[2]=indata[2];
        
    }
     if (header.from_node == 02){
       indata2[0]=indata[0];
       indata2[1]=indata[1];
        indata2[2]=indata[2];
         indata2[3]=indata[3];
          indata2[4]=indata[4];
           indata2[5]=indata[5];
           f=indata2[1];
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
    outdata[1]=t1;//t2;changed from t2 to t1 due to damage of t2 ultrasonic sensor
    outdata[2]=mode1;
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
void tanklevel(){
   
 if(indata1[0]>threshold1 ){
   t1=1;
   //threshold1=147;written in notify function
    
 }
if (indata1[0]<threshold1 ){
  t1=0;
   threshold1=152;
}
   
//=============================
   if(indata1[1]>threshold2){
     t2=1;
   //threshold2=110;written in notify function
    
  }
if (indata1[1]<threshold2){
   t2=0;
   threshold2=115;
  
}
  
}
//=========================================================
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

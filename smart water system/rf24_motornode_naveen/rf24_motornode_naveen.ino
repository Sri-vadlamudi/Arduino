 
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Servo.h>


//=============================================================================
RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

Servo myservotap; 
//========================================================================================
volatile int NbTopsFan; //measuring the rising edges of the signal
int flowrate;
int hallsensor = 2;    //The pin location of the sensor
int relay=3;
int pos=20;
int servopos=0;
int relaystatus=0;
float motorstatus=0.00;
float mode1=0.00;

int tappos = 0;    // variable to store the servo position
int tappos1 = 0;
int tapstatus = 0;


unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousservoMillis = 0; 
unsigned long previouswaterflowMillis = 0; 
//unsigned long previouselectrodeMillis = 0; 

const long interval = 1000;
const long waterflowinterval = 1000;
unsigned long tapmillisec1=0;
unsigned long tapmillisec2=0;
unsigned long tapmillisec3=0;
unsigned long tapmillisec4=0;

int f=0;
int t2=0;

 int i=0;
 int j=0;
 int k=0;
 int l=0;
 int m=0;

       
unsigned long millisec1=0;
unsigned long millisec2=0;
unsigned long millisec3=0;
unsigned long millisec4=0;


 
unsigned long radiosendinterval = 1050; //ms  // How often to send 'hello world to the other unit
unsigned long radiolastsent=0;  // When did we last send?
//=============================================================================
const uint16_t tank_node = 01;        // Address of our node in Octal format
const uint16_t master_node = 00;       // Address of the other node in Octal format
const uint16_t motor_node=02;

float outdata1[6];

float tank2=0.00;

int sampledata=0;

 
 //============================================================================


 //=============================================================================
void rpm ()     //This is the function that the interupt calls
{
    NbTopsFan++;  //This function measures the rising and falling edge of the
 
   // hall effect sensors signal
}
// ================================================================================================
void setup() //
{
    
   Serial.begin(9600); //This is the setup function where the serial port is
   pinMode(relay, OUTPUT);
   pinMode(hallsensor, INPUT_PULLUP); //initializes digital pin 2 as an input
  
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
   
  Serial.println("Transmitting...");
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 25, /*node address*/ motor_node);
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_LOW);
  myservotap.attach(6);
 
}

//===================================================================================================
void loop ()
{

  network.update();                  // Check the network regularly
 receivedata();
 senddata();
  
  flowsensor();
 flowratestatus();
tapstatusfunction();
tap();
motorrelay();

}
//===========================================================================================
void flowsensor(){
 unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    cli();      //Disable interrupts
    flowrate = (NbTopsFan * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate
 // in L/hour
 
    Serial.print (flowrate, DEC); //Prints the number calceulated above
    Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a  new line
   previousMillis = currentMillis;
   NbTopsFan = 0;   //Set NbTops to 0 ready for calceulations
    sei();      //Enables interrupts


  
 }
}

//=============================================================================================
void tap(){

unsigned long taptimer = millis();
  if(tapstatus==1){//opening tap
   
   tapmillisec2=taptimer-tapmillisec1;
   tapmillisec3=taptimer;
   
  }
if (tapstatus==0){
  tapmillisec1=taptimer;
 tapmillisec4=taptimer-tapmillisec3;
  
}
  tapmillisec2=taptimer-tapmillisec1;
  tapmillisec4=taptimer-tapmillisec3;
  //===========
   if (tapstatus==1&&tapmillisec2<2000){//opening valve
  myservotap.attach(6);
  myservotap.write(10);
 // Serial.println(valve4pos1);
  
 }

 if (tapmillisec2>2000){
  myservotap.detach();
 }
  
  
  
  if (tapstatus==0&&tapmillisec4<2000){//closing valve
    myservotap.attach(6);
    myservotap.write(172);
    // Serial.println(valve4pos1);
  }
  if (tapmillisec4>2000){
  myservotap.detach();
 }
  
    
tappos1=myservotap.read();

  }
//==================================================================================================
void motorrelay(){
  if (mode1==1.00){
if ((f==1||motorstatus==1.00) && t2==0){
    relaystatus=HIGH;
   
  }
  else {
  relaystatus=LOW;
    }
  }
  if (mode1==0.00){
    if (motorstatus==1.00){
      relaystatus=HIGH;
   
  }
  else {
  relaystatus=LOW;
    }
   }
  
   digitalWrite(relay, relaystatus);

}
//=================================================================================================
void senddata(){
     outdata1[0]=flowrate;
     outdata1[1]=f;
     outdata1[2]=relaystatus;
     outdata1[3]=sampledata;
     outdata1[4]=0;
     outdata1[5]=0;

         
     
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - radiolastsent >= radiosendinterval  )
  {
    radiolastsent = now;
     Serial.println(outdata1[0]);
     Serial.println(outdata1[1]);
     Serial.println(outdata1[2]);
     Serial.println(outdata1[3]);
   
   Serial.print("f:");
   Serial.println(f);
   Serial.print("t2:");
   Serial.println(t2);

   Serial.print("Sending...");
    

    RF24NetworkHeader header02(/*to node*/ master_node);
    bool ok = network.write(header02,&outdata1,sizeof(outdata1));
    if (ok){
      Serial.println("ok.");  
      sampledata++;
      if(sampledata>10) {
        sampledata=0;
      }
    }
    else{
      Serial.println("failed.");
    }
  }
  }
//==================================================================================
void receivedata(){
 float indata[3];
    while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    
    
    network.read(header,&indata,sizeof(indata));
     
      motorstatus=indata[0];
      t2=indata[1];
      mode1=indata[2];
     Serial.print("motorstatus:");
     Serial.println(motorstatus);
     Serial.print("t2:");
     Serial.println(t2);
     Serial.print("mode1:");
     Serial.println(indata[2]);
 }
}
//=============================================================================================
void flowratestatus(){
      
  unsigned long timer = millis();
 if(flowrate>100){
  
  millisec2=timer-millisec1;
  millisec3=timer;
 }
if (flowrate<100){
  millisec1=timer;
  millisec4=timer-millisec3;
  
}
   millisec2=timer-millisec1;
   millisec4=timer-millisec3;
   if (millisec2>2000){
    f=1;
   }
   if (millisec4>5000){
    f=0;
   }
}
//==========================================================================================
void tapstatusfunction(){
if(f==1){
      tapstatus=0;
    }

    if(f==0){
        
      tapstatus=1;
    }
}

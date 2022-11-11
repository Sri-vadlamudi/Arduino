#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Servo.h>
//==========================================================
RF24 radio(49, 53);//for arduino mega                 // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

Servo myservotap; 

//==============================================================


volatile int NbTopsFan; //measuring the rising edges of the signal
int flowrate;
int hallsensor = 2;    //The pin location of the sensor
int relay1=3;
int relay2=4;
int relay3=5;
int relay4=6;
int switch1=12;
int switch2=13;
int tap=7;
int valve1=8;
int valve2=9;
int valve3=10;
int valve4=11;

int tappos=A0;

char valvestatus=0;
char valvestatus1=0;
char currentvalvestatus=0;
char relaystatus=0;
bool switch1status=0;
bool switch2status=0;
bool v1=0;
bool v2=0;
bool v3=0;
bool v4=0;

char motorstatus=0;

bool tapstatus=0;
float tapposval=0;

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

unsigned long valvemillisec1=0;
unsigned long valvemillisec2=0;
unsigned long valvemillisec3=0;
unsigned long valvemillisec4=0;

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
 
unsigned long radiosendinterval = 1050; 
unsigned long radiolastsent=0;  
unsigned long relaylast=0;  
//=============================================================================
const uint16_t tank_node = 01;        // Address of our node in Octal format
const uint16_t master_node = 00;       // Address of the other node in Octal format
const uint16_t motor_node=02;

unsigned char outdata1[5];

float tank2=0.00;

bool sampledata=0;

//=============================================================================
void rpm ()     //This is the function that the interupt calls
{
    NbTopsFan++;  //This function measures the rising and falling edge of the
 
   // hall effect sensors signal
}
void setup() {
 Serial.begin(9600); //This is the setup function where the serial port is
  pinMode(switch1, INPUT_PULLUP);
   pinMode(switch2, INPUT_PULLUP);
   pinMode(relay1, OUTPUT);
   pinMode(relay2, OUTPUT);
   pinMode(relay3, OUTPUT);
   pinMode(relay4, OUTPUT);
  
   pinMode(hallsensor, INPUT_PULLUP); //initializes digital pin 2 as an input
  
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
   
  Serial.println("Transmitting...");
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 25, /*node address*/ motor_node);
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_LOW);
//  myservotap.attach(6);
//  myservotap.write(172);
  
 

}

void loop() {
  
  network.update();                  // Check the network regularly
 receivedata();
 senddata();
  valvemechanism();
  relaymechanism();
  flowsensor();
  flowratestatus();
  tapposfunc();
  tapfunc();
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
void tapfunc(){

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
  
  myservotap.attach(7);
  myservotap.write(180);
 // Serial.println(valve4pos1);
  
 }

 if (tapmillisec2>2000){
  myservotap.detach();
  
 }
  
  
  
  if (tapstatus==0&&tapmillisec4<2000){//closing valve
   
    myservotap.attach(7);
    myservotap.write(90);
    // Serial.println(valve4pos1);
  }
  if (tapmillisec4>2000){
  myservotap.detach();
  
 }
  

  }
//=================================================================================================
void senddata(){
     outdata1[0]=char(sampledata);
     outdata1[1]=char(flowrate/12);
     outdata1[2]=char(f);
     outdata1[3]=char(valvestatus1);
     outdata1[4]=char(tapposval);
             
     
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - radiolastsent >= radiosendinterval  )
  {
    radiolastsent = now;
     Serial.println(outdata1[0]);
     Serial.println(outdata1[1]);
     Serial.println(outdata1[2]);
     Serial.println(outdata1[3]);
  

   Serial.print("Sending...");
    

    RF24NetworkHeader header02(/*to node*/ master_node);
    bool ok = network.write(header02,&outdata1,sizeof(outdata1));
    if (ok){
      Serial.println("ok.");  
     
      if(sampledata==0) {
        sampledata=1;
      }
      else{
        sampledata=0;
      }
    }
    else{
      Serial.println("failed.");
      sampledata=0;
    }
  }
  }
//==================================================================================
void receivedata(){
  unsigned char indata[1];
    while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    
    
    network.read(header,&indata,sizeof(indata));
     
      valvestatus=indata[0];
     
     
     Serial.print("valvetatus:");
     Serial.println(indata[0]);

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
    tapstatus=0;
   }
   if (millisec4>5000){
    f=0;
    tapstatus=1;
   }
}
//==========================================================================================
void tapposfunc(){
  int tapposvaltemp;
  tapposvaltemp=analogRead(tappos);
  delay(10);
  tapposvaltemp=analogRead(tappos);
  tapposval=tapposvaltemp/5;
  
}
//===========================================================================================
void valvemechanism(){
  switch1status=digitalRead(switch1);
  switch2status=digitalRead(switch2);
// switch1status=HIGH;
//  switch2status=HIGH;

  if(valvestatus==0&& switch1status==HIGH && switch2status==HIGH){
    valvestatus1=0;
  }
  else if(valvestatus==1 && switch1status==LOW && switch2status==HIGH){
    valvestatus1=0;
   }
   else if(valvestatus==1 && switch1status==HIGH && switch2status==LOW){
    valvestatus1=0;
   }
    else if(valvestatus==2 && switch1status==LOW && switch2status==HIGH){
    valvestatus1=0;
   }
   else if(valvestatus==2 && switch1status==HIGH && switch2status==LOW){
    valvestatus1=0;
   }
   else if(valvestatus==3 && switch1status==LOW && switch2status==HIGH){
    valvestatus1=0;
   }
   else if(valvestatus==3 && switch1status==HIGH && switch2status==LOW){
    valvestatus1=0;
   }
   else if(valvestatus==4 && switch1status==LOW && switch2status==HIGH){
    valvestatus1=0;
   }
   else if(valvestatus==4 && switch1status==HIGH && switch2status==LOW){
    valvestatus1=0;
   }
   else if(valvestatus==5 && switch1status==LOW && switch2status==HIGH){
    valvestatus1=0;
   }
   else if(valvestatus==5 && switch1status==HIGH && switch2status==LOW){
    valvestatus1=0;
   }
   else if(valvestatus==6 && switch1status==LOW && switch2status==HIGH){
    valvestatus1=0;
   }
   else if(valvestatus==6 && switch1status==HIGH && switch2status==LOW){
    valvestatus1=0;
   }
   else if(valvestatus==1 && switch1status==HIGH && switch2status==HIGH){
    valvestatus1=1;
   }
   else if(valvestatus==2 && switch1status==HIGH && switch2status==HIGH){
    valvestatus1=2;
   }
    else if(valvestatus==3 && switch1status==HIGH && switch2status==HIGH){
    valvestatus1=3;
   }
   else if(valvestatus==4 && switch1status==HIGH && switch2status==HIGH){
    valvestatus1=4;
   }
    else if(valvestatus==5 && switch1status==HIGH && switch2status==HIGH){
    valvestatus1=5;
   }
   else if(valvestatus==6 && switch1status==HIGH && switch2status==HIGH){
    valvestatus1=6;
   }
    else if(valvestatus==0 && switch1status==LOW && switch2status==HIGH){
    valvestatus1=7;
   }
   else if(valvestatus==0 && switch1status==HIGH && switch2status==LOW){
    valvestatus1=8;
   }

   unsigned long valvetimer = millis();
  if(currentvalvestatus!=valvestatus1){
   
   valvemillisec2=valvetimer-valvemillisec1;
   valvemillisec3=valvetimer;
   relaystatus=0;
   
  }
if (currentvalvestatus==valvestatus1){
  valvemillisec1=valvetimer;
 valvemillisec4=valvetimer-valvemillisec3;
  
}
  valvemillisec2=valvetimer-valvemillisec1;
  valvemillisec4=valvetimer-valvemillisec3;
  //========================================================
   if (currentvalvestatus!=valvestatus1&&valvemillisec2>2000){       
       currentvalvestatus=valvestatus1;
         }
   
if(currentvalvestatus==valvestatus1){
   
    if(valvestatus1==0){
      relaystatus=0;
    }
    else if(valvestatus1==1){
      relaystatus=1;
    }
    else if(valvestatus1==2){
      relaystatus=2;
    }
    else if(valvestatus1==3){
      relaystatus=3;
    }
    else if(valvestatus1==4){
      relaystatus=3;
    }
    else if(valvestatus1==5){
      relaystatus=4;
    }
    else if(valvestatus==6){
      relaystatus=4;
    }
    else if(valvestatus1==7){
      relaystatus=5;
    }
     else if(valvestatus1==8){
      relaystatus=6;
    }
  }
  
}
  
 //==================================================================================================
 void relaymechanism(){
if(relaystatus==0){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
}
else if(relaystatus==1){
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
}
else if(relaystatus==2){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
}
else if(relaystatus==3){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, LOW);
}
else if(relaystatus==4){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
}
else if(relaystatus==5){
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, HIGH);
}
else if(relaystatus==6){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, HIGH);
} 
 
}
//===========================================================================================
  


#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <VL53L0X.h>

//VL53L0X sensor;
//=============================================================================
//#define LONG_RANGE
//=============================================================================
RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio
//=============================================================================
const uint16_t tank_node = 01;        // Address of our node in Octal format
const uint16_t master_node = 00;       // Address of the other node in Octal format
const uint16_t motor_node=02;

float outdata1[3];
int indata;
float outdata2=0.00;
//================================================================================
//int greenled=;

int trigpin1 = 2;
//int trigpin2=4;
int echopin1= 3;
//int echopin2 = 6;
//======================================================================================
float distancetemp1 = 0.00;
//float distancetemp2 = 0.00;
float duration1 = 0.00;
//float duration2 = 0.00;
float distance1 = 0.00;
//float distance2= 0.00;
int lidarval=0.0;


float tank1=0.00;
float tank2=0.00;

int trigstate1 = LOW;
int trigstate2= LOW;
int greenledstatus=LOW;
//==================================================================================
unsigned long ultratriginterval=10;
unsigned long previousMicros =0;;
unsigned long previousMicros1 =0;;
unsigned long ultrasent=100;
unsigned long ultrainterval=10;

unsigned long radiosendinterval = 1000; //ms  // How often to send 'hello world to the other unit
unsigned long radiolastsent=0;  // When did we last send?

//=============================================================================
const int numReadings = 20;
float readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0.00;                  // the running total
float tank2average = 0.00;   


//============================================================================
void setup(void)
{
  Serial.begin(9600);
  Serial.println("Transmitting...");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 25, /*node address*/ tank_node);
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel(RF24_PA_LOW);
  pinMode(trigpin1,OUTPUT); 
  pinMode(echopin1,INPUT);
 // pinMode(trigpin2,OUTPUT); 
 // pinMode(echopin2,INPUT);
//  pinMode(greenled,OUTPUT);
  //digitalWrite(greenled, greenledstatus);
  //===================================================================


//============================================================
for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
}
//=======================================================================
void loop(void){
  ultrasonic();
 // lidar();
  network.update();                  // Check the network regularly
  senddata(); 
  }
//==========================================================================
  void ultrasonic(){
    
     unsigned long ultranow = millis();              // If it's time to send a message, send it!
  if ( ultranow - ultrasent >= ultrainterval  )
  {
    ultrasent = ultranow;
      unsigned long currentMicros = micros(); //time in milliseconds from which the code was started
  if (currentMicros-previousMicros >= ultratriginterval) { //check "blink without delay" code
    previousMicros = currentMicros;
    if (trigstate1 == LOW){
      (trigstate1 = HIGH);
    }
    else {
      (trigstate1 = LOW);
    }
  }
  digitalWrite(trigpin1,trigstate1);  
  duration1 = pulseIn(echopin1,HIGH);
  distancetemp1 = (duration1*.0343)/2;
 
 if (distancetemp1>0){
    distance1=distancetemp1;
  }
  else {
    distance1=distance1;
  }
  tank1=(186.0-distance1);
    }
 }
 //============================================================================================
  


//==================================================================================
 void senddata(){
     
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - radiolastsent >= radiosendinterval  )
  {
    radiolastsent = now;
    outdata1[0]=tank1;
     outdata1[1]=tank2average;
     outdata1[2]=greenledstatus;

     Serial.println(outdata1[0]);
     Serial.println(outdata1[1]);
     Serial.println(outdata1[2]);
    

    Serial.print("Sending...");

    RF24NetworkHeader header(/*to node*/ master_node);
    bool ok = network.write(header,&outdata1,sizeof(outdata1));
     if (ok){
      Serial.println("ok.");
      if (greenledstatus==LOW){
        greenledstatus=HIGH;
      }
      else{
        greenledstatus=LOW;
      }        
    }
    else{
      Serial.println("failed.");
      greenledstatus=LOW;
    }
    //digitalWrite(greenled, greenledstatus);
  }
// if ( now - radiolastsent >= radiosendinterval +50)
//  { 
//    outdata2=outdata1[1];
//  Serial.println(outdata2);
//     radiolastsent = now;
//   RF24NetworkHeader header1(/*to node*/ motor_node);
//   bool ok1 = network.write(header1,&outdata2,sizeof(outdata2));
//   
//
//  if (ok1){
//      Serial.println("ok.");
//            }
//
//   else{
//      Serial.println("failed.");
//     }
// }
}
  //==================================================================================

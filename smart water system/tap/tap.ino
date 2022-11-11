#include <Servo.h>
//================================
Servo myservotap; 
//====================================================
int tappos = 0;    // variable to store the servo position
int tappos1 = 0;
int tapstatus = 0;

unsigned long tapmillisec1=0;
unsigned long tapmillisec2=0;
unsigned long tapmillisec3=0;
unsigned long tapmillisec4=0;

//===============================
void setup() {
    
   Serial.begin(9600);

}

void loop() {
  byte serialdata;
  if (Serial.available() > 0){
    serialdata=Serial.read();
    //Serial.println(serialdata);
  }

  if (serialdata>10){
   serialdata=serialdata;
  }
  else{
    serialdata=0;
  }
  if(serialdata==50){
     Serial.println(serialdata);
    tapstatus=0;
  }
  else if(serialdata==51){
     Serial.println(serialdata);
    tapstatus=1;
  }
  
  tap();

}
//======================================================================
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
   if (tapstatus==1&&tapmillisec2<10000){//opening valve
    //digitalWrite(servopower, HIGH);
  myservotap.attach(6);
  myservotap.write(10);
 // Serial.println(valve4pos1);
  
 }

 if (tapmillisec2>10000){
  myservotap.detach();
 // digitalWrite(servopower, LOW);
 }
  
  
  
  if (tapstatus==0&&tapmillisec4<10000){//closing valve
   // digitalWrite(servopower, HIGH);
    myservotap.attach(6);
    myservotap.write(172);
    // Serial.println(valve4pos1);
  }
  if (tapmillisec4>10000){
  myservotap.detach();
 // digitalWrite(servopower, LOW);
 }
  
    
tappos1=myservotap.read();



  }
  //======================================

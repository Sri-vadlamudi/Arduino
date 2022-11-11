#include <Servo.h>
//================================
Servo myservovalve1; 
Servo myservovalve2; 
Servo myservovalve3; 
Servo myservovalve4; 
//====================================================
//int valve1pos = 0;    // variable to store the servo position
//int valve1pos1 = 0;
int valvestatus=0;
int valvestatus1=0;
int valve1status = 0;
int valve2status = 0;
int valve3status = 0;
int valve4status = 0;
int switch1=7;
int switch2=8;
int relay1=3;
int relay2=4;
int relay3=5;
int relay4=6;
int relaystatus=0;

bool switch1status=0;
bool switch2status=0;
bool v1=0;
bool v2=0;
bool v3=0;
bool v4=0;

unsigned long valve1millisec1=0;
unsigned long valve1millisec2=0;
unsigned long valve1millisec3=0;
unsigned long valve1millisec4=0;

unsigned long valve2millisec1=0;
unsigned long valve2millisec2=0;
unsigned long valve2millisec3=0;
unsigned long valve2millisec4=0;

unsigned long valve3millisec1=0;
unsigned long valve3millisec2=0;
unsigned long valve3millisec3=0;
unsigned long valve3millisec4=0;

unsigned long valve4millisec1=0;
unsigned long valve4millisec2=0;
unsigned long valve4millisec3=0;
unsigned long valve4millisec4=0;

//===============================
void setup() {
    
   Serial.begin(9600);
   pinMode(switch1, INPUT_PULLUP);
   pinMode(switch2, INPUT_PULLUP);
   pinMode(relay1, OUTPUT);
   pinMode(relay2, OUTPUT);
   pinMode(relay3, OUTPUT);
   pinMode(relay4, OUTPUT);

}

void loop() {
  serialfunc();  
  valve1();
  valve2();
  valve3();
  valve4();
  valvemechanism();
  relaymechanism();

}
//===================================================================================================
void serialfunc(){
  byte serialdata;
  if (Serial.available() > 0){
    serialdata=Serial.read();
    //Serial.println(serialdata);
  }
   valvestatus=serialdata;
  Serial.write(valvestatus1);
  //Serial.println(valvestatus1);

}
//======================================================================
void valve1(){

unsigned long valve1timer = millis();
  if(valve1status==1){//opening valve1
   
   valve1millisec2=valve1timer-valve1millisec1;
   valve1millisec3=valve1timer;
   
  }
if (valve1status==0){
  valve1millisec1=valve1timer;
 valve1millisec4=valve1timer-valve1millisec3;
  
}
  valve1millisec2=valve1timer-valve1millisec1;
  valve1millisec4=valve1timer-valve1millisec3;
  //========================================================
   if (valve1status==1&&valve1millisec2<10000){//opening valve
  myservovalve1.attach(A1);
  myservovalve1.write(49);
  
 }

 if (valve1millisec2>10000){
  myservovalve1.detach();
  v1=1;
 }
  
  
  
  if (valve1status==0&&valve1millisec4<10000){//closing valve
  
    myservovalve1.attach(A1);
    myservovalve1.write(1);
    
  }
  if (valve1millisec4>10000){
  myservovalve1.detach();
  v1=0;
 
 }
  
    
//valve1pos1=myservovalve1.read();

  }
//=======================================================================
void valve2(){

unsigned long valve2timer = millis();
  if(valve2status==1){//opening valve2
   
   valve2millisec2=valve2timer-valve2millisec1;
   valve2millisec3=valve2timer;
   
  }
if (valve2status==0){
  valve2millisec1=valve2timer;
 valve2millisec4=valve2timer-valve2millisec3;
  
}
  valve2millisec2=valve2timer-valve2millisec1;
  valve2millisec4=valve2timer-valve2millisec3;
  //=============================
   if (valve2status==1&&valve2millisec2<10000){//opening valve
  myservovalve2.attach(A2);
  myservovalve2.write(52);
  
 }

 if (valve2millisec2>10000){
  myservovalve2.detach();
  v2=1;
 }
  
  
  
  if (valve2status==0&&valve2millisec4<10000){//closing valve
  
    myservovalve2.attach(A2);
    myservovalve2.write(0);
    
  }
  if (valve2millisec4>10000){
  myservovalve2.detach();
  v2=0;
 
 }
  
    
//valve2pos1=myservovalve2.read();

  }
  //=========================================================================================
  void valve3(){

unsigned long valve3timer = millis();
  if(valve3status==1){//opening valve3
   
   valve3millisec2=valve3timer-valve3millisec1;
   valve3millisec3=valve3timer;
   
  }
if (valve3status==0){
  valve3millisec1=valve3timer;
 valve3millisec4=valve3timer-valve3millisec3;
  
}
  valve3millisec2=valve3timer-valve3millisec1;
  valve3millisec4=valve3timer-valve3millisec3;
  //=====================
   if (valve3status==1&&valve3millisec2<10000){//opening valve
  myservovalve3.attach(A3);
  myservovalve3.write(158);
  
 }

 if (valve3millisec2>10000){
  myservovalve3.detach();
  v3=1;
 }
  
  
  
  if (valve3status==0&&valve3millisec4<10000){//closing valve
  
    myservovalve3.attach(A3);
    myservovalve3.write(0);
    
  }
  if (valve3millisec4>10000){
  myservovalve3.detach();
  v3=0;
 
 }
  
    
//valve3pos1=myservovalve3.read();

  }
  //=====================================================================================================
    void valve4(){

unsigned long valve4timer = millis();
  if(valve4status==1){//opening valve4
   
   valve4millisec2=valve4timer-valve4millisec1;
   valve4millisec3=valve4timer;
   
  }
if (valve4status==0){
  valve4millisec1=valve4timer;
 valve4millisec4=valve4timer-valve4millisec3;
  
}
  valve4millisec2=valve4timer-valve4millisec1;
  valve4millisec4=valve4timer-valve4millisec3;
  //=====================
   if (valve4status==1&&valve4millisec2<10000){//opening valve
  myservovalve4.attach(A4);
  myservovalve4.write(46);
  
 }

 if (valve4millisec2>10000){
  myservovalve4.detach();
  v4=1;
 }
  
  
  
  if (valve4status==0&&valve4millisec4<10000){//closing valve
  
    myservovalve4.attach(A4);
    myservovalve4.write(0);
    
  }
  if (valve4millisec4>10000){
  myservovalve4.detach();
  v4=0;
 
 }
  
    
//valve4pos1=myservovalve4.read();

  }
 //==================================================================================================
 void valvemechanism(){
  switch1status=digitalRead(switch1);
  switch2status=digitalRead(switch2);

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

   if(valvestatus1==0){
    valve4status=1;    
    valve1status=0;
    valve2status=0;
    valve3status=0;
     relaystatus=0;
      
   }
   else if(valvestatus1==1){
    valve1status=1;
    if(v1==1){
    valve2status=0;
    valve3status=0;
    valve4status=0;  
     if(valve1millisec2>10000 &&valve1millisec2<12000) {
      relaystatus=0;        
    }
    else if(valve1millisec2>12000) {
      relaystatus=1;        
     }   
     }    
        
   }
     else if(valvestatus1==2){
      valve2status=1;
      if(v2==1){
    valve1status=0;
    valve3status=0;
    valve4status=0; 
     if(valve2millisec2>10000 &&valve2millisec2<12000) {
      relaystatus=0;        
    }
    else if(valve2millisec2>12000) {
      relaystatus=2;        
     }       
      }
         
   }
    else if(valvestatus1==3){
    valve3status=1;
    if(v3==1){
    valve1status=0;
    valve2status=0;
    valve4status=0;
     if(valve3millisec2>10000 &&valve3millisec2<12000) {
      relaystatus=0;        
    }
    else if(valve3millisec2>12000) {
      relaystatus=3;        
     }      
    }
    
         
   }
    else if(valvestatus1==4){
      valve4status=1;  
      if(v4==1){
    valve1status=0;
    valve2status=0;
    valve3status=0;   
    if(valve4millisec2>10000 &&valve4millisec2<12000) {
      relaystatus=0;        
    }
    else if(valve4millisec2>12000) {
      relaystatus=3;        
    }
      }
        
   }
   else if(valvestatus1==5){
    valve3status=1;
    if(v3==1){
    valve1status=0;
    valve2status=0;
    valve4status=0;   
    if(valve3millisec2>10000 &&valve3millisec2<12000) {
      relaystatus=0;        
    }
    else if(valve3millisec2>12000) {
      relaystatus=4;        
       }
      }    
    }
  
       
   
   else if(valvestatus1==6){
     valve4status=1;    
     if(v4==1){
    valve1status=0;
    valve2status=0;
    valve3status=0;  
    if(valve4millisec2>10000 &&valve4millisec2<12000) {
      relaystatus=0;        
    }
    else if(valve4millisec2>12000) {
      relaystatus=4;        
    }    
     }
    
        
   }
   else if(valvestatus1==7){
    valve1status=1;
    if(v1==1){
    valve2status=0;
    valve3status=0;
    valve4status=0; 
     if(valve1millisec2>10000 &&valve1millisec2<12000) {
      relaystatus=0;        
    }
    else if(valve1millisec2>12000) {
      relaystatus=5;        
     }
    }
   
       
   }
    else if(valvestatus1==8){
      valve2status=1;
      if(v2==1){
        valve1status=0;
        valve3status=0;
        valve4status=0;      
         if(valve2millisec2>10000 &&valve2millisec2<12000) {
      relaystatus=0;        
    }
    else if(valve2millisec2>12000) {
      relaystatus=6;        
     }   
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

  

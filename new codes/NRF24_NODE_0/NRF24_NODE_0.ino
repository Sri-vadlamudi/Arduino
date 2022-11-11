

/////////////////////////////////////////////////
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
///////////////////////////////////////////////////
RF24 radio(9,10);               // nRF24L01 (CE,CSN) ,gnodemcu d4 and d8 pins)
RF24Network network(radio);      // Include the radio in the network
const uint16_t node00 = 00;   // Address of this wifi node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the tank node in Octal format
const uint16_t node02 = 02;       // Address of the motor node in Octal format
//////////////////////////////////////////////////
float outdata1[6];

//////////////////////////////////////////////////
const unsigned long interval = 1000;  //ms  // How often to send data to the other unit
unsigned long last_sent; 
////////////////////////////////////////////
void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, node00);  //(channel, node address)
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  Serial.begin(9600);
     bool result=radio.isChipConnected();
  Serial.print("result:");
  Serial.println(result);
  
 



}

void loop() {
  network.update();
 senddata();


  }
  

///////////////////////////////////////////
void senddata(){
  //===== Sending =====//
   unsigned long now = millis();
  if (now - last_sent >= interval) {   // If it's time to send a data, send it!
     last_sent = now;
     Serial.print("Sending...");
      outdata1[0]=0.0;
      outdata1[1]=1.0;
      outdata1[2]=2.0;
      outdata1[3]=3.0;
      outdata1[4]=4.0;
      outdata1[5]=5.0;

  RF24NetworkHeader header1(node01);     // (Address where the data is going)
  bool ok1 = network.write(header1, outdata1, sizeof(outdata1)); // Send the data
   if (ok1)
      Serial.println("ok.");
    else
      Serial.println("failed.");

 
  }
  delay(2000);
}
